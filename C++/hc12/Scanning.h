#include <iostream>
#include <fstream>
#include <string>
using namespace std;
const int a=97 ,z=122 ,A=65 ,Z=90 ,ZERO=48 ,NINE=57; //ASCII VALUES

class Scanning
{
private:
    string sourceCode;
    string label="",operation="",operand="";
    
    const int ERR=-1;
    void labelWritingRules( string lab );
    void operationWritingRules( string ope );
    void operandWritingRules( string ope );

public:
    Scanning(){};
    Scanning( char* filePath );
    void initialize( char* filePath );
    void setSourceCode( char* filePath );
    
    string getSourceCode(){return sourceCode;}//getSourceCode
    string getLabel(){return label;};
    string getOperation(){return operation;};
    string getOperand(){return operand;};

    void nextTokens( string line );
};//class

Scanning::Scanning( char* filePath )
{
    setSourceCode(filePath);
}//constructor

void Scanning::initialize( char* filePath)
{
	setSourceCode(filePath);
}//initialize

void Scanning::setSourceCode( char* filePath )
{
    ifstream inSourceCodeFile( filePath ); //opens the file
    if ( !inSourceCodeFile )
    {
        cerr << "File could not be opened" << endl;

    }//end if
    else
    {
        //File could be opened (:
        sourceCode += inSourceCodeFile.get();
        while ( inSourceCodeFile.good() )
        {
            sourceCode+= inSourceCodeFile.get();
        }//end while
        inSourceCodeFile.close();
    }//END ELSE
}//setSourceCode

void Scanning::nextTokens( string line )
{
    int lineLength=line.length();
    label="";
    operation="";
    operand="";
    char c;
    int i=0;
    c=line[i++];
    if ( c == ' ' || c == '\t')
    {
        //ignore blanks until it finds something else
        do
        {
            c=line[i++];
        }while( (c == ' ' || c == '\t') && i < lineLength);
        //blanks ignored
        if ( i < lineLength) /** if there's more characters at the line */
        {
            if ( c == ';' )/** is a COMMENT */
            {
                cout << "COMMENT" << endl;
            }//end if
            else if (c != ';' )/** is a OPERATION */
            {
                /** Storage into operation */
                do
                {
                    operation+=c;
                    c=line[i++];
                }while( (c != ' ' && c != '\t') && i <= lineLength);
                operationWritingRules(operation);
                /** operation storaged */
                if ( i <= lineLength) /** if there's more characters at the line */
                {
                    //ignore blanks
                    while ( c == ' ' || c == '\t')
                    {
                        c=line[i++];
                    }//end while
                    //blanks ignored
                    if ( c == ';' )/** is a COMMENT */
                    {
                        cout << "COMMENT" << endl;
                    }//end if
                    else if (c != ';' )/** is an OPERAND */
                    {
                        /** Storage into operand */
                        do
                        {
                            operand+=c;
                            c=line[i++];
                        }while( i <= lineLength);
                        operandWritingRules(operand);
                        /** operand storaged */
                    }//end else if
                }//end if
            }//end else if
        }//end if
    }//end if
    else if ( c == ';' )/** is a COMMENT */
    {
        cout << "COMMENT" << endl;
    }//end else if
    else /** is a LABEL */
    {
        /** Storage label */
        do
        {
            label+=c;
            c=line[i++];
        }while( (c != ' ' && c != '\t') && i <= lineLength);
        labelWritingRules(label);
        /** label storaged */
        if ( i <= lineLength) /** if there's more characters at the line */
        {
            //ignore blanks
            while ( c == ' ' || c == '\t')
            {
                c=line[i++];
            }//end while
            //blanks ignored
            if ( c == ';' )/** is a COMMENT */
            {
                cout << "COMMENT" << endl;
            }//end if
            else if (c != ';' )/** is an OPERATION */
            {
                /** Storage into operation */
                do
                {
                    operation+=c;
                    c=line[i++];
                }while( (c != ' ' && c != '\t') && i <= lineLength);
                operationWritingRules(operation);
                /** operation storaged */
                if ( i <= lineLength) /** if there's more characters at the line */
                {
                    //ignore blanks
                    while ( c == ' ' || c == '\t')
                    {
                        c=line[i++];
                    }//end while
                    //blanks ignored
                    if ( c == ';' )/** is a COMMENT */
                    {
                        cout << "COMMENT" << endl;
                    }//end if
                    else if (c != ';' )/** is an OPERAND */
                    {
                        /** Storage into operand */
                        do
                        {
                            operand+=c;
                            c=line[i++];
                        }while( i <= lineLength );
                        operandWritingRules(operand);
                        /** operand storaged */
                    }//end else if
                }//end if
            }//end else if
        }//end if
    }//end else
}//nextToken

void Scanning::labelWritingRules( string lab )
{
     cout << "LABEL: " << lab << endl;
    int n=lab.length();
    if ( n <= 8)
    {
        int i=0;
        int state=0;
        while ( i < n && state != ERR)
        {
            switch(state)
            {
            case 0:
                if (!( lab[i] >= a && lab[i] <= z
                    || lab[i] >= A && lab[i] <= Z))
                {
                    state=ERR;
                }//end if
                else
                {
                    state=1;
                }//end else
                break;

            case 1:
                if (!( lab[i] >= a && lab[i] <= z
                    || lab[i] >= A && lab[i] <= Z
                    || lab[i] >= ZERO && lab[i] <= NINE
                    || lab[i] == '_'))
                {
                    state=ERR;
                }//end if
                break;
            }//switch
            i++;
        }//end while
        if ( state == ERR )
        {
            cout << "\tERROR: LABEL WRITING RULES: [a-zA-Z][a-zA-Z0-9_]*" << endl;
        }//end if
    }//end if
    else
    {
        cout << "\tERROR: LABEL MAXIMUM LENGTH = 8" << endl;
    }//end else
}//labelWritingRules

void Scanning::operationWritingRules( string ope )
{
     int n=ope.length();
    cout << "OPERATION: " << ope << endl;
    if ( n <= 5)
    {
        int i=0;
        int state=0;
        while ( i < n && state != ERR)
        {
            switch(state)
            {
            case 0:
                if (!( ope[i] >= a && ope[i] <= z
                    || ope[i] >= A && ope[i] <= Z))
                {
                    state=ERR;
                }//end if
                else
                {
                    state=1;
                }//end else
                break;

            case 1:
                if ( ope[i] == '.')
                {
                    state=2;
                }//end if
                else if (!( ope[i] >= a && ope[i] <= z
                         || ope[i] >= A && ope[i] <= Z))
                {
                    state=ERR;
                }//end else if
                break;

            case 2:
                if (!( ope[i] >= a && ope[i] <= z
                    || ope[i] >= A && ope[i] <= Z))
                {
                    state=ERR;
                }//end if
                break;
            }//switch
            i++;
        }//end while
        if ( state == ERR )
        {
            cout << "\tERROR: OPERATION WRITING RULES: [a-zA-Z][a-zA-Z]*\\\\.[a-zA-Z]*" << endl;
        }//end if
    }//end if
    else
    {
        cout << "\tERROR: OPERATION MAXIMUM LENGTH = 5" << endl;
    }//end else
}//operationWritingRules

void Scanning::operandWritingRules( string ope )
{
    cout << "OPERAND: " << ope << endl;
}//operandWritingRules
