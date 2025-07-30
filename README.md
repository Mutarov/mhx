# Mutarov's Hex Editor (mhx)

A minimalist hex editor designed for command-line usage.

## Features

- **Allocate Memory**:
  - `ab <num>` — Allocate `<num>` bytes.
  - `adb <num>` — Allocate `<num>` double bytes (8*2).
  - `abs <num>` — Allocate `<num>` bits.
  
- **Preview File**:
  - `p` — Preview the entire file.
  - `p <loc>` — Preview content at a specific location `<loc>`.

- **Editing**:
  - `c <loc>` — Change current location to `<loc>`, enabling edits.
  - `e <bytes>` — Edit the content at the current location. Example: `e 4b 9d 12` changes the byte values at the current location.

- **Exit**:
  - `q` — Quit the editor. It will ask if you want to save the changes before quitting.
  
## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/Mutarov/mhx.git
   ```
2. Navigate to the project directory:
   ```bash
   cd mhx
   ```
3. Compile the program using ```make```:
   ```bash
   make
   ```
4. Optionally, install the program to your system:
   ```bash
   sudo make install
   ```
## Usage
To run the hex editor, simply execute the compiled binary:
```bash
mhx <file>
```
## License
This project is licensed under the GPL v3 - see [LICENSE](LICENSE) file for details
## Author
By Murat N.S. 2024
## Contribute
I welcome contributions! if you'd like to contribute to Murat's Hex Editor (mhx), please follow these steps:
1. Fork the repository: Click the "Fork" button on the top-right corner of this page.
2. Clone your fork:
   ```bash
   git clone https://github.com/Mutarov/mhx.git
   ```
3. Create a new branch for your feature or bugfix:
   ```bash
   git checkout -b <feature-name>
   ```
4. Make your changes and commit them:
   ```bash
   git commit -m "Description of changes"
   ```
5. Push your changes to your fork:
   ```bash
   git push origin <feature-name>
   ```
6. Create a pull request from your fork to the main repository. This is where you'll propose your changes to the original project.
