#include "Compiler.h"
#include <string>
#include <iostream>

int main(int argc, char* argv[]){
    Compiler testCompiler;

    if(argc != 2){
        std::cout << "There is an error in the number of arguments provided, please use the format:" << std::endl;
        std::cout << "Parser filename.txt" << std::endl;
    }else{
        testCompiler.compileFile(argv[1]);
    }
}
