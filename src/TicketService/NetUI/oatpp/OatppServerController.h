#pragma once
#include "OatppDTOs.h"
#include <unistd.h>
#include <iostream>
#include <exceptions/database_exceptions.h>
#include <string>
#include <cstring>

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/core/utils/String.hpp"
#include "IBLFacade.h"
#include OATPP_CODEGEN_BEGIN(ApiController)"oatpp/codegen/ApiController_define.hpp"


class OatppServerController : public oatpp::web::server::api::ApiController {

protected:
    OatppServerController(const std::shared_ptr<ObjectMapper>& objectMapper)
            : oatpp::web::server::api::ApiController(objectMapper)
    {}
public:
    static IBLFacadePtr facade;
    static std::shared_ptr<OatppServerController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                                                      objectMapper)){
        return std::shared_ptr<OatppServerController>(new OatppServerController(objectMapper));
    }

    ENDPOINT_ASYNC("GET", "/api/v1/tickets", TicketsGetPoint ) {

    ENDPOINT_ASYNC_INIT(TicketsGetPoint)

        Action act() override {
            auto un = request->getHeader("X-User-Name");
            if (!un){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: no username provided");
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }

            try {
                auto tickets = facade->GetTicketsByUsername(un);
                oatpp::Vector<oatpp::Object<TicketResponseDto>> dtoVector({});
                for (const auto &t : tickets) {
                    auto TicketDto = TicketResponseDto::createShared();
                    TicketDto->ticketUid = t.ticket_uid;
                    TicketDto->flightNumber = t.flight_number;
                    TicketDto->price = t.price;
                    TicketDto->status = t.status;
                    dtoVector->push_back(TicketDto);
                }
                return _return(controller->createDtoResponse(Status::CODE_200, dtoVector));
            } catch (DatabaseException) {
                return _return(controller->createResponse(Status::CODE_500));
            }
        }

    };
    ENDPOINT_ASYNC("GET", "/api/v1/tickets/{uuid}", TicketGetPoint ) {

    ENDPOINT_ASYNC_INIT(TicketGetPoint)
        Action act() override {
            std::string uuid = request->getPathVariable("uuid");
            if (uuid.empty()){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: no uuid provided");
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }
            try {
                auto ticket = facade->GetTicketByUUID(uuid);
                auto TicketDto = TicketResponseDto::createShared();
                TicketDto->ticketUid = uuid;
                TicketDto->flightNumber = ticket.flight_number;
                TicketDto->price = ticket.price;
                TicketDto->status = ticket.status;
                auto response = controller->createDtoResponse(Status::CODE_200, TicketDto);

                return _return(response);
            } catch (DatabaseException) {
                return _return(controller->createResponse(Status::CODE_500));
            }
        }

    };
    ENDPOINT_ASYNC("POST", "/api/v1/tickets", TicketPostPoint ) {

    ENDPOINT_ASYNC_INIT(TicketPostPoint)
        Action act() override {
            return request->readBodyToStringAsync().callbackTo(&TicketPostPoint::returnTicketResponse);
        }
        Action returnTicketResponse( const oatpp::String& str) {
            oatpp::Object<TicketRequestDto> body;
            try {
                body = controller->getDefaultObjectMapper()->readFromString<oatpp::Object<TicketRequestDto>>(str);
            } catch (const oatpp::parser::ParsingError& error){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request body: " + error.getMessage());
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }
            auto un = request->getHeader("X-User-Name");
            if (!un){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: no username provided");
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }

            Ticket ticket(-1,
                     "",
                     un,
                     body->flightNumber,
                     std::stoi(std::to_string(body->price)),
                     "PAID");
            std::string uuid;
            try {
                uuid = facade->CreateTicket(ticket);
            } catch (const DatabaseException &err) {
                auto dto = ErrorResponse::createShared();
                dto->message = "Internal server error: " + err.GetInfo();
                return _return(controller->createDtoResponse(Status::CODE_500, dto));
            }
            auto TicketDto = TicketResponseDto::createShared();
            TicketDto->ticketUid = uuid;
            TicketDto->flightNumber = ticket.flight_number;
            TicketDto->price = ticket.price;
            TicketDto->status = "PAID";
            auto response = controller->createDtoResponse(Status::CODE_200, TicketDto);

            return _return(response);
        }

    };
    ENDPOINT_ASYNC("DELETE", "/api/v1/tickets/{uuid}", TicketDeletePoint ) {
    ENDPOINT_ASYNC_INIT(TicketDeletePoint)
        Action act() override {
            std::string uuid = request->getPathVariable("uuid");
            if (uuid.empty()){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: no uuid provided");
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }
            auto un = request->getHeader("X-User-Name");
            if (!un){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: no username provided");
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }
            try {
                facade->RollbackCreateTicket(uuid, un);
                return _return(controller->createResponse(Status::CODE_200));
            } catch (DatabaseException) {
                auto dto = ErrorResponse::createShared();
                dto->message = "Not found!";
                return _return(controller->createDtoResponse(Status::CODE_404, dto));
            }
        }

    };
    ENDPOINT_ASYNC("PATCH", "/api/v1/tickets/{uuid}", TicketUpdatePoint ) {
    ENDPOINT_ASYNC_INIT(TicketUpdatePoint)
        Action act() override {
            std::string uuid = request->getPathVariable("uuid");
            if (uuid.empty()){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: no uuid provided");
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }
            auto un = request->getHeader("X-User-Name");
            if (!un){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: no username provided");
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }
            try {
                facade->DeleteTicket(uuid, un);
                return _return(controller->createResponse(Status::CODE_200));
            } catch (DatabaseException) {
                auto dto = ErrorResponse::createShared();
                dto->message = "Not found!";
                return _return(controller->createDtoResponse(Status::CODE_404, dto));
            }
        }

    };

    ENDPOINT_ASYNC("GET", "/manage/health", HealthPoint ) {
    ENDPOINT_ASYNC_INIT(HealthPoint)
        Action act() override {
            return _return(controller->createResponse(Status::CODE_200));
        };
    };

};

#include OATPP_CODEGEN_BEGIN(ApiController)"oatpp/codegen/ApiController_define.hpp"

using OatppServerControllerPtr = std::shared_ptr<OatppServerController>;

