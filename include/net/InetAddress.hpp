 ///
 /// @file    InetAddress.hpp
 /// @author  https://icoty.github.io
 ///
#ifndef __YANGYU_INETADDRESS_H__
#define __YANGYU_INETADDRESS_H__

#include <string>
#include <netinet/in.h>
using std::string;

namespace yangyu
{

class InetAddress{
public:
	explicit InetAddress(unsigned short port);
	InetAddress(const string &ip, unsigned short port);
	InetAddress(const struct sockaddr_in &addr);

	void setSockAddrInet(const struct sockaddr_in &addr);
	const struct sockaddr_in *getSockAddrInet()const;
	string toIp()const;
	unsigned short toPort()const;
private:
	struct sockaddr_in _addr;
};

}
#endif
