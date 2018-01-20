#pragma once
#include "Engine/Base/Common/Common.h"
#include <iostream>
#include <unordered_map>

#define GFX_DRIVER_LOG "Graphics"
#define VULKAN_VALIDATTION_LOG "VulkanValidation"

enum class LogType
{
	GENERAL,
	ERROR
};

class Logger
{
public:
	Logger(std::string channel);
	void Log(std::string stringToLog);

private:
	std::string mChannelName;
};

class SingletonLogger : public NonCopyable
{
public:
	SingletonLogger() = default;
	static Logger& Log(std::string channel, LogType type);
	static void RegisterStream(std::string channel); // todo: is requiring stream registration actually a good idea?

private:
	Logger& GetStreamSingleton(std::string channel, LogType type);
	void RegisterStreamSingleton(std::string channel);

	std::unordered_map<std::string, Logger> mStreams;
	static SingletonLogger& GetSingleton();
};


