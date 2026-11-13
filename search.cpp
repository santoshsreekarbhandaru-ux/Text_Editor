#include <string>    // claude edited: replaced <bits/stdc++.h> (GCC only) with the standard headers
#include <vector>
#include "Editor.h"
using namespace std;

static const string k = " .!-,:;";   // claude edited: static const so it doesn't clash with other files

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

            // claude edited: the old check was inverted (it SKIPPED matches that had a separator next to them).
            // Now a match is kept only if it is a whole word: start of line or a separator before it,
            // and end of line or a separator after it.
            size_t after=pos+target.size();
            bool leftOK  = (pos==0) || (k.find(lines[i][pos-1])!=string::npos);
            bool rightOK = (after>=lines[i].size()) || (k.find(lines[i][after])!=string::npos);

            if(leftOK && rightOK){
                result.push_back(c);
            }
            // claude edited: advance by target size (was pos+1) so overlapping matches like "aa" in "aaa" don't corrupt replaceAll
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
    for (int idx=(int)matches.size()-1; idx>=0; idx--){   // claude edited: renamed loop variable k to idx (k is the global separator string)
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
    // claude edited: cast to int so the comparisons are signed-safe
    if (cursor.line>=(int)lines.size()) cursor.line=(int)lines.size()-1;
    if (cursor.col>(int)lines[cursor.line].size()) cursor.col=(int)lines[cursor.line].size();
    edit.cursorAfter=cursor;
    recordEdit(edit);
    return (int)matches.size();
}