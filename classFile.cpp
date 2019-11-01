//
// Created by Ryan on 9/26/2019.
//


#include "extract.h"

long bytesRemaining;

long sizeofFile(std::string);

bool DEBUG = true;

ClassFile::ClassFile(char *fileName) {
    uint16_t twoByteBuffer;

    /*                            Set up necessary paths to get .class File                    */
    std::string filePath = std::string(std::experimental::filesystem::current_path().parent_path());
    filePath.append("/");
    filePath.append(fileName);
    bytesRemaining = sizeofFile(filePath);
    std::cout << "Size of " << fileName << " is " << bytesRemaining << " bytes" << std::endl;
    std::ifstream inFile;
    inFile.open(filePath, std::ifstream::binary);
    if (!inFile) {
        std::cout << "The header file could not be found" << std::endl;
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
    for (int i = 1; i <= constant_pool_count; i++) {
        uint8_t tag = read1B(inFile);

        /*  Declare all necessary variables before the switch statement */

        CONSTANT_Class_info *class_ptr;
        CONSTANT_Fieldref_info *field_ptr;
        CONSTANT_Float_info *float_ptr;
        CONSTANT_Integer_info *int_ptr;
        CONSTANT_InterfaceMethodref_info *interfaceMethod_ptr;
        CONSTANT_InvokeDynamic_info *invdyn_ptr;
        CONSTANT_MethodHandle_info *methodhdl_ptr;
        CONSTANT_Methodref_info *method_ptr;
        CONSTANT_MethodType_info *method_type_ptr;
        CONSTANT_NameAndType_info *nat_ptr;
        CONSTANT_String_info *str_ptr;
        CONSTANT_Utf8_info *utf_ptr;
        CONSTANT_Long_info *long_ptr;
        CONSTANT_Double_info *dub_ptr;

        switch (tag) {
            case CONSTANT_Class:
                class_ptr = (CONSTANT_Class_info *) malloc(sizeof(struct CONSTANT_Class_info));
                class_ptr->tag = tag;
                class_ptr->name_index = read2B(inFile);
                constant_pool->at(i) = (void *) class_ptr;
                break;

            case CONSTANT_Fieldref:
                field_ptr = (CONSTANT_Fieldref_info *) malloc(sizeof(struct CONSTANT_Fieldref_info));
                field_ptr->tag = tag;
                field_ptr->class_index = read2B(inFile);
                field_ptr->name_and_type_index = read2B(inFile);
                constant_pool->at(i) = ((void *) field_ptr);
                break;

            case CONSTANT_Float:
                float_ptr = (CONSTANT_Float_info *) malloc(sizeof(struct CONSTANT_Float_info));
                float_ptr->tag = tag;
                float_ptr->bytes = read4B(inFile);
                constant_pool->at(i) = ((void *) float_ptr);
                break;

            case CONSTANT_Integer:
                int_ptr = (CONSTANT_Integer_info *) malloc(sizeof(struct CONSTANT_Integer_info));
                int_ptr->tag = tag;
                int_ptr->bytes = read4B(inFile);
                constant_pool->at(i) = ((void *) int_ptr);
                break;

            case CONSTANT_InterfaceMethodref:
                interfaceMethod_ptr = (CONSTANT_InterfaceMethodref_info *) malloc(
                        sizeof(struct CONSTANT_InterfaceMethodref_info));
                interfaceMethod_ptr->tag = tag;
                interfaceMethod_ptr->class_index = read2B(inFile);
                interfaceMethod_ptr->name_and_type_index = read2B(inFile);
                constant_pool->at(i) = ((void *) interfaceMethod_ptr);
                break;

            case CONSTANT_InvokeDynamic:
                invdyn_ptr = (CONSTANT_InvokeDynamic_info *) malloc(sizeof(struct CONSTANT_InvokeDynamic_info));
                invdyn_ptr->tag = tag;
                invdyn_ptr->bootstrap_method_attr_index = read2B(inFile);
                invdyn_ptr->name_and_type_index = read2B(inFile);
                constant_pool->at(i) = ((void *) invdyn_ptr);
                break;

            case CONSTANT_MethodHandle:
                methodhdl_ptr = (CONSTANT_MethodHandle_info *) malloc(sizeof(struct CONSTANT_MethodHandle_info));
                methodhdl_ptr->tag = tag;
                methodhdl_ptr->reference_kind = read1B(inFile);
                methodhdl_ptr->reference_index = read2B(inFile);
                constant_pool->at(i) = ((void *) methodhdl_ptr);
                break;

            case CONSTANT_Methodref:
                method_ptr = (CONSTANT_Methodref_info *) malloc(sizeof(struct CONSTANT_Methodref_info));
                method_ptr->tag = tag;
                method_ptr->class_index = read2B(inFile);
                method_ptr->name_and_type_index = read2B(inFile);
                constant_pool->at(i) = ((void *) method_ptr);
                break;

            case CONSTANT_MethodType:
                method_type_ptr = (CONSTANT_MethodType_info *) malloc(sizeof(struct CONSTANT_MethodType_info));
                method_type_ptr->tag = tag;
                method_type_ptr->descriptor_index = read2B(inFile);
                constant_pool->at(i) = ((void *) method_type_ptr);
                break;

            case CONSTANT_NameAndType:
                nat_ptr = (CONSTANT_NameAndType_info *) malloc(sizeof(struct CONSTANT_NameAndType_info));
                nat_ptr->tag = tag;
                nat_ptr->name_index = read2B(inFile);
                nat_ptr->descriptor_index = read2B(inFile);
                constant_pool->at(i) = ((void *) nat_ptr);
                break;

            case CONSTANT_String:
                str_ptr = (CONSTANT_String_info *) malloc(sizeof(struct CONSTANT_String_info));
                str_ptr->tag = tag;
                str_ptr->string_index = read2B(inFile);
                constant_pool->at(i) = ((void *) str_ptr);

                break;
            case CONSTANT_Utf8:
                utf_ptr = (CONSTANT_Utf8_info *) malloc(sizeof(struct CONSTANT_Utf8_info));
                utf_ptr->tag = tag;
                utf_ptr->length = read2B(inFile);
                utf_ptr->bytes = (uint8_t *) malloc(sizeof(uint8_t) * (utf_ptr->length) + 1);
                for (int i = 0; i < utf_ptr->length; i++) {
                    utf_ptr->bytes[i] = read1B(inFile);
                }
                utf_ptr->bytes[utf_ptr->length] = NULL;
                constant_pool->at(i) = ((void *) utf_ptr);
                break;

            case CONSTANT_Long:
                long_ptr = (CONSTANT_Long_info *) malloc(sizeof(struct CONSTANT_Long_info));
                long_ptr->tag = tag;
                long_ptr->high_bytes = read4B(inFile);
                long_ptr->low_bytes = read4B(inFile);
                constant_pool->at(i++) = ((void *) long_ptr);
                constant_pool->at(i) = NULL;
                break;

            case CONSTANT_Double:
                dub_ptr = (CONSTANT_Double_info *) malloc(sizeof(struct CONSTANT_Double_info));
                dub_ptr->tag = tag;
                dub_ptr->high_bytes = read4B(inFile);
                dub_ptr->low_bytes = read4B(inFile);
                constant_pool->at(i++) = ((void *) dub_ptr);
                constant_pool->at(i) = NULL;
                break;
        }
    }
    /*                      Get Access Flags                       */

    access_flag = read2Brev(inFile);
    // todo figure out why this is little endian
    /*                      Get This Class                         */

    //todo figure out why this is little endian
    this_class = read2Brev(inFile);
    /*                      Get Super Class                         */

    //todo figure out why this is little endian
    super_class = read2Brev(inFile);
    /*                      Get Interfaces Count                         */

    interfaces_count = read2B(inFile);
    std::cout << "Bytes remaining after parsing super class: " << bytesRemaining << std::endl;

    /*                      Get Interfaces[]                         */

    interfaces = (uint16_t *) malloc(interfaces_count * sizeof(uint16_t));
    for (int i = 0; i < interfaces_count; i++) {
        *(interfaces + i) = read2B(inFile);
    }

    /*                      Get Fields Count                         */
    fields_count = read2B(inFile);

    /*                      Get Fields[]                         */
    fields = (struct Field *) malloc(sizeof(struct Field) * fields_count);
    for (int i = 0; i < fields_count; i++) {
        struct Field currentField = *(fields + i);
        currentField.access_flags = read2B(inFile);
        currentField.descriptor_index = read2B(inFile);
        currentField.attribute_count = read2B(inFile);
    }
    /*                      Get Methods Count                         */
    //todo why is method count also little endian?????????????????
    methods_count = read2Brev(inFile);

    /*                      Get Methods[]                         */
    methods = (struct Method *) malloc(methods_count * sizeof(struct Method));
    for (int i = 0; i < methods_count; i++) {
        struct Method *current = (methods + i);
        current->access_flags = read2Brev(inFile);
        //todo why is access mask also little endian?????????????????
        //todo why is name index also little endian?????????????????
        current->name_index = read2Brev(inFile);
        //todo why is descriptor index also little endian?????????????????
        current->descriptor_index = read2Brev(inFile);
        //todo why is attributes count also little endian?????????????????
        current->attributes_count = read2Brev(inFile);
        current->attributes = new std::vector<void *>;
        for (int i = 0; i < current->attributes_count; i++) {
            twoByteBuffer = read2Brev(inFile);
            std::cout << "Bytes remaining after getting attribute name: " << bytesRemaining << std::endl;
            /* Deal with attribute identification */
            std::string attributeName = (char *) (*((CONSTANT_Utf8_info *) constant_pool->at(twoByteBuffer))).bytes;
            std::cout << "Attribute name is " << attributeName << std::endl;



            /*
             * TODO There is a known bug in this function described below
             *
             * When looking at the hexdumped output of the classFile, it is possible to see what the values for
             * attribute length and code_length should be--however each of these functions initially used read4B.
             * When using read4B or read4Brev, the current byte of the file would increase by 5 which would throw
             * off all subsequent reads. To get around this, I am using read2B and read1B and discarding the rest
             * of the bytes, because for small programs, these are usually 0s. This means this will only work for
             * very small functions, but I am hopeful that this can be fixed.
             */
            if (attributeName == "Code") {

                struct code_attribute *code_ptr = (struct code_attribute *) malloc(sizeof(struct code_attribute));
                code_ptr->attribute_name_index = twoByteBuffer;
                read2Brev(inFile); //discards MSB of attribute_length
                code_ptr->attribute_length = (uint16_t) read2Brev(inFile); //incorrect read
                if (DEBUG) std::cout << "attribute_length: " << code_ptr->attribute_length << std::endl;
                code_ptr->max_stack = read2Brev(inFile);
                code_ptr->max_locals = read2Brev(inFile);

                read2Brev(inFile); //discard MSB of code_length
                code_ptr->code_length = (uint32_t) read1B(inFile); //incorrect read
                //std::cout << std::hex << (short) read1B(inFile); //discard LSB of code_length
                code_ptr->code = (uint8_t *) malloc(code_ptr->code_length);
                for (int j = 0; j <= code_ptr->code_length; j++) {
                    *(code_ptr->code + j) = (short) read1B(inFile);
                }
                code_ptr->exception_table_length = read2Brev(inFile);
                std::cout << "Length of exception table is " << code_ptr->exception_table_length << std::endl;
                code_ptr->exceptionTable = (code_attribute::exception *) malloc(
                        code_ptr->exception_table_length * sizeof(code_attribute::exception *));
                for (int j = 0; j < code_ptr->exception_table_length; j++) {
                    (code_ptr->exceptionTable + j)->start_pc = read2Brev(inFile);
                    (code_ptr->exceptionTable + j)->end_pc = read2Brev(inFile);
                    (code_ptr->exceptionTable + j)->handler_pc = read2Brev(inFile);
                    (code_ptr->exceptionTable + j)->catch_type = read2Brev(inFile);
                }
                code_ptr->attribute_length = read2Brev(inFile);
                std::cout << "Number of attributes possessed by code attribute: " << code_ptr->attribute_length << std::endl;
                for (int j = 0; j < code_ptr->attribute_length; j++) {
                    std::cout << "name of attribute possessed by code attribute: " <<
                    (char *) (*((CONSTANT_Utf8_info *) constant_pool->at(read2Brev(inFile)))).bytes << std::endl;
                    read2B(inFile);
                    int length = (unsigned int)read2Brev(inFile);
                    std::cout << "Length of that attribute is " << std::hex << length << std::endl;
                    for (int k = 0; k < length; k++) {
                        read1B(inFile); //discard attribute length bytes
                    }
                }
                current->attributes->push_back(code_ptr);
            } else {
                std::cout << "was not code" << std::endl;
                read2B(inFile);
                for (int j = 0; j < read2Brev(inFile); j++) {
                    read1B(inFile);
                }
            }
        }
    }
    printClassFile();
}



                /*                      Get Attributes Count                         */

    /*                      Get Attributes[]                         */
    //TODO: Implement Methods


uint8_t ClassFile::read1B(std::ifstream& inFile){
    uint8_t oneByteBuffer;
    inFile.read(reinterpret_cast<char *>(&oneByteBuffer), 1);
    bytesRemaining--;
    return oneByteBuffer;
}

uint16_t ClassFile::read2Brev(std::ifstream& inFile){
    uint16_t twoByteBuffer;
    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
    if (this->bigEndian){
        return swapEndian16(twoByteBuffer);
    }
    bytesRemaining -= 2;
    return twoByteBuffer;
}

uint16_t ClassFile::read2B(std::ifstream& inFile){
    uint16_t twoByteBuffer;
    inFile.read(reinterpret_cast<char *>(&twoByteBuffer), 2);
    if (!this->bigEndian){
        return swapEndian16(twoByteBuffer);
    }
    bytesRemaining -= 2;
    return twoByteBuffer;
}

uint32_t ClassFile::read4Brev(std::ifstream& inFile){
    uint32_t fourByteBuffer;
    inFile.read(reinterpret_cast<char *>(&fourByteBuffer), 4);
    if (this->bigEndian){
        return swapEndian32(fourByteBuffer);
    }
    bytesRemaining -= 4;
    return fourByteBuffer;
}

uint32_t ClassFile::read4B(std::ifstream& inFile) {
    uint32_t fourByteBuffer;
    inFile.read(reinterpret_cast<char *>(&fourByteBuffer), 4);
    if (!this->bigEndian) {
        return swapEndian32(fourByteBuffer);
    }
    bytesRemaining -= 4;
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
    //std::cout <<  "-------------------------------------\n" << "super class: " << std::endl;
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
   // std::cout << std::dec << "--------------------------" << std::endl;
    std::cout << "interfaces_count: " << interfaces_count << std::endl;
    std::cout << "interfaces:" << std::endl;
    for (int i = 0; i < interfaces_count; i++){
        std::cout << "\t" << constant_pool->at(*(interfaces + i));
    }
}

void ClassFile::printFields(){
  //  std::cout << std::dec << "--------------------------" << std::endl;
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
        //std::cout << "\n\tHas " << current.attribute_count << " attributes" << std::endl;
    }
}

/**
 * Warning operands are not currently implemented generally. They only work for those operands which are constant pool
 * indicies
 * @param m The method whose bytecodes are to be printed.
 */
void ClassFile::printMethodBytecode(struct Method m){
    for (auto &codeAttr : *m.attributes){
        std::cout << "Code: (" << std::dec << ((code_attribute *) codeAttr)->code_length << " bytes)" << std::endl;
        for (int i = 0; i < ((code_attribute *) codeAttr)->code_length; i++) {
            setCurrentInstruction((short) *(((code_attribute *) codeAttr)->code + i));
            if (currentInstruction.instrName != "") {
                std::cout << std::dec << i << ": " << currentInstruction.instrName;
            }
            if (currentInstruction.numOperands == 2){
                uint16_t operand = (short) *(((code_attribute *) codeAttr)->code + ++i) |
                                   (short) *(((code_attribute *) codeAttr)->code + ++i);
                std::cout << std::dec << " #" << operand;
                //std::cout << "\t//" << (char *) (*((CONSTANT_Utf8_info *) constant_pool->at(operand))).bytes;
                //std::cout << "\t\\";
                //printUTFEntry((*((CONSTANT_Class_info *)constant_pool->at(operand))).name_index);
            }
            std::cout << std::endl;
        }
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
        std::cout << "\nHas access flags:" << std::endl;
        printAccessTypes(current.access_flags);
        std::cout << "\n\tDescriptor: ";
        printUTFEntry(current.descriptor_index);
        std::cout << std::endl;
        //std::cout << "\n\tHas " << current.attributes_count << " attributes" << std::endl;
        printMethodBytecode(current);
        std::cout << std::endl;
    }
}

void ClassFile::printClassFile(){
    std::cout << "magic number: " << std::hex << magic_number << std::endl;
    std::printf("major version: %u\n", majorVersion);
    std::printf("minor version: %u\n", minorVersion);
    std::printf("constant_pool_count: %u\n", constant_pool_count);
    printConstantPool();
    printAccessTypes(access_flag);
    std::cout << std::endl;
    printThisClass();
    printSuperClass();
    printInterfaces();
    printFields();
    printMethods();
}

uint16_t swapEndian16(uint16_t littleEndianInt){
    return (littleEndianInt >> 8 | littleEndianInt << 8);
}

uint32_t swapEndian32(uint32_t littleEndianInt) {
    return littleEndianInt >> 24 |
            ((littleEndianInt << 8) & 0x00FF0000) |
            ((littleEndianInt >> 8) & 0x0000FF00) |
            littleEndianInt << 24;

}

long sizeofFile(std::string filePath){
    std::ifstream inFile;
    inFile.open(filePath, std::ifstream::binary);
    long begin = inFile.tellg();
    inFile.seekg(0, std::ios::end);
    long end = inFile.tellg();
    return end - begin;
}



