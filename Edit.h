#pragma once
#include<string>
#include<vector>
using namespace std;

//cursor struct
struct Cursor{
    int line;
    int col;
};

//change struct
struct Change{
    Cursor pos;
    string text;
    bool isInsert;
};

//edit struct
struct Edit{
    vector<Change> changes;
    Cursor cursorBefore;
    Cursor cursorAfter;
};
