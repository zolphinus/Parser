#include "Tokenizer.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

Tokenizer::Tokenizer(){
    //testID();

    initializeLists();
    //testIDLookup();
    //testNonIDLookup();

}


void Tokenizer::getTokens(std::string tokenFile, std::list<Token*>*& tokenList){
    //open ifstream with tokenFile + ".token"
    std::ifstream fin(tokenFile.c_str());
    char firstChar;
    bool validIDcharacter = false;
    std::string currentToken;
    TokenType tokenType;
    Token* newToken;
    int lineCount = 0;
    int columnCount = -1;

    if(fin.is_open()){
    //parse the first char
    while(fin.get(firstChar)){
        if(firstChar != '$'){
            columnCount++;
            newToken = new Token();
            tokenType = ERROR;

            currentToken = ""; //resets the token building string

            firstChar = toupper(firstChar);

            validIDcharacter = validIDchar(firstChar, FIRST);
            if(validIDcharacter){
                currentToken += firstChar;
                while(validIDcharacter){
                    //builds string until invalid char is found
                    tokenType = lookUpKeyword(currentToken);
                    if(tokenType == KEYWORD){
                        newToken->keyword = currentToken;
                        break;
                    }

                    //check if currentToken is a keyword, if so, say so and break
                    firstChar = fin.peek();
                    validIDcharacter = validIDchar(firstChar, NOT_FIRST);
                    if(validIDcharacter){
                        fin.get(firstChar);
                        firstChar = toupper(firstChar);
                        columnCount++;
                        currentToken += firstChar;
                    }
                }

                if(tokenType == IDENTIFIER){
                    newToken->identifier = currentToken;
                }

            }else{
                //do the stuff for symbols here


                tokenType = lookUpInt(firstChar);

                //should be either INT_CONST or ERROR
                if(tokenType == INT_CONST){
                    bool stillDigit = true;
                    while(stillDigit){
                        firstChar = fin.peek();
                        if(isdigit(firstChar)){
                            fin.get(firstChar);
                            columnCount++;
                            currentToken += firstChar;
                        }else{
                           stillDigit = false;
                        }
                    }
                    newToken->intVal = atoi(currentToken.c_str());
                }else{
                    //this is the case where we check symbols

                    std::string symbol_string;
                    symbol_string.push_back(firstChar);



                    tokenType = lookUpSymbol(symbol_string);
                    if (tokenType == SYMBOL){
                        //this will check for every single character symbol
                        newToken->symbol = symbol_string;
                    }else{
                        //otherwise we need to check if we have an assignment operator

                        if (firstChar == ':'){
                            fin.get(firstChar);
                            columnCount++;
                            symbol_string.push_back(firstChar);

                            tokenType = lookUpSymbol(symbol_string);
                            newToken->symbol = symbol_string;
                        }
                    }

                }
            }

            if(newToken->tokenType != ERROR){
                newToken->errorCode = 0;
            }
            newToken->tokenType = tokenType;
            newToken->lineCount = lineCount;
            newToken->columnCount = columnCount;
            //add to the list
            tokenList->push_back(newToken);

            if(newToken->tokenType == ERROR){
                //ends when we find the first error
                break;
            }
        }else{
            //increment line count
            lineCount++;

            //reset column counter
            columnCount = -1;
        }
    }


    //while not EoF

    //also test for EoF during smaller cases

    //first test if it's an identifier legal character.

    //if it is, build until you hit an illegal character or EOF

    //test built string against keywords, if not, then identifier
        fin.close();
    }else{
        std::cout << "fail" << std::endl;
    }





    //if an illegal character is first, first check type of token


    //symbol

    //int const

    //string constant

    //add the created token to the tokenList


}

void Tokenizer::printTokens(std::list<Token*>*& tokenList){
    std::list<Token*>::iterator curToken;
    curToken = tokenList->begin();
    Token* temp = NULL;

    for(curToken; curToken != tokenList->end(); curToken++){
        temp = *curToken;

        if(temp->tokenType == KEYWORD){
            std::cout << temp->keyword << std::endl;
        }

        if(temp->tokenType == INT_CONST){
            std::cout << temp->intVal << std::endl;
        }

        if(temp->tokenType == IDENTIFIER){
            std::cout << temp->identifier << std::endl;
        }

        if(temp->tokenType == SYMBOL){
            std::cout << temp->symbol << std::endl;
        }
    }

}

void Tokenizer::initializeLists(){
    keywordList.push_back("BEGIN");
    keywordList.push_back("WRITE");
    keywordList.push_back("READ");
    keywordList.push_back("END");

    symbolList.push_back("(");
    symbolList.push_back(")");
    symbolList.push_back(",");
    symbolList.push_back(";");
    symbolList.push_back("+");
    symbolList.push_back("-");
    symbolList.push_back(":=");

}



void Tokenizer::testIDLookup(){

    Token testToken;
    testToken.tokenType = lookUpKeyword("boolean");

    std::cout << "booleaan" << std::endl;
    if(testToken.tokenType == KEYWORD){
        std::cout << "KEYWORD" << std::endl;
    }

    if(testToken.tokenType == IDENTIFIER){
        std::cout << "IDENTIFIER" << std::endl;
    }

    testToken.tokenType = lookUpKeyword("boolean");
    std::cout << "boolean" << std::endl;
    if(testToken.tokenType == KEYWORD){
        std::cout << "KEYWORD" << std::endl;
    }

    if(testToken.tokenType == IDENTIFIER){
        std::cout << "IDENTIFIER" << std::endl;
    }
}

TokenType Tokenizer::lookUpKeyword(std::string curToken){
    TokenType tokenType = IDENTIFIER;
    //lookup keyword
    std::list<std::string>::iterator it;

    for(it = keywordList.begin();
        it != keywordList.end(); it++){
        if(curToken == *it){
            tokenType = KEYWORD;
            return tokenType;
        }
    }
    return tokenType;
}

TokenType Tokenizer::lookUpInt(char firstChar){
    TokenType tokenType = ERROR;

    //else if first char is a digit, set to digit
    if(isdigit(firstChar)){
        tokenType = INT_CONST;
        return tokenType;
    }

    return tokenType;
}

TokenType Tokenizer::lookUpSymbol(std::string curToken){
    TokenType tokenType = ERROR;
    //if token found in symbols, set to symbol
    std::list<std::string>::iterator it;

    for(it = symbolList.begin();
        it != symbolList.end(); it++){
        if(curToken == *it){
            tokenType = SYMBOL;
            return tokenType;
        }
    }

    return tokenType;
}

void Tokenizer::testID(){
    bool test = false;
    ID_RULE id_rule = FIRST;

    test = this->validIDchar('5', id_rule);
    std::cout << "Testing 5, first " << test << std::endl;

    test = validIDchar('_', id_rule);
    std::cout << "Testing _, first " << test << std::endl;

    test = validIDchar('a', id_rule);
    std::cout << "Testing a, first " << test << std::endl;

    test = validIDchar('?', id_rule);
    std::cout << "Testing ?, first " << test << std::endl;

    id_rule = NOT_FIRST;

    test = validIDchar('5', id_rule);
    std::cout << "Testing 5, second " << test << std::endl;

    test = validIDchar('_', id_rule);
    std::cout << "Testing _, second " << test << std::endl;

    test = validIDchar('a', id_rule);
    std::cout << "Testing a, second " << test << std::endl;

    test = validIDchar('?', id_rule);
    std::cout << "Testing ?, second " << test << std::endl;
}

bool Tokenizer::validIDchar(char curChar, ID_RULE idRule){


    if(idRule == FIRST){
        //if digit, then false
        if(isdigit(curChar)){
            return false;
        }
        //if letter true
        if(isalpha(curChar)){
            return true;
        }

        //if underscore true
        if(curChar == '_'){
            return true;
        }
    }else if(idRule == NOT_FIRST){
        //if letter, true
        if(isalpha(curChar)){
            return true;
        }
        //if underscore, true
        if(curChar == '_'){
            return true;
        }

        //if digit, true
        if(isdigit(curChar)){
            return true;
        }
    }
    return false;
}

