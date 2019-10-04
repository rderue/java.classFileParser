//
// Created by Ryan on 9/26/2019.
//


#include "classFile.h"



ClassFile::ClassFile(char *fileName) {
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
    /*                      Get Access Flags                       */

    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
    access_flag = twoByteBuffer;
    //access_flag = (bigEndian ? twoByteBuffer : swapEndian16(twoByteBuffer)); todo figure out why this is little endian
    printAccessTypes();
    /*                      Get This Class                         */

    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
    //this_class = (bigEndian ? twoByteBuffer : swapEndian16(twoByteBuffer)); todo figure out why this is little endian
    this_class = twoByteBuffer;
    printThisClass();
    /*                      Get Super Class                         */

    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
    //super_class = (bigEndian ? twoByteBuffer : swapEndian16(twoByteBuffer)); todo figure out why this is little endian
    super_class = twoByteBuffer;
    printSuperClass();
    /*                      Get Fields Count                         */

    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
    fields_count = (bigEndian ? twoByteBuffer : swapEndian16(twoByteBuffer));

    /*                      Get Fields[]                         */
    //TODO: Implement Fields

    /*                      Get Methods Count                         */

    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
    methods_count = (bigEndian ? twoByteBuffer : swapEndian16(twoByteBuffer));

    /*                      Get Methods[]                         */
    //TODO: Implement Methods

    /*                      Get Attributes Count                         */

    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
    methods_count = (bigEndian ? twoByteBuffer : swapEndian16(twoByteBuffer));

    /*                      Get Attributes[]                         */
    //TODO: Implement Methods
}



uint16_t swapEndian16(uint16_t littleEndianInt){
    return (littleEndianInt >> 8 | littleEndianInt << 8);
}




