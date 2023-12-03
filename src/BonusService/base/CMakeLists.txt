set( PROJECT_NAME "base" )
project( ${PROJECT_NAME} CXX )

add_subdirectory(logger_test)

add_library_( ${PROJECT_NAME} STATIC
    DEPENDS
        yaml-cpp
        spdlog::spdlog
)

target_sources( ${PROJECT_NAME} PRIVATE
    common_macros.h
    exceptions/base_exception.h
    exceptions/database_exceptions.h
    exceptions/logic_exceptions.h
    exceptions/server_exceptions.h
    config/base_config.h
    config/base_sections.h
    config/yamlcpp_config.h
    config/yamlcpp_config.cpp
    logger/ILogger.h
    logger/SpdLogger.h
    logger/SpdLogger.cpp
    logger/LoggerFactory.h
    logger/LoggerFactory.cpp
)
