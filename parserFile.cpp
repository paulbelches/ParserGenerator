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
    cout << "Error expected token(s): "<< token << endl;
    //throw std::invalid_argument( "bad expresion received" );
}

void get() {
    //cout << "Entre al get" << endl;
    lastToken = nextToken;
    scanner->readTokens.pop();
    nextToken = scanner->readTokens.front();
    //cout << "token " << nextToken.value << nextToken.type << endl;
}

void verify(string type){
    /*
    cout << "Entre al verify" << endl;
    cout << "Busco " << type << endl;
    cout << "Recibi" <<nextToken.type << endl;*/
    bool doesPass = false; 
    doesPass = equal(nextToken.type.begin(), nextToken.type.end(), type.begin(), type.end());
    if (doesPass){
        get();
    } else {
        error(type);
    }
}

//INSERT METHODS

int main(int argc, char **argv)
{
    if (argc < 2){
        cout << "An error ocurred\n";
        cout << "Missing file path argument" << endl;
        return 0;
    }
    
    string filePath = argv[1];
    string input;
    generateStream(input, filePath);
    
    if (input.size() <= 0){
        cout << "Input file is empty\n";
        return 0;
    }
    cout << input << endl;
    scanner = new ScannerGrammar(input);
    /*
    while (!scanner->readTokens.empty()){
        cout << scanner->readTokens.front().value << endl;
        scanner->readTokens.pop();
    }*/
    nextToken = scanner->readTokens.front();
    //cout << "token " << nextToken.value << nextToken.type << endl;
    //INSERT FIRSTCALL
    return 0;
}