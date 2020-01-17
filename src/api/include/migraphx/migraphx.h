#ifndef MIGRAPHX_GUARD_C_API_MIGRAPHX_H
#define MIGRAPHX_GUARD_C_API_MIGRAPHX_H

#include <stdlib.h>

/*! Constructs type name from a struct */
#define MIGRAPHX_DECLARE_OBJECT(name) \
    typedef struct                    \
    {                                 \
        void* handle;                 \
    } name;

// Add new types here
// clang-format off
#define MIGRAPHX_SHAPE_VISIT_TYPES(m) \
    m(half_type, half) \
    m(float_type, float) \
    m(double_type, double) \
    m(uint8_type, uint8_t) \
    m(int8_type, int8_t) \
    m(uint16_type, uint16_t) \
    m(int16_type, int16_t) \
    m(int32_type, int32_t) \
    m(int64_type, int64_t) \
    m(uint32_type, uint32_t) \
    m(uint64_type, uint64_t)
// clang-format on

#ifdef __cplusplus
extern "C" {
#endif

// return code, more to be added later
typedef enum {
    migraphx_status_success        = 0,
    migraphx_status_bad_param      = 1,
    migraphx_status_unknown_target = 3,
    migraphx_status_unknown_error  = 4,

} migraphx_status;

#define MIGRAPHX_SHAPE_GENERATE_ENUM_TYPES(x, t) migraphx_shape_##x,
typedef enum {
    MIGRAPHX_SHAPE_VISIT_TYPES(MIGRAPHX_SHAPE_GENERATE_ENUM_TYPES)
} migraphx_shape_datatype_t;
#undef MIGRAPHX_SHAPE_GENERATE_ENUM_TYPES

typedef struct
{
    bool offload_copy;
} migraphx_compile_options;

typedef struct migraphx_shape* migraphx_shape_t;
typedef const struct migraphx_shape* const_migraphx_shape_t;

typedef struct migraphx_argument* migraphx_argument_t;
typedef const struct migraphx_argument* const_migraphx_argument_t;

typedef struct migraphx_target* migraphx_target_t;
typedef const struct migraphx_target* const_migraphx_target_t;

typedef struct migraphx_program_parameter_shapes* migraphx_program_parameter_shapes_t;
typedef const struct migraphx_program_parameter_shapes* const_migraphx_program_parameter_shapes_t;

typedef struct migraphx_program_parameters* migraphx_program_parameters_t;
typedef const struct migraphx_program_parameters* const_migraphx_program_parameters_t;

typedef struct migraphx_program* migraphx_program_t;
typedef const struct migraphx_program* const_migraphx_program_t;

migraphx_status migraphx_shape_destroy(migraphx_shape_t shape);

migraphx_status migraphx_shape_create(migraphx_shape_t* shape,
                                      migraphx_shape_datatype_t type,
                                      size_t* lengths,
                                      size_t lengths_size);

migraphx_status
migraphx_shape_lengths(const size_t** out, size_t* out_size, migraphx_shape_t shape);

migraphx_status
migraphx_shape_strides(const size_t** out, size_t* out_size, migraphx_shape_t shape);

migraphx_status migraphx_shape_type(migraphx_shape_datatype_t* out, migraphx_shape_t shape);

migraphx_status migraphx_argument_destroy(migraphx_argument_t argument);

migraphx_status
migraphx_argument_create(migraphx_argument_t* argument, const_migraphx_shape_t shape, void* buffer);

migraphx_status migraphx_argument_shape(const_migraphx_shape_t* out, migraphx_argument_t argument);

migraphx_status migraphx_argument_buffer(char** out, migraphx_argument_t argument);

migraphx_status migraphx_target_destroy(migraphx_target_t target);

migraphx_status migraphx_target_create(migraphx_target_t* target, const char* name);

migraphx_status migraphx_program_parameter_shapes_destroy(
    migraphx_program_parameter_shapes_t program_parameter_shapes);

migraphx_status migraphx_program_parameter_shapes_size(
    size_t* out, migraphx_program_parameter_shapes_t program_parameter_shapes);

migraphx_status
migraphx_program_parameter_shapes_get(const_migraphx_shape_t* out,
                                      migraphx_program_parameter_shapes_t program_parameter_shapes,
                                      const char* name);

migraphx_status migraphx_program_parameter_shapes_names(
    const char** out, migraphx_program_parameter_shapes_t program_parameter_shapes);

migraphx_status
migraphx_program_parameters_destroy(migraphx_program_parameters_t program_parameters);

migraphx_status
migraphx_program_parameters_create(migraphx_program_parameters_t* program_parameters);

migraphx_status migraphx_program_parameters_add(migraphx_program_parameters_t program_parameters,
                                                const char* name,
                                                migraphx_argument_t argument);

migraphx_status migraphx_program_destroy(migraphx_program_t program);

migraphx_status migraphx_program_compile(migraphx_program_t program,
                                         migraphx_target_t target,
                                         migraphx_compile_options* options);

migraphx_status migraphx_program_get_parameter_shapes(migraphx_program_t program);

migraphx_status migraphx_program_run(migraphx_argument_t* out,
                                     migraphx_program_t program,
                                     migraphx_program_parameters_t params);

#ifdef __cplusplus
}
#endif

#endif
