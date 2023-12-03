#pragma once
#pragma once
#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)


class FullTicketResponseDto: public oatpp::DTO {
    DTO_INIT(FullTicketResponseDto, DTO)
    DTO_FIELD(String, ticketUid);
    DTO_FIELD(Int64, price);
    DTO_FIELD(String, flightNumber);
    DTO_FIELD(String, status);
    DTO_FIELD(String, fromAirport);
    DTO_FIELD(String, toAirport);
    DTO_FIELD(String, date);
};
class FullTicketsResponseDto: public oatpp::DTO {
    DTO_INIT(FullTicketsResponseDto, DTO)
    DTO_FIELD(Vector<Object<FullTicketResponseDto>>, tickets);
};
class BalanceStatusDto: public oatpp::DTO {
    DTO_INIT(BalanceStatusDto, DTO)
    DTO_FIELD(Int64, balance);
    DTO_FIELD(String, status);
};
class UserInfoDto: public oatpp::DTO {
    DTO_INIT(UserInfoDto, DTO)
    DTO_FIELD(Vector<Object<FullTicketResponseDto>>, tickets);
    DTO_FIELD(Object<BalanceStatusDto>, privilege);

};
class FullBuyRequestDto: public oatpp::DTO {
    DTO_INIT(FullBuyRequestDto, DTO)
    DTO_FIELD(String, flightNumber);
    DTO_FIELD(Int64, price);
    DTO_FIELD(Boolean, paidFromBalance);
};

class FullBuyResponseDto: public oatpp::DTO {
    DTO_INIT(FullBuyResponseDto, DTO)
    DTO_FIELD(String, ticketUid);
    DTO_FIELD(String, flightNumber);
    DTO_FIELD(String, fromAirport);
    DTO_FIELD(String, toAirport);
    DTO_FIELD(String, date);
    DTO_FIELD(Int64, price);
    DTO_FIELD(String, status);
    DTO_FIELD(Int64, paidByMoney);
    DTO_FIELD(Int64, paidByBonuses);
    DTO_FIELD(Object<BalanceStatusDto>, privilege);
};

#include OATPP_CODEGEN_END(DTO)