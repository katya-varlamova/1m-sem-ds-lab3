cmake_minimum_required( VERSION 3.16 )

list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )
include( common )

project( GatewayService ) #set variablse BACKEND_SOURCE_DIR
#project command must be before include directives

include( libs )

add_subdirectory( base )
add_subdirectory( main )
add_subdirectory( NetUI )