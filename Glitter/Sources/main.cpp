#include "glitter.h"

#include "Engine/Base/Client/Client.h"
#include <memory>
#include "Engine/Base/Client/GameFramework.h"

int main(int argc, char* argv[])
{
	Client c(std::make_unique<GameFramework>());
	c.Initialize();
	c.Run();
}
