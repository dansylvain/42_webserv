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
	bool				samePathWrite;
	string				indexRedirection;

	MyConfig()
	{
		allowedMethods.push_back("GET");
		allowedMethods.push_back("POST");
		allowedMethods.push_back("DELETE");
		redirection.clear();
		uri.clear();
		root.clear();
		listingDirectory = false;
		index.clear();
		cgiAllowed.clear();
		uploadAllowed = true;
		uploadDirectory.clear();

	// ========== my stuff ==========
		samePathWrite = true;
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

	// Enum
	typedef enum
	{
		GET,
		POST,
		DELETE
	} e_method;

	typedef enum
	{
		TOKEN_DELIM,
		TOKEN_END,
		LINE_SEPARATOR,
		BINARY_DATA,
		CONTENT_DISPOSITION
	} e_lineNature;

	// ------------- Priv Variables
	
	map<string, string> _mimeTypes;

	string _realURI;

	string _originalURI;
	e_method _method;

	// Nature File
	bool _isDirectory;
	bool _isFile;

	// errorNotFoundGenerator
	bool _errorNotFound;
	const string _backupNameFile;

	// CGI Stuff
	bool _isCGI;
	e_errorCodes _errorType;

	// Struct for File Info
	struct stat _fileInfo; // ! PAS DANS LES CONSTRUCTEURS

	// File Characteristics
	bool _isROK;
	bool _isWOK;
	bool _isXOK;


	Client* _client;
	Config* _config;

	MyConfig _myconfig;

	ResponseHeaders Headers;

	// ------------- Priv Methods
	void	resolveURI( void );
	void	sanatizeURI( string & );
	void	buildHeaders( void );
	void	setContentLenght( void ); // not a regular setter
	void	extractMethod( void );
	string	extractType( const string& extension ) const;
	void	initMimes( void );
	void	checkAutho( void );
	void	checkNature( void );
	bool 	redirectURI( void );
	void 	rootMapping( void );
	bool	isErrorRedirect( void );
	void	extractFileNature( string &target);
	void	checkMethod( void );
	void	uploadCheck( void );


	// extractRouteConfig
	void	extractRouteConfig( void );
	void	clearingRoutes( vector< string >&routeNames, vector< string >&routeURIS );
	void	buildRouteConfig( string path );
	void	printMyConfig( void );
	
	bool	isDirectory(string &uri);

	void	slashManip( string& target );
	
	void extractRedirectionIndex( vector< string >&routeNames, vector< string >&routeURIS );

	void	errorNotFoundGenerator( void );



	// generateListingHTML.cpp
	/*
		map<string, timespec> _lastDir_M_Time;
		map<string, timespec> _lastDir_C_Time;
		bool	isDirectoryUnchanged( void );
	*/
	void	generateListingHTML( void );
	bool	foundDefaultPath( void );
	void	listingHTMLBuilder( void);


	// CGI.cpp
	void	checkCGI( void );

	// POST
	bool	isLineDelim( vector< char >& curLine, vector< char >& nextLine);
	e_lineNature	processCurrentLine( vector< char >& curLine );

	void	boundarySetBodyPost( Client & client, bool eof );
	void	initBoundaryTokens( void );
	// ! TO DELETE, serves as a blueprint
	void	_setBodyPost( Client & client, bool eof );

	
	string _tokenDelim;
	string _tokenEnd;
	string _postFileName;

	// DELETE
	void	deleteEngine( void );


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

	// ✅ GET ONLY
	void	getHeader( Client &, Config&, e_errorCodes codeInput = CODE_200_OK );
	bool	getBody( Client &inputClient );
	Cgi		_cgi;//! provisoire sinon private

	// ✅ POST ONLY
	void	getHeaderPost( Client &, Config &, e_errorCodes codeInput = CODE_200_OK );
	void	setBodyPost( Client & client, bool eof );



	// For testing
	void	setMethod( const e_method& method );

	void	printAllHeaders( void )const;

	class CodeErrorRaised : public exception
	{
		public:
			virtual const char* what( void ) const throw()
			{
				return ("CODE ERROR RAISED");
			}
	};

	// Public method for CGI error timeout
	void	setError( e_errorCodes, bool skip = false );
};
