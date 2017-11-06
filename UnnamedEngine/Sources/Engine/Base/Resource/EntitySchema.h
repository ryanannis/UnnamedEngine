#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Resource/PropTree.h"

/*
 * The data system is driven by compositional inheritence.
 * Schemas are defined in code, which defines the serialization
 * for a 'pack' of components.  These schemas can then be 
 * composed into entities via data-fetched resources at runtime.
 */
class EntitySchema
{
public:
	virtual std::string GetSchemaName() const = 0;
	virtual Entity<Ptr> Deserialize(SerializedComponentTree componentTree) = 0 ;
	virtual PropTree Serialize(Entity * entity) const = 0;
};