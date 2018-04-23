#ifndef GUARD_RTGLIB_INSTRUCTION_HPP
#define GUARD_RTGLIB_INSTRUCTION_HPP

#include <rtg/literal.hpp>
#include <rtg/shape.hpp>
#include <rtg/builtin.hpp>
#include <string>

namespace rtg {

struct instruction
{
    instruction() {}

    instruction(operand o, shape r, std::vector<instruction*> args)
        : op(std::move(o)), result(std::move(r)), arguments(std::move(args)), lit()
    {
    }

    instruction(literal l)
        : op(builtin::literal{}), result(l.get_shape()), arguments(), lit(std::move(l))
    {
    }

    operand op;
    shape result;
    std::vector<instruction*> arguments;
    literal lit;
};

} // namespace rtg

#endif
