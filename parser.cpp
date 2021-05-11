#include <iostream>
#include <string>
#include "scanner.h"
using namespace std;

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

int main(int argc, char **argv) {     
    if (argc < 2){
        cout << "An error ocurred\n";
        cout << "Missing file path argument" << endl;
        return 0;
    }
    try {
        string filePath = argv[1];
        string input;
        generateStream(input, filePath);
        if (input.size() <= 0){
            cout << "Input file is empty\n";
            return 0;
        }
        Scanner* s = new Scanner(input);
        cout << "Impresion" << endl;
        for (int i = 0; i < s->readTokens.size(); i++){
            cout << s->readTokens.front().type << " : " << s->readTokens.front().value << endl;
            s->readTokens.pop();
        }
    } catch (std::exception& e) {
        cout << "Error: An error ocurred\n";
        cout << "Check your expression and try again\n";
    }
    return 0;
}