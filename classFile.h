//
// Created by Ryan on 9/26/2019.
//

#ifndef EXTRACTCLASS_CLASSFILE_H
#define EXTRACTCLASS_CLASSFILE_H

#include "extract.h"
#include "Field.h"


class ClassFile {
public:
    int32_t magic_number;                    /* 4 bytes that should be 0xCAFEBABE or it compiled wrong       */
    u_int16_t majorVersion;                   /* Supported by JVM if <minV>.0 <= version <= <majV>.<minV>     */
    u_int16_t minorVersion;                   /* Supported by JVM if <minV>.0 <= version <= <majV>.<minV>     */
    u_int16_t constant_pool_count;            /* Number of items in constant pool plus one                    */
    std::vector<void *> *constant_pool;      /* Strings containing all constants, interfaces, and fields     */
    short access_flag;                       /* Mask determining the access properties of this class         */
    short this_class;                        /* Index of self CONSTANT_Class_info in constant pool           */
    short super_class;                       /* Index of superclass CONSTANT_Class_info or 0 if Object class */
    short interfaces_count;                  /* The number of direct superinterfaces to this class           */
    u_int16_t * interfaces;                   /* Each element is an index into constant pool for interfaces   */
    short fields_count;                      /* The number of field_info structures in the field table       */
    struct Field *fields;                           /* Each element of the table is a field_info structure          */
    short methods_count;                     /* The number of method_info structures in the method table     */
    char **methods;                          /* Each element is a method_info class for all declared methods */
    short attributes_count;                  /* The number of attributes in the attributes table             */
    char **attributes;                       /* Additional descriptive information                           */
    bool bigEndian;                          /* True if machine is bigEndian, false otherwise                */

    ClassFile(char *fileName);

    u_int8_t read1B(std::ifstream& inFile);

    uint16_t read2B(std::ifstream& inFile);

    uint32_t read4B(std::ifstream& inFile);

    void printThisClass();

    void printSuperClass();

    void printUTFEntry(uint16_t constant_index);

    void printInterfaces();

    void printFields();

    void printClassFile();

    void printConstantPool();
};


#endif //EXTRACTCLASS_CLASSFILE_H
