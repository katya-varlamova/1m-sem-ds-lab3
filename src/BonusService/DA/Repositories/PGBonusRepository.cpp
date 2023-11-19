#include "PGBonusRepository.h"

BalanceResponse PGBonusRepository::GetBalanceByUsername(const std::string &username)
{
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );
    std::string query(
            "select id, balance, status from privilege where username = '" + username + "';"
    );
    PGresult* pgRes = PQexec( m_conn, query.c_str() );
    if ( PQresultStatus( pgRes ) != PGRES_TUPLES_OK ) {
        LoggerFactory::GetLogger()->LogWarning(
                ( std::string(
                        "GetBalanceByUsername: "
                ) +
                  PQresultErrorMessage( pgRes ) )
                        .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "No such user!"
        );
    }
    BalanceResponse response;
    std::map<std::string, std::string> res;
    for ( int i = 0; i < PQntuples( pgRes ); ++i ) {
        for (int j = 0; j < PQnfields(pgRes); ++j) {
            char *fn = PQfname(pgRes, j);
            char *val = PQgetvalue(pgRes, 0, j);
            res[std::string(fn)] = std::string(val);
        }
    }
    int id = std::stoi(res["id"]);
    response.status = res["status"];
    response.balance = std::stoi(res["balance"]);

    query = "select ticket_uid, datetime, balance_diff, operation_type "
            "from privilege_history "
            "where privilege_id = '" + std::to_string(id) + "';";
    pgRes = PQexec( m_conn, query.c_str() );

    if ( PQresultStatus( pgRes ) != PGRES_TUPLES_OK ) {
        LoggerFactory::GetLogger()->LogWarning(
                ( std::string(
                        "GetBalanceByUsername: "
                ) +
                  PQresultErrorMessage( pgRes ) )
                        .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "No such user!"
        );
    }


    for ( int i = 0; i < PQntuples( pgRes ); i++ ) {
        res.clear();
        HistoryRecord record;
        for (int j = 0; j < PQnfields(pgRes); ++j) {
            char *fn = PQfname(pgRes, j);
            char *val = PQgetvalue(pgRes, i, j);
            res[std::string(fn)] = std::string(val);
        }
        record.ticketUid = res["ticket_uid"];
        record.date = res["datetime"];
        record.balanceDiff = std::stoi(res["balance_diff"]);
        record.operationType = res["operation_type"];
        response.history.push_back(record);
    }
    return response;
}
BuyResponse PGBonusRepository::PerfromBuyOperation(const BuyRequest &request)
{
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );

    std::string query(
            "select id, balance, status "
            "from privilege "
            "where username = '" + request.username + "';"
    );
    PGresult* pgRes = PQexec( m_conn, query.c_str() );
    if ( PQresultStatus( pgRes ) != PGRES_TUPLES_OK ) {
        LoggerFactory::GetLogger()->LogWarning(
                ( std::string(
                        "GetBalanceByUsername: "
                ) +
                  PQresultErrorMessage( pgRes ) )
                        .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "No such user!"
        );
    }

    BuyResponse response;


    std::map<std::string, std::string> res;
    for ( int i = 0; i < PQntuples( pgRes ); ++i ) {
        for (int j = 0; j < PQnfields(pgRes); ++j) {
            char *fn = PQfname(pgRes, j);
            char *val = PQgetvalue(pgRes, 0, j);
            res[std::string(fn)] = std::string(val);
        }
    }
    int id = std::stoi(res["id"]);
    response.status = res["status"];
    response.balance = std::stoi(res["balance"]);

    std::string op;
    int balanceDiff;
    if (request.paidFromBalance) {
        op = "DEBIT_THE_ACCOUNT";
        balanceDiff = -std::min(response.balance, request.price);
        response.paidByMoney = request.price + balanceDiff;
        response.paidByBonuses = -balanceDiff;
    }
    else {
        op = "FILL_IN_BALANCE";
        balanceDiff = int(request.price * 0.1);
        response.paidByMoney = request.price;
        response.paidByBonuses = 0;
    }
    response.balance += balanceDiff;
    query = "INSERT INTO privilege_history (privilege_id, ticket_uid, datetime, balance_diff, operation_type) VALUES ("
            + std::to_string(id) + ", " +
            "'" + request.ticket_uid + "', " +
            "(select NOW()), " +
            std::to_string(balanceDiff) + ", " +
            "'" + op + "');" +
            "UPDATE privilege set balance = " + std::to_string(response.balance) +" where id = " + std::to_string(id) + ";";

    pgRes = PQexec( m_conn, query.c_str() );
    if ( PQresultStatus( pgRes ) != PGRES_COMMAND_OK ) {
        LoggerFactory::GetLogger()->LogWarning(
                ( std::string(
                        "PerfromBuyOperation: "
                ) +
                  PQresultErrorMessage( pgRes ) )
                        .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "Error with buy ticket operation!"
        );
    }
    return response;
}
void PGBonusRepository::PerfromReturnOperation(const std::string &username, const std::string &ticket_uid)
{
    m_connectionCreator->CheckConnectAndReopenIfNeeded( m_conn );

    std::string query(
            "select privilege.id, balance, balance_diff, operation_type "
            "from privilege join privilege_history on privilege_history.privilege_id = privilege.id "
            "where username = '" + username + "' and ticket_uid = '" + ticket_uid + "';"
    );
    PGresult* pgRes = PQexec( m_conn, query.c_str() );
    if ( PQresultStatus( pgRes ) != PGRES_TUPLES_OK ) {
        LoggerFactory::GetLogger()->LogWarning(
                ( std::string(
                        "PerfromReturnOperation: "
                ) +
                  PQresultErrorMessage( pgRes ) )
                        .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "No such history info or user!");
    }
    if (PQntuples( pgRes ) != 1)
    {
        LoggerFactory::GetLogger()->LogWarning(
                ( std::string(
                        "PerfromReturnOperation: no such active ticket") )
                        .c_str());
        throw DatabaseExecutionException( "PerfromReturnOperation: no such active ticket");
    }

    std::map<std::string, std::string> res;
    for (int j = 0; j < PQnfields(pgRes); ++j) {
        char *fn = PQfname(pgRes, j);
        char *val = PQgetvalue(pgRes, 0, j);
        res[std::string(fn)] = std::string(val);
    }
    int id = std::stoi(res["id"]);
    int oldBalanceDiff = std::stoi(res["balance_diff"]);
    std::string oldOperationType = res["operation_type"];
    int oldBalance = std::stoi(res["balance"]);

    int newBalanceDiff;
    std::string newOperationType;
    if (oldOperationType == "DEBIT_THE_ACCOUNT") {
        newOperationType = "FILL_IN_BALANCE";
        newBalanceDiff = -oldBalanceDiff;
    } else {
        newOperationType = "DEBIT_THE_ACCOUNT";
        newBalanceDiff = -std::min(oldBalanceDiff, oldBalance);
    }

    int newBalance = oldBalance + newBalanceDiff;
    query = "INSERT INTO privilege_history (privilege_id, ticket_uid, datetime, balance_diff, operation_type) VALUES ("
            + std::to_string(id) + ", " +
            "'" + ticket_uid + "', " +
            "(select NOW()), " +
            std::to_string(newBalanceDiff) + ", " +
            "'" + newOperationType + "');" +
            "UPDATE privilege set balance = " + std::to_string(newBalance) +" where id = " + std::to_string(id) + ";";
    pgRes = PQexec( m_conn, query.c_str() );
    if ( PQresultStatus( pgRes ) != PGRES_COMMAND_OK ) {
        LoggerFactory::GetLogger()->LogWarning(
                ( std::string(
                        "PerfromReturnOperation: "
                ) +
                  PQresultErrorMessage( pgRes ) )
                        .c_str()
        );
        PQclear( pgRes );
        throw DatabaseExecutionException( "Error with return ticket operation!"
        );
    }

}
