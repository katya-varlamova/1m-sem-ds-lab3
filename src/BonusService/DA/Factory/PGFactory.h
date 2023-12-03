#pragma once

#include <boost/di.hpp>

#include "Connection/PGConnection.h"
#include "Facade/PGFacade.h"
#include "IDAFactory.h"
#include "Repositories/PGBonusRepository.h"

class PGDAFactory : public IDAFactory
{
public:
    BOOST_DI_INJECT( PGDAFactory, const std::shared_ptr<BaseConfig>& config )
            : m_config( config )
    {}
    IDAFacadePtr CreateDAFacade() override;
    ~PGDAFactory() = default;

protected:
    IBonusRepositoryPtr CreateBonusRepository();

private:
    std::shared_ptr<BaseConfig> m_config;
    IBonusRepositoryPtr m_BonusRepository;
    IDAFacadePtr m_idaFacade;
};
