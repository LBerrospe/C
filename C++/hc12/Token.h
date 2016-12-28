#include <string>
using namespace std;

const string LABEL="LABEL", OPERATION="OPERATION", OPERAND="OPERAND";

class Token
{
public:
	Token(){}

    Token ( string token, string value)
    {
        setTokenName(token);
        setAttributeValue(value);
    }//Constructor
	
	void initialize( string token, string value)
    {
        setTokenName(token);
        setAttributeValue(value);
    }//initialize

    void setTokenName( string token )
    {
        tokenName=token;
    }//setTokenName

    string getTokenName()
    {
        return tokenName;
    }//getTokenName

    void setAttributeValue( string value )
    {
        attributeValue=value;
    }//setAttributeValue

    string getAttributeValue()
    {
        return attributeValue;
    }//getAttribute Value

private:
    string tokenName;
    string attributeValue;
};//class
