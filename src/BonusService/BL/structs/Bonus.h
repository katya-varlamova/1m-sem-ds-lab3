#pragma once

#include <string>
#include <vector>
class BuyRequest {
public:
    BuyRequest() = default;
//    BuyRequest(const std::string& username,
//                  const std::string& ticket_uid,
//                  bool paidFromBalance,
//                  int price) :
//                  username(username),
//                  ticket_uid(ticket_uid),
//                  paidFromBalance(paidFromBalance),
//                  price(price) {}
    std::string username, ticket_uid;
    bool paidFromBalance;
    int price;
};
class BuyResponse {
public:
    BuyResponse() = default;
//    BuyResponse(int balance,
//               const std::string& status,
//               int paidByMoney,
//               int paidByBonuses) :
//            balance(balance),
//            status(status),
//            paidByMoney(paidByMoney),
//            paidByBonuses(paidByBonuses) {}
    int balance;
    std::string status;
    int paidByMoney, paidByBonuses;
};

class HistoryRecord {
public:
    HistoryRecord() = default;
//    HistoryRecord(const std::string &ticketUid,
//                const std::string& date,
//                int balanceDiff,
//                const std::string &operationType) :
//            ticketUid(ticketUid),
//            date(date),
//            balanceDiff(balanceDiff),
//            operationType(operationType) {}
    std::string ticketUid, date;
    int balanceDiff;
    std::string  operationType;
};
class BalanceResponse {
public:
    BalanceResponse() = default;
//    BalanceResponse(int balance,
//                const std::string& status,
//                const std::vector<HistoryRecord> &history) :
//            balance(balance),
//            status(status),
//            history(history) {}
    int balance;
    std::string status;
    std::vector<HistoryRecord> history;
};




