#ifndef MIGRAPHX_GUARD_OPERATORS_ATAN_HPP
#define MIGRAPHX_GUARD_OPERATORS_ATAN_HPP

#include <array>
#include <migraphx/op/unary.hpp>
#include <migraphx/check_shapes.hpp>
#include <migraphx/stringutils.hpp>
#include <migraphx/streamutils.hpp>
#include <migraphx/literal.hpp>
#include <migraphx/shape_for_each.hpp>
#include <migraphx/config.hpp>
#include <cmath>
#include <utility>

namespace migraphx {
inline namespace MIGRAPHX_INLINE_NS {
namespace op {

struct atan : unary<atan>
{
    auto apply() const
    {
        return [](auto x) { return std::atan(x); };
    }
};

} // namespace op
} // namespace MIGRAPHX_INLINE_NS
} // namespace migraphx

#endif
