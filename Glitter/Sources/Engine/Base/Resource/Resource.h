#pragma once

class Resource
{
public:
	bool IsReady() const = 0;
	void Load() const = 0;

private:
	bool mReady;
};