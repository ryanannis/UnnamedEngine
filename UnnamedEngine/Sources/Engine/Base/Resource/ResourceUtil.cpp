#include "ResourceUtil.h"
#include "Engine/Base/Utility/SingletonLogger.h"

#include <iostream>
#include <fstream>

std::vector<char> ResourceUtil::LoadBinaryFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary);
      if(file.fail())
	  {
		SingletonLogger::Log("Loader", LogType::ERROR).Log("Could not open file " + filename);
		assert(false);
      }

      std::streampos begin = file.tellg();;
	  std::streampos end;
      file.seekg( 0, std::ios::end );
      end = file.tellg();

      std::vector<char> result(static_cast<size_t>(end - begin));
      file.seekg(0, std::ios::beg);
      file.read(&result[0], end - begin);
      file.close();

      return(result);
}