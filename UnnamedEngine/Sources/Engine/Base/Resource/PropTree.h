#pragma once
#include "Engine/Base/Common/Common.h"

#include <unordered_map>
#include <optional>

// for << overrides
#include "Engine/Base/Resource/MaterialResource.h"
#include "Engine/Base/Resource/ModelResource.h"

enum class ParsedTokenType
{
	NONE,
	LBRACKET,
	RBRACKET,
	LSQUARE,
	RSQUARE,
	LCURLY,
	RCURLY,
	COLON,
	COMMA,
	ALPHANUM,
	NUMBER,
	STRING,
	DECIMALNUMBER,
	FILEPATH,
	EQUALS,
	SEMI
};

struct UDFToken
{
	UDFToken(ParsedTokenType type, std::string value) : type(type), value(value) {}
	const ParsedTokenType type;
	const std::string value;
};

struct URI;
class PropParser;
class Serializer;

enum class ValueType
{
	String,
	Float,
	Int,
	URI,
	Vector3
};

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
	std::optional<URI> GetAsURI() const;
	std::optional<Vector3f> GetAsVector() const;

private:
	ValueType mType;
	void* mData;
	
	explicit PropTreeLeaf(std::vector<UDFToken>&& tokens);
	friend PropParser;
	friend Serializer;
};

class ModelResource;
class MaterialResource;

void operator<<(ResourceType<ModelResource>& res, const PropTreeLeaf& p);
void operator<<(ResourceType<MaterialResource>& res, const PropTreeLeaf& p);
void operator<<(std::string& val, const PropTreeLeaf& p);
void operator<<(float& val, const PropTreeLeaf& p);
void operator<<(int& val, const PropTreeLeaf& p);
void operator<<(URI& val, const PropTreeLeaf& p);
void operator<<(Vector3f& val, const PropTreeLeaf& p);

struct PropTree
{
public:
	PropTree() {}
	std::unordered_map<std::string, PropTree> components;
	std::unordered_map<std::string, PropTreeLeaf> leaves;
};
