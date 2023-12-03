#pragma once
#include <memory>
class IServer {
public:
    virtual void run() = 0;
};
using IServerPtr = std::shared_ptr<IServer>;