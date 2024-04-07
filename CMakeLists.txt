cmake_minimum_required(VERSION 3.20)

project(Differentiator)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/)

set(MY_DEBUG_FLAGS
    -DDEBUG
    -ggdb3
    -O0 
    -Wall 
    -Wextra 
    -Waggressive-loop-optimizations
    -Wmissing-declarations 
    -Wcast-align 
    -Wcast-qual 
    -Wno-char-subscripts    
    -Wconversion 
    -Wempty-body 
    -Wfloat-equal 
    -Wformat-nonliteral 
    -Wformat-security 
    -Wformat-signedness 
    -Wformat=2 
    -Winline 
    -Wlogical-op 
    -Wopenmp-simd 
    -Wpacked 
    -Wpointer-arith 
    -Winit-self 
    -Wredundant-decls 
    -Wshadow 
    -Wsign-conversion 
    -Wstrict-overflow=2 
    -Wsuggest-attribute=noreturn 
    -Wsuggest-final-methods 
    -Wsuggest-final-types 
    -Wswitch-default 
    -Wswitch-enum 
    -Wsync-nand 
    -Wundef 
    -Wunreachable-code 
    -Wunused 
    -Wvariadic-macros 
    -Wno-missing-field-initializers 
    -Wno-narrowing 
    -Wno-varargs 
    -Wstack-protector 
    -fcheck-new 
    -fstack-protector 
    -fstrict-overflow 
    -fno-omit-frame-pointer 
    -Wlarger-than=16384 
    -Wstack-usage=8192  
    -fPIE 
    -Werror=vla 
    -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
)


set(SOURCES 
    ./src/main.c 
    ./src/calculate.c 
    ./src/differentiator.c 
    ./src/print.c
    ./src/read.c
)

set(INCLUDE_DIRS 
    ./include/
)

add_executable(differentiator ${SOURCES})

target_compile_features(differentiator PRIVATE c_std_99)

target_include_directories(differentiator PRIVATE ${INCLUDE_DIRS})

target_link_libraries(differentiator PRIVATE m)

if (CMAKE_BUILD_TYPE STREQUAL "MyDebug")
    target_compile_options(differentiator PRIVATE ${MY_DEBUG_FLAGS})
    target_link_options   (differentiator PRIVATE ${MY_DEBUG_FLAGS})
endif()