find_package(Doxygen REQUIRED)
add_custom_target(
	Documentation ALL
	COMMAND
	${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/Doxyfile
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	VERBATIM
)