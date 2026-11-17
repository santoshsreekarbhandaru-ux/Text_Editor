// The code is moved from main.cpp (readLine, loadFile, saveFile) into the Editor class.
#include "Editor.h"
#include <cstdio>
#include <iostream>
#include <string>
using namespace std;

static bool readLine(FILE* f, string& line) { // returns if there is something in the file or not
    line.clear();
    int c;
    bool gotAny = false;
    while ((c = fgetc(f)) != EOF) { // while loop till eof
        gotAny = true;
        if (c == '\n') break; //end of line reached
        line += (char)c;
    }
    if (!line.empty() && line.back() == '\r') line.pop_back();
    return gotAny;
}

// file -> lines
bool Editor::load(const string& filename) { // 
    FILE* in = fopen(filename.c_str(), "r"); // read mode
    if (!in) return false; // file does not exist

    lines.clear();
    string line;
    while (readLine(in, line)) { // reads every line
        lines.push_back(line);
    }
    fclose(in);

    if (lines.size() == 0) 
    {
        lines.push_back("");
        cursor.line = 0;
        cursor.col = 0;
    }

    cursor.line = lines.size() - 1;
    cursor.col = (lines[lines.size() - 1]).length();
    
    hasSelection = false;
    undoStack.clear(); // history of the old document no longer applies
    redoStack.clear();
    return true;
}

// lines -> file
bool Editor::save(const string& filename) {
    FILE* out = fopen(filename.c_str(), "w"); // write mode, makes a file if the fle is not already present
    // every file in write mode overwrites the existing file 
    if (!out) return false;

    for (int i = 0; i < (int)lines.size(); i++) {
        fputs(lines[i].c_str(), out); // fputs for string
        fputc('\n', out); // fputc for character
    }
    fclose(out);
    return true;
}

void Editor::show() const {
    const string HIGHLIGHT_ON  = "\033[7m";   // swap foreground/background colors
    const string HIGHLIGHT_OFF = "\033[0m";   // back to normal

    for (int i = 0; i < (int)lines.size(); i++) {
        cout << i << "| ";
        if (i != cursor.line) {
            cout << lines[i];
        } else {
            const string& s = lines[i];
            int col = cursor.col;
            if (col < 0) col = 0;
            if (col > (int)s.size()) col = (int)s.size();

            cout << s.substr(0, col);                      // text before the cursor
            cout << HIGHLIGHT_ON;
            if (col < (int)s.size()) cout << s[col];       // the character under the cursor
            else cout << ' ';                              // cursor at end of line
            cout << HIGHLIGHT_OFF;
            if (col < (int)s.size()) cout << s.substr(col + 1);   // text after the cursor
        }
        cout << "\n";
    }
    cout << "(cursor: line " << cursor.line << ", col " << cursor.col << ")\n";
}