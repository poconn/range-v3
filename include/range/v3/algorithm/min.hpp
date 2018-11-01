/// \file
// Range v3 library
//
//  Copyright Eric Niebler 2014-present
//  Copyright Casey Carter 2015
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//
#ifndef RANGES_V3_ALGORITHM_MIN_HPP
#define RANGES_V3_ALGORITHM_MIN_HPP

#include <range/v3/range_fwd.hpp>
#include <range/v3/begin_end.hpp>
#include <range/v3/range_concepts.hpp>
#include <range/v3/range_traits.hpp>
#include <range/v3/utility/iterator_concepts.hpp>
#include <range/v3/utility/iterator_traits.hpp>
#include <range/v3/utility/iterator.hpp>
#include <range/v3/utility/functional.hpp>
#include <range/v3/utility/static_const.hpp>

namespace ranges
{
    inline namespace v3
    {
        /// \addtogroup group-algorithms
        /// @{
        struct min_fn
        {
            template<typename Rng, typename C = ordered_less, typename P = ident>
            constexpr /*c++14*/ auto operator()(Rng &&rng, C pred = C{}, P proj = P{}) const ->
                CPP_ret(iter_value_t<iterator_t<Rng>>)(
                    requires InputRange<Rng> && Copyable<iter_value_t<iterator_t<Rng>>> &&
                        IndirectRelation<C, projected<iterator_t<Rng>, P>>)
            {
                auto begin = ranges::begin(rng);
                auto end = ranges::end(rng);
                RANGES_EXPECT(begin != end);
                iter_value_t<iterator_t<Rng>> result = *begin;
                while(++begin != end)
                {
                    auto && tmp = *begin;
                    if(invoke(pred, invoke(proj, tmp), invoke(proj, result)))
                        result = (decltype(tmp) &&) tmp;
                }
                return result;
            }

            template<typename T, typename C = ordered_less, typename P = ident>
            constexpr auto operator()(T const &a, T const &b, C pred = C{}, P proj = P{}) const ->
                CPP_ret(T const &)(
                    requires IndirectRelation<C, projected<const T *, P>>)
            {
                return invoke(pred, invoke(proj, b), invoke(proj, a)) ? b : a;
            }
        };

        /// \sa `min_fn`
        /// \ingroup group-algorithms
        RANGES_INLINE_VARIABLE(with_braced_init_args<min_fn>, min)
        /// @}
    } // namespace v3
} // namespace ranges

#endif // include guard
