#include "PGFactory.h"
IDAFacadePtr
PGDAFactory::CreateDAFacade()
{
    if ( m_idaFacade )
        return m_idaFacade;

    return (
      m_idaFacade = IDAFacadePtr( new PGFacade(
        CreateTicketRepository()
      ) )
    );
}
ITicketRepositoryPtr
PGDAFactory::CreateTicketRepository()
{
    PGConnectionCreatorPtr connectionCreator =
      PGConnectionCreatorPtr( new PGConnection( m_config ) );
    return m_TicketRepository ? m_TicketRepository
                            : ( m_TicketRepository = ITicketRepositoryPtr(
                                  new PGTicketRepository( connectionCreator )
                                ) );
}
