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
	Add executable.
	]]
	string(REGEX REPLACE "(.*/)([a-zA-Z0-9_ ]+)(\.cpp)" "\\2" test_name ${file})
	add_executable(DynArrayTest ${file})
	#[[
	Set compiler standard.
	]]
	target_compile_features(DynArrayTest PUBLIC ${CXX_STANDARD})
	#[[
	Set testing framework.
	]]
	if(${BUILD_EXECUTABLE})
		set(${CMAKE_PROJECT_NAME}_TEST_LIB ${CMAKE_PROJECT_NAME}_LIB)
	else()
		set(${CMAKE_PROJECT_NAME}_TEST_LIB ${CMAKE_PROJECT_NAME})
	endif()
	#[[
	GTest.
	]]
	find_package(GTest REQUIRED)
	target_link_libraries(
		DynArrayTest
		PUBLIC
		GTest::GTest
		GTest::Main
		${${CMAKE_PROJECT_NAME}_TEST_LIB}
	)
	#[[
	Catch2.
	find_package(Catch2 REQUIRED)
	target_link_libraries(
		DynArrayTest
		PUBLIC
		Catch2::Catch2
		${${CMAKE_PROJECT_NAME}_TEST_LIB}
	)
	]]
	#[[
	Set test(s).
	]]
	add_test(
		NAME
		${test_name}
		COMMAND
		DynArrayTest
	)
endforeach()