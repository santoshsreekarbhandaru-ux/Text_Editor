#include <string> 
#include <vector>
#include "Editor.h"
using namespace std;

static const string k = " .!-,:;";

// Find every place where target appears. Returns (line, col) of each match.
vector<Cursor> Editor::find(const string& target)const{
    vector<Cursor> result; // vector of cursor to record the position at which the word is found 
    if (target.empty()) return result;
    for (int i=0;i<(int)lines.size();i++) {
        size_t pos=lines[i].find(target);
        while (pos!=string::npos){
            Cursor c;
            c.line=i;
            c.col=(int)pos;
            size_t after=pos+target.size();
            bool leftOK  = (pos==0) || (k.find(lines[i][pos-1])!=string::npos);
            bool rightOK = (after>=lines[i].size()) || (k.find(lines[i][after])!=string::npos);

            if(leftOK && rightOK){
                result.push_back(c);
            }
            pos=lines[i].find(target, pos+target.size());
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
    for (int idx=(int)matches.size()-1; idx>=0; idx--){
        Cursor start=matches[idx];
        Cursor end;
        end.line=start.line;
        end.col=start.col+(int)oldStr.size();
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
    if (cursor.line>=(int)lines.size()) cursor.line=(int)lines.size()-1;
    if (cursor.col>(int)lines[cursor.line].size()) cursor.col=(int)lines[cursor.line].size();
    edit.cursorAfter=cursor;
    recordEdit(edit);
    return (int)matches.size();
}