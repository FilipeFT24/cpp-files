cmake_minimum_required(VERSION 3.25.1)

#[[
Project details.
]]
project(
	"Dynarray"
	VERSION 0.1.0
	LANGUAGES CXX
)
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(FATAL_ERROR "In-source builds are not allowed.\n")
endif()

#[[
Project directories.
]]
include(cmake/Utils.cmake)
include(cmake/CompilerWarnings.cmake)
include(cmake/Doxygen.cmake)
include(cmake/SourcesAndHeaders.cmake)
include(cmake/StandardSettings.cmake)
include(cmake/StaticAnalyzers.cmake)
include(cmake/Vcpkg.cmake)

#[[
Project setup.
]]
if(${PROJECT_NAME}_BUILD_HEADERS_ONLY)
	#[[
	Header-only.
	]]
	add_library(${PROJECT_NAME} INTERFACE)
elseif(${PROJECT_NAME}_BUILD_EXECUTABLE)
	#[[
	Executable...
	]]
	add_executable(${PROJECT_NAME} ${exe_sources})
	#[[
	...w/ unit testing.
	]]
	if(${PROJECT_NAME}_ENABLE_UNIT_TESTING)
		add_library(${PROJECT_NAME}_LIB ${headers} ${sources})
	endif()
else()
	#[[
	Else.
	]]
	add_library(
		${PROJECT_NAME}
		${headers}
		${sources}
	)
endif()

set_target_properties(
	${PROJECT_NAME}
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}"
)
if(${PROJECT_NAME}_BUILD_EXECUTABLE AND ${PROJECT_NAME}_ENABLE_UNIT_TESTING)
	set_target_properties(
		${PROJECT_NAME}_LIB
		PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
		OUTPUT_NAME ${PROJECT_NAME}
	)
endif()

#[[
Project standard and warnings.
]]
if(${PROJECT_NAME}_BUILD_HEADERS_ONLY)
	#[[
	Header-only.
	]]
	target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)
else()
	#[[
	Else.
	]]
	target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
	#[[
	Executable && w/ unit testing.
	]]
	if(${PROJECT_NAME}_BUILD_EXECUTABLE AND ${PROJECT_NAME}_ENABLE_UNIT_TESTING)
		target_compile_features(${PROJECT_NAME}_LIB PUBLIC cxx_std_20)
	endif()
endif()
set_project_warnings(${PROJECT_NAME})

#[[
Project build/user include directories.
]]
if(${PROJECT_NAME}_BUILD_HEADERS_ONLY)
	#[[
	Header-only.
	]]
	target_include_directories(
		${PROJECT_NAME}
		INTERFACE
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	)
else()
	#[[
	Else.
	]]
	target_include_directories(
		${PROJECT_NAME}
		PUBLIC
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src
	)
	if(${PROJECT_NAME}_BUILD_EXECUTABLE AND ${PROJECT_NAME}_ENABLE_UNIT_TESTING)
		#[[
		Executable && w/ unit testing.
		]]
		target_include_directories(
			${PROJECT_NAME}_LIB
			PUBLIC
			$<INSTALL_INTERFACE:include>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
			PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}/src
		)
	endif()
endif()

#[[
Unit testing.
]]
if(${PROJECT_NAME}_ENABLE_UNIT_TESTING)
	enable_testing()
	add_subdirectory(test)
endif()

#[[
Format the project using “clang-format” (i.e., cmake --build build --target clang-format).
]]
#add_clang_format_target()