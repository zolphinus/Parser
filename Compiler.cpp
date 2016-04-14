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
    std::cout << strippedFileName << std::endl;

    tokenFile = strippedFileName + ".token";

    //makes a list that contains the tokens
    tokenizer.getTokens(tokenFile, tokenList);


    //parse file, return error object
    compileEngine.recieveTokens(tokenList);

    //if error object has error, report it and the line



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

