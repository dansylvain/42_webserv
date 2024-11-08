#pragma once

#include <vector>
#include "errorCode.hpp"
#include "RequestParser.hpp"
#include "ResponseBuilder.hpp"

struct Client
{
	int					fd;
	sockaddr_in 		address;
	socklen_t 			addressLen;

	std::vector<char>	messageRecv;
	RequestParser		headerRequest;
	size_t				bodySize;

	ResponseBuilder		responseBuilder;
	std::vector<char>	headerRespons;
	std::vector<char>	messageSend;
	
	short int			ping;
	bool 				pongHeader;
	bool				exitRequired;
	
	Client();
	Client(const Client &);
	~Client();
};