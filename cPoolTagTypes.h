//
// Created by Ryan on 9/27/2019.
//

#ifndef EXTRACTCLASS_CPOOLTAGTYPES_H
#define EXTRACTCLASS_CPOOLTAGTYPES_H

enum cPoolTag{
    CONSTANT_Utf8                   = 1,
    CONSTANT_Integer                = 3,
    CONSTANT_Float                  = 4,
    CONSTANT_Long                   = 5,
    CONSTANT_Double                 = 6,
    CONSTANT_Class                  = 7,
    CONSTANT_String                 = 8,
    CONSTANT_Fieldref               = 9,
    CONSTANT_Methodref              = 10,
    CONSTANT_InterfaceMethodref     = 11,
    CONSTANT_NameAndType            = 12,
    CONSTANT_MethodHandle           = 15,
    CONSTANT_MethodType             = 16,
    CONSTANT_InvokeDynamic          = 18
};
struct CONSTANT_Class_info {
    uint8_t tag;
    uint16_t name_index;    /* Index into the pool for the string */
};

/**
* For the next 3 structs:
* tag: tag must correspond to appropriate enum
* class_index: Constant pool index for the class_info object for which this field ia member
* name_and_type_index- Constant pool index for the Name and Type Info struct for this field
*/
struct CONSTANT_Fieldref_info {
    uint8_t tag;
    uint16_t class_index;
    uint16_t name_and_type_index;
};

struct CONSTANT_Methodref_info {
    uint8_t tag;
    uint16_t class_index;
    uint16_t name_and_type_index;
};

struct CONSTANT_InterfaceMethodref_info {
    uint8_t tag;
    uint16_t class_index;
    uint16_t name_and_type_index;
};

/**
* tag: tag must correspond with enum
* string_index: Constant pool index for the Constant_Utf8_info pointing to where string will be
*/
struct CONSTANT_String_info {
    uint8_t tag;
    uint16_t string_index;
};

/**
* For the next 2 structs:
* tag: tag must correspond with enum
* bytes: 4 bytes in big endian order holding the int and float constants
* disclaimer: many special floating point cases, see java specification
*/

struct CONSTANT_Integer_info {
    uint8_t tag;
    uint64_t bytes;
};

struct CONSTANT_Float_info {
    uint8_t tag;
    uint64_t bytes;
};

/**
* For the next 2 structs:
* NOTE: THESE STRUCTS TAKE UP 2 ELEMENTS IN CONST POOL, NEXT VALID ENTRY IS AT INDEX N+2
* tag: tag must correspond with enum
* high_bytes: 4 bytes in big endian order holding the 32 MSB
* low_bytes: 4 bytes in little endian order holding the 32 LSB
* disclaimer: many special floating point cases, see java specification
*/

struct CONSTANT_Long_info {
    uint8_t tag;
    uint64_t high_bytes;
    uint64_t low_bytes;
};

struct CONSTANT_Double_info {
    uint8_t tag;
    uint64_t high_bytes;
    uint64_t low_bytes;
};

/**
* tag: tag must correspond with enum
* name_index: Constant pool index for a Constant_utf8_info struct containing the name
* descriptor_index: Constant pool index for a Constant_utf8_info struct containing the descriptor
*/

struct CONSTANT_NameAndType_info {
    uint8_t tag;
    uint16_t name_index;
    uint16_t descriptor_index;
};

/**
* tag: tag must correspond with enum
* length: the number of bytes in the string
* bytes: the bytes that make up the string: NOT NULL TERMINATED
* Note: Java uses their own UTF Encoding.
* Read it here: https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-4.html#jvms-4.4
*/
struct CONSTANT_Utf8_info {
    uint8_t tag;
    uint16_t length;
    uint8_t * bytes;
};
/**
* tag: tag must correspond with enum
* reference kind: in the range of 1-9 denoting the method handle
* reference index: Specific encoding for each type of reference
* Read it here https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-4.html#jvms-4.4
*/

struct CONSTANT_MethodHandle_info {
    uint8_t tag;
    uint8_t reference_kind;
    uint16_t reference_index;
};

/**
* tag: tag must correspond with enum
* descriptor_index: Constant pool index containing a Constant_utf8_info struct containing the method descriptor
*/
struct CONSTANT_MethodType_info {
    uint8_t tag;
    uint16_t descriptor_index;
};

/**
 * tag: tag must correspond with enum
 * bootstrap_method_attr_index: Bootstrap_methods array index
 * name_and_type_index: Constant pool index containing a Constant_NameAndType_info struct with method name & descriptor
 */
struct CONSTANT_InvokeDynamic_info {
    uint8_t tag;
    uint16_t bootstrap_method_attr_index;
    uint16_t name_and_type_index;
};
#endif //EXTRACTCLASS_CPOOLTAGTYPES_H
