#include "URI.h"


#include <fstream>
#include <sstream>

static const std::string DATA_FILE_EXTENSION = "udf";

URI::URI(const std::string strURI)
{
	// Split string
	std::vector<std::string> split;
	std::stringstream ss(strURI);
	std::string item;
	while(getline(ss, item, '/')) {
		split.push_back(item);
	}

	// We consider the path, [split] from 0 to pathIndex
	size_t pathIndex = 0;

	if(split.size() > 1)
	{
		std::string lastToken = split[split.size() - 1];
		std::string secondLastToken = split[split.size() - 2];
		size_t dotLocation = lastToken.find('.');

		// No dot - must be a serialized resource
		if(dotLocation == std::string::npos)
		{
			component = lastToken;
			ext = DATA_FILE_EXTENSION;
			file = secondLastToken;
			pathIndex = split.size() - 2;
		}
		else
		{
			ext = lastToken.substr(dotLocation + 1);
			file = lastToken.substr(0, dotLocation);
			pathIndex = split.size() - 1;
		}

		std::stringstream restOfPath;
		for(size_t i = 0; i < pathIndex; i++)
		{
			restOfPath << split[i] << '/';
		}
		path = restOfPath.str();
	}
}

std::string URI::GetFilePath() const
{
	return(CONTENT_DIR + path + file + "." + ext);
}

std::string URI::GetComponent() const
{
	return(component);
}

std::string URI::GetExtension() const
{
	return(ext);
}

std::string URI::GetHash() const
{
	return(GetFilePath());
}

std::string URI::GetSerialized() const
{
	//todo
	return(GetFilePath());
}