//
// Created by Ryan on 9/26/2019.
//

#ifndef EXTRACTCLASS_CLASSFILE_H
#define EXTRACTCLASS_CLASSFILE_H

#include "extract.h"

class classFile {
public:
    int32_t magic_number;                    /* 4 bytes that should be 0xCAFEBABE or it compiled wrong       */
    uint16_t majorVersion;                   /* Supported by JVM if <minV>.0 <= version <= <majV>.<minV>     */
    uint16_t minorVersion;                   /* Supported by JVM if <minV>.0 <= version <= <majV>.<minV>     */
    uint16_t constant_pool_count;            /* Number of items in constant pool plus one                    */
    std::vector<void *>* constant_pool;       /* Strings containing all constants, interfaces, and fields     */
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

    classFile(char *fileName) {
        uint8_t  oneByteBuffer;
        uint16_t twoByteBuffer;
        uint32_t fourByteBuffer;

        /*                            Set up necessary paths to get .class File                    */
        std::string filePath = std::string(std::experimental::filesystem::current_path().parent_path());
        filePath.append("/");
        filePath.append(fileName);
        std::ifstream inFile;
        inFile.open(filePath, std::ifstream::binary);
        if (!inFile) {
            std::cout << "The header file could not be found";
            exit(1);
        }

        /*                      Get Magic Number                    */
        inFile.read(reinterpret_cast<char *>(&magic_number), 4);
        bigEndian = (magic_number == 0xCAFEBABE);
        if (!bigEndian) {
            magic_number = __bswap_32(magic_number);
        }
        std::cout << "magic number: " << std::hex << magic_number << std::endl;

        /*                      Get Version Number                  */
        inFile.read(reinterpret_cast<char *>(&fourByteBuffer), 4);
        if (!bigEndian) {
            majorVersion = (unsigned short) (fourByteBuffer >> 16);
            minorVersion = (unsigned short) (fourByteBuffer & 65535); // versionInfo & 0000 ... 0000 1111 1111 1111 1111
            majorVersion = swapEndian16(majorVersion);
            minorVersion = swapEndian16(minorVersion);
        } else {
            majorVersion = (unsigned short) (fourByteBuffer & 65535); // versionInfo & 0000 ... 0000 1111 1111 1111 1111
            minorVersion = (unsigned short) (fourByteBuffer >> 16);
        }
        std::printf("major version: %u\n", majorVersion);
        std::printf("minor version: %u\n", minorVersion);

        /*                      Get Constant Pool                   */
        inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
        constant_pool_count = (bigEndian ? twoByteBuffer : swapEndian16(twoByteBuffer));
        std::printf("constant_pool_count: %u\n", constant_pool_count);
        constant_pool = new std::vector<void *>(constant_pool_count + 1);
        for (int i = 1; i <= constant_pool_count; i++){
            uint8_t tag;
            inFile.read(reinterpret_cast<char *>(&tag), 1);

            /*  Declare all necessary variables before the switch statement */

            CONSTANT_Class_info * class_ptr;
            CONSTANT_Fieldref_info * field_ptr;
            CONSTANT_Float_info * float_ptr;
            CONSTANT_Integer_info * int_ptr;
            CONSTANT_InterfaceMethodref_info * interfaceMethod_ptr;
            CONSTANT_InvokeDynamic_info * invdyn_ptr;
            CONSTANT_MethodHandle_info * methodhdl_ptr;
            CONSTANT_Methodref_info * method_ptr;
            CONSTANT_MethodType_info * method_type_ptr;
            CONSTANT_NameAndType_info * nat_ptr;
            CONSTANT_String_info * str_ptr;
            CONSTANT_Utf8_info * utf_ptr;
            CONSTANT_Long_info * long_ptr;
            CONSTANT_Double_info * dub_ptr;

            switch(tag) {
                case CONSTANT_Class:
                    class_ptr = (CONSTANT_Class_info *)malloc(sizeof(struct CONSTANT_Class_info));
                    class_ptr->tag=tag;
                    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
                    class_ptr->name_index = (bigEndian ? twoByteBuffer : swapEndian16(twoByteBuffer));
                    constant_pool->at(i) =(void*) class_ptr;
                    break;

                case CONSTANT_Fieldref:
                    field_ptr = (CONSTANT_Fieldref_info *)malloc(sizeof(struct CONSTANT_Fieldref_info));
                    field_ptr->tag=tag;
                    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
                    field_ptr->class_index = (bigEndian ? twoByteBuffer : swapEndian16(twoByteBuffer));
                    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
                    field_ptr->name_and_type_index = (bigEndian ? twoByteBuffer : swapEndian16(twoByteBuffer));
                    constant_pool->at(i) = ((void*) field_ptr);
                    break;

                case CONSTANT_Float:
                    float_ptr = (CONSTANT_Float_info *)malloc(sizeof(struct CONSTANT_Float_info));
                    float_ptr->tag=tag;
                    inFile.read(reinterpret_cast<char *>(&fourByteBuffer), 4);
                    float_ptr->bytes = (bigEndian ? fourByteBuffer : __bswap_32(fourByteBuffer));
                    constant_pool->at(i) = ((void*) float_ptr);
                    break;

                case CONSTANT_Integer:
                    int_ptr = (CONSTANT_Integer_info *)malloc(sizeof(struct CONSTANT_Integer_info));
                    int_ptr->tag=tag;
                    inFile.read(reinterpret_cast<char *>(&fourByteBuffer), 4);
                    int_ptr->bytes = (bigEndian ? fourByteBuffer : __bswap_32(fourByteBuffer));
                    constant_pool->at(i) = ((void*) int_ptr);
                    break;

                case CONSTANT_InterfaceMethodref:
                    interfaceMethod_ptr = (CONSTANT_InterfaceMethodref_info *)malloc(sizeof(struct CONSTANT_InterfaceMethodref_info));
                    interfaceMethod_ptr->tag=tag;
                    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
                    interfaceMethod_ptr->class_index = (bigEndian ? twoByteBuffer : swapEndian16(twoByteBuffer));
                    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
                    interfaceMethod_ptr->name_and_type_index = (bigEndian ? twoByteBuffer : swapEndian16(twoByteBuffer));
                    constant_pool->at(i) = ((void*) interfaceMethod_ptr);
                    break;

                case CONSTANT_InvokeDynamic:
                    invdyn_ptr = (CONSTANT_InvokeDynamic_info *)malloc(sizeof(struct CONSTANT_InvokeDynamic_info));
                    invdyn_ptr->tag=tag;
                    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
                    invdyn_ptr->bootstrap_method_attr_index = (bigEndian ? twoByteBuffer : swapEndian16(twoByteBuffer));
                    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
                    invdyn_ptr->name_and_type_index = (bigEndian ? twoByteBuffer : swapEndian16(twoByteBuffer));
                    constant_pool->at(i) = ((void*) invdyn_ptr);
                    break;

                case CONSTANT_MethodHandle:
                    methodhdl_ptr = (CONSTANT_MethodHandle_info *)malloc(sizeof(struct CONSTANT_MethodHandle_info));
                    methodhdl_ptr->tag=tag;
                    inFile.read(reinterpret_cast<char *>(&oneByteBuffer), 1);
                    methodhdl_ptr->reference_kind = (oneByteBuffer);
                    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
                    methodhdl_ptr->reference_index = (bigEndian ? twoByteBuffer : swapEndian16(twoByteBuffer));
                    constant_pool->at(i) = ((void*) methodhdl_ptr);
                    break;

                case CONSTANT_Methodref:
                    method_ptr = (CONSTANT_Methodref_info *)malloc(sizeof(struct CONSTANT_Methodref_info));
                    method_ptr->tag=tag;
                    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
                    method_ptr->class_index = (bigEndian ? twoByteBuffer : swapEndian16(twoByteBuffer));
                    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
                    method_ptr->name_and_type_index = (bigEndian ? twoByteBuffer : swapEndian16(twoByteBuffer));
                    constant_pool->at(i) = ((void*) method_ptr);
                    break;

                case CONSTANT_MethodType:
                    method_type_ptr = (CONSTANT_MethodType_info *)malloc(sizeof(struct CONSTANT_MethodType_info));
                    method_type_ptr->tag=tag;
                    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
                    method_type_ptr->descriptor_index = (bigEndian ? twoByteBuffer : swapEndian16(twoByteBuffer));
                    constant_pool->at(i) = ((void*) method_type_ptr);
                    break;

                case CONSTANT_NameAndType:
                    nat_ptr = (CONSTANT_NameAndType_info *)malloc(sizeof(struct CONSTANT_NameAndType_info));
                    nat_ptr->tag=tag;
                    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
                    nat_ptr->name_index = (bigEndian ? twoByteBuffer : swapEndian16(twoByteBuffer));
                    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
                    nat_ptr->descriptor_index = (bigEndian ? twoByteBuffer : swapEndian16(twoByteBuffer));
                    constant_pool->at(i) = ((void*) nat_ptr);
                    break;

                case CONSTANT_String:
                    str_ptr = (CONSTANT_String_info *)malloc(sizeof(struct CONSTANT_String_info));
                    str_ptr->tag=tag;
                    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
                    str_ptr->string_index = (bigEndian ? twoByteBuffer : swapEndian16(twoByteBuffer));
                    constant_pool->at(i) = ((void*) str_ptr);

                    break;
                case CONSTANT_Utf8:
                    utf_ptr = (CONSTANT_Utf8_info *)malloc(sizeof(struct CONSTANT_Utf8_info));
                    utf_ptr->tag=tag;
                    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
                    utf_ptr->length = (bigEndian ? twoByteBuffer : swapEndian16(twoByteBuffer));
                    utf_ptr->bytes = (uint8_t *) malloc(sizeof(uint8_t) * utf_ptr->length + 1);
                    for (int i = 0; i < utf_ptr->length; i++){
                        inFile.read(reinterpret_cast<char *>(&oneByteBuffer), 1);
                        utf_ptr->bytes[i] = oneByteBuffer;
                    }
                    utf_ptr->bytes[i] = NULL;
                    constant_pool->at(i) = ((void*) utf_ptr);
                    break;

                case CONSTANT_Long:
                    long_ptr = (CONSTANT_Long_info *)malloc(sizeof(struct CONSTANT_Long_info));
                    long_ptr->tag=tag;
                    inFile.read(reinterpret_cast<char *>(&fourByteBuffer), 4);
                    long_ptr->high_bytes = (bigEndian ? fourByteBuffer : __bswap_32(fourByteBuffer));
                    inFile.read(reinterpret_cast<char *>(&fourByteBuffer), 4);
                    long_ptr->low_bytes = (bigEndian ? fourByteBuffer : __bswap_32(fourByteBuffer));
                    constant_pool->at(i++) = ((void*) long_ptr);
                    constant_pool->at(i) = (NULL);
                    break;

                case CONSTANT_Double:
                    dub_ptr = (CONSTANT_Double_info *)malloc(sizeof(struct CONSTANT_Double_info));
                    dub_ptr->tag=tag;
                    inFile.read(reinterpret_cast<char *>(&fourByteBuffer), 4);
                    dub_ptr->high_bytes = (bigEndian ? fourByteBuffer : __bswap_32(fourByteBuffer));
                    inFile.read(reinterpret_cast<char *>(&fourByteBuffer), 4);
                    dub_ptr->low_bytes = (bigEndian ? fourByteBuffer : __bswap_32(fourByteBuffer));
                    constant_pool->at(i++) = ((void*) dub_ptr);
                    constant_pool->at(i) = (NULL);
                    break;
            }
        }
        printConstantPool();
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


    uint16_t swapEndian16(uint16_t littleEndianInt){
        return (littleEndianInt >> 8 | littleEndianInt << 8);
    }
};


#endif //EXTRACTCLASS_CLASSFILE_H
