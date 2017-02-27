#include <string>
using namespace std;

class StringTokenizer
{
private:
    string str;
    string delim;
    int strLength;
    int index=0;
    const int ACP=100;

    bool charEqualsDelim( char c, string d)
    {
        int n = d.length();
        int i;
        bool flag=false;
        for (i=0;i < n; i++)
        {
            if ( c == d[i])
            {
                flag=true;
                break;
            }//end if
        }//end for
        return flag;
    }//charEqualsDelim

public:
	StringTokenizer(){};

    StringTokenizer( string s , string d )
    {
        str=s;
        delim=d;
        strLength=str.length()-1; //avoid the end of string
    }//constructor
	
	void initialize( string s , string d )
    {
        str=s;
        delim=d;
        strLength=str.length()-1; //avoid the end of string
    }//initialize

    bool hasMoreTokens()
    {
        int i;
        bool flag=false;
        for (i=index ; i < strLength; i++)
        {
            if ( !charEqualsDelim(str[index], delim) )
            {
                flag=true;
                break;
            }//end if
        }//for
        return flag;
    }//hasMoreTokens

    string nextToken()
    {
        string strn="";
        int state=0;
        while ( state != ACP  && index < strLength)
        {
            if ( charEqualsDelim(str[index], delim) )
            {
                if ( state != 0 )
                {
                    state=ACP;
                }//end if
            }//end if
            else
            {
                state=1;
                strn+=str[index];
            }//end else
            index++;
        }//end while
        return strn;
    }//nextToken
};//class
