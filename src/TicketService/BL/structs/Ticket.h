#pragma once

#include <string>

class Ticket {
public:
    Ticket() = default;
    Ticket(
            int id,
            const std::string& ticket_uid,
            const std::string& username,
            const std::string& flight_number,
            int price,
            const std::string& status
    );
    int id, price;
    std::string ticket_uid, username, flight_number, status;
    bool operator==( const Ticket& l ) const
    {
        return l.id == this->id && l.ticket_uid == this->ticket_uid &&
               l.username == this->username && l.flight_number == this->flight_number &&
               l.price == this->price && l.status == this->status ;
    }
};



