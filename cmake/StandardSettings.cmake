#[[
Project settings.
#]]
option(${PROJECT_NAME}_BUILD_EXECUTABLE "Build the project as an executable, rather than a library." OFF)
option(${PROJECT_NAME}_BUILD_HEADERS_ONLY "Build the project as a header-only library." ON)

#[[
Compiler options.
#]]
option(${PROJECT_NAME}_WARNINGS_AS_ERRORS "Treat (compiler) warnings as errors." OFF)

#[[
Doxygen.
#]]
option(${PROJECT_NAME}_ENABLE_DOXYGEN "Enable Doxygen documentation builds of source." ON)

#[[
Unit testing.
#]]
option(${PROJECT_NAME}_ENABLE_UNIT_TESTING "Enable unit tests for the projects (from the `test` subfolder)." ON)

option(${PROJECT_NAME}_USE_GTEST "Use the GoogleTest project for creating unit tests." ON)
option(${PROJECT_NAME}_USE_GOOGLE_MOCK "Use the GoogleMock project for extending the unit tests." OFF)

option(${PROJECT_NAME}_USE_CATCH2 "Use the Catch2 project for creating unit tests." OFF)






#[[
Static analyzers.
#]]
option(${PROJECT_NAME}_ENABLE_CLANG_TIDY "Enable static analysis with Clang-Tidy." OFF)
option(${PROJECT_NAME}_ENABLE_CPPCHECK "Enable static analysis with Cppcheck." OFF)

#[[
Code coverage.
#]]
option(${PROJECT_NAME}_ENABLE_CODE_COVERAGE "Enable code coverage through GCC." OFF)

#[[
Miscellaneous options.
#]]
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(${PROJECT_NAME}_VERBOSE_OUTPUT "Enable verbose output, allowing for a better understanding of each step taken." ON)

#[[
Link Time Optimization (LTO).
#]]
option(${PROJECT_NAME}_ENABLE_LTO "Enable Link Time Optimization (LTO)." OFF)
if(${PROJECT_NAME}_ENABLE_LTO)
	include(CheckIPOSupported)
	check_ipo_supported(RESULT result OUTPUT output)
	if(result)
		set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
	else()
		message(SEND_ERROR "IPO is not supported: ${output}.")
	endif()
endif()

#[[
Ccache.
#]]
option(${PROJECT_NAME}_ENABLE_CCACHE "Enable the usage of Ccache, in order to speed up rebuild times." ON)
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK    ccache)
endif()

option(${PROJECT_NAME}_ENABLE_ASAN "Enable Address Sanitize to detect memory error." ON)
if(${PROJECT_NAME}_ENABLE_ASAN)
	add_compile_options(-fsanitize=address)
	add_link_options(-fsanitize=address)
endif()