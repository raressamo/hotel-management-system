#ifndef HOTEL_H
#define HOTEL_H

#include <iostream>
#include "Room.h"
#include "Guest.h"
#include "Reservation.h"

class Hotel {
private:
    static int totalHotels;
    const int hotelId;

    char* name; //numele hotelului
    int noRooms; // numarul de camere din hotel
    Room* rooms; // array de camere
    int noGuests; // numarul de oaspeti inregistrati
    Guest* guests; // array de oaspeti
    int noReservations; // numarul de rezervari
    Reservation* reservations; // array de rezervari
    double rating; // ratingul hotelului
    void copyName(const char* src);          // deep copy pentru numele hotelului
    void copyRooms(int n, Room* r);          // deep copy pentru array-ul de camere
    void copyGuests(int n, Guest* g);        // deep copy pentru array-ul de oaspeti
    void copyReservations(int n, Reservation* r); // deep copy pentru array-ul de rezervari
    int findRoomIndex(int roomId) const; // cauta indexul unei camere dupa id
    int findGuestIndex(int guestId) const; // cauta indexul unui oaspete
    int findReservationIndex(int reservationId) const; // cauta indexul unei rezervari

public:
    Hotel(); // constructor default
    Hotel(const char* name, double rating); // constructor cu parametri
    Hotel(const Hotel& obj); // copy constructor
    Hotel& operator=(const Hotel& obj); // operator=
    ~Hotel(); // destructor

    // getters
    int getHotelId() const;
    const char* getName() const;
    int getNoRooms() const;
    int getNoGuests() const;
    int getNoReservations() const;
    double getRating() const;
    static int getTotalHotels();

    // setters
    void setName(const char* name);
    void setRating(double rating);

    // functii externe
    friend std::ostream& operator<<(std::ostream& os, const Hotel& obj);
    friend std::istream& operator>>(std::istream& is, Hotel& obj);

    // functii de adaugare
    void addRoom(const Room& room); // adauga camera
    void addGuest(const Guest& guest); // adauga oaspete

    // functii publice
    void checkIn(int roomId, int guestId); // asociere oaspete camera
    void checkOut(int reservationId); // eliberare camera + calcul cost
    void getAvailableRooms() const; // afisare camere disponibile
    double getTotalRevenue() const; // calcul venit total al hotelului
    void findCheapestRoom() const; // cea mai ieftina camera disponibila
    void printGuests() const;
    void printReservations() const;
};

#endif