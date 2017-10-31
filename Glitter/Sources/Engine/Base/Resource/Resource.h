#pragma once

class Resource
{
public:
	virtual bool IsReady() const = 0;
	virtual void Load() const = 0;

private:
	bool mReady;
};