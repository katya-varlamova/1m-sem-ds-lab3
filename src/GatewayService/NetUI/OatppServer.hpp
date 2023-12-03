#pragma once

#include "oatpp/web/server/api/ApiController.hpp"
#include "AppComponent.hpp"
#include "controller/GatewayController.hpp"
//#include "controller/RabbitMqQueueHandler.h"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/network/Server.hpp"

class OatppServer {
public:
  void run();

};
