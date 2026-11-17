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



void Editor::remove(){
    if(hasSelection==false){
        return;
    }
    
    
    Edit edit;
    edit.cursorBefore=cursor;

    Change change;
    change.isInsert=false;
    change.pos=selStart;
    change.text=getRange(selStart,selEnd);
    edit.changes.push_back(change);

    rawErase(selStart,selEnd);
    cursor=selStart;
    edit.cursorAfter=cursor;
    hasSelection=false;

    recordEdit(edit);
}

void Editor::deleteLine(unsigned int k) {
    if (k >= lines.size()) {
        cout << "Invalid line number.\n";
        return;
    }
    Cursor c1, c2;
    c1.line = k;
    c1.col = 0;

    if (k < lines.size() - 1) {
        // Delete current line along with its newline
        c2.line = k + 1;
        c2.col = 0;
    }
    else {
        // Last line: delete its contents
        c2.line = k;
        c2.col = lines[k].size();
        // Include the preceding newline if possible
        if (k > 0) {
            c1.line = k - 1;
            c1.col = lines[k - 1].size();
        }
    }
    select(c1, c2);
    remove();
}