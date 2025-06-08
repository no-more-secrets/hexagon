#include <ctype.h>
#include <fcntl.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BYTES_PER_LINE 16
#define MAX_FILE_SIZE  1048576 // 1 MB max

unsigned char buffer[MAX_FILE_SIZE];
size_t file_size  = 0;
size_t cursor_pos = 0;
char filename[256];

void load_file( const char *path ) {
  int fd = open( path, O_RDONLY );
  if( fd < 0 ) {
    perror( "open" );
    exit( 1 );
  }

  ssize_t bytes_read = read( fd, buffer, MAX_FILE_SIZE );
  if( bytes_read == -1 ) {
    perror( "read" );
    close( fd );
    exit( 1 );
  }
  file_size = bytes_read;

  close( fd );
  strncpy( filename, path, sizeof( filename ) );
}

void save_file() {
  int fd = open( filename, O_WRONLY | O_TRUNC );
  if( fd < 0 ) {
    perror( "save open" );
    return;
  }

  if( write( fd, buffer, file_size ) != (ssize_t)file_size ) {
    perror( "write" );
  }

  close( fd );
}

void draw_editor( WINDOW *win, size_t offset ) {
  werase( win );
  box( win, 0, 0 );

  for( int i = 0; i < getmaxy( win ) - 2; i++ ) {
    size_t line_offset = offset + i * BYTES_PER_LINE;
    if( line_offset >= file_size ) break;

    mvwprintw( win, i + 1, 1, "%08lx: ", line_offset );

    for( int j = 0; j < BYTES_PER_LINE; j++ ) {
      size_t idx = line_offset + j;
      if( idx < file_size )
        wprintw( win, "%02x ", buffer[idx] );
      else
        wprintw( win, "   " );
    }

    wprintw( win, " " );
    for( int j = 0; j < BYTES_PER_LINE; j++ ) {
      size_t idx = line_offset + j;
      if( idx < file_size ) {
        char c = buffer[idx];
        wprintw( win, "%c", isprint( c ) ? c : '.' );
      }
    }
  }

  // Move cursor
  size_t row = ( cursor_pos - offset ) / BYTES_PER_LINE;
  size_t col = ( cursor_pos % BYTES_PER_LINE ) * 3 + 10;
  if( cursor_pos < file_size ) wmove( win, row + 1, col );

  wrefresh( win );
}

// Function to show confirmation dialog, returns 1 if user
// confirms quit, 0 to cancel
int confirm_quit() {
  int width = 40, height = 7;
  int starty  = ( LINES - height ) / 2;
  int startx  = ( COLS - width ) / 2;
  WINDOW *dlg = newwin( height, width, starty, startx );
  box( dlg, 0, 0 );

  const char *msg1 = "Unsaved changes.";
  const char *msg2 = "Quit without saving?";
  mvwprintw( dlg, 1, ( width - (int)strlen( msg1 ) ) / 2, "%s",
             msg1 );
  mvwprintw( dlg, 2, ( width - (int)strlen( msg2 ) ) / 2, "%s",
             msg2 );

  // Buttons
  const char *btn_cancel = " Cancel ";
  const char *btn_quit   = " Quit ";
  int focus              = 0; // 0 = cancel, 1 = quit

  keypad( dlg, TRUE );
  curs_set( 0 );

  while( 1 ) {
    // Draw buttons
    if( focus == 0 ) {
      wattron( dlg, A_REVERSE );
      mvwprintw( dlg, height - 3,
                 width / 4 - (int)strlen( btn_cancel ) / 2, "%s",
                 btn_cancel );
      wattroff( dlg, A_REVERSE );
      mvwprintw( dlg, height - 3,
                 3 * width / 4 - (int)strlen( btn_quit ) / 2,
                 "%s", btn_quit );
    } else {
      mvwprintw( dlg, height - 3,
                 width / 4 - (int)strlen( btn_cancel ) / 2, "%s",
                 btn_cancel );
      wattron( dlg, A_REVERSE );
      mvwprintw( dlg, height - 3,
                 3 * width / 4 - (int)strlen( btn_quit ) / 2,
                 "%s", btn_quit );
      wattroff( dlg, A_REVERSE );
    }
    wrefresh( dlg );

    int c = wgetch( dlg );
    if( c == KEY_LEFT || c == '\t' ) {
      focus = 0;
    } else if( c == KEY_RIGHT ) {
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
  return focus == 1;
}

void run_editor() {
  initscr();
  noecho();
  cbreak();
  keypad( stdscr, TRUE );
  curs_set( 1 );

  int win_height = 24;
  int win_width  = 80;
  int starty     = ( LINES - win_height ) / 2;
  int startx     = ( COLS - win_width ) / 2;
  WINDOW *win = newwin( win_height, win_width, starty, startx );
  keypad( win, TRUE );     // enable keypad for main window
  intrflush( win, FALSE ); // prevent input flush on interrupts

  size_t offset = 0;

  int high_nibble = 1;
  int modified    = 0;

  while( 1 ) {
    draw_editor( win, offset );
    int ch = wgetch( win );

    if( ch == KEY_DOWN &&
        cursor_pos + BYTES_PER_LINE < file_size ) {
      cursor_pos += BYTES_PER_LINE;
      if( (ssize_t)( cursor_pos / BYTES_PER_LINE ) >= LINES - 2 )
        offset += BYTES_PER_LINE;
      high_nibble = 1; // Reset on cursor move
    } else if( ch == KEY_UP && cursor_pos >= BYTES_PER_LINE ) {
      cursor_pos -= BYTES_PER_LINE;
      if( ( cursor_pos / BYTES_PER_LINE ) <
          offset / BYTES_PER_LINE )
        offset -= BYTES_PER_LINE;
      high_nibble = 1;
    } else if( ch == KEY_LEFT && cursor_pos > 0 ) {
      cursor_pos--;
      high_nibble = 1;
    } else if( ch == KEY_RIGHT && cursor_pos + 1 < file_size ) {
      cursor_pos++;
      high_nibble = 1;
    } else if( isxdigit( ch ) ) {
      unsigned char val =
          isdigit( ch ) ? ch - '0' : tolower( ch ) - 'a' + 10;

      if( cursor_pos < file_size ) {
        if( high_nibble ) {
          buffer[cursor_pos] =
              ( val << 4 ) | ( buffer[cursor_pos] & 0x0F );
        } else {
          buffer[cursor_pos] =
              ( buffer[cursor_pos] & 0xF0 ) | val;
          cursor_pos++;
        }
        high_nibble = !high_nibble;
        modified    = 1;
      }
    } else if( ch == 's' ) {
      save_file();
      modified = 0; // reset modified flag on save
    } else if( ch == 'q' ) {
      if( modified ) {
        int confirm = confirm_quit();
        if( confirm ) break;
        // else continue editing
      } else {
        break;
      }
    }
  }

  delwin( win );
  endwin();
}

int main( int argc, char *argv[] ) {
  if( argc != 2 ) {
    fprintf( stderr, "Usage: %s <file>\n", argv[0] );
    return 1;
  }

  load_file( argv[1] );
  run_editor();
  return 0;
}