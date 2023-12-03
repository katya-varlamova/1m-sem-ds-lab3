#pragma once

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class ValidationErrorResponse: public oatpp::DTO {
    DTO_INIT(ValidationErrorResponse, DTO)

    DTO_FIELD(String, message);
    DTO_FIELD(Vector<String>, errors);
};
class ErrorResponse: public oatpp::DTO {
    DTO_INIT(ErrorResponse, DTO)

    DTO_FIELD(String, message);
};

#include OATPP_CODEGEN_END(DTO)
