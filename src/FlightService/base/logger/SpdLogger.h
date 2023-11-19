#pragma once

#include <memory>

#include "ILogger.h"
#include "config/base_config.h"

class SpdLogger : public ILogger
{
public:
    static std::shared_ptr<SpdLogger> Instance();
    static void InitLogger( const std::shared_ptr<BaseConfig>& config );
    virtual void LogInfo( const char* msg ) override;
    virtual void LogWarning( const char* msg ) override;
    virtual void LogError( const char* msg ) override;

private:
    SpdLogger();
};
