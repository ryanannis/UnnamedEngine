#include "glitter.h"

#include "Engine/Base/Client/Client.h"
#include <memory>
#include "Engine/Base/Client/GameFramework.h"

int main(int argc, char* argv[])
{
	Client c;
	c.Initialize();
	c.SetTarget(std::make_unique<GameFramework>());
	c.Run();
}
