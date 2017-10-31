#include "ResourceManager.h"

#include <fstream>
#include <sstream>

static const std::string DATA_FILE_EXTENSION = ".udf";

Resource* ResourceManager::SpotLoadResource(const std::string dataLocation)
{
	// Read file into buffer
	std::ifstream t("dataLocation" + DATA_FILE_EXTENSION);
	std::stringstream buffer;
	buffer << t.rdbuf();
}

Resource* ResourceManager::LoadResourceAsync(const std::string dataLocation)
{
	return(nullptr);
}
