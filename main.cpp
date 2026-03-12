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

	table.forEach(
		[](Vec3& pos, Vec3& vel)
		{
			pos.x += vel.x;
			pos.y += vel.y;
			pos.z += vel.z;
		});

	printf("\n--- after forEach ---\n");
	for (size_t i = 0; i < table.count; i++)
	{
		printf("positions[%zu] = { %.1f, %.1f, %.1f }\n", i, table.position()[i].x, table.position()[i].y, table.position()[i].z);
	}

	/** ===== ArenaRegistry — 테이블 중앙 관리 ===== */
	printf("\n--- registry ---\n");

	lmt::engine::ArenaRegistry registry(1024 * 1024);

	auto& moveable = registry.create<IMoveable_Tag>(1024);
	auto& combat = registry.create<ICombat_Tag>(1024);

	moveable.push(Vec3{ 1.0f, 2.0f, 3.0f }, Vec3{ 0.1f, 0.2f, 0.3f });
	combat.push(100.0f, 50.0f, 25.0f);
	combat.push(200.0f, 80.0f, 40.0f);

	combat.forEach([](float& hp, float& armor, float& damage) { hp -= damage; });

	/** get으로 기존 테이블 조회 */
	auto& combat2 = registry.get<ICombat_Tag>();
	printf("same table? %s\n", &combat == &combat2 ? "yes" : "no");

	printf("moveable count = %zu\n", moveable.count);
	for (size_t i = 0; i < combat.count; i++)
	{
		printf("unit[%zu] hp=%.0f\n", i, combat.column<0>()[i]);
	}

	return 0;
}
