cmake_minimum_required(VERSION 3.7.1)

project(pkgcmp_util VERSION 1.0 DESCRIPTION "CLI utility for comparing packages provided by ALTRepoAPI")

add_executable(${PROJECT_NAME}
    src/main.cpp
)

set(SHARED_LIBRARIES_TO_INCLUDE
  pkgcmp
  boost_system
  ssl
  crypto 
  rpm
  rpmbuild
  popt
  pthread
)

target_link_libraries(${PROJECT_NAME} ${SHARED_LIBRARIES_TO_INCLUDE})

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "/usr")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME pkgcmp)

install(TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT utility
)