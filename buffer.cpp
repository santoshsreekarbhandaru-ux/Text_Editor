#include "Editor.h"
#include <string>
#include <vector>
using namespace std;

void Editor::rawInsert(Cursor pos,string& text){
    if(text.empty()==true) return;
    
    if(pos.line>lines.size()-1){
        pos.line=lines.size()-1;
        pos.col=lines[pos.line].size();
    }
    if(pos.line<0){
        pos.line=0;
        pos.col=0;
    }
    if(pos.col>lines[pos.line].size()){
        pos.col=lines[pos.line].size();
    }
    if(pos.col<0) pos.col=0;

    vector<string> split;
    int start=0;

    for(int i=start;i<text.length;i++){
        if(text[i]=='\n'){
            split.push_back(text.substr(start,i-start))
            start=i+1;
        }
    }

    string head=lines[pos.line].substring(0,pos.col);
    string tail=lines[pos.line].substring(pos.col);

    if(split.empty()){
        lines[pos.line]=head+text+tail;
        return;
    }
    lines[pos.line]=head+split[0];
    vector<string> newlines(split.begin()+1,split.end());
    newlines.back+=tail;
    
    lines.insert(lines.begin()+pos.line+1,newlines.begin(),newlines.end());
   
}

void Editor::rawErase(Cursor start,Cursor end){
    string t;
    if(start.line>end.line) return;
    if(start.line==end.line && start.col>end.col) return;
    if(start.line==end.line){
        lines[start.line].erase(start.col,end.col-start.col+1);
        return;
    }//L1:abc 
    //L2:de
    //if we want it to finally become abde that is not
    //possible as we are unable to give end cursor position
    //will handle that later
    if(end.col==0){
        t=lines[start.line].substr(0,start.col)
            +lines[end.line].substr(end.col);
    }
    t=lines[start.line].substr(0,start.col)
            +lines[end.line].substr(end.col+1);
    lines[start.line]=t;
    lines.erase(lines.begin()+start.line+1,lines.begin()+end.line+1);
}

string Editor::getRange(Cursor start,Cursor end){
    string answer;
    if(start.line>end.line) return;
    if(start.line==end.line && start.col>end.col) return;
    if(start.line==end.line){
        answer=lines[start.line].substr(start.col,end.col-start.col+1);
        return answer;
    }
    //same edge case as before cannot be handled here also
    answer=lines[start.line].substr(start.col);
    for(int i=start.line+1;i<end.line;i++){
        answer+='\n';
        answer+=lines[i];
    }
    answer+='\n';
    answer+=lines[end.line].substr(0,end.col+1);
    return answer;
}

bool Editor::moveCursor(int line,int col){
    if(line>lines.size() || line<0) return false;
    if(col>lines[line].size())
}