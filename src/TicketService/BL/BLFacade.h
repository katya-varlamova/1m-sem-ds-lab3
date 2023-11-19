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
    virtual Ticket GetTicketByUUID( const std::string & uuid ) override;
    std::vector<Ticket> GetTicketsByUsername(const std::string &username) override;
    virtual std::string CreateTicket( const Ticket& Ticket ) override;
    virtual void DeleteTicket(const std::string & uuid, const std::string &username ) override;
protected:
    IDAFacadePtr m_da_facade;
    IUUIDGeneratorPtr uuidGenerator;
};



