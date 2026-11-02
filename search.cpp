#include <bits/stdc++.h>
#include <Editor.h>
using namespace std;
vector<vector<int>> find(vector<vector<string>>& file,string target){
    vector <vector<int>>  positions;
    for (int i = 0; i < file.size(); i++){
        for(int j =0;j<file[i].size(); j++){
            if(file[i][j] == target){
                vector<int> curr;
                curr.push_back(i);
                curr.push_back(j);
                positions.push_back(curr);
            }
        }
    }
    return positions;
}

vector <vector<int>> replaceAll(vector<vector<string>>& file,string original, string changed ){
    vector <vector<int>> positions = find(file, original);
    for(int i =0 ; i<positions.size();i++){
        file[positions[i][0]][positions[i][1]] = changed;
    }
    return positions; // returns the edited positions for the edit stack to push
}

// called in main (included), original and new string stored there, positions along with old,new string is passed to edit
