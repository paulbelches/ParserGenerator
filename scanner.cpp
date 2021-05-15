#include <iostream>
#include <string>
#include <queue>
#include <set>
//INSERT NAME
//class Scanner {
    public:
    vector<string> expressions;
    vector<string> expressionsId;
    map<string,bool> exceptTokens;
    vector<int> finalids;
    map<string,string> savedCharacters;
    set<int> whitespaces;
    set<int> registerIds;
    map<string,string> keywords;
    queue<token> readTokens;
    SyntaxTree* syntaxtree;
    AFDirect* afdirect;
    //Scanner(string input){
    //INSERT CONSTRUCTOR
        //INSERT EXPRESSIONS
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
        //writeAFDirect(afdirect, "afdirect.txt");
        //printAFDirect(afdirect);
        afdirect->simulate(input, readTokens);
    }
};
