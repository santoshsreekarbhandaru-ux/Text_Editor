#include "Editor.h"
#include <algorithm>
using namespace std;

void Editor::select(Cursor start , Cursor end){
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
    if(clipboard.empty()){
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
    edit.cursorBefore=cursor;

    if(hasSelection){
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
    cursor=endOfText(cursor,clipboard);
    edit.cursorAfter=cursor;

    recordEdit(edit);

}