#pragma once

#include "utils_templates.hpp"
#include "libraries.hpp"

class buildResponse
{

private:

	// Useless Coplian methods
	buildResponse( const buildResponse& copy );
	buildResponse& operator=( const buildResponse& right_operator );


	// Output Final Answer
	vector<char> _httpResponse;
	
	// Headers
	string _statusLine;
	string _timeStamp; // TODO : need to code a timestamp generator
	string _contentType;

	string _transfertEncoding; // ! OPTION ONE
	//             OR
	string _contentLenght; // ! OPTION TWO

	// Body
	string _body;

	// Utils
	u_int32_t _bodyLenght; // TODO : getter, setter, init list and <<

	// TODO : Create variables for coockies

protected:

	// Some protected members

public:

	buildResponse( void );
	~buildResponse();

	// *		---------------- GETTERS ----------------
	// ! STATUS LINE
	const vector<char> &getHttpResponse( void )const;

	// ! HEADERS
	const string &getStatusLine( void )const;
	const string &getTimeStamp( void )const;
	const string &getContentType( void )const;
	const string &getTransfertEncoding( void )const;
	const string &getContentLenght( void )const;
	
	// ! BODY
	const string &getBody( void )const;
	
	// ! Template getter
	// const string &get( void )const;
	
	// *		---------------- SETTERS ----------------
	// ! STATUS LINE
	void	setHttpResponse( vector<char>  /* could be string or char*   */ );

	// ! HEADERS
	void	setStatusLine( string );
	void	setTimeStamp( string );
	void	setContentType( string );
	void	setTransfertEncoding( string );
	void	setContentLenght( string );

	// ! BODY
	void	setBody( string input );

	// *		---------------- METHODS ----------------
	void buildBody( vector<char>& );

};

ostream& operator<<( ostream& output_stream, const buildResponse& input );