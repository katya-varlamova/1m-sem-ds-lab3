#pragma once
#include "IUUIDGenerator.h"
#include <boost/uuid/uuid_io.hpp>
#include <boost/di.hpp>
#include <boost/uuid/uuid_generators.hpp>

class BoostUUIDGenerator : public IUUIDGenerator {
public:
    BOOST_DI_INJECT( BoostUUIDGenerator) { }
    std::string GenerateUUID() override {
        return boost::uuids::to_string(boost::uuids::random_generator()());
    }
};



