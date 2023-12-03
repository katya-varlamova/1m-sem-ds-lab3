#pragma once

#include "IBLFacade.h"
#include <boost/di.hpp>
#include "IUUIDGenerator.h"
class BLFacade : public IBLFacade {
public:
    BOOST_DI_INJECT( BLFacade, const std::shared_ptr<IDAFactory> &factory, const std::shared_ptr<IUUIDGenerator> &generator) {
        m_da_facade = factory->CreateDAFacade();
        uuidGenerator = generator;
    }
    virtual Flight GetFlightByFlightNumber( const std::string & number ) override;
    virtual std::vector<Flight> GetFlights( ) override;
protected:
    IDAFacadePtr m_da_facade;
    IUUIDGeneratorPtr uuidGenerator;
};



