#include <vector>
#include <iostream>
using namespace std;

bool findNumber(const vector<vector<int>>& input, const int target)
{
    int rows = input.size();
    int cols = input[0].size();
    
    bool found = false;

    if(input.empty())
        return found;

    int row = 0, col = cols - 1;
    while(row < rows && col >= 0)
    {
        if(input[row][col] == target)
        {
            found = true;
            break;
        }
        else if(input[row][col] > target)
        {
            --col;
        }
        else 
        {
            ++row;
        }
    }
    return found; 
}

int main()
{
    typedef vector<vector<int>> vec;
    vec test2{ 
        {1, 2, 8, 9},
        {2, 4, 9, 12},
        {4, 7, 10, 13},
        {6, 8, 11, 15}
    };

    cout << findNumber(test2, 1) << endl;
    cout << findNumber(test2, 15) << endl;
    cout << findNumber(test2, 7) << endl;
    cout << findNumber(test2, 0) << endl;
    cout << findNumber(test2, 16) << endl;
    return 0;
}
