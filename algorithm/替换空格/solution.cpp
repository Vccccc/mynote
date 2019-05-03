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

int main()
{
    string test("We are happy.");
    string test1(" We are happy.");
    string test2("We are happy. ");
    string test3("We   are  happy.");
    string test4("          ");
    
    replaceSpace(test);
    replaceSpace(test1);
    replaceSpace(test2);
    replaceSpace(test3);
    replaceSpace(test4);
    cout << test << "\n";
    cout << test1 << "\n";
    cout << test2 << "\n";
    cout << test3 << "\n";
    cout << test4 << "\n";
    return 0;
}
