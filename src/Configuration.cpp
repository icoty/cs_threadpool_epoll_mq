 ///
 /// @file    Configuration.cpp
 /// @author  https://icoty.github.io
 ///
#include "Configuration.hpp"
#include "Log4func.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

namespace yangyu
{

Configuration::Configuration(const string& filepath)
:_filepath(filepath)
{
	init();
}

void Configuration::debug()
{
	for(auto & ele:_configMap)
		std::cout << ele.first << '\t' << ele.second << std::endl;
}

void Configuration::init()
{
	std::ifstream _in(_filepath.c_str());
	if(!_in.good()){
		LogError("_in open error!");
		_in.close();
		return;
	}

	string line;
	while(getline(_in,line))
	{
		std::istringstream iss(line);
		std::string key;
		std::string value;
		iss >> key >> value;
		_configMap[key]=value;
	}
	_in.close();
}

map<string,string> & Configuration::getConfigMap()
{	return _configMap;	}

}
