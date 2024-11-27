#include "Cgi.hpp"
#include "Logger.hpp"

void errnoHandle(); //! a suppr

void Cgi::hasError(Client & client, std::string err)
{
	Logger::getInstance().log(DEBUG, "Cgi Has Error", client);

	int status;
	pid_t pid = waitpid(this->_pid, &status, WNOHANG);
	if (pid > 0)
	{	
		Logger::getInstance().log(DEBUG, "cgi exited", client);	

		if (WIFEXITED(status))		
			this->exitCodeHandle(status, client, err);		
		else if (WIFSIGNALED(status)) { 
            throw (Logger::getInstance().log(ERROR, err, client), Server::
				ShortCircuitException(CODE_502_BAD_GATEWAY)); 
        }
	}
	else if (pid < 0)
		throw (Logger::getInstance().log(ERROR, "waitpid", client), //!
			Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR));
	else
		Logger::getInstance().log(DEBUG, "cgi is running", client);
}

void Cgi::exitCodeHandle(int &status, Client &client, std::string &err)
{
	int exitCode = WEXITSTATUS(status);
	std::stringstream ss;
	ss << "cgi exited with code : " << exitCode;
	Logger::getInstance().log(DEBUG, ss.str(), client);
	if (exitCode == 242)
		Logger::getInstance().log(ERROR, "execve", client);
	if (exitCode != 0)
		throw(Logger::getInstance().log(ERROR, err, client), Server::
			ShortCircuitException(CODE_503_SERVICE_UNAVAILABLE));
}

void Cgi::isTimeout(Client & client, std::string err)
{
	Logger::getInstance().log(DEBUG, "Cgi Is Timeout", client);

	if (!this->_start.tv_sec && gettimeofday(&this->_start, NULL))
		throw (Logger::getInstance().log(ERROR, "isTimeS gettimeofday", client),
			Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR));
    double span = getTimeSpan(client);   
    if (span >= client.conf->timeoutCgi / 2.0
		&& this->_lastSpan != static_cast<int>(span))
    { 
		this->_lastSpan = static_cast<int>(span);
        std::stringstream ss;
		ss << "Timeout : "  << this->_lastSpan << " secs/"
		<< client.conf->timeoutCgi;			
        Logger::getInstance().log(WARNING, ss.str(), client);	
    }
	if (span >= client.conf->timeoutCgi)
	{      
        kill(this->_pid, SIGTERM);
		std::memset(&this->_start, 0, sizeof(this->_start));
		throw (Logger::getInstance().log(ERROR, err, client),
			Server::ShortCircuitException(CODE_508_LOOP_DETECTED));           
    } 
}

void Cgi::retHandle(Client & client, ssize_t ret, std::string err,
	std::string info)
{		
	std::stringstream ss; ss << "ret: " << ret;
	Logger::getInstance().log(DEBUG, ss.str());

    if (!ret)    
        Logger::getInstance().log(DEBUG, info);
    else if (ret < 0)
    {	      
        errnoHandle();//! a suppr  
		throw (Logger::getInstance().log(ERROR, err, client),
			Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR));
    }
	if (gettimeofday(&this->_start, NULL))
		throw (Logger::getInstance().log(ERROR, "retH gettimeofday", client),
			Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR));	
}

void Cgi::setBody(Client & client, bool eof)
{
    Logger::getInstance().log(DEBUG, "Cgi Set Body", client);

	this->hasError(client, "cgi get body has error");
	this->isTimeout(client, "Timeout is over");
    if (!FD_ISSET(this->_fds[1], &Kernel::_writeSet))    
        return Logger::getInstance().log(DEBUG, "cgi not ready to send");     
    Logger::getInstance().log(DEBUG, "\e[31;103mcgi ready to send\e[0m");
 	ssize_t ret = send(this->_fds[1], client.messageRecv.data(),
        client.messageRecv.size(), MSG_NOSIGNAL);
    Kernel::cleanFdSet(client);	
	this->retHandle(client, ret, "send", "cgi exited");    			
	std::vector<char> str(client.messageRecv.data(), client.messageRecv.data()
		+ static_cast<size_t>(ret));
	Logger::getInstance().log(DEBUG, "sent to cgi", client);	
		Server::printVector(client, str);
	client.messageRecv.erase(client.messageRecv.begin(),
        client.messageRecv.begin() + ret);
	if (eof && client.messageRecv.empty() && shutdown(_fds[1], SHUT_WR < 0))			
		throw (Logger::getInstance().log(ERROR, "set body shutdown", client),
			Server::ShortCircuitException(CODE_500_INTERNAL_SERVER_ERROR));
}

bool Cgi::getBody(Client & client)
{
    Logger::getInstance().log(DEBUG, "Cgi Get Body", client);

	this->hasError(client, "cgi get body has error");
	this->isTimeout(client, "Timeout is over");
    if (!FD_ISSET(this->_fds[1], &Kernel::_readSet))  
        return Logger::getInstance().
			log(DEBUG, "cgi not ready to recev", client), true;  
    client.messageSend.clear();
    client.messageSend.resize(client.conf->send_buff_size);
	Logger::getInstance().log(DEBUG, "\e[31;103mcgi ready to recev\e[0m",
		client);
    ssize_t ret = recv(this->_fds[1], client.messageSend.data(),
        client.messageSend.size(), 0);
	Kernel::cleanFdSet(client);
	this->retHandle(client, ret, "recv", "end cgi");
	client.messageSend.erase(client.messageSend.begin() + ret,
		client.messageSend.end());
	return false; 
}
