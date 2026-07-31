#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

// Unicode invisible character blocks used for encoding
// These are real Unicode codepoints that render as invisible/blank
const string INVISIBLE_CHARS[] = {
    "\u200B", // Zero Width Space
    "\u200C", // Zero Width Non-Joiner
    "\u200D", // Zero Width Joiner
    "\u2060", // Word Joiner
    "\uFEFF", // Zero Width No-Break Space
    "\u2061", // Function Application
    "\u2062", // Invisible Times
    "\u2063", // Invisible Separator
    "\u2064", // Invisible Plus
    "\u180E", // Mongolian Vowel Separator
    "\u00AD", // Soft Hyphen
    "\u2065", // invisible
    "\u2066", // invisible
    "\u2067", // invisible
    "\u2068", // invisible
    "\u2069", // invisible
    "\u2028", // Line Separator (renders invisible in most contexts)
    "\u2029", // Paragraph Separator
    "\u202A", // Embedding
    "\u202B", // RLO
    "\u202C", // Pop Directional
    "\u202D", // LRO
    "\u202E", // RLO
    "\u202F", // Narrow No-Break Space
    "\u206A", // Inhibit Symmetric Swapping
    "\u206B", // Activate Symmetric Swapping
    "\u206C", // Inhibit Arabic Form Shaping
    "\u206D", // Activate Arabic Form Shaping
    "\u206E", // National Digit Shapes
    "\u206F", // Nominal Digit Shapes
    "\uFFF9", // Interlinear Annotation Anchor
    "\uFFFA", // Interlinear Annotation Separator
    "\uFFFB", // Interlinear Annotation Terminator
};

const int NUM_BASE_CHARS = 32; // 5 bits per character (2^5 = 32)

// Encode an integer into invisible characters (5-bit chunks)
string encodeNumber(int num) {
    string result;
    if (num == 0) {
        return INVISIBLE_CHARS[0];
    }
    while (num > 0) {
        int remainder = num % NUM_BASE_CHARS;
        result = INVISIBLE_CHARS[remainder] + result;
        num /= NUM_BASE_CHARS;
    }
    return result;
}

// Decode invisible characters back to an integer
int decodeNumber(const string& encoded) {
    int result = 0;
    for (size_t i = 0; i < encoded.length(); i++) {
        int val = -1;
        // Identify which invisible char this is by comparing bytes
        string charStr = encoded.substr(i, 1);
        // Try multi-byte sequences
        if (i < encoded.length()) {
            // Check UTF-8 encoded forms
            unsigned char c = (unsigned char)encoded[i];
            if (c == 0xE2) { // U+200x range
                if (i + 2 < encoded.length()) {
                    unsigned char c2 = (unsigned char)encoded[i+1];
                    unsigned char c3 = (unsigned char)encoded[i+2];
                    int codepoint = ((c & 0x0F) << 12) | ((c2 & 0x3F) << 6) | (c3 & 0x3F);
                    if (codepoint >= 0x200B && codepoint <= 0x200F) val = codepoint - 0x200B;
                    else if (codepoint == 0x2060) val = 4;
                    else if (codepoint == 0xFEFF) val = 5;
                    else if (codepoint >= 0x2061 && codepoint <= 0x2064) val = codepoint - 0x2061 + 6;
                    else if (codepoint == 0x180E) val = 10;
                    else if (codepoint == 0x00AD) val = 11;
                    else if (codepoint >= 0x2065 && codepoint <= 0x206F) val = codepoint - 0x2065 + 12;
                    else if (codepoint == 0x2028) val = 23;
                    else if (codepoint == 0x2029) val = 24;
                    else if (codepoint >= 0x202A && codepoint <= 0x202F) val = codepoint - 0x202A + 25;
                    else if (codepoint >= 0xFFFA && codepoint <= 0xFFFB) val = codepoint - 0xFFFA + 28;
                    else val = 29; // fallback
                    i += 2;
                }
            }
            else if (c == 0xEF) { // U+FEFF (BOM/ZWNBSP)
                val = 5;
                i += 2;
            }
            else if (c == 0xE1) { // U+180E
                val = 10;
                i += 2;
            }
            else if (c == 0xC2) { // U+00AD
                if (i + 1 < encoded.length()) {
                    val = 11;
                    i += 1;
                }
            }
            else if (c == 0xE2 && i + 2 < encoded.length()) { // U+202x range
                unsigned char c2 = (unsigned char)encoded[i+1];
                unsigned char c3 = (unsigned char)encoded[i+2];
                int codepoint = ((c & 0x0F) << 12) | ((c2 & 0x3F) << 6) | (c3 & 0x3F);
                if (codepoint >= 0x2028 && codepoint <= 0x202F) val = codepoint - 0x2028 + 23;
                i += 2;
            }
            if (val == -1) val = 0; // fallback
        }
        result = result * NUM_BASE_CHARS + val;
    }
    return result;
}

void printMenu() {
    cout << "============================================" << endl;
    cout << "     INVISIBLE TEXT CONVERTER v1.0" << endl;
    cout << "============================================" << endl;
    cout << "  Converts text to invisible Unicode chars" << endl;
    cout << "  Developer: Joaquim Pedro de Morais Filho" << endl;
    cout << "  Site: USAcomment.com" << endl;
    cout << "  Email: zicutake@mail.ru" << endl;
    cout << "============================================" << endl;
    cout << endl;
    cout << "  [1] Convert text to invisible" << endl;
    cout << "  [2] Convert invisible text back" << endl;
    cout << "  [3] Encode text to file (.inv)" << endl;
    cout << "  [4] Decode file back to text" << endl;
    cout << "  [5] Copy invisible text to clipboard" << endl;
    cout << "  [6] Exit" << endl;
    cout << endl;
    cout << "  Choose: ";
}

string textToInvisible(const string& text) {
    string result;
    for (size_t i = 0; i < text.length(); i++) {
        unsigned char c = text[i];
        result += encodeNumber((int)c);
    }
    return result;
}

string invisibleToText(const string& invisible) {
    string result;
    size_t pos = 0;
    while (pos < invisible.length()) {
        // Find the next invisible character boundary
        size_t start = pos;
        // Each invisible char is 3 bytes in UTF-8 (for U+200x-U+206F range)
        // or 2 bytes (for U+00AD, U+180E) or 3 bytes (for U+FEFF, U+FFx)
        // We detect by looking at the leading byte
        if (pos >= invisible.length()) break;

        unsigned char lead = (unsigned char)invisible[pos];
        int charLen = 1;
        if ((lead & 0xE0) == 0xC0) charLen = 2;
        else if ((lead & 0xF0) == 0xE0) charLen = 3;
        else if ((lead & 0xF8) == 0xF0) charLen = 4;

        string oneChar = invisible.substr(pos, charLen);
        pos += charLen;

        // Decode this single invisible character
        int val = -1;
        unsigned char c = (unsigned char)oneChar[0];
        if (c == 0xE2 && oneChar.length() >= 3) {
            unsigned char c2 = (unsigned char)oneChar[1];
            unsigned char c3 = (unsigned char)oneChar[2];
            int codepoint = ((c & 0x0F) << 12) | ((c2 & 0x3F) << 6) | (c3 & 0x3F);
            if (codepoint >= 0x200B && codepoint <= 0x200F) val = codepoint - 0x200B;
            else if (codepoint == 0x2060) val = 4;
            else if (codepoint >= 0x2061 && codepoint <= 0x2064) val = codepoint - 0x2061 + 6;
            else if (codepoint == 0x2028) val = 23;
            else if (codepoint == 0x2029) val = 24;
            else if (codepoint >= 0x202A && codepoint <= 0x202F) val = codepoint - 0x202A + 25;
            else if (codepoint >= 0x2065 && codepoint <= 0x206F) val = codepoint - 0x2065 + 12;
        }
        else if (c == 0xEF && oneChar.length() >= 3) {
            unsigned char c2 = (unsigned char)oneChar[1];
            unsigned char c3 = (unsigned char)oneChar[2];
            int codepoint = ((c & 0x0F) << 12) | ((c2 & 0x3F) << 6) | (c3 & 0x3F);
            if (codepoint == 0xFEFF) val = 5;
            else if (codepoint >= 0xFFF9 && codepoint <= 0xFFFB) val = codepoint - 0xFFF9 + 28;
        }
        else if (c == 0xE1 && oneChar.length() >= 3) {
            unsigned char c2 = (unsigned char)oneChar[1];
            unsigned char c3 = (unsigned char)oneChar[2];
            int codepoint = ((c & 0x0F) << 12) | ((c2 & 0x3F) << 6) | (c3 & 0x3F);
            if (codepoint == 0x180E) val = 10;
        }
        else if (c == 0xC2 && oneChar.length() >= 2) {
            unsigned char c2 = (unsigned char)oneChar[1];
            int codepoint = ((c & 0x1F) << 6) | (c2 & 0x3F);
            if (codepoint == 0x00AD) val = 11;
        }

        if (val == -1) val = 0;
        result += (char)val;
    }
    return result;
}

int main() {
    int choice;
    string input, output;

    do {
        printMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: {
                cout << "\n  Enter text: ";
                getline(cin, input);
                output = textToInvisible(input);
                cout << "\n  --- INVISIBLE OUTPUT ---" << endl;
                cout << "  " << output << endl;
                cout << "  --- END ---" << endl;
                cout << "\n  (The text above appears blank but contains " << output.length() << " bytes)" << endl;
                cout << "  Characters: " << input.length() << " -> Invisible bytes: " << output.length() << endl;
                break;
            }
            case 2: {
                cout << "\n  Paste invisible text (press Enter twice to finish):" << endl;
                cout << "  > ";
                getline(cin, input);
                output = invisibleToText(input);
                cout << "\n  Decoded text: " << output << endl;
                break;
            }
            case 3: {
                cout << "\n  Enter text to encode: ";
                getline(cin, input);
                string filename;
                cout << "  Output filename (without extension): ";
                getline(cin, filename);
                filename += ".inv";

                output = textToInvisible(input);
                ofstream file(filename);
                if (file.is_open()) {
                    // Write header magic bytes
                    file << "\x00INV1";
                    file << output;
                    file.close();
                    cout << "\n  Saved to: " << filename << " (" << output.length() << " invisible bytes)" << endl;
                } else {
                    cout << "\n  Error: Could not create file!" << endl;
                }
                break;
            }
            case 4: {
                string filename;
                cout << "\n  Enter .inv filename to decode: ";
                getline(cin, filename);

                ifstream file(filename);
                if (file.is_open()) {
                    string content;
                    // Skip header (6 bytes: \x00 + "INV1" + 1 byte)
                    char header[6];
                    file.read(header, 6);
                    getline(file, content, '\0');
                    file.close();

                    output = invisibleToText(content);
                    cout << "\n  Decoded text: " << output << endl;
                } else {
                    cout << "\n  Error: Could not open file!" << endl;
                }
                break;
            }
            case 5: {
                cout << "\n  Enter text to convert: ";
                getline(cin, input);
                output = textToInvisible(input);
                string cmd = "echo|set /p=\"" + output + "\"|clip";
                system(cmd.c_str());
                cout << "\n  Invisible text copied to clipboard!" << endl;
                cout << "  You can paste it anywhere (Ctrl+V)" << endl;
                break;
            }
            case 6: {
                cout << "\n  Goodbye!" << endl;
                break;
            }
            default:
                cout << "\n  Invalid option!" << endl;
        }
        cout << endl;
    } while (choice != 6);

    return 0;
}
