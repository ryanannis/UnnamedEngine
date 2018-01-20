#include "SingletonLogger.h"

Logger::Logger(std::string channel) :
	mChannelName(channel)
{}

void Logger::Log(std::string stringToLog)
{
	std::cout << "[" << mChannelName << "] " << stringToLog << std::endl;
}


SingletonLogger& SingletonLogger::GetSingleton()
{
	static SingletonLogger sLoggerInstance;
	return(sLoggerInstance);
}

Logger& SingletonLogger::GetStreamSingleton(std::string channel, LogType type)
{
	auto stream = mStreams.find(channel);
	if(stream == mStreams.end())
	{
		// Tried to access invalid logging stream
		assert(false);
	}

	return(stream->second);
}

void SingletonLogger::RegisterStreamSingleton(std::string channel)
{
	auto stream = mStreams.find(channel);
	if(stream != mStreams.end())
	{
		// Logging stream already exists!
		assert(false);
	}

	mStreams.emplace(channel, Logger(channel));
}

Logger& SingletonLogger::Log(std::string channel, LogType type)
{
	return(GetSingleton().GetStreamSingleton(channel, type));
}

void SingletonLogger::RegisterStream(std::string channel)
{
	GetSingleton().RegisterStreamSingleton(channel);
}
