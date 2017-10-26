#pragma once
#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Types/System.h"

class SystemAdmin : public NonCopyable
{
public:
	SystemAdmin();
	~SystemAdmin();

	template <typename T, typename ... Args>
	std::shared_ptr<T> AddSystem(Args&& ... args)
	{
		auto system = std::shared_ptr<T>(new T(std::forward(args)...));
		mSystems.emplace_back(system);
		return(system);
	}

private:
	std::vector<std::shared_ptr<System>> mSystems;

};