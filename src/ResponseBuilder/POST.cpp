#include "ResponseBuilder.hpp"
#include "Logger.hpp"

void	ResponseBuilder::getHeaderPost( Client &inputClient, Config &inputConfig ){

	// Se mettre d'accord avec Seb pour le feedback de Post pour l'URI.

	Logger::getInstance().log(DEBUG, "Response Builder POST Header", inputClient);
		
	_client = &inputClient; // init client
	_config = &inputConfig; // init config
	
	_realURI = _client->headerRequest.getURI();

	_errorType = CODE_201_CREATED;
	/*
		! POST RETURN VALUES
		201 Created: 
			This status code is common if the POST request results in the creation of a new resource (e.g., a new file, record, or entry).
		200 OK:
			This is used if the POST request is successful,
			but no new resource is specifically created
			(e.g., form submission or data processing).
		204 No Content:
			This status code is used if the POST request is processed successfully
			but the server does not need to return any additional information
			(i.e., no response body is required).
	*/
	extractMethod();
	// if ( not redirectURI())
	// {
		
		// resolveURI();
		validateURI();
		
		// if (_isCGI and _errorType <= CODE_400_BAD_REQUEST) // or potentially another adress
		// 	launchCGI();
		
		checkNatureAndAuthoURI();
		setContentLenght();

	// }


	

	buildHeaders();

}

void	ResponseBuilder::setBodyPost(){

}