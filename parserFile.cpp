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

//INSERT METHODS

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
    //INSERT FIRSTCALL
    return 0;
}