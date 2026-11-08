#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
using namespace std;

vector<string> lines;         
int curLine = 0, curCol = 0;   // cursor (will be used by Task 2)

bool readLine(FILE* f, string& line) {
    line.clear();
    int c;
    bool gotAny = false;
    while ((c = fgetc(f)) != EOF) {
        gotAny = true;
        if (c == '\n') break;
        line += (char)c;
    }
    return gotAny;
}

// Task 1: file -> vector
bool loadFile(string path) {
    FILE* in = fopen(path.c_str(), "r");
    if (!in) return false;                 // file does not exist

    lines.clear();
    string line;
    while (readLine(in, line)) {
        lines.push_back(line);
    }
    fclose(in);

    if (lines.size() == 0) lines.push_back("");   // never keep the vector empty
    curLine = 0;
    curCol = 0;
    return true;
}

// Task 1: vector -> file
bool saveFile(string path) {
    FILE* out = fopen(path.c_str(), "w");
    if (!out) return false;

    for (int i = 0; i < (int)lines.size(); i++) {
        fputs(lines[i].c_str(), out);
        fputc('\n', out);
    }
    fclose(out);
    return true;
}

int main() {
    string fileName;
    cout << "Enter file name: ";
    getline(cin, fileName);

    // 1. LOAD
    if (loadFile(fileName)) {
        cout << "Loaded " << lines.size() << " line(s).\n";
    } else {
        cout << "File not found. Starting with an empty document.\n";
        lines.push_back("");
    }

    // 2. COMMAND LOOP 
    string input;
    while (true) {
        cout << "> ";
        if (!getline(cin, input)) break;       // stop if input ends (Ctrl+D / Ctrl+Z)

        if (input == "quit") {
            break;
        }
        else if (input == "show") {            // temporary, to check that loading worked
            for (int i = 0; i < (int)lines.size(); i++) {
                cout << i << "| " << lines[i] << "\n";
            }
        }
        // TODO Task 2: move, insert, delete
        else if(input == "move"){}
        else if(input == "insert"){}
        else if(input == "delete"){}
        // TODO Task 3: select, copy, cut, paste
        else if(input == "select"){}
        else if(input == "copy"){}
        else if(input == "cut"){}
        else if(input == "paste"){}
        // TODO Task 4: find, replace
        else if(input == "find"){}
        else if(input =="replace"){}
        // TODO Task 5: undo, redo
        else if(input == "undo"){}
        else if(input == "redo"){}
        else {
            cout << "Unknown command.\n";
        }
    }

    // 3. SAVE
    if (saveFile(fileName)) cout << "Saved.\n";
    else cout << "Could not save.\n";

    return 0;
}