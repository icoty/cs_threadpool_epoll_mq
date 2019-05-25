///
/// @file    main.cpp
/// @author  https://icoty.github.io
///
#include "Log4func.hpp"
#include "String2UpperServer.hpp"
#include <iostream>
using std::cout;
using std::endl;

int main(int argc,char **argv)
{
	if(argc!=2){
		LogError("input my.conf path!");
		return -1;
	}
	cout<<argv[1]<<endl;
	
	yangyu::String2UpperServer query(argv[1]);
	query.start();
	return 0;
}
