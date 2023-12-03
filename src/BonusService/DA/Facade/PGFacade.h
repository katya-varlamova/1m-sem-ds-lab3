#pragma once

#include "IDAFacade.h"
#include "Repositories/IBonusRepository.h"
class PGFacade : public IDAFacade
{
public:
    PGFacade(const IBonusRepositoryPtr& BonusRepository);
    BalanceResponse GetBalanceByUsername(const std::string &username) override;
    BuyResponse PerfromBuyOperation(const BuyRequest &request) override;
    void PerfromReturnOperation(const std::string &username, const std::string &ticket_uid) override;

private:
    IBonusRepositoryPtr m_BonusRepository;
};
