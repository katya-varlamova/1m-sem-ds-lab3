#include "PGTicketRepository.h"
Ticket
PGTicketRepository::ParseTicket( std::map<std::string, std::string> strs )
{
    return Ticket(
      std::stoi( strs[std::string( "id" )] ),
      strs[std::string( "ticket_uid" )],
      strs[std::string( "username" )],
      strs[std::string( "flight_number" )],
      std::stoi(strs[std::string( "price" )]),
      strs[std::string( "status" )]
    );
}
std::vector<Ticket>
PGTicketRepository::GetTicketsByUsername(const std::string &username)
{
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );
    std::string query(
            "select * from ticket where username = '" +  username + "';"
    );
    PGresult* pgRes = PQexec( m_conn, query.c_str() );

    if ( PQresultStatus( pgRes ) != PGRES_TUPLES_OK ) {
        LoggerFactory::GetLogger()->LogWarning(
                ( std::string(
                        "GetTicketsByUsername: "
                ) +
                  PQresultErrorMessage( pgRes ) )
                        .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "No such tickets!"
        );
    }

    std::vector<Ticket> users;
    for ( int i = 0; i < PQntuples( pgRes ); i++ ) {
        std::map<std::string, std::string> res;
        for ( int j = 0; j < PQnfields( pgRes ); ++j ) {
            char* fn = PQfname( pgRes, j );
            char* val = PQgetvalue( pgRes, i, j );
            res[std::string( fn )] = std::string( val );
        }
        users.push_back( ParseTicket( res ) );
    }

    LoggerFactory::GetLogger()->LogInfo(
            "GetTicketsByUsername: no errors!"
    );
    PQclear( pgRes );
    return users;
}
Ticket
PGTicketRepository::GetTicketByUUID (const std::string & uuid)
{
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );
    std::string query(
      "select * from ticket where ticket_uid = '" +  uuid + "';"
    );
    PGresult* pgRes = PQexec( m_conn, query.c_str() );

    if ( PQresultStatus( pgRes ) != PGRES_TUPLES_OK || PQntuples( pgRes ) == 0 ) {
        LoggerFactory::GetLogger()->LogWarning(
          ( std::string( "GetTicketByUUID: no such Ticket: " ) +
            PQresultErrorMessage( pgRes ) )
            .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "no such Ticket" );
    }

    std::map<std::string, std::string> res;
    for ( int j = 0; j < PQnfields( pgRes ); ++j ) {
        char* fn = PQfname( pgRes, j );
        char* name = PQgetvalue( pgRes, 0, j );
        res[std::string( fn )] = std::string( name );
    }
    auto Ticket = ParseTicket( res );

    LoggerFactory::GetLogger()->LogInfo( "GetTicketByUUID: Ticket found" );
    PQclear( pgRes );
    return Ticket;
}
std::string
PGTicketRepository::CreateTicket( const Ticket& ticket )
{
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );

    std::string query(
            "insert into ticket (ticket_uid, username, flight_number, price, status) values ('"
            + ticket.ticket_uid + "', '"
            + ticket.username + "', '"
            + ticket.flight_number + "', "
            + std::to_string(ticket.price) + ", '"
            + ticket.status + "') returning ticket_uid;"
    );

    PGresult* pgRes = PQexec( m_conn, query.c_str() );

    if ( PQresultStatus( pgRes ) != PGRES_TUPLES_OK ) {
        LoggerFactory::GetLogger()->LogWarning(
                ( std::string( "CreateTicket: create failed: " ) +
                  PQresultErrorMessage( pgRes ) )
                        .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "create failed" );
    }
    std::string uuid;
    for ( int i = 0; i < PQntuples( pgRes ); i++ ) {
        int fn = PQfnumber( pgRes, "ticket_uid" );
        char* name = PQgetvalue( pgRes, i, fn );
        uuid = name;
    }

    LoggerFactory::GetLogger()->LogInfo(
            ( std::string( "CreateTicket: create succeed with uuid = " ) +
               uuid )
                    .c_str()
    );

    PQclear( pgRes );
    return uuid;
}

void
PGTicketRepository::DeleteTicket(const std::string & uuid, const std::string &username )
{
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );
    std::string query(
      "update ticket set status = 'CANCELED' where ticket_uid = '" + uuid + "' and username = '" + username +"';"
    );
    PGresult* pgRes = PQexec( m_conn, query.c_str() );

    if ( PQresultStatus( pgRes ) != PGRES_COMMAND_OK ) {
        LoggerFactory::GetLogger()->LogWarning(
          ( std::string( "CancelTicket: there was no such Ticket: " ) +
            PQresultErrorMessage( pgRes ) )
            .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "there was no such Ticket" );
    }

    LoggerFactory::GetLogger()->LogInfo( "CancelTicket: Ticket was deleted" );
    PQclear( pgRes );
}

void PGTicketRepository::RollbackCreateTicket(const std::string &uuid, const std::string &username) {
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );
    std::string query(
            "delete from ticket where ticket_uid = '" + uuid + "' and username = '" + username +"';"
    );
    PGresult* pgRes = PQexec( m_conn, query.c_str() );

    if ( PQresultStatus( pgRes ) != PGRES_COMMAND_OK ) {
        LoggerFactory::GetLogger()->LogWarning(
                ( std::string( "RollbackCreateTicket: there was no such Ticket: " ) +
                  PQresultErrorMessage( pgRes ) )
                        .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "there was no such Ticket" );
    }

    LoggerFactory::GetLogger()->LogInfo( "RollbackCreateTicket: Ticket was deleted" );
    PQclear( pgRes );
}