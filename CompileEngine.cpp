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
    }else{
        //tried to advance token but no more tokens remain
        currentToken = *tokenList->end();
        currentToken->errorCode = 12;
        throw(currentToken);
    }
}

bool CompileEngine::checkIdentifier(){
    if(currentToken->tokenType == IDENTIFIER){
        advanceToken();
        return true;
    }
    return false;
}

bool CompileEngine::checkComma(){
    if(currentToken->tokenType == SYMBOL){
        if(currentToken->symbol == ","){
            advanceToken();
            return true;
        }
    }

    return false;
}

bool CompileEngine::checkExpression(){
    bool valid_factor = checkFactor();
    if(valid_factor){
        bool valid_operation = checkOperation();
        if(valid_operation){
            while(valid_operation){
                valid_factor = checkFactor();
                if(valid_factor){
                    valid_operation = checkOperation();
                }else{
                    //if we have an operation we MUST have a factor
                    currentToken->errorCode = 14;
                    throw(currentToken);
                }
            }
            return true;

        }else{
            //we need a minimum of one factor, but do not care if we lack an operation
            return true;
        }
    }

    return false;
}

bool CompileEngine::checkFactor(){
    if(currentToken->tokenType == INT_CONST){
        advanceToken();
        return true;
    }

    if(currentToken->tokenType == IDENTIFIER){
        advanceToken();
        return true;
    }

    if(currentToken->tokenType == SYMBOL){
        if(currentToken->symbol == "("){
            advanceToken();

            bool valid_expression = checkExpression();

            if(valid_expression){
                if(currentToken->tokenType == SYMBOL){
                    if(currentToken->symbol == ")"){
                        advanceToken();
                        return true;
                    }else{
                        return false;
                    }
                }else{
                    return false;
                }
            }else{
                return false;
            }
        }else{
            return false;
        }
    }

    return false;
}

bool CompileEngine::checkOperation(){
    if(currentToken->tokenType == SYMBOL){
        if(currentToken->symbol == "+" || currentToken->symbol == "-"){
            advanceToken();
            return true;
        }else{
            return false;
        }
    }

    return false;
}

bool CompileEngine::checkStatement(){
    //check for a keyword
    if(currentToken->tokenType == KEYWORD){
        if(currentToken->keyword == "READ"){
            advanceToken();
            if(currentToken->tokenType == SYMBOL){
                if(currentToken->symbol == "("){
                    advanceToken();
                    bool valid_identifier = checkIdentifier();
                    if(valid_identifier){
                        bool valid_comma = checkComma();
                        while(valid_comma){
                            valid_identifier = checkIdentifier();
                            if(valid_identifier){
                                valid_comma = checkComma();

                            }else{
                                //must have valid identifier after a comma
                                currentToken->errorCode = 11;
                                throw(currentToken);
                            }
                        }
                        if(currentToken->tokenType == SYMBOL){
                            if(currentToken->symbol == ")"){
                                advanceToken();
                                if(currentToken->tokenType == SYMBOL){
                                    if(currentToken->symbol == ";"){
                                        advanceToken();
                                        return true;
                                    }else{
                                        currentToken->errorCode = 8;
                                        throw(currentToken);
                                    }
                                }else{
                                    currentToken->errorCode = 8;
                                    throw(currentToken);
                                }
                            }else{
                                currentToken->errorCode = 10;
                                throw(currentToken);
                            }
                        }else{
                            currentToken->errorCode = 10;
                            throw(currentToken);
                        }

                    }else{
                        currentToken->errorCode = 11;
                        throw(currentToken);
                    }
                }else{
                    currentToken->errorCode = 9;
                    throw(currentToken);
                }
            }else{
                currentToken->errorCode = 9;
                throw(currentToken);
            }

        }else if (currentToken->keyword == "WRITE"){
            advanceToken();
            if(currentToken->tokenType == SYMBOL){
                if(currentToken->symbol == "("){
                    advanceToken();
                    bool valid_expression = checkExpression();
                    if(valid_expression){
                        bool valid_comma = checkComma();
                        while(valid_comma){
                            valid_expression = checkExpression();
                            if(valid_expression){
                                valid_comma = checkComma();
                            }else{
                                //must have a valid expression after a comma
                                currentToken->errorCode = 7;
                                throw(currentToken);
                            }
                        }

                        if(currentToken->tokenType == SYMBOL){
                            if(currentToken->symbol == ")"){
                                advanceToken();
                                if(currentToken->tokenType == SYMBOL){
                                    if(currentToken->symbol == ";"){
                                        advanceToken();
                                        return true;
                                    }else{
                                        currentToken->errorCode = 8;
                                        throw(currentToken);
                                    }
                                }else{
                                    currentToken->errorCode = 8;
                                    throw(currentToken);
                                }
                            }else{
                                currentToken->errorCode = 10;
                                throw(currentToken);
                            }
                        }else{
                            currentToken->errorCode = 10;
                            throw(currentToken);
                        }

                    }else{
                        currentToken->errorCode = 7;
                        throw(currentToken);
                    }

                }else{
                    currentToken->errorCode = 9;
                    throw(currentToken);
                }
            }else{
                currentToken->errorCode = 9;
                throw(currentToken);
            }

        }else{
            //we have a keyword that isn't expected
            if(currentToken->keyword == "BEGIN"){
                currentToken->errorCode = 5;
                throw(currentToken);
            }else{
                //must be the end statement
                return false;
            }
        }
    }else if (currentToken->tokenType == IDENTIFIER){
        advanceToken();

        if(currentToken->tokenType == SYMBOL){
            if(currentToken->symbol == ":="){
                advanceToken();

                bool valid_expression = checkExpression();
                if(valid_expression){
                    if(currentToken->tokenType == SYMBOL){
                        if(currentToken->symbol == ";"){
                            advanceToken();
                            return true;
                        }else{
                            currentToken->errorCode = 8;
                            throw(currentToken);
                        }
                    }else{
                        currentToken->errorCode = 8;
                        throw(currentToken);
                    }
                }else{
                    currentToken->errorCode = 7;
                    throw(currentToken);
                }
            }else{
                currentToken->errorCode = 6;
                throw(currentToken);
            }
        }else{
            currentToken->errorCode = 6;
            throw(currentToken);
        }
    }

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
        std::cout << "ERROR CODE: " << errorCode << std::endl;
        std::cout << "LINE: " << errorLine << std::endl;
        std::cout << "COL:  " << errorCol << std::endl;
        std::cout << "word: " << e->keyword << std::endl;

    }
}
