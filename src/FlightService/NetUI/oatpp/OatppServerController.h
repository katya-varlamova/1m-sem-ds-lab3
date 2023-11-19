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

    ENDPOINT_ASYNC("GET", "/api/v1/flights", FlightsGetPoint ) {

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

            try {

                auto flights = facade->GetFlights();
                auto flightsDto = FlightsResponseDto::createShared();
                flightsDto->page = std::min((flights.size() - 1) / pageSize + 1, (unsigned long )page);
                flightsDto->pageSize = pageSize;
                flightsDto->totalElements = flights.size();

                oatpp::Vector<oatpp::Object<FlightResponseDto>> dtoVector({});
                for (unsigned i = (flightsDto->page - 1) * pageSize; i < std::min((unsigned long )(flightsDto->page * pageSize), flights.size()); i++) {
                    auto flightDto = FlightResponseDto::createShared();
                    flightDto->fromAirport = flights[i].from_airport_city + " " + flights[i].from_airport_name;
                    flightDto->toAirport = flights[i].to_airport_city + " " + flights[i].to_airport_name;
                    flightDto->flightNumber = flights[i].flight_number;
                    flightDto->price = flights[i].price;
                    flightDto->date = flights[i].datetime;
                    dtoVector->push_back(flightDto);
                }
                flightsDto->items = dtoVector;
                return _return(controller->createDtoResponse(Status::CODE_200, flightsDto));
            } catch (DatabaseException) {
                return _return(controller->createResponse(Status::CODE_500));
            }
        }

    };
    ENDPOINT_ASYNC("GET", "/api/v1/flights/{flight_number}", FlightGetPoint ) {

    ENDPOINT_ASYNC_INIT(FlightGetPoint)
        Action act() override {
            std::string flight_number = request->getPathVariable("flight_number");
            if (flight_number.empty()){
                auto dto = ValidationErrorResponse::createShared();
                dto->message = "Invalid data";
                oatpp::Vector<String> errors({});
                errors->push_back("Invalid request header: no flight number provided");
                dto->errors = errors;
                return _return(controller->createDtoResponse(Status::CODE_400, dto));
            }
            try {
                auto flight = facade->GetFlightByFlightNumber(flight_number);
                auto flightDto = FlightResponseDto::createShared();
                flightDto->fromAirport = flight.from_airport_city + " " + flight.from_airport_name;
                flightDto->toAirport = flight.to_airport_city + " " + flight.to_airport_name;
                flightDto->flightNumber = flight.flight_number;
                flightDto->price = flight.price;
                flightDto->date = flight.datetime;

                auto response = controller->createDtoResponse(Status::CODE_200, flightDto);

                return _return(response);
            } catch (DatabaseException) {
                return _return(controller->createResponse(Status::CODE_500));
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

