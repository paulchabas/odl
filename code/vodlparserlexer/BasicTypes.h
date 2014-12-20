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

typedef unsigned int u32;
typedef int i32;

#endif







