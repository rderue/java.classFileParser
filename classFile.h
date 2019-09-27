//
// Created by Ryan on 9/26/2019.
//

#ifndef EXTRACTCLASS_CLASSFILE_H
#define EXTRACTCLASS_CLASSFILE_H
#include "extract.h"

class classFile {
    public:
        int magic_number;                   /* 4 bytes that should be 0xCAFEBABE or it compiled wrong       */
        short majorVersion;                 /* Supported by JVM if <minV>.0 <= version <= <majV>.<minV>     */
        short minorVersion;                 /* Supported by JVM if <minV>.0 <= version <= <majV>.<minV>     */
        short constant_pool_count;          /* Number of items in constant pool plus one                    */
        char* constant_pool[];              /* Strings containing all constants, interfaces, and fields     */
        short access_flag;                  /* Mask determining the access properties of this class         */
        short this_class;                   /* Index of self CONSTANT_Class_info in constant pool           */
        short super_class;                  /* Index of superclass CONSTANT_Class_info or 0 if Object class */
        short interfaces_count;             /* The number of direct superinterfaces to this class           */
        char* interfaces[];                 /* Each element is an index into constant pool for interfaces   */
        short fields_count;                 /* The number of field_info structures in the field table       */
        char* fields[];                     /* Each element of the table is a field_info structure          */
        short methods_count;                /* The number of method_info structures in the method table     */
        char* methods[];                    /*
        short attributes_count;
        char* attributes[];

        classFile(char * fileName)
        {
            std::ifstream inFile;
            inFile.open(fileName);
            if (!inFile) {
                std::cout << "The header file could not be found";
                exit(1);
            }

            inFile.read(&magic_number, 4)
        }
};


#endif //EXTRACTCLASS_CLASSFILE_H
