#include "Editor.h"
#include <string>
#include <vector>
using namespace std;

void Editor::rawInsert(Cursor pos,string& text){
    //cursor position changes are not handled yet
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
    if(start.line>=lines.size() || start.col>lines[start.line].size()) return;
    if(end.line>=lines.size() || end.col>lines[end.line].size()) return;
    if(start.line<0 || start.col<0) return;
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
    if(start.line>=lines.size() || start.col>lines[start.line].size()) return "";
    if(end.line>=lines.size() || end.col>lines[end.line].size()) return "";
    if(start.line<0 || || start.col<0) return "";
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
