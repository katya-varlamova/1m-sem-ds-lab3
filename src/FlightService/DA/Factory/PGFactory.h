#pragma once

#include <boost/di.hpp>

#include "Connection/PGConnection.h"
#include "Facade/PGFacade.h"
#include "IDAFactory.h"
#include "Repositories/PGFlightRepository.h"

class PGDAFactory : public IDAFactory
{
public:
    BOOST_DI_INJECT( PGDAFactory, const std::shared_ptr<BaseConfig>& config )
            : m_config( config )
    {}
    IDAFacadePtr CreateDAFacade() override;
    ~PGDAFactory() = default;

protected:
    IFlightRepositoryPtr CreateFlightRepository();

private:
    std::shared_ptr<BaseConfig> m_config;
    IFlightRepositoryPtr m_FlightRepository;
    IDAFacadePtr m_idaFacade;
};
