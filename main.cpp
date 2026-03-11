#include <cassert>
#include <cstdio>
#include <limit_engine/core/interface_meta.h>
#include <tuple>
struct Vec3
{
	float x, y, z;
};

struct IMoveable_Tag
{
};

template <>
struct lmt::engine::InterfaceMeta<IMoveable_Tag>
{
	using ColumnTypes = std::tuple<Vec3, Vec3>;
};
struct IMoveable_Table : lmt::engine::SoATableFor<IMoveable_Tag>
{
	using lmt::engine::SoATableFor<IMoveable_Tag>::SoATableFor;

	auto* position()
	{
		return column<0>();
	}

	auto* velocity()
	{
		return column<1>();
	}
};

int main()
{
	constexpr size_t bufferSize = 1024 * 1024;
	uint8_t* buffer = new uint8_t[bufferSize];
	lmt::Continuum arena(buffer, bufferSize);

	IMoveable_Table table(arena, 1024);

	table.push(Vec3{ 1.0f, 2.0f, 3.0f }, Vec3{ 0.1f, 0.2f, 0.3f });
	table.push(Vec3{ 2.0f, 3.0f, 4.0f }, Vec3{ 0.2f, 0.3f, 0.4f });

	table.remove(0);

	printf("positions[0] = { %.1f, %.1f, %.1f }\n", table.position()[0].x, table.position()[0].y, table.position()[0].z);
	printf("velocities[0] = { %.1f, %.1f, %.1f }\n", table.velocity()[0].x, table.velocity()[0].y, table.velocity()[0].z);
	printf("count = %zu\n", table.count);

	/** forEach 순회 — pos += vel */
	table.push(Vec3{ 1.0f, 2.0f, 3.0f }, Vec3{ 0.1f, 0.2f, 0.3f });
	table.push(Vec3{ 10.0f, 20.0f, 30.0f }, Vec3{ 1.0f, 2.0f, 3.0f });

	table.forEach([](Vec3& pos, Vec3& vel) {
		pos.x += vel.x;
		pos.y += vel.y;
		pos.z += vel.z;
	});

	printf("\n--- after forEach ---\n");
	for (size_t i = 0; i < table.count; i++)
	{
		printf("positions[%zu] = { %.1f, %.1f, %.1f }\n", i,
			table.position()[i].x, table.position()[i].y, table.position()[i].z);
	}

	delete[] buffer;
	return 0;
}
