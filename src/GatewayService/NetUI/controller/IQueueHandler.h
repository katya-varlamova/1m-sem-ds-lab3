#pragma once

#include <string>
#include <memory>

class IQueueHandler
{
public:
    virtual ~IQueueHandler() = default;

    virtual bool Publish(const std::string &message, const std::string& routingKey) = 0;
};

using IQueueHandlerPtr = std::shared_ptr<IQueueHandler>;



