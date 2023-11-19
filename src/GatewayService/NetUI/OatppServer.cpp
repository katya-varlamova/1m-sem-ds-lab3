
#include "OatppServer.hpp"

void OatppServer::run() {
    oatpp::base::Environment::init();
    AppComponent components(
            {"0.0.0.0", 8080},
            {"0.0.0.0", 8060},
            {"0.0.0.0", 8070},
            {"0.0.0.0", 8050}
    );

    GatewayController::bonusService = components.bonusService.getObject();
    GatewayController::flightService = components.flightService.getObject();
    GatewayController::ticketService = components.ticketService.getObject();

    auto router = components.httpRouter.getObject();

    router->addController(GatewayController::createShared());


    oatpp::network::Server server(
            components.serverConnectionProvider.getObject(),
            components.serverConnectionHandler.getObject());

    server.run();
    oatpp::base::Environment::destroy();

}

