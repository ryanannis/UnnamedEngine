#pragma once
#include "Engine/Base/Common/Common.h"

// Handles both file references (eg. /Content/Test/Bunny.udf)
// and data deferences (eg. /Content/Test/TestFile/TestEntity -> /Content/Test/TestFile : Entity)
struct URI
{
public:
	URI() = default;
	URI(const std::string strURI);
	std::string GetFilePath() const;
	std::string GetComponent() const;
	std::string GetExtension() const;
	std::string GetHash() const;
	std::string GetSerialized() const;

private:
	std::string path;
	std::string file;
	std::string ext;
	std::string component;
};
