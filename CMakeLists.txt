cmake_minimum_required(VERSION 3.16.0)
project(cxxlearn VERSION 0.1.0)

include(CTest)
#enable_testing()


add_executable(cxxlearn co5.cpp sched.cpp)
target_precompile_headers(cxxlearn PUBLIC pch.h)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest  /DUNICODE /D_UNICODE")
include(CPack)
