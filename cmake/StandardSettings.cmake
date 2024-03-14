#[[
Link Time Optimization (LTO).
]]
if(${ENABLE_LTO})
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
]]
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK    ccache)
endif()
if(${ENABLE_ASAN})
	add_compile_options(-fsanitize=address)
	add_link_options(-fsanitize=address)
endif()