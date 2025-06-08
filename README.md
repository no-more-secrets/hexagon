# Hexagon
Hex Editor for the Linux terminal, written in C.

## Screenshot
![hexagon](https://github.com/user-attachments/assets/1045f62f-6aa5-4bec-b394-a56c24541379)

## Features

* **Hexadecimal View** Displays file contents in a traditional hex dump format, showing
both hex bytes and their ASCII representations.
* **NCurses TUI** Built with ncurses for a full-screen terminal-based user interface
with box borders and key input handling. Editor window is centered in the terminal, with dimensions adjusted
to file size and terminal size.
* **Editable Buffer** Allows in-place editing of file contents directly in the hex view
using hex digit keys (0–9, A–F).
* **Cursor Navigation** Navigate through the file using arrow keys or Vim-style keys (h,
j, k, l).
* **Scrolling Support** Automatically scrolls the view when the cursor moves beyond the
visible region of the screen.
* **File Loading and Saving** Loads binary files into memory and allows saving modified contents
back to disk with the s key.
* **Modification Tracking** Tracks whether the file has been modified and displays the modified
status in the status line. Warns about unsaved changes on quit (q) and prompts for confirmation
via a centered dialog box.
* **Status Bar** Displays cursor position, modified status, and key hints at the
bottom of the editor window.
* **Resilient File Handling** Handles errors such as failure to open, read, or write files with
appropriate error messages.

These features make it a functional and user-friendly terminal
hex editor suitable for binary file inspection and in-place editing.

## How to Build
After cloning the repo, just run `make`:
```sh
$ make
```
Then to run:
```sh
$ ./hexagon <file>
```
Or for a quicker start, run the following to build, generate a
test file (`test.bin`), and then open it in the editor:
```sh
$ make run
```
## How to Use

Keyboard Input:

<div align="center">

| Key          | Action                               |
| ------------ | ------------------------------------ |
| `←` / `h`    | Move cursor left                     |
| `→` / `l`    | Move cursor right                    |
| `↑` / `k`    | Move cursor up                       |
| `↓` / `j`    | Move cursor down                     |
| `0–9`, `A–F` | Enter hex digit (nibble editing)     |
| `s`          | Save file                            |
| `q`          | Quit (with confirmation if modified) |
| `Enter`      | Confirm quit in dialog               |
| `Esc`        | Cancel quit in dialog                |

</div>

## 🧰 Use Cases

Here are some examples of how you can use **Hexagon** or any hex editor:

- 🔍 Binary File Inspection
- 🐞 Debugging and Reverse Engineering
- ✍️ Manual File Patching
- 🎮 Game Save Editing
- 🧪 Testing and Fuzzing
- 🛠 File Repair
- 🔐 Security Auditing
- 🗂 File Signature Detection

---
## Credits
The entire contents of this repository were written by ChatGPT
with some guidance, testing, and minor bug fixes by myself. This
is a real program that really works... try it out!
