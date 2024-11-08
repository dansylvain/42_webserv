#include "Kernel.hpp"

bool Kernel::_exit = false;
int Kernel::_maxFd = 0;
fd_set Kernel::_actualSet;
fd_set Kernel::_readSet;
fd_set Kernel::_writeSet;
Kernel::Kernel(void) : _conf()
{
	this->_servers.reserve(300);//!
	FD_ZERO(&this->_actualSet);
	this->setup();
	this->launch();
}

Kernel::Kernel(char* path) : _conf(path)
{
	this->_servers.reserve(300);//!
	FD_ZERO(&this->_actualSet);
	this->setup();
	this->launch();
}

void Kernel::callCatch(Server & server)
{
	server.catchClients();
}

void Kernel::callListen(Server & server)
{
	server.listenClients();
}

void Kernel::callReply(Server & server)
{
	server.replyClients();
}

void Kernel::callExit(Server & server)
{
	server.exitServer();
}

void Kernel::setup()
{	
	for (size_t i = 0; i < this->_conf.sockAddress.size(); i++)
	{
		Server server(this->_conf.sockAddress[i], this->_maxFd,
			this->_actualSet, this->_readSet, this->_writeSet, this->_conf);
		if (server.setup())
			this->_servers.push_back(server);	
	}	
}

void Kernel::launch()
{
	ConfigFileParser::printRoutesData(_conf.routes);

	while (true)
	{
		struct timeval timeout = {1, 0};
		this->_readSet = this->_writeSet = this->_actualSet;		
		if (select(this->_maxFd + 1, &this->_readSet, &this->_writeSet,
			0, &timeout) < 0)
		{	
			if (!this->_exit)
				Logger::getInstance().log(ERROR, "select");
			continue;
		}
		if (this->_exit)
			break;
		std::for_each(this->_servers.begin(), this->_servers.end(),
			this->callCatch);
		std::for_each(this->_servers.begin(), this->_servers.end(),
			this->callListen);
		std::for_each(this->_servers.begin(), this->_servers.end(),
			this->callReply);
		usleep(100);	
	}

	std::for_each(this->_servers.begin(), this->_servers.end(),
		this->callExit);
}
