#include <iostream>
#include <string>
#include "methods.h"
#include "cocol.h"
#include <algorithm>
using namespace std;
const string flag = "        //INSERT EXPRESSIONS";
const string methodsflag = "//INSERT METHODS";
const string firstcallflag = "    //INSERT FIRSTCALL";
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

bool isOperand(string type){
    if (  (equal(type.begin(), type.end(), "ident"))
    ||    (equal(type.begin(), type.end(), "eq"))
    ||    (equal(type.begin(), type.end(), "p_end"))
    ||    (equal(type.begin(), type.end(), "attr"))
    ||    (equal(type.begin(), type.end(), "s_action"))
    ||    (equal(type.begin(), type.end(), "string")) ){
        return true;
    } 
    return false;
}

int precedence(string type){
    if ( (equal(type.begin(), type.end(), "p_open")) ){
        return 0;
    } else if ( (equal(type.begin(), type.end(), "p_close")) ){
        return 1;
    } else if ( (equal(type.begin(), type.end(), "union")) ){
        return 2;
    } else if ( (equal(type.begin(), type.end(), "concat")) ){
        return 3;
    } else if ( 
        (equal(type.begin(), type.end(), "conditional")) 
        || (equal(type.begin(), type.end(), "lock")) 
    ){
        return 4;
    } else {
        return -1;
    }
}


vector<token> expand(vector<token> production){
    vector<token> result;
    //cout << "entre" << endl;
    int cont = 0;
    bool operand = 0;
    for (int i = 0; i < production.size(); i = i + 1){
        //cout << "-----------------------" << production[i].type << endl;
        if (equal(production[i].type.begin(), production[i].type.end(), "union")){
            cont = 0;
            operand = 0;
        } else if ((equal(production[i].type.begin(), production[i].type.end(), "p_open"))
        ||    (equal(production[i].type.begin(), production[i].type.end(), "br_open"))
        ||    (equal(production[i].type.begin(), production[i].type.end(), "sq_open"))
        ){
            if (cont == 1){
                token tempToken;
                tempToken.type = "concat";
                tempToken.value = "concat";
                result.push_back(tempToken);
                //cout << "           concat" << endl;
                cont = 0;
                operand = 0;
            }
        } else if (  (equal(production[i].type.begin(), production[i].type.end(), "p_close"))
        ||    (equal(production[i].type.begin(), production[i].type.end(), "br_close"))
        ||    (equal(production[i].type.begin(), production[i].type.end(), "sq_close"))
        ){
            operand = 0;
        } else {
            operand = 1;
            cont = cont + 1;
        }
        if (cont == 2){
             //r = r + '.' + expr[i];
            token tempToken;
            tempToken.type = "concat";
            tempToken.value = "concat";
            result.push_back(tempToken);
            //cout << "               concat" << endl;
            result.push_back(production[i]);
            //cout << production[i].value << " " << production[i].type << endl;
            cont = 1;
        } else {
            result.push_back(production[i]);
            //cout << production[i].value << " " << production[i].type << endl;
        }
    }
    //cout << "sali" << endl;
    return result;
}

vector<token> addAtributes(vector<token> production){
    vector<token> result;
    for (int i = 0; i < production.size(); i = i + 1){
        result.push_back(production[i]);
        if (equal(production[i].type.begin(), production[i].type.end(), "ident")){
            token tempToken;
            tempToken.type = "attr";
            tempToken.value = "<>";
            if (i < production.size()){
                if (!equal(production[i+1].type.begin(), production[i+1].type.end(), "attr")){
                    result.push_back(tempToken);
                } 
            } else {
                result.push_back(tempToken);
            }
        }
    }
    return result;
}

vector<token> sustitute(vector<token> production){
    vector<token> result;
    for (int i = 0; i < production.size(); i = i + 1){
        if ((equal(production[i].type.begin(), production[i].type.end(), "br_open"))
        ||    (equal(production[i].type.begin(), production[i].type.end(), "sq_open"))
        ){
            token tempToken;
            tempToken.type = "p_open";
            tempToken.value = "(";
            result.push_back(tempToken);
        } else if (equal(production[i].type.begin(), production[i].type.end(), "br_close")){
            token tempToken;
            tempToken.type = "p_close";
            tempToken.value = ")";
            result.push_back(tempToken);
            token tempToken2;
            tempToken2.type = "lock";
            tempToken2.value = "*";
            result.push_back(tempToken2);
        } else if (equal(production[i].type.begin(), production[i].type.end(), "sq_close")){
            token tempToken;
            tempToken.type = "p_close";
            tempToken.value = ")";
            result.push_back(tempToken);
            token tempToken2;
            tempToken2.type = "conditional";
            tempToken2.value = "?";
            result.push_back(tempToken2);
        } else {
            result.push_back(production[i]);
            //Add empty atributes
        }
    }
    return result;
}
class ProductionNode{
  public:
  int id;
  token data;
  bool nullable;
  set<string> firstpos;
  ProductionNode* left;
  ProductionNode* right;
  ProductionNode* father;
  /*---------------------------------------------------------------------
 * Contructor
 * In arg:        d value of the node, left child, right child
 */
  ProductionNode(token d, ProductionNode* left, ProductionNode* right, ProductionNode* father, set<int>& registerIds){
    id = rand() % 5000 + 1;
    while (registerIds.find(id) != registerIds.end()){
        id = rand() % 5000 + 1;
    }
    registerIds.insert(id);
    data=d;    
    this->left=left;
    this->right=right;
    this->father;
  }
};

/*---------------------------------------------------------------------
 * Function:      nullable
 * Purpose:       Calculate nullable of a node of the syntax tree
 * In arg:        root, a node from the tree
 * Return val:    Whether is nullable or not
 */
bool nullable(ProductionNode* root){
    if (equal(root->data.type.begin(), root->data.type.end(), "lock")){
        return true;
    } else if (equal(root->data.type.begin(), root->data.type.end(), "union")){
        return nullable(root->left) || nullable(root->right);
    } else if (equal(root->data.type.begin(), root->data.type.end(), "concat")){
        return nullable(root->left) && nullable(root->right);
    } else if (equal(root->data.type.begin(), root->data.type.end(), "conditional")){
        return true;
    } else if (equal(root->data.type.begin(), root->data.type.end(), "ident")){
        return false;
    } else if (equal(root->data.type.begin(), root->data.type.end(), "string")){
        return false;
    } else {
        return true;
    }
    ///Mmore cases?
}
set<string> productionFirstpos( string ident  , vector<string> productionsIds, map<string, vector<token>> productionsMap){
    //cout << ident << endl;
    int cont = 0; 
    int state = 0;
    bool inJoin = false;
    bool readIdent = false;
    bool working = true;

    set<string> result;
    while (cont < productionsMap[ident].size() && working) {
        switch (state)
        {
        case 0:
            if ( equal(productionsMap[ident][cont].value.begin(), productionsMap[ident][cont].value.end(), "=")  ) {
                //cout << "es un igual"<< endl;
                state = 1;
                readIdent = true;
            }   
        break;
        case 1:
            if ( 
                (
                    equal(productionsMap[ident][cont].type.begin(), productionsMap[ident][cont].type.end(), "ident") ||
                    equal(productionsMap[ident][cont].type.begin(), productionsMap[ident][cont].type.end(), "string") 
                ) && readIdent
            ) {
                if (find(productionsIds.begin(), productionsIds.end(), productionsMap[ident][cont].value) != productionsIds.end()){
                    set<string> tempResult = productionFirstpos( productionsMap[ident][cont].value, productionsIds, productionsMap);
                    result.insert(tempResult.begin(), tempResult.end());
                } else {
                    result.insert( 
                        equal(productionsMap[ident][cont].type.begin(), productionsMap[ident][cont].type.end(), "ident") ?
                        "\"" + productionsMap[ident][cont].value + "\"":
                        "\"token" + productionsMap[ident][cont].value.substr(1, productionsMap[ident][cont].value.size())
                    );
                }
                readIdent = false;
                if (!inJoin){
                    working = false;
                }
            } else if ( 
                equal(productionsMap[ident][cont].type.begin(), productionsMap[ident][cont].type.end(), "union") 
            ) {
                readIdent = true;
            } 
            else if ( 
                equal(productionsMap[ident][cont].type.begin(), productionsMap[ident][cont].type.end(), "p_open") ||
                equal(productionsMap[ident][cont].type.begin(), productionsMap[ident][cont].type.end(), "br_open") ||
                equal(productionsMap[ident][cont].type.begin(), productionsMap[ident][cont].type.end(), "sq_open")
            ) {
                //cout << "agrupación" << endl;
                inJoin = true;
            } else if ( 
                equal(productionsMap[ident][cont].type.begin(), productionsMap[ident][cont].type.end(), "p_close") 
            ) {
                working = false;
            } else if (equal(productionsMap[ident][cont].type.begin(), productionsMap[ident][cont].type.end(), "sq_close") ||
                equal(productionsMap[ident][cont].type.begin(), productionsMap[ident][cont].type.end(), "br_close")
            ) {
                readIdent = true;
                readIdent = true;
            }
        break;
        default:
        break;
        }
        cont++;
    }
    return result; 
}

/*---------------------------------------------------------------------
 * Function:      firstpos
 * Purpose:       Calculate firstpos of a node of the syntax tree
 * In arg:        root, a node from the tree
 * Return val:    The result lastpos set of nodes
 */
set<string> firstpos(ProductionNode* root, vector<string> productionsIds,map<string, vector<token>> productionsMap){
    set<string> result;
    set<string> tempSet;
    if (equal(root->data.type.begin(), root->data.type.end(), "union")){
        tempSet = firstpos(root->right,  productionsIds, productionsMap);
        result = firstpos(root->left,  productionsIds, productionsMap);
        result.insert(tempSet.begin(), tempSet.end());
    } else if (equal(root->data.type.begin(), root->data.type.end(), "concat")){
        if (nullable(root->left)){
            tempSet = firstpos(root->right,  productionsIds, productionsMap);
            result = firstpos(root->left,  productionsIds, productionsMap);
            result.insert(tempSet.begin(), tempSet.end());
        } else {
            result = firstpos(root->left,  productionsIds, productionsMap);
        }
    } else if (equal(root->data.type.begin(), root->data.type.end(), "lock")){
        result = firstpos(root->left,  productionsIds, productionsMap);
    } else if (equal(root->data.type.begin(), root->data.type.end(), "conditional")){
        result = firstpos(root->left,  productionsIds, productionsMap);
    } else if (equal(root->data.type.begin(), root->data.type.end(), "ident")){
        if (find(productionsIds.begin(), productionsIds.end(), root->data.value) != productionsIds.end()){
            tempSet =  productionFirstpos( root->data.value , productionsIds, productionsMap);
            result.insert(tempSet.begin(), tempSet.end());
        } else {
            //cout << "ident " << root->data.value << endl; 
            result.insert("\""+root->data.value+"\"");
        }
    } else if (equal(root->data.type.begin(), root->data.type.end(), "string")){
        //cout << "token " << root->data.value << endl; 
        result.insert("\"token" + root->data.value.substr(1, root->data.value.size()));
    } else {
        //cout << "aja...." << root->data.value << endl;
        result.insert(root->data.value);
    } 
    return result;
}

/*---------------------------------------------------------------------
 * Function:      fillFunctions
 * Purpose:       Calculate nullable and firstpos of a node of the syntac tree
 * In arg:        root, a node from the tree
 * Return val:    -------
 */
void fillFunctions(ProductionNode* root, vector<string> productionsIds, map<string, vector<token>>  productionsMap){
    root->nullable = nullable(root);
    root->firstpos = firstpos(root, productionsIds, productionsMap);
    if (root->left != NULL) {
        fillFunctions(root->left, productionsIds, productionsMap);
    }
    if (root->right != NULL) {
        fillFunctions(root->right, productionsIds, productionsMap);
    }
}

/*---------------------------------------------------------------------
 * Class:           Graph
 * Purpose:         Represents a Graph
 * Attributes:    
 *         root     The root node
 */ 
class Graph{
  public:
  ProductionNode* root;
/*---------------------------------------------------------------------
 * Contructor
 * In arg:        expr, the regular expresion
 */
  Graph(vector<token> production ){ //, set<int>& registerIds){
    stack<token> opStack; 
    bool readingString = false;
    //Read the expression
    vector<token> result;
    set<int> registerIds;
    production = sustitute(expand(production));
    //cout << "pase" << endl;
    //cout << "antes" << endl;
    for (int i = 0; i < production.size(); i++){
        //cout << production[i].value << " " << production[i].type << endl;
        if (!isOperand(production[i].type)){
            if (opStack.size() == 0){
                opStack.push(production[i]);
            } else {
                if (equal(production[i].type.begin(), production[i].type.end(), "p_open")) {
                    opStack.push(production[i]);
                } else if (equal(production[i].type.begin(), production[i].type.end(), "p_close")) {
                    while (!(equal(opStack.top().type.begin(), opStack.top().type.end(), "p_open"))){
                        result.push_back(opStack.top());
                        opStack.pop();
                    }
                    opStack.pop();
                
                } else if (precedence(production[i].type) > precedence(opStack.top().type)) {
                    opStack.push(production[i]);
                } else {
                    while (precedence(production[i].type) <= precedence(opStack.top().type)){
                        result.push_back(opStack.top());
                        opStack.pop();
                        if (opStack.empty()) {
                            break;
                        } 
                    }
                    opStack.push(production[i]);
                }
            }
        } else {
            result.push_back(production[i]);
        }
    }
    //cout << result << endl;
    while (!opStack.empty()){
        result.push_back(opStack.top());
        opStack.pop();
    }

    /*
    for(int i = 0; i < result.size(); i++){
        cout << result[i].value << " " << result[i].type << endl;
    }
    */
    //cout << result.size() << endl;
    stack<ProductionNode*> tree;
    for (int i=0; i < result.size(); i=i+1){
        if (!isOperand(result[i].type)){
            if (equal(result[i].type.begin(), result[i].type.end(), "lock")){ //while
                //cout << tree.front()->data << "\n";
                if (tree.size() == 0){
                    throw std::invalid_argument( "bad expresion received" );
                }
                ProductionNode* temp = new ProductionNode(result[i], tree.top(), NULL, NULL, registerIds);
                tree.top() = temp;
            } else if (equal(result[i].type.begin(), result[i].type.end(), "concat")){ //concat
                if (tree.size() == 0){
                    throw std::invalid_argument( "bad expresion received" );
                }
                ProductionNode* firstValue = tree.top();
                tree.pop();
                if (tree.size() == 0){
                    throw std::invalid_argument( "bad expresion received" );
                }
                ProductionNode* temp = new ProductionNode(result[i], tree.top(), firstValue, NULL, registerIds);
                tree.top() = temp;               
            } else if (equal(result[i].type.begin(), result[i].type.end(), "union")){ //switch With error
                if (tree.size() == 0){
                    throw std::invalid_argument( "bad expresion received" );
                }
                ProductionNode* firstValue = tree.top();
                tree.pop();
                if (tree.size() == 0){
                    throw std::invalid_argument( "bad expresion received" );
                }
                ProductionNode* temp = new ProductionNode(result[i], tree.top(), firstValue, NULL, registerIds);
                tree.top() = temp;
            } else if (equal(result[i].type.begin(), result[i].type.end(), "conditional")){ //switch
                if (tree.size() == 0){
                    throw std::invalid_argument( " bad expresion received" );
                }
                ProductionNode* temp = new ProductionNode(result[i], tree.top(), NULL, NULL, registerIds);
                tree.top() = temp;
            }
            //cout << result[i][0] << "\n";
        } else {
            tree.push(new ProductionNode(result[i], NULL, NULL, NULL, registerIds));
        }
       //opStack.push(new Node(opStack.top(), NULL, NULL);); 
    }
    //cout << tree.size() << endl;
    root = tree.top();
  }
};

string printGraph(ProductionNode* root, int counter){
    string left = "";
    string right = "";
    string result = "";

    if (root->left != NULL){
        ProductionNode* leftNode = root->left;
        left = to_string(leftNode->id);
        result = result + to_string(root->id) + " " + to_string(leftNode->id) + "\n";
    } 
    if (root->right != NULL){
        ProductionNode* rightNode = root->right;
        right = to_string(rightNode->id);
        result = result + to_string(root->id) + " " +to_string(rightNode->id) + "\n";
    }
    
    cout << " id: " << root->id << " value: " <<  root->data.value << " type: " <<  root->data.type << endl;//<< ") firstpos: " << printNode(root->firstpos) << " lastpos: " << printNode(root->lastpos) << "\n";

    return result + (root->left == NULL ? "" : printGraph(root->left, counter++)) + (root->right == NULL ? "" : printGraph(root->right, counter++));
}

string getNextToken(ProductionNode* root, int state){
    if (state == 0){
        //ver si soy el hijo izquierdo
        if (equal(root->data.type.begin(), root->data.type.end(), root->father->left->data.type.begin(), root->father->left->data.type.end()) 
            && 
            equal(root->data.value.begin(), root->data.value.end(), root->father->left->data.value.begin(), root->father->left->data.value.end())){
            return getNextToken(root->father->right, 1);
        } else {
            return getNextToken(root->father, 0);
        }
    } else {
        //encontrar un hijo imprimible
        if (equal(root->data.type.begin(), root->data.type.end(), "concat")){
            return getNextToken(root->left, 1);
        } else {
            return root->data.type;
        }
    }
    return "";
}

string generateCode(ProductionNode* root, ProductionNode* father, vector<string> productionsIds, map<string, vector<token>>  productionsMap){
    string value = "";
    root->father = father;
    if (root == NULL) {
        return "";
    } else if (equal(root->data.type.begin(), root->data.type.end(), "concat")){
        if (equal(root->left->data.type.begin(), root->left->data.type.end(), "ident") && equal(root->right->data.type.begin(), root->right->data.type.end(), "attr"))
            return "" + (root->left == NULL ? "" : generateCode(root->left, root, productionsIds, productionsMap)) + (root->right == NULL ? "" : generateCode(root->right, root, productionsIds, productionsMap));
        else
            return "" + (root->left == NULL ? "" : generateCode(root->left, root, productionsIds, productionsMap)) + (root->right == NULL ? "" : generateCode(root->right, root, productionsIds, productionsMap));
    } else if (equal(root->data.type.begin(), root->data.type.end(), "attr")){
        string nextTokenType = getNextToken(father->right,0);
        return "(" + root->data.value.substr(1, root->data.value.size() - 2) + ")" 
                + (equal(nextTokenType.begin(), nextTokenType.end(), "eq") ? "" : ";") + "\n" 
                + (root->left == NULL ? "" : generateCode(root->left, root, productionsIds, productionsMap)) + (root->right == NULL ? "" : generateCode(root->right, root, productionsIds, productionsMap));
    } else if (equal(root->data.type.begin(), root->data.type.end(), "s_action")){
        return root->data.value.substr(2, root->data.value.size() - 4) + "\n" 
                + (root->left == NULL ? "" : generateCode(root->left, root, productionsIds, productionsMap)) + (root->right == NULL ? "" : generateCode(root->right, root, productionsIds, productionsMap));
    } else if (equal(root->data.type.begin(), root->data.type.end(), "eq")){
        return "{\n"
                + (root->left == NULL ? "" : generateCode(root->left, root, productionsIds, productionsMap)) + (root->right == NULL ? "" : generateCode(root->right, root, productionsIds, productionsMap));
    } else if (equal(root->data.type.begin(), root->data.type.end(), "p_end")){
        return "}\n"
                + (root->left == NULL ? "" : generateCode(root->left, root, productionsIds, productionsMap)) + (root->right == NULL ? "" : generateCode(root->right, root, productionsIds, productionsMap));
    } else if (equal(root->data.type.begin(), root->data.type.end(), "union")){
        if (root->firstpos.size() == 0){
            fillFunctions(root, productionsIds, productionsMap);
        }
        bool first = true;
        string condition1 ;
        for(const auto &i : root->left->firstpos) {
            condition1 = condition1 + (first ? " " : "||" ) 
            +  "equal(nextToken.type.begin(), nextToken.type.end(), " + i +") ";
            first = false;
        }
        first = true;
        string condition2 ;
        for(const auto &i : root->right->firstpos) {
            condition2 = condition2 + (first ? " " : "||" ) 
            +  "equal(nextToken.type.begin(), nextToken.type.end(), " + i +") ";
            first = false;
        }
        return "if ("+condition1 + "){ \n" + generateCode(root->left, root, productionsIds, productionsMap)
            + "} \n else if ("+condition2+ "){ \n" + generateCode(root->right, root, productionsIds, productionsMap) 
            + "} else { error();\n}";
    } else if (equal(root->data.type.begin(), root->data.type.end(), "lock")){
        if (root->firstpos.size() == 0){
            fillFunctions(root, productionsIds, productionsMap);
        }
        string condition ;
        bool first = true;
        for(const auto &i : root->firstpos) {
            //cout << i << "\n";
            condition = condition + (first ? " " : "||" ) 
            + "equal(nextToken.type.begin(), nextToken.type.end(), " + i + ") ";
            first = false;
        }
        return "while("+condition+"){ \n" + generateCode(root->left, root, productionsIds, productionsMap) + "}\n";
    } else if (equal(root->data.type.begin(), root->data.type.end(), "conditional")){
        if (root->firstpos.size() == 0){
            fillFunctions(root, productionsIds, productionsMap);
        }
        string condition ;
        bool first = true;
        for(const auto &i : root->left->firstpos) {
            condition = condition + (first ? " " : "||" ) 
            +  "equal(nextToken.type.begin(), nextToken.type.end(), " + i +") ";
            first = false;
        }
        return "if ("+condition+ "){ \n" + generateCode(root->left, root, productionsIds, productionsMap)+ "\n}";
    } else if (equal(root->data.type.begin(), root->data.type.end(), "string")){
        //add to extra tokens list
        return "verify(\"token" + root->data.value.substr(1, root->data.value.size()) + ");\n";
    } else if (equal(root->data.type.begin(), root->data.type.end(), "ident")){
        //Search if ident in term list
        bool isterm = false;
        //cout << root->data.value << endl; 
        for (int i = 0; i < productionsIds.size(); i++){
            //cout << productionsIds[i] << endl;
            if (equal(root->data.value.begin(), root->data.value.end(), productionsIds[i].begin(), productionsIds[i].end())){
                isterm = true;
                break;
            }
        }
        if (isterm) {
            string nextTokenType = getNextToken(root,0);
            //cout << "Next: " << nextTokenType << endl ;
            return root->data.value + 
                + (equal(nextTokenType.begin(), nextTokenType.end(), "attr") ? "" : "()")  
                + (equal(nextTokenType.begin(), nextTokenType.end(), "eq") ? "" : (equal(nextTokenType.begin(), nextTokenType.end(), "attr") ? "" : ";")) 
                + (equal(nextTokenType.begin(), nextTokenType.end(), "attr") ? "" : "\n") ;
        } 
        return "verify(\"" + root->data.value + "\");";
    } else {
        return root->data.value
                    + (root->left == NULL ? "" : generateCode(root->left, root, productionsIds, productionsMap)) + (root->right == NULL ? "" : generateCode(root->right, root, productionsIds, productionsMap));
    }
}

/*---------------------------------------------------------------------
 * Function:      lettersToNums
 * Purpose:       Pass strings and chars for its numerical representations
 * In arg:        line the input line
 * Return val:    sustituted line
 */
string lettersToNums(string line){
  string result = "";
  for (int i = 1; i < line.size()-1; i++){
    result = result + '(' + to_string( (int)line[i] ) + ')';
  }  
  return "(" + result + ")";
}

void insertBlankTokens(map<string, string> blankTokens){
    string line;
    ifstream basefile;
    basefile.open("scanner.h");
    string filecontent;
    if (basefile.is_open())
    {
        while ( getline (basefile,line) )
        {
            if (equal(line.begin(), line.end(), flag.begin(), flag.end())){
                map<string, string>::iterator it = blankTokens.begin();
                while (it != blankTokens.end())
                {
                    filecontent = filecontent + "        expressions.push_back(\"" + it->second + "\");\n";
                    filecontent = filecontent + "        expressionsId.push_back(\"" + it->first + "\");\n";
                    it++;
                }
            } else {
                filecontent = filecontent + line + "\n";
            }
        }
        basefile.close();
    }

    ofstream newfile;
    newfile.open("scanner.h");
    newfile << filecontent << endl;
    newfile.close();
    //cout << filecontent << endl;
}

void insertMethods(string methods, string firstcall){
    string line;
    ifstream basefile;
    basefile.open("parserFile.cpp");
    string filecontent;
    if (basefile.is_open())
    {
        while ( getline (basefile,line) )
        {
            if (equal(line.begin(), line.end(), methodsflag.begin(), methodsflag.end())){
                filecontent = filecontent + methods + "\n";
            } else if (equal(line.begin(), line.end(), firstcallflag.begin(), firstcallflag.end())){
                filecontent = filecontent + firstcall + "\n";
            } else {
                filecontent = filecontent + line + "\n";
            }
        }
        basefile.close();
    }

    ofstream newfile;
    newfile.open("generatedParser.cpp");
    newfile << filecontent << endl;
    newfile.close();
    //cout << filecontent << endl;
}

string optainFirm(string input){
    int i = 0;
    for (i = 0; i < input.size(); i++){
        if (input[i] == '{' ){
            break;
        }
    }
    return "void " + input.substr(0, i-1)+";";
}

int main(int argc, char **argv) {   
    /*  
    if (argc < 2){
        cout << "An error ocurred\n";
        cout << "Missing file path argument" << endl;
        return 0;
    }*/
    map<string, vector<token>> productionsMap;
    map<string, string> blankTokens;

    try {
        string filePath = argv[1];
        string input;
        generateStream(input, filePath);
        if (input.size() <= 0){
            cout << "Input file is empty\n";
            return 0;
        }
        ScannerCocol* cocolS = new ScannerCocol(input);
        vector<token> production;
        string id = "";
        vector<string> productionsIds;
        while (!cocolS->readTokens.empty()){
            //cout << cocolS->readTokens.front().value << " " << cocolS->readTokens.front().type << endl;
            if (id.size() == 0) {
                if (equal(cocolS->readTokens.front().type.begin(), cocolS->readTokens.front().type.end(), "ident")){
                    //cout << cocolS->readTokens.front().value << endl;
                    id = cocolS->readTokens.front().value;
                    production.push_back(cocolS->readTokens.front());
                } else {
                    cout << "error" << endl;
                    return 0;
                }
            } else {
                if (!equal(cocolS->readTokens.front().type.begin(), cocolS->readTokens.front().type.end(), "whiteToken")){
                    production.push_back(cocolS->readTokens.front());
                    if (equal(cocolS->readTokens.front().type.begin(), cocolS->readTokens.front().type.end(), "string")){ 
                        string s = cocolS->readTokens.front().value;
                        blankTokens["token" + s.substr(1, s.size() - 2)] = lettersToNums(s);
                    }
                    if (equal(cocolS->readTokens.front().type.begin(), cocolS->readTokens.front().type.end(), "p_end")){ 
                        productionsMap[id] = production;
                        productionsIds.push_back(id);
                        production.clear();
                        id = "";
                    }
                }
            }
            cocolS->readTokens.pop();
        }
        
        string methods;
        string firms;
        string firstcall;
        for (int i = 0; i < productionsIds.size(); i++){
            cout << "///////// tree"+ to_string(i) +" ///////////" << endl;
            cout << "Next: " << productionsIds[i] << endl ;
            Graph* tempGraph = new Graph(productionsMap[productionsIds[i]]);
            string value = printGraph(tempGraph->root, 0);
            string generatedMethod = generateCode(tempGraph->root, NULL, productionsIds, productionsMap);
            string tempFirm = optainFirm(generatedMethod);
            if (firstcall.size() == 0){
                firstcall = tempFirm.substr(5, tempFirm.size() - 5);
            }
            firms = firms + optainFirm(generatedMethod) + "\n";
            methods =  "void " + generatedMethod + "\n" + methods;
        }
        methods = firms + "\n" + methods;
        insertBlankTokens(blankTokens);
        insertMethods(methods, firstcall);
    } catch (std::exception& e) {
        cout << "Error: An error ocurred\n";
        cout << "Check your expression and try again\n";
    }
    return 0;
}