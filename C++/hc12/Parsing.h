#include <iostream>
#include <string>
#include "Token.h"
#include "StringTokenizer.h"
#include "Scanning.h"
#include "SemanticAnalyzer.h"
using namespace std;

class Parsing{
private:
    Scanning s;
    StringTokenizer st;
    Token label;
    Token operation;
    Token operand;
    SemanticAnalyzer sa;

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
        sa.initialize("Symbol table/symbolTable.txt");
    }//Parsing

    void initialize(char* filePath)
    {
        s.initialize(filePath);
        st.initialize(s.getSourceCode(),"\n");
        sa.initialize("Symbol table/symbolTable.txt");
    }//initialize

    void parse()
    {
        bool foundEnd=false;
		if (st.hasMoreTokens())
		{
			while (!foundEnd && st.hasMoreTokens())
			{
				s.nextTokens(st.nextToken());       //get the next line
				if (!( iStringComparison(s.getLabel(), "")      //if is not a comment
					&& iStringComparison(s.getOperation(),"")   //if is not a comment
					&& iStringComparison(s.getOperand(), "")))  //if is not a comment
				{
					label.initialize(LABEL, s.getLabel());
					operation.initialize(OPERATION, s.getOperation());
					operand.initialize(OPERAND, s.getOperand());
					if (iStringComparison(operation.getAttributeValue(), ""))   //must have an operation
					{
						cout << "\tSYNTAX ERROR: MUST HAVE TOKEN: <" << operation.getTokenName()
							<<",attribute value>" <<endl;
					}//END IF
					else    //if its syntax is correct..
					{
						if (sa.searchOperation(operation.getAttributeValue()))
						{
							if (sa.operationHasOperand(operation.getAttributeValue()))
							{
								if (iStringComparison(operand.getAttributeValue(),""))
								{
									cout << "\tERROR: MUST HAVE OPERAND" << endl;
								}//END IF
							}//END IF
							else    //if doesn't has operand
							{
								if (!iStringComparison(operand.getAttributeValue(),""))
								{
									cout << "\tERROR: MUST NOT HAVE OPERAND" << endl;
								}//END IF
							}//END ELSE

							sa.addressingModeToString(operand.getAttributeValue());
						}//END IF
						else
						{
							cout << "\tERROR: OPERATION DOES NOT FOUND" << endl;
						}//END ELSE
					}//END ELSE
				}//end if
				foundEnd=iStringComparison(operation.getAttributeValue(), "END");
			}//END WHILE

			if (!foundEnd)
			{
				cout << "\tSYNTAX ERROR: MUST HAVE TOKEN: <" << operation.getTokenName()
					 <<",END>" << endl;
			}//end if
		}//END IF
        
    }//readLine
};
