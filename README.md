# Invisible Text Converter

> Convert any text into invisible Unicode characters and back

![Version](https://img.shields.io/badge/version-1.0-blue)
![Python](https://img.shields.io/badge/Python-3.x-3776AB)
![C++](https://img.shields.io/badge/C++-17-00599C)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey)

**Developer:** Joaquim Pedro de Morais Filho
**Website:** [USAcomment.com](https://USAcomment.com)
**Email:** zicutake@mail.ru

---

## About

Invisible Text Converter is a tool that transforms any readable text into a sequence of **invisible Unicode characters** and back. The output text is completely invisible to the human eye, but contains the original message encoded within it using a custom encoding system based on zero-width and format Unicode codepoints.

This creates a new type of "invisible ASCII" — a way to hide messages in plain sight inside any text field, document, or message that supports Unicode.

---

## How It Works

### The Invisible Character Table

The program uses **33 real Unicode codepoints** that render as invisible or blank on screens:

| Index | Codepoint | Name | Description |
|-------|-----------|------|-------------|
| 0 | U+200B | Zero Width Space | Invisible space |
| 1 | U+200C | Zero Width Non-Joiner | Invisible non-joiner |
| 2 | U+200D | Zero Width Joiner | Invisible joiner |
| 3 | U+2060 | Word Joiner | Invisible word joiner |
| 4 | U+FEFF | Zero Width No-Break Space | BOM / invisible space |
| 5-8 | U+2061-U+2064 | Format characters | Math/format invisibles |
| 9 | U+180E | Mongolian Vowel Separator | Invisible separator |
| 10 | U+00AD | Soft Hyphen | Invisible hyphen |
| 11-16 | U+2065-U+2069 | Format characters | Additional invisibles |
| 17 | U+2028 | Line Separator | Invisible line break |
| 18 | U+2029 | Paragraph Separator | Invisible paragraph break |
| 19-26 | U+202A-U+202F | Directional formatting | Bidirectional invisibles |
| 27-29 | U+206A-U+206F | Arabic shaping | Form shaping invisibles |
| 30-32 | U+FFF9-U+FFFB | Annotation characters | Annotation invisibles |

### Encoding Algorithm

Each character is converted to its ASCII value and then encoded in **base-33** using the invisible character table as digits:

```
Input: "A" (ASCII 65)
65 in base-33 = 1 * 33 + 32
Digits: [1, 32]
Output: U+200C + U+FFFB (two invisible characters)
```

Each character is encoded into **1 to 3 invisible Unicode characters** depending on its ASCII value. The encoding is **reversible** — the decoder reads each invisible character, maps it back to its index value, and reconstructs the original text.

### Decoding Algorithm

```
Decoder reads invisible character stream:
  U+200C -> index 1
  U+FFFB -> index 32
  Reconstruct: 1 * 33 + 32 = 65 -> 'A'
```

---

## Features

| Feature | Description |
|---------|-------------|
| **Text to Invisible** | Convert any text into invisible Unicode characters |
| **Invisible to Text** | Decode invisible text back to readable form |
| **File Save (.inv)** | Save invisible text to files with .inv extension |
| **File Load (.inv)** | Load and decode .inv files |
| **Clipboard Copy** | Copy invisible text directly to clipboard (C++ version) |
| **Bidirectional** | Full support for encoding and decoding |

---

## Installation

### Pre-compiled (EXE)

Download `InvisibleText.exe` from the `dist/` folder and run it directly.

### From Source (Python)

```bash
git clone https://github.com/elevbit-ai/InvisibleText.git
cd InvisibleText
python invisible_text.py
```

### Compile the EXE yourself

```bash
pip install pyinstaller
pyinstaller --onefile --console --name InvisibleText invisible_text.py
```

### From Source (C++)

```bash
g++ -o InvisibleText.exe invisible_text.cpp -std=c++17
```

---

## Usage

```
==================================================
       INVISIBLE TEXT CONVERTER v1.0
==================================================
  Converts text to invisible Unicode chars
  Using a new type of ASCII-like invisible encoding
--------------------------------------------------
  Developer : Joaquim Pedro de Morais Filho
  Site      : USAcomment.com
  Email     : zicutake@mail.ru
==================================================

  [1] Convert text to invisible
  [2] Convert invisible text back to readable
  [3] Save invisible text to file (.inv)
  [4] Load and decode .inv file
  [5] Exit

  Choose an option:
```

### Example

```
Enter text: Hello World

INVISIBLE OUTPUT (copy everything between lines):
--------------------------------------------------
 (invisible characters rendered as blank)
--------------------------------------------------
Input chars: 11 | Invisible chars: 22
(The output above appears BLANK but contains hidden text!)
```

---

## File Format (.inv)

Files saved with the `.inv` extension use a simple format:

```
[Header: \x00INV1]
[Invisible Unicode content encoded in UTF-8]
```

The header identifies the file as an Invisible Text Converter file.

---

## Technical Details

- **Encoding:** Base-33 positional system using Unicode invisible codepoints
- **Character Set:** 33 Unicode codepoints (U+200B to U+FFFB)
- **Reversibility:** 100% lossless encoding/decoding
- **Compatibility:** Works with any text editor, chat, or field that supports Unicode
- **Languages:** Python 3.x, C++17

---

## License

This project is open source.

---

## Contact

**Joaquim Pedro de Morais Filho**
- Website: [USAcomment.com](https://USAcomment.com)
- Email: zicutake@mail.ru
- GitHub: [elevbit-ai](https://github.com/elevbit-ai)
