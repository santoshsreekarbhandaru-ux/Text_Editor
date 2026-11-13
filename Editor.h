#pragma once

#include "Edit.h"
#include "Stack.h"
#include <string>
#include <vector>

// claude edited: removed "using namespace std;" from this header, using std:: instead

class Editor {
private:

    std::vector<std::string> lines;   // claude edited: std::
    Cursor cursor;                    // Current cursor position (line, col)
    Cursor selStart;                  // Selection start position
    Cursor selEnd;                    // Selection end position
    bool hasSelection = false;        // True if a selection is active (claude edited: default false)
    std::string clipboard;            // Text stored in clipboard

    // History Stacks
    Stack<Edit> undoStack;            // Undo history stack
    Stack<Edit> redoStack;            // Redo history stack

    // Private Helper Functions used in buffer.cpp & history.cpp
    void rawInsert(Cursor pos, const std::string& text);  // claude edited: const string& (was string&, caused compile errors)
    void rawErase(Cursor start, Cursor end);
    std::string getRange(Cursor start, Cursor end);
    void recordEdit(const Edit& edit);

public:
    // Constructor
    Editor();

    // File I/O (fileio.cpp)
    bool load(const std::string& filename);   // claude edited: returns bool so main can report success/failure
    bool save(const std::string& filename);   // claude edited: returns bool so main can report success/failure
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
    std::vector<Cursor> find(const std::string& target) const; // claude edited: const string& to match search.cpp
    int replaceAll(const std::string& oldStr, const std::string& newStr);

    // History Operations (history.cpp)
    void undo();
    void redo();
};