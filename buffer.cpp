#include "Editor.h"
#include <string>
#include <vector>
using namespace std;

void Editor::moveCursor(int line, int col){
    if(line>=(int)lines.size() || line<0) return;
    if(col>(int)lines[line].size() || col<0) return;
    cursor={line,col};
}

void swap(Cursor& a, Cursor& b){
    Cursor temp=b;
    b=a;
    a=temp;
}

void Editor::rawInsert(Cursor pos,const string& text){
    //if invalid positions we are making it to eof or beginning
    if(text.empty()==true) return;
    
    if(pos.line>(int)lines.size()-1){
        pos.line=(int)lines.size()-1;
        pos.col=(int)lines[pos.line].size();
    }
    if(pos.line<0){
        pos.line=0;
        pos.col=0;
    }
    if(pos.col>(int)lines[pos.line].size()){
        pos.col=(int)lines[pos.line].size();
    }
    if(pos.col<0) pos.col=0;

    vector<string> split;
    int start=0;

    for(int i=start;i<text.length();i++){
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
        moveCursor(pos.line,pos.col+text.size());
        return;
    }
    lines[pos.line]=head+split[0];
    vector<string> newlines(split.begin()+1,split.end());
    newlines.back()+=tail;
    
    lines.insert(lines.begin()+pos.line+1,newlines.begin(),newlines.end());
    moveCursor(pos.line+newlines.size(),split.back().size());
   
}

void Editor::rawErase(Cursor start,Cursor end){
    //right exclusive
    string t;
    if(start.line<0 || end.line<0 || start.col<0 || end.col<0) return;
    if(start.line>=lines.size() || start.col>lines[start.line].size()) return;
    if(end.line>=lines.size() || end.col>lines[end.line].size()) return;
    if(start.line>end.line) swap(start,end);
    if(start.line==end.line && start.col>end.col) swap(start,end);
    if(start.line==end.line){
        lines[start.line].erase(start.col,end.col-start.col);
        moveCursor(start.line,start.col);
        return;
    }
    t=lines[start.line].substr(0,start.col)
            +lines[end.line].substr(end.col);
    lines[start.line]=t;
    lines.erase(lines.begin()+start.line+1,lines.begin()+end.line+1);
    moveCursor(start.line,start.col);
}

string Editor::getRange(Cursor start,Cursor end) const{
    //right exclusive
    string answer;
    if(start.line<0 || end.line<0 || start.col<0 || end.col<0) return "";
    if(start.line>=lines.size() || start.col>lines[start.line].size()) return "";
    if(end.line>=lines.size() || end.col>lines[end.line].size()) return "";
    if(start.line>end.line) swap(start,end);
    if(start.line==end.line && start.col>end.col) swap(start,end);
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

void Editor::insertText(const string& text){
    if(text.empty()) return;

    Edit t;
    t.cursorBefore=cursor;
    t.changes.push_back({cursor,text,true});
    rawInsert(cursor,text);
    t.cursorAfter=cursor;
    hasSelection=false; // think about this
    recordEdit(t);
}

void Editor::deleteChars(int n){
    if(n<=0) return;
    Cursor end={cursor.line,cursor.col};
    while(n>0){
        if(end.line==0 && end.col<n){
            end={0,0};//if trying to delete more characters than present
            break;
        }
        if(end.col>=n){
            end.col-=n;
            break;
        }
        n=n-end.col-1;
        end.line--;
        end.col=lines[end.line].size();
    }
    string erased=getRange(end,cursor);
    if(erased.empty()) return;
    Edit t;
    t.cursorBefore=cursor;
    t.changes.push_back({end,erased,false});
    rawErase(end,cursor);
    t.cursorAfter=cursor;
    hasSelection=false; // think about this
    recordEdit(t);
}