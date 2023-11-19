#pragma once

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

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

#include OATPP_CODEGEN_END(DTO)
