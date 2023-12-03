#pragma once
#include <memory>
class ICircuitBreaker {
public:
    virtual void onSuccess() = 0;
    virtual bool isAvailable() = 0;
    virtual void onFailure() = 0;
};
using ICircuitBreakerPtr = std::shared_ptr<ICircuitBreaker>;