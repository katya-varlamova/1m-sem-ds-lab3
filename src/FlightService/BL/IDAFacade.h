#pragma once

#include <config/base_config.h>
#include <structs/Flight.h>

class IDAFacade
{
public:
    virtual Flight GetFlightByFlightNumber( const std::string & number ) = 0;
    virtual std::vector<Flight> GetFlights( ) = 0;

    virtual ~IDAFacade() = default;
};

using IDAFacadePtr = std::shared_ptr<IDAFacade>;