#include "SingletonLogger.h"


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
	//mStreams.emplace(channel, std::cout);
}

Logger& SingletonLogger::Log(std::string channel, LogType type)
{
	return(GetSingleton().GetStreamSingleton(channel, type));
}

void SingletonLogger::RegisterStream(std::string channel)
{
	GetSingleton().RegisterStreamSingleton(channel);
}
