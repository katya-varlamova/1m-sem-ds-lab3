#pragma once

#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "dto/BonusDtos.hpp"
#include OATPP_CODEGEN_BEGIN(ApiClient)

class BonusService : public oatpp::web::client::ApiClient {
public:

  API_CLIENT_INIT(BonusService)
  API_CALL("GET", "/api/v1/balance", BalanceGetPoint, HEADER(String, username, "X-User-Name") )
  API_CALL("PATCH", "/api/v1/purchase", PurchasePoint, BODY_DTO(Object<BuyRequestDto>, request) )
  API_CALL("PATCH", "/api/v1/return", ReturnPoint, HEADER(String, ticket_uid, "ticket_uid"), HEADER(String, username, "X-User-Name"))

};

#include OATPP_CODEGEN_END(ApiClient)

