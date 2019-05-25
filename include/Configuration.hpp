 ///
 /// @file    Configuration.hpp
 /// @author  https://icoty.github.io
 ///

#ifndef __YANGYU_CONFIGURATION_H__
#define __YANGYU_CONFIGURATION_H__

#include "FileName.hpp"
#include <map>
#include <set>
#include <string>
#include <iostream>
using std::map;
using std::set;
using std::string;

namespace yangyu
{

class Configuration{
public:
	Configuration(const string& filepath);
	void init();
	map<string,string> & getConfigMap();
	void debug();
private:
	string				_filepath;
	map<string,string>	_configMap;
};

}
#endif
