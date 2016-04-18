#include "Compiler.h"
#include <iostream>

Compiler::Compiler(){
}

void Compiler::compileFile(std::string inputFile){
    std::string strippedFileName;
    std::string strippedFile;
    std::string tokenFile;
    tokenList = new std::list<Token*>();

    //strip the input file of comments and whitespace
    strippedFileName = stripFile(inputFile);

    tokenFile = strippedFileName + ".token";

    //makes a list that contains the tokens
    tokenizer.getTokens(tokenFile, tokenList);

    //tokenizer.printTokens(tokenList);

    //parse file, return error object
    compileEngine.recieveTokens(tokenList);

    printError(compileEngine.errorCode, compileEngine.errorLine, compileEngine.errorCol, inputFile);

    //if error object has error, report it and the line

}

void Compiler::printError(int errorCode, int errorLine, int errorColumn, std::string fileName){
    if(errorCode == 0){
        std::cout << "Parser completed with no errors." << std::endl;
    }else{
        std::ifstream fin(fileName.c_str());
        if(fin.is_open()){
            std::cout << "AN ERROR HAS OCCURRED WHILE PARSING THE FILE" << std::endl << std::endl;
            std::string tempString;
            for(int i = 0; i <= errorLine; i++){
                //grabs the error line
                std::getline(fin, tempString);
            }
            std::cout << tempString << std::endl;

            tempString = "";
            for(int i = 0; i < errorColumn; i++){
                tempString = tempString + " ";
            }
            tempString = tempString + "^";
            std::cout << tempString << std::endl << std::endl;

            //error codes

            switch(errorCode){
            case 1:
                std::cout << "Unknown token";
                break;
            case 2:
                std::cout << "Missing BEGING statement";
                break;
            case 3:
                std::cout << "Missing END statement";
                break;
            case 4:
                std::cout << "Missing statements after BEGIN statement";
                break;
            case 5:
                std::cout << "Unexpected instance of the BEGIN keyword";
                break;
            case 6:
                std::cout << "Expected the := symbol";
                break;
            case 7:
                std::cout << "Expected an expression";
                break;
            case 8:
                std::cout << "Expected ;";
                break;
            case 9:
                std::cout << "Expected (";
                break;
            case 10:
                std::cout << "Expected )";
                break;
            case 11:
                std::cout << "Expected an identifier";
                break;
            case 12:
                //this error should never show up unless there are bugs in the code
                std::cout << "Ran out of tokens to parse";
                break;
            case 13:
                std::cout << "Expected either a + or a - operator";
                break;
            case 14:
                std::cout << "Expected a factor";
                break;
            default:
                //this error should only occur if the code has bugs
                std::cout << "An undocumented error has occurred";
                break;
            }
            std::cout << std::endl;

        }else{
            std::cout << "An error occurred while attempting to display\nthe line the error occurred on" << std::endl;
        }

        std::cout << std::endl;
    }
}

std::string Compiler::stripFile(std::string inputFile){
    int dotLoc = 0;
    for(dotLoc = 0; dotLoc < inputFile.length(); dotLoc++){
        if(inputFile[dotLoc] == '.')
          break;
    }

    std::string strippedFile = inputFile.substr(0, dotLoc);
    std::string tokenFile = strippedFile + ".token";
    std::ifstream fin(inputFile.c_str());
    std::ofstream fout(tokenFile.c_str());

    if(fin.is_open()){
    char c;
    char lastChar = ' ';
    bool lineComment = false;
    bool blockComment = false;
    bool variable = false;

    while (fin.get(c))
    {
        variable = false;
        if(blockComment == true){
            if(c == '/' && lastChar == '*'){
                //turns off block comments
                blockComment = false;
                lineComment = false;
            }
        }else if(lineComment == true){
            if(c == '\n'){
                //turns off line comments
                lineComment = false;
            }
        }else if(c == '\n'){
            if(c == '\n'){
                lineComment = false;
            }
            //INSERT A $ as a newline marker for error tracking in tokenizer
            fout << '$';
        }else if(c == '\t'){
            //strips out tabs
        }else{
            if(c == '/'){
                char temp = fin.peek();
                if(temp == '/'){
                    //turns on line comments
                    lineComment = true;
                }else if(temp == '*'){
                    //turns on block comments
                    blockComment = true;
                }else{
                  //output


                  fout << c;
                }
            }else{
                if(c != ' '){
                    //strips out any leftover whitespaces
                    fout << c;
                }

            }
        }

        lastChar = c;
    }
        fin.close();
        fout.close();
    }else{
       std::cout << "the file could not be found" << std::endl;
    }


    return strippedFile;
}

