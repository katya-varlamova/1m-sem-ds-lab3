#pragma once

#include <memory>
#include <vector>
#include "structs/Ticket.h"

class ITicketRepository
{
public:
    virtual Ticket GetTicketByUUID( const std::string & uuid ) = 0;
    virtual std::vector<Ticket> GetTicketsByUsername( const std::string &username ) = 0;
    virtual std::string CreateTicket( const Ticket& Ticket ) = 0;
    virtual void DeleteTicket(const std::string & uuid, const std::string &username ) = 0;
    virtual void RollbackCreateTicket(const std::string & uuid, const std::string &username ) = 0;
    virtual ~ITicketRepository() = default;
};
using ITicketRepositoryPtr = std::shared_ptr<ITicketRepository>;