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
 *
 * 내부에 아레나(Continuum)를 소유하고,
 * Tag별 SoATable을 생성(create) / 조회(get) 한다.
 *
 * 사용자는 버퍼 할당이나 아레나를 직접 다룰 필요 없이
 * Tag 타입만으로 테이블에 접근할 수 있다.
 *
 * 예시:
 *   ArenaRegistry registry(1024 * 1024);
 *   auto& table = registry.create<IMoveable_Tag>(1024);
 *   auto& same  = registry.get<IMoveable_Tag>();
 */
class ArenaRegistry
{
public:
	/** buffersize 만큼 힙 버퍼를 할당하고 아레나를 초기화한다 */
	ArenaRegistry(size_t buffersize) :
		buffer(new uint8_t[buffersize]),
		arena(buffer, buffersize)
	{
	}

	/** 힙 버퍼 해제 */
	~ArenaRegistry()
	{
		delete[] buffer;
	}

	/**
	 * Tag에 대응하는 SoATable을 아레나 위에 생성하고 등록한다.
	 *
	 * 테이블 메타데이터(포인터, count, capacity)는 힙에,
	 * 실제 컬럼 데이터 배열은 아레나에 연속 할당된다.
	 */
	template <typename Tag>
	SoATableFor<Tag>& create(size_t capacity)
	{
		auto* ptr = new SoATableFor<Tag>(arena, capacity);
		tables[typeid(Tag)] = ptr;
		return *ptr;
	}

	/**
	 * 이미 create로 등록된 테이블을 Tag로 조회한다.
	 *
	 * 내부적으로 void* → SoATableFor<Tag>* 캐스팅을 수행한다.
	 */
	template <typename Tag>
	SoATableFor<Tag>& get()
	{
		auto* ptr = static_cast<SoATableFor<Tag>*>(tables[typeid(Tag)]);
		return *ptr;
	}

private:
	/** 아레나 백킹 메모리 — 소멸자에서 해제 */
	uint8_t* buffer;

	/** 컬럼 데이터가 연속 할당되는 아레나 */
	lmt::Continuum arena;

	/** Tag의 typeid → 테이블 void* 매핑 */
	std::unordered_map<std::type_index, void*> tables;
};

} // namespace lmt::engine
