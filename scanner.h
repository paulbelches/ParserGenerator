#include <iostream>
#include <string>
#include <queue>
#include <set>
class ScannerGrammar {
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
    ScannerGrammar(string input){
        exceptTokens["number"] = 1;
        expressions.push_back("(48|49|50|51|52|53|54|55|56|57)((48|49|50|51|52|53|54|55|56|57))*");
        expressionsId.push_back("number");
        exceptTokens["decnumber"] = 0;
        expressions.push_back("(48|49|50|51|52|53|54|55|56|57)((48|49|50|51|52|53|54|55|56|57))*(46)(48|49|50|51|52|53|54|55|56|57)((48|49|50|51|52|53|54|55|56|57))*");
        expressionsId.push_back("decnumber");
        exceptTokens["white"] = 0;
        expressions.push_back("(10|13|9|32)((10|13|9|32))*");
        expressionsId.push_back("white");
        keywords["do"] = "do";
        keywords["while"] = "while";
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
