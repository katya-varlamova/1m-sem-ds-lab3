#pragma once

#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/core/macro/codegen.hpp"


#include OATPP_CODEGEN_BEGIN(ApiClient)

class FlightService : public oatpp::web::client::ApiClient {
public:

  API_CLIENT_INIT(FlightService)
  API_CALL("GET", "/manage/health", HealthPoint )
  API_CALL("GET", "/api/v1/flights", FlightsGetPoint, QUERY(Int64, page), QUERY(Int64, size) )
  API_CALL("GET", "/api/v1/flights/{flight_number}", FlightGetPoint, PATH(String, flight_number))

};

#include OATPP_CODEGEN_END(ApiClient)