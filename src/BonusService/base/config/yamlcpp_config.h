#ifndef YAMLCPPCONFIG_H
#define YAMLCPPCONFIG_H

#include <common_macros.h>

#include <boost/di.hpp>

#include "base_config.h"
#include "yaml-cpp/yaml.h"

namespace di = boost::di;

DI_NAME_DECLARE( configFileName )

class YamlCppConfig : public BaseConfig
{
public:
    BOOST_DI_INJECT(
      YamlCppConfig,
      ( named = configFileName ) const std::string& filename
    );

    virtual int GetIntField( const std::vector<std::string>& path
    ) const override;
    virtual uint GetUintField( const std::vector<std::string>& path
    ) const override;
    virtual std::string GetStringField( const std::vector<std::string>& path
    ) const override;

    virtual std::vector<int> GetIntArray( const std::vector<std::string>& path
    ) const override;
    virtual std::vector<uint> GetUintArray( const std::vector<std::string>& path
    ) const override;
    virtual std::vector<std::string> GetStringArray(
      const std::vector<std::string>& path
    ) const override;

private:
    template<typename T>
    std::vector<T> GetArray( const std::vector<std::string>& path ) const;
    void GetNode( YAML::Node& node, const std::vector<std::string>& path )
      const;

    YAML::Node m_StartNode;
};

template<typename T>
std::vector<T>
YamlCppConfig::GetArray( const std::vector<std::string>& path ) const
{
    YAML::Node node;
    GetNode( node, path );

    std::vector<T> res;
    for ( size_t i = 0; i < node.size(); i++ )
        res.push_back( node[i].as<T>() );

    return res;
}

#endif // YAMLCPPCONFIG_H
