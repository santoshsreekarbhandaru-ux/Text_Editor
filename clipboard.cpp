#include "Editor.h"
#include <algorithm>
using namespace std;

void Editor::select(Cursor start , Cursor end){
    if(start.line>end.line|| start.line==end.line && start.col>end.col){
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
    Edit edit;
    edit.cursorBefore=cursor;
    
    if(clipboard.empty()){
        return;
    }
    if(hasSelection){
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
    edit.cursorAfter=cursor;

    recordEdit(edit);

}



