#pragma once

#include "service/TicketService.hpp"
#include "service/FlightService.hpp"
#include "service/BonusService.hpp"
#include "oatpp/web/server/AsyncHttpConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"

#include "oatpp/network/virtual_/server/ConnectionProvider.hpp"
#include "oatpp/network/virtual_/client/ConnectionProvider.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"
#include "oatpp/network/ConnectionPool.hpp"

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "oatpp/core/macro/component.hpp"
#include "Constants.hpp"

struct HostPort {
  oatpp::String host;
  v_uint16 port;
};

class AppComponent {
private:
  HostPort m_facade;
  HostPort m_flightService;
  HostPort m_ticketService;
  HostPort m_bonusService;
public:

  AppComponent(const HostPort& facade, const HostPort& flightService, const HostPort& ticketService, const HostPort& bonusService)
    : m_facade(facade)
    , m_flightService(flightService)
    , m_ticketService(ticketService)
    , m_bonusService(bonusService)
  {}

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor)([] {
        return std::make_shared<oatpp::async::Executor>(
                9 /* Data-Processing threads */,
                2 /* I/O threads */,
                1 /* Timer threads */
        );
    }());


  /**
   * Create "real-port" connection provider
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)(Qualifiers::SERVICE_FACADE, [this] {
    return oatpp::network::tcp::server::ConnectionProvider::createShared({m_facade.host, m_facade.port});
  }());

  /**
   * Create Router component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)(Qualifiers::SERVICE_FACADE, [] {
    return oatpp::web::server::HttpRouter::createShared();
  }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
        OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor); // get Async executor component
        return oatpp::web::server::AsyncHttpConnectionHandler::createShared(router, executor);
    }());


    /**
     * Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)(Qualifiers::SERVICE_FACADE, [] {
        auto serializerConfig = oatpp::parser::json::mapping::Serializer::Config::createShared();
        auto deserializerConfig = oatpp::parser::json::mapping::Deserializer::Config::createShared();
        deserializerConfig->allowUnknownFields = false;
        auto objectMapper = oatpp::parser::json::mapping::ObjectMapper::createShared(serializerConfig, deserializerConfig);
        return objectMapper;
    }());

  /**
   * Create FlightService component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<FlightService>, flightService)(Qualifiers::SERVICE_FACADE, [this] {

    OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper, Qualifiers::SERVICE_FACADE);

    std::shared_ptr<oatpp::network::ClientConnectionProvider> connectionProvider;

    connectionProvider = oatpp::network::tcp::client::ConnectionProvider::createShared({m_flightService.host, m_flightService.port});

    auto connectionPool = oatpp::network::ClientConnectionPool::createShared(connectionProvider, 10, std::chrono::seconds(5));

    auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared(connectionPool);
    return FlightService::createShared(requestExecutor, objectMapper);

  }());

  /**
   * Create BonusService component
   */
    OATPP_CREATE_COMPONENT(std::shared_ptr<BonusService>, bonusService)(Qualifiers::SERVICE_FACADE, [this] {

        OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper, Qualifiers::SERVICE_FACADE);

        std::shared_ptr<oatpp::network::ClientConnectionProvider> connectionProvider;

        connectionProvider = oatpp::network::tcp::client::ConnectionProvider::createShared({m_bonusService.host, m_bonusService.port});

        auto connectionPool = oatpp::network::ClientConnectionPool::createShared(connectionProvider, 10, std::chrono::seconds(5));

        auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared(connectionPool);
        return BonusService::createShared(requestExecutor, objectMapper);

    }());

    /**
     * Create TicketService component
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<TicketService>, ticketService)(Qualifiers::SERVICE_FACADE, [this] {

        OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper, Qualifiers::SERVICE_FACADE);

        std::shared_ptr<oatpp::network::ClientConnectionProvider> connectionProvider;

        connectionProvider = oatpp::network::tcp::client::ConnectionProvider::createShared({m_ticketService.host, m_ticketService.port});

        auto connectionPool = oatpp::network::ClientConnectionPool::createShared(connectionProvider, 10, std::chrono::seconds(5));

        auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared(connectionPool);
        return TicketService::createShared(requestExecutor, objectMapper);

    }());
};

