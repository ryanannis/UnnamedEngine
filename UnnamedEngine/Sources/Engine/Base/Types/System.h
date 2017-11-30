#pragma once
#include "Engine/Base/Common/Common.h"

#include <algorithm>
#include <optional>

#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Types/Entity.h"
#include "Engine/Base/Types/SystemBase.h"

class SystemAdmin;

typedef uint64_t SystemFlag;

struct SystemStatic
{
	static SystemFlag sSystemGroup;
};

template <typename Derived>
class System : public SystemBase {
public:
	System() = default;
	virtual void StaticInitDependencies() = 0 {};
	virtual void Update(float dt, Ptr<EntityAdmin> entityAdmin) = 0;
	
	// This will be used by the job scheduler - write dependencies are a subset
	// of read dependncies -- items in write dependencies do not need to be
	// returned from GetSystsemReadDependencies
	const std::vector<ComponentFlag>& GetReadDependencies() const { return(mReadDependencies); }
	const std::vector<ComponentFlag>& GetReadWriteDependencies() const{ return(mWriteDependencies); }

	// This is used to specify an (optional) 
	std::optional<SystemFlag> GetPrelink() { return(mPreLink); }

	static SystemFlag SystemGroup()
	{
		static const int group = SystemStatic::sSystemGroup++;
		return(group);
	}

protected:
	static void AddReadDependency(ComponentFlag f)
	{
		mReadDependencies.push_back(f);
	}

	static void AddWriteDependency(ComponentFlag f)
	{
		mWriteDependencies.push_back(f);
	}

	static void SetPrelink(SystemFlag f)
	{
		mPrelink = f;
	}

	// [To be disabled outside debug]
	// Checks whether you actually checked out a component before taking it
	template <typename T>
	Ptr<const T> GetComponent(Ptr<EntityAdmin> admin, const Entity& e) const
	{
		const std::vector<ComponentFlag>& readDeps = GetReadDependencies();
		const ComponentFlag accessingComponent = ComponentGroup<T>();
		assert(std::find(readDeps.begin(), readDeps.end(), accessingComponent) != readDeps.end());

		return(admin->GetComponent<T>(e));
	}

	template <typename T>
	Ptr<T> GetWriteComponent(Ptr<EntityAdmin> admin, const Entity& e) const
	{
		const std::vector<ComponentFlag>& writeDeps = GetReadWriteDependencies();
		const ComponentFlag accessingComponent = ComponentGroup<T>();
		assert(
			std::find(
				writeDeps.begin(),
				writeDeps.end(),
				accessingComponent
			) != writeDeps.end()
		);

		return(admin->GetComponent<T>(e));
	}

	template <typename T>
	Ptr<const T> GetSingletonComponent(Ptr<EntityAdmin> admin) const
	{
		const std::vector<ComponentFlag>& readDeps = GetReadDependencies();
		const ComponentFlag accessingComponent = ComponentGroup<T>();
		assert(std::find(readDeps.begin(), readDeps.end(), accessingComponent) != readDeps.end());

		return(admin->GetSingletonComponent<T>());
	}

	template <typename T>
	Ptr<T> GetSingletonWriteComponent(Ptr<EntityAdmin> admin) const
	{
		const std::vector<ComponentFlag>& writeDeps = GetReadWriteDependencies();
		const ComponentFlag accessingComponent = ComponentGroup<T>();
		assert(
			std::find(
				GetReadWriteDependencies().begin(),
				GetReadWriteDependencies().end(),
				accessingComponent
			) != writeDeps.end()
		);

		return(admin->GetSingletonComponent<T>());
	}
	void GetContext()
	{
		return(mContext);
	}

private:
	static std::vector<ComponentFlag> mReadDependencies;
	static std::vector<ComponentFlag> mWriteDependencies;
	static std::optional<SystemFlag> mPreLink;

	static bool mDependenciesInitialized;
};

// Static defs
template <typename Derived>
std::vector<ComponentFlag> System<typename Derived>::mReadDependencies;

template <typename Derived>
std::vector<ComponentFlag> System<typename Derived>::mWriteDependencies;

template <typename Derived>
std::optional<SystemFlag> System<typename Derived>::mPreLink;

template <typename Derived>
bool System<typename Derived>::mDependenciesInitialized;

// Get static group for system
template <typename T>
static SystemFlag SystemGroup() {
	return System<typename std::remove_const<T>::type>::SystemGroup();
}