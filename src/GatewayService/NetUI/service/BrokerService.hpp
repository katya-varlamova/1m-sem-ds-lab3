#pragma once


#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "dto/BrokerDtos.hpp"
#include OATPP_CODEGEN_BEGIN(ApiClient)

class BrokerService : public oatpp::web::client::ApiClient {
public:

    API_CLIENT_INIT(BrokerService)
    API_CALL("POST", "/api/exchanges/%2F/amq.default/publish", PublishPoint, AUTHORIZATION_BASIC(String, authString), BODY_DTO(Object<PublishDto>, publishDto))

};

#include OATPP_CODEGEN_END(ApiClient)

