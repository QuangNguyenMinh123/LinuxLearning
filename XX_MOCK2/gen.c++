#include <iostream>
using namespace std;

int main()
{
    int i = 0;
    while (i < 100)
    {
        if (i<10)
        {
            cout<<"echo "<<i<<".THIS IS LINE "<<i<<"THIS IS LINE "<<i<<" > /proc/ili9341"<<endl;
        }
        else
        {
            cout<<"echo "<<"THIS IS LINE "<<i<<"THIS IS LINE "<<i<<" > /proc/ili9341"<<endl;
        }
        i++;
    }
    return 0;
}