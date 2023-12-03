#pragma once

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class StringsDto:  public oatpp::DTO {
    DTO_INIT(StringsDto, DTO)
    DTO_FIELD(String, smth);
};

class PublishDto:  public oatpp::DTO {
    DTO_INIT(PublishDto, DTO)
    DTO_FIELD(Object<StringsDto>, properties);
    DTO_FIELD(String, routing_key);
    DTO_FIELD(String, payload);
    DTO_FIELD(String, payload_encoding);
};

#include OATPP_CODEGEN_END(DTO)

