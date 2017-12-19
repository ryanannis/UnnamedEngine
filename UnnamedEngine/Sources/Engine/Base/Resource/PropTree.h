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

// A simple wrapper
class PropTreeLeafBase {
public:
	PropTreeLeafBase() {};
	virtual ~PropTreeLeafBase() {};
};

template <typename T>
class PropTreeLeaf : public PropTreeLeafBase {
public:
	PropTreeLeaf(const T& t) : object(t) {};
	const T& Get() const { return(object); };
private:
	T object;
};

class ModelResource;
class MaterialResource;

struct PropTree
{
public:
	PropTree() {}
	~PropTree() {}
	std::unordered_map<std::string, PropTree> components;
	std::unordered_map<std::string, PropTreeLeafBase*> leaves;

	template <typename T>
	PropTreeLeaf<T>* GetLeaf(std::string s) const
	{
		auto leaf = leaves.find(s);
		assert(leaf != leaves.end());
		return(dynamic_cast<PropTreeLeaf<T>*>(leaf->second));
	}

	void AddLeaf(std::string name, const std::string& leaf);
	void AddLeaf(std::string name, float leaf);
	void AddLeaf(std::string name, int leaf);
	void AddLeaf(std::string name, const URI& leaf);
	void AddLeaf(std::string name, const Vector3f& leaf);

private:
	PropTree & operator=(const PropTree&) = delete;
	PropTree(const PropTree&) = delete;
};
