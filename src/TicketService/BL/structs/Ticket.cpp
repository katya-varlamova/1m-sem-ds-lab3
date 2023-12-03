#include "Ticket.h"
Ticket::Ticket(
        int id,
        const std::string& ticket_uid,
        const std::string& username,
        const std::string& flight_number,
        int price,
        const std::string& status
)
        : id( id )
        , ticket_uid( ticket_uid )
        , username( username )
        , flight_number( flight_number )
        , price( price )
        , status( status )
{
}