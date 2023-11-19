#include "GatewayController.hpp"

std::shared_ptr<BonusService> GatewayController::bonusService;
std::shared_ptr<FlightService> GatewayController::flightService;
std::shared_ptr<TicketService> GatewayController::ticketService;