#include "Editor.h"
#include <algorithm>
using namespace std;

void Editor::select(Cursor start , Cursor end){
    // claude edited: added parentheses around the && so the intent is clear (same logic as before)
    if(start.line>end.line|| (start.line==end.line && start.col>end.col)){
        swap(start,end);
    }
    selStart=start;
    selEnd=end;
    hasSelection=true;
}

void Editor::copy(){
    if(hasSelection==false){
        return;
    }
    clipboard=getRange(selStart,selEnd);
}

void Editor::cut(){
    if(hasSelection==false){
        return;
    }
    copy();
    if(clipboard.empty()){   // claude edited: empty/invalid selection, nothing to cut so don't record a useless edit
        hasSelection=false;
        return;
    }
    Edit edit;
    edit.cursorBefore=cursor;

    Change change;
    change.isInsert=false;
    change.pos=selStart;
    change.text=clipboard;
    edit.changes.push_back(change);

    rawErase(selStart,selEnd);
    cursor=selStart;
    edit.cursorAfter=cursor;
    hasSelection=false;

    recordEdit(edit);
}

void Editor::paste(){
    if(clipboard.empty()){
        return;
    }
    Edit edit;
    edit.cursorBefore=cursor;   // claude edited: moved above the selection erase so undo restores the cursor properly

    if(hasSelection){
        // claude edited: the erase of the selected text is now recorded in the same Edit,
        // so one undo restores both the old selected text and removes the pasted text
        Change erased;
        erased.isInsert=false;
        erased.pos=selStart;
        erased.text=getRange(selStart,selEnd);
        edit.changes.push_back(erased);

        rawErase(selStart, selEnd);
        cursor = selStart;
        hasSelection = false;
    }

    Change change;
    change.isInsert=true;
    change.pos=cursor;
    change.text=clipboard;
    edit.changes.push_back(change);

    rawInsert(cursor,clipboard);
    cursor=endOfText(cursor,clipboard);   // claude edited: cursor now moves to the end of the pasted text
    edit.cursorAfter=cursor;

    recordEdit(edit);

}