#pragma once
#include <libpq-fe.h>

#include "config/base_config.h"
#include "config/base_sections.h"
#include "exceptions/database_exceptions.h"
#include "logger/LoggerFactory.h"
#include <memory>
using PGConnectPtr = PGconn*;
class PGConnection
{
public:
    PGConnection( const std::shared_ptr<BaseConfig>& conf, int section = 0 )
      : m_section( section )
    {
        m_conn =
          "host=" + conf->GetStringField( { DbHostSection } ) +
          " port=" + conf->GetStringField( { DbPortSection } ) +
          " user=" + conf->GetStringField( { DbUserSection } ) +
          " dbname=" + conf->GetStringField( { DbNameSection } ) +
          " password=" + conf->GetStringField( { DbUserPasswordSection } );
    }
    PGConnectPtr GetConnect();
    void CheckConnectAndReopenIfNeeded( PGConnectPtr& conn );
    void FinishConnect( PGConnectPtr conn ) { PQfinish( conn ); }

private:
    std::string m_conn;
    int m_section;
};
using PGConnectionCreatorPtr = std::shared_ptr<PGConnection>;
