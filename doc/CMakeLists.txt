find_package(Doxygen)

if(NOT DOXYGEN_FOUND)
	message(FATAL_ERROR "Doxygen is needed to build the documentation.")
endif()

set(doxyfile_in ${CMAKE_CURRENT_LIST_DIR}/Doxyfile)
set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/doxyfile)

configure_file(${doxyfile_in} ${doxyfile} @ONLY)

message("Doxygen build started.")

add_custom_target(
	doc ALL
	COMMAND
	${DOXYGEN_EXECUTABLE} ${doxyfile_in}
	WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
	COMMENT "Generating API documentation with Doxygen"
	VERBATIM
)