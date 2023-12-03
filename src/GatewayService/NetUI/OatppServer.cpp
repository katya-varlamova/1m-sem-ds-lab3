
#include "OatppServer.hpp"
void OatppServer::run() {
    oatpp::base::Environment::init();
    AppComponent components(
            {"0.0.0.0", 8080},
            {"flight-service", 8060},
            {"ticket-service", 8070},
            {"bonus-service", 8050},
            {"rabbitmq", 15672}
    );
//    boost::asio::io_context context(2);

    GatewayController::bonusService = components.bonusService.getObject();
    GatewayController::flightService = components.flightService.getObject();
    GatewayController::ticketService = components.ticketService.getObject();
    GatewayController::brokerService = components.brokerService.getObject();
    GatewayController::circuits[Qualifiers::SERVICE_FLIGHT] = ICircuitBreakerPtr(new CircuitBreaker(10, 10));
    GatewayController::circuits[Qualifiers::SERVICE_TICKET] = ICircuitBreakerPtr(new CircuitBreaker(10, 10));
    GatewayController::circuits[Qualifiers::SERVICE_BONUS] = ICircuitBreakerPtr(new CircuitBreaker(10, 10));
//    GatewayController::queue =  IQueueHandlerPtr (new RabbitMqQueueHandler(context, "cancel-bonus"));
    auto router = components.httpRouter.getObject();

    router->addController(GatewayController::createShared());


    oatpp::network::Server server(
            components.serverConnectionProvider.getObject(),
            components.serverConnectionHandler.getObject());

    server.run();
    oatpp::base::Environment::destroy();

}

