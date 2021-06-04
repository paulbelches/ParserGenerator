// reading a text file
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <cctype>
#include <algorithm>
#include <map>
#include <stdlib.h>
using namespace std;

/* File:     
 *     main.cpp
 *
 * Purpose:
 *
 * Input:
 *
 * Output:
 *     
 * Compile:  
 *    c++ fileReader.cpp -o main.run
 * 
 * Usage:
 *   main.run <filepath>
 *
 */


const string flag = "        //INSERT EXPRESSIONS";
const string nameFlag = "//INSERT NAME";
const string constructorFlag = "    //INSERT CONSTRUCTOR";
const string keywords[] = {"COMPILER", "CHARACTERS", "KEYWORDS", "TOKENS", "IGNORE", "PRODUCTIONS", "END "};
const string keywordsFlag = {"EXCEPT KEYWORDS"} ;
const string tokenOperator = "()|*+?";
/*---------------------------------------------------------------------
 * Function:      isKeyWord
 * Purpose:       Check whether a input is a keyword
 * In arg:        line,the input line   cont,the current position of the iterator   keyword,array of keywords
 * Return val:    if its is a keyword or not
 */
int isKeyWord(string line, int cont, const string keywords[]){
  for (int i = 0; i < 7; i++){
    if (line.substr(cont,keywords[i].size()).compare(keywords[i]) == 0){
      return i;
    }
  }
  return -1;
}
/*---------------------------------------------------------------------
 * Function:      isString
 * Purpose:       Check whether a input is a string
 * In arg:        line,the input line  
 * Return val:    if its is a string or not
 */
bool isString(string line){
  return (line[0] == '"'  && line[line.size() - 1] == '"');
}
/*---------------------------------------------------------------------
 * Function:      isChar
 * Purpose:       Check whether a input is a char
 * In arg:        line,the input line  
 * Return val:    if its is a char or not
 */
bool isChar(string line){
  return (line[0] == '\''  && line[line.size() - 1] == '\'');
}
/*---------------------------------------------------------------------
 * Function:      sustituteRange
 * Purpose:       sustitute a range into the list of characters it represents
 * In arg:        line,the input line  
 * Return val:    sustituted line
 */
string sustituteRange(string line){
  size_t firstPosition = line.find("$");
  if (firstPosition != string::npos) {
    int num1 = std::stoi(line.substr(1, firstPosition - 2));
    int num2 = std::stoi(line.substr(firstPosition + 2, line.size() - firstPosition - 3));
    line = "";
    for (int i = num1; i < num2 + 1; i++ ){
        line = line + "'" + to_string(i) + "'";
      }
  }
  return line;
}
/*---------------------------------------------------------------------
 * Function:      replaceString
 * Purpose:       sustitute a string patron , for a new patron taking in consideration inside string and chars or not
 * In arg:        line,the input line  patron,the patron that is goig to be recognize newPatron,the patron that is going to sustituted  considerChain, whether to check inside strings or not
 * Return val:    sustituted line
 */
string replaceString(string line, string patron, string newPatron, bool considerChains){
  bool readingString = false;
  bool readingChar = false;
  string result = "";
  int i = 0;
  while (i < line.size()){
    if (readingString){
      if (line[i] == '"'){
        readingString = false;
      }
      result = result + line[i];
    } else if (readingChar){
      if (line[i] == '\''){
        readingChar= false;
      }
      result = result + line[i];
    } else if ((line.size() - i >= patron.size() ) && line.substr(i,patron.size()).compare(patron) == 0){
      result = result + newPatron;
      i = i + patron.size() - 1;
    }
    else if (line[i] == '"' && !considerChains){
      readingString = true;
      result = result + line[i];
    }
    else if (line[i] == '\'' && !considerChains){
      readingChar = true;
      result = result + line[i];
    } else {
      result = result + line[i];
    }
      
    i++;
  }/*
  cout << "//////////////////////////////////////////////////////////////" << patron << endl;
  cout << "result?" << result << endl;*/
  return result;
}

/*---------------------------------------------------------------------
 * Function:      passToParentesis
 * Purpose:       pass from quotes to parentesis
 * In arg:        line,the input line
 * Return val:    sustituted line
 */
string passToParentesis(string line){
  string result = "";
  int p = 0;
  for (int i = 0; i < line.size(); i++){
    if (line[i] == '\''){
      if (p % 2 == 0){
        result = result + "(";
      } else {
        result = result + ")";
      }
      p++;
    } else {
      result = result + line[i];
    }
  }
  return result;
}

/*---------------------------------------------------------------------
 * Function:      sustituteChar
 * Purpose:       Remove all  "CHR()" from a input line
 * In arg:        line,the input line
 * Return val:    sustituted line
 */
string sustituteChar(string line){
  size_t firstPosition = line.find("CHR(");
  while (firstPosition != string::npos) {
    size_t lastPosition = line.find(")");
    string snum = line.substr(firstPosition + 4 , lastPosition - (firstPosition + 4));
    line = line.substr(0, firstPosition) + "'" + snum + "'" + line.substr(lastPosition + 1, line.size() - lastPosition + 1 );
    firstPosition = line.find("CHR(");
  }
  return line;
}

/*---------------------------------------------------------------------
 * Function:      lettersToNums
 * Purpose:       Pass strings and chars for its numerical representations
 * In arg:        line the input line
 * Return val:    sustituted line
 */
string lettersToNums(string line){
  bool readingString = false;
  bool readingChar = false;
  string result = "";
  int i = 0;
  /*
  cout << "letters to numbers" << endl;
  cout << "line: " << line << endl;*/
  while (i < line.size()){
    if (readingString){
      if (line[i] == '"'){
        readingString = false;
      } else {
        //cout << to_string( (int)line[i] ) << endl;
        result = result + '\'' + to_string( (int)line[i] ) + '\'';
      }
    } else if (readingChar){
      if (line[i] == '\''){
        readingChar= false;
      } else {
        //cout << to_string( (int)line[i] ) << endl;
        result = result + '\'' + to_string( (int)line[i] ) + '\'';
      }
    } else if (line[i] == '"'){
      readingString = true;
    }
    else if (line[i] == '\''){
      readingChar = true;
    } else {
      result = result + line[i];
    }
    i++;
  }
  return result;
}

/*---------------------------------------------------------------------
 * Function:      checkIfItsAId
 * Purpose:       Check if the input is a valid id
 * In arg:        line,the input line  references,the reference table
 * Return val:    sustituted line
 */
string checkIfItsAId(string line, map<string, string>& references){
  if (!isChar(line)){
    if (references[line].size() > 0){
      line = references[line];
    } else {
      throw std::invalid_argument( "Reference to nonexistent character set "+line);
    }
  }
  return line;
}

/*---------------------------------------------------------------------
 * Function:      join
 * Purpose:       join two sets
 * In arg:        s1,set 1  s2,set 2  references,the reference table
 * Return val:    sustituted line
 */
string join(string s1, string s2, map<string, string>& references){
  //cout << "sum s1:" << s1 << " s2:" << s2 <<  " result:";
  s1 = checkIfItsAId(s1,references);
  s2 = checkIfItsAId(s2,references);
  string result;
  bool readingNumber = false;
  string number;
  for (int i = 0; i < s2.size(); i++){
    if (s2[i] == '\'') {
      if (readingNumber){
        readingNumber = false;
        if (s1.find(number) == string::npos){
          s1 = s1 + "'" + number + "'";
        }
        number = "";
      } else {
        readingNumber = true;
      }
    } else {
      number = number + s2[i];
    }
  }
  //cout << s1 << endl;
  return s1;
}

/*---------------------------------------------------------------------
 * Function:      remove
 * Purpose:       remove one set from another set
 * In arg:        s1,set 1  s2,set 2  references,the reference table
 * Return val:    sustituted line
 */
 string remove(string s1, string s2, map<string, string>& references){
  //cout << "res s1:" << s1 << " s2:" << s2 <<  " result:";
  s1 = checkIfItsAId(s1,references);
  s2 = checkIfItsAId(s2,references);
  string result;
  bool readingNumber = false;
  string number;
  for (int i = 0; i < s2.size(); i++){
    if (s2[i] == '\'') {
      if (readingNumber){
        readingNumber = false;
        s1 = replaceString(s1, '\'' + number + '\'', "", true);
        number = "";
      } else {
        readingNumber = true;
      }
    } else {
      number = number + s2[i];
    }
  }
  //cout << s1 << endl;
  return s1;
}

/*---------------------------------------------------------------------
 * Function:      format
 * Purpose:       format the strings into the numeric form
 * In arg:        term , the input string
 * Return val:    sustituted line
 */
string format(string term){
  return sustituteRange(sustituteChar(lettersToNums(term)));
}

/*---------------------------------------------------------------------
 * Function:      replaceTokens
 * Purpose:       Replace tokens for its set
 * In arg:        line, input line   map, reference map
 * Return val:    sustituted line
 */
string replaceTokens(string line, map<string,string>& map){
  string acum = "";
  string result = "";
  for (int i = 0; i < line.size(); i++){
    if (tokenOperator.find(line[i]) == string::npos && (!isspace(line[i]))){
      acum = acum + line[i];
      //cout << acum<< endl;
    } else {
      //cout << "|" << acum << "|" << endl;
      //check the acum content
      if (acum.size() > 0){
        if (map[acum].size() > 0){
          result = result  + map[acum];
        } else {
          if (isalpha(acum[0])){
            throw std::invalid_argument( "Reference to nonexistent character set "+acum );
          }
          result = result  + acum;
        }
        acum = "";
      }
      //operators
      result = result + line[i];
    }
  }
  if (acum.size() > 0){
    if (map[acum].size() > 0){
      result = result  + map[acum];
    } else {
      result = result  + acum;
    }
  }
  return result;
}

/*---------------------------------------------------------------------
 * Function:      passToOrs
 * Purpose:       Replace quotes to or operators
 * In arg:        savedCharacters, reference map
 * Return val:    -------------
 */
void passToOrs(map<string,string>& savedCharacters){
  for(auto it = savedCharacters.cbegin(); it != savedCharacters.cend(); ++it){
    savedCharacters[it->first] = "(" + replaceString(it->second.substr(1, it->second.size() - 2), "''", "|", true ) + ")";
  }
};

/*---------------------------------------------------------------------
 * Function:      replace
 * Purpose:       Call all token replacements
 * In arg:        savedCharacters, saved characters  savedTokens, saved tokens  tokenIds, the vector of the input tokens
 * Return val:    -------------
 */
void replace(map<string,string>& savedCharacters, map<string,string>& savedTokens, vector<string>& tokenIds){
  for(int i = 0; i < tokenIds.size(); i++){
    bool isReady = false;
    while (!isReady) {
      string resultString = replaceTokens(savedTokens[tokenIds[i]], savedCharacters);
      isReady = equal( resultString.begin(), resultString.end(), savedTokens[tokenIds[i]].begin(), savedTokens[tokenIds[i]].end());
      if (!isReady){
        savedTokens[tokenIds[i]] = replaceString(resultString, " ", "", false );
      }
    }
  }
}
/*---------------------------------------------------------------------
 * Function:      generateStream
 * Purpose:       Read input file
 * In arg:        file, the pointer where the file is going to be saved      filepath, the file path
 * Return val:    Resulting state
 */
int generateStream(string& file, string filePath){
    string filename(filePath);
    vector<char> bytes;
    char byte = 0;

    ifstream input_file(filename);
    if (!input_file.is_open()) {
        throw std::invalid_argument( "Could not open file");
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

/*---------------------------------------------------------------------
 * Function:      fillmaps
 * Purpose:       Fill maps with the input from the file
 * In arg:        filePath, fiel path to atg file savedKeywords, map of keywords  savedCharacters, map of the input charcter sets
 *                savedTokens, the saved tokens map  whiteSpaces, the whitespaces vector  exceptTokens, the excepted tokens  tokenIds, the token id vector
 * Return val:    -------------
 */
void fillmaps(string filePath, map<string,string>& savedKeywords, map<string,string>& savedCharacters, map<string,string>& savedTokens, 
  vector<string>& whiteSpaces, map<string,bool>& exceptTokens, vector<string>& tokenIds){
  string line;
  
  int currentKeyword = 0;
  bool readingString = false;
  bool readingChar = false;
  bool range = true;
  bool exceptKeyWordsFlag = false;

  vector<string> terms;
  vector<char> operands;

  generateStream(line, filePath);
  line = replaceString(line, "..", "$", false);
  int cont = 0;
  string acum = "";
  string tempFile = "";
  while (line.size() > cont){
  // check for key word
    int tempCurrentKeyword = isKeyWord(line, cont, keywords);
    if (tempCurrentKeyword > 0){
      //check if the temp is lower than the actual, if that is the case, return errror
      currentKeyword = tempCurrentKeyword;
      //cout << currentKeyword << endl;
      cont = cont + keywords[currentKeyword].size() - 1;
      terms.clear();
      operands.clear();
      acum= "";
    } else if (currentKeyword == 5){
      //check if the temp is lower than the actual, if that is the case, return errror;
      //cout <<line[cont]<< endl;
      tempFile = tempFile + line[cont];
    } else if (currentKeyword == 6){
      //check if the temp is lower than the actual, if that is the case, return errror;
      if (tempFile.size() > 0){
        ofstream newfile("productions.txt");
        newfile << tempFile << endl;
        newfile.close();
        tempFile = "";
      }
    } else if (readingChar && line[cont] != '\''){
      acum = acum + line[cont];
    } else if (readingString && line[cont] != '"'){
      acum = acum + line[cont];
    } else if (line.substr(cont,keywordsFlag.size()).compare(keywordsFlag) == 0){
      if (acum.size() > 0){
        if (currentKeyword == 1 ||currentKeyword == 4){
          acum = format(acum);
        }
        terms.push_back(acum);
        acum = "";
      }
      exceptKeyWordsFlag = true;
      cont = cont + keywordsFlag.size() - 1;
    } else if (line[cont] == '\''){
      if (readingChar){
        readingChar = false;
      } else {
        readingChar = true;
      }
      acum = acum + line[cont];
    } else if (line[cont] == '"'){
      if (readingString){
        readingString = false;
      } else {
        readingString = true;
      }
      acum = acum + line[cont];
    } else if (line[cont] == '='){
      if (acum.size() > 0){
        if (currentKeyword == 1 ||currentKeyword == 4){
          acum = format(acum);
        }
        if (terms.size() > 0){
          throw std::invalid_argument( "Bad declaration");
        }
        terms.push_back(acum);
        acum = "";
      } else {
        throw std::invalid_argument( "Missing identifier");
      }
    } else if (line[cont] == '+' || line[cont] == '-'){
      if (acum.size() > 0){
        if (currentKeyword == 1 ||currentKeyword == 4){
          acum = format(acum);
        }
        terms.push_back(acum);
        acum = "";
      }
      operands.push_back(line[cont]);
      acum = "";
    } else if (line[cont] == '.'){
      if (acum.size() > 0){
        if (currentKeyword == 1 ||currentKeyword == 4){
          acum = format(acum);
        }
        terms.push_back(acum);
        acum = "";
      }
      switch(currentKeyword) {
          case 1:
          case 4:
            while (operands.size() > 0){
               if (operands[operands.size() - 1] == '+'){
                terms[terms.size() - 2] = join( (terms[terms.size() - 2]), (terms[terms.size() - 1]), savedCharacters);
                terms.pop_back();
                operands.pop_back();
              } else if (operands[operands.size() - 1] == '-'){
                terms[terms.size() - 2] = remove( (terms[terms.size() - 2]), (terms[terms.size() - 1]), savedCharacters);
                terms.pop_back();
                operands.pop_back();
              }
              else {
                //error
              }
            }
            //cout << terms[terms.size() - 2] << endl;
            if (currentKeyword == 1) {
              if (terms.size() == 2){
                //cout << terms[0] << " " << terms[1] << endl;
                terms[1] = checkIfItsAId(terms[1], savedCharacters);
                savedCharacters[terms[0]] = terms[1];
                terms.clear();
                operands.clear();
              } else {
                cout << terms.size() << endl;
                for (int i = 0; i < terms.size(); i++){
                  cout << terms[i] << endl;
                }
                throw std::invalid_argument( "Bad set declaration ");
              } 
            } else {
              if (terms.size() == 1){
                cout << "//////////////////////////////////" << endl;
                cout << terms[0]<< endl;
                whiteSpaces.push_back(checkIfItsAId(terms[0], savedCharacters));
                terms.clear();
                operands.clear();
              }  else {
                cout << terms.size() << endl;
                for (int i = 0; i < terms.size(); i++){
                  cout << terms[i] << endl;
                }
                throw std::invalid_argument( "Bad set declaration ");
              } 
            }
            break;
          case 2:
            if (terms.size() == 2){
              savedKeywords[terms[0]] = terms[1];
              terms.clear();
              operands.clear();
            } 
            break;
          case 3:
            if (terms.size() == 2){
              //cout << "token" << endl;
              //cout << terms[1] << endl;
              terms[1] = format(terms[1]);
              //cout << terms[1] << endl;
              terms[1] = passToParentesis(replaceString(terms[1], "''", ")(", true));
              //cout << terms[1] << endl;
              size_t firstPosition = terms[1].find("{");
              //cout << terms[1] << endl;
              if (firstPosition != string::npos) {
                terms[1] = replaceString(terms[1], "{", "(", false);
                terms[1] = replaceString(terms[1], "}", ")*", false);
              }
              //cout << terms[1] << endl;
              firstPosition = terms[1].find("[");
              if (firstPosition != string::npos) {
                terms[1] = replaceString(terms[1], "[", "(", false);
                terms[1] = replaceString(terms[1], "]", ")?", false);
              }
              //cout << terms[1] << endl;
              savedTokens[terms[0]] = terms[1];
              tokenIds.push_back(terms[0]);
              exceptTokens[terms[0]] = exceptKeyWordsFlag;
              exceptKeyWordsFlag = false;
              terms.clear();
              operands.clear();
            } else {
              //Error
            }
            break;
      }
    } else if (isspace(line[cont])){
      if (currentKeyword == 3 && terms.size() > 0) {
        acum = acum + line[cont];
      }
    } else {
      acum = acum + line[cont];
      //cout << line[cont];
    }
    cont++;
  }
}

/*---------------------------------------------------------------------
 * Function:      mergeWhiteSpaces
 * Purpose:       Merge white spaces
 * In arg:        whiteSpaces, vector of the input whitespaces
 * Return val:    Resulting chain
 */
string mergeWhiteSpaces(vector<string>& whiteSpaces){
  string result = "";
  for(int i = 0; i < whiteSpaces.size(); i++){
    result = result + whiteSpaces[i];
  }
  return result; 
}

/*---------------------------------------------------------------------
 * Function:      removeSpaces
 * Purpose:       Remove spaces
 * In arg:        chain, the chain to be clean
 * Return val:    Resulting chain
 */
string removeSpaces (string chain){
  //cout << chain << endl;
  string result = "";
  for (int i = 0; i < chain.size(); i++){
    if (!isspace(chain[i])){
      result = result + chain[i];
    }
  }
  return result;
}

int main (int argc, char* argv[]) {

  if (argc < 4){
    cout << "An error ocurred\n";
    cout << "Missing file path argument" << endl;
    return 0;
  }
  string filePath = argv[1];

  map<string,string> savedKeywords;
  map<string,string> savedCharacters;
  map<string,string> savedTokens;
  map<string,bool> exceptTokens;
  vector<string> tokenIds;
  vector<string> whiteSpaces;
  string resultWhiteSpaces;
  savedCharacters["ANY"] = format("CHR(32)$CHR(126)");
  try {
    fillmaps(filePath, savedKeywords, savedCharacters, savedTokens, whiteSpaces, exceptTokens, tokenIds);
    passToOrs(savedCharacters);
    //makeUnique(savedCharacters);
    replace(savedCharacters, savedTokens, tokenIds);
    resultWhiteSpaces = replaceString(mergeWhiteSpaces(whiteSpaces), "''", ",", true);
  } catch (exception& e){
    cout << "An error ocurred\n";
    cout << e.what() << endl;
    cout << "Check your expression and try again\n";
    return 0;
  }
  /*
  //Prints
  for(auto it = savedCharacters.cbegin(); it != savedCharacters.cend(); ++it){
    if (it->second.size() > 0) {
      cout << it->first << " " << it->second << endl;
    }
  }
  for(auto it = savedTokens.cbegin(); it != savedTokens.cend(); ++it){
    if (it->second.size() > 0) {
      cout << it->first << " " << it->second << endl;
    }
  }
  for(auto it = savedKeywords.cbegin(); it != savedKeywords.cend(); ++it){
    cout << it->first << " " << it->second << endl;
  }
  cout << resultWhiteSpaces << endl;*/
  //write file
  
  string line;
  ifstream basefile;
  basefile.open("scanner.cpp");
  ofstream newfile(argv[2]);
  string classId = argv[3];
  if (basefile.is_open())
  {
    while ( getline (basefile,line) )
    {
      if (equal(line.begin(), line.end(), nameFlag.begin(), nameFlag.end())){
        newfile <<  "class Scanner"+classId+" {" << endl;
      } else if (equal(line.begin(), line.end(), constructorFlag.begin(), constructorFlag.end())){
        newfile <<  "    Scanner"+classId+"(string input){" << endl;
      } else if (equal(line.begin(), line.end(), flag.begin(), flag.end())){
        string acum = "";
        cout << "Writing whitespaces " << endl;
        newfile << flag << endl;
        if (resultWhiteSpaces.size() > 0){
          for(int i = 1; i < resultWhiteSpaces.size() - 1; i++){
            if (resultWhiteSpaces[i] == ','){
              newfile <<  "        whitespaces.insert(" << acum << ");" << endl;
              acum = "";
            } else {
              acum = acum  + resultWhiteSpaces[i];
            }
          }
          newfile <<  "        whitespaces.insert(" << acum << ");" << endl;
        }
        cout << "Writing tokens" << endl;
        for (int i = 0; i < tokenIds.size(); i++){
          newfile <<  "        exceptTokens[\"" << tokenIds[i] << "\"] = "<< exceptTokens[tokenIds[i]] <<";\n";
          newfile <<  "        expressions.push_back(\"" << removeSpaces(savedTokens[tokenIds[i]]) <<"\");"<< endl;
          newfile <<  "        expressionsId.push_back(\"" << tokenIds[i] <<"\");"<< endl;
        }
        cout << "Writing keywords" << endl;
        for(auto it = savedKeywords.cbegin(); it != savedKeywords.cend(); ++it){
          newfile <<  "        keywords[\"" << it->first << "\"] = "<< it->second <<";\n";
        }
      } else {
        newfile << line << "\n";
      }
    }
    basefile.close();
    newfile.close();
  } else {
    cout << "Unable to open file";
  }
  return 0;
}