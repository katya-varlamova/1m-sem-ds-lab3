#include "PGFactory.h"
IDAFacadePtr
PGDAFactory::CreateDAFacade()
{
    if ( m_idaFacade )
        return m_idaFacade;

    return (
      m_idaFacade = IDAFacadePtr( new PGFacade(
        CreateBonusRepository()
      ) )
    );
}
IBonusRepositoryPtr
PGDAFactory::CreateBonusRepository()
{
    PGConnectionCreatorPtr connectionCreator =
      PGConnectionCreatorPtr( new PGConnection( m_config ) );
    return m_BonusRepository ? m_BonusRepository
                            : ( m_BonusRepository = IBonusRepositoryPtr(
                                  new PGBonusRepository( connectionCreator )
                                ) );
}
