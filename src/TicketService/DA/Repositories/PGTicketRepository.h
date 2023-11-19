#pragma once
#include <cstring>
#include <map>
#include <memory>
#include <string>

#include "Connection/PGConnection.h"
#include "ITicketRepository.h"

#include "exceptions/database_exceptions.h"
#include "logger/LoggerFactory.h"
class PGTicketRepository : public ITicketRepository
{
public:
    PGTicketRepository( const PGConnectionCreatorPtr& connectionCreator )
    {
        m_conn = connectionCreator->GetConnect();
        m_connectionCreator = connectionCreator;
    }
    virtual Ticket GetTicketByUUID( const std::string & uuid ) override;
    std::vector<Ticket> GetTicketsByUsername(const std::string &username) override;
    virtual std::string CreateTicket( const Ticket& Ticket ) override;
    virtual void DeleteTicket(const std::string & uuid, const std::string &username ) override;

    ~PGTicketRepository() override
    {
        m_connectionCreator->FinishConnect( m_conn );
    }

protected:
    Ticket ParseTicket( std::map<std::string, std::string> strs );

private:
    PGConnectPtr m_conn;
    PGConnectionCreatorPtr m_connectionCreator;
};
