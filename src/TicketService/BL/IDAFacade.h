#pragma once

#include <config/base_config.h>
#include <structs/Ticket.h>

class IDAFacade
{
public:
    virtual Ticket GetTicketByUUID( const std::string & uuid ) = 0;
    virtual std::vector<Ticket> GetTicketsByUsername( const std::string &username ) = 0;
    virtual std::string CreateTicket( const Ticket& Ticket ) = 0;
    virtual void DeleteTicket(const std::string & uuid, const std::string &username ) = 0;

    virtual ~IDAFacade() = default;
};

using IDAFacadePtr = std::shared_ptr<IDAFacade>;