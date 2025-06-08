# Hexagon
Hex Editor for the Linux terminal, written in C.

## Features
* Adapts to terminal dimensions and centers the view.
* Allows in-place editing of bytes (insert not supported).
* Asks to save before exiting if changes are made.
* Supports both arrow keys and vim keys (h/j/k/l) for navigation.
* Status line with modified state and help.
* Classic style TUI using the ncurses library.

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
or for a quicker start, run the following to have it generate a sample binary file (`test.bin`) and open it in the editor:
```
$ make run
```
## Credits
This program was fully written by ChatGPT together with guidance, testing, and some minor bug fixes by myself.
