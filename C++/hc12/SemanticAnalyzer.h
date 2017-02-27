#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class SemanticAnalyzer{

private:
    int column=1,row=1,i;
    string **symbolTable;

    void storageSymbolTable( char* symbolTablePath )
    {
        ifstream inSymbolTableFile( symbolTablePath ); //opens the file
        int rowTemp=600;     //row = max number operations at symbol table file
        int columnTemp=7;    //column = max number operations attributes at symbol table file
        string st[rowTemp][columnTemp];
        int i,j;
        char c;

        if ( !inSymbolTableFile )
        {
            cerr << "Symbol table file could not be opened" << endl;
        }//end if
        else
        {
            //File could be opened (:
            while ( inSymbolTableFile.good() )
            {
                c=inSymbolTableFile.get();
                if (c == '|')
                {
                    column++;
                }//END IF
                else if ( c == '\n')
                {
                    column=1;
                    row++;
                }//END ELSE IF
                else
                {
                    st[row-1][column-1]+=c;
                }//END ELSE
            }//end while
            inSymbolTableFile.close();
        }//END ELSE
        /**     Create a dynamic array temp    */
        string **str= new string* [row];
        for (i=0; i < row ; i++ )
        {
            str[i]= new string[column];
        }//END FOR
        /**     symbol table array copy to dynamic array temp  */
        for (i=0; i < row ; i++ )
        {
            for (j=0; j < column ; j++)
            {
                str[i][j]=st[i][j];
            }//END FOR
        }//END FOR

        symbolTable=str;    //copy the dynamic array to the instance variable : symbolTable
    }//storageSymbolTable

public:
    SemanticAnalyzer(){};
    SemanticAnalyzer( char* symbolTablePath )
    {
        storageSymbolTable( symbolTablePath );
    }//constructor

    void initialize( char* symbolTablePath )
    {
        storageSymbolTable( symbolTablePath );
    }//initialize

    bool searchOperation( string op )
    {
        bool flag=false;
        /** string operation to upper */
        for (i=0; i < op.length(); i++)
            op[i]=toupper(op[i]);
        /** to upper finished */

        /**  Search the operation in the table   */
        for (i=0; i < row; i++)
        {
            if ( symbolTable[i][0].compare(op) == 0 )
            {
                flag=true;
                break;
            }//END IF
        }//END FOR
        return flag;
    }//searchOperation

    bool operationHasOperand ( string operation )
    {
        /** string operation to upper */
        int index;
        for (index=0; index < operation.length(); index++)
            operation[index]=toupper(operation[index]);
        /** to upper finished */

        if ( symbolTable[i][1].compare("SI") == 0)
        {
            return true;
        }//END IF
        else
        {
            return false;
        }//END ELSE
    }//validateOperationOperand

    bool validateAddressingMode( string operation, string addr )
    {
        bool flag=false;
        int j=i;
        while (symbolTable[j][0].compare(operation) == 0)
        {
            if ( symbolTable[j][2].compare(addr) )
            {
                flag=true;
                break;
            }//END IF
            j++;
        }//END WHILE
        return flag;
    }//validateAddresingMode

    void addressingModeToString( string operation )
    {
        //searchOperation(operation);
        //cout << operation << endl;
        while (symbolTable[i][0].compare(operation) == 0)
        {
            cout << "[" << symbolTable[i][2] << ",";
                   cout << symbolTable[i][3] << ",";
                   cout << symbolTable[i][4] << ",";
                   cout << symbolTable[i][5] << ",";
                   cout << symbolTable[i][6] << "]" << endl;
            i++;
        }//END WHILE
    }//getAddressingMode
};
/*
int main()
{
    SemanticAnalyzer s("Symbol table/symbolTable.txt");
    cout << s.searchOperation("org") << endl;
    cout << s.operationHasOperand("orG");
    return 0;
}//main
*/
