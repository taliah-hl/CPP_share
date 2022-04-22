#include <iostream>
#include<string>
#include"function.h"
using namespace std;

char dict[37]="0123456789abcdefghijklmnopqrstuvwxyz";

String_Calculator::String_Calculator(const string str){
    this->s=str;

}
String_Calculator& String_Calculator:: Add(const string str)
{
    this->s=this->s+str;
    return (*this);

}
String_Calculator& String_Calculator:: Subtract(const string str)
{
    int len=str.size();
    int found=this->s.find(str);
    if(found >=0 && found < this->s.size())
    {
        this->s.erase(found, len);
    }
    else{
        this->s="error";
    }
    //cout << "s is: " <<this->s <<endl;
    return (*this);
}

String_Calculator& String_Calculator:: Shift(const string str)
{
    
    int num=0;
    char c='\0';
    int len_b=str.size();
    for(int i=0;i<len_b ;i++)
    {
        //cout << "for...\n";
        c=str[i];
        //cout << c <<endl;
        if(c >='0' && c<='9'){
            num=num*10+ (c-'0');
        }
        else{
            this->s="error";
            return (*this);
        }
    }
    //cout << "num is:" << num <<endl;
    int len_a=this->s.size();
    int idx[100]={0};
    for(int i=0;i<len_a;i++)
    {
        if(this->s[i] >='0' && this->s[i]<='9'){
            idx[i]=this->s[i]-'0';
        }
        else if(this->s[i] >='a' && this->s[i]<='z')
        {
            idx[i]=this->s[i]-'a'+10;
        }
        else{

            break;
        }

    }
    char newc[101]={'\0'};
    for(int i=0;i<len_a;i++)
    {

        this->s[i]=dict[(idx[i]+num)%36];

    }

    return (*this);


}

void String_Calculator:: output() const
{  
    cout << this->s << endl;

}
