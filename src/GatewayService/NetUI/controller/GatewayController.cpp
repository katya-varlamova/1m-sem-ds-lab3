#include "GatewayController.hpp"

std::shared_ptr<BonusService> GatewayController::bonusService;
std::shared_ptr<FlightService> GatewayController::flightService;
std::shared_ptr<TicketService> GatewayController::ticketService;
std::map<std::string, ICircuitBreakerPtr> GatewayController::circuits;
std::map<std::string, std::mutex> GatewayController::locks;