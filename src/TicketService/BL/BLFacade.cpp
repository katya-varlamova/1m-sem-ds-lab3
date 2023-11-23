#include "BLFacade.h"
std::string BLFacade::CreateTicket(const Ticket &ticket) {
    Ticket t = ticket;
    t.ticket_uid = uuidGenerator->GenerateUUID();
    return m_da_facade->CreateTicket(t);
}

Ticket BLFacade::GetTicketByUUID(const std::string & uuid) {
    return m_da_facade->GetTicketByUUID(uuid);
}

std::vector<Ticket> BLFacade::GetTicketsByUsername(const std::string &username) {
    return m_da_facade->GetTicketsByUsername(username);
}

void BLFacade::DeleteTicket(const std::string & uuid, const std::string &username )
{
    m_da_facade->DeleteTicket(uuid, username);
}
void BLFacade::RollbackCreateTicket(const std::string &uuid, const std::string &username) {
    m_da_facade->RollbackCreateTicket(uuid, username);
}
