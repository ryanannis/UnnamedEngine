#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/Entity.h"
#include "Engine/Base/Resource/PropTree.h"


class EntityAdmin;

/*
 * The data system is driven by compositional inheritence.
 * EntitySchema is to decouple Engine Component composition
 * (which considers things like cache coherence) from composition
 * aimed at high level design.
 *
 * Schemas are defined in code, which defines the serialization
 * for a 'pack' of components.  These schemas can then be 
 * composed into entities via data-fetched resources at runtime.
 */

class EntitySchema
{
public:
	// A UNIQUE schema name for this entity
	virtual std::string GetSchemaName() const = 0;
	// Takes an entity and adds the components in this schema to it
	virtual void ProcessEntity(const PropTree& propTree, Ptr<EntityAdmin> entityAdmin, Entity entity) = 0;
};