#pragma once

#include <string>

class Flight {
public:
    Flight() = default;
    Flight(const std::string& flight_number,
            const std::string& datetime,
            int price,
            const std::string& from_airport_name,
            const std::string& from_airport_city,
            const std::string& to_airport_name,
            const std::string& to_airport_city
    );

    std::string flight_number, datetime;
    int price;
    std::string  from_airport_name, from_airport_city, to_airport_name, to_airport_city;
    bool operator==( const Flight& l ) const
    {
        return l.flight_number == this->flight_number && l.datetime == this->datetime &&
               l.from_airport_city == this->from_airport_city && l.from_airport_name == this->from_airport_name &&
               l.price == this->price && l.to_airport_city == this->to_airport_city &&  l.to_airport_name == this->to_airport_name;
    }
};



