#include "Token.h"

Token::Token(){
    tokenType = ERROR;

    keyword = "";
    symbol = ' ';
    identifier = "";
    intVal = 0;

    lineCount = 0;
    errorCode = 1;
}

