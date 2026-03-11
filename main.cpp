#include <limit_engine/arena/soa_storage.h>
#include <cassert>
#include <cstdio>

struct Vec3
{
	float x, y, z;
};

int main()
{
	constexpr size_t bufferSize = 1024 * 1024;
	uint8_t *buffer = new uint8_t[bufferSize];
	lmt::Continuum arena(buffer, bufferSize);

	lmt::engine::SoATable<Vec3, Vec3> table(arena, 1024);

	table.push(Vec3{ 1.0f, 2.0f, 3.0f }, Vec3{ 0.1f, 0.2f, 0.3f });
	table.push(Vec3{ 2.0f, 3.0f, 4.0f }, Vec3{ 0.2f, 0.3f, 0.4f });

	table.remove(0);

	printf("positions[0] = { %.1f, %.1f, %.1f }\n", table.column<0>()[0].x, table.column<0>()[0].y, table.column<0>()[0].z);
	printf("velocities[0] = { %.1f, %.1f, %.1f }\n", table.column<1>()[0].x, table.column<1>()[0].y, table.column<1>()[0].z);
	printf("count = %zu\n", table.count);

	delete[] buffer;
	return 0;
}
