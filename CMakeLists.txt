cmake_minimum_required(VERSION 3.0)
set(PROJECT_NAME CppCompiler)

project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 17)

########################################### Flex/Bison Config ###########################################

set(CMAKE_FIND_DEBUG_MODE ON)
set(BISON_DEBUG ON)
set(FLEX_DEBUG ON)
set(VERBOSE ON)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

file(REAL_PATH src/grammar/parser.y YACC_FILE)
file(REAL_PATH src/grammar/lexer.l LEX_FILE)

set(GRAMMAR_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/src/grammar)
file(MAKE_DIRECTORY ${GRAMMAR_BUILD_DIR})

FLEX_TARGET(LEXER ${LEX_FILE} ${GRAMMAR_BUILD_DIR}/lexer.cpp COMPILE_FLAGS "--verbose")

BISON_TARGET(PARSER ${YACC_FILE} ${GRAMMAR_BUILD_DIR}/parser.cpp COMPILE_FLAGS "--verbose")

ADD_FLEX_BISON_DEPENDENCY(LEXER PARSER)

#########################################################################################################

file(GLOB_RECURSE CPP_FILES src/*.cpp)
set(SOURCE_FILES_LIST ${CPP_FILES})

# CMake List is semicolon separated string, for printing it is better to convert it to new line separated
function(PRINT_NEW_LINE_LIST list)
    message(STATUS "${list}")
    if (ARGN)
        PRINT_NEW_LINE_LIST(${ARGN})
    else()
        message(STATUS "")
    endif()
endfunction()       

message(STATUS "C++ Source files:")
PRINT_NEW_LINE_LIST(${SOURCE_FILES_LIST})

message(STATUS "FLEX OUTPUT files:")
PRINT_NEW_LINE_LIST(${FLEX_LEXER_OUTPUTS})

message(STATUS "BISON OUTPUT files:")
PRINT_NEW_LINE_LIST(${BISON_PARSER_OUTPUTS})

add_executable(${PROJECT_NAME} ${SOURCE_FILES_LIST} ${FLEX_LEXER_OUTPUTS} ${BISON_PARSER_OUTPUTS})

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${GRAMMAR_BUILD_DIR})

########################################### For Visual Studio ###########################################

# Generate Folder Hierarchy instead of adding all files in the same folder
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES_LIST} ${LEX_FILE} ${YACC_FILE})

# Group Cmake predefined projects in CMakePredefinedTargets folder (as ZERO_CHECK , ALL_BUILD)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set this project as startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
