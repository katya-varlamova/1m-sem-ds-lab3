#pragma once

class ILogger
{
public:
    virtual ~ILogger() = default;
    virtual void LogInfo( const char* msg ) = 0;
    virtual void LogWarning( const char* msg ) = 0;
    virtual void LogError( const char* msg ) = 0;
};
