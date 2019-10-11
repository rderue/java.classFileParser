//
// Created by Ryan on 9/26/2019.
//

#ifndef EXTRACTCLASS_CLASSFILE_H
#define EXTRACTCLASS_CLASSFILE_H

#include "extract.h"



class ClassFile {
public:
    int32_t magic_number;                    /* 4 bytes that should be 0xCAFEBABE or it compiled wrong       */
    uint16_t majorVersion;                   /* Supported by JVM if <minV>.0 <= version <= <majV>.<minV>     */
    uint16_t minorVersion;                   /* Supported by JVM if <minV>.0 <= version <= <majV>.<minV>     */
    uint16_t constant_pool_count;            /* Number of items in constant pool plus one                    */
    std::vector<void *> *constant_pool;       /* Strings containing all constants, interfaces, and fields     */
    short access_flag;                       /* Mask determining the access properties of this class         */
    short this_class;                        /* Index of self CONSTANT_Class_info in constant pool           */
    short super_class;                       /* Index of superclass CONSTANT_Class_info or 0 if Object class */
    short interfaces_count;                  /* The number of direct superinterfaces to this class           */
    char **interfaces;                       /* Each element is an index into constant pool for interfaces   */
    short fields_count;                      /* The number of field_info structures in the field table       */
    char **fields;                           /* Each element of the table is a field_info structure          */
    short methods_count;                     /* The number of method_info structures in the method table     */
    char **methods;                          /* Each element is a method_info class for all declared methods */
    short attributes_count;                  /* The number of attributes in the attributes table             */
    char **attributes;                       /* Additional descriptive information                           */
    bool bigEndian;                          /* True if machine is bigEndian, false otherwise                */

    ClassFile(char *fileName);

    void printThisClass(){
            std::cout <<  "-------------------------------------\n" << "this: " << std::endl;
            (CONSTANT_Class_info *) constant_pool->at(this_class);
            std::cout << "\t";
            printUTFEntry((*((CONSTANT_Class_info *)constant_pool->at(this_class))).name_index);
            std::cout << std::endl;
    }

    void printSuperClass(){
        if (super_class == 0) return;
        std::cout <<  "-------------------------------------\n" << "super class: " << std::endl;
        (CONSTANT_Class_info *) constant_pool->at(super_class);
        std::cout << "\t";
        printUTFEntry((*((CONSTANT_Class_info *)constant_pool->at(super_class))).name_index);
        std::cout << std::endl;
    }

    void printUTFEntry(uint16_t constant_index){
        std::printf("%s", (*((CONSTANT_Utf8_info *)constant_pool->at(constant_index))).bytes);
    }

    void printConstantPool(){
        std::cout << std::dec << "--------------------------" << std::endl;
        std::cout << "Constant Pool" << std::endl;
        for (int i = 1; i <= constant_pool_count - 1; i++){
            if (*((char*) constant_pool->at(i)) == CONSTANT_Class){
                std::cout << "Entry " << i << ": Class" << std::endl;
                (CONSTANT_Class_info *) constant_pool->at(i);
                std::cout << "\t" << "NameAndType Index is: " << (*((CONSTANT_Class_info *)constant_pool->at(i))).name_index << " [";
                printUTFEntry((*((CONSTANT_Class_info *)constant_pool->at(i))).name_index);
                std::cout << "]" << std::endl;
            }

            else if (*((char*) constant_pool->at(i)) == CONSTANT_Integer){
                std::cout << "Entry " << i << ": Integer" << std::endl;
                (CONSTANT_Class_info *) constant_pool->at(i);
                std::cout << "\t" << "Bytes are: " << (*((CONSTANT_Integer_info *)constant_pool->at(i))).bytes;
                std::cout << "\n";
            }

            else if (*((char*) constant_pool->at(i)) == CONSTANT_Float){
                std::cout << "Entry " << i << ": Float" << std::endl;
                (CONSTANT_Float_info *) constant_pool->at(i);
                std::printf("\tBytes are: %f", (float) (*((CONSTANT_Float_info *)constant_pool->at(i))).bytes);
                std::cout << "\n";
            }

            else if (*((char*) constant_pool->at(i)) == CONSTANT_Double){
                std::cout << "Entry " << i << ": Float" << std::endl;
                (CONSTANT_Double_info *) constant_pool->at(i);
                std::printf("\tMSB Bytes are: %X\n", (int) (*((CONSTANT_Double_info *)constant_pool->at(i++))).high_bytes);
                std::printf("\tLSB Bytes are: %X", (int) (*((CONSTANT_Double_info *)constant_pool->at(i))).low_bytes);
                uint64_t value = (*((CONSTANT_Double_info *)constant_pool->at(i))).high_bytes << 32 |
                                 (*((CONSTANT_Double_info *)constant_pool->at(i))).low_bytes;
                std::printf("\tFloating Point is: %f", (double) value);
                std::cout << "\n";
            }

            else if (*((char*) constant_pool->at(i)) == CONSTANT_Long){
                std::cout << "Entry " << i << ": Long" << std::endl;
                (CONSTANT_Long_info *) constant_pool->at(i);
                std::printf("\tMSB Bytes are: %X\n", (int) (*((CONSTANT_Long_info *)constant_pool->at(i++))).high_bytes);
                std::printf("\tLSB Bytes are: %X", (int) (*((CONSTANT_Long_info *)constant_pool->at(i))).low_bytes);
                uint64_t value = (*((CONSTANT_Long_info *)constant_pool->at(i))).high_bytes << 32 |
                                 (*((CONSTANT_Long_info *)constant_pool->at(i))).low_bytes;
                std::printf("\tFloating Point is: %f", (double) value);
                std::cout << "\n";
            }

            else if (*((char*) constant_pool->at(i)) == CONSTANT_String) {
                std::cout << "Entry " << i << ": Class" << std::endl;
                (CONSTANT_String_info *) constant_pool->at(i);
                std::cout << "\t" << "String Index is: "
                          << (*((CONSTANT_String_info *) constant_pool->at(i))).string_index << " [";
                printUTFEntry((*((CONSTANT_String_info *) constant_pool->at(i))).string_index);
                std::cout << "]" << std::endl;
            }

                //TODO printing the actual name and class of this field gives memory errors--check initialization
            else if (*((char*) constant_pool->at(i)) == CONSTANT_Fieldref){
                std::cout << "Entry " << i << ": Field" << std::endl;
                int nat_ndx = (*((CONSTANT_Fieldref_info *) constant_pool->at(i))).name_and_type_index;
                int class_ndx = (*((CONSTANT_Fieldref_info *)constant_pool->at(i))).class_index;
                std::cout << "\t" << "Field of class : " <<
                          (*((CONSTANT_Fieldref_info *)constant_pool->at(i))).class_index << " [";
                printUTFEntry((*((CONSTANT_Class_info *)constant_pool->at(class_ndx))).name_index);
                std::cout << "]" << std::endl;
                std::cout << "\t" << "NameAndTypeIndex : " <<
                          (*((CONSTANT_Fieldref_info *)constant_pool->at(i))).name_and_type_index;
                std::cout << "\n\t\tName: ";
                printUTFEntry((*((CONSTANT_NameAndType_info *) constant_pool->at(nat_ndx))).name_index);
                std::cout << "\n\t\tDescription: ";
                printUTFEntry((*((CONSTANT_NameAndType_info *)constant_pool->at(nat_ndx))).descriptor_index);
                std::cout << std::endl;
            }

            else if (*((char*) constant_pool->at(i)) == CONSTANT_Methodref){
                std::cout << "Entry " << i << ": MethodRef" << std::endl;
                int class_ndx = (*((CONSTANT_Methodref_info *)constant_pool->at(i))).class_index;
                int nat_ndx = (*((CONSTANT_Methodref_info *)constant_pool->at(i))).name_and_type_index;
                std::cout << "\t" << "Method of Class : " <<
                          (*((CONSTANT_Methodref_info *)constant_pool->at(i))).class_index << " [";
                printUTFEntry((*((CONSTANT_Class_info *)constant_pool->at(class_ndx))).name_index);
                std::cout << "]" << std::endl;
                std::cout << "" << "\tNameAndType Index : " <<
                          (*((CONSTANT_Methodref_info *)constant_pool->at(i))).name_and_type_index;
                std::cout << "\n\t\tName:";
                printUTFEntry((*((CONSTANT_NameAndType_info *)constant_pool->at(nat_ndx))).name_index);
                std::cout << "\n\t\tDescription: ";
                printUTFEntry((*((CONSTANT_NameAndType_info *)constant_pool->at(nat_ndx))).descriptor_index);
                std::cout << std::endl;
            }

            else if (*((char*) constant_pool->at(i)) == CONSTANT_InterfaceMethodref){
                std::cout << "Entry " << i << ": InterfaceMethodRef" << std::endl;
                int class_ndx = (*((CONSTANT_InterfaceMethodref_info *)constant_pool->at(i))).class_index;
                int nat_ndx = (*((CONSTANT_InterfaceMethodref_info *)constant_pool->at(i))).name_and_type_index;
                std::cout << "\t" << "Method of Interface: " <<
                          (*((CONSTANT_InterfaceMethodref_info *)constant_pool->at(i))).class_index << " [";
                printUTFEntry((*((CONSTANT_Fieldref_info *)constant_pool->at(i))).name_and_type_index);
                std::cout << "]" << std::endl;
                std::cout <<  "\tNameAndType Index : " <<
                          (*((CONSTANT_InterfaceMethodref_info *)constant_pool->at(i))).name_and_type_index << " [";
                std::cout << "\n\t\tName:";
                printUTFEntry((*((CONSTANT_NameAndType_info *)constant_pool->at(nat_ndx))).name_index);
                std::cout << "\n\t\tDescription: ";
                printUTFEntry((*((CONSTANT_NameAndType_info *)constant_pool->at(nat_ndx))).descriptor_index);
                std::cout << std::endl;
            }

            else if (*((char*) constant_pool->at(i)) == CONSTANT_NameAndType){
                std::cout << "Entry " << i << ": NameAndType" << std::endl;
                std::cout << "\t" << "Name Index: " <<
                          (*((CONSTANT_NameAndType_info *)constant_pool->at(i))).name_index << " [";
                printUTFEntry((*((CONSTANT_NameAndType_info *)constant_pool->at(i))).name_index);
                std::cout << "]" << std::endl;
                std::cout << "\t" << "Descriptor Index : " <<
                          (*((CONSTANT_NameAndType_info *)constant_pool->at(i))).descriptor_index<< " [";
                //printUTFEntry((*((CONSTANT_NameAndType_info *)constant_pool->at(i))).descriptor_index);
                std::cout << "]" << std::endl;
            }

                //TODO implement reference kind and reference index
            else if (*((char*) constant_pool->at(i)) == CONSTANT_MethodHandle){
                std::cout << "Entry " << i << ": MethodHandle" << std::endl;
                std::cout << "\t" << "Reference Index: " <<
                          (*((CONSTANT_MethodHandle_info *)constant_pool->at(i))).reference_index << " [";
                //printUTFEntry((*((CONSTANT_NameAndType_info *)constant_pool->at(i))).name_index);
                std::cout << "Indirection of reference index not yet implemented]" << std::endl;
                std::cout << "\t" << "Reference Kind: " <<
                          (*((CONSTANT_MethodHandle_info *)constant_pool->at(i))).reference_kind<< " [";
                //printUTFEntry((*((CONSTANT_NameAndType_info *)constant_pool->at(i))).descriptor_index);
                std::cout << "Description of reference kind not yet implemented]" << std::endl;
            }

            else if (*((char*) constant_pool->at(i)) == CONSTANT_MethodType) {
                std::cout << "Entry " << i << ": MethodType" << std::endl;
                std::cout << "\t" << "Descriptor Index: " <<
                          (*((CONSTANT_MethodType_info *) constant_pool->at(i))).descriptor_index << " [";
                printUTFEntry((*((CONSTANT_MethodType_info *) constant_pool->at(i))).descriptor_index);
                std::cout << "]" << std::endl;
            }

            else if (*((char*) constant_pool->at(i)) == CONSTANT_InvokeDynamic) {
                std::cout << "Entry " << i << ": InvokeDynamic" << std::endl;
                int nat_ndx = (*((CONSTANT_InvokeDynamic_info *) constant_pool->at(i))).name_and_type_index;
                std::cout << "\t" << "Name and Type Index: " <<
                          (*((CONSTANT_InvokeDynamic_info *) constant_pool->at(i))).name_and_type_index << " [";
                printUTFEntry((*((CONSTANT_InvokeDynamic_info *) constant_pool->at(i))).name_and_type_index);
                std::cout << "]" << std::endl;
                std::cout << "\t\t << Name: ";
                printUTFEntry((*((CONSTANT_NameAndType_info *) constant_pool->at(nat_ndx))).name_index);
                std::cout << "\t\t << Description: ";
                printUTFEntry((*((CONSTANT_NameAndType_info *) constant_pool->at(nat_ndx))).descriptor_index);

            }

            else if (*((char*) constant_pool->at(i)) == CONSTANT_Utf8){
                std::cout << "Entry " << i << ": Utf-8" << std::endl;
                std::cout << "\t" << "Length is: " << (*((CONSTANT_Utf8_info *)constant_pool->at(i))).length << std::endl;
                std::cout << "\t" << "String is: ";
                printUTFEntry(i);
                std::cout << "\n";
            }
        }
    }
};


#endif //EXTRACTCLASS_CLASSFILE_H
