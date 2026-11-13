#include "Editor.h"
#include <string>
#include <vector>
#include <algorithm>   // claude edited: needed for min()
using namespace std;

// claude edited: constructor was declared but never defined
Editor::Editor(){
    lines.push_back("");     // document is never empty
    cursor.line=0;
    cursor.col=0;
    selStart=cursor;
    selEnd=cursor;
    hasSelection=false;
}

// claude edited: moveCursor was declared but never defined
void Editor::moveCursor(int line, int col){
    if(line<0) line=0;
    if(line>=(int)lines.size()) line=(int)lines.size()-1;
    if(col<0) col=0;
    if(col>(int)lines[line].size()) col=(int)lines[line].size();
    cursor.line=line;
    cursor.col=col;
}

// claude edited: insertText was declared but never defined
void Editor::insertText(const string& text){
    if(text.empty()) return;

    Edit edit;
    edit.cursorBefore=cursor;

    Change change;
    change.isInsert=true;
    change.pos=cursor;
    change.text=text;
    edit.changes.push_back(change);

    rawInsert(cursor,text);
    cursor=endOfText(cursor,text);   // cursor goes to the end of inserted text
    edit.cursorAfter=cursor;
    hasSelection=false;

    recordEdit(edit);
}

// claude edited: deleteChars was declared but never defined (forward delete of n characters from the cursor)
void Editor::deleteChars(int n){
    if(n<=0) return;

    Cursor start=cursor;
    while(n>0){
        if(start.col>0){
            int s=min(n,start.col);
            start.col-=s;
            n-=s;
        }
        else if(start.line>0){
            start.line--;     // backspace at column 0 joins with the previous line
            start.col=(int)lines[start.line].size();
            n--;
        }
        else break;
    }

    string t=getRange(start,cursor);
    if(t.empty()) return;

    Edit edit;
    edit.cursorBefore=cursor;

    Change change;
    change.isInsert=false;
    change.pos=start;
    change.text=t;
    edit.changes.push_back(change);

    rawErase(start,cursor);
    cursor=start;             // cursor moves back to where the deletion began
    edit.cursorAfter=cursor;
    hasSelection=false;

    recordEdit(edit);
}

void Editor::rawInsert(Cursor pos,const string& text){   // claude edited: const string&
    //cursor position changes are not handled yet
    if(text.empty()==true) return;

    int n=(int)lines.size();   // claude edited: signed copy of size, lines.size()-1 on unsigned is unsafe

    // claude edited: negative check moved first (negative int compared with unsigned size became a huge number)
    if(pos.line<0){
        pos.line=0;
        pos.col=0;
    }
    if(pos.line>n-1){
        pos.line=n-1;
        pos.col=(int)lines[pos.line].size();
    }
    if(pos.col>(int)lines[pos.line].size()){
        pos.col=(int)lines[pos.line].size();
    }
    if(pos.col<0) pos.col=0;

    vector<string> split;
    int start=0;

    for(int i=start;i<(int)text.length();i++){   // claude edited: cast to int
        if(text[i]=='\n'){
            split.push_back(text.substr(start,i-start));
            start=i+1;
        }
    }
    split.push_back(text.substr(start));

    string head=lines[pos.line].substr(0,pos.col);
    string tail=lines[pos.line].substr(pos.col);

    if(split.size()==1){
        lines[pos.line]=head+split[0]+tail;
        return;
    }
    lines[pos.line]=head+split[0];
    vector<string> newlines(split.begin()+1,split.end());
    newlines.back()+=tail;

    lines.insert(lines.begin()+pos.line+1,newlines.begin(),newlines.end());

}

void Editor::rawErase(Cursor start,Cursor end){
    //right exclusive
    string t;
    int n=(int)lines.size();   // claude edited: signed size so negative checks work
    if(start.line<0 || start.col<0) return;   // claude edited: moved before the indexing checks
    if(start.line>=n || start.col>(int)lines[start.line].size()) return;
    if(end.line<0 || end.col<0) return;       // claude edited: added negative check for end
    if(end.line>=n || end.col>(int)lines[end.line].size()) return;
    if(start.line>end.line) return;
    if(start.line==end.line && start.col>=end.col) return;
    if(start.line==end.line){
        lines[start.line].erase(start.col,end.col-start.col);
        return;
    }
    t=lines[start.line].substr(0,start.col)
            +lines[end.line].substr(end.col);
    lines[start.line]=t;
    lines.erase(lines.begin()+start.line+1,lines.begin()+end.line+1);
}

string Editor::getRange(Cursor start,Cursor end){
    //right exclusive
    string answer;
    int n=(int)lines.size();   // claude edited: signed size so negative checks work
    if(start.line<0 || start.col<0) return "";   // claude edited: moved before the indexing checks
    if(start.line>=n || start.col>(int)lines[start.line].size()) return "";
    if(end.line<0 || end.col<0) return "";       // claude edited: added negative check for end
    if(end.line>=n || end.col>(int)lines[end.line].size()) return "";
    if(start.line>end.line) return "";
    if(start.line==end.line && start.col>=end.col) return "";
    if(start.line==end.line){
        answer=lines[start.line].substr(start.col,end.col-start.col);
        return answer;
    }
    answer=lines[start.line].substr(start.col);
    for(int i=start.line+1;i<end.line;i++){
        answer+='\n';
        answer+=lines[i];
    }
    answer+='\n';
    answer+=lines[end.line].substr(0,end.col);
    return answer;
}