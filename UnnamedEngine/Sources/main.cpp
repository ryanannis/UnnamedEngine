#include "Engine/Base/Client/Client.h"
#include <memory>
#include "Engine/Base/Client/GameFramework.h"

#include "Engine/Base/Types/StaticRegistry.h"
static int dummy = StaticRegister();

int main(int, char* [])
{
	Client c(std::make_unique<GameFramework>(nullptr));
	c.Initialize();
	c.Run();
}