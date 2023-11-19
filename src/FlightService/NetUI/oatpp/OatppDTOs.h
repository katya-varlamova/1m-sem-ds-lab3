#pragma once
#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)"oatpp/codegen/DTO_define.hpp"

class FlightResponseDto: public oatpp::DTO {
    DTO_INIT(FlightResponseDto, DTO)
    DTO_FIELD(String, flightNumber);
    DTO_FIELD(Int64, price);
    DTO_FIELD(String, fromAirport);
    DTO_FIELD(String, toAirport);
    DTO_FIELD(String, date);
};
class FlightsResponseDto: public oatpp::DTO {
    DTO_INIT(FlightsResponseDto, DTO)
    DTO_FIELD(Int64, page);
    DTO_FIELD(Int64, pageSize);
    DTO_FIELD(Int64, totalElements);
    DTO_FIELD(Vector<Object<FlightResponseDto>>, items);
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


