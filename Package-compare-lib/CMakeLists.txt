cmake_minimum_required(VERSION 3.5)

set(PATH_TO_LIBRPM /usr/include/rpm/ CACHE PATH "Path to librpm headers")

project(pkgcmp VERSION 1.0 DESCRIPTION "Library for comparing packages provided by ALTRepoAPI")

add_library(${PROJECT_NAME} SHARED
    src/lib.cpp
    src/lib.hpp
    ${PATH_TO_LIBRPM}/rpmvercmp.h
)

target_include_directories(${PROJECT_NAME} PUBLIC src ${PATH_TO_LIBRPM})

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER src/lib.hpp)


install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_PREFIX_PATH}/lib64
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
