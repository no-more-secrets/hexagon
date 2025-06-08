# Hexagon
Hex Editor for the Linux terminal, written in C.

## Screenshots
![hexagon](https://github.com/user-attachments/assets/1045f62f-6aa5-4bec-b394-a56c24541379)

## Features

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
Edits are done nibble-by-nibble (high nibble first, then low nibble),
offering precise byte manipulation.

- **ASCII Rendering**
Printable ASCII characters are shown alongside hex values, with
non-printables rendered as dots (.).

- **Scrolling Support**
Automatically scrolls the view when the cursor moves beyond the
visible region of the screen.

- **File Loading and Saving**
Loads binary files into memory and allows saving modified contents
back to disk with the s key.

- **Modification Tracking**
Tracks whether the file has been modified and displays the modified
status in the status line.

- **Quit Confirmation Dialog**
Warns about unsaved changes on quit (q) and prompts for confirmation
via a centered dialog box.

- **Ncurses TUI**
Built with ncurses for a full-screen terminal-based user interface
with box borders and key input handling.

- **Centered Interface Window**
Editor window is centered in the terminal, with dimensions adjusted
to file size and terminal size.

- **Status Bar**
Displays cursor position, modified status, and key hints at the
bottom of the editor window.

- **Resilient File Handling**
Handles errors such as failure to open, read, or write files with
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

## 🧰 Use Cases

Here are some examples of how you can use **Hexagon** or any hex editor:

### 🔍 Binary File Inspection
- View the raw bytes of executables, firmware, or binary blobs.
- Explore undocumented or proprietary file formats.

### 🐞 Debugging and Reverse Engineering
- Examine compiled machine code or memory dumps.
- Identify magic numbers, file headers, or embedded metadata.
- Reverse engineer file formats by observing changes byte-by-byte.

### ✍️ Manual File Patching
- Edit specific bytes to modify hardcoded data or settings.
- Example: Change a version number, toggle a flag, or fix corrupted bytes.

### 🎮 Game Save Editing
- Modify save files to adjust stats, inventory, or unlock features.
- Example: Find a known health value (`0x64` = 100 HP) and change it to `0xFF`.

### 🧪 Testing and Fuzzing
- Inject patterns or corrupted bytes to test file parsers and software robustness.
- Create edge cases or minimal binary test files.

### 🛠 File Repair
- Manually fix format errors or headers in corrupted files.
- Restore data when only a few bytes are missing or incorrect.

### 🔐 Security Auditing
- Search for embedded payloads, shellcode, or obfuscated data.
- Analyze metadata or content appended to common file types.

### 🗂 File Signature Detection
- Identify file types by inspecting the magic number at the beginning.
  - Example: `89 50 4E 47` = PNG, `25 50 44 46` = PDF.

---
## Credits
The entire contents of this repository were written by ChatGPT
with some guidance, testing, and minor bug fixes by myself. This
is a real program that really works... try it out!
