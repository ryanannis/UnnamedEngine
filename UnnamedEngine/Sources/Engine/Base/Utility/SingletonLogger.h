#pragma once
#include "Engine/Base/Common/Common.h"
#include <iostream>
#include <unordered_map>

#define GFX_DRIVER_LOG "Graphics"

enum class LogType
{
	GENERAL,
	ERROR
};

class Logger
{
public:
	void Log(std::string stringToLog);
};

class SingletonLogger : public NonCopyable
{
public:
	SingletonLogger() = default;
	static Logger& Log(std::string channel, LogType type);
	static void RegisterStream(std::string channel);

private:
	Logger& GetStreamSingleton(std::string channel, LogType type);
	void RegisterStreamSingleton(std::string channel);

	std::unordered_map<std::string, Logger> mStreams;
	static SingletonLogger& GetSingleton();
};


