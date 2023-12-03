#pragma once

#include <exception>
#include <string>

#define BADOON_EXCEPTION( ExceptionName, ExceptionParent, message )            \
    class ExceptionName : public ExceptionParent                               \
    {                                                                          \
    public:                                                                    \
        ExceptionName() = default;                                             \
        ExceptionName( const std::string& msg )                                \
          : ExceptionParent( std::string( message ) + msg )                    \
        {                                                                      \
        }                                                                      \
    };

class BaseException : public std::exception
{
public:
    BaseException() = default;
    explicit BaseException( const std::string& message )
      : m_ErrorInfo( message )
    {
    }

    const char* what() const noexcept override { return m_ErrorInfo.c_str(); }

    const std::string& GetInfo() const noexcept { return m_ErrorInfo; }

protected:
    std::string m_ErrorInfo;
};
