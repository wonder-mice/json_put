cmake_minimum_required(VERSION 3.2)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
if(NOT DEFINED CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type")
endif()

project(json_put)

set(INSTALL_INCLUDE_DIR include CACHE PATH
	"Installation directory for header files")
set(INSTALL_LIB_DIR lib CACHE PATH
	"Installation directory for libraries")
set(INSTALL_CMAKE_DIR lib/cmake/json_put CACHE PATH
	"Installation directory for CMake files")
option(JSON_PUT_CONFIGURE_INSTALL "Generate install target" ON)
option(JSON_PUT_EXAMPLES "Build examples" OFF)
option(JSON_PUT_TESTS "Build tests" OFF)

add_subdirectory(json_put)

if(JSON_PUT_EXAMPLES)
	add_subdirectory(examples)
endif()
if(JSON_PUT_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()

if(JSON_PUT_CONFIGURE_INSTALL)
	export(EXPORT json_put)
	install(EXPORT json_put
		DESTINATION ${INSTALL_CMAKE_DIR})
	configure_file(json_put-config.cmake.in json_put-config.cmake @ONLY)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/json_put-config.cmake
		DESTINATION ${INSTALL_CMAKE_DIR})
endif()
