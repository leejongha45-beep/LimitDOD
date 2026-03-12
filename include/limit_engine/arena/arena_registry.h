#pragma once

#include <cstddef>
#include <cstdint>
#include <limit/limit.h>
#include <limit_engine/core/interface_meta.h>
#include <typeindex>
#include <unordered_map>

namespace lmt::engine
{

/**
 * 테이블 중앙 관리자.
 * 내부에 아레나를 소유하고, Tag별 SoATable을 생성/조회한다.
 */
class ArenaRegistry
{
public:
	ArenaRegistry(size_t buffersize) :
		buffer(new uint8_t[buffersize]),
		arena(buffer, buffersize)
	{
	}

	~ArenaRegistry()
	{
		delete[] buffer;
	}

	template <typename Tag>
	SoATableFor<Tag>& create(size_t capacity)
	{
		auto* ptr = new SoATableFor<Tag>(arena, capacity);
		tables[typeid(Tag)] = ptr;
		return *ptr;
	}

	template <typename Tag>
	SoATableFor<Tag>& get()
	{
		auto* ptr = static_cast<SoATableFor<Tag>*>(tables[typeid(Tag)]);
		return *ptr;
	}

private:
	uint8_t* buffer;
	lmt::Continuum arena;
	std::unordered_map<std::type_index, void*> tables;
};

} // namespace lmt::engine
