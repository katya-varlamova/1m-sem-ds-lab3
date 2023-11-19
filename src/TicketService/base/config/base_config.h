#ifndef BASE_CONFIG_H
#define BASE_CONFIG_H

#include <memory>
#include <string>
#include <vector>

#include "base_sections.h"

class BaseConfig;

using BaseConfigPtr = std::shared_ptr<BaseConfig>;
using uint = unsigned int;

class BaseConfig
{
public:
    virtual ~BaseConfig() = default;
    virtual int GetIntField( const std::vector<std::string>& path ) const = 0;
    virtual uint GetUintField( const std::vector<std::string>& path ) const = 0;
    virtual std::string GetStringField( const std::vector<std::string>& path
    ) const = 0;

    virtual std::vector<int> GetIntArray( const std::vector<std::string>& path
    ) const = 0;
    virtual std::vector<uint> GetUintArray( const std::vector<std::string>& path
    ) const = 0;
    virtual std::vector<std::string> GetStringArray(
      const std::vector<std::string>& path
    ) const = 0;
};

#endif // BASE_CONFIG_H
