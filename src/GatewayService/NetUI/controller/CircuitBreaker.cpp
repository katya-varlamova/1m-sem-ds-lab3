#include "CircuitBreaker.h"
void CircuitBreaker::onSuccess()  {
    failureCount_ = 0;
}
bool CircuitBreaker::isAvailable()  {
    if (open && isRetryTimeoutExceeded()) {
        open = false;
    }
    return !open;
}
void CircuitBreaker::onFailure()  {
    if (!open) {
        failureCount_++;
        if (failureCount_ >= failureThreshold_) {
            open = true;
            openTime_ = std::chrono::steady_clock::now();
        }
    } else {
        openTime_ = std::chrono::steady_clock::now();
    }
}