#include "extract.h"

const char * usage = "This tool extracts the fields of a Java .class file. In order to use it, you must enter: \n"
                     "./extractClass <.class File Name>";

int main(int argc, char** argv) {
    if (argc != 2){
        std::cout << usage << std::endl;
        exit(1);
    }
    classFile classfile = classFile(argv[1]);
    return 0;
}