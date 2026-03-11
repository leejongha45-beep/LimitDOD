#pragma once

#include <limit_engine/arena/soa_storage.h>
#include <tuple>

namespace lmt::engine
{

template <typename Tag>
struct InterfaceMeta;

template <typename Tuple>
struct TupleToSoATable;
template <typename... Columns>
struct TupleToSoATable<std::tuple<Columns...>>
{
	using Type = SoATable<Columns...>;
};

template <typename Tag>
using SoATableFor = typename TupleToSoATable<typename InterfaceMeta<Tag>::ColumnTypes>::Type;

} // namespace lmt::engine
