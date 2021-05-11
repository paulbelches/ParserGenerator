#include <iostream>
#include <bits/stdc++.h> 
#include <utility> 
#include <fstream>
#include <string>
#include <stack>
#include <queue>
#include <algorithm>
#include <set>
#include <chrono> 

/* File:     
 *     main.cpp
 *
 * Purpose:
 *     Using a input regular expression the code generates 3 automatas.
 *     A non/deterministic finite automata, a deterministic finite automata and 
 *     a deterministic finite automata using the direct method. After the automatas
 *     are generated, using a input string the program simulates the automatas to see whether
 *     they acept the string or not.
 *
 * Input:
 *      the regular expresion, and the string to be review
 *
 * Output:
 *     the process done to create an simulate the automatas, 5 text files with the 
 *      representation of the automatas generated, and the result of the simulation
 *     
 * Compile:  
 *    c++ main.cpp -o main.run
 * 
 * Usage:
 *   main.run <'regular expresion'> <'string'>
 *
 */
using namespace std::chrono; 
using namespace std;

/*---------------------------------------------------------------------
 * Function:      isOperator
 * Purpose:       Determine whether a character is a operator or not
 * In arg:        v, the character that is going to be review
 * Return val:    Whether a character is a operand or not
 */
bool isOperator(char v){
    string ops = "()|.*?+";
    for (int i = 0; i < ops.length(); i = i + 1){
        if (ops[i] == v)
            return 1;
    }
    return 0;
}

/*---------------------------------------------------------------------
 * Function:      prcedence
 * Purpose:       Optain operand precedence
 * In arg:        v, the character that is going to be review
 * Return val:    An integer representing the value of the precedence
 */
int precedence(char v){
    if (v == '?' || v == '+'){
        return 4;
    }
    string ops = "()|.*";
    for (int i = 0; i < ops.length(); i = i + 1){
        if (ops[i] == v)
            return i;
    }
    return -1;
}

/*---------------------------------------------------------------------
 * Class:         Node
 * Purpose:       Represent nodes of a syntax tree
 * Attributes:    
 *      id        The id of the node
 *      data      The character it represents
 *      nullable  Whether is nullable or not
 *      firstpos  Firstpos value
 *      lastpos   LastPos value
 *      followpos Followpos value
 *      left      Child node to the left
 *      right     Child node to the right
 */
class Node{
  public:
  int id;
  string data;
  bool nullable;
  set<Node*> firstpos;
  set<Node*> lastpos;
  set<Node*> followpos;
  Node* left;
  Node* right;
  /*---------------------------------------------------------------------
 * Contructor
 * In arg:        d value of the node, left child, right child
 */
  Node(string d, Node* left, Node* right, set<int>& registerIds){
    id = rand() % 5000 + 1;
    while (registerIds.find(id) != registerIds.end()){
        id = rand() % 5000 + 1;
    }
    registerIds.insert(id);
    data=d;
    this->left=left;
    this->right=right;
  }
};

/*---------------------------------------------------------------------
 * Function:      expand
 * Purpose:       Expand a regular expresion to include characters for concatenation  
 * In arg:        expr, the regular expresion
 * Return val:    The expand expresion
 */
string expand(string expr){
    string r = "";
    int cont = 0;
    bool operand = 0;
    bool readingString = false;
    for (int i = 0; i < expr.length(); i = i + 1){
        if (readingString){
            if (expr[i] == '"'){
                readingString = false;
                cont = 1;
            }
        } else if (expr[i] == '|'){
            cont = 0;
            operand = 0;
        } else if (expr[i] == '('){
            if (cont == 1){
                r = r + '.';
                cont = 0;
                operand = 0;
            }
        } else if (expr[i] == ')' || expr[i] == '*' || expr[i] == '+' || expr[i] == '?'){
            operand = 0;
        } else if (expr[i] == '"'){
            readingString = true;
            cont = cont + 1;
        } else {
            if (!operand) {
                operand = 1;
                cont = cont + 1;
            }
        }
        if (cont == 2){
            r = r + '.' + expr[i];
            cont = 1;
        } else {
            r = r + expr[i];
        }
    }
    return r;
}
/*---------------------------------------------------------------------
 * Function:      nullable
 * Purpose:       Calculate nullable of a node of the syntax tree
 * In arg:        root, a node from the tree
 * Return val:    Whether is nullable or not
 */
bool nullable(Node* root){
    if (root->data[0] == '$'){
        return true;
    } else if (root->data[0] == '|'){
        return nullable(root->left) || nullable(root->right);
    } else if (root->data[0] == '.'){
        return nullable(root->left) && nullable(root->right);
    } else if (root->data[0] == '*'){
        return true;
    } else {
        return false;
    }
}
/*---------------------------------------------------------------------
 * Function:      lastpos
 * Purpose:       Calculate lastpos of a node of the syntax tree
 * In arg:        root, a node from the tree
 * Return val:    The result lastpos set of nodes
 */
set<Node*> lastpos(Node* root){
    set<Node*> result;
    set<Node*> tempSet;
    if (root->data[0] == '$'){
        return result;
    } else if (root->data[0] == '|'){
        tempSet = lastpos(root->right);
        result = lastpos(root->left);
        result.insert(tempSet.begin(), tempSet.end());
    } else if (root->data[0] == '.'){
        if (nullable(root->right)){
            tempSet = lastpos(root->right);
            result = lastpos(root->left);
            result.insert(tempSet.begin(), tempSet.end());
        } else {
            result = lastpos(root->right);
        }
    } else if (root->data[0] == '*'){
        result = lastpos(root->left);
    } else {
        result.insert(root);
    } 
    return result;
}
/*---------------------------------------------------------------------
 * Function:      firstpos
 * Purpose:       Calculate firstpos of a node of the syntax tree
 * In arg:        root, a node from the tree
 * Return val:    The result lastpos set of nodes
 */
set<Node*> firstpos(Node* root){
    set<Node*> result;
    set<Node*> tempSet;
    if (root->data[0] == '$'){
        return result;
    } else if (root->data[0] == '|'){
        tempSet = firstpos(root->right);
        result = firstpos(root->left);
        result.insert(tempSet.begin(), tempSet.end());
    } else if (root->data[0] == '.'){
        if (nullable(root->left)){
            tempSet = firstpos(root->right);
            result = firstpos(root->left);
            result.insert(tempSet.begin(), tempSet.end());
        } else {
            result = firstpos(root->left);
        }
    } else if (root->data[0] == '*'){
        result = firstpos(root->left);
    } else {
        result.insert(root);
    } 
    return result;
}

/*---------------------------------------------------------------------
 * Function:      fillFunctions
 * Purpose:       Calculate nullable, firstpos and lastpos of a node of the syntac tree
 * In arg:        root, a node from the tree
 * Return val:    -------
 */
void fillFunctions(Node* root){
    root->nullable = nullable(root);
    root->firstpos = firstpos(root);
    root->lastpos = lastpos(root);
    if (root->left != NULL) {
        fillFunctions(root->left);
    }
    if (root->right != NULL) {
        fillFunctions(root->right);
    }
}
/*---------------------------------------------------------------------
 * Function:      getAlphabet
 * Purpose:       Get the alphabet (operands) of a regular expresion
 * In arg:        s, the regular expresion
 * Return val:    -------
 */
set<string> getAlphabet(string s){
    //string nonAlphabet = '.*|()';
    set<string> result;
    string tempValue = "";
    bool readingString = false;
    for (int i=0; i<s.length();i++){
        if ((s[i] != '.' && s[i] != '*' && s[i] != '|' && s[i] != '(' && s[i] != ')' && s[i] != '$' && s[i] != '?' && s[i] != '+') || readingString) {
            if (s[i] == '"') {
                readingString = readingString ? false : true;
            }
            string t(1,s[i]);
            tempValue = tempValue + t;
        } else {
            if (tempValue.size()>0) {
                result.insert(tempValue);
            }
            tempValue = "";
        }
    }
    if (tempValue.size() > 0){
        result.insert(tempValue);
    }
    return result;
}

/*---------------------------------------------------------------------
 * Class:           SyntaxTree
 * Purpose:         Represents a SyntaxTree
 * Attributes:    
 *         root     The root node
 */ 
class SyntaxTree{
  public:
  Node* root;
  void join(Node* tree, set<int>& registerIds);
/*---------------------------------------------------------------------
 * Contructor
 * In arg:        expr, the regular expresion
 */
  SyntaxTree(string expr, set<int>& registerIds){
    stack<char> opStack; 
    string tempValue = ""; 
    bool readingString = false;
    //Read the expression
    vector<string> result;
    for (int i = 0; i < expr.length(); i = i + 1){
        //cout << expr[i] << endl;
        if (isOperator(expr[i])){
            if(tempValue.size() > 0){
                result.push_back(tempValue);
                tempValue = "";
            }
            if (opStack.size() == 0){
                opStack.push(expr[i]);
            } else {
                if (expr[i] == '(') {
                    opStack.push(expr[i]);
                } else if (expr[i] == ')') {
                    while (opStack.top() != '('){
                        string s(1,opStack.top());
                        result.push_back(s);
                        opStack.pop();
                    }
                    opStack.pop();
                 }else if (precedence(expr[i]) > precedence(opStack.top())) {
                    opStack.push(expr[i]);
                } else {
                    while (precedence(expr[i]) <= precedence(opStack.top())){
                        string s(1,opStack.top());
                        result.push_back(s);
                        opStack.pop();
                        if (opStack.empty()) {
                            break;
                        } 
                    }
                    opStack.push(expr[i]);
                }
            }
        } else {
            tempValue = tempValue + expr[i];
        }
    }
    if(tempValue.size() > 0){
        result.push_back(tempValue);
        tempValue = "";
    }
    //cout << result << endl;
    while (!opStack.empty()){
        string s(1,opStack.top());
        result.push_back(s);
        opStack.pop();
    }

    stack<Node*> tree;
    for (int i=0; i < result.size(); i=i+1){
        if (isOperator(result[i][0])){
            if (result[i][0] == '*'){
                //cout << tree.front()->data << "\n";
                if (tree.size() == 0){
                    throw std::invalid_argument( "bad expresion received" );
                }
                Node* temp = new Node("*", tree.top(), NULL, registerIds);
                tree.top() = temp;
            } else if (result[i][0] == '.' || result[i][0] == '|'){
                if (tree.size() == 0){
                    throw std::invalid_argument( "bad expresion received" );
                }
                Node* firstValue = tree.top();
                tree.pop();
                if (tree.size() == 0){
                    throw std::invalid_argument( "bad expresion received" );
                }
                string value(1, result[i][0]);
                Node* temp = new Node(value, tree.top(), firstValue, registerIds);
                tree.top() = temp;
            } else if (result[i][0] == '+'){
                if (tree.size() == 0){
                    throw std::invalid_argument( "bad expresion received" );
                }
                Node* firstValue = new Node("*", tree.top(), NULL, registerIds);
                Node* temp = new Node(".", tree.top(), firstValue, registerIds);
                tree.top() = temp;
            } else if (result[i][0] == '?'){
                if (tree.size() == 0){
                    throw std::invalid_argument( " bad expresion received" );
                }
                Node* firstValue = new Node("$", NULL, NULL, registerIds);
                Node* temp = new Node("|", tree.top(), firstValue, registerIds);
                tree.top() = temp;
            }
            //cout << result[i][0] << "\n";
        } else {
            tree.push(new Node(result[i], NULL, NULL, registerIds));
        }
       //opStack.push(new Node(opStack.top(), NULL, NULL);); 
    }
    //cout << "Resultado: " << result << "\n";
    //cout << "stack: " << tree.size() << "\n";
    root = tree.top();
  }
};

/*---------------------------------------------------------------------
 * Function:      join
 * Purpose:       Joins 2 syntax trees
 * In arg:        tree, syntax tree to join with
 * Return val:    -------
 */

void SyntaxTree::join(Node* tree, set<int>& registerIds){
    root = new Node("|", root, tree, registerIds);
}

/*---------------------------------------------------------------------
 * Function:      setNodeToSetInt
 * Purpose:       Generate a set of ints from a set of nodes
 * In arg:        nodes, set of nodes
 * Return val:    Resulting set
 */
set<int> setNodeToSetInt(set<Node*> nodes){
    set<int> result;
    for (auto const &e: nodes) {
        result.insert(e->id);
    }
    return result;
}

/*---------------------------------------------------------------------
 * Class:           AFDirect
 * Purpose:         Represents a direct deterministic finite automata
 * Attributes:    
 *         nodes       the number of nodes in the follow pos
 *         followposV  Which node represents each position in followpos
 *         ids         The ids of each node
 *         leafs       The char value of each node
 *         states      The states of the automata, the vector representes the states in the afd and the set contains the afn states
 *         transitions The transitions of each of the the automata nodes
 *         alphabet    The alphabet of the automata
 */ 
class AFDirect{
    public:
    //followpos table
    vector<int> nodes; //which node represents each position in followpos
    vector<set<int>> followposV;
    //Reference table
    vector<int> ids;
    vector<string> leafs;
    //AFDode* start;
    vector <set<int>> states;
    vector <vector <int>> transitions;
    set<string> alphabet;
    vector<int> finalids;
    vector<string> expressions;
    vector<string> expressionsId;
    map<string,bool> exceptTokens;
    map<string, string> keywords;
    set<int> whitespaces;
    //Methods
    int isTerminal(int currentState);
    void followpos(Node* node);
    set<int> getFollowpos(int id);
    string getLetter(int id);
    int getNumber(string letter);
    void simulate(string chain);
    int getTransition(string charcterNumber);
    /*---------------------------------------------------------------------
    * Contructor
    * In arg:        start, the root of the syntax tree    alphabet, the automata alphabet
    */
    AFDirect(Node* start, set<string> alphabet, vector<int> finalids, vector<string> expressions , vector<string> expressionsId, 
    set<int> whitespaces, map<string,bool> exceptTokens, map<string, string> keywords){
        followpos(start);
       // cout << "alhabet " << setToString(alphabet) << endl;
        this->finalids = finalids;
        this->alphabet = alphabet;
        this->expressions = expressions;
        this->expressionsId = expressionsId;
        this->whitespaces = whitespaces;
        this->exceptTokens = exceptTokens;
        this->keywords = keywords;

        queue<set<int>> pendingStates;
        states.push_back(setNodeToSetInt(start->firstpos));
        pendingStates.push(setNodeToSetInt(start->firstpos));
        while (!pendingStates.empty()){
            vector<int> temporalTransitions;
            for (auto const &e: alphabet) {
                //cout << e << endl;
                set<int> newState;
                set<int> result;
                for (auto const &h: pendingStates.front()) {
                    if(getLetter(h).compare(e) == 0){
                        //cout << h << getLetter(h) << " " << printIntSet(getFollowpos(h)) << "\n";
                        result = getFollowpos(h);
                        newState.insert(result.begin(), result.end());
                    }
                }
                int pos = -1;
                if (newState.size() > 0){
                    //ver si existe
                    for (int i = 0; i < states.size(); i = i + 1){
                        if (equal(states[i].begin(), states[i].end(), newState.begin(), newState.end())){
                            pos = i;
                        }
                    }
                    //sino agreagar al pool de estados
                    if (pos == -1){
                        states.push_back(newState);
                        pendingStates.push(newState);
                        pos = states.size() - 1;
                    }
                //Agregar transicipon
                }
                temporalTransitions.push_back(pos);
                //cout << printIntSet(pendingStates.front()) << " " << e << " " << printIntSet(newState) << pos << "\n";
            }
            transitions.push_back(temporalTransitions);
            pendingStates.pop();
        }
    }
};
/*---------------------------------------------------------------------
 * Function:      followpos
 * Purpose:       Calculate the followpos from a syntax tree node
 * In arg:        node, a syntaxtree node
 * Return val:    -------
 */
void AFDirect::followpos(Node* node){
    Node* root;
    set<int> result;
    queue<Node*> pendingnodes;
    pendingnodes.push(node);
    while(!pendingnodes.empty()){
        Node* root = pendingnodes.front();
        pendingnodes.pop();
        if (root->data[0] == '.'){
            for (auto const &e: lastpos(root->left)) {
                int pos  = -1;
                for (int i = 0; i < nodes.size(); i = i + 1){
                    if (e->id == nodes[i]){
                        pos = i;
                    } 
                }                if (pos == -1){
                    nodes.push_back(e->id);
                    set<int> dummy;
                    followposV.push_back(dummy);
                    pos = nodes.size() - 1;
                }
                result = setNodeToSetInt(firstpos(root->right));
                followposV[pos].insert(result.begin(), result.end());
            } 

        } else if (root->data[0] == '*'){
            for (auto const &e: root->lastpos) {         
                int pos  = -1;
                for (int i = 0; i < nodes.size(); i = i + 1){
                    if (e->id == nodes[i]){
                        pos = i;
                    } 
                }
                if (pos == -1){
                    nodes.push_back(e->id);
                    set<int> dummy;
                    followposV.push_back(dummy);
                    pos = nodes.size() - 1;
                }
                result = setNodeToSetInt(root->firstpos);
                followposV[pos].insert(result.begin(), result.end());
            };

        } else if (root->data[0] != '|'){ //create reference table
            int pos  = -1;
            for (int i = 0; i < ids.size(); i = i + 1){
                if (root->id == ids[i]){
                        pos = i;
                } 
            }
            if (pos = -1){
                ids.push_back(root->id);
                leafs.push_back(root->data);
            }
        }
        // follow pos
        
        if (root->left != NULL) {
            pendingnodes.push(root->left);
        }
        if (root->right != NULL) {
            pendingnodes.push(root->right);
        }
    }
}

/*---------------------------------------------------------------------
 * Function:      getFollowpos
 * Purpose:       Optain the followpos from a node
 * In arg:        id, id of the node
 * Return val:    set of ints that contain the ids of the followpos result
 */
set<int> AFDirect::getFollowpos(int id){
    set<int> result;
    for (int i = 0; i < nodes.size(); i = i + 1){
        if (nodes[i] == id){
            result = followposV[i];
        }
    }
    return result;
}

/*---------------------------------------------------------------------
 * Function:      getLetter
 * Purpose:       Optain the the letter of a node using its id
 * In arg:        id, id of the node
 * Return val:    nodes letter
 */
string AFDirect::getLetter(int id){
    for (int i = 0; i < ids.size(); i= i +1){
        if (ids[i] == id){
            return leafs[i];
        }
    }
}
/*---------------------------------------------------------------------
 * Function:      getNumber
 * Purpose:       Optain the the id of a node using its letter
 * In arg:        letter, letter of the node
 * Return val:    nodes id
 */
int AFDirect::getNumber(string letter){
    for (int i = 0; i < leafs.size(); i= i +1){
        if (leafs[i].compare(letter) == 0){
            return ids[i];
        }
    }
    return -1;
}

/*---------------------------------------------------------------------
 * Function:      isTerminal
 * Purpose:       Check is a state is a terminal state
 * In arg:        currentState, the state to be checked
 * Return val:    -------
 */
int AFDirect::isTerminal(int currentState){
    for (int j = 0; j < finalids.size(); j = j + 1){
        int finalNum = finalids[j];
        if (states[currentState].find(finalNum) != states[currentState].end()){
            return j;
        }
    }
    return -1;
}


/*---------------------------------------------------------------------
 * Function:      getTransition
 * Purpose:       Get the position of the character in the alphabets set
 * In arg:        charcterNumber, the character that is going to be search
 * Return val:    The position of the character
 */
int AFDirect:: getTransition(string charcterNumber){
    int cont = 0 ;
    int transition = -1 ;//check if it does not change
    //Make transition
    for (auto const &e: alphabet) {
        //cout << e << " " << charcterNumber << endl;
        if (  charcterNumber == e ){
            //cout << "si" << endl;
            transition = cont;
        //subStringlength = subchain.size();
         }
        cont = cont + 1;
    }
    return transition;
}

/*---------------------------------------------------------------------
 * Function:      simulate
 * Purpose:       Simulate a direct deterministic finite automata for a input chain
 * In arg:        afd, the direct deterministic finite automata     chain, the input chain
 * Return val:    -------
 */
void AFDirect::simulate(string chain){
    //check alphabet
    stack<int> chequedStates;
    int i = 0;
    int currentState = 0;
    chequedStates.push(0);
    string readCharacters = "";
    while(i < chain.size()){
        if (this->whitespaces.find((int)chain[i]) != whitespaces.end()){
            //check if terminal state
            i = i + 1;
        } else {
            int transition = getTransition(to_string((int)chain[i]));
            if (transitions[currentState][transition] == -1 || transition == -1){
            //cout << readCharacters << " " << chain[i] << " " << (int)chain[i] << " " << currentState << " " << transition << " " << transitions[currentState][transition] << endl;
                if (chequedStates.size() == 1){
                    readCharacters = readCharacters + chain[i];
                    i++;
                    cout << "<" << readCharacters << ", error>\n";
                    readCharacters = "";
                } else {
                    int goback = 0;
                    int terminalId = isTerminal(currentState);
                    while(terminalId == -1){
                        if (chequedStates.size() == 1){
                            cout << "<" << readCharacters << ", error>\n";
                            readCharacters = "";
                            currentState = 0;
                            chequedStates.top() = 0;
                            goback = -1;
                            break;
                        }
                        chequedStates.pop();
                        currentState = chequedStates.top();
                        terminalId = isTerminal(currentState);
                        goback++;
                    }
                    if (goback > -1){
                        i = i - goback;
                        terminalId = isTerminal(currentState);
                        readCharacters = readCharacters.substr(0, readCharacters.size()-goback);
                        if (exceptTokens[expressionsId[terminalId]] && keywords[readCharacters].size() > 0){
                            cout << "<" << readCharacters << ", " << keywords[readCharacters] << ">" << endl;
                        } else {
                            cout << "<" << readCharacters << "," << expressionsId[terminalId] << ">" << endl;
                        }
                        readCharacters = "";
                        while (chequedStates.size() > 1){
                            chequedStates.pop();
                        }
                        currentState = 0;
                    }
                }
            } else {
                currentState = transitions[currentState][transition];
                chequedStates.push(currentState);
                readCharacters = readCharacters + chain[i];
                i++;
            }
        }
    }
    //Process the pending chain
    int terminalId = isTerminal(currentState);
    if (terminalId > 0){
        if (exceptTokens[expressionsId[terminalId]] && keywords[readCharacters].size() > 0){
            cout << "<" << readCharacters << ", " << keywords[readCharacters] << ">" << endl;
        } else {
            cout << "<" << readCharacters << "," << expressionsId[terminalId] << ">" << endl;
        }
    }

}

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

class Scanner {
    public:
    vector<string> expressions;
    vector<string> expressionsId;
    map<string,bool> exceptTokens;
    vector<int> finalids;
    map<string,string> savedCharacters;
    set<int> whitespaces;
    set<int> registerIds;
    map<string,string> keywords;
    SyntaxTree* syntaxtree;
    AFDirect* afdirect;
    Scanner(string input){
        whitespaces.insert(13);
        whitespaces.insert(10);
        whitespaces.insert(9);
        exceptTokens["startcode"] = 0;
        expressions.push_back("(40)(46)");
        expressionsId.push_back("startcode");
        exceptTokens["char"] = 0;
        expressions.push_back("(39)((47))?(65|66|67|68|69|70|71|72|73|74|75|76|77|78|79|80|81|82|83|84|85|86|87|88|89|90|97|98|99|100|101|102|103|104|105|106|107|108|109|110|111|112|113|114|115|116|117|118|119|120|121|122)(39)");
        expressionsId.push_back("char");
        exceptTokens["endcode"] = 0;
        expressions.push_back("(46)(41)");
        expressionsId.push_back("endcode");
        exceptTokens["charinterval"] = 0;
        expressions.push_back("(67)(72)(82)(40)(48|49|50|51|52|53|54|55|56|57)((48|49|50|51|52|53|54|55|56|57))*(41)(46)(46)(67)(72)(82)(40)(48|49|50|51|52|53|54|55|56|57)((48|49|50|51|52|53|54|55|56|57))*(41)");
        expressionsId.push_back("charinterval");
        exceptTokens["charnumber"] = 0;
        expressions.push_back("(67)(72)(82)(40)(48|49|50|51|52|53|54|55|56|57)((48|49|50|51|52|53|54|55|56|57))*(41)");
        expressionsId.push_back("charnumber");
        exceptTokens["string"] = 0;
        expressions.push_back("(34)(((33|35|36|37|38|39|40|41|42|43|44|45|46|47|48|49|50|51|52|53|54|55|56|57|58|59|60|61|62|63|64|65|66|67|68|69|70|71|72|73|74|75|76|77|78|79|80|81|82|83|84|85|86|87|88|89|90|91|92|93|94|95|96|97|98|99|100|101|102|103|104|105|106|107|108|109|110|111|112|113|114|115|116|117|118|119|120|121|122|123|124|125|126))*)(34)");
        expressionsId.push_back("string");
        exceptTokens["ident"] = 1;
        expressions.push_back("(65|66|67|68|69|70|71|72|73|74|75|76|77|78|79|80|81|82|83|84|85|86|87|88|89|90|97|98|99|100|101|102|103|104|105|106|107|108|109|110|111|112|113|114|115|116|117|118|119|120|121|122)((65|66|67|68|69|70|71|72|73|74|75|76|77|78|79|80|81|82|83|84|85|86|87|88|89|90|97|98|99|100|101|102|103|104|105|106|107|108|109|110|111|112|113|114|115|116|117|118|119|120|121|122)|(48|49|50|51|52|53|54|55|56|57))*");
        expressionsId.push_back("ident");
        exceptTokens["nontoken"] = 0;
        expressions.push_back("(33|34|35|36|37|38|39|42|44|47|48|49|50|51|52|53|54|55|56|57|58|59|63|64|65|66|67|68|69|70|71|72|73|74|75|76|77|78|79|80|81|82|83|84|85|86|87|88|89|90|92|94|95|96|97|98|99|100|101|102|103|104|105|106|107|108|109|110|111|112|113|114|115|116|117|118|119|120|121|122|126)");
        expressionsId.push_back("nontoken");
        string expr = expand(expressions[0]);
        set<string> alphabet = getAlphabet(expr);
        expr = '(' + expr + ").#";
        SyntaxTree* syntaxtree = new SyntaxTree(expr,registerIds);
        finalids.push_back(syntaxtree->root->right->id);
        for (int i = 1; i < expressions.size(); i = i + 1){
            string expr = expand(expressions[i]);
            set<string> tempAlphabet = getAlphabet(expr);
            alphabet.insert(tempAlphabet.begin(), tempAlphabet.end());
            expr = '(' + expr + ").#";
            SyntaxTree* tempsyntaxtree = new SyntaxTree(expr,registerIds);;
            finalids.push_back(tempsyntaxtree->root->right->id);
            syntaxtree->join(tempsyntaxtree->root,registerIds);
        }
        fillFunctions(syntaxtree->root);   
        AFDirect* afdirect = new AFDirect(syntaxtree->root, alphabet, finalids, expressions, expressionsId, whitespaces, exceptTokens, keywords);
        afdirect->simulate(input);
    }
};

/*
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
        
    } catch (std::exception& e) {
        cout << "Error: An error ocurred\n";
        cout << "Check your expression and try again\n";
    }
    return 0;
}*/
