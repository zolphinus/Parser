#include <fstream>
#include <string>
#include "Tokenizer.h"
#include "CompileEngine.h"
#include "Token.h"
#include <List>

class Compiler{
public:
    Compiler();
    void compileFile(std::string);

private:
    std::string stripFile(std::string); //pre pass to remove comments and whitespace
    void printError(int errorCode, int lineNumber, int columnNumber, std::string fileName);

    Tokenizer tokenizer; //makes tokens then adds to list for parser to parse
    CompileEngine compileEngine;

    std::list<Token*>* tokenList;
};

