#include "PGFlightRepository.h"
Flight
PGFlightRepository::ParseFlight( std::map<std::string, std::string> strs )
{
    return Flight(
      strs[std::string( "flight_number" )],
      strs[std::string( "datetime" )],
      std::stoi(strs[std::string( "price" )]),
      strs[std::string( "from_airport_name" )],
      strs[std::string( "from_airport_city" )],
      strs[std::string( "to_airport_name" )],
      strs[std::string( "to_airport_city" )]
    );
}
std::vector<Flight>
PGFlightRepository::GetFlights()
{
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );
    std::string query(
            "select flight_number, datetime, price, a1.name as from_airport_name, a1.city as from_airport_city, "
            "a2.name as to_airport_name, a2.city as to_airport_city "
            "from flight join airport a1 on flight.from_airport_id = a1.id join airport a2 on flight.to_airport_id = a2.id "
            "order by flight_number ASC;"
    );
    PGresult* pgRes = PQexec( m_conn, query.c_str() );

    if ( PQresultStatus( pgRes ) != PGRES_TUPLES_OK ) {
        LoggerFactory::GetLogger()->LogWarning(
                ( std::string(
                        "GetFlights: "
                ) +
                  PQresultErrorMessage( pgRes ) )
                        .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "No such Flights!"
        );
    }

    std::vector<Flight> flights;
    for ( int i = 0; i < PQntuples( pgRes ); i++ ) {
        std::map<std::string, std::string> res;
        for ( int j = 0; j < PQnfields( pgRes ); ++j ) {
            char* fn = PQfname( pgRes, j );
            char* val = PQgetvalue( pgRes, i, j );
            res[std::string( fn )] = std::string( val );
        }
        flights.push_back( ParseFlight( res ) );
    }

    LoggerFactory::GetLogger()->LogInfo(
            "GetFlights: no errors!"
    );
    PQclear( pgRes );
    return flights;
}
Flight
PGFlightRepository::GetFlightByFlightNumber (const std::string & number)
{
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );
    std::string query(
      "select flight_number, datetime, price, a1.name as from_airport_name, a1.city as from_airport_city, "
      "a2.name as to_airport_name, a2.city as to_airport_city "
      "from flight join airport a1 on flight.from_airport_id = a1.id join airport a2 on flight.to_airport_id = a2.id "
      "where flight_number = '" +  number + "';"
    );
    PGresult* pgRes = PQexec( m_conn, query.c_str() );

    if ( PQresultStatus( pgRes ) != PGRES_TUPLES_OK || PQntuples( pgRes ) == 0 ) {
        LoggerFactory::GetLogger()->LogWarning(
          ( std::string( "GetFlightByFlightNumber: no such Flight: " ) +
            PQresultErrorMessage( pgRes ) )
            .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "no such Flight" );
    }

    std::map<std::string, std::string> res;
    for ( int j = 0; j < PQnfields( pgRes ); ++j ) {
        char* fn = PQfname( pgRes, j );
        char* name = PQgetvalue( pgRes, 0, j );
        res[std::string( fn )] = std::string( name );
    }
    auto Flight = ParseFlight( res );

    LoggerFactory::GetLogger()->LogInfo( "GetFlightByFlightNumber: Flight found" );
    PQclear( pgRes );
    return Flight;
}
