#[[
Project details.
]]
project(
	${CMAKE_PROJECT_NAME}Tests
	VERSION 0.1.0
	LANGUAGES CXX
)

#[[
For each file...
]]
foreach(file ${test_sources})
	#[[
	Add executable...
	]]
	string(REGEX REPLACE "(.*/)([a-zA-Z0-9_ ]+)(\.cpp)" "\\2" test_name ${file})
	add_executable(DynarrayTest ${file})
	#[[
	Compiler standard.
	]]
	target_compile_features(DynarrayTest PUBLIC cxx_std_20)
	#[[
	Code coverage.
	]]
	if(${CMAKE_PROJECT_NAME}_ENABLE_CODE_COVERAGE)
		target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
		target_link_options(${CMAKE_PROJECT_NAME} PUBLIC -fprofile-arcs -ftest-coverage)
	endif()
	#[[
	Testing framework.
	]]
	if(${CMAKE_PROJECT_NAME}_BUILD_EXECUTABLE)
		set(${CMAKE_PROJECT_NAME}_TEST_LIB ${CMAKE_PROJECT_NAME}_LIB)
	else()
		set(${CMAKE_PROJECT_NAME}_TEST_LIB ${CMAKE_PROJECT_NAME})
	endif()
	if(${CMAKE_PROJECT_NAME}_USE_GTEST)
		find_package(GTest REQUIRED)
		target_link_libraries(
			DynarrayTest
			PRIVATE
			GTest::GTest
			GTest::Main
			GTest::gmock
			GTest::gmock_main
			${${CMAKE_PROJECT_NAME}_TEST_LIB}
		)
	elseif(${CMAKE_PROJECT_NAME}_USE_CATCH2)
		find_package(Catch2 REQUIRED)
		target_link_libraries(
			DynarrayTest
			PRIVATE
			Catch2::Catch2
			${${CMAKE_PROJECT_NAME}_TEST_LIB}
		)
	else()
		message(FATAL_ERROR "Unknown testing framework.\n")
	endif()
	#[[
	Test(s).
	]]
	add_test(
		NAME
		${test_name}
		COMMAND
		DynarrayTest
	)
endforeach()