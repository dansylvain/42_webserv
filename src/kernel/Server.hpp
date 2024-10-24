#pragma once

#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <vector>
#include "Config.hpp"
#include <iostream>
#include "RequestParser.hpp"
#include <arpa/inet.h>
#include <algorithm>
#include "Logger.hpp"
#include "buildResponse.hpp"

#define BUFF_SIZE 4096
#define MAX_HDR_SIZE 8192
#define MAX_CNT_SIZE 30000000

vector<char> masterBuilder(vector<char> &bodyInput, e_errorCodes errorCode,
	string& fileName);
	
struct Client
{
	int					id;
	int					fd;
	std::vector<char>	message;
	std::vector<char>	messageSend;
	sockaddr_in 		address;
	socklen_t 			len;
	size_t				bodySize;
	RequestParser		header;

	Client()
	{
		id = 0;
		fd = 0;		
		memset(&address, 0, sizeof(address));
		len = sizeof(address);	
		bodySize = 0;
		messageSend.reserve(5);		
	}
};

class Server
{
	Config 				_conf;
	sockaddr_in 		_sockAddr;
	std::vector<Client> _clients;
	std::vector<char>	_writeBuffer;
	std::vector<char>	_readBuffer;
	int					_fd;

	int	&				_maxFd;
	fd_set &			_actualSet;
	fd_set &			_readSet;
	fd_set &			_writeSet;

	void displayClient(Client & client) const;
	void handleClientHeader(size_t i, ssize_t ret);
	void handleClientBody(size_t i, ssize_t ret);
	bool isMaxHeaderSize(std::vector<char>::iterator it, size_t i);
	bool isContentLengthValid(size_t i);
	bool isBodyTerminated(size_t i);
	bool isBodyTooLarge(size_t i);
	void replyClient(Client & client, std::vector<char> & response);
	void exitClient(size_t index);
	void exitClients();

	public:

		Server(sockaddr_in & sockaddr, int & maxFd, fd_set & actualSet,
			fd_set & readSet, fd_set & writeSet);

		const sockaddr_in & getSockAdress() const;

		bool setup();
		void catchClients();
		void listenClients();
		void replyClients();
		void exitServer();
};