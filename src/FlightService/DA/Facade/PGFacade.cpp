#include "PGFacade.h"
PGFacade::PGFacade(
  const IFlightRepositoryPtr& FlightRepository
) : m_FlightRepository( FlightRepository )
{
}
Flight PGFacade::GetFlightByFlightNumber(const std::string &number) {
    return m_FlightRepository->GetFlightByFlightNumber(number);
}
std::vector<Flight> PGFacade::GetFlights() {
    return m_FlightRepository->GetFlights();
}
