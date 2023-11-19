#pragma once
#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)"oatpp/codegen/DTO_define.hpp"
class BuyRequestDto:  public oatpp::DTO {
    DTO_INIT(BuyRequestDto, DTO)
    DTO_FIELD(String, ticketUid);
    DTO_FIELD(String, username);
    DTO_FIELD(Int64, price);
    DTO_FIELD(Boolean, paidFromBalance);
};
class BuyResponseDto:  public oatpp::DTO {
    DTO_INIT(BuyResponseDto, DTO)
    DTO_FIELD(Int64, balance);
    DTO_FIELD(String, status);
    DTO_FIELD(Int64, paidByMoney);
    DTO_FIELD(Int64, paidByBonuses);
};
class HistoryDto:  public oatpp::DTO {
    DTO_INIT(HistoryDto, DTO)
    DTO_FIELD(String, ticketUid);
    DTO_FIELD(String, date);
    DTO_FIELD(Int64, balanceDiff);
    DTO_FIELD(String, operationType);
};
class BalanceResponseDto:  public oatpp::DTO {
    DTO_INIT(BalanceResponseDto, DTO)
    DTO_FIELD(Int64, balance);
    DTO_FIELD(String, status);
    DTO_FIELD(Vector<Object<HistoryDto>>, history);
};
class ValidationErrorResponse: public oatpp::DTO {
    DTO_INIT(ValidationErrorResponse, DTO)

    DTO_FIELD(String, message);
    DTO_FIELD(Vector<String>, errors);
};
class ErrorResponse: public oatpp::DTO {
    DTO_INIT(ErrorResponse, DTO)

    DTO_FIELD(String, message);
};
#include OATPP_CODEGEN_END(DTO)"oatpp/codegen/DTO_undef.hpp"


