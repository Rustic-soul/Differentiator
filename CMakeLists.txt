cmake_minimum_required(VERSION 3.20)

project(Differentiator)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC_FILES src/main.c src/calculate.c src/differentiator.c src/print.c)
set(INCLUDE_DIR include/)

set(FLAGS -std=c99 -lm)

set(RELEASE_FLAGS -O3)

set(DEBUG_FLAGS 
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

option(debug OFF)
option(release ON)

add_compile_options(${FLAGS})
add_link_options   (${FLAGS})
if (debug)
    add_compile_options(${DEBUG_FLAGS})
  add_link_options     (${DEBUG_FLAGS})
elseif(release)
  add_compile_options(${RELEASE_FLAGS})
  add_link_options   (${RELEASE_FLAGS})
endif()

add_executable(differentiator ${SRC_FILES})
target_include_directories(differentiator PRIVATE ${INCLUDE_DIR})
