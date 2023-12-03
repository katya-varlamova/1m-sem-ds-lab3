#pragma once
#include <chrono>
#include <atomic>
#include <chrono>
#include "ICircuitBreaker.h"

class CircuitBreaker : public ICircuitBreaker{
public:
    CircuitBreaker(int failureThreshold, int retryTimeout)
            : failureThreshold_(failureThreshold), retryTimeout_(retryTimeout) {}
    void onSuccess() override ;
    bool isAvailable() override ;
    void onFailure() override ;

private:
    bool isRetryTimeoutExceeded() {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - openTime_).count();
        return elapsed >= retryTimeout_;
    }

    int failureThreshold_;
    int failureCount_ = 0;
    int retryTimeout_;
    bool open = false;
    std::chrono::time_point<std::chrono::steady_clock> openTime_;
};
