#pragma once
#include <cstring>
#include <map>
#include <memory>
#include <string>

#include "Connection/PGConnection.h"
#include "IFlightRepository.h"

#include "exceptions/database_exceptions.h"
#include "logger/LoggerFactory.h"
class PGFlightRepository : public IFlightRepository
{
public:
    PGFlightRepository( const PGConnectionCreatorPtr& connectionCreator )
    {
        m_conn = connectionCreator->GetConnect();
        m_connectionCreator = connectionCreator;
    }
    virtual Flight GetFlightByFlightNumber( const std::string & number ) override;
    virtual std::vector<Flight> GetFlights( ) override;
    ~PGFlightRepository() override
    {
        m_connectionCreator->FinishConnect( m_conn );
    }

protected:
    Flight ParseFlight( std::map<std::string, std::string> strs );

private:
    PGConnectPtr m_conn;
    PGConnectionCreatorPtr m_connectionCreator;
};
