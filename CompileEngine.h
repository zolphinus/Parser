#include <list>
#include "Token.h"

class CompileEngine{

public:
    CompileEngine();
    ~CompileEngine();

    void recieveTokens(std::list<Token*>*& newTokenList);
    void recursiveParse();

private:
    std::list<Token*>* tokenList;
    std::list<Token*>::iterator curToken;
    std::list<Token*>::iterator nextToken;

    int errorCode;
    int errorLine;
    int errorCol;

    void advanceToken();
    void getNextToken();
    void outputToken();

    void checkStatementList();
    bool checkStatement();
    bool checkExpression();
    bool checkIdentifier();
    bool checkOperation();
    bool checkFactor();
    bool checkComma();

    Token* currentToken;
    Token* tokenNext;

};
