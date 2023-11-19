#pragma once

#include <memory>
#include <vector>
#include "structs/Bonus.h"

class IBonusRepository
{
public:
    virtual BuyResponse PerfromBuyOperation( const BuyRequest &request ) = 0;
    virtual void PerfromReturnOperation( const std::string &username, const std::string &ticket_uid ) = 0;
    virtual BalanceResponse GetBalanceByUsername( const std::string &username ) = 0;
    virtual ~IBonusRepository() = default;
};
using IBonusRepositoryPtr = std::shared_ptr<IBonusRepository>;