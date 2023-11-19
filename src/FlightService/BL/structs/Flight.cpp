#include "Flight.h"
Flight::Flight(const std::string& flight_number,
               const std::string& datetime,
               int price,
               const std::string& from_airport_name,
               const std::string& from_airport_city,
               const std::string& to_airport_name,
               const std::string& to_airport_city
)
        : flight_number( flight_number )
        , datetime( datetime )
        , price( price )
        , from_airport_name( from_airport_name )
        , from_airport_city( from_airport_city )
        , to_airport_name( to_airport_name )
        , to_airport_city( to_airport_city )
{
}