#pragma once

#include <cstddef>
#include <limit/limit.h>
#include <tuple>

namespace lmt::engine
{

/**
 * SoA 컬럼 스토어 테이블.
 * 각 컬럼 타입별로 별도 배열을 Continuum 아레나에 연속 할당한다.
 * 포인터 체이싱 없이 배열 인덱스만으로 접근/순회 가능.
 */
template <typename... Columns>
struct SoATable
{
	std::tuple<Columns *...> data;
	size_t capacity = 0;
	size_t count = 0;

	/** 아레나에서 컬럼당 cap개만큼 메모리를 연속 할당 */
	SoATable(lmt::Continuum &arena, size_t cap) :
		data{ arena.alloc<Columns>(cap)... },
		capacity(cap)
	{
	}

	template <size_t... I>
	void push_impl(size_t index, std::index_sequence<I...>, Columns... args)
	{
		((std::get<I>(data)[index] = args), ...);
	}

	/** 끝에 한 행 추가. 삽입된 인덱스를 반환 */
	size_t push(Columns... args)
	{
		size_t index = count;
		count++;
		push_impl(index, std::index_sequence_for<Columns...>{}, args...);
		return index;
	}

	template <size_t... I>
	void remove_impl(size_t index, size_t last, std::index_sequence<I...>)
	{
		((std::get<I>(data)[index] = std::get<I>(data)[last]), ...);
	}

	/** 마지막 행을 해당 위치로 덮어써서 삭제 (swap-back) */
	void remove(size_t index)
	{
		count--;
		size_t last = count;
		remove_impl(index, last, std::index_sequence_for<Columns...>{});
	}

	/** I번째 컬럼의 원시 배열 포인터 반환 */
	template <size_t I>
	auto *column()
	{
		return (std::get<I>(data));
	}
};

} // namespace lmt::engine
