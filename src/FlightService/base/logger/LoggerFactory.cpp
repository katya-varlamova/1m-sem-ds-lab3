#include "LoggerFactory.h"

#include "SpdLogger.h"

void
LoggerFactory::InitLogger( const std::shared_ptr<BaseConfig>& config )
{
    SpdLogger::InitLogger( config );
}

std::shared_ptr<ILogger>
LoggerFactory::GetLogger()
{
    return SpdLogger::Instance();
}
