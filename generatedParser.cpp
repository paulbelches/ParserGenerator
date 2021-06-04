#include <iostream>
#include <stdlib.h>
#include "methods.h"
#include "scanner.h"


token lastToken; 
token nextToken; 
ScannerGrammar* scanner;

bool isString(string value){
    return value[0] == '"' && value[value.size() - 1] == '"';
}

int generateStream(string& file, string filePath){

    string filename(filePath);
    vector<char> bytes;
    char byte = 0;

    ifstream input_file(filename);
    if (!input_file.is_open()) {
        throw std::invalid_argument( "Could not open input file");
    }
    while (input_file.get(byte)) {
        bytes.push_back(byte);
    }
    for (const auto &i : bytes) {
        file = file + i;
        //cout << i << " " << (int)i << " " << isspace(i) << endl;
    }
    input_file.close();

    return 1;
}

void error(string token){
    cout << "Error expected "<< token << endl;
    throw std::invalid_argument( "bad expresion received" );
}

void get() {
    lastToken = nextToken;
    nextToken = scanner->readTokens.front();
    scanner->readTokens.pop();
}

void verify(string value){
    bool doesPass = false; 
    if (isString(value)) {
        doesPass = nextToken.value == value;
    } else {
        doesPass = nextToken.value == value;
    }
    if (doesPass){
        get();
    } else {
        error();
    }
}

void Expr();
void Stat();
void Expression(double& result);
void Term(double& result);
void Factor(double& result);
void Number(double& result);

void Number(double& result)
{
if ( equal(nextToken.type.begin(), nextToken.type.end(), "number") ){ 
verify("number");} 
if ( equal(nextToken.type.begin(), nextToken.type.end(), "decnumber") ){ 
verify("decnumber");} else { error();
}result = atof(lastToken.value.c_str());
}

void Factor(double& result)
{
double sign=1;
if ( equal(nextToken.type.begin(), nextToken.type.end(), "tonken-") ){ 
verify("tonken-");
sign = -1;

}if ( equal(nextToken.type.begin(), nextToken.type.end(), "decnumber") ||equal(nextToken.type.begin(), nextToken.type.end(), "number") ){ 
Number(result);
} 
if ( equal(nextToken.type.begin(), nextToken.type.end(), "tonken(") ){ 
verify("tonken(");
Expression(result);
verify("tonken)");
} else { error();
}result*=sign;
}

void Term(double& result)
{
double result1=0;double result2=0;
Factor(result1);
while( equal(nextToken.type.begin(), nextToken.type.end(), "tonken*") ||equal(nextToken.type.begin(), nextToken.type.end(), "tonken/") ){ 
if ( equal(nextToken.type.begin(), nextToken.type.end(), "tonken*") ){ 
verify("tonken*");
Factor(result2);
result1*=result2;
} 
if ( equal(nextToken.type.begin(), nextToken.type.end(), "tonken/") ){ 
verify("tonken/");
Factor(result2);
result1/=result2;
} else { error();
}}
result=result1;
}

void Expression(double& result)
{
double result1=0;double result2=0;
Term(result1);
while( equal(nextToken.type.begin(), nextToken.type.end(), "tonken+") ||equal(nextToken.type.begin(), nextToken.type.end(), "tonken-") ){ 
if ( equal(nextToken.type.begin(), nextToken.type.end(), "tonken+") ){ 
verify("tonken+");
Term(result2);
result1+=result2;
} 
if ( equal(nextToken.type.begin(), nextToken.type.end(), "tonken-") ){ 
verify("tonken-");
Term(result2);
result1-=result2;
} else { error();
}}
result=result1;
}

void Stat()
{
double value=0;
Expression(value);
cout << "Resultado: "<< value;
}

void Expr()
{
while( equal(nextToken.type.begin(), nextToken.type.end(), "decnumber") ||equal(nextToken.type.begin(), nextToken.type.end(), "number") ||equal(nextToken.type.begin(), nextToken.type.end(), "tonken(") ||equal(nextToken.type.begin(), nextToken.type.end(), "tonken-") ){ 
Stat();
verify("tonken;");
while( equal(nextToken.type.begin(), nextToken.type.end(), "white") ){ 
verify("white");}
}
while( equal(nextToken.type.begin(), nextToken.type.end(), "white") ){ 
verify("white");}
verify("tonken.");
}



int main(int argc, char **argv)
{
    //string filePath = argv[1];
    string input;
    generateStream(input, "input.txt");
    if (input.size() <= 0){
        cout << "Input file is empty\n";
        return 0;
    }
    cout << input << endl;
    scanner = new ScannerGrammar(input);
Expr();
    return 0;
}

