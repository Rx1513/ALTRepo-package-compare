cmake_minimum_required(VERSION 3.10.0)

set(PATH_TO_LIBRPM /usr/include/rpm/ CACHE PATH "Path to librpm headers")

project(pkgcmp VERSION 1.0 DESCRIPTION "Library for comparing packages provided by ALTRepoAPI")

option(ENABLE_TEST "Build test executable instead of shared library" OFF)
if (NOT ENABLE_TEST)
add_library(${PROJECT_NAME} SHARED
    src/lib.cpp
    src/lib.hpp
    ${PATH_TO_LIBRPM}/rpmvercmp.h
)

target_include_directories(${PROJECT_NAME} PUBLIC src ${PATH_TO_LIBRPM})

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER src/lib.hpp)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "/usr")
endif()

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib64
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include
    COMPONENT library
)
else()
#TESTS

include(GoogleTest)

enable_testing()

add_executable(test_lib
    tests/test.cpp
    ${PATH_TO_LIBRPM}/rpmvercmp.h
)

target_link_libraries(test_lib pthread gtest)

target_include_directories(test_lib PUBLIC src ${PATH_TO_LIBRPM})

target_link_libraries(test_lib pkgcmp rpm rpmbuild popt)
target_compile_options(test_lib PRIVATE -pthread -lrmpio)

add_library(
    lib_cpp STATIC
    src/lib.cpp
    ${PATH_TO_LIBRPM}/rpmvercmp.h
)

target_link_libraries(lib_cpp pthread gtest)

target_link_libraries(test_lib lib_cpp)
target_include_directories(lib_cpp PUBLIC src ${PATH_TO_LIBRPM})


gtest_add_tests(
    TARGET test_lib
)
endif()