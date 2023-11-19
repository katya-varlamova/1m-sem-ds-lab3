#pragma once

#include <config/base_config.h>
#include <structs/Bonus.h>

class IDAFacade
{
public:
    virtual BuyResponse PerfromBuyOperation( const BuyRequest &request ) = 0;
    virtual void PerfromReturnOperation( const std::string &username, const std::string &ticket_uid ) = 0;
    virtual BalanceResponse GetBalanceByUsername( const std::string &username ) = 0;

    virtual ~IDAFacade() = default;
};

using IDAFacadePtr = std::shared_ptr<IDAFacade>;