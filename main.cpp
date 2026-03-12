#include <cassert>
#include <cstdio>
#include <limit_engine/arena/arena_registry.h>

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
struct ICombat_Tag
{
};

template <>
struct lmt::engine::InterfaceMeta<ICombat_Tag>
{
	using ColumnTypes = std::tuple<float, float, float>;
};

struct ICombat_Table : lmt::engine::SoATableFor<ICombat_Tag>
{
	using lmt::engine::SoATableFor<ICombat_Tag>::SoATableFor;

	auto* hp()
	{
		return column<0>();
	}

	auto* armor()
	{
		return column<1>();
	}

	auto* damage()
	{
		return column<2>();
	}
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
	lmt::engine::ArenaRegistry registry(1024 * 1024);

	auto& moveable = registry.create<IMoveable_Tag>(1024);
	auto& combat = registry.create<ICombat_Tag>(1024);

	moveable.push(Vec3{ 1.0f, 2.0f, 3.0f }, Vec3{ 0.1f, 0.2f, 0.3f });
	moveable.push(Vec3{ 10.0f, 20.0f, 30.0f }, Vec3{ 1.0f, 2.0f, 3.0f });

	moveable.forEach(
		[](Vec3& pos, Vec3& vel)
		{
			pos.x += vel.x;
			pos.y += vel.y;
			pos.z += vel.z;
		});

	combat.push(100.0f, 50.0f, 25.0f);
	combat.push(200.0f, 80.0f, 40.0f);

	combat.forEach([](float& hp, float& armor, float& damage) { hp -= damage; });

	for (size_t i = 0; i < moveable.count; i++)
	{
		printf("pos[%zu] = { %.1f, %.1f, %.1f }\n", i, moveable.column<0>()[i].x, moveable.column<0>()[i].y, moveable.column<0>()[i].z);
	}
	for (size_t i = 0; i < combat.count; i++)
	{
		printf("unit[%zu] hp=%.0f\n", i, combat.column<0>()[i]);
	}

	return 0;
}
