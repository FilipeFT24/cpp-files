#[[
Print a message: verbose_message("...").
#]]
function(verbose_message content)
	if(${PROJECT_NAME}_VERBOSE_OUTPUT)
		message(STATUS ${content})
	endif()
endfunction()

#[[
Format the project using “clang-format” (i.e., w/ cmake --build build --target clang-format).
#]]
function(add_clang_format_target)
	if(NOT ${PROJECT_NAME}_CLANG_FORMAT_BINARY)
		find_program(${PROJECT_NAME}_CLANG_FORMAT_BINARY clang-format)
	endif()
	if(${PROJECT_NAME}_CLANG_FORMAT_BINARY)
		if(${PROJECT_NAME}_BUILD_HEADERS_ONLY)
			#[[
			Header-only.
			#]]
			add_custom_target(clang-format
			                  COMMAND           ${${PROJECT_NAME}_CLANG_FORMAT_BINARY} -i ${headers}
			                  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
		elseif(${PROJECT_NAME}_BUILD_EXECUTABLE)
			#[[
			Executable.
			#]]
			add_custom_target(clang-format
			                  COMMAND           ${${PROJECT_NAME}_CLANG_FORMAT_BINARY} -i ${exe_sources} ${headers}
			                  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
		else()
			#[[
			Else.
			#]]
			add_custom_target(clang-format
			                  COMMAND           ${${PROJECT_NAME}_CLANG_FORMAT_BINARY} -i ${sources} ${headers}
			                  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
		endif()
		message(STATUS "Format the project using “clang-format” (i.e., cmake --build build --target clang-format).\n")
	endif()
endfunction()