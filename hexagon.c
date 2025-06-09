#include <ctype.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYTES_PER_LINE 16

#define HEADER_LINES   3
#define STATUS_LINES   1
#define NON_BODY_LINES ( HEADER_LINES + STATUS_LINES )

#define COLOR_BRIGHT_WHITE 255

unsigned char *buffer = NULL;
size_t file_size      = 0;

int cursor_pos = 0; // Current byte index in buffer
int offset     = 0; // Offset of first visible byte in buffer
int high_nibble =
    1; // Editing state: 1 = editing high nibble, 0 = low nibble
int modified = 0; // Buffer modified flag

void load_file( const char *filename ) {
  FILE *f = fopen( filename, "rb" );
  if( !f ) {
    perror( "Failed to open file" );
    exit( 1 );
  }
  fseek( f, 0, SEEK_END );
  file_size = ftell( f );
  fseek( f, 0, SEEK_SET );
  buffer = malloc( file_size );
  if( !buffer ) {
    perror( "Out of memory" );
    fclose( f );
    exit( 1 );
  }
  [[maybe_unused]] long x = fread( buffer, 1, file_size, f );
  fclose( f );
}

void save_file( const char *filename ) {
  FILE *f = fopen( filename, "wb" );
  if( !f ) {
    perror( "Failed to save file" );
    return;
  }
  fwrite( buffer, 1, file_size, f );
  fclose( f );
  modified = 0;
}

int confirm_quit( WINDOW *parent_win ) {
  int height = 7, width = 40;
  int starty = ( LINES - height ) / 2;
  int startx = ( COLS - width ) / 2;

  WINDOW *dlg = newwin( height, width, starty, startx );
  box( dlg, 0, 0 );
  keypad( dlg, TRUE );
  curs_set( 0 );

  const char *msg1 = "Unsaved changes.";
  const char *msg2 = "Quit without saving?";
  mvwprintw( dlg, 1, ( width - (int)strlen( msg1 ) ) / 2, "%s",
             msg1 );
  mvwprintw( dlg, 2, ( width - (int)strlen( msg2 ) ) / 2, "%s",
             msg2 );

  const char *btn_cancel = " Cancel ";
  const char *btn_quit   = " Quit ";
  int focus              = 0; // 0 = cancel, 1 = quit

  while( 1 ) {
    if( focus == 0 ) {
      wattron( dlg, A_REVERSE );
      mvwprintw( dlg, height - NON_BODY_LINES,
                 width / 4 - (int)strlen( btn_cancel ) / 2, "%s",
                 btn_cancel );
      wattroff( dlg, A_REVERSE );
      mvwprintw( dlg, height - NON_BODY_LINES,
                 3 * width / 4 - (int)strlen( btn_quit ) / 2,
                 "%s", btn_quit );
    } else {
      mvwprintw( dlg, height - NON_BODY_LINES,
                 width / 4 - (int)strlen( btn_cancel ) / 2, "%s",
                 btn_cancel );
      wattron( dlg, A_REVERSE );
      mvwprintw( dlg, height - NON_BODY_LINES,
                 3 * width / 4 - (int)strlen( btn_quit ) / 2,
                 "%s", btn_quit );
      wattroff( dlg, A_REVERSE );
    }
    wrefresh( dlg );

    int c = wgetch( dlg );
    if( c == '\t' ) {
      focus = !focus;
    } else if( c == KEY_LEFT || c == 'h' ) {
      focus = 0;
    } else if( c == KEY_RIGHT || c == 'l' ) {
      focus = 1;
    } else if( c == '\n' || c == KEY_ENTER ) {
      break;
    } else if( c == 27 ) { // ESC cancels
      focus = 0;
      break;
    }
  }

  delwin( dlg );
  curs_set( 1 );
  touchwin( parent_win );
  wrefresh( parent_win );

  return focus == 1;
}

void draw_editor( WINDOW *win ) {
  werase( win );
  box( win, 0, 0 );

  int win_height, win_width;
  getmaxyx( win, win_height, win_width );

  // Draw title bar
  mvwprintw( win, 0, ( win_width - 30 ) / 2,
             " Hexagon: ncurses Hex Editor " );

  // Draw column headers for hex and ascii
  mvwprintw( win, 1, 1, "Offset  " );
  for( int i = 0; i < BYTES_PER_LINE; i++ ) {
    mvwprintw( win, 1, 11 + i * 3, "%02X", i );
  }
  // mvwprintw( win, 1, 11 + BYTES_PER_LINE * 3 + 1, "ASCII" );

  // Draw horizontal separator with T junctions at the sides
  mvwaddch( win, 2, 0, ACS_LTEE );
  mvwhline( win, 2, 1, ACS_HLINE, win_width - 2 );
  mvwaddch( win, 2, win_width - 1, ACS_RTEE );

  // Draw visible bytes
  for( int line = 0; line < win_height - NON_BODY_LINES;
       line++ ) {
    int line_offset = offset + line * BYTES_PER_LINE;
    if( line_offset >= (ssize_t)file_size ) break;

    // Draw offset (address)
    wattron( win, COLOR_PAIR( 1 ) );
    mvwprintw( win, line + 3, 1, "%08X", line_offset );
    wattroff( win, COLOR_PAIR( 1 ) );
    mvwprintw( win, line + 3, 1 + 8, ":" );

    // Draw hex bytes
    for( int i = 0; i < BYTES_PER_LINE &&
                    ( line_offset + i ) < (ssize_t)file_size;
         i++ ) {
      unsigned char byte = buffer[line_offset + i];
      if( !byte ) wattron( win, COLOR_PAIR( 4 ) );
      if( byte == 0xff ) wattron( win, COLOR_PAIR( 2 ) );
      mvwprintw( win, line + 3, 11 + i * 3, "%02X", byte );
      if( !byte ) wattroff( win, COLOR_PAIR( 4 ) );
      if( byte == 0xff ) wattroff( win, COLOR_PAIR( 2 ) );
    }

    // Draw ASCII chars
    for( int i = 0; i < BYTES_PER_LINE &&
                    ( line_offset + i ) < (ssize_t)file_size;
         i++ ) {
      unsigned char c  = buffer[line_offset + i];
      int is_printable = ( c >= 32 && c < 127 );
      wattron( win, COLOR_PAIR( 5 ) );
      if( !is_printable ) wattron( win, COLOR_PAIR( 2 ) );
      mvwaddch( win, line + 3, 11 + BYTES_PER_LINE * 3 + 1 + i,
                is_printable ? c : '.' );
      if( !is_printable ) wattroff( win, COLOR_PAIR( 2 ) );
      wattroff( win, COLOR_PAIR( 5 ) );
    }
  }

  // Draw status line
  mvwprintw( win, win_height - 1, 1,
             "Cursor: %04X  Modified: %s  Use arrows to move, "
             "s=save, q=quit",
             cursor_pos, modified ? "YES" : "NO" );

  // Calculate cursor position inside window
  int cursor_line = ( cursor_pos - offset ) / BYTES_PER_LINE;
  int cursor_col  = ( cursor_pos - offset ) % BYTES_PER_LINE;
  int cursor_x = 11 + cursor_col * 3 + ( high_nibble ? 0 : 1 );
  int cursor_y = cursor_line + 3;

  if( cursor_y >= 2 && cursor_y < win_height - 1 )
    wmove( win, cursor_y, cursor_x );
  else
    wmove( win, win_height - 1, 1 ); // fallback position

  wrefresh( win );
}

void run_editor( const char *filename ) {
  load_file( filename );

  initscr();
  noecho();
  cbreak();
  keypad( stdscr, TRUE );
  curs_set( 1 );

  start_color();
  init_color( COLOR_WHITE, 800, 800, 800 );
  init_color( COLOR_GREEN, 700, 800, 400 );
  init_pair( 1, COLOR_MAGENTA, COLOR_BLACK );
  init_pair( 2, COLOR_WHITE, COLOR_BLACK );
  init_pair( 3, COLOR_CYAN, COLOR_BLACK );
  init_pair( 4, COLOR_BLUE, COLOR_BLACK );
  init_pair( 5, COLOR_GREEN, COLOR_BLACK );

  int base_lines =
      ( file_size + BYTES_PER_LINE - 1 ) / BYTES_PER_LINE;
  int min_height = base_lines + 4; // offset + headers + status
  int win_height = min_height < LINES ? min_height : LINES;
  // int win_width  = COLS < 100 ? 100 : COLS;
  int win_width = 77;
  int starty    = ( LINES - win_height ) / 2;
  int startx    = ( COLS - win_width ) / 2;
  WINDOW *win = newwin( win_height, win_width, starty, startx );
  box( win, 0, 0 );
  keypad( win, TRUE );

  while( 1 ) {
    draw_editor( win );

    int ch = wgetch( win );

    if( ch == KEY_DOWN || ch == 'j' ) {
      if( cursor_pos + BYTES_PER_LINE < (ssize_t)file_size ) {
        cursor_pos += BYTES_PER_LINE;
        if( cursor_pos >=
            offset + ( win_height - NON_BODY_LINES ) *
                         BYTES_PER_LINE ) {
          offset += BYTES_PER_LINE;
        }
        high_nibble = 1;
      }
    } else if( ch == KEY_UP || ch == 'k' ) {
      if( cursor_pos >= BYTES_PER_LINE ) {
        cursor_pos -= BYTES_PER_LINE;
        if( cursor_pos < offset ) {
          offset -= BYTES_PER_LINE;
          if( offset < 0 ) offset = 0;
        }
        high_nibble = 1;
      }
    } else if( ch == KEY_LEFT || ch == 'h' ) {
      if( cursor_pos > 0 ) {
        cursor_pos--;
        if( cursor_pos < offset ) {
          offset -= BYTES_PER_LINE;
          if( offset < 0 ) offset = 0;
        }
        high_nibble = 1;
      }
    } else if( ch == KEY_RIGHT || ch == 'l' ) {
      if( cursor_pos + 1 < (ssize_t)file_size ) {
        cursor_pos++;
        if( cursor_pos >=
            offset + ( win_height - NON_BODY_LINES ) *
                         BYTES_PER_LINE ) {
          offset += BYTES_PER_LINE;
        }
        high_nibble = 1;
      }
    } else if( isxdigit( ch ) ) {
      unsigned char val =
          isdigit( ch ) ? ch - '0' : tolower( ch ) - 'a' + 10;
      if( cursor_pos < (ssize_t)file_size ) {
        if( high_nibble ) {
          buffer[cursor_pos] =
              ( val << 4 ) | ( buffer[cursor_pos] & 0x0F );
          high_nibble = 0;
        } else {
          buffer[cursor_pos] =
              ( buffer[cursor_pos] & 0xF0 ) | val;
          cursor_pos++;
          high_nibble = 1;
          // Scroll if cursor moves out of view
          if( cursor_pos >=
              offset + ( win_height - NON_BODY_LINES ) *
                           BYTES_PER_LINE ) {
            offset += BYTES_PER_LINE;
          }
        }
        modified = 1;
      }
    } else if( ch == 's' ) {
      save_file( filename );
    } else if( ch == 'q' ) {
      if( modified ) {
        int quit = confirm_quit( win );
        if( quit ) break;
        // else continue editing
      } else {
        break;
      }
    }
  }

  delwin( win );
  endwin();

  free( buffer );
}

int main( int argc, char **argv ) {
  if( argc < 2 ) {
    fprintf( stderr, "Usage: %s filename\n", argv[0] );
    return 1;
  }

  run_editor( argv[1] );

  return 0;
}