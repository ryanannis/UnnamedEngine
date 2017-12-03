#pragma once
#include "Engine/Base/Common/Common.h"
#include <iostream>
#include <unordered_map>


enum class LogType
{
	GENERAL,
	ERROR
};

class SingletonLogger : public NonCopyable
{
public:
	SingletonLogger() = default;
	static std::iostream& Log(std::string channel, LogType type);
	static void RegisterStream(std::string channel);

private:
	std::iostream& GetStreamSingleton(std::string channel, LogType type);
	void RegisterStreamSingleton(std::string channel);

	std::unordered_map<std::string, std::iostream> mStreams;
	static SingletonLogger& GetSingleton();
};


