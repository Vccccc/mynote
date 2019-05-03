#include <string>
#include <iostream>
using namespace std;


void replaceSpace(string& str)
{
    int n = 0;
    for(auto& i : str)
    {
        if(i == ' ')
        {
            ++n;
        }
    }
    
    n *= 2;
    n += str.size();
    string res;
    str.resize(n);
    for(int i = str.size() - 1, j = n - 1; i >= 0; --i)
    {
        if(str[i] != ' ') 
        {
            res[j--] = str[i];
        }
        else
        {
            res[j--] = '0';
            res[j--] = '2';
            res[j--] = '%';
        }
    }
    str = res;
}
