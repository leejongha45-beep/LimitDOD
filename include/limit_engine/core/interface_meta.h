#pragma once

#include <limit_engine/arena/soa_storage.h>
#include <tuple>

namespace lmt::engine
{

/**
 * Tag 타입을 특수화하여 해당 인터페이스의 컬럼 타입들을 정의한다.
 *
 * 사용자는 빈 Tag 구조체를 만들고 이 템플릿을 특수화하여
 * ColumnTypes(std::tuple)로 컬럼 구성을 선언한다.
 *
 * 예시:
 *   struct IMoveable_Tag {};
 *   template<> struct InterfaceMeta<IMoveable_Tag> {
 *       using ColumnTypes = std::tuple<Vec3, Vec3>;
 *   };
 */
template <typename Tag>
struct InterfaceMeta;

/**
 * std::tuple<Columns...>을 SoATable<Columns...>로 변환하는 메타함수.
 *
 * InterfaceMeta::ColumnTypes(tuple) → SoATable 타입으로 브릿지한다.
 */
template <typename Tuple>
struct TupleToSoATable;
template <typename... Columns>
struct TupleToSoATable<std::tuple<Columns...>>
{
	using Type = SoATable<Columns...>;
};

/**
 * Tag로부터 대응하는 SoATable 타입을 바로 얻는 별칭.
 *
 * InterfaceMeta<Tag>::ColumnTypes → TupleToSoATable → SoATable<...>
 *
 * 사용:
 *   using MyTable = SoATableFor<IMoveable_Tag>;
 */
template <typename Tag>
using SoATableFor = typename TupleToSoATable<typename InterfaceMeta<Tag>::ColumnTypes>::Type;

} // namespace lmt::engine
