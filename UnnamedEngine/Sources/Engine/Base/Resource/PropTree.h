#pragma once
#include "Engine/Base/Common/Common.h"
#include <unordered_map>

#include <optional>

class PropParser;
class Serializer;
struct UDFToken;

// A container for value tokens parsed by the PropParser
// Our language is looser than something eg. JSON
// so the token type cannot be determined strictly from
// the language itself, and is contextually dependent.
struct PropTreeLeaf
{
public:
	std::string GetAsString() const;
	std::optional<float> GetAsFloat() const;
	std::optional<uint32_t> GetAsInt() const;
	std::optional<uint32_t> GetAsFilePath() const;


private:
	const std::vector<UDFToken> mTokens;
	
	explicit PropTreeLeaf(std::vector<UDFToken>&& tokens);
	friend PropParser;
	friend Serializer;
};

struct PropTree
{
public:
	PropTree() {}
	std::unordered_map<std::string, PropTree> components;
	std::unordered_map<std::string, PropTreeLeaf> leaves;
};