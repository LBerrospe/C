#include <iostream>
#include <string>
#include "Token.h"
#include "StringTokenizer.h"
#include "Scanning.h"
using namespace std;

class Parsing{
private:
    Scanning s;
    StringTokenizer st;
    Token label;
    Token operation;
    Token operand;

    bool iStringComparison( string str1, string str2 )
    {
        bool flag=true;
        if ( str1.length() == str2.length())
        {
            int i;
            for (i=0; i<str1.length(); i++)
            {
                if ( toupper(str1[i]) != toupper(str2[i]) )
                {
                    flag=false;
                    break;
                }//end if
            }//enf or
        }//end if
        else
        {
            flag=false;
        }//end else if
        return flag;
    }//iStrCmp

public:
    Parsing(){}//Parsing

    Parsing(char* filePath)
    {
        s.initialize(filePath);
        st.initialize(s.getSourceCode(),"\n");
    }//Parsing

    void initialize(char* filePath)
    {
        s.initialize(filePath);
        st.initialize(s.getSourceCode(),"\n");
    }//initialize

    void parse()
    {
        bool foundEnd=false;
        do
        {
            s.nextTokens(st.nextToken());
            if (!( iStringComparison(s.getLabel(), "")
                && iStringComparison(s.getOperation(),"")
                && iStringComparison(s.getOperand(), "")))
            {
                label.initialize(LABEL, s.getLabel());
                operation.initialize(OPERATION, s.getOperation());
                operand.initialize(OPERAND, s.getOperand());
                if (iStringComparison(operation.getAttributeValue(), ""))
                {
                    cout << "\tSYNTAX ERROR: MUST HAVE TOKEN: <" << operation.getTokenName()
                        <<",attribute value>" <<endl;
                }//end if
            }//end if
            foundEnd=iStringComparison(operation.getAttributeValue(), "END");
            cout << endl;
        }while (!foundEnd && st.hasMoreTokens());
        if (!foundEnd)
        {
            cout << "\tSYNTAX ERROR: MUST HAVE TOKEN: <" << operation.getTokenName()
                 <<",END>" << endl;
        }//end if
    }//readLine
};
