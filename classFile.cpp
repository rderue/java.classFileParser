//
// Created by Ryan on 9/26/2019.
//

#include "extract.h"

ClassFile::ClassFile(char *fileName) {
    uint16_t twoByteBuffer;

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

    /*                      Get Version Number                  */
    //fourByteBuffer = read4B(inFile);
    /*if (bigEndian) {
        majorVersion = (unsigned short) (fourByteBuffer >> 16);
        minorVersion = (unsigned short) (fourByteBuffer & 65535); // versionInfo & 0000 ... 0000 1111 1111 1111 1111
        majorVersion = swapEndian16(majorVersion);
        minorVersion = swapEndian16(minorVersion);
    } else {
        majorVersion = (unsigned short) (fourByteBuffer & 65535); // versionInfo & 0000 ... 0000 1111 1111 1111 1111
        minorVersion = (unsigned short) (fourByteBuffer >> 16);
    }*/
    minorVersion = read2B(inFile);
    majorVersion = read2B(inFile);

    /*                      Get Constant Pool                   */
    constant_pool_count = read2B(inFile);
    constant_pool = new std::vector<void *>(constant_pool_count + 1);
    for (int i = 1; i <= constant_pool_count; i++){
        uint8_t tag = read1B(inFile);

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
                class_ptr->name_index = read2B(inFile);
                constant_pool->at(i) =(void*) class_ptr;
                break;

            case CONSTANT_Fieldref:
                field_ptr = (CONSTANT_Fieldref_info *)malloc(sizeof(struct CONSTANT_Fieldref_info));
                field_ptr->tag=tag;
                field_ptr->class_index = read2B(inFile);
                field_ptr->name_and_type_index = read2B(inFile);
                constant_pool->at(i) = ((void*) field_ptr);
                break;

            case CONSTANT_Float:
                float_ptr = (CONSTANT_Float_info *)malloc(sizeof(struct CONSTANT_Float_info));
                float_ptr->tag=tag;
                float_ptr->bytes = read4B(inFile);
                constant_pool->at(i) = ((void*) float_ptr);
                break;

            case CONSTANT_Integer:
                int_ptr = (CONSTANT_Integer_info *)malloc(sizeof(struct CONSTANT_Integer_info));
                int_ptr->tag=tag;
                int_ptr->bytes = read4B(inFile);
                constant_pool->at(i) = ((void*) int_ptr);
                break;

            case CONSTANT_InterfaceMethodref:
                interfaceMethod_ptr = (CONSTANT_InterfaceMethodref_info *)malloc(sizeof(struct CONSTANT_InterfaceMethodref_info));
                interfaceMethod_ptr->tag=tag;
                interfaceMethod_ptr->class_index = read2B(inFile);
                interfaceMethod_ptr->name_and_type_index = read2B(inFile);
                constant_pool->at(i) = ((void*) interfaceMethod_ptr);
                break;

            case CONSTANT_InvokeDynamic:
                invdyn_ptr = (CONSTANT_InvokeDynamic_info *)malloc(sizeof(struct CONSTANT_InvokeDynamic_info));
                invdyn_ptr->tag=tag;
                invdyn_ptr->bootstrap_method_attr_index = read2B(inFile);
                invdyn_ptr->name_and_type_index = read2B(inFile);
                constant_pool->at(i) = ((void*) invdyn_ptr);
                break;

            case CONSTANT_MethodHandle:
                methodhdl_ptr = (CONSTANT_MethodHandle_info *)malloc(sizeof(struct CONSTANT_MethodHandle_info));
                methodhdl_ptr->tag=tag;
                methodhdl_ptr->reference_kind = read1B(inFile);
                methodhdl_ptr->reference_index = read2B(inFile);
                constant_pool->at(i) = ((void*) methodhdl_ptr);
                break;

            case CONSTANT_Methodref:
                method_ptr = (CONSTANT_Methodref_info *)malloc(sizeof(struct CONSTANT_Methodref_info));
                method_ptr->tag=tag;
                method_ptr->class_index = read2B(inFile);
                method_ptr->name_and_type_index = read2B(inFile);
                constant_pool->at(i) = ((void*) method_ptr);
                break;

            case CONSTANT_MethodType:
                method_type_ptr = (CONSTANT_MethodType_info *)malloc(sizeof(struct CONSTANT_MethodType_info));
                method_type_ptr->tag=tag;
                method_type_ptr->descriptor_index = read2B(inFile);
                constant_pool->at(i) = ((void*) method_type_ptr);
                break;

            case CONSTANT_NameAndType:
                nat_ptr = (CONSTANT_NameAndType_info *)malloc(sizeof(struct CONSTANT_NameAndType_info));
                nat_ptr->tag=tag;
                nat_ptr->name_index = read2B(inFile);
                nat_ptr->descriptor_index = read2B(inFile);
                constant_pool->at(i) = ((void*) nat_ptr);
                break;

            case CONSTANT_String:
                str_ptr = (CONSTANT_String_info *)malloc(sizeof(struct CONSTANT_String_info));
                str_ptr->tag=tag;
                str_ptr->string_index = read2B(inFile);
                constant_pool->at(i) = ((void*) str_ptr);

                break;
            case CONSTANT_Utf8:
                utf_ptr = (CONSTANT_Utf8_info *)malloc(sizeof(struct CONSTANT_Utf8_info));
                utf_ptr->tag=tag;
                utf_ptr->length = read2B(inFile);
                utf_ptr->bytes = (uint8_t *) malloc(sizeof(uint8_t) * (utf_ptr->length) + 1);
                for (int i = 0; i < utf_ptr->length; i++){
                    utf_ptr->bytes[i] = read1B(inFile);
                }
                utf_ptr->bytes[utf_ptr->length] = NULL;
                constant_pool->at(i) = ((void*) utf_ptr);
                break;

            case CONSTANT_Long:
                long_ptr = (CONSTANT_Long_info *)malloc(sizeof(struct CONSTANT_Long_info));
                long_ptr->tag=tag;
                long_ptr->high_bytes = read4B(inFile);
                long_ptr->low_bytes = read4B(inFile);
                constant_pool->at(i++) = ((void*) long_ptr);
                constant_pool->at(i) = (NULL);
                break;

            case CONSTANT_Double:
                dub_ptr = (CONSTANT_Double_info *)malloc(sizeof(struct CONSTANT_Double_info));
                dub_ptr->tag=tag;
                dub_ptr->high_bytes = read4B(inFile);
                dub_ptr->low_bytes = read4B(inFile);
                constant_pool->at(i++) = ((void*) dub_ptr);
                constant_pool->at(i) = (NULL);
                break;
        }
    }
    /*                      Get Access Flags                       */

    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
    access_flag = twoByteBuffer;
    //access_flag = (bigEndian ? twoByteBuffer : swapEndian16(twoByteBuffer)); todo figure out why this is little endian
    /*                      Get This Class                         */

    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
    //this_class = (bigEndian ? twoByteBuffer : swapEndian16(twoByteBuffer)); todo figure out why this is little endian
    this_class = twoByteBuffer;
    /*                      Get Super Class                         */

    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
    //super_class = (bigEndian ? twoByteBuffer : swapEndian16(twoByteBuffer)); todo figure out why this is little endian
    super_class = twoByteBuffer;
    /*                      Get Interfaces Count                         */

    interfaces_count = read2B(inFile);

    /*                      Get Interfaces[]                         */

    interfaces = (uint16_t *) malloc(interfaces_count * sizeof(uint16_t));
    for (int i = 0; i < interfaces_count; i++){
        *(interfaces + i) = read2B(inFile);
    }

    /*                      Get Fields Count                         */
    fields_count = read2B(inFile);

    /*                      Get Fields[]                         */
    fields = (struct Field *) malloc(sizeof(struct Field) * fields_count);
    for (int i = 0; i < fields_count; i++){
        struct Field currentField = *(fields + i);
        currentField.access_flags = read2B(inFile);
        currentField.descriptor_index = read2B(inFile);
        currentField.attribute_count = read2B(inFile);
    }
    /*                      Get Methods Count                         */
    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
    if (this->bigEndian){
        twoByteBuffer = swapEndian16(twoByteBuffer);
    }
    methods_count = twoByteBuffer;

    /*                      Get Methods[]                         */

    methods = (struct Method *) malloc(methods_count * sizeof(struct Method));
    for (int i = 0; i < methods_count; i++){
        struct Method current = *(methods + i);
        inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
        if (this->bigEndian){
            twoByteBuffer = swapEndian16(twoByteBuffer);
        }
        current.access_flags = twoByteBuffer;
        std::cout << "Has access flags:" << std::endl;
        printMethodAccessMask(current.access_flags);
        inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
        if (this->bigEndian){
            twoByteBuffer = swapEndian16(twoByteBuffer);
        }
        current.name_index = twoByteBuffer;
        std::cout << "\tMethod Name: ";
        printUTFEntry(current.name_index);
        inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
        if (this->bigEndian){
            twoByteBuffer = swapEndian16(twoByteBuffer);
        }
        current.descriptor_index = twoByteBuffer;
        std::cout << "\n\tDescriptor: ";
        printUTFEntry(current.descriptor_index);
        inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
        if (this->bigEndian){
            twoByteBuffer = swapEndian16(twoByteBuffer);
        }
        current.attributes_count = twoByteBuffer;
        std::cout << "\n\tHas " << current.attributes_count << " attributes" << std::endl;

    }

    /*                      Get Attributes Count                         */
    attributes_count = read2B(inFile);
    printClassFile();

    /*                      Get Attributes[]                         */
    //TODO: Implement Methods
}

uint8_t ClassFile::read1B(std::ifstream& inFile){
    uint8_t oneByteBuffer;
    inFile.read(reinterpret_cast<char *>(&oneByteBuffer), 1);
    return oneByteBuffer;
}

uint16_t ClassFile::read2B(std::ifstream& inFile){
    uint16_t twoByteBuffer;
    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
    if (!this->bigEndian){
        return swapEndian16(twoByteBuffer);
    }
    return twoByteBuffer;
}

uint32_t ClassFile::read4B(std::ifstream& inFile){
    uint32_t fourByteBuffer;
    inFile.read(reinterpret_cast<char *>(&fourByteBuffer), 4);
    if (!this->bigEndian){
        return __bswap_32(fourByteBuffer);
    }
    return fourByteBuffer;
}

void ClassFile::printThisClass(){
    std::cout <<  "-------------------------------------\n" << "this: " << std::endl;
    (CONSTANT_Class_info *) constant_pool->at(this_class);
    std::cout << "\t";
    printUTFEntry((*((CONSTANT_Class_info *)constant_pool->at(this_class))).name_index);
    std::cout << std::endl;
}

void ClassFile::printSuperClass(){
    if (super_class == 0) return;
    std::cout <<  "-------------------------------------\n" << "super class: " << std::endl;
    (CONSTANT_Class_info *) constant_pool->at(super_class);
    std::cout << "\t";
    printUTFEntry((*((CONSTANT_Class_info *)constant_pool->at(super_class))).name_index);
    std::cout << std::endl;
}

void ClassFile::printUTFEntry(uint16_t constant_index){
    std::printf("%s", (*((CONSTANT_Utf8_info *)constant_pool->at(constant_index))).bytes);
}

void ClassFile::printConstantPool(){
    std::cout << std::dec << "--------------------------" << std::endl;
    std::cout << "Constant Pool" << std::endl;
    for (int i = 1; i < constant_pool_count; i++){
        if (*((char*) constant_pool->at(i)) == CONSTANT_Class){
            std::cout << "Entry " << i << ": Class" << std::endl;
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
            printUTFEntry((*((CONSTANT_NameAndType_info *)constant_pool->at(i))).descriptor_index);
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

void ClassFile::printInterfaces(){
    std::cout << std::dec << "--------------------------" << std::endl;
    std::cout << "interfaces_count: " << interfaces_count << std::endl;
    std::cout << "interfaces:" << std::endl;
    for (int i = 0; i < interfaces_count; i++){
        std::cout << "\t" << constant_pool->at(*(interfaces + i));
    }
}

void ClassFile::printFields(){
    std::cout << std::dec << "--------------------------" << std::endl;
    std::cout << "Fields Count: " << fields_count << std::endl;
    std::cout << "Fields:" << std::endl;
    for (int i = 0; i < fields_count; i++){
        struct Field current = *(fields + i);
        std::cout << "\tField Name: ";
        printUTFEntry(current.name_index);
        std::cout << "\n\tDescriptor: ";
        printUTFEntry(current.descriptor_index);
        std::cout << "Has access flags:" << std::endl;
        printAccessTypes(current.access_flags);
        std::cout << "\n\tHas " << current.attribute_count << " attributes" << std::endl;
    }
}

void ClassFile::printMethods(){
    std::cout << std::dec << "--------------------------" << std::endl;
    std::cout << "Methods Count: " << methods_count << std::endl;
    std::cout << "Methods:" << std::endl;
    for (int i = 0; i < methods_count; i++) {
        struct Method current = *(methods + i);
        std::cout << "\tMethod Name: ";
        printUTFEntry(current.name_index);
        std::cout << "\n\tDescriptor: ";
        printUTFEntry(current.descriptor_index);
        std::cout << "Has access flags:" << std::endl;
        printAccessTypes(current.access_flags);
        std::cout << "\n\tHas " << current.attributes_count << " attributes" << std::endl;
    }
}

void ClassFile::printClassFile(){
    std::cout << "magic number: " << std::hex << magic_number << std::endl;
    std::printf("major version: %u\n", majorVersion);
    std::printf("minor version: %u\n", minorVersion);
    std::printf("constant_pool_count: %u\n", constant_pool_count);
    printConstantPool();
    printAccessTypes(access_flag);
    printThisClass();
    printSuperClass();
    printInterfaces();
    printFields();
    printMethods();
}
uint16_t swapEndian16(uint16_t littleEndianInt){
    return (littleEndianInt >> 8 | littleEndianInt << 8);
}




