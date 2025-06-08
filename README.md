# Hexagon
Hex Editor for the Linux terminal, written in C.

## Features
* Adapts to terminal dimensions and centers the view.
* Allows in-place editing of bytes (insert not supported).
* Asks to save before exiting if changes are made.
* Supports both arrow keys and vim keys (h/j/k/l) for navigation.
* Status line with modified/save state and help.
* Classic style TUI using the ncurses library.

- **Hexadecimal View**
Displays file contents in a traditional hex dump format, showing
both hex bytes and their ASCII representations.

- **Editable Buffer**
Allows in-place editing of file contents directly in the hex view
using hex digit keys (0–9, A–F).

- **Cursor Navigation**
Navigate through the file using arrow keys or Vim-style keys (h,
j, k, l).

- **Nibble-wise Editing**
Edits are done nibble-by-nibble (high nibble first, then low nib-
ble), offering precise byte manipulation.

- **ASCII Rendering**
Printable ASCII characters are shown alongside hex values, with
non-printables rendered as dots (.).

- **Scrolling Support**
Automatically scrolls the view when the cursor moves beyond the
visible region of the screen.

- **File Loading and Saving**
Loads binary files into memory and allows saving modified con-
tents back to disk with the s key.

- **Modification Tracking**
Tracks whether the file has been modified and displays the modi-
fied status in the status line.

- **Quit Confirmation Dialog**
Warns about unsaved changes on quit (q) and prompts for confirma-
tion via a centered dialog box.

- **Ncurses TUI**
Built with ncurses for a full-screen terminal-based user inter-
face with box borders and key input handling.

- **Centered Interface Window**
Editor window is centered in the terminal, with dimensions ad-
justed to file size and terminal size.

- **Status Bar**
Displays cursor position, modified status, and key hints at the
bottom of the editor window.

- **Resilient File Handling**
Handles errors such as failure to open, read, or write files with
appropriate error messages.

These features make it a functional and user-friendly terminal
hex editor suitable for binary file inspection and in-place edit-
ing.

## Screenshots
![hexagon](https://github.com/user-attachments/assets/1045f62f-6aa5-4bec-b394-a56c24541379)

## How to Build
After cloning the repo, just run `make`:
```bash
$ make
```
Then to run:
```
$ ./hexagon <file>
```
Or for a quicker start, run the following to build, generate a test file (`test.bin`), and then open it in the editor:
```
$ make run
```

## How to Use

| **Key(s)**                    | **Action**                       | **Description**                                                                                                                                                                                                                      |
| ----------------------------- | -------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| **Arrow Up** or **k**         | Move cursor up                   | Moves the cursor one line upward (previous set of 16 bytes) if within bounds.                                                                                                                                                        |
| **Arrow Down** or **j**       | Move cursor down                 | Moves the cursor one line downward (next set of 16 bytes) if within bounds.                                                                                                                                                          |
| **Arrow Left** or **h**       | Move cursor left                 | Moves the cursor one byte to the left. In the quit confirmation dialog, it shifts the focus to the "Cancel" button.                                                                                                                  |
| **Arrow Right** or **l**      | Move cursor right                | Moves the cursor one byte to the right. In the quit confirmation dialog, it shifts the focus to the "Quit" button.                                                                                                                   |
| **Hex Digit Keys (0-9, A-F)** | Edit current byte                | When a hex digit is entered, it updates the current byte’s high nibble (if editing the high nibble) or low nibble (if editing the low nibble). After finishing the low nibble, the cursor automatically moves one byte to the right. |
| **s**                         | Save file                        | Saves the changes made to the file.                                                                                                                                                                                                  |
| **q**                         | Quit editor                      | Initiates the quit routine. If there are unsaved changes, a confirmation dialog is displayed.                                                                                                                                        |
| **Enter** (in dialog)         | Confirm selection in quit dialog | In the quit confirmation dialog, pressing Enter accepts the highlighted option (either to quit or cancel).                                                                                                                           |
| **ESC** (in dialog)           | Cancel quit dialog               | Cancels the quit confirmation dialog, keeping the editor active (focus remains on "Cancel").                                                                                                                                         |

## Credits
The entire contents of this repository were written by ChatGPT
with some guidance, testing, and minor bug fixes by myself. This
is a real program that really works... try it out!
