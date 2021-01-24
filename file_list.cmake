# Load strings submodule
include(lib/strings/file_list)
list(APPEND INCLUDE_DIRS ${PUBLIC_HEADERS_DIR})

# paths to various directories
get_filename_component(GENERATED_HEADERS_DIR ${CMAKE_BINARY_DIR}/generated-headers ABSOLUTE)
get_filename_component(PUBLIC_HEADERS_DIR    ${CMAKE_CURRENT_LIST_DIR}/include ABSOLUTE)
get_filename_component(PRIVATE_HEADERS_DIR   ${CMAKE_CURRENT_LIST_DIR}/private-headers ABSOLUTE)
get_filename_component(SRC_DIR               ${CMAKE_CURRENT_LIST_DIR}/src ABSOLUTE)
get_filename_component(TEST_DIR              ${CMAKE_CURRENT_LIST_DIR}/test ABSOLUTE)
get_filename_component(LIBRARY_DIR           ${CMAKE_CURRENT_LIST_DIR}/lib ABSOLUTE)
list(APPEND INCLUDE_DIRS ${PUBLIC_HEADERS_DIR} ${PRIVATE_HEADERS_DIR} ${GENERATED_HEADERS_DIR})

unset(DEBUG_SCOPES CACHE)

# public headers
set(PUBLIC_HEADERS
  ${PUBLIC_HEADERS_DIR}/document.hpp
  ${PUBLIC_HEADERS_DIR}/error.hpp
  ${PUBLIC_HEADERS_DIR}/parse.hpp
  ${PUBLIC_HEADERS_DIR}/string_ref.hpp
)
message(${PUBLIC_HEADERS})

# source files
set(SOURCES
  ${SRC_DIR}/document.cpp
  ${SRC_DIR}/execstream.cpp
  ${SRC_DIR}/logger.cpp
  ${SRC_DIR}/add_key.cpp
  ${SRC_DIR}/parse.cpp
  ${SRC_DIR}/string_ref.cpp
  ${TSTRING_SOURCES}
)

set(INTERNAL_TESTS execstream_test)
set(EXTERNAL_TESTS parse_file parse_manual)
set(COPIED_FILES key_file.txt assign_test.txt parse_test.txt)