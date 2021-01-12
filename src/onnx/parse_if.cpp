#include <migraphx/onnx/op_parser.hpp>
#include <migraphx/onnx/onnx_parser.hpp>
#include <migraphx/onnx/checks.hpp>
#include <migraphx/ranges.hpp>
#include <migraphx/instruction.hpp>
#include <migraphx/make_op.hpp>

namespace migraphx {
inline namespace MIGRAPHX_INLINE_NS {
namespace onnx {

struct parse_if : op_parser<parse_if>
{
    std::vector<op_desc> operators() const { return {{"If"}}; }

    std::vector<instruction_ref> parse(const op_desc& /*opd*/,
                                       onnx_parser& parser,
                                       const onnx_parser::node_info& info,
                                       std::vector<instruction_ref> args) const
    {
        migraphx::argument cond_arg = args.front()->eval();
        check_arg_empty(cond_arg, "PARSE_IF: Only support constant input condition");
        std::vector<bool> vec_conds;
        cond_arg.visit([&](auto s) { vec_conds.assign(s.begin(), s.end()); });
        if(vec_conds.size() != 1)
        {
            MIGRAPHX_THROW("PARSE_IF: condition input can have only one element!");
        }

        bool cond = vec_conds[0];
        // then branch
        if(cond)
        {
            const auto& then_graph = info.attributes.at("then_branch").g();
            parser.parse_graph(info.mdl, then_graph);

            // inputs of the return instruction are that of the output of the
            // if instruction
            instruction_ref ret_ins = std::prev(info.mdl->end());
            auto inputs             = ret_ins->inputs();
            info.mdl->remove_instruction(ret_ins);

            return inputs;
        }
        // else branch
        else
        {
            const auto& else_graph = info.attributes.at("else_branch").g();
            parser.parse_graph(info.mdl, else_graph);

            // inputs of the return instruction are that of the output of the
            // if instruction
            instruction_ref ret_ins = std::prev(info.mdl->end());
            auto inputs             = ret_ins->inputs();
            info.mdl->remove_instruction(ret_ins);

            return inputs;
        }
    }
};

} // namespace onnx
} // namespace MIGRAPHX_INLINE_NS
} // namespace migraphx