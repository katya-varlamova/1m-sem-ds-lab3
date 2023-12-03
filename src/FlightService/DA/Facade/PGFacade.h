#pragma once

#include "IDAFacade.h"
#include "Repositories/IFlightRepository.h"
class PGFacade : public IDAFacade
{
public:
    PGFacade(const IFlightRepositoryPtr& FlightRepository);
    virtual Flight GetFlightByFlightNumber( const std::string & number ) override;
    virtual std::vector<Flight> GetFlights( ) override;

private:
    IFlightRepositoryPtr m_FlightRepository;
};
