#pragma once

#include <memory>
#include <vector>
#include "structs/Flight.h"

class IFlightRepository
{
public:
    virtual Flight GetFlightByFlightNumber( const std::string & number ) = 0;
    virtual std::vector<Flight> GetFlights( ) = 0;
    virtual ~IFlightRepository() = default;
};
using IFlightRepositoryPtr = std::shared_ptr<IFlightRepository>;