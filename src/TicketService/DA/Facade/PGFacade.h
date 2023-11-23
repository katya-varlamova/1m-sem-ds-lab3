#pragma once

#include "IDAFacade.h"
#include "Repositories/ITicketRepository.h"
class PGFacade : public IDAFacade
{
public:
    PGFacade(const ITicketRepositoryPtr& TicketRepository);
    virtual Ticket GetTicketByUUID( const std::string & uuid ) override;
    std::vector<Ticket> GetTicketsByUsername(const std::string &username) override;
    virtual std::string CreateTicket( const Ticket& Ticket ) override;
    virtual void DeleteTicket(const std::string & uuid, const std::string &username ) override;
    void RollbackCreateTicket(const std::string &uuid, const std::string &username) override;
private:
    ITicketRepositoryPtr m_TicketRepository;
};
