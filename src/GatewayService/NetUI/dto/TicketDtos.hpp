#pragma once
#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class TicketRequestDto: public oatpp::DTO {
    DTO_INIT(TicketRequestDto, DTO)
    DTO_FIELD(Int64, price);
    DTO_FIELD(String, flightNumber);
};
class TicketResponseDto: public oatpp::DTO {
    DTO_INIT(TicketResponseDto, DTO)
    DTO_FIELD(String, ticketUid);
    DTO_FIELD(Int64, price);
    DTO_FIELD(String, flightNumber);
    DTO_FIELD(String, status);
};
class TicketsResponseDto: public oatpp::DTO {
    DTO_INIT(TicketsResponseDto, DTO)
    DTO_FIELD(Vector<Object<TicketResponseDto>>, tickets);
};

#include OATPP_CODEGEN_END(DTO)


