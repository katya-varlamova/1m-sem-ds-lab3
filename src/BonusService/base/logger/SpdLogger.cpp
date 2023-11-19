#include "SpdLogger.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include "config/base_sections.h"

SpdLogger::SpdLogger()
{
}

std::shared_ptr<SpdLogger>
SpdLogger::Instance()
{
    static std::shared_ptr<SpdLogger> logger( new SpdLogger );
    return logger;
}

void
SpdLogger::InitLogger( const std::shared_ptr<BaseConfig>& config )
{
    std::string filePath = config->GetStringField( { LogFileSection } );

    if ( filePath != "stdout" ) {
        auto logger = spdlog::basic_logger_mt( "file_logger", filePath, true );
        spdlog::set_default_logger( logger );
    }
}

void
SpdLogger::LogInfo( const char* msg )
{
    spdlog::info( msg );
}

void
SpdLogger::LogWarning( const char* msg )
{
    spdlog::warn( msg );
}

void
SpdLogger::LogError( const char* msg )
{
    spdlog::error( msg );
}
