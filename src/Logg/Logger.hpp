#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "Server.hpp"
#include "RequestParser.hpp"
#include "timeStamp.hpp"
#include "Kernel.hpp"
#include "Client.hpp"
#include <ctime>
#include <arpa/inet.h>

class Kernel;
class Server;

typedef enum
{
	INFO = 0,
	DEBUG,
	EVAL,
	WARNING,
	ERROR,
	LOG_LEVEL_COUNT
}	logLevel;

typedef enum
{
	L_DEB = 1,
	L_VRB = 2,
	L_SLN = 4,
	L_ALW = 8,	
}	logFlags;

/**========================================================================
 *                             LOGGER
 *  LOG CONFIG: values to be changed in .cpp file:
 *?  Logger::Logger() : logToStdOut(1) 	=> 	0 to silence console log
 *?{
 *?	_logLevel[INFO] = 		1;			=>	0 to filter
 *?	_logLevel[DEBUG] = 		1;			=>	0 to filter
 *?	_logLevel[WARNING] = 	1;			=>	0 to filter
 *?	_logLevel[ERROR] = 		1;			=>	0 to filter
 *?	... 
 *========================================================================**/
class Logger
{
	#ifdef UNIT_TEST
	public:
	#else
	private:
	#endif

		std::ofstream	_accessFile;
		std::ofstream	_errorFile;
		bool			logToStdOut;
		int				_flags;

		void		logOut(logLevel logLevel, const std::string& message);
		std::string	formatLogLevel(logLevel loglevel) const;
		std::string removeAnsiCodes(const std::string& message);
		int			portToInt(const struct sockaddr_in& addr);
		std::string	intToString(int value);
	public:
		int	_logLevel[LOG_LEVEL_COUNT];
		static std::string ipToString(const struct sockaddr_in& addr);

		Logger(int verbosez = 1);
		~Logger();
		void log(logLevel logLevel, const std::string& message, int _logFlags = 0);
		void log(logLevel logLevel, const std::string& message, const RequestParser& context, int _logFlags = 0);
		void log(logLevel logLevel, const std::string& message, const Client& context, int _logFlags = 0);
		// void log(logLevel logLevel, const std::string& message, const Client& client, bool yesNo);
		// void log(logLevel logLevel, const std::string& message, const Server&server, bool yesNo);
		void log(logLevel logLevel, const std::string& message, const Client& client, const Server&obj, int _logFlags = 0);
		void log(logLevel logLevel, const std::string& message, const Server&server, int _logFlags = 0);
		static Logger& getInstance(int verbosez = 1);
};

