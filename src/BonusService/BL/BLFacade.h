#pragma once

#include "IBLFacade.h"
#include <boost/di.hpp>
class BLFacade : public IBLFacade {
public:
    BOOST_DI_INJECT( BLFacade, const std::shared_ptr<IDAFactory> &factory) {
        m_da_facade = factory->CreateDAFacade();
    }
    BalanceResponse GetBalanceByUsername(const std::string &username) override;
    BuyResponse PerfromBuyOperation(const BuyRequest &request) override;
    void PerfromReturnOperation(const std::string &username, const std::string &ticket_uid) override;
protected:
    IDAFacadePtr m_da_facade;
};



