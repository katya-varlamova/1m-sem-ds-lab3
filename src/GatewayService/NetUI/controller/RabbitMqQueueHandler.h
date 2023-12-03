#pragma once

#include "IQueueHandler.h"

#include <config/base_config.h>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include <amqpcpp.h>
#include <amqpcpp/libboostasio.h>
#include <boost/asio/io_service.hpp>
class RabbitMqQueueHandler
        : public IQueueHandler
{
public:
    RabbitMqQueueHandler(boost::asio::io_context& context, const std::string& queueName);
    ~RabbitMqQueueHandler() = default;

    virtual bool Publish(const std::string &message, const std::string& routingKey) override;

private:
    void Connect();

    boost::asio::io_context &m_context;
    std::string m_queueName;

    AMQP::LibBoostAsioHandler m_handler;
    std::shared_ptr<AMQP::TcpConnection> m_connection;
    std::shared_ptr<AMQP::TcpChannel> m_channel;
};