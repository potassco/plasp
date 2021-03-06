#ifndef __PLASP_APP__UTILS_H
#define __PLASP_APP__UTILS_H

#include <cxxopts.hpp>

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Command
//
////////////////////////////////////////////////////////////////////////////////////////////////////

template <std::size_t... Index>
auto makeIndexDispatcher(std::index_sequence<Index...>)
{
	return
		[](auto &&f)
		{
			(f(std::integral_constant<std::size_t, Index>{}), ...);
		};
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template <std::size_t N>
auto makeIndexDispatcher()
{
	return makeIndexDispatcher(std::make_index_sequence<N>{});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename Tuple, typename Functor>
void forEach(Tuple &&tuple, Functor &&functor)
{
	constexpr auto n = std::tuple_size<std::decay_t<Tuple>>::value;
	auto dispatcher = makeIndexDispatcher<n>();
	dispatcher(
		[&functor, &tuple](auto index)
		{
			functor(std::get<index>(std::forward<Tuple>(tuple)));
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
