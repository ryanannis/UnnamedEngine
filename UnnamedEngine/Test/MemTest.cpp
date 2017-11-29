#include <Catch.h>
#include "Engine/Base/Memory/ArrayPool.h"
#include <iostream>

TEST_CASE("Array Pool Memory Works", "[Mem]")
{
	ArrayPool a(sizeof(int));
	int* b = static_cast<int*>(a.AllocComponent(2));
	*b = 1;
	int* c = static_cast<int*>(a.AllocComponent(3));
	*c = 2;

	int* d = static_cast<int*>(a.AllocComponent(4));
	*d = 3;

	int* e = static_cast<int*>(a.AllocComponent(5));
	*e = 4;

	int* f = static_cast<int*>(a.AllocComponent(6));
	*f = 5;

	int* g = static_cast<int*>(a.AllocComponent(7));
	*g = 6;

	int* h = static_cast<int*>(a.AllocComponent(8));
	*h = 7;


	REQUIRE(*b == 1);
	REQUIRE(*c == 2);
	REQUIRE(*d == 3);
	REQUIRE(*e == 4);
	REQUIRE(*f == 5);
	REQUIRE(*g == 6);
	REQUIRE(*h == 7);

	a.OnBetweenFrames();

	REQUIRE(*b == 1);
	REQUIRE(*c == 2);
	REQUIRE(*d == 3);
	REQUIRE(*e == 4);
	REQUIRE(*f == 5);
	REQUIRE(*g == 6);
	REQUIRE(*h == 7);
}
