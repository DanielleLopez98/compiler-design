
/**
 * Dani Lopez - CS 6820
 * Assignment #3 : LL1 Parsing Table
 *
 * ------------------------------------------------
 * Compiling Instructions
 * all of the ll1 class and associated code, and the main function is in the LL1.cpp file, just either build an executable from this file or press play on your IDE
 * no input required for main
 */

#include <map>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <iostream>


class LL1{
private:
    //vectors
    std::vector<std::string> nonTerminals = {"Goal","Expr","Expr_","Term","Term_","Factor"}; //nonterminals
    std::vector<std::string> terminals = {"+", "-", "*", "/", "(", ")", "name", "num"}; //terminals
    std::vector<std::string> epseol = {"epsilon", "eol"}; //epsilon and eol
    std::vector<std::string> termsUepsUeol; //vector combining terminals, epsilon and eol
    std::vector<std::string> termsUnonTerms; //vector combining terminals and non terminals

    //data structures to associate symbols and productions with int values for the ll1 table
    std::map<std::string, int> nonTermIdxs;
    std::map<std::string, int> termIdxs;
    std::map<std::pair<std::string,std::vector<std::string>>, int> productionInts;

    //tables
    std::map<std::string, std::vector<std::vector<std::string>>> productionTable; //production table
    std::map<std::string, std::set<std::string>> first; //first table
    std::map<std::string, std::set<std::string>> follow; //follow table
    std::map<std::pair<std::string,std::vector<std::string>>, std::set<std::string>> firstPlus; //first+ table
    std::vector<std::vector<std::string>> ll1Table;

public:
    LL1(); //constructor
    void buildIdxAssoc(); //build data structures associating index values for ll1
    void buildProdTable(); //build production table
    void buildFirst(); //build first table
    void buildFollow(); //build follow table
    void buildFirstPlus(); //build first+ table
    void buildLL1(); //build LL1 table

    //Getters
    std::map<std::string, std::vector<std::vector<std::string>>> getProdTable();
    std::vector<std::string> getInsertionOrder();
    std::vector<std::string> getTerminals();
    std::map<std::string, std::set<std::string>> getFirst();
    std::map<std::string, std::set<std::string>> getFollow();
    std::map<std::pair<std::string,std::vector<std::string>>, std::set<std::string>> getFirstPlus();
    std::vector<std::vector<std::string>> getLL1Table();
    std::map<std::pair<std::string,std::vector<std::string>>, int> getProductionInts();

};

/**
 * _______________________________________________________________________________________________________________________
 * Constructor: build LL1 object
 * _______________________________________________________________________________________________________________________
 */
LL1::LL1(){
    //build vector for Terminals U Epsilon U EOF
    termsUepsUeol.insert(termsUepsUeol.begin(), terminals.begin(), terminals.end());
    termsUepsUeol.insert(termsUepsUeol.begin(), epseol.begin(), epseol.end());

    //build vector for Terminals U Epsilon U EOF
    termsUnonTerms.insert(termsUnonTerms.begin(),terminals.begin(),terminals.end());
    termsUnonTerms.insert(termsUnonTerms.begin(),nonTerminals.begin(),nonTerminals.end());

    //build data structures for associating index values in LL1 table
    buildIdxAssoc();

    //build tables
    buildProdTable();
    buildFirst();
    buildFollow();
    buildFirstPlus();
    buildLL1();
}

/**
 * _______________________________________________________________________________________________________________________
 * buildProd: build data structures for associating idxs with symbols and in LL1 table
 * _______________________________________________________________________________________________________________________
 */
void LL1::buildIdxAssoc() {
    //build structure for terminal idxs
    termIdxs["eol"] = 0;
    termIdxs["+"] = 1;
    termIdxs["-"] = 2;
    termIdxs["*"] = 3;
    termIdxs["/"] = 4;
    termIdxs["("] = 5;
    termIdxs[")"] = 6;
    termIdxs["name"] = 7;
    termIdxs["num"] = 8;

    //build structure for nonterminal idxs
    nonTermIdxs["Goal"] = 0;
    nonTermIdxs["Expr"] = 1;
    nonTermIdxs["Expr_"] = 2;
    nonTermIdxs["Term"] = 3;
    nonTermIdxs["Term_"] = 4;
    nonTermIdxs["Factor"] = 5;

}

/**
 * _______________________________________________________________________________________________________________________
 * buildProd: build production table and additional data structure, production ints, to number each production
 * _______________________________________________________________________________________________________________________
 */
void LL1::buildProdTable(){

    //Productions for Goal
    std::vector<std::string> goalProduction1 = {"Expr"};
    std::vector<std::vector<std::string>> goalProductions = {goalProduction1};
    //make pairs for productions
    std::pair<std::string, std::vector<std::string>> goalProdPair = std::make_pair("Goal", goalProduction1);
    //Add to int matching data structure
    productionInts[goalProdPair] = 0;
    //Add Goal Productions to table
    productionTable["Goal"] = goalProductions;


    //Productions for Expr
    std::vector<std::string> exprProduction1 = {"Term","Expr_"};
    std::vector<std::vector<std::string>> exprProductions = {exprProduction1};
    //make pairs for productions
    std::pair<std::string, std::vector<std::string>> exprProdPair = std::make_pair("Expr", exprProduction1);
    //Add to int matching data structure
    productionInts[exprProdPair] = 1;
    //Add Expr Productions to table
    productionTable["Expr"] = exprProductions;

    //Productions for Expr_
    std::vector<std::string> expr_Production1 = {"+","Term","Expr_"};
    std::vector<std::string> expr_Production2 = {"-","Term","Expr_"};
    std::vector<std::string> expr_Production3 = {"epsilon"};
    std::vector<std::vector<std::string>> expr_Productions = {expr_Production1, expr_Production2, expr_Production3};
    //make pairs for productions
    std::pair<std::string, std::vector<std::string>> expr_ProdPair1 = std::make_pair("Expr_", expr_Production1);
    std::pair<std::string, std::vector<std::string>> expr_ProdPair2 = std::make_pair("Expr_", expr_Production2);
    std::pair<std::string, std::vector<std::string>> expr_ProdPair3 = std::make_pair("Expr_", expr_Production3);
    //Add to int matching data structure
    productionInts[expr_ProdPair1] = 2;
    productionInts[expr_ProdPair2] = 3;
    productionInts[expr_ProdPair3] = 4;
    //Add Expr_ Productions to table
    productionTable["Expr_"] = expr_Productions;

    //Productions for Term
    std::vector<std::string> termProduction1 = {"Factor","Term_"};
    std::vector<std::vector<std::string>> termProductions = {termProduction1};
    //make pairs for productions
    std::pair<std::string, std::vector<std::string>> termProdPair = std::make_pair("Term", termProduction1);
    //Add to int matching data structure
    productionInts[termProdPair] = 5;
    //Add Term Productions to table
    productionTable["Term"] = termProductions;

    //Productions for Term_
    std::vector<std::string> term_Production1 = {"*","Factor","Term_"};
    std::vector<std::string> term_Production2 = {"/","Factor","Term_"};
    std::vector<std::string> term_Production3 = {"epsilon"};
    std::vector<std::vector<std::string>> term_Productions = {term_Production1, term_Production2, term_Production3};
    //make pairs for productions
    std::pair<std::string, std::vector<std::string>> term_ProdPair1 = std::make_pair("Term_", term_Production1);
    std::pair<std::string, std::vector<std::string>> term_ProdPair2 = std::make_pair("Term_", term_Production2);
    std::pair<std::string, std::vector<std::string>> term_ProdPair3 = std::make_pair("Term_", term_Production3);
    //Add to int matching data structure
    productionInts[term_ProdPair1] = 6;
    productionInts[term_ProdPair2] = 7;
    productionInts[term_ProdPair3] = 8;
    //Add Term_ Productions to table
    productionTable["Term_"] = term_Productions;

    //Productions for Factor
    std::vector<std::string> factorProduction1 = {"(","Expr",")"};
    std::vector<std::string> factorProduction2 = {"num"};
    std::vector<std::string> factorProduction3 = {"name"};
    std::vector<std::vector<std::string>> factorProductions = {factorProduction1, factorProduction2, factorProduction3};
    //make pairs for productions
    std::pair<std::string, std::vector<std::string>> factorProdPair1 = std::make_pair("Factor", factorProduction1);
    std::pair<std::string, std::vector<std::string>> factorProdPair2 = std::make_pair("Factor", factorProduction2);
    std::pair<std::string, std::vector<std::string>> factorProdPair3 = std::make_pair("Factor", factorProduction3);
    //Add to int matching data structure
    productionInts[factorProdPair1] = 9;
    productionInts[factorProdPair2] = 10;
    productionInts[factorProdPair3] = 11;
    //Add Factor Productions to table
    productionTable["Factor"] = factorProductions;

}

/**
 * _______________________________________________________________________________________________________________________
 * buildFirst: build FIRST sets table for all symbols
 * _______________________________________________________________________________________________________________________
 */
void LL1::buildFirst() {
    //mke vector to hold the first symbols for each symbol
    std::set<std::string> firsts4symbol;
    //Initialize the table
    for(std::string symbol : termsUepsUeol){ //for terminals, epsilon, and eol
        firsts4symbol.clear();
        firsts4symbol.insert(symbol);
        first[symbol] = firsts4symbol; // make the rhs entry for first for the given symbol that same symbol
    }
    firsts4symbol.clear();
    for(std::string symbol : nonTerminals){ //for non terminals
        first[symbol] = firsts4symbol; //make the rhs entry for first for the given symbol an empty set
    }

    bool firstAdditions = true; //set first Additions (variable to keep track of additions to first table) to true

    while(firstAdditions) { //while the first table is still being added to
        firstAdditions = false; //default firstAddition to false until there is an addition
        for (auto nonTermKey : productionTable) {//for each non terminal key
            for (auto production : nonTermKey.second) { //for each production for the nonterminal key
                int i = 0;
                int k = production.size() - 1;
                std::set<std::string> rhs; //create a set called rhs
                if ((!production.empty())) { //if the production is not empty
                    rhs = first[production[i]]; //set rhs equal to the values in the first table for the key equal to the first symbol in the right hand side expression
                    rhs.erase("epsilon"); //remove epsilon
                    while ((first[production[i]].count("epsilon") == 1) && i < k) { //while epsilon is in the first table for the current symbol
                        rhs.insert(first[production[i+1]].begin(), first[production[i+1]].end()); //insert first table of next symbol to rhs
                        rhs.erase("epsilon"); //remove epsilon symbol from rhs
                        i++; // go make next symbol the current symbol
                    }
                }
                if ((i == k && (first[production[k]].count("epsilon") == 1))) { //if we are at he last symbol in the production and the first table entry for that symbol contains an epsilon
                    rhs.insert("epsilon"); //put an epsilon in rhs
                }

                int prevSize = first[nonTermKey.first].size(); //get size of first table entry before inserting new rhs
                first[nonTermKey.first].insert(rhs.begin(), rhs.end()); //insert rhs into first table entry for the nonterminal key
                if(first[nonTermKey.first].size() > prevSize){  //if the size of the first table entry for this symbol is now bigger,
                    firstAdditions = true; //then the first table was added to on this iteration so set first additions to true
                }

            }
        }
    }

}

/**
 * _______________________________________________________________________________________________________________________
 * buildFollow: build FOLLOW sets table for all nonterminals
 * _______________________________________________________________________________________________________________________
 */
void LL1::buildFollow(){
    std::set<std::string> follows4symbol = {"eol"}; //create set to add to the righthand side of the follow tables for all entries, each will contain eol so the set contains eol
    for(auto nonTerm: nonTerminals){ //for each nonterminal
        follow[nonTerm] = follows4symbol; //add eol to its follow sets
    }

    bool followAdditions = true;  //set followAdditions (variable to keep track of additions to follow table) to true

    while(followAdditions){
        followAdditions = false;  //default followAdditions to false until there is an addition
        for(auto nonTerm : nonTerminals){ //for each nonTerminal
            for(auto production : productionTable[nonTerm]){ //for each production for each nonterminal
                std::set<std::string> trailer = follow[nonTerm]; //create trailer set and set it to current follow set for the nonterminal
                int k = production.size() - 1; //set k to the highest index for a symbol in the current production
                for(int i = k; i >= 0; i--){ //for all symbols in the production, start at the last and move towards the first
                    int prevFollowListSize = follow[production[i]].size(); //set the original size of the follow set for the current symbol in the current production
                    if(std::find(nonTerminals.begin(), nonTerminals.end(), production[i]) != nonTerminals.end()){ //if current symbol is a nonterminal
                        follow[production[i]].insert(trailer.begin(),trailer.end()); //insert the current trailer set into the follow set for the current symbol in the current production
                        if(first[production[i]].count("epsilon")) { //if the first set for the current symbol contains epsilon
                            trailer.insert(first[production[i]].begin(), first[production[i]].end()); //put the insert the first set for the current symbol into trailer
                            trailer.erase("epsilon"); //remove epsilon from trailer
                        }
                        else{ //otherwise set trailer to the first set for the current symbol
                            trailer.clear();
                            trailer.insert(first[production[i]].begin(), first[production[i]].end());
                        }
                    }
                    else{ //if the current symbol isnt a nonterminal
                        trailer.clear();
                        trailer.insert(first[production[i]].begin(), first[production[i]].end());

                    }
                    int currFollowListSize = follow[production[i]].size(); //save current size of the follow set for the current symbol
                    if(prevFollowListSize < currFollowListSize) { //if it is larger than the size of the follow list at the beginning of the loop iteration s
                        followAdditions = true; //set follow additions to true
                    }

                }
            }
        }
    }
}

/**
 * _______________________________________________________________________________________________________________________
 * buildFirstPlus: build FIRS+ sets table for all productions
 * _______________________________________________________________________________________________________________________
 */
void LL1::buildFirstPlus() {
    for( auto nonTermA: productionTable){ // for each non terminal paired with its vector of productions
        for ( auto production : nonTermA.second){ //for each production within the specific nonterminals vector of productions
            std::pair<std::string, std::vector<std::string>> firstPlusLhs = std::make_pair(nonTermA.first, production); //make the left hand side/production part of the entry in the first+ table
            std::set<std::string> firstPlusRhs;
            firstPlusRhs.insert(first[production[0]].begin(), first[production[0]].end()); //set the right hand side of the first plus entry to the first symbol of the productions first set
            if(first[production[0]].count("epsilon") == 1){ //if the first symbol in the production contains an epsilon
                firstPlusRhs.insert(follow[nonTermA.first].begin(), follow[nonTermA.first].end()); //insert the follow set to the right hand side of the first plus entry
            }
            firstPlus[firstPlusLhs] = firstPlusRhs; //add entry of production on left, first+ symbols on right to the first plus table
        }
    }
}


/**
 * _______________________________________________________________________________________________________________________
 * buildLL1: build LL1 table
 * _______________________________________________________________________________________________________________________
 */
void LL1::buildLL1() {
    //Initialize ll1 table
    std::vector<std::string> errorVector; //Make vector to hold error signs
    for (auto nonTerm : nonTermIdxs){ //for each nonterm (row in ll1 table
        errorVector.clear();
        for(auto term: termIdxs){ //for each term column in ll1 table
            errorVector.push_back("---"); //append ll1 to error vector/row
        }
        ll1Table.push_back(errorVector); //push back row of errors
    }
    for(auto nonTerm : productionTable){ //for all non terminals
        for( auto production : nonTerm.second){ //for each one of the non terminals productions
            std::pair<std::string, std::vector<std::string>> prodPair = std::make_pair(nonTerm.first, production); //make pair out of the production
            for(auto symbol : firstPlus[prodPair]){ //for each symbol in the productions first plus table
                if(symbol != "epsilon"){ //if the symbol is not epsilon
                    ll1Table[nonTermIdxs[nonTerm.first]][termIdxs[symbol]] = std::to_string(productionInts[prodPair]); //set the entry in the ll1 table for the current terminal and non terminal symbol to the production int
                }
            }
        }
    }

}

/**
 * _______________________________________________________________________________________________________________________
 * Getters for member variables
 * _______________________________________________________________________________________________________________________
 */
std::map<std::string, std::vector<std::vector<std::string>>> LL1::getProdTable(){
    return productionTable;
}

std::vector<std::string> LL1::getInsertionOrder(){
    return nonTerminals;
}

std::vector<std::string> LL1::getTerminals(){
    return terminals;
}

std::map<std::string, std::set<std::string>> LL1::getFirst(){
    return first;
}

std::map<std::string, std::set<std::string>> LL1::getFollow(){
    return follow;
}

std::map<std::pair<std::string,std::vector<std::string>>, std::set<std::string>> LL1::getFirstPlus(){
    return firstPlus;
}

std::vector<std::vector<std::string>> LL1::getLL1Table(){
    return ll1Table;
}

std::map<std::pair<std::string,std::vector<std::string>>, int> LL1::getProductionInts(){
    return productionInts;
};








int main(){
    //create LL1 object
    LL1 ll1;

    //use getters to use object attributes
    std::map<std::string, std::vector<std::vector<std::string>>> prodTable = ll1.getProdTable();
    std::vector<std::string> insertionOrder = ll1.getInsertionOrder();
    std::vector<std::string> terminals = ll1.getTerminals();
    std::map<std::string, std::set<std::string>> firstTable = ll1.getFirst();
    std::map<std::string, std::set<std::string>> followTable = ll1.getFollow();
    std::map<std::pair<std::string,std::vector<std::string>>, std::set<std::string>> firstPlusTable = ll1.getFirstPlus();
    std::vector<std::vector<std::string>> ll1Table = ll1.getLL1Table();
    std::map<std::pair<std::string,std::vector<std::string>>, int> productionInts = ll1.getProductionInts();

    //Print Productions Table
    std::cout << "PRODUCTIONS TABLE" << std::endl;
    for(auto key : insertionOrder){
        std::cout << key << std::endl;
        for(auto production : prodTable[key]){
            std::cout << "->";
            for(auto symbol : production){
                std::cout << " " << symbol;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "----------------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << std::endl;

    //Create vector for first table order
    std::vector<std::string> firstOrder;
    firstOrder.push_back("eol");
    firstOrder.push_back("epsilon");

    firstOrder.insert(firstOrder.begin(), terminals.begin(), terminals.end());
    firstOrder.insert(firstOrder.begin(), insertionOrder.begin(), insertionOrder.end());

    //Print First Table
    std::cout << "FIRST TABLE" << std::endl;
    for(auto symbol : firstOrder){
        std::cout << "FIRST[ " << symbol << " ]: ";
        int firstListSize = firstTable[symbol].size();
        int i = 0;
        for(auto firstSymbol : firstTable[symbol]){
            if(i < (firstListSize - 1)) {
                std::cout << firstSymbol << ", ";
            }
            else{
                std::cout << firstSymbol;
            }
            i++;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "----------------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << std::endl;

    //Print Follow Table
    std::cout << "FOLLOW TABLE" << std::endl;
    for(auto nonTerm : insertionOrder){
        std::cout << "FOLLOW[ " << nonTerm << " ]: ";
        int followListSize = followTable[nonTerm].size();
        int i = 0;
        for(auto followSymbol : followTable[nonTerm]){
            if(i < (followListSize - 1)) {
                std::cout << followSymbol << ", ";
            }
            else{
                std::cout << followSymbol;
            }
            i++;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "----------------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << std::endl;

    //Print FirstPlus Table

    //create vector of map associating productions and ints so they can be printed in the correct order
    std::vector<std::pair<std::pair<std::string,std::vector<std::string>>, int>> prodIntVector(productionInts.begin(), productionInts.end());
    std::sort(prodIntVector.begin(), prodIntVector.end(), [](const auto &left, const auto &right) {
        return left.second < right.second;
    });

    std::cout << "FIRST+ TABLE" << std::endl;
    for(auto production : prodIntVector){
        int firstPlusListSize = firstPlusTable[production.first].size(); //COME BACK
        std::cout << "FIRST+[ " << production.first.first <<  " -> ";
        for(auto rhsProdSymbol : production.first.second){
            std::cout << rhsProdSymbol << " ";
        }
        std::cout << "]: ";
        int i = 0;
        for(auto firstPlusSymbol : firstPlusTable[production.first]){ //COME BACK
            if(i < (firstPlusListSize - 1)){
                std::cout << firstPlusSymbol << ", ";
            }
            else{
                std::cout << firstPlusSymbol;
            }
            i++;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << "----------------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << std::endl;

    //Print LL1 table
    std::cout << "LL1 PARSE TABLE" << std::endl;
    std::cout << std::left << std::setw(8) << " ";
    std::cout << std::left << std::setw(6) << "eol";
    for(auto term : terminals){
        std::cout << std::left << std::setw(6) << term;
    }
    std::cout << std::endl;
    int i = 0; //loop counter
    for(auto ll1Row : ll1Table){
        std::cout << std::left << std::setw(8) << insertionOrder[i];
        for(auto production : ll1Row){
            std::cout << std::left << std::setw(6) << production;
        }
        std::cout << std::endl;
        i++;
    }

    return 0;
}
