#include "ResponseBuilder.hpp"
#include "Logger.hpp"

typedef std::map< string, map<string, vector< string > > >::iterator mapIterator;
typedef std::vector< string >::iterator vectorIterator;

void ResponseBuilder::clearingRoutes( vector< string >&routeNames, vector< string >&routeURIS ){

	vector < string > toClear;

	// extracting all uris
	for (mapIterator it = _config->routes.begin(); it != _config->routes.end(); ++it)
	{
		vector< string > curVector;
		try
		{
			curVector =  _config->routes.at(it->first).at("uri");
			if (curVector.empty())
				throw std::bad_exception();
			routeURIS.push_back(curVector[0]);
			routeNames.push_back(it->first); // get the current route Names [route1], [route2], ect...
		}
		catch(const std::exception& e)
		{
			// not URI key found in the current node, need to delete the node
			toClear.push_back(it->first);
		}
		curVector.clear();
	}

	// clear nodes without the URI
	for (vectorIterator it = toClear.begin(); it != toClear.end(); ++it)
	{
		_config->routes.erase(*it); // erase only in ResponseBuilder scope
	}

	// TODO : Delete duplicates nodes
	{
		// don't forget to update routeURIS and routeNames accordingly
	}
}

void	ResponseBuilder::buildRouteConfig( string path ){


	try
	{
		_myconfig.uri = *_config->routes.at(path).at("uri").begin();
	}
	catch(const std::exception& e)
	{
		// log something here
	}

	// ! allowedMethods
	try
	{
		_myconfig.allowedMethods = _config->routes.at(path).at("allowedMethods");
	}
	catch(const std::exception& e)
	{
		// log something here
	}

	// ! redirection
	try
	{
		_myconfig.redirection = *_config->routes.at(path).at("redirection").begin();
	}
	catch(const std::exception& e)
	{
		// log something here
	}
	
	// ! root
	try
	{
		_myconfig.root = *_config->routes.at(path).at("root").begin();
	}
	catch(const std::exception& e)
	{
		// log something here
	}

	// ! listingDirectory
	try
	{
		string myString = *_config->routes.at(path).at("listingDirectory").begin();
		_myconfig.listingDirectory = (myString[0] == '0') ? false : true;
	}
	catch(const std::exception& e)
	{
		// log something here
	}

	// ! index
	try
	{
		_myconfig.index = *_config->routes.at(path).at("index").begin();
	}
	catch(const std::exception& e)
	{
		// log something here
	}

	// ! cgiAllowed
	try
	{
		_myconfig.cgiAllowed = _config->routes.at(path).at("cgiAllowed");
	}
	catch(const std::exception& e)
	{
		// log something here
	}

	// ! uploadAllowed
	try
	{
		string myString = *_config->routes.at(path).at("uploadAllowed").begin();
		_myconfig.uploadAllowed = (myString[0] == '0') ? false : true;
	}
	catch(const std::exception& e)
	{
		// log something here
	}

	// ! uploadDirectory
	try
	{
		_myconfig.uploadDirectory = *_config->routes.at(path).at("uploadDirectory").begin();
	}
	catch(const std::exception& e)
	{
		// log something here
	}
}

void	ResponseBuilder::extractRouteConfig( void ){

	vector< string > routeNames;
	vector< string > routeURIS;

	clearingRoutes(routeNames, routeURIS);

	// string originalURI = _realURI;
	string trimmedRoute = _realURI;
	bool found = false;
	u_int8_t pos;

	// From this point, we only got route with existing `uris`
	// ! STEP 1 : Try every MainRoute in reverse order. If not found, then trim the URI from the end.
	while (not trimmedRoute.empty())
	{
		pos = 0;
		// Look accross routeURIS
		for (vectorIterator it = routeURIS.begin(); it != routeURIS.end(); ++it, ++pos)
		{
			if (*it == trimmedRoute ) // happy path, found mathcing URI
			{
				found = true;
				break;
			}
		}
		if (not found) // trimmed the URI
		{
			if (trimmedRoute.length() == 1)
				break; // not found the "/" Route
			else if (trimmedRoute.find_first_of('/') == trimmedRoute.find_last_of('/'))
			{
				if (trimmedRoute.length() == 1) // (if only left with a /)
					trimmedRoute.erase();
				else
					trimmedRoute.erase(trimmedRoute.find_first_of('/') + 1);
			}
			else
				trimmedRoute.erase(trimmedRoute.find_last_of('/'));
		}
		else
			break;
	}

	if (found)
		buildRouteConfig(routeNames[pos]);
}

void	ResponseBuilder::printMyConfig( void ){

	printColor(CYAN, "================= PRINT MY CONFIG =============\n");
	
	print("ALLOWED METHODS =");
	for (vectorIterator it = _myconfig.allowedMethods.begin(); it != _myconfig.allowedMethods.end(); ++it)
	{
		printColor(BOLD_RED, *it);
	}

	print("REDIRECTION =");
	printColor(BOLD_RED, _myconfig.redirection);
	print("ROOT =");
	printColor(BOLD_RED, _myconfig.root);

	print("LISTING-DIRECTORY =");
	printColor(BOLD_RED, _myconfig.listingDirectory ? "true" : "false");
	
	print("INDEX =");
	printColor(BOLD_RED, _myconfig.index);

	print("CGI ALLOWED =");
	for (vectorIterator it = _myconfig.cgiAllowed.begin(); it != _myconfig.cgiAllowed.end(); ++it)
	{
		printColor(BOLD_RED, *it);
	}

	print("UPLOAD ALLOWED =");
	printColor(BOLD_RED, _myconfig.uploadAllowed ? "true" : "false");

	print("UPLOAD DIRECTORY =");
	printColor(BOLD_RED, _myconfig.uploadDirectory);

	printColor(CYAN, "================= PRINT MY CONFIG =============");
}