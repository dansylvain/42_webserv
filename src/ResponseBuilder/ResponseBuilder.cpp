#include "ResponseBuilder.hpp"
#include "Logger.hpp"

// ------------------------- METHODS ---------------------------------

void ResponseBuilder::sanatizeURI( string &oldURI ){

	string needle = "../";

	std::string::size_type found = oldURI.find(needle);

	while (found != std::string::npos)
	{
		oldURI.erase(found, 3);
		found = oldURI.find(needle);
	}
}

bool ResponseBuilder::redirectURI( void ){

	try
	{
		_config->redirection.at(_realURI);
	}
	catch(const std::exception& e)
	{
		return false;
	}
	
	_errorType = CODE_307_TEMPORARY_REDIRECT;
	return true;
}

void ResponseBuilder::rootMapping( void ){

	// ! STEP 1 : Check for any routes matching the URI, in reverse order
	string originalURI = _realURI;
	string trimmedURI = _realURI;
	/*
		/dir1/dir2/foo/bar/www/hello.jpeg

	*/

	// Try every Route in reverse order. If not found, then trim the URI from the end.
	while (not trimmedURI.empty())
	{
		try
		{
			_config->routeMapping.at(trimmedURI);
		}
		catch(const std::out_of_range& e) // Route not found, need to trim the URI and try again
		{
			if (trimmedURI.size() == 1)
				return; // Route not found
			// If the first "/" is also the last, we're left with the last "/" default path
			else if (trimmedURI.find_first_of('/') == trimmedURI.find_last_of('/'))
			{
				// erasing keeping the "/"
				trimmedURI.erase(trimmedURI.find_first_of('/') + 1);
			}
			else
				trimmedURI.erase(trimmedURI.find_last_of('/'));
		}
	}

	// From this Point, a route has been found
	// if ()


}

void ResponseBuilder::resolveURI( void )
{
	// ! STEP 1 : Check the rootMapping
	rootMapping();
	
	// ! STEP 3 : Trim all "../" from the URI for transversal path attacks
	sanatizeURI(_realURI);

	if (_realURI.size() > 1)
	{
		_realURI.erase(_realURI.begin() + 0); // turn a regular URI ("/index.html" into "index.html")
		if ( *_realURI.rbegin() == '/' ) // Removing all ending '/' URIs
		{
			_realURI.erase(_realURI.size() -1);
		}
	}

}

void	ResponseBuilder::validateURI( void ){

	// ! STEP 1 = EDGE CASES FOR EMPTY PATH or PATH == "/"
	if (_realURI.empty())
	{
		setError(CODE_404_NOT_FOUND); return;
	}
	else if (_realURI == "/") // What if the resolved URI is a directory and not just "/"
	{
		// string originalURI = "/";
		vector<string>::iterator it;
		for (it = _config->indexFiles.begin(); it < _config->indexFiles.end(); ++it)
		{
			_realURI = *it;
			
			// Check permissions with stat
			if (stat(_realURI.c_str(), &_fileInfo) == 0)
				break ;
		}

		if (it == _config->indexFiles.end())
		{
			if (errno == EACCES) // The process does not have execute permissions on one or more directories in the path.
			{
				setError(CODE_401_UNAUTHORIZED); return;
			}
			else if (errno == ENOENT) // The file or directory does not exist.
			{
				setError(CODE_404_NOT_FOUND); return;
			}
		}
	}

	// TODO = Is URI a CGI ??
	checkCGI();

	// TODO = Does the route accepts the METHOD ?
	{
		// Set un fichier par défaut comme réponse si la requête est un répertoire.
	}
	
	checkNatureAndAuthoURI();

	if (_isDirectory and (_method == GET) and (not _isCGI))
	{
		generateListingHTML();
	}

}


void	ResponseBuilder::buildHeaders(){

	errorCode codes;

	stringstream	streamStatusLine,
					streamTimeStamp,
					streamContentLenght,
					streamMasterHeader;

	
	// -------------- Madatory Headers --------------  

	streamStatusLine	<< HTTP_PROTOCOL
						<< SPACE
						<< _errorType
						<< SPACE 
						<< codes.getCode(_errorType)
						<< HTTP_HEADER_SEPARATOR;
	Headers.statusLine = streamStatusLine.str();

	streamTimeStamp		<< "Date:"
						<< SPACE 
						<< timeStamp::getTime()
						<< HTTP_HEADER_SEPARATOR;
	Headers.timeStamp = streamTimeStamp.str();
	
	// Content Lenght
	streamContentLenght	<< "Content-Length:"
						<< SPACE
						<< Headers.bodyLenght
						<< HTTP_HEADER_SEPARATOR;
	Headers.contentLenght = streamContentLenght.str();

	
	// --------------  Optionals Headers --------------  

	if (Headers.bodyLenght > 0)
	{
		stringstream streamContentType;
		
		// Content Type
		string contentType = extractType(_fileExtension);
		streamContentType	<< "Content-Type:"
							<< SPACE 
							<< contentType 
							<< HTTP_HEADER_SEPARATOR;
		
		Headers.contentType = streamContentType.str();
	}

	if (isErrorRedirect())
	{
		stringstream streamLocation;
		streamLocation	<< "Location:"
						<< SPACE
						<< _config->redirection.at(_realURI)
						// << "http://www.github.com/maitreverge"
						<< HTTP_HEADER_SEPARATOR;
		Headers.location = streamLocation.str();
	}

	// TODO : Coockie and session generator
	{
		
	}
	// ----------------- Building Final Headers ----------------

	streamMasterHeader	<< Headers.statusLine
						<< Headers.timeStamp
						<< Headers.contentType 
						// Optionals
						<< Headers.location
						// << (Headers.bodyLenght ? Headers.contentLenght : Headers.transfertEncoding)
						<< Headers.contentLenght
						<< HTTP_HEADER_SEPARATOR; // HEADER / BODY SEPARATOR
	
	string tempAllHeaders = streamMasterHeader.str();

	// Insert all headers in a vector char
	Headers.masterHeader.insert(Headers.masterHeader.end(), tempAllHeaders.begin(), tempAllHeaders.end());
}

void	ResponseBuilder::getHeader( Client &inputClient, Config &inputConfig ){

	Logger::getInstance().log(DEBUG, "Response Builder Get Header", inputClient);
		
	_client = &inputClient; // init client
	_config = &inputConfig; // init config
	
	_realURI = _client->header.getURI();

	extractMethod();
	if ( not redirectURI())
	{
		
		resolveURI();
		validateURI();
		
		if (_isCGI and _errorType <= CODE_400_BAD_REQUEST) // or potentially another adress
			launchCGI();
		
		checkNatureAndAuthoURI();
		setContentLenght();

	}
	
	buildHeaders();


	// Copying the build Headers in headerSend
	inputClient.headerSend = Headers.masterHeader;
	
	Headers.masterHeader.clear();//!

	// printAllHeaders();
}

