 ///
 /// @file    String2Upper.hpp
 /// @author  https://icoty.github.io
 ///
#ifndef __YANGYU_WORDQUERY_H__
#define __YANGYU_WORDQUERY_H__

#include "Configuration.hpp"

#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include <set>
using std::vector;
using std::string;
using std::map;
using std::pair;
using std::set;
using std::unordered_map;
//using namespace std::tr1;

namespace yangyu
{

class String2Upper{
public:
    String2Upper(Configuration & conf);

	string doQuery(const string & word);
private:
    Configuration & _conf;
};

}
#endif
