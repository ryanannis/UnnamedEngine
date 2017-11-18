#include <Catch.h>
#include <chrono>
#include <array>
#include <iostream>

#include "Engine/Base/Managers/EntityAdmin.h"
#include "Engine/Base/Components/CameraComponent.h"
#include "Engine/Base/Components/HealthComponent.h"
#include "Engine/Base/Components/IdentityComponent.h"
#include "Engine/Base/Components/RenderComponent.h"
#include "Engine/Base/Components/TransformComponent.h"
#include "Engine/Base/Components/VelocityComponent.h"

//#pragma optimize("",off)
void BasicBenchmark(const size_t ENTITIES, const size_t UPDATES)
{
	std::vector<Entity> entityList(ENTITIES);

	std::cout << "========== BASIC BENCHMARK =============" << std::endl;
	auto before = std::chrono::high_resolution_clock::now();

	EntityAdmin a;
	for(size_t i = 0; i < ENTITIES; i++)
	{
		Entity e = a.CreateEntity();
		entityList[i] = e;
		auto t = a.AddComponent<TransformComponent>(e);
		auto v = a.AddComponent<VelocityComponent>(e);

		// initialize with some bullshit
		t->pEntityWorldRotation = Quat(5, i * i, i * 6, 5);
		t->pEntityWorldTranslation = Vector3f(i * i, i * 6, 5);

		v->pAngularVelocity = Vector3f(i*i*i, 5, 2);
		v->pVelocity = Vector3f(i << 6, i & 0xfff, 2);
	}

	auto init = std::chrono::high_resolution_clock::now();

	for(size_t i = 0; i < UPDATES; i++)
	{
		for(size_t q = 0; q < ENTITIES; q++)
		{
			const Entity& e = entityList[q];
			auto t = a.GetComponent<TransformComponent>(e);
			auto b = a.GetComponent<VelocityComponent>(e);
			t->pEntityWorldTranslation += b->pVelocity;
		}
	}

	auto after = std::chrono::high_resolution_clock::now();
	auto iteration = std::chrono::duration_cast<std::chrono::milliseconds>(after - init).count();
	auto mid = std::chrono::duration_cast<std::chrono::milliseconds>(init - before).count();

	std::cout << ENTITIES << " entities" << UPDATES << " updates" << std::endl;
	std::cout << "Init" << mid << "ms, Iteration: " << iteration << "ms" << std::endl;
}

TEST_CASE("Benchmark ECS Lite - Pos & Velocity", "[Benchmark]")
{
	BasicBenchmark(10000, 5000);
}
