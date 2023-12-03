#include "yamlcpp_config.h"

#include <exceptions/server_exceptions.h>

DI_NAME_DEFINE( configFileName )

YamlCppConfig::YamlCppConfig( const std::string& filename )
  : m_StartNode( YAML::LoadFile( filename ) )
{
}

void
YamlCppConfig::GetNode( YAML::Node& node, const std::vector<std::string>& path )
  const
{
    node = YAML::Clone( m_StartNode );
    try {
        for ( std::string section : path ) {
            node = node[section.c_str()];
        }
    } catch ( YAML::InvalidNode& ex ) {
        throw ConfigReaderException( ex.what() );
    }
}

int
YamlCppConfig::GetIntField( const std::vector<std::string>& path ) const
{
    YAML::Node node;
    GetNode( node, path );

    return node.as<int>();
}

uint
YamlCppConfig::GetUintField( const std::vector<std::string>& path ) const
{
    YAML::Node node;
    GetNode( node, path );

    return node.as<uint>();
}

std::string
YamlCppConfig::GetStringField( const std::vector<std::string>& path ) const
{
    YAML::Node node;
    GetNode( node, path );

    return node.as<std::string>();
}

std::vector<int>
YamlCppConfig::GetIntArray( const std::vector<std::string>& path ) const
{
    return GetArray<int>( path );
}

std::vector<uint>
YamlCppConfig::GetUintArray( const std::vector<std::string>& path ) const
{
    return GetArray<uint>( path );
}

std::vector<std::string>
YamlCppConfig::GetStringArray( const std::vector<std::string>& path ) const
{
    return GetArray<std::string>( path );
}
