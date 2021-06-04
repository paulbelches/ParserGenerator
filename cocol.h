#include <iostream>
#include <string>
#include <queue>
#include <set>
class ScannerCocol {
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
    ScannerCocol(string input){
        //INSERT EXPRESSIONS
        whitespaces.insert(10);
        whitespaces.insert(13);
        exceptTokens["whiteToken"] = 0;
        expressions.push_back("(9|32)((9|32))*");
        expressionsId.push_back("whiteToken");
        exceptTokens["ident"] = 0;
        expressions.push_back("(65|66|67|68|69|70|71|72|73|74|75|76|77|78|79|80|81|82|83|84|85|86|87|88|89|90|97|98|99|100|101|102|103|104|105|106|107|108|109|110|111|112|113|114|115|116|117|118|119|120|121|122)((65|66|67|68|69|70|71|72|73|74|75|76|77|78|79|80|81|82|83|84|85|86|87|88|89|90|97|98|99|100|101|102|103|104|105|106|107|108|109|110|111|112|113|114|115|116|117|118|119|120|121|122)|(48|49|50|51|52|53|54|55|56|57))*");
        expressionsId.push_back("ident");
        exceptTokens["eq"] = 0;
        expressions.push_back("(61)");
        expressionsId.push_back("eq");
        exceptTokens["p_end"] = 0;
        expressions.push_back("(46)");
        expressionsId.push_back("p_end");
        exceptTokens["br_open"] = 0;
        expressions.push_back("(123)");
        expressionsId.push_back("br_open");
        exceptTokens["br_close"] = 0;
        expressions.push_back("(125)");
        expressionsId.push_back("br_close");
        exceptTokens["sq_open"] = 0;
        expressions.push_back("(91)");
        expressionsId.push_back("sq_open");
        exceptTokens["sq_close"] = 0;
        expressions.push_back("(93)");
        expressionsId.push_back("sq_close");
        exceptTokens["p_open"] = 0;
        expressions.push_back("(40)");
        expressionsId.push_back("p_open");
        exceptTokens["p_close"] = 0;
        expressions.push_back("(41)");
        expressionsId.push_back("p_close");
        exceptTokens["union"] = 0;
        expressions.push_back("(124)");
        expressionsId.push_back("union");
        exceptTokens["string"] = 0;
        expressions.push_back("(92|34)((32|33|35|36|37|38|39|40|41|42|43|44|45|46|47|48|49|50|51|52|53|54|55|56|57|58|59|60|61|62|63|64|65|66|67|68|69|70|71|72|73|74|75|76|77|78|79|80|81|82|83|84|85|86|87|88|89|90|91|93|94|95|96|97|98|99|100|101|102|103|104|105|106|107|108|109|110|111|112|113|114|115|116|117|118|119|120|121|122|123|124|125|126))*(92|34)");
        expressionsId.push_back("string");
        exceptTokens["attr"] = 0;
        expressions.push_back("(60)((32|33|34|35|36|37|38|39|40|41|42|43|44|45|46|47|48|49|50|51|52|53|54|55|56|57|58|59|60|61|62|63|64|65|66|67|68|69|70|71|72|73|74|75|76|77|78|79|80|81|82|83|84|85|86|87|88|89|90|91|92|93|94|95|96|97|98|99|100|101|102|103|104|105|106|107|108|109|110|111|112|113|114|115|116|117|118|119|120|121|122|123|124|125|126))*(62)");
        expressionsId.push_back("attr");
        exceptTokens["s_action"] = 0;
        expressions.push_back("(40)(46)((32|33|34|35|36|37|38|39|40|41|42|43|44|45|46|47|48|49|50|51|52|53|54|55|56|57|58|59|60|61|62|63|64|65|66|67|68|69|70|71|72|73|74|75|76|77|78|79|80|81|82|83|84|85|86|87|88|89|90|91|92|93|94|95|96|97|98|99|100|101|102|103|104|105|106|107|108|109|110|111|112|113|114|115|116|117|118|119|120|121|122|123|124|125|126))*(46)(41)");
        expressionsId.push_back("s_action");
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
