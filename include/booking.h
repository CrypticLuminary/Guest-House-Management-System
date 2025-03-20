#ifndef BOOKING_H
#define BOOKING_H

#include <string>

class Booking {
public:
    Booking(int guestID, int roomNumber);
    void makeReservation();
    void cancelReservation();
};

#endif
