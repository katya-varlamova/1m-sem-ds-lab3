#include "PGFacade.h"
PGFacade::PGFacade(
  const ITicketRepositoryPtr& TicketRepository
) : m_TicketRepository( TicketRepository )
{
}
std::string PGFacade::CreateTicket(const Ticket &Ticket) {
    return m_TicketRepository->CreateTicket(Ticket);
}

Ticket PGFacade::GetTicketByUUID(const std::string & uuid) {
    return m_TicketRepository->GetTicketByUUID(uuid);
}

std::vector<Ticket> PGFacade::GetTicketsByUsername(const std::string &username) {
    return m_TicketRepository->GetTicketsByUsername(username);
}

void PGFacade::DeleteTicket(const std::string &uuid, const std::string &username )
{
    m_TicketRepository->DeleteTicket(uuid, username);
}
void PGFacade::RollbackCreateTicket(const std::string &uuid, const std::string &username)
{
    m_TicketRepository->RollbackCreateTicket(uuid, username);
}
