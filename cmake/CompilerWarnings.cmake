function(set_project_warnings project_name)
	#[[
	Clang warnings.
	#]]
	set(CLANG_WARNINGS
	    -Wall
	    -Wextra
	    -Wshadow
	    -Wnon-virtual-dtor
	    -Wold-style-cast
	    -Wcast-align
	    -Wunused
	    -Woverloaded-virtual
	    -Wpedantic
	    -Wconversion
	    -Wsign-conversion
	    -Wnull-dereference
	    -Wdouble-promotion
	    -Wformat=2
	)
	#[[
	GCC warnings.
	#]]
	set(GCC_WARNINGS
	    ${CLANG_WARNINGS}
	    -Wmisleading-indentation
	    -Wduplicated-cond
	    -Wduplicated-branches
	    -Wlogical-op
	    -Wuseless-cast
	)
	#[[
	MSVC warnings.
	#]]
	set(MSVC_WARNINGS
	    /W4
	    /w14242
	    /w14254
	    /w14263
	    /w14287
	    /we4289
	    /w14296
	    /w14311
	    /w14545
	    /w14546
	    /w14547
	    /w14549
	    /w14555
	    /w14619
	    /w14640
	    /w14826
	    /w14905
	    /w14906
	    /w14928
	    /permissive-
	)

	#[[
	Treat warnings as errors?
	#]]
	if(${PROJECT_NAME}_WARNINGS_AS_ERRORS)
		set(CLANG_WARNINGS ${CLANG_WARNINGS} -Werror)
		set(MSVC_WARNINGS ${MSVC_WARNINGS} /WX)
	endif()
	#[[
	Project warnings.
	#]]
	if(MSVC)
		set(PROJECT_WARNINGS ${MSVC_WARNINGS})
	elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
		set(PROJECT_WARNINGS ${CLANG_WARNINGS})
	elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		set(PROJECT_WARNINGS ${GCC_WARNINGS})
	else()
		message(AUTHOR_WARNING "No (compiler) warnings set for '${CMAKE_CXX_COMPILER_ID}' compiler.\n")
	endif()
	if(${PROJECT_NAME}_BUILD_HEADERS_ONLY)
		target_compile_options(${project_name} INTERFACE ${PROJECT_WARNINGS})
	else()
		target_compile_options(${project_name} PUBLIC ${PROJECT_WARNINGS})
	endif()
	if(NOT TARGET ${project_name})
		message(AUTHOR_WARNING "${project_name} is not a target, so no compiler warning was added.\n")
	endif()
endfunction()