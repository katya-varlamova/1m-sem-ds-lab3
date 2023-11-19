#include "OatppServer.h"

void OatppServer::run( ) {
    oatpp::base::Environment::init();
    OatppServerSettings components;
    auto router = components.httpRouter.getObject();

    router->addController(OatppServerController::createShared());

    oatpp::network::Server server(
            components.serverConnectionProvider.getObject(),
            components.serverConnectionHandler.getObject());

    server.run();
    oatpp::base::Environment::destroy();
}