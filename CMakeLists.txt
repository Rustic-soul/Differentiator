cmake_minimum_required(VERSION 3.20)

project(Differentiator)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SRC_FILES src/main.cpp src/calculate.cpp src/differentiator.cpp src/print.cpp)
set(INCLUDE_DIR include/)

set(FLAGS -std=c++17)

set(RELEASE_FLAGS -O3)

set(DEBUG_FLAGS 
      -DDEBUG 
      -O0 
      -g
      -ggdb3 
      -Wall 
      -Wextra 
      -Weffc++ 
      -Waggressive-loop-optimizations 
      -Wc++14-compat 
      -Wmissing-declarations 
      -Wcast-align 
      -Wcast-qual 
      -Wchar-subscripts 
      -Wconditionally-supported 
      -Wconversion 
      -Wctor-dtor-privacy 
      -Wempty-body -Wfloat-equal 
      -Wformat-nonliteral 
      -Wformat-security 
      -Wformat-signedness 
      -Wformat=2 
      -Winline 
      -Wlogical-op 
      -Wnon-virtual-dtor 
      -Wopenmp-simd 
      -Woverloaded-virtual 
      -Wpacked 
      -Wpointer-arith 
      -Winit-self 
      -Wredundant-decls 
      -Wshadow 
      -Wsign-conversion 
      -Wsign-promo 
      -Wstrict-null-sentinel 
      -Wstrict-overflow=2 
      -Wsuggest-attribute=noreturn 
      -Wsuggest-final-methods 
      -Wsuggest-final-types 
      -Wsuggest-override 
      -Wswitch-enum 
      -Wsync-nand 
      -Wundef 
      -Wunreachable-code 
      -Wunused 
      -Wuseless-cast 
      -Wvariadic-macros 
      -Wno-literal-suffix 
      -Wno-missing-field-initializers 
      -Wno-narrowing 
      -Wno-old-style-cast 
      -Wno-varargs 
      -Wstack-protector 
      -fcheck-new 
      -fsized-deallocation 
      -fstack-protector 
      -fstrict-overflow 
      -fno-omit-frame-pointer 
      -Wlarger-than=8192 
      -Wstack-usage=8192 
      -pie 
      -fPIE 
      -Werror=vla 
      -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
)

option(debug OFF)
option(release ON)

if (debug)
  add_compile_options(${FLAGS} ${DEBUG_FLAGS})
  add_link_options   (${FLAGS} ${DEBUG_FLAGS})
elseif(release)
  add_compile_options(${FLAGS} ${RELEASE_FLAGS})
  add_link_options   (${FLAGS} ${RELEASE_FLAGS})
endif()

add_executable(differentiator ${SRC_FILES})
target_include_directories(differentiator PRIVATE ${INCLUDE_DIR})
