#pragma once

#include <boost/di.hpp>

#include "Connection/PGConnection.h"
#include "Facade/PGFacade.h"
#include "IDAFactory.h"
#include "Repositories/PGTicketRepository.h"

class PGDAFactory : public IDAFactory
{
public:
    BOOST_DI_INJECT( PGDAFactory, const std::shared_ptr<BaseConfig>& config )
            : m_config( config )
    {}
    IDAFacadePtr CreateDAFacade() override;
    ~PGDAFactory() = default;

protected:
    ITicketRepositoryPtr CreateTicketRepository();

private:
    std::shared_ptr<BaseConfig> m_config;
    ITicketRepositoryPtr m_TicketRepository;
    IDAFacadePtr m_idaFacade;
};
