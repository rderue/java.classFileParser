//
// Created by Ryan on 9/26/2019.
//

#ifndef EXTRACTCLASS_CLASSFILE_H
#define EXTRACTCLASS_CLASSFILE_H
#include "extract.h"
#include <stdio.h>

class classFile {
    public:
        int32_t magic_number;                   /* 4 bytes that should be 0xCAFEBABE or it compiled wrong       */
        int16_t   majorVersion;                   /* Supported by JVM if <minV>.0 <= version <= <majV>.<minV>     */
        int16_t   minorVersion;                   /* Supported by JVM if <minV>.0 <= version <= <majV>.<minV>     */
        int16_t   constant_pool_count;            /* Number of items in constant pool plus one                    */
        char**  constant_pool;                  /* Strings containing all constants, interfaces, and fields     */
        short   access_flag;                    /* Mask determining the access properties of this class         */
        short   this_class;                     /* Index of self CONSTANT_Class_info in constant pool           */
        short   super_class;                    /* Index of superclass CONSTANT_Class_info or 0 if Object class */
        short   interfaces_count;               /* The number of direct superinterfaces to this class           */
        char**  interfaces;                     /* Each element is an index into constant pool for interfaces   */
        short   fields_count;                   /* The number of field_info structures in the field table       */
        char**  fields;                         /* Each element of the table is a field_info structure          */
        short   methods_count;                  /* The number of method_info structures in the method table     */
        char**  methods;                        /* Each element is a method_info class for all declared methods */
        short   attributes_count;               /* The number of attributes in the attributes table             */
        char**  attributes;                     /* Additional descriptive information                           */
        bool    bigEndian;                      /* True if machine is bigEndian, false otherwise                */

        classFile(char * fileName) {
            std::string filePath = std::string(std::experimental::filesystem::current_path().parent_path());
            filePath.append("/");
            filePath.append(fileName);
            std::cout << "Current path to " << filePath << std::endl;
            char a;
            std::ifstream inFile;
            inFile.open(filePath, std::ifstream::binary);
            if (!inFile) {
                std::cout << "The header file could not be found";
                exit(1);
            }
            inFile.read(reinterpret_cast<char *>(&magic_number), 4);
            bigEndian = (magic_number == 0xCAFEBABE);
            if (!bigEndian) {
                magic_number = __bswap_32(magic_number);
            }
            std::cout << "magic number: " << std::hex << magic_number << std::endl;

            int32_t versionInfo;
            inFile.read(reinterpret_cast<char *>(&versionInfo), 4);
            std::cout << versionInfo;
            if (!bigEndian) {
                minorVersion = (int16_t) (versionInfo >> 16);
                majorVersion = (int16_t) (versionInfo & 65535); // versionInfo & 0000 0000 0000 0000 1111 1111 1111 1111
                __bswap_16(minorVersion);
                __bswap_16(majorVersion);
            } else {
                minorVersion = (int16_t) (versionInfo & 65535); // versionInfo & 0000 0000 0000 0000 1111 1111 1111 1111
                majorVersion = (int16_t) (versionInfo >> 16);
            }
            std::cout << "major version: " << majorVersion << std::endl;
            std::cout << "minor version: " << minorVersion << std::endl;
        }
};


#endif //EXTRACTCLASS_CLASSFILE_H
