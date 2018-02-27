#pragma once
#include "Engine/Base/Common/Common.h"

#include "PropTree.h"
#include "Engine/Base/Resource/ResourceType.h"

#include <string>
#include <vector>

class Resource;
class ComponentBase;
class EntityResource;

#define SERIALIZE(serializer, name, var) \
if(serializer.GetSerializationState() == SerializationState::SERIALIZING){ \
	serializer.Serialize(name, var);								       \
}																		   \
else{																	   \
	serializer.Deserialize(name, var);									   \
}																		   \

class ResourceManager;

enum class SerializationState
{
	SERIALIZING,
	DESERIALIZING
};

class Serializer
{
public:
	// Serializes with an empty proptree
	Serializer();

	SerializationState GetSerializationState();
	Serializer& Serialize(ComponentBase& c);
	Serializer& Serialize(std::string tag, std::string s);
	Serializer& Serialize(std::string tag, int i);
	Serializer& Serialize(std::string tag, const Vector3f& vec);

	template <typename T>
	Serializer& Serialize(std::string tag, ResourceType<T>& res)
	{
		mSerializationTree.AddLeaf(tag, res.GetURI());
		return(*this);
	}
	
	template <typename T>
	void Deserialize(std::string name, ResourceType<T>& ref)
	{
		auto leaf = mSerializationTree.leaves.find(name);
		if(leaf != mSerializationTree.leaves.end())
		{
			auto typedLeaf = dynamic_cast<PropTreeLeaf<URI>*>(leaf->second);
			if(typedLeaf)
			{
				ref = ResourceType<T>(typedLeaf->Get());
			}
		}
		// otherwise, the component doesn't exist - this is ok, just means we use default
	}

	template <typename T>
	void Deserialize(std::string name, T& ref)
	{
		auto leaf = mSerializationTree.leaves.find(name);
		if(leaf != mSerializationTree.leaves.end())
		{
			auto typedLeaf = dynamic_cast<PropTreeLeaf<T>*>(leaf->second);
			if(typedLeaf)
			{
				ref = typedLeaf->Get();
			}
		}
		// otherwise, the component doesn't exist - this is ok, just means we use default
	}

private:
	Serializer(PropTree tree, SerializationState s = SerializationState::SERIALIZING);

	std::string Serialize(std::string s) const;
	std::string Serialize(int i) const;
	
	template <typename T>
	std::string Serialize(ResourceType<T>& res) const
	{
		return(res.mURI.GetSerialized());
	}
	
	PropTree mSerializationTree;
	SerializationState mSerializationState;

	friend EntityResource;
};
