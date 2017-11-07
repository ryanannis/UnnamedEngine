#include "Engine/Base/Client/Client.h"
#include <memory>
#include "Engine/Base/Client/GameFramework.h"

int main(int, char* [])
{
	Client c(std::make_unique<GameFramework>(nullptr));
	c.Initialize();
	c.Run();
}