#include "ConfigFileParser.hpp"

using namespace std;
map<string, map<string, vector<string> > > data;

void	print(string str)
{
	cout << str << endl;
}

void	trim(string& str)
{
	str.erase(0, str.find_first_not_of(" \t\r\n"));
	str.erase(str.find_last_not_of(" \t\r\n") + 1);
}

void printData(const map<string, map<string, vector<string> > >& data) {
	for (map<string, map<string, vector<string> > >::const_iterator catIt = data.begin(); catIt != data.end(); ++catIt)
	{
		cout << "Category: " << catIt->first << endl;
				for (map<string, vector<string> >::const_iterator itemIt = catIt->second.begin(); itemIt != catIt->second.end(); ++itemIt)
		{
			cout << "  Key: >" << itemIt->first +"<" << endl;
			cout << "  Values: >";
			for (vector<string>::const_iterator valueIt = itemIt->second.begin(); valueIt != itemIt->second.end(); ++valueIt)
			{
				cout << *valueIt << "< >"; 
			}
			cout << endl;
		}
	}
}
int	ignoreComents(string& line)
{
	size_t firstChar = line.find('#');
	if (firstChar != string::npos)
		line.erase(firstChar);
	firstChar = line.find_first_not_of(" \t");
	if (firstChar != string::npos)
		line.erase(0, firstChar);
	firstChar = line.find("]");
	if (firstChar != string::npos)
		line.erase(firstChar + 1);
	if (line.find_first_not_of(" \t") == string::npos)
		return (0);
	return (1);
}

int	getCurrentCategory(string& line, string& currentCategory)
{
	if (!line.empty() && line[0] == '[')
	{
		size_t lastChar = line.find_last_not_of(" \t");
		if (lastChar != string::npos && line[lastChar] == ']')
		{
			currentCategory = line.substr(1, lastChar - 1);
			print("\nCurrent category: " + currentCategory);
			if (data.find(currentCategory) == data.end()) {
				data[currentCategory] = map<string, vector<string> >();
				print("Added new category: " + currentCategory);
			}
			return (0);
		}
	}
	print(currentCategory + " =>	" + line);
	return (1);
}

void	extractKeyValuePairs(string& line, string& currentCategory)
{
	size_t colonPos = line.find('=');
	if (colonPos != string::npos)
	{
		string key = line.substr(0, colonPos);
		string value = line.substr(colonPos + 1);
		trim(key); trim(value);
		istringstream valueStream(value);
		string singleValue;
		while (getline(valueStream, singleValue, ','))
		{
			trim(singleValue);
			data[currentCategory][key].push_back(singleValue);
		}
	}
}

int main(void)
{
	ifstream file("config.ini");
	if (!file.is_open())
		return (cerr << "could not open config file" << endl, 1);
	print("Config file opened");
	string line;
	string currentCategory;
	while (getline(file, line))
	{
		// ignore comments
		if (ignoreComents(line) == 0)
			continue ;
		// get current category
		if(getCurrentCategory(line, currentCategory) == 0)
			continue ;
		// add key-value(s) to category
		extractKeyValuePairs(line, currentCategory);
	}
	file.close();
	print("Config file closed");
	printData(data);
	return (0);
}