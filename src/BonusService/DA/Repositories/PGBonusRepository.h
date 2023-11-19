#pragma once
#include <cstring>
#include <map>
#include <memory>
#include <string>

#include "Connection/PGConnection.h"
#include "IBonusRepository.h"

#include "exceptions/database_exceptions.h"
#include "logger/LoggerFactory.h"
class PGBonusRepository : public IBonusRepository
{
public:
    PGBonusRepository( const PGConnectionCreatorPtr& connectionCreator )
    {
        m_conn = connectionCreator->GetConnect();
        m_connectionCreator = connectionCreator;
    }
    BalanceResponse GetBalanceByUsername(const std::string &username) override;
    BuyResponse PerfromBuyOperation(const BuyRequest &request) override;
    void PerfromReturnOperation(const std::string &username, const std::string &ticket_uid) override;
private:
    PGConnectPtr m_conn;
    PGConnectionCreatorPtr m_connectionCreator;
};
