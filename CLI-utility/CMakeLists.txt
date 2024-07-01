cmake_minimum_required(VERSION 3.7.1)

project(pkgcmp_util VERSION 1.0 DESCRIPTION "CLI utility for comparing packages provided by ALTRepoAPI")

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} pkgcmp boost_system ssl crypto rpm rpmbuild popt)
target_compile_options(${PROJECT_NAME} PRIVATE -pthread -lrmpio)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "/usr")
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR}
)