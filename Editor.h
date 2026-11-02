#pragma once

#include "Edit.h"
#include "Stack.h"
#include <string>
#include <vector>

using namespace std;

class Editor {
private:
  
    vector<string> lines;          
    Cursor cursor;                  // Current cursor position (line, col)
    Cursor selStart;                // Selection start position
    Cursor selEnd;                  // Selection end position
    bool hasSelection;              // True if a selection is active
    string clipboard;          // Text stored in clipboard
    
    // History Stacks
    Stack<Edit> undoStack;          // Undo history stack
    Stack<Edit> redoStack;          // Redo history stack

    // Private Helper Functions used in buffer.cpp & history.cpp
    void rawInsert(Cursor pos, const std::string& text);
    void rawErase(Cursor start, Cursor end);
    string getRange(Cursor start, Cursor end) const;
    void recordEdit(const Edit& edit);

public:
    // Constructor
    Editor();

    // File I/O (fileio.cpp)
    void load(const string& filename);
    void save(const string& filename);
    void show() const;

    // Buffer Operations (buffer.cpp)
    void moveCursor(int line, int col);
    void insertText(const string& text);
    void deleteChars(int n);

    // Clipboard Operations (clipboard.cpp)
    void select(Cursor start, Cursor end);
    void copy();
    void cut();
    void paste();

    // Search Operations (search.cpp)
    std::vector<Cursor> find(const string& target) const;
    int replaceAll(const string& oldStr, const string& newStr);

    // History Operations (history.cpp)
    void undo();
    void redo();
};
