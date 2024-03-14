#[[
Clang-tidy.
]]
if(${ENABLE_CLANG_TIDY})
	find_program(CLANGTIDY clang-tidy)
	if(CLANGTIDY)
		set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY} -extra-arg = -Wno-unknown-warning-option)
	else()
		message(SEND_ERROR "Clang-Tidy requested but executable not found.")
	endif()
endif()

#[[
Cppcheck.
]]
if(${ENABLE_CPPCHECK})
	find_program(CPPCHECK cppcheck)
	if(CPPCHECK)
		set(CMAKE_CXX_CPPCHECK ${CPPCHECK} --suppress = missingInclude --enable = all --inline-suppr --inconclusive -i ${CMAKE_SOURCE_DIR}/imgui/lib)
	else()
		message(SEND_ERROR "Cppcheck requested but executable not found.")
	endif()
endif()