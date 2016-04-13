#ifndef TOKEN
#define TOKEN

#include <string>

enum TokenType{
    KEYWORD,
    SYMBOL,
    IDENTIFIER,
    INT_CONST,
    ERROR
};

class Token{
public:
    Token();

    TokenType tokenType;
    std::string keyword;
    std::string symbol;
    std::string identifier;
    int intVal;

    int lineCount;
    int columnCount;
};

#endif // TOKEN

