#pragma once
#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Types/System.h"
#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Managers/EntityAdmin.h"

class SystemAdmin : public NonCopyable
{
public:
	explicit SystemAdmin();

	void Update(float dt, Ptr<EntityAdmin> entityAdmin);

	template <typename T, typename ... Args>
	std::shared_ptr<T> AddSystem(Args&& ... args)
	{
		auto system = std::shared_ptr<T>(new T(std::forward<Args>(args)...));
		mSystems.emplace_back(system);
		return(system);
	}

	inline const std::vector<std::shared_ptr<SystemBase>>& GetSystems() const
	{
		return(mSystems);
	}

private:
	std::vector<std::shared_ptr<SystemBase>> mSystems;
};
