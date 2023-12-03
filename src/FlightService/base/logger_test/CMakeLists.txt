## THIS PROJECT JUST SHOWS LOGGER USAGE
## AND SHOULD BE DELETED WHEN THERE WILL
## BE ENOUGH EXAMPLES IN CODE

set( PROJECT_NAME "logger_test" )
project( ${PROJECT_NAME} CXX )

add_executable_( ${PROJECT_NAME}
    DEPENDS
        base
)

target_sources( ${PROJECT_NAME} PRIVATE
    main.cpp
)
