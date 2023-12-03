#include "BLFacade.h"

Flight BLFacade::GetFlightByFlightNumber(const std::string &number) {
    return m_da_facade->GetFlightByFlightNumber(number);
}
std::vector<Flight> BLFacade::GetFlights() {
    return m_da_facade->GetFlights();
}
