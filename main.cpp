#include <iostream>
#include <sstream> 
#include <string>
#include <vector>
#include "Editor.h" 
using namespace std;

// They now live inside the Editor class (buffer.cpp, fileio.cpp).

static string unescape(const string& s) {
    string out;
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] == '\\' && i + 1 < s.size() && s[i + 1] == 'n') {
            out += '\n';
            i++;
        } else {
            out += s[i];
        }
    }
    return out;
}

static void printHelp() {
    cout << "Commands:\n"
         << "  show                          print the document\n"
         << "  move <line> <col>             move the cursor\n"
         << "  insert <text>                 insert text at the cursor (use \\n for a new line)\n"
         << "  delete <n>                    delete n characters after the cursor\n"
         << "  select <l1> <c1> <l2> <c2>    select a range\n"
         << "  copy | cut | paste\n"
         << "  find <word>                   list whole-word matches\n"
         << "  replace <old> <new>           replace all whole-word matches\n"
         << "  undo | redo\n"
         << "  help | quit\n";
}

int main() {
    Editor ed; 

    string fileName;
    cout << "Enter file name: ";
    getline(cin, fileName);

    // 1. LOAD
    if (ed.load(fileName)) { 
        cout << "Loaded file.\n";
    } else {
        cout << "File not found. Starting with an empty document.\n";

    }
    cout << "Type 'help' to see the commands.\n";

    // 2. COMMAND LOOP 
    string input;
    while (true) {
        cout << "> ";
        if (!getline(cin, input)) break;       // stop if input ends (Ctrl+D / Ctrl+Z)

        istringstream iss(input);
        string cmd;
        iss >> cmd;
        if (cmd.empty()) continue;

        if (cmd == "quit") {
            break;
        }
        else if (cmd == "help") {
            printHelp();
        }
        else if (cmd == "show") {
            ed.show();
        }
        // Task 2: move, insert, delete 
        else if (cmd == "move") {
            int l, c;
            if (iss >> l >> c) ed.moveCursor(l, c);
            else cout << "Usage: move <line> <col>\n";
        }
        else if (cmd == "insert") {
            string text;
            getline(iss, text);
            if (!text.empty() && text[0] == ' ') text.erase(0, 1);   // drop the space after the command
            if (text.empty()) cout << "Usage: insert <text>\n";
            else ed.insertText(unescape(text));
    
        }
        else if (cmd == "delete") {
            int n;
            if (iss >> n) ed.deleteChars(n);
            else cout << "Usage: delete <n>\n";
        }
        // Task 3: select, copy, cut, paste 
        else if (cmd == "select") {
            int l1, c1, l2, c2;
            if (iss >> l1 >> c1 >> l2 >> c2) {
                Cursor a, b;
                a.line = l1; a.col = c1;
                b.line = l2; b.col = c2;
                ed.select(a, b);
            } else {
                cout << "Usage: select <l1> <c1> <l2> <c2>\n";
            }
        }
        else if (cmd == "copy") {
            ed.copy();
        }
        else if (cmd == "cut") {
            ed.cut();
        }
        else if (cmd == "paste") {
            ed.paste();
        }
        // Task 4: find, replace 
        else if (cmd == "find") {
            string target;
            iss >> target;
            if (target.empty()) {
                cout << "Usage: find <word>\n";
            } else {
                vector<Cursor> res = ed.find(target);
                for (size_t i = 0; i < res.size(); i++) {
                    cout << "line " << res[i].line << ", col " << res[i].col << "\n";
                }
                cout << res.size() << " match(es).\n";
            }
        }
        else if (cmd == "replace") {
            string a, b;
            iss >> a >> b;
            if (a.empty()) {
                cout << "Usage: replace <old> <new>\n";
            } else {
                int count = ed.replaceAll(a, b);
                cout << count << " replacement(s).\n";
            }
        }
        // Task 5: undo, redo 
        else if (cmd == "undo") {
            ed.undo();
        }
        else if (cmd == "redo") {
            ed.redo();
        }
        // Task 6:save
        else if (cmd == "save") {
            string target;
            iss >> target;                       // optional: "save other.txt"
            if (target.empty()) target = fileName;   // plain "save" uses the loaded file name
            if (ed.save(target)) cout << "Saved to " << target << ".\n";
            else cout << "Could not save.\n";
        }
        else {
            cout << "Unknown command. Type 'help'.\n";
        }
    
    }

    // 3. SAVE
    if (ed.save(fileName)) cout << "Saved.\n"; 
    else cout << "Could not save.\n";

    return 0;
}