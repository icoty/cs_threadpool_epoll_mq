 ///
 /// @file    String2Upper.cpp
 /// @author  https://icoty.github.io
 ///
#include "String2Upper.hpp"
#include "FileName.hpp"
#include "Log4func.hpp"
#include <math.h>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm> // 全部转大写或小写
using std::istringstream;
using std::ifstream;
using std::istringstream;
using std::vector;
using std::pair;

namespace yangyu
{
    
String2Upper::String2Upper(Configuration & conf)
:_conf(conf)
{ }
    
string String2Upper::doQuery(const string & word)
{
    string ret = word;
    // 全部转大写
    transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
    return ret;
}

}
