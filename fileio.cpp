// claude edited: this whole file is new. load/save/show were declared in Editor.h but never defined.
// The code is moved from main.cpp (readLine, loadFile, saveFile) into the Editor class.
#include "Editor.h"
#include <cstdio>
#include <iostream>
#include <string>
using namespace std;

static bool readLine(FILE* f, string& line) {
    line.clear();
    int c;
    bool gotAny = false;
    while ((c = fgetc(f)) != EOF) {
        gotAny = true;
        if (c == '\n') break;
        line += (char)c;
    }
    if (!line.empty() && line.back() == '\r') line.pop_back();   // claude edited: strip \r from Windows line endings
    return gotAny;
}

// file -> lines
bool Editor::load(const string& filename) {
    FILE* in = fopen(filename.c_str(), "r");
    if (!in) return false;                 // file does not exist

    lines.clear();
    string line;
    while (readLine(in, line)) {
        lines.push_back(line);
    }
    fclose(in);

    if (lines.size() == 0) lines.push_back("");   // never keep the vector empty
    cursor.line = 0;
    cursor.col = 0;
    hasSelection = false;
    undoStack.clear();   // history of the old document no longer applies
    redoStack.clear();
    return true;
}

// lines -> file
bool Editor::save(const string& filename) {
    FILE* out = fopen(filename.c_str(), "w");
    if (!out) return false;

    for (int i = 0; i < (int)lines.size(); i++) {
        fputs(lines[i].c_str(), out);
        fputc('\n', out);
    }
    fclose(out);
    return true;
}

void Editor::show() const {
    for (int i = 0; i < (int)lines.size(); i++) {
        cout << i << "| " << lines[i] << "\n";
    }
    cout << "(cursor: line " << cursor.line << ", col " << cursor.col << ")\n";
}