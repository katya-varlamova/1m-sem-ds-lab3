#pragma once

class IUUIDGenerator {
public:
    virtual std::string GenerateUUID() = 0;
};
using IUUIDGeneratorPtr = std::shared_ptr<IUUIDGenerator>;


