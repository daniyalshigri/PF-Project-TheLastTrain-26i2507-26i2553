#include "util.h"
#include <iostream>
using namespace std;
int clampInt(int val, int minVal, int maxVal)
{
    if(val<minVal)
    {
        return minVal;
    }
    else if(val>maxVal)
    {
        return maxVal;

    }
    else{
        return val;
    }
}

bool stringEquals(const char* a, const char* b)
{
    int i=0;
    while(a[i]!='\0')
    
    {
    if(a[i]==b[i])
    {
    i++;
    }
    else
    {
        return false;
    }
    }
    return (b[i]=='\0');

}

void printHeader(const char* title)
{
    for (int i =0; i<40 ; i++)
    {
        cout<< "=";
    }
    cout<<endl;
    cout<<title<<endl;
     for (int i =0; i<40 ; i++)
    {
        cout<< "=";
    }cout<<endl;
}
void clearScreen()
{
    system("cls");
}

void trimWhitespace(char* str)
{
int len=0;
while(str[len]!='\0')
{
len = len +1;
}


while(len>0 && (str[len -1]==' '||str[len -1]=='\t'||str[len -1]=='\n'||str[len -1]=='\r'))
{
    str[len -1]='\0';
    len = len -1;
}

int start =0;
while(str[start]==' '||str[start]=='\t'||str[start]=='\n'||str[start]=='\r')
{
    start =start +1;

}
if(start >0)
    {
        int k=0;
        while(str[start+k]!='\0')
        {
            str[k]=str[start+k];
            k=k+1;
        }
        str[k]='\0';
    }
}
// Primitive helper utilities, clamping, and screen control