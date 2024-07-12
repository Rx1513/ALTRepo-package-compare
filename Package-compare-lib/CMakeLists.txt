cmake_minimum_required(VERSION 3.10.0)
include(GNUInstallDirs)
set(PATH_TO_LIBRPM /usr/include/rpm/ CACHE PATH "Path to librpm headers")

project(pkgcmp VERSION 1.0 DESCRIPTION "Library for comparing packages provided by ALTRepoAPI")

option(ENABLE_TEST "Build test executable instead of shared library" OFF)
if (NOT ENABLE_TEST)

    add_library(${PROJECT_NAME} SHARED
    src/libpkgcmp.cpp
    src/libpkgcmp.hpp
        ${PATH_TO_LIBRPM}/rpmvercmp.h
    )

    target_include_directories(${PROJECT_NAME} PUBLIC src ${PATH_TO_LIBRPM})

    set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
    set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
    set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER src/libpkgcmp.hpp)

    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
      set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "/usr")
    endif()
    
    install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        COMPONENT library
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
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

    target_include_directories(test_lib PUBLIC src ${PATH_TO_LIBRPM})

    add_library(
        lib_cpp STATIC
        src/libpkgcmp.cpp
        ${PATH_TO_LIBRPM}/rpmvercmp.h
    )
    set(SHARED_LIBRARIES_TO_INCLUDE
        pkgcmp
        rpm
        rpmbuild
        popt
        pthread
    )

    target_link_libraries(test_lib gtest ${SHARED_LIBRARIES_TO_INCLUDE})

    target_link_libraries(test_lib lib_cpp)
    target_include_directories(lib_cpp PUBLIC src ${PATH_TO_LIBRPM})

    gtest_add_tests(
        TARGET test_lib
    )
endif()