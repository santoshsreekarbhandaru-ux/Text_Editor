#include <bits/stdc++.h>
#include "Editor.h"
using namespace std;

string k = " .!-,:;";
// Find every place where target appears. Returns (line, col) of each match.
vector<Cursor> Editor::find(const string& target)const{
    vector<Cursor> result; // vector of cursor to record the position at which the word is found 
    if (target.empty()) return result;
    for (int i=0;i<lines.size();i++) {
        size_t pos=lines[i].find(target);
        while (pos!=string::npos){
            Cursor c;
            c.line=i;
            c.col=(int)pos;
            if((pos>0 && k.find(lines[i][pos-1])!=-1) || (pos < lines[i].size() && k.find(lines[i][pos+target.size()])!=-1)){
            pos=lines[i].find(target, pos+1);
            continue;
            }
            result.push_back(c);
            pos=lines[i].find(target, pos+1);
        }
    }
    return result;
}

int Editor::replaceAll(const string& oldStr,const string& newStr){
    if(oldStr.empty()||oldStr==newStr)return 0;
    vector<Cursor> matches;
    matches=find(oldStr);
    if (matches.empty()) return 0;
    Edit edit;
    edit.cursorBefore=cursor;
    for (int k =matches.size()-1; k>=0;k--){
        Cursor start=matches[k];
        Cursor end;
        end.line=start.line;
        end.col=start.col+oldStr.size();
        Change erased;
        erased.pos=start;
        erased.text=oldStr;
        erased.isInsert=false;
        edit.changes.push_back(erased);
        rawErase(start,end);

        if (!newStr.empty()) {
            Change inserted;
            inserted.pos=start;
            inserted.text=newStr;
            inserted.isInsert = true;
            edit.changes.push_back(inserted);
            rawInsert(start, newStr);
        }
    }
    hasSelection = false;
    if (cursor.line>=lines.size()) cursor.line=lines.size()-1;
    if (cursor.col>lines[cursor.line].size()) cursor.col=lines[cursor.line].size();
    edit.cursorAfter=cursor;
    recordEdit(edit);
    return matches.size();
}