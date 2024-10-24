#include "ResponseBuilder.hpp"

// ------------------------- COPLIAN FORM -----------------------------
ResponseBuilder::ResponseBuilder( void ) :
	_isHeaderDone(false),
	_isDirectory(false),
	_isCGI(false),
	_errorType(CODE_200_OK){

	_mimeTypes.insert(std::make_pair("html", "text/html"));
	_mimeTypes.insert(std::make_pair("htm", "text/htm"));
	_mimeTypes.insert(std::make_pair("txt", "text/txt"));
	_mimeTypes.insert(std::make_pair("css", "text/css"));
	_mimeTypes.insert(std::make_pair("xml", "text/xml"));
	// Application Content Types
	_mimeTypes.insert(std::make_pair("js", "application/javascript")); // FIXME doubt on this one
	_mimeTypes.insert(std::make_pair("json", "application/json"));
	_mimeTypes.insert(std::make_pair("pdf", "application/pdf"));
	_mimeTypes.insert(std::make_pair("zip", "application/zip"));
	// Image Content Types
	_mimeTypes.insert(std::make_pair("jpeg", "image/jpeg"));
	_mimeTypes.insert(std::make_pair("jpg", "image/jpg"));
	_mimeTypes.insert(std::make_pair("png", "image/png"));
	_mimeTypes.insert(std::make_pair("gif", "image/gif"));
	_mimeTypes.insert(std::make_pair("webp", "image/webp"));
	_mimeTypes.insert(std::make_pair("bmp", "image/bmp"));
	// Audio Content Types
	_mimeTypes.insert(std::make_pair("mp3", "audio/mp3"));
	_mimeTypes.insert(std::make_pair("mpeg", "audio/mpeg"));
	_mimeTypes.insert(std::make_pair("ogg", "audio/ogg"));
	_mimeTypes.insert(std::make_pair("wav", "audio/wav"));
	// Video Content Types
	_mimeTypes.insert(std::make_pair("mp4", "video/mp4"));
	_mimeTypes.insert(std::make_pair("webm", "video/webm"));
	_mimeTypes.insert(std::make_pair("ogv", "video/ogv"));
}


ResponseBuilder::ResponseBuilder( const ResponseBuilder& copy ){ static_cast<void>(copy); }


ResponseBuilder& ResponseBuilder::operator=( const ResponseBuilder& right_operator ){

	static_cast<void>(right_operator);
	return *this;
}


ResponseBuilder::~ResponseBuilder( void ){}


// ------------------------- METHODS ---------------------------------


void ResponseBuilder::sanatizeURI( string &oldURI ){

	string needle = "../";

	std::string::size_type found = oldURI.find(needle);

	while (found != -1)
	{
		oldURI.erase(found, 3);
		found = oldURI.find(needle);
	}
}


void ResponseBuilder::resolveURI( Client& inputClient, Config& config )
{
	// /images/../config/../../logo.png

	_realURI = _client->header.getURI();

	// ! STEP 1 : Trim all "../" from the URI for transversal path attacks
	sanatizeURI(_realURI);

	// TODO STEP 2 : Resolve URI with rooted path from config file
}

void	ResponseBuilder::initialChecks( Client& inputClient, Config& config ){

	// ! STEP 1 = RESSOURCE EXISTS AND READEABLE ?
	if (_realURI.empty())
		_errorType = CODE_404_NOT_FOUND;
	else if (_realURI == "/") // What if the resolved URI is a directory and not just "/"
	{
		vector<string>::iterator it;
		for (it = config.indexFiles.begin(); it < config.indexFiles.end(); ++it)
		{
			_realURI += *it;
			
			// Check permissions with stat
			if (stat(_realURI.c_str(), &_fileInfo) == 0)
				break ;
		}

		if (it == config.indexFiles.end())
		{
			if (errno == EACCES) // The process does not have execute permissions on one or more directories in the path.
				_errorType = CODE_401_UNAUTHORIZED;
			else if (errno == ENOENT) // The file or directory does not exist.
				_errorType = CODE_404_NOT_FOUND;
		}
		else // ressource found
		{
			bool readRights = (_fileInfo.st_mode & S_IRUSR) || (_fileInfo.st_mode & S_IRGRP);
			if (not readRights)
				_errorType = CODE_401_UNAUTHORIZED;
		}
	}
	else // check classic path
	{
		if (stat(_realURI.c_str(), &_fileInfo) == -1) // if given path fails
		{
			if (errno == EACCES)
				_errorType = CODE_401_UNAUTHORIZED;
			else if (errno == ENOENT or errno == EFAULT) // EFAULT The provided path is invalid or points to a restricted memory space.
				_errorType = CODE_404_NOT_FOUND;
		}
		else // Supposed valid path
		{
			/*
				! STAT FLAGS : https://www.man7.org/linux/man-pages/man7/inode.7.html
			*/
			if (_fileInfo.st_mode & S_IFMT == S_IFDIR) // checks is the given path is a directory
				_isDirectory = true;
		}
	}

	// Implementer un booleen qui signifie que une erreur donnee 

	// TODO = Does the route accepts the METHOD ?
	{
		// Set un fichier par défaut comme réponse si la requête est un répertoire.
	}

	// TODO = Is URI a CGI ??
	{

	}


	// _isCheckingDone = true;
}

void ResponseBuilder::launchCGI( void ){

	// TODO
}

void	ResponseBuilder::buildHeaders(){

	errorCode codes;

	stringstream	streamStatusLine,
					streamTimeStamp,
					streamContentType,
					streamContentLenght,
					streamMasterHeader,
					stream;

	streamStatusLine	<< HTTP_PROTOCOL
						<< SPACE
						<< _errorType // DETERMINE AN ERROR BY DEFAULT
						<< SPACE 
						<< codes.getCode(_errorType)
						<< HTTP_REPONSE_SEPARATOR;
	
	Headers.statusLine = streamStatusLine.str();

	streamTimeStamp	<< "Date:"
					<< SPACE 
					<< timeStamp::getTime() // ! TimeStamp here
					<< HTTP_REPONSE_SEPARATOR;

	Headers.timeStamp = streamTimeStamp.str();

	
	// * Content-Type (if body)
	if (this->_bodyLenght > 0)
	{
		string contentType = buildContentType(fileName);
		streamContentType	<< "Content-Type:"
							<< SPACE 
							<< contentType 
							<< HTTP_REPONSE_SEPARATOR;
		
		_contentType = streamContentType.str();
	}

	// ContentLenght
	streamContentLenght	<< "Content-Length:"
						<< SPACE
						<< _bodyLenght
						<< HTTP_REPONSE_SEPARATOR;
	
	_contentLenght = streamContentLenght.str();

	// Building Final Headers
	streamMasterHeader	<< _statusLine
						<< _timeStamp
						<< _contentType
						<< (_bodyLenght ? _contentLenght : _transfertEncoding)
						<< HTTP_REPONSE_SEPARATOR;
	
	_masterHeader = streamMasterHeader.str();

	
	
	// FIXME
	// ! Potential hangling HTTP_SEPARATOR if there is extra headers or not !!!!
}

void	ResponseBuilder::setContentLenght(){

	if (stat(_realURI.c_str(), &_fileInfo) == -1)
	{
		if (errno == EACCES)
			_errorType = CODE_401_UNAUTHORIZED;
		else if (errno == ENOENT or errno == EFAULT)
			_errorType = CODE_404_NOT_FOUND;
	}
	else // correct path 
		Headers.bodyLenght = _fileInfo.st_size;
}

	

vector<char>	ResponseBuilder::getHeader( Client &inputClient, Config &config ){

	_client = &inputClient; // init client

	{
		// TODO How do I know if the call is a errorCall or a regular call ?
	}

	// StatusCode set up by Dan in errorProcress

	resolveURI(inputClient, config);
	initialChecks(inputClient, config);

	if (_errorType >= 400)
	{
		// TODO Remplace URI with the 404.html path from config files
		// _realURI = config.getError404
	}
	else if (_isDirectory)
	{
		if (config.listingDirectories)
		{
			// TODO :  generate an HTML page for listing directories
			// _realURI = getListing.html
		}
		else
		{
			// man idk ?
		}
	}
	else if (_isCGI)
	{
		launchCGI(); // CGI must write within a file
		// _realURI = outputCGI.html
	}

	/*
		! At this point of the function, the _readURI has ben correctly set up
		We can safely extract the ContentLenght with `stat`
	*/
	setContentLenght();
	buildHeaders();


	return Headers._masterHeader;
}


void			ResponseBuilder::getBody( Client &, Config& ){


}