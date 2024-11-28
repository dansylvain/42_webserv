#pragma once

#include "utils_templates.hpp"
#include "errorCode.hpp"
#include "timeStamp.hpp"
#include "Config.hpp"
#include "Cgi.hpp"

#define HTTP_PROTOCOL "HTTP/1.1"
#define SPACE " "
#define HTTP_HEADER_SEPARATOR "\r\n"

struct Client;
struct Config;
class MockConfig;

struct ResponseHeaders
{
	vector<char> masterHeader;
	
	string statusLine;
	string timeStamp;
	string contentType;
	string transfertEncoding; // ! OPTION ONE
	// !           OR
	string contentLenght; // ! OPTION TWO
	
	// TODO : All Headers
	string location; // Only if redirection

	// Utils
	uint64_t bodyLenght;

	// ! ADD COCKIES HERE
	
	ResponseHeaders()
	{
		masterHeader.clear();
		bodyLenght = 0;
	}
};

struct MyConfig
{
	string				uri; // ! main key

	vector< string >	allowedMethods; // exploited ✅
	string				redirection; // exploited ✅
	string				root; // exploited ✅
	bool				listingDirectory; // exploited ✅
	string				index; // exploited ✅
	vector< string >	cgiAllowed; // exploited ✅
	bool				uploadAllowed; // exploited ✅
	string				uploadDirectory; // checked access ✅

	// ========== my stuff ==========
	string				indexRedirection;

	MyConfig()
	{
		allowedMethods.clear();
		redirection.clear();
		uri.clear();
		root.clear();
		listingDirectory = false;
		index.clear();
		cgiAllowed.clear();
		uploadAllowed = true;
		uploadDirectory.clear();

	// ========== my stuff ==========
		indexRedirection.clear();

	}
};

class ResponseBuilder
{

	#ifdef UNIT_TEST
	public:
	#else
	private:
	#endif

	typedef enum
	{
		TOKEN_DELIM,
		TOKEN_END,
		LINE_SEPARATOR,
		BINARY_DATA,
		CONTENT_DISPOSITION,
		OTHER
	} e_lineNature;

	// ------------- Priv Variables
	Client* _client;
	Config* _config;
	
	MyConfig _myconfig;
	ResponseHeaders Headers;

	struct stat _fileInfo; // ! PAS DANS LES CONSTRUCTEURS

	map<string, string> _mimeTypes;
	
	string _realURI;
	string _originalURI;


	// Nature File
	bool _isDirectory;
	bool _isFile;

	// CGI Stuff
	bool _isCGI;

	// File Characteristics
	bool _isROK;
	bool _isWOK;
	bool _isXOK;

	// errorNotFoundGenerator
	bool _deleteURI; // used for auto generated errors or listing.html
	string _backupNameFile;

	// CGI Stuff
	e_errorCodes _errorType;

	bool _isMultipart;
	string _setBodyExtension;


	// ===================== METHODS ==================

	// setBody.cpp
	void	initCurrentFiles( vector< string> & );

	bool isLineDelim(vector<char> &, vector<char> &);
	void determineSeparator(std::string &separator, size_t &separatorLength, vector<char>& curLine);
	e_lineNature	processCurrentLine( vector< char >&  );
	void	initBoundaryTokens( void );
	void	extractFileBodyName( vector< char >&, vector< string >& );
	void	setRegularPost( Client & client );
	void	setMultiPartPost( Client & client );
vector<char>::iterator searchSeparator(vector<char>& curLine, string &separator, size_t &separatorLength);


	// buildHeaders.cpp
	void	buildHeaders( void );

	// CGI.cpp
	void	checkCGI( void );

	// cookies.cpp

	// coplianForm.cpp
	void	initMimes( void );

	// deleteEngine.cpp
	void	generateDeleteHTML( void );
	void	deleteEngine( void );

	// errorNotFoundGenerator.cpp
	void	errorNotFoundGenerator( void );

	// generateListingHTML.cpp
	bool	foundDefaultPath( void );
	bool	isFileIgnored( string & );
	void listingHTMLBuilder(void);
	void makeHeaderListing(std::stringstream &result);
	void	generateListingHTML( void );

	// ResponseBuilder.cpp
	void	resolveURI( void );
	void	sanatizeURI( string & );
	bool 	redirectURI( void );
	void 	rootMapping( void );
	void	checkMethod( void );

	// utilsResponseBuilder.cpp
	string	extractType( const string& ) const;
	void	extractMethod( void );
	void	setContentLenght( void ); // ! not a regular setter
	void	uploadCheck( void );
	void	checkAutho( void );
	void	extractFileNature( string &);
	void	checkNature( void );
	bool	isErrorRedirect( void );

	string _tokenDelim;
	string _tokenEnd;
	string _postFileName;

	string _fileStreamName;

	bool _writeReady;
	bool _parsedBoundaryToken;

	// extractRouteConfig
	void extractRouteConfig(void);
	void extraStartingChecks();
	void resetMyVariables();
	void	clearingRoutes( vector< string >&, vector< string >& );
	void	buildRouteConfig( string );
	void	printMyConfig( void );
	
	bool	isDirectory(string &);

	void	slashManip( string& );
	
	void extractRedirectionIndex( vector< string >&, vector< string >& );

	void pathSlashs(string &);

	string _uploadTargetDirectory;

	string generateFileName( void );

	string generateRandomString(size_t, bool underscoreNeeded = false );



public:

	std::ifstream 	_ifs; // ! PAS DANS LES CONSTRUCTEURS
	std::streampos	_ifsStreamHead; // ! ABSOLUMENT METTRE DANS LES CONSTRUCTEURS
    std::ofstream	_ofs; //! need public for seb
	string 			_pathInfo; //! need public for seb
	string 			_folderCGI; //! need public for seb
	string 			_fileName; //! need public for seb
	string 			_fileExtension; //! need public for seb

	ResponseBuilder( void );
	~ResponseBuilder( void );
	ResponseBuilder( const ResponseBuilder & );
	ResponseBuilder & operator=( const ResponseBuilder & rhs);

	void	getHeader( Client &, Config&, e_errorCodes codeInput = CODE_200_OK );
	bool	getBody( Client &inputClient );
	
	Cgi		_cgi;//! provisoire sinon private

	void	setBody( Client & client, bool eof );


	void	printAllHeaders( void )const;

	class CodeErrorRaised : public exception
	{
		public:
			virtual const char* what( void ) const throw()
			{
				return ("CODE ERROR RAISED");
			}
	};

	// Enum
	typedef enum
	{
		GET,
		POST,
		DELETE
	} e_method;

	e_method _method;

	// Public method for CGI error timeout
	void	setError( e_errorCodes, bool skip = false );
	e_method getMethod( void );
	// For testing
	void	setMethod( const e_method& method );

};
