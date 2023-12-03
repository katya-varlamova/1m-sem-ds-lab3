set( CMAKE_CXX_STANDARD 20 )

if ( UNIX OR MINGW )
    set( CMAKE_CXX_FLAGS "-pedantic -Wall -Wextra -Wdiv-by-zero" )
    set( CMAKE_CXX_FLAGS_RELEASE "-O3 -finline-functions -DNDEBUG" )
    set( CMAKE_CXX_FLAGS_DEBUG "-g -fno-inline -DDEBUG" )
endif()

# should be removed, when all developers start work with single enviroment
if (${DOCKER_BUILD})
    set(CMAKE_CXX_COMPILER g++-10)

    # set(CMAKE_CXX_STANDARD 20)
    # set(CMAKE_CXX_STANDARD_REQUIRED ON)

    set(CMAKE_INCLUDE_PATH /usr/include/c++/10/)
endif()

function( add_library_ target library_type )
    add_library( ${target} ${library_type} )

    cmake_parse_arguments( ARGS_PARSED "CXX_20" "" "DEPENDS" ${ARGN} )
    
    if ( "${library_type}" STREQUAL "STATIC" )
        target_link_libraries( ${target} PUBLIC ${ARGS_PARSED_DEPENDS} )
    elseif( "${library_type}" STREQUAL "SHARED")
        target_link_libraries( ${target} PRIVATE ${ARGS_PARSED_DEPENDS} ) #TODO add object type
    else()
        message( FATAL_ERROR "WRONG LIBRARY TYPE" )
    endif()

    if ( ARGS_PARSED_CXX_20 )
        set_property( TARGET ${target} PROPERTY CXX_STANDARD 20 )
    endif()
    target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
endfunction()

macro( add_executable_ target )
    cmake_parse_arguments( ARGS_PARSED "TESTS;CXX_20" "" "DEPENDS" ${ARGN} )
    
    if ( "${ARGS_PARSED_TESTS}" AND NOT "${NEED_TESTS}")
        message( "SKIP TARGET: ${target} because NEED_TESTS is enabled" )
        return()
    endif()

    add_executable( ${target} )
    target_link_libraries( ${target} PRIVATE ${ARGS_PARSED_DEPENDS} )

    if ( ARGS_PARSED_TESTS )
        set_target_properties( ${target} PROPERTIES TESTS TRUE )
    endif()

    if ( ARGS_PARSED_CXX_20 )
        set_property( TARGET ${target} PROPERTY CXX_STANDARD 20 )
    endif()

    target_include_directories( ${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} )
endmacro()

enable_testing()

function( add_test_ target )
    get_target_property( IS_TEST ${target} TESTS )

    if ( NOT IS_TEST )
        message( FATAL_ERROR "Target: ${target} is not a test project, please add TEST FLAG TESTS to add_executable_ function call" )
    endif()

    cmake_parse_arguments( ARGS_PARSED "" "" "COMMAND_ARGUMENTS" ${ARGN} )

    string(REPLACE ";" " " ARGS_PARSED_COMMAND_ARGUMENTS "${ARGS_PARSED_COMMAND_ARGUMENTS}")

    add_test( NAME ${target} 
				COMMAND "$<TARGET_FILE:${target}>" -- ${ARGS_PARSED_COMMAND_ARGUMENTS}
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )
endfunction()
