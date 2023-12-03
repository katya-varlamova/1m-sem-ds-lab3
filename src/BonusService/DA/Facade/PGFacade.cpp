#include "PGFacade.h"
PGFacade::PGFacade(
        const IBonusRepositoryPtr& bonusRepository
) : m_BonusRepository( bonusRepository )
{
}
BalanceResponse PGFacade::GetBalanceByUsername(const std::string &username)
{
    return m_BonusRepository->GetBalanceByUsername(username);
}
BuyResponse PGFacade::PerfromBuyOperation(const BuyRequest &request)
{
    return m_BonusRepository->PerfromBuyOperation(request);
}
void PGFacade::PerfromReturnOperation(const std::string &username, const std::string &ticket_uid)
{
    m_BonusRepository->PerfromReturnOperation(username, ticket_uid);
}