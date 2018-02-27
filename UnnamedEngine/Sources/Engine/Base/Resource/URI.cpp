#include "URI.h"


#include <fstream>
#include <sstream>

static const std::string DATA_FILE_EXTENSION = "udf";

URI::URI()
{
	mIsEmpty = true;
}

URI::URI(const std::string strURI)
{
	if(strURI == "")
	{
		mIsEmpty = true;
	}
	else
	{
		mIsEmpty = false;
	}
	

	// Do a pass to normalize all '\\' stuff into nice '/'s
	// Todo - could do this inplace...
	std::stringstream ss;
	for(size_t i = 0; i < strURI.length(); i++)
	{
		if(strURI[i] == '\\' && i + 1 < strURI.length() && strURI[i + 1] == '\\')
		{
			continue;
		}
		else if(strURI[i] == '\\')
		{
			ss << '/';
		}
		else
		{
			ss << strURI[i];
		}
	}

	// Split string
	std::vector<std::string> split;
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

std::string URI::GetCachePath() const
{
	return(CONTENT_DIR + path + file + "." + ext + ".uchc");
}

std::string URI::GetPathFromRoot() const
{
	return(path);
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

bool URI::IsEmpty() const
{
	return(mIsEmpty);
}
