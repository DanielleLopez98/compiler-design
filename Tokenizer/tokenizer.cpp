#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>

enum Type{
    AlphaNumeric,
    Digit,
    Operator,
    Bracket,

};

struct Token{
    Type type;
    std::string data;

};

//Tokenize Line: adds each token to the token list which is passed in by reference to ensure it is continuously updated, returns an empty string if the line is blank, is a comment, or has no errors, if there is an error, returns an error message
std::string tokenize(std::string line, int lineCounter, std::vector<Token> &tokenList){
    //Get error message ready in case a condition results in returning the error message
    std::string lineNumString = std::to_string(lineCounter);
    std::string errorMessage = "Line " + lineNumString + ": ERROR - Cannot continue tokenizing line due to an invalid character or pattern of characters";

    //create sets of valid operators and valid brackets
    std::set<char> validOperators = {'+', '-', '^', '=', '*', '%', '/', ','};
    std::set<char> validBrackets = {'(', '[', '{', '}', ']', ')'};

    //While the line still exists
    while(!line.empty()){

        //If first characters of remaining string are "//" stop reading and return
        if(line[0] == '/' && line[1] == '/'){
            return ""; //return empty string
        }

        //If first character of remaining string is ' ' (space)
        else if(line[0] == ' '){
            line.erase(line.begin()); //erase the character from the remaining string and then continue
            continue; //start loop over with cut string

        }

        //If first character of remaining string is alphabetical
        else if((line[0] >= 'a' && line[0] <= 'z') || (line[0] >= 'A' && line[0] <= 'Z')){ //if first character is alphabetical
            //create a buffer to hold the letters
            std::string alphaBuffer;

            //add first char to alphaBuffer
            alphaBuffer.push_back(line[0]);
            int i = 1; //create i to index into string

            //While we are going into a valid index and the character at i is alphanumeric or an underscore, put character into buffer and increment index
            while ((line.length() > i) && ((line[i] >= 'a' && line[i] <= 'z') || (line[i] >= 'A' && line[i] <= 'Z') || (line[i] >= '0' && line[i] <= '9') || (line[i] == '_'))) { //While we are going into a valid index and the character at i is alpha numeric or an underscor// e
                alphaBuffer.push_back(line[i]); //put character into alphabuffer
                i++; //increment i
            }

            //Place buffer string into alphanumeric token and put into token list, and erase tokenized section from the line, then restart the while loop
            Token t; //create token
            t.type = AlphaNumeric; //set type to alphaNumeric
            t.data = alphaBuffer; //set data to alphaBuffer
            tokenList.push_back(t); //put token in the token list
            line.erase(line.begin(), (line.begin() + i)); //erase tokenized section from line
            continue; //start loop over with cut string
        }

        //If first character of remaining string is a number, or it is a decimal point immediately followed by a number
        else if(line[0] >= '0' && line[0] <= '9' || (line[0] == '.' && (line.length() > 1 && (line[1] >= '0' && line[1] <= '9')))){
            int decimalCount = 0; //variable to count the number of decimal points and ensure there is only one decimal at most per digit function
            if(line[0] == '.'){
                decimalCount++;
            }

            //create a buffer to hold the numbers and/or decimal points
            std::string digitBuffer;

            //add first char to digitBuffer
            digitBuffer.push_back(line[0]);
            int i = 1;//create i to index into string

            //While we are going into a valid index and the character at i is alphanumeric or an underscore, put character into buffer and increment index
            while ((line.length() > i) && ((line[i] >= '0' && line[i] <= '9') || (line[i] == '.'))) { //While we are going into a valid index and the character at i is a number or decimal point
                digitBuffer.push_back(line[i]); //put character into digitbuffer
                if(line[i] == '.'){ //If there is a decimal add to the decimal count
                    decimalCount++;
                }
                i++; //increment i
            }

            //if a letter immediately follows a digit (ie 123abc) it is an invalid pattern of characters so return an error message
            if(line.length() > i && ((line[0] >= 'a' && line[0] <= 'z') || (line[0] >= 'A' && line[0] <= 'Z'))){
                return errorMessage;
            }
            else if(decimalCount > 1){ //If there is more than one decimal in one digit it is an invalid pattern of characters so return an error message
                return errorMessage;
            }
            else{ //Otherwise create a token holding the digit, erase the tokenized section from the line, and restart the loop with the cut line
                Token t; //create token
                t.type = Digit; //set type to alphaNumeric
                t.data = digitBuffer; //set data to alphaBuffer
                tokenList.push_back(t); //put token in the token list
                line.erase(line.begin(), (line.begin() + i)); //erase tokenized section from line
                continue; //start loop over with cut string
            }
        }

        //If the first character of the remaining string is an operator
        else if (validOperators.count(line[0])){
            //create a token holding the digit, erase the tokenized section from the line, and restart the loop with the cut line
            Token t; //create token
            t.type = Operator; //set type to alphaNumeric
            t.data = line[0]; //set data to alphaBuffer
            tokenList.push_back(t); //put token in the token list
            line.erase(line.begin()); //erase tokenized section from line
            continue; //start loop over with cut string
        }

        //If the first character of the remaining string is a bracket
        else if (validBrackets.count(line[0])){
            //create a token holding the digit, erase the tokenized section from the line, and restart the loop with the cut line
            Token t; //create token
            t.type = Bracket; //set type to alphaNumeric
            t.data = line[0]; //set data to alphaBuffer
            tokenList.push_back(t); //put token in the token list
            line.erase(line.begin()); //erase tokenized section from line
            continue; //start loop over with cut string
        }

        //If none of the above conditions are true we are at an invalid character or pattern of characters
        else{
            return errorMessage;
        }
    }
    return "";
}



//Write main
int main(int argc, char* argv[]){

    //Open file
    std::ifstream file(argv[1]);
    //If unable to open file
    if (!file){
        //If unable to open file display error message and exit
        std::cerr << "File would not open" << std::endl;
        return 1;
    }

    std::string line; //holds the line in string form
    int lineCounter = 0; //counts the line
    std::vector<Token> tokenList; //maintains a list of all the tokens in the file

    //For each line in file tokenize line and put tokens in a string to be printed. If there is an error put the error in the string
    while(std::getline(file, line)) {
        lineCounter++;
        std::vector<Token> tokenList; //maintains a list of all the tokens in the line
        tokenList.clear(); //clear tokenList before processing each line
        std::string errorStr = tokenize(line, lineCounter, tokenList);
        if (errorStr.size() != 0) {
            std::cout << errorStr << std::endl;
        }
        else{ //otherwise there were no errors in the line
            if(!tokenList.empty()){ //if there are tokens in the list
                int i = 0; //set first index of token vector to 0
                while(i < tokenList.size()){ //while we are not at a newLine character
                    std::cout << tokenList[i].data << ",";
                    i++;
                }
                std::cout << std::endl;
            }
        }
    }



}