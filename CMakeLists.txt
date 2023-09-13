cmake_minimum_required(VERSION 3.9)
project(DemoSSIM-PSNR VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_GENERATOR_PLATFORM x64)
set(CMAKE_BUILD_TYPE "Release")

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /we4013 /we4431 /we4133 /we4716 /we6244 /we6246 /we4457 /we4456 /we4172 /we4700 /we4477 /we4018 /we4047")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /we4013 /we4431 /we4133 /we4716 /we6244 /we6246 /we4457 /we4456 /we4172 /we4700 /we4477 /we4018 /we4047")
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux" OR CMAKE_SYSTEM_NAME MATCHES "Darwin")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=implicit-function-declaration -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=return-type -Werror=shadow -Werror=return-local-addr -Werror=uninitialized -Werror=format -Werror=sign-compare -Werror=int-conversion")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=implicit-function-declaration -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=return-type -Werror=shadow -Werror=return-local-addr -Werror=uninitialized -Werror=format -Werror=sign-compare -Werror=int-conversion")
endif()

find_package(OpenMP REQUIRED)
if(OpenMP_FOUND)
    message(STATUS "found openmp") 
    set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS}  ${OPENMP_C_FLAGS})
    set(CMAKE_CXX_FLAGS "-std=c++11" ${CMAKE_CXX_FLAGS} ${OPENMP_CXX_FLAGS})
    add_definitions(-D_OPENMP)
else()
    message(FATAL_ERROR "openmp not found!")
endif()

set(DEMO_SRC ${CMAKE_SOURCE_DIR}/ssim-psnr.cc
             ${CMAKE_SOURCE_DIR}/ssim-psnr.h
             ${CMAKE_SOURCE_DIR}/stb_image.h
             ${CMAKE_SOURCE_DIR}/timing.h
             ${CMAKE_SOURCE_DIR}/tiny_jpeg.h
             ${CMAKE_SOURCE_DIR}/browse.h
             ${CMAKE_SOURCE_DIR}/main.cc
             )

set(SRC_FILES ${DEMO_SRC})

add_executable(${PROJECT_NAME}   ${SRC_FILES} )
# target_link_libraries(${PROJECT_NAME} )