#include "PGConnection.h"
PGConnectPtr
PGConnection::GetConnect()
{
    PGConnectPtr connPtr = PQconnectdb( m_conn.c_str() );
    if ( PQstatus( connPtr ) != CONNECTION_OK ) {
        PQfinish( connPtr );
        throw DatabaseConnectException( "unable to connect" );
    }
    return connPtr;
}
void
PGConnection::CheckConnectAndReopenIfNeeded( PGConnectPtr& conn )
{
    if ( PQstatus( conn ) != CONNECTION_OK ) {
        PQfinish( conn );
        LoggerFactory::GetLogger()->LogWarning(
          ( std::string( "CheckConnectAndReopenIfNeeded: not ok, reopen" ) )
            .c_str()
        );
        conn = GetConnect();
        LoggerFactory::GetLogger()->LogWarning(
          ( std::string( "CheckConnectAndReopenIfNeeded: not ok, reopened" ) )
            .c_str()
        );
    }
}
