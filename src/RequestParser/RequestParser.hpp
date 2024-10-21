#pragma once

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <cstring>
#include <sstream>

struct Headers
{
	std::string							Connection;
	std::string							ContentType;
	std::string							Host;
	std::vector<std::string>			Accept;
	size_t								ContentLength;
	std::map<std::string, std::string>	Cookie;
	
	Headers();
};

/**========================================================================
 *                           REQUESTPARSER
 * ? gets the request as a std::vector<char>
 * ? parses it and fills its attributes (including 'Headers' struct)
 *========================================================================**/
class RequestParser
{
	private:
		//attributes
		struct Client										*_Client;
		std::string											_method;
		std::string											_URI;
		std::string											_HTTP_version;
		bool												_isValid;
		std::map<std::string, std::vector<std::string> >	_tmpHeaders;
		Headers												_Headers;

		// coplien
		// RequestParser(const RequestParser& other);
		// RequestParser& operator=(const RequestParser& rhs);
		
		// utils
		void		trim(std::string& str);
		std::string	charVectorToString(const std::vector<char>& vector);

		// action
		void	handleFirstLine(std::istringstream& requestStream);
		void	handleHeaderLines(std::istringstream& requestStream);
		void	extractHeaders();
		void	assignHeader(const std::string& key, std::string& value);
		void	assignHeader(const std::string& key, std::vector<std::string>& headerField);
		void	assignHeader(const std::string& key, size_t& headerField);
		void	assignHeader(const std::string& key, std::map<std::string, std::string>& cookieField);
		std::map<std::string, std::string>	extractCookies(std::vector<std::string> vec);
		void	displayHeaders() const;
	
	public:
		//coplien
		RequestParser();
		~RequestParser();
	
		// getters
		std::string	getMethod() const;
		std::string	getURI() const;
		std::string	getHTTP_version() const;
		Headers		getHeaders() const;
		Client		*getClient() const;

		// display methods
		void 		displayParsingResult() const;
		//! display methods must be changed in file server.cpp!
		void	displayAttributes() const;

		// main method
		void	parse(struct Client& client);
};