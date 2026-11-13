#pragma once
#include <string>
#include <vector>
//cursor struct
struct Cursor{
    int line = 0;
    int col = 0;
};

//change struct
struct Change{
    Cursor pos;
    std::string text;
    bool isInsert = true;
};

//edit struct
struct Edit{
    std::vector<Change> changes;
    Cursor cursorBefore;
    Cursor cursorAfter;
};

inline Cursor endOfText(Cursor start, const std::string& text)
{
    Cursor end = start;

    for (char ch : text)
    {
        if (ch == '\n')
        {
            end.line++;
            end.col = 0; // new line starts at column 0
        }
        else
        {
            end.col++;
        }
    }
    return end;
}