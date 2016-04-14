#include "CompileEngine.h"
#include <iterator>
#include <iostream>

CompileEngine::CompileEngine(){
    errorCode = 0;
}



void CompileEngine::recieveTokens(std::list<Token*>*& newTokenList){
    //grabs the tokens and sets the current token iterator to the beginning
    this->tokenList = newTokenList;
    curToken = tokenList->begin();


    errorCode = 0;
    currentToken = *curToken;

    recursiveParse();
}

CompileEngine::~CompileEngine(){
    tokenList = NULL;
}

void CompileEngine::getNextToken(){
    nextToken = curToken;
    std::advance(nextToken, 1);
    tokenNext = *nextToken;

    if (tokenNext != NULL){
        if(tokenNext->tokenType = ERROR){
            tokenNext->errorCode = 1;
            throw(tokenNext);
        }
    }
}

void CompileEngine::outputToken(){
    if(currentToken->tokenType == KEYWORD){
        std::cout << "<KEYWORD>" << currentToken->keyword << "</KEYWORD>" << std::endl;
    }

    if(currentToken->tokenType == SYMBOL){
        std::cout << "<SYMBOL>" << currentToken->symbol << "</SYMBOL>" << std::endl;
    }

    if(currentToken->tokenType == IDENTIFIER){
        std::cout << "<IDENTIFIER>" << currentToken->identifier << "</IDENTIFIER>" << std::endl;
    }

    if(currentToken->tokenType == INT_CONST){
        std::cout << "<INT_CONST>" << currentToken->intVal << "</INT_CONST>" << std::endl;
    }
}

void CompileEngine::advanceToken(){
    if(curToken != tokenList->end()){
        this->curToken++;
        currentToken = *curToken;
    }
}

void CompileEngine::checkStatementList(){

}

bool CompileEngine::checkStatement(){

    return false;
}

void CompileEngine::checkStatementList(){
    bool valid_statement = checkStatement();

    if(valid_statement){
        //check as long as there are statements
        while(valid_statement){
            valid_statement = checkStatement();
        }

    }else{
        //language requires at least one valid statement
        currentToken->errorCode = 4;
        throw(currentToken);
    }

}

void CompileEngine::recursiveParse(){
    try{
        if(currentToken->tokenType == KEYWORD){
            if(currentToken->keyword == "BEGIN"){
                advanceToken();

                checkStatementList();

                if(currentToken->tokenType == KEYWORD){
                    if(currentToken->keyword == "END"){

                    }else{
                        currentToken->errorCode = 3;
                        throw(currentToken);
                    }
                }else{
                    currentToken->errorCode = 3;
                    throw(currentToken);
                }


            }else{
                currentToken->errorCode = 2;
                throw(currentToken);
            }

        }else{
            currentToken->errorCode = 2;
            throw(currentToken);
        }
    }
    catch (const Token* e){
        // catch will allow us to break out of the recursion stack and destroy all memory frames related to it
        //this lets us handle error codes without extra code bloat to verify if an error has been thrown
        std::cout << "TOKEN WAS CAUGHT" << std::endl;
        errorCode = e->errorCode;
        errorLine = e->lineCount;
        errorCol = e->columnCount;

    }
}
