#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
INVISIBLE TEXT CONVERTER v1.0
Developer: Joaquim Pedro de Morais Filho
Site: USAcomment.com
Email: zicutake@mail.ru

Converts any text into invisible Unicode characters and back.
"""

import sys
import os

INVISIBLE_CHARS = [
    "\u200B",  # Zero Width Space
    "\u200C",  # Zero Width Non-Joiner
    "\u200D",  # Zero Width Joiner
    "\u2060",  # Word Joiner
    "\uFEFF",  # Zero Width No-Break Space
    "\u2061",  # Function Application
    "\u2062",  # Invisible Times
    "\u2063",  # Invisible Separator
    "\u2064",  # Invisible Plus
    "\u180E",  # Mongolian Vowel Separator
    "\u00AD",  # Soft Hyphen
    "\u2065",  # invisible
    "\u2066",  # invisible
    "\u2067",  # invisible
    "\u2068",  # invisible
    "\u2069",  # invisible
    "\u2028",  # Line Separator
    "\u2029",  # Paragraph Separator
    "\u202A",  # Left-to-Right Embedding
    "\u202B",  # Right-to-Left Embedding
    "\u202C",  # Pop Directional Formatting
    "\u202D",  # Left-to-Right Override
    "\u202E",  # Right-to-Left Override
    "\u202F",  # Narrow No-Break Space
    "\u206A",  # Inhibit Symmetric Swapping
    "\u206B",  # Activate Symmetric Swapping
    "\u206C",  # Inhibit Arabic Form Shaping
    "\u206D",  # Activate Arabic Form Shaping
    "\u206E",  # National Digit Shapes
    "\u206F",  # Nominal Digit Shapes
    "\uFFF9",  # Interlinear Annotation Anchor
    "\uFFFA",  # Interlinear Annotation Separator
    "\uFFFB",  # Interlinear Annotation Terminator
]

NUM_BASE = len(INVISIBLE_CHARS)


def text_to_invisible(text):
    result = []
    for ch in text:
        code = ord(ch)
        digits = []
        if code == 0:
            digits = [0]
        else:
            while code > 0:
                digits.append(code % NUM_BASE)
                code //= NUM_BASE
        digits.reverse()
        result.append("".join(INVISIBLE_CHARS[d] for d in digits))
    return "".join(result)


def invisible_to_text(invisible):
    result = []
    i = 0
    while i < len(invisible):
        cp = ord(invisible[i])
        i += 1
        val = 0
        if cp == 0x200B:
            val = 0
        elif cp == 0x200C:
            val = 1
        elif cp == 0x200D:
            val = 2
        elif cp == 0x2060:
            val = 3
        elif cp == 0xFEFF:
            val = 4
        elif 0x2061 <= cp <= 0x2064:
            val = cp - 0x2061 + 5
        elif cp == 0x180E:
            val = 9
        elif cp == 0x00AD:
            val = 10
        elif 0x2065 <= cp <= 0x2069:
            val = cp - 0x2065 + 11
        elif cp == 0x2028:
            val = 17
        elif cp == 0x2029:
            val = 18
        elif 0x202A <= cp <= 0x202F:
            val = cp - 0x202A + 19
        elif 0x206A <= cp <= 0x206F:
            val = cp - 0x206A + 26
        elif 0xFFF9 <= cp <= 0xFFFB:
            val = cp - 0xFFF9 + 32
        else:
            val = 0
        result.append(chr(val))
    return "".join(result)


def print_banner():
    print()
    print("=" * 50)
    print("       INVISIBLE TEXT CONVERTER v1.0")
    print("=" * 50)
    print("  Converts text to invisible Unicode chars")
    print("  Using a new type of ASCII-like invisible encoding")
    print("-" * 50)
    print("  Developer : Joaquim Pedro de Morais Filho")
    print("  Site      : USAcomment.com")
    print("  Email     : zicutake@mail.ru")
    print("=" * 50)
    print()


def print_menu():
    print("  [1] Convert text to invisible")
    print("  [2] Convert invisible text back to readable")
    print("  [3] Save invisible text to file (.inv)")
    print("  [4] Load and decode .inv file")
    print("  [5] Exit")
    print()


def main():
    print_banner()

    while True:
        print_menu()
        choice = input("  Choose an option: ").strip()

        if choice == "1":
            text = input("\n  Enter text to convert: ")
            invisible = text_to_invisible(text)
            print()
            print("  " + "-" * 46)
            print("  INVISIBLE OUTPUT (copy everything between lines):")
            print("  " + "-" * 46)
            print(f"  {invisible}")
            print("  " + "-" * 46)
            print(f"  Input chars: {len(text)} | Invisible bytes: {len(invisible.encode('utf-8'))}")
            print(f"  Invisible chars: {len(invisible)}")
            print("  (The output above appears BLANK but contains hidden text!)")
            print()

        elif choice == "2":
            print("\n  Paste the invisible text below, then press Enter:")
            invisible = input("  > ")
            try:
                text = invisible_to_text(invisible)
                print(f"\n  Decoded text: {text}")
            except Exception as e:
                print(f"\n  Error decoding: {e}")
            print()

        elif choice == "3":
            text = input("\n  Enter text to encode and save: ")
            filename = input("  Output filename (without extension): ").strip()
            if not filename:
                filename = "output"
            filename += ".inv"

            invisible = text_to_invisible(text)
            with open(filename, "w", encoding="utf-8") as f:
                f.write("\x00INV1")
                f.write(invisible)

            size = os.path.getsize(filename)
            print(f"\n  Saved to: {filename} ({size} bytes)")
            print(f"  Original text: {len(text)} chars -> Invisible: {len(invisible)} chars")
            print()

        elif choice == "4":
            filename = input("\n  Enter .inv filename to decode: ").strip()
            try:
                with open(filename, "r", encoding="utf-8") as f:
                    content = f.read()
                # Skip header
                invisible = content[5:]  # Skip \x00 + "INV1"
                text = invisible_to_text(invisible)
                print(f"\n  Decoded text: {text}")
            except FileNotFoundError:
                print(f"\n  Error: File '{filename}' not found!")
            except Exception as e:
                print(f"\n  Error: {e}")
            print()

        elif choice == "5":
            print("\n  Thank you for using Invisible Text Converter!")
            print("  Developer: Joaquim Pedro de Morais Filho")
            print("  Site: USAcomment.com")
            print("  Email: zicutake@mail.ru")
            print()
            break

        else:
            print("\n  Invalid option! Please choose 1-5.\n")


if __name__ == "__main__":
    main()
