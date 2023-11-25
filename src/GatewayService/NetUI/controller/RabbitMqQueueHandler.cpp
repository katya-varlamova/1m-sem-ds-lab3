#include "RabbitMqQueueHandler.h"

#include <logger/LoggerFactory.h>
#include <exceptions/server_exceptions.h>
#include <config/base_sections.h>

RabbitMqQueueHandler::RabbitMqQueueHandler(const BaseConfigPtr &config, boost::asio::io_context &context, const std::string& queueName)
        : m_config(config), m_context(context), m_queueName(queueName), m_handler(context)
{
    Connect();
}

bool RabbitMqQueueHandler::Publish(const std::string &message, const std::string &routingKey)
{
    if (!m_channel->connected())
    {
        LoggerFactory::GetLogger()->LogError("queue channel is not connected");
        return false;
    }

    m_channel->bindQueue(m_queueName, m_queueName, routingKey);
    return m_channel->publish(m_queueName, routingKey, message);
}

void RabbitMqQueueHandler::Connect()
{
    if (m_connection && !m_connection->closed())
        return;

    std::string username = "username";//m_config->GetStringField({ BROKER_SECTION, BROKER_USER_SECTION });
    std::string password = "password";//m_config->GetStringField({ BROKER_SECTION, BROKER_USER_PASSWORD_SECTION });
    std::string host = "rabbitmq";//m_config->GetStringField({ BROKER_SECTION, BROKER_HOST_SECTION });
    std::ostringstream addressStream;
    addressStream << "amqp://" << username << ":" << password << "@" << host;
    AMQP::Address address(addressStream.str());
    m_connection = std::make_shared<AMQP::TcpConnection>(&m_handler, address);

    if (!m_connection->closed())
        LoggerFactory::GetLogger()->LogInfo("Connected to rabbit mq broker");
    else
        return;
//    else
//        throw BrokerConnectionException("can't connect to rabbit mq");

    m_channel = std::make_shared<AMQP::TcpChannel>(m_connection.get());

    m_channel->declareQueue(m_queueName).onSuccess([&]()
                                                   {
                                                       LoggerFactory::GetLogger()->LogInfo((m_queueName + std::string(": queue declared")).c_str());
                                                   }).onError([&](const char *msg)
                                                              {
                                                                  std::ostringstream oss;
                                                                  oss << m_queueName << ": failed declare queue: " << msg;
                                                                  LoggerFactory::GetLogger()->LogError(oss.str().c_str());
                                                              });

    m_channel->declareExchange(m_queueName, AMQP::fanout).onSuccess([&]()
                                                                    {
                                                                        LoggerFactory::GetLogger()->LogInfo((m_queueName + std::string(": exchange declared")).c_str());
                                                                    }).onError([&](const char *msg)
                                                                               {
                                                                                   std::ostringstream oss;
                                                                                   oss << m_queueName << ": failed declare exchange: " << msg;
                                                                                   LoggerFactory::GetLogger()->LogError(oss.str().c_str());
                                                                               });
}