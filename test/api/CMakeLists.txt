
function(add_api_test TEST_NAME TEST_SRC TEST_DIR)
    set(NAME test_api_${TEST_NAME})
    add_executable(${NAME} EXCLUDE_FROM_ALL ${TEST_SRC})
    rocm_clang_tidy_check(${NAME})
    target_link_libraries(${NAME} migraphx_c)
    target_include_directories(${NAME} PUBLIC ../include)
    add_test(NAME ${NAME} COMMAND $<TARGET_FILE:${NAME}> WORKING_DIRECTORY ${TEST_DIR}) 
    add_dependencies(tests ${NAME})
    add_dependencies(check ${NAME})
endfunction()


add_api_test(ref test_cpu.cpp ${TEST_ONNX_DIR})
add_api_test(save_load test_save_load.cpp ${TEST_ONNX_DIR})
add_api_test(op test_op_construct.cpp ${TEST_ONNX_DIR})
add_api_test(tf_parser test_tf_parser.cpp ${TEST_TF_DIR})
if(MIGRAPHX_ENABLE_GPU)
add_api_test(gpu test_gpu.cpp ${TEST_ONNX_DIR})
# GPU-based tests
endif()
