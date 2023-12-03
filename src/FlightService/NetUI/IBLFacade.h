#pragma once

#include <config/base_config.h>
#include "structs/Flight.h"
#include "IDAFactory.h"
class IBLFacade{
public:
    virtual Flight GetFlightByFlightNumber( const std::string & number ) = 0;
    virtual std::vector<Flight> GetFlights( ) = 0;
    virtual ~IBLFacade() = default;
};

using IBLFacadePtr = std::shared_ptr<IBLFacade>;


