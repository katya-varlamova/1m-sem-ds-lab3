#pragma once

#include <config/base_config.h>
#include "structs/Bonus.h"
#include "IDAFactory.h"
class IBLFacade{
public:
    virtual BuyResponse PerfromBuyOperation( const BuyRequest &request ) = 0;
    virtual void PerfromReturnOperation( const std::string &username, const std::string &ticket_uid ) = 0;
    virtual BalanceResponse GetBalanceByUsername( const std::string &username ) = 0;
    virtual ~IBLFacade() = default;
};

using IBLFacadePtr = std::shared_ptr<IBLFacade>;


