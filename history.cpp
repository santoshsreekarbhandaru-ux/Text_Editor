#include "Editor.h"
#include <iostream>
#include <string>

using namespace std;

void Editor::recordEdit(const Edit& edit) 
{
    undoStack.push(edit);
    redoStack.clear();
}

void Editor::undo() 
{
    if (undoStack.isEmpty()) 
    {
        cout << "Nothing to undo.\n";
        return;
    }

    Edit edit = undoStack.pop();

    for (int i = (int)edit.changes.size() - 1; i >= 0; i--) 
    {
        const Change& c = edit.changes[i];

        if (c.isInsert) 
        {
            rawErase(c.pos, endOfText(c.pos, c.text));
        } 
        else 
        {
            rawInsert(c.pos, c.text);
        }
    }

    cursor = edit.cursorBefore;
    hasSelection = false;

    redoStack.push(edit);
}

void Editor::redo() 
{
    if (redoStack.isEmpty()) 
    {
        cout << "Nothing to redo.\n";
        return;
    }

    Edit edit = redoStack.pop();

    for (size_t i = 0; i < edit.changes.size(); i++) 
    {
        const Change& c = edit.changes[i];

        if (c.isInsert) 
        {
            rawInsert(c.pos, c.text);
        } 
        else 
        {
            rawErase(c.pos, endOfText(c.pos, c.text));
        }
    }

    cursor = edit.cursorAfter;
    hasSelection = false;

    undoStack.push(edit);
}