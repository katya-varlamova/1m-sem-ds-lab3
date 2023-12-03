#pragma once

#include "dto/ErrorsDtos.hpp"
#include "dto/BonusDtos.hpp"
#include "dto/FlightDtos.hpp"
#include <dto/TicketDtos.hpp>
#include <dto/GatewayDtos.h>
#include "logger/LoggerFactory.h"

#include "service/BonusService.hpp"
#include "service/FlightService.hpp"
#include "service/TicketService.hpp"
#include "service/BrokerService.hpp"
#include "oatpp/web/server/api/ApiController.hpp"

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include <iostream>
#include "CircuitBreaker.h"
#include "Constants.hpp"
#include <map>
#include "IQueueHandler.h"
#include OATPP_CODEGEN_BEGIN(ApiController) //<--- codegen begin

class GatewayController : public oatpp::web::server::api::ApiController {

protected:

    GatewayController(const std::shared_ptr<ObjectMapper>& objectMapper)
            : oatpp::web::server::api::ApiController(objectMapper)
    {}
    static std::map<std::string, std::mutex> locks;
public:
    static std::map<std::string, ICircuitBreakerPtr> circuits;
    static std::shared_ptr<BonusService> bonusService;
    static std::shared_ptr<FlightService> flightService;
    static std::shared_ptr<TicketService> ticketService;
    static std::shared_ptr<BrokerService> brokerService;

    static std::shared_ptr<GatewayController> createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>,
                                                                           objectMapper)) {
        return std::shared_ptr<GatewayController>(new GatewayController(objectMapper));
    }

    ENDPOINT_ASYNC("GET", "/api/v1/flights", FlightsGetPoint) {
    ENDPOINT_ASYNC_INIT(FlightsGetPoint)

        Action act() override {

            int page = std::stoi(request->getQueryParameter("page"));
            if (page < 1){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: wrong page");
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }

            int pageSize = std::stoi(request->getQueryParameter("size"));
            if (pageSize < 1 || pageSize > 100){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: wrong page size");
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }

            std::shared_ptr<oatpp::web::protocol::http::incoming::Response> response;
            locks[Qualifiers::SERVICE_FLIGHT].lock();

            if (!circuits[Qualifiers::SERVICE_FLIGHT]->isAvailable()) {
                locks[Qualifiers::SERVICE_FLIGHT].unlock();
                return _return(controller->createResponse(Status::CODE_503));
            }
            try {
                response = flightService->FlightsGetPoint(page, pageSize);
            } catch (...){
                circuits[Qualifiers::SERVICE_FLIGHT]->onFailure();
                locks[Qualifiers::SERVICE_FLIGHT].unlock();
                return _return(controller->createResponse(Status::CODE_503));
            }

            circuits[Qualifiers::SERVICE_FLIGHT]->onSuccess();

            locks[Qualifiers::SERVICE_FLIGHT].unlock();

            auto flights = response->readBodyToDto<oatpp::Object<FlightsResponseDto>>(
                    controller->getDefaultObjectMapper());
            return _return(controller->createDtoResponse(Status::CODE_200, flights));
        }

    };

    ENDPOINT_ASYNC("GET", "/api/v1/tickets/{ticketUid}", TicketGetPoint) {
    ENDPOINT_ASYNC_INIT(TicketGetPoint)
        Action act() override {
            std::string uuid = request->getPathVariable("ticketUid");
            if (uuid.empty()){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: no uuid provided");
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }

            std::shared_ptr<oatpp::web::protocol::http::incoming::Response> ticketResponse;
            locks[Qualifiers::SERVICE_TICKET].lock();

            if (!circuits[Qualifiers::SERVICE_TICKET]->isAvailable()) {
                locks[Qualifiers::SERVICE_TICKET].unlock();
                return _return(controller->createResponse(Status::CODE_503));
            }
            try {
                ticketResponse = ticketService->TicketGetPoint(uuid);
            } catch (...){
                circuits[Qualifiers::SERVICE_TICKET]->onFailure();
                locks[Qualifiers::SERVICE_TICKET].unlock();
                return _return(controller->createResponse(Status::CODE_503));
            }

            circuits[Qualifiers::SERVICE_TICKET]->onSuccess();

            locks[Qualifiers::SERVICE_TICKET].unlock();


            if (ticketResponse->getStatusCode() != 200)
            {
                if (ticketResponse->getStatusCode() == 404)
                    return _return(controller->createResponse(Status::CODE_404));
                return _return(controller->createResponse(Status::CODE_500));
            }
            auto ticket = ticketResponse->readBodyToDto<oatpp::Object<TicketResponseDto>>(controller->getDefaultObjectMapper());

            auto ticketDto = FullTicketResponseDto::createShared();
            ticketDto->flightNumber = ticket->flightNumber;
            ticketDto->ticketUid = ticket->ticketUid;
            ticketDto->status = ticket->status;
            ticketDto->price = ticket->price;
            ticketDto->date = "";
            ticketDto->fromAirport = "";
            ticketDto->toAirport = "";
            std::shared_ptr<oatpp::web::protocol::http::incoming::Response> flightResponse;

            locks[Qualifiers::SERVICE_FLIGHT].lock();

            if (circuits[Qualifiers::SERVICE_FLIGHT]->isAvailable()) {
                try {
                    flightResponse = flightService->FlightGetPoint(ticket->flightNumber);
                    circuits[Qualifiers::SERVICE_FLIGHT]->onSuccess();
                    if (flightResponse->getStatusCode() != 200)
                    {
                        return _return(controller->createResponse(Status::CODE_500));
                    }
                    auto flight = flightResponse->readBodyToDto<oatpp::Object<FlightResponseDto>>(controller->getDefaultObjectMapper());

                    ticketDto->date = flight->date;
                    ticketDto->fromAirport = flight->fromAirport;
                    ticketDto->toAirport = flight->toAirport;
                } catch (...) {
                    circuits[Qualifiers::SERVICE_FLIGHT]->onFailure();
                }
            }

            locks[Qualifiers::SERVICE_FLIGHT].unlock();


            return _return(controller->createDtoResponse(Status::CODE_200, ticketDto));

        }

    };

    ENDPOINT_ASYNC("GET", "/api/v1/tickets", TicketsGetPoint) {
    ENDPOINT_ASYNC_INIT(TicketsGetPoint)
        Action act() override {
            auto un = request->getHeader("X-User-Name");
            if (!un) {
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: no username provided");
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }
            std::shared_ptr<oatpp::web::protocol::http::incoming::Response> ticketResponse;
            locks[Qualifiers::SERVICE_TICKET].lock();

            if (!circuits[Qualifiers::SERVICE_TICKET]->isAvailable()) {
                locks[Qualifiers::SERVICE_TICKET].unlock();
                return _return(controller->createResponse(Status::CODE_503));
            }
            try {
                ticketResponse = ticketService->TicketsGetPoint(un);
            } catch (...){
                circuits[Qualifiers::SERVICE_TICKET]->onFailure();
                locks[Qualifiers::SERVICE_TICKET].unlock();
                return _return(controller->createResponse(Status::CODE_503));
            }

            circuits[Qualifiers::SERVICE_TICKET]->onSuccess();

            locks[Qualifiers::SERVICE_TICKET].unlock();

            auto tickets = ticketResponse->readBodyToDto<oatpp::Vector<oatpp::Object<TicketResponseDto>>>(
                    controller->getDefaultObjectMapper());
            oatpp::Vector<oatpp::Object<FullTicketResponseDto>> dtoVector({});
            for (const auto &t : *tickets) {

                auto ticketDto = FullTicketResponseDto::createShared();

                std::shared_ptr<oatpp::web::protocol::http::incoming::Response> flightResponse;

                locks[Qualifiers::SERVICE_FLIGHT].lock();

                if (circuits[Qualifiers::SERVICE_FLIGHT]->isAvailable()) {
                    try {
                        flightResponse = flightService->FlightGetPoint(t->flightNumber);
                        circuits[Qualifiers::SERVICE_FLIGHT]->onSuccess();
                        if (flightResponse->getStatusCode() != 200)
                        {
                            return _return(controller->createResponse(Status::CODE_500));
                        }
                        auto flight = flightResponse->readBodyToDto<oatpp::Object<FlightResponseDto>>(controller->getDefaultObjectMapper());

                        ticketDto->date = flight->date;
                        ticketDto->fromAirport = flight->fromAirport;
                        ticketDto->toAirport = flight->toAirport;
                    } catch (...) {
                        circuits[Qualifiers::SERVICE_FLIGHT]->onFailure();
                    }
                }

                locks[Qualifiers::SERVICE_FLIGHT].unlock();

                ticketDto->flightNumber = t->flightNumber;
                ticketDto->ticketUid = t->ticketUid;
                ticketDto->status = t->status;
                ticketDto->price = t->price;
                dtoVector->push_back(ticketDto);
            }
            return _return(controller->createDtoResponse(Status::CODE_200, dtoVector));
        }

    };

    ENDPOINT_ASYNC("GET", "/api/v1/privilege", PrivilegeGetPoint) {
    ENDPOINT_ASYNC_INIT(PrivilegeGetPoint)

        Action act() override {
            auto un = request->getHeader("X-User-Name");
            if (!un) {
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: no username provided");
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }

            std::shared_ptr<oatpp::web::protocol::http::incoming::Response> bonusResponse;
            locks[Qualifiers::SERVICE_BONUS].lock();

            if (!circuits[Qualifiers::SERVICE_BONUS]->isAvailable()) {
                locks[Qualifiers::SERVICE_BONUS].unlock();
                auto dto = ErrorResponse::createShared();
                dto->message = "Bonus Service unavailable";
                return _return(controller->createDtoResponse(Status::CODE_502, dto));
            }
            try {
                bonusResponse = bonusService->BalanceGetPoint(un);
            } catch (...){
                circuits[Qualifiers::SERVICE_BONUS]->onFailure();
                locks[Qualifiers::SERVICE_BONUS].unlock();

                auto dto = ErrorResponse::createShared();
                dto->message = "Bonus Service unavailable";
                return _return(controller->createDtoResponse(Status::CODE_503, dto));
            }

            circuits[Qualifiers::SERVICE_BONUS]->onSuccess();

            locks[Qualifiers::SERVICE_BONUS].unlock();

            if (bonusResponse->getStatusCode() != 200) {
                return _return(controller->createResponse(Status::CODE_500));
            }
            auto bonus = bonusResponse->readBodyToDto<oatpp::Object<BalanceResponseDto>>(
                    controller->getDefaultObjectMapper());

            return _return(controller->createDtoResponse(Status::CODE_200, bonus));
        }

    };

    ENDPOINT_ASYNC("GET", "/api/v1/me", MeGetPoint) {
    ENDPOINT_ASYNC_INIT(MeGetPoint)

        Action act() override {
            auto un = request->getHeader("X-User-Name");
            if (!un) {
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: no username provided");
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }
            std::shared_ptr<oatpp::web::protocol::http::incoming::Response> bonusResponse;
            auto balancePartDto = BalanceStatusDto::createShared();
            auto meDto = UserInfoDto::createShared();
            locks[Qualifiers::SERVICE_BONUS].lock();
            bool pr = false;
            if (circuits[Qualifiers::SERVICE_BONUS]->isAvailable()) {
                try {
                    bonusResponse = bonusService->BalanceGetPoint(un);
                    circuits[Qualifiers::SERVICE_BONUS]->onSuccess();
                    if (bonusResponse->getStatusCode() != 200) {
                        return _return(controller->createResponse(Status::CODE_503));
                    }
                    auto bonus = bonusResponse->readBodyToDto<oatpp::Object<BalanceResponseDto>>(
                            controller->getDefaultObjectMapper());
                    balancePartDto->balance = bonus->balance;
                    balancePartDto->status = bonus->status;
                    meDto->privilege = balancePartDto;
                    pr = true;
                } catch (...) {
                    circuits[Qualifiers::SERVICE_BONUS]->onFailure();
                    meDto->privilege = nullptr;
                }
            }
            locks[Qualifiers::SERVICE_BONUS].unlock();


            std::shared_ptr<oatpp::web::protocol::http::incoming::Response> ticketResponse;
            locks[Qualifiers::SERVICE_TICKET].lock();

            if (!circuits[Qualifiers::SERVICE_TICKET]->isAvailable()) {
                locks[Qualifiers::SERVICE_TICKET].unlock();
                return _return(controller->createResponse(Status::CODE_503));
            }
            try {
                ticketResponse = ticketService->TicketsGetPoint(un);
            } catch (...){
                circuits[Qualifiers::SERVICE_TICKET]->onFailure();
                locks[Qualifiers::SERVICE_TICKET].unlock();
                return _return(controller->createResponse(Status::CODE_503));
            }

            circuits[Qualifiers::SERVICE_TICKET]->onSuccess();

            locks[Qualifiers::SERVICE_TICKET].unlock();

            auto tickets = ticketResponse->readBodyToDto<oatpp::Vector<oatpp::Object<TicketResponseDto>>>(
                    controller->getDefaultObjectMapper());
            oatpp::Vector<oatpp::Object<FullTicketResponseDto>> dtoVector({});
            for (const auto &t : *tickets) {

                auto ticketDto = FullTicketResponseDto::createShared();

                std::shared_ptr<oatpp::web::protocol::http::incoming::Response> flightResponse;

                locks[Qualifiers::SERVICE_FLIGHT].lock();

                if (circuits[Qualifiers::SERVICE_FLIGHT]->isAvailable()) {
                    try {
                        flightResponse = flightService->FlightGetPoint(t->flightNumber);
                        circuits[Qualifiers::SERVICE_FLIGHT]->onSuccess();
                        if (flightResponse->getStatusCode() != 200)
                        {
                            return _return(controller->createResponse(Status::CODE_500));
                        }
                        auto flight = flightResponse->readBodyToDto<oatpp::Object<FlightResponseDto>>(controller->getDefaultObjectMapper());

                        ticketDto->date = flight->date;
                        ticketDto->fromAirport = flight->fromAirport;
                        ticketDto->toAirport = flight->toAirport;
                    } catch (...) {
                        circuits[Qualifiers::SERVICE_FLIGHT]->onFailure();
                    }
                }

                locks[Qualifiers::SERVICE_FLIGHT].unlock();

                ticketDto->flightNumber = t->flightNumber;
                ticketDto->ticketUid = t->ticketUid;
                ticketDto->status = t->status;
                ticketDto->price = t->price;
                dtoVector->push_back(ticketDto);
            }



            meDto->tickets = dtoVector;

            if (!pr){
                auto ticketsJson = controller->getDefaultObjectMapper()->writeToString(dtoVector);
                oatpp::String json = "{\"tickets\":" + ticketsJson + ",\"privilege\":{}}";
                auto resp = controller->createResponse(Status::CODE_200, json);
                resp->putHeader("Content-Type", "application/json");
                return _return(resp);
            }
            return _return(controller->createDtoResponse(Status::CODE_200, meDto));
        }

    };

    ENDPOINT_ASYNC("POST", "/api/v1/tickets", BuyPoint) {
    ENDPOINT_ASYNC_INIT(BuyPoint)

        Action act() override {
            return request->readBodyToStringAsync().callbackTo(&BuyPoint::buyTicketResponse);
        }
        Action buyTicketResponse( const oatpp::String& str) {
            oatpp::Object<FullBuyRequestDto> body;
            try {
                body = controller->getDefaultObjectMapper()->readFromString<oatpp::Object<FullBuyRequestDto>>(str);
            } catch (const oatpp::parser::ParsingError& error){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request body: " + error.getMessage());
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }
            auto un = request->getHeader("X-User-Name");
            if (!un) {
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: no username provided");
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }
            auto fbrDto = FullBuyResponseDto::createShared();


            std::shared_ptr<oatpp::web::protocol::http::incoming::Response> flightResponse;
            locks[Qualifiers::SERVICE_FLIGHT].lock();

            if (!circuits[Qualifiers::SERVICE_FLIGHT]->isAvailable()) {
                locks[Qualifiers::SERVICE_FLIGHT].unlock();
                return _return(controller->createResponse(Status::CODE_503));
            }
            try {
                flightResponse = flightService->FlightGetPoint(body->flightNumber);
            } catch (...){
                circuits[Qualifiers::SERVICE_FLIGHT]->onFailure();
                locks[Qualifiers::SERVICE_FLIGHT].unlock();
                return _return(controller->createResponse(Status::CODE_503));
            }

            circuits[Qualifiers::SERVICE_FLIGHT]->onSuccess();

            locks[Qualifiers::SERVICE_FLIGHT].unlock();

            if (flightResponse->getStatusCode() != 200)
            {
                return _return(controller->createResponse(Status::CODE_500));
            }
            auto flight = flightResponse->readBodyToDto<oatpp::Object<FlightResponseDto>>(controller->getDefaultObjectMapper());


            auto ticketDto = TicketRequestDto::createShared();
            ticketDto->flightNumber = body->flightNumber;
            ticketDto->price = body->price;



            std::shared_ptr<oatpp::web::protocol::http::incoming::Response> ticketResponse;
            locks[Qualifiers::SERVICE_TICKET].lock();

            if (!circuits[Qualifiers::SERVICE_TICKET]->isAvailable()) {
                locks[Qualifiers::SERVICE_TICKET].unlock();
                return _return(controller->createResponse(Status::CODE_503));
            }
            try {
                ticketResponse = ticketService->TicketPostPoint(ticketDto, un);
            } catch (...){
                circuits[Qualifiers::SERVICE_TICKET]->onFailure();
                locks[Qualifiers::SERVICE_TICKET].unlock();
                return _return(controller->createResponse(Status::CODE_503));
            }

            circuits[Qualifiers::SERVICE_TICKET]->onSuccess();

            locks[Qualifiers::SERVICE_TICKET].unlock();


            if (ticketResponse->getStatusCode() != 200) {
                return _return(controller->createResponse(Status::CODE_500));
            }
            auto ticket = ticketResponse->readBodyToDto<oatpp::Object<TicketResponseDto>>(
                    controller->getDefaultObjectMapper());


            auto brDto = BuyRequestDto::createShared();
            brDto->username = un;
            brDto->ticketUid = ticket->ticketUid;
            brDto->price = body->price;
            brDto->paidFromBalance = body->paidFromBalance;

            std::shared_ptr<oatpp::web::protocol::http::incoming::Response> bonusResp;
            locks[Qualifiers::SERVICE_BONUS].lock();

            if (!circuits[Qualifiers::SERVICE_BONUS]->isAvailable()) {
                locks[Qualifiers::SERVICE_BONUS].unlock();

                ticketService->TicketDeletePoint( un, ticket->ticketUid);

                auto dto = ErrorResponse::createShared();
                dto->message = "Bonus Service unavailable";
                return _return(controller->createDtoResponse(Status::CODE_503, dto));
            }
            try {
                bonusResp = bonusService->PurchasePoint(brDto);
            } catch (...){
                circuits[Qualifiers::SERVICE_BONUS]->onFailure();
                locks[Qualifiers::SERVICE_BONUS].unlock();

                ticketService->TicketDeletePoint( un, ticket->ticketUid);

                auto dto = ErrorResponse::createShared();
                dto->message = "Bonus Service unavailable";
                return _return(controller->createDtoResponse(Status::CODE_503, dto));
            }

            circuits[Qualifiers::SERVICE_BONUS]->onSuccess();

            locks[Qualifiers::SERVICE_BONUS].unlock();

            if (bonusResp->getStatusCode() != 200) {
                ticketService->TicketDeletePoint(un, ticket->ticketUid);
                return _return(controller->createResponse(Status::CODE_500));
            }

            auto bonus = bonusResp->readBodyToDto<oatpp::Object<BuyResponseDto>>(
                    controller->getDefaultObjectMapper());

            fbrDto->price = flight->price;
            fbrDto->flightNumber = flight->flightNumber;
            fbrDto->fromAirport = flight->fromAirport;
            fbrDto->toAirport = flight->toAirport;
            fbrDto->status = ticket->status;
            fbrDto->ticketUid = ticket->ticketUid;
            fbrDto->date = flight->date;
            fbrDto->paidByBonuses = bonus->paidByBonuses;
            fbrDto->paidByMoney = bonus->paidByMoney;
            auto bs = BalanceStatusDto::createShared();
            bs->status = bonus->status;
            bs->balance = bonus->balance;
            fbrDto->privilege = bs;
            return _return(controller->createDtoResponse(Status::CODE_200, fbrDto));
        }

    };

    ENDPOINT_ASYNC("DELETE", "/api/v1/tickets/{ticketUid}", ReturnPoint) {
    ENDPOINT_ASYNC_INIT(ReturnPoint)

        Action act() override {
            std::string username = request->getHeader("X-User-Name") ? request->getHeader("X-User-Name") : "";
            std::string ticket_uid = request->getPathVariable("ticketUid") ? request->getPathVariable("ticketUid") : "";
            if (username.empty() || ticket_uid.empty()){
                auto dto = ErrorResponse::createShared();
                dto->message = "Invalid username or ticketUID";

                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }


            std::shared_ptr<oatpp::web::protocol::http::incoming::Response> ticketResp;
            locks[Qualifiers::SERVICE_TICKET].lock();

            if (!circuits[Qualifiers::SERVICE_TICKET]->isAvailable()) {
                locks[Qualifiers::SERVICE_TICKET].unlock();
                return _return(controller->createResponse(Status::CODE_503));
            }
            try {
                ticketResp = ticketService->TicketUpdatePoint(username, ticket_uid);
            } catch (...){
                circuits[Qualifiers::SERVICE_TICKET]->onFailure();
                locks[Qualifiers::SERVICE_TICKET].unlock();
                return _return(controller->createResponse(Status::CODE_503));
            }

            circuits[Qualifiers::SERVICE_TICKET]->onSuccess();

            locks[Qualifiers::SERVICE_TICKET].unlock();


            if (ticketResp->getStatusCode() != 200) {
                auto dto = ErrorResponse::createShared();
                dto->message = "Not found!";
                return _return(controller->createDtoResponse(Status::CODE_404, dto));
            }
            std::shared_ptr<oatpp::web::protocol::http::incoming::Response> bonusResp;
            try {
                bonusResp = bonusService->ReturnPoint(ticket_uid, username);
            } catch (...){
                    FILE * f = fopen("message.txt", "w");
                    fprintf(f, "%s,%s", username.c_str(), ticket_uid.c_str());
                    fclose(f);
                    system("python3 /app/build/main/publish.py");
                return _return(controller->createResponse(Status::CODE_204));
            }

            if (bonusResp->getStatusCode() != 200) {
                FILE * f = fopen("message.txt", "w");
                fprintf(f, "%s,%s", username.c_str(), ticket_uid.c_str());
                fclose(f);
                system("python3 /app/build/main/publish.py");
//                    auto publishDto = PublishDto::createShared();
//                    publishDto->properties = StringsDto::createShared();
//                    publishDto->properties->smth = "smth";
//                    publishDto->routing_key = "cancel-bonus";
//                    publishDto->payload = "aab";
//                    publishDto->payload_encoding = "string";
//                    LoggerFactory::GetLogger()->LogInfo("try to publish");
//                    auto resp = brokerService->PublishPoint("guest:guest", publishDto);
//                    LoggerFactory::GetLogger()->LogInfo((std::to_string(resp->getStatusCode()) + " -- published with code").c_str());
                return _return(controller->createResponse(Status::CODE_204));
            }
            return _return(controller->createResponse(Status::CODE_204));
        }

    };

    ENDPOINT_ASYNC("GET", "/manage/health", HealthPoint ) {
    ENDPOINT_ASYNC_INIT(HealthPoint)
        Action act() override {
            return _return(controller->createResponse(Status::CODE_200));
        };
    };
};

#include OATPP_CODEGEN_END(ApiController) //<--- codegen end
