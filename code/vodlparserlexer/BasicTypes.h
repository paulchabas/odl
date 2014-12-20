#ifndef BASIC_TYPES_H
#define BASIC_TYPES_H

#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif  // _DEBUG


#include <array>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <hash_map>

// {TODO} Paul(2014/12/20) 64bits portability.
typedef unsigned long long u64;
typedef long long i64;
typedef unsigned int u32;
typedef int i32;
typedef unsigned short u16;
typedef short i16;
typedef char i8;
typedef unsigned char u8;

static_assert(sizeof(u64) == 8, "invalid primitive type sizeof.");
static_assert(sizeof(i64) == 8, "invalid primitive type sizeof.");
static_assert(sizeof(u32) == 4, "invalid primitive type sizeof.");
static_assert(sizeof(i32) == 4, "invalid primitive type sizeof.");
static_assert(sizeof(u16) == 2, "invalid primitive type sizeof.");
static_assert(sizeof(i16) == 2, "invalid primitive type sizeof.");
static_assert(sizeof(u8) == 1, "invalid primitive type sizeof.");
static_assert(sizeof(i8) == 1, "invalid primitive type sizeof.");


#endif







