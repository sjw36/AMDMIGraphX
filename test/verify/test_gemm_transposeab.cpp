
#include "verify_program.hpp"
#include <migraphx/program.hpp>
#include <migraphx/generate.hpp>
#include <migraphx/make_op.hpp>


struct test_gemm_transposeab : verify_program<test_gemm_transposeab>
{
    migraphx::program create_program() const
    {
        migraphx::program p;
        auto* mm = p.get_main_module();
        auto a   = mm->add_parameter("a", migraphx::shape{migraphx::shape::float_type, {5, 4}});
        auto b   = mm->add_parameter("b", migraphx::shape{migraphx::shape::float_type, {3, 5}});
        auto at  = mm->add_instruction(migraphx::make_op("transpose", {{"dims", {1, 0}}}), a);
        auto bt  = mm->add_instruction(migraphx::make_op("transpose", {{"dims", {1, 0}}}), b);
        mm->add_instruction(migraphx::make_op("dot"), at, bt);
        return p;
    }
};
