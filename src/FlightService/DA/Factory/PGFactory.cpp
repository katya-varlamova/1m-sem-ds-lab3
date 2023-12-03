#include "PGFactory.h"
IDAFacadePtr
PGDAFactory::CreateDAFacade()
{
    if ( m_idaFacade )
        return m_idaFacade;

    return (
      m_idaFacade = IDAFacadePtr( new PGFacade(
        CreateFlightRepository()
      ) )
    );
}
IFlightRepositoryPtr
PGDAFactory::CreateFlightRepository()
{
    PGConnectionCreatorPtr connectionCreator =
      PGConnectionCreatorPtr( new PGConnection( m_config ) );
    return m_FlightRepository ? m_FlightRepository
                            : ( m_FlightRepository = IFlightRepositoryPtr(
                                  new PGFlightRepository( connectionCreator )
                                ) );
}
