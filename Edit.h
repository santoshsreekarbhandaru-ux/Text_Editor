#pragma once
#include <string>
#include <vector>
// claude edited: removed "using namespace std;" from this header (it leaks into every file); using std:: instead

//cursor struct
struct Cursor{
    int line = 0;   // claude edited: default value so it is never garbage
    int col = 0;    // claude edited: default value so it is never garbage
};

//change struct
struct Change{
    Cursor pos;
    std::string text;     // claude edited: std::string
    bool isInsert = true; // claude edited: default value
};

//edit struct
struct Edit{
    std::vector<Change> changes; // claude edited: std::vector
    Cursor cursorBefore;
    Cursor cursorAfter;
};

// claude edited: moved endOfText here from history.cpp so buffer.cpp, clipboard.cpp etc. can also use it
// Returns the cursor position that comes right after "text" if it is inserted at "start"
inline Cursor endOfText(Cursor start, const std::string& text)
{
    Cursor end = start;

    for (char ch : text)
    {
        if (ch == '\n')
        {
            end.line++;
            end.col = 0;   // new line starts at column 0
        }
        else
        {
            end.col++;
        }
    }
    return end;
}