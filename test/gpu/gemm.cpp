#include <iostream>
#include <vector>
#include <migraphx/operators.hpp>
#include <migraphx/instruction.hpp>
#include <migraphx/generate.hpp>
#include <migraphx/cpu/target.hpp>
#include <migraphx/gpu/target.hpp>
#include <migraphx/verify.hpp>
#include <migraphx/dead_code_elimination.hpp>
#include <migraphx/propagate_constant.hpp>
#include <migraphx/pass_manager.hpp>
#include <migraphx/onnx.hpp>
#include "test.hpp"

TEST_CASE(gemm_copy)
{
    auto run_prog = [](migraphx::program p,
                       const migraphx::target& t,
                       migraphx::program::parameter_map& m_in,
                       std::vector<float>& res) {
        p.compile(t);
        migraphx::program::parameter_map m;
        for(auto&& x : p.get_parameter_shapes())
        {
            if(m_in.count(x.first) > 0)
            {
                m[x.first] = t.copy_to(m_in[x.first]);
            }
            else
            {
                m[x.first] = t.copy_to(migraphx::generate_argument(x.second));
            }
        }

        t.copy_from(p.eval(m));
        auto result = t.copy_from(p.eval(m));
        result.visit([&](auto v) { res.assign(v.begin(), v.end()); });
    };

    auto create_program = [] {
        migraphx::program p;
        migraphx::shape sa{migraphx::shape::float_type, {2, 16}};
        migraphx::shape sb{migraphx::shape::float_type, {16, 8}};
        migraphx::shape sc{migraphx::shape::float_type, {2, 8}};
        auto pa = p.add_parameter("a", sa);
        auto pb = p.add_parameter("b", sb);
        auto pc = p.add_parameter("c", sc);
        auto dr = p.add_instruction(migraphx::op::dot{}, pa, pb, pc);
        p.add_instruction(migraphx::op::add{}, dr, dr);

        return p;
    };

    {
        auto p = create_program();
        migraphx::program::parameter_map m;
        migraphx::shape sa{migraphx::shape::float_type, {2, 16}};
        migraphx::shape sb{migraphx::shape::float_type, {16, 8}};
        migraphx::shape sc{migraphx::shape::float_type, {2, 8}};
        m["a"] = migraphx::generate_argument(sa);
        m["b"] = migraphx::generate_argument(sb);
        m["c"] = migraphx::generate_argument(sc);
        std::vector<float> cpu_result;
        migraphx::target cpu_t = migraphx::cpu::target{};
        run_prog(p, cpu_t, m, cpu_result);

        std::vector<float> gpu_result;
        migraphx::target gpu_t = migraphx::gpu::target{};
        run_prog(p, gpu_t, m, gpu_result);

        EXPECT(migraphx::verify_range(cpu_result, gpu_result));
    }
}

int main(int argc, const char* argv[]) { test::run(argc, argv); }