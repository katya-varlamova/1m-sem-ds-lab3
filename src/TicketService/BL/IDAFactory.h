#pragma once

#include <memory>

#include "IDAFacade.h"
class IDAFactory
{
public:
    virtual IDAFacadePtr CreateDAFacade() = 0;
    virtual ~IDAFactory() = default;

};
using IDAFactoryPtr = std::shared_ptr<IDAFactory>;
