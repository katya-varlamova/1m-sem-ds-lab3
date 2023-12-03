#include "BLFacade.h"

BalanceResponse BLFacade::GetBalanceByUsername(const std::string &username)
{
    return m_da_facade->GetBalanceByUsername(username);
}
BuyResponse BLFacade::PerfromBuyOperation(const BuyRequest &request)
{
    return m_da_facade->PerfromBuyOperation(request);
}
void BLFacade::PerfromReturnOperation(const std::string &username, const std::string &ticket_uid)
{
    m_da_facade->PerfromReturnOperation(username, ticket_uid);
}