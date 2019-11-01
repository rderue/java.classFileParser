//
// Created by Ryan on 9/26/2019.
//

#ifndef EXTRACTCLASS_EXTRACT_H
#define EXTRACTCLASS_EXTRACT_H
#define JAVA_PATH "/usr/bin/java"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <experimental/filesystem>
#include <cstdint>
#include "cPoolTagTypes.h"
#include "classFile.h"
#include "AccessMask.h"
#include "Field.h"
#include "Method.h"
#include "attributeInfo.h"




uint16_t swapEndian16(uint16_t littleEndianInt);

uint32_t swapEndian32(uint32_t littleEndianInt);

#endif //EXTRACTCLASS_EXTRACT_H



