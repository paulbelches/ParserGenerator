#include <iostream>
#include <string>
#include "methods.h"
#include "scanner.h"
#include "cocol.h"
#include <algorithm>
using namespace std;

/*
   cout << "Compiling file" << endl;
   string command = "c++ parser.cpp -o parser.run";
   system(command.c_str());
/*---------------------------------------------------------------------
 * Function:      generateStream
 * Purpose:       Read input file
 * In arg:        file the pointer where the file is going to be saved      filepath the file path
 * Return val:    Resulting state
 */
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

string printVectorToken(vector<token> vector){
    for (int i = 0; i < vector.size(); i++){
        cout << vector[i].value << endl;
    }
}
///Generar un escanner para leer coco/R
//Leer las producciones , y mapearlas o algo
//Generar un escanner para leer la gramatica ingresada
//---------Parsear

int main(int argc, char **argv) {   
    /*  
    if (argc < 2){
        cout << "An error ocurred\n";
        cout << "Missing file path argument" << endl;
        return 0;
    }*/
    map<string, vector<token>> productionsMap;
    try {
        string filePath = argv[1];
        string input;
        generateStream(input, filePath);
        if (input.size() <= 0){
            cout << "Input file is empty\n";
            return 0;
        }
        cout << input << endl;
        ScannerCocol* cocolS = new ScannerCocol(input);
        //ScannerGrammar* s; //= new Scanner(input);
        cout << "Impresion" << endl;
        vector<token> production;
        string id = "";
        map<string, vector<token>> productionsMap;
        while (!cocolS->readTokens.empty()){
            if (id.size() == 0) {
                if (equal(cocolS->readTokens.front().type.begin(), cocolS->readTokens.front().type.end(), "ident")){
                    id = cocolS->readTokens.front().value;
                    cout <<  cocolS->readTokens.front().type << " : " <<  cocolS->readTokens.front().value << endl;
                } else {
                    cout << "error" << endl;
                    return 0;
                }
            } else {
                if ( true /*
                    !equal(cocolS->readTokens.front().type.begin(), cocolS->readTokens.front().type.end(), "whiteToken")
                    &&
                    !equal(cocolS->readTokens.front().type.begin(), cocolS->readTokens.front().type.end(), "attr")
                    &&
                    !equal(cocolS->readTokens.front().type.begin(), cocolS->readTokens.front().type.end(), "s_action")*/
                ){
                    cout <<  cocolS->readTokens.front().type << " : " <<  cocolS->readTokens.front().value << endl;
                    production.push_back(cocolS->readTokens.front());
                }
            }
            //cout <<  cocolS->readTokens.front().type << " : " <<  cocolS->readTokens.front().value << endl;
            production.push_back(cocolS->readTokens.front());
            if (equal(cocolS->readTokens.front().type.begin(), cocolS->readTokens.front().type.end(), "p_end")){
                cout << endl;
                productionsMap[id] = production;
                production.clear();
                id = "";
            }
            cocolS->readTokens.pop();
        }

        // Create a map iterator and point to beginning of map
        map<string, vector<token>>::iterator it = productionsMap.begin();
        // Iterate over the map using Iterator till end.
        /*
        while (it != productionsMap.end())
        {
            // Accessing KEY from element pointed by it.
            std::string word = it->first;;
            cout << word << endl;// printVectorToken(it->second);
            // Increment the Iterator to point to next entry
            it++;
        }*/


    } catch (std::exception& e) {
        cout << "Error: An error ocurred\n";
        cout << "Check your expression and try again\n";
    }
    return 0;
}