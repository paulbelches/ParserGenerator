#include <iostream>
#include <stdlib.h>
using namespace std;
/*
c++ parser.cpp -o main.run
*/
int main(int argc, char **argv) {    
    if (argc < 2){
        cout << "An error ocurred\n";
        cout << "Missing file path argument" << endl;
        return 0;
    }

    string filePath = argv[1];
    //Create CoCol scanner
    string command = " c++ fileReader.cpp -o fileReader.run";
    system(command.c_str());
    command = "./fileReader.run CoCoL.ATG cocol.h Cocol";
    system(command.c_str());
    command = "./fileReader.run " + filePath + " scanner.h Grammar";
    system(command.c_str());
    command = " c++ parserMain.cpp -o parserMain.run";
    system(command.c_str());
    command = "./parserMain.run productions.txt";
    system(command.c_str());

    return 0;
}