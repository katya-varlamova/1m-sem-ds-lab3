#pragma once

#include <memory>

#include "ILogger.h"
#include "config/base_config.h"

class LoggerFactory
{
public:
    static void InitLogger( const std::shared_ptr<BaseConfig>& config );
    static std::shared_ptr<ILogger> GetLogger();
    LoggerFactory() = delete;
};
