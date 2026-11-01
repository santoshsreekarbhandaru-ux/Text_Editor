#pragma once

#include "Edit.h"
#include "Stack.h"
#include <string>
#include <vector>

class Editor {
private:
    // Core Data Members
    std::vector<std::string> lines; // Text buffer lines (starts as {""})
    Cursor cursor;                  // Current cursor position (line, col)
    Cursor selStart;                // Selection start position
    Cursor selEnd;                  // Selection end position
    bool hasSelection;              // True if a selection is active
    std::string clipboard;          // Text stored in clipboard
    
    // History Stacks
    Stack<Edit> undoStack;          // Undo history stack
    Stack<Edit> redoStack;          // Redo history stack

    // Private Helper Functions (buffer.cpp & history.cpp)
    void rawInsert(Cursor pos, const std::string& text);
    void rawErase(Cursor start, Cursor end);
    std::string getRange(Cursor start, Cursor end) const;
    void recordEdit(const Edit& edit);

public:
    // Constructor
    Editor();

    // File I/O (fileio.cpp)
    void load(const std::string& filename);
    void save(const std::string& filename);
    void show() const;

    // Buffer Operations (buffer.cpp)
    void moveCursor(int line, int col);
    void insertText(const std::string& text);
    void deleteChars(int n);

    // Clipboard Operations (clipboard.cpp)
    void select(Cursor start, Cursor end);
    void copy();
    void cut();
    void paste();

    // Search Operations (search.cpp)
    std::vector<Cursor> find(const std::string& target) const;
    int replaceAll(const std::string& oldStr, const std::string& newStr);

    // History Operations (history.cpp)
    void undo();
    void redo();
};