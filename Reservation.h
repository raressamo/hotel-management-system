#ifndef RESERVATION_H
#define RESERVATION_H

#include <iostream>

class Reservation {
private:
    static int totalReservations;
    const int reservationId;

    int roomNumber; // numarul camerei
    char* guestName; // numele oaspetelui alocat dinamic pe heap, dimensiunea variabila
    int noNights; // numarul de nopti
    float pricePerNight; // pretul camerei
    float* extraCharges; // servicii extra array
    int noExtras; // numarul de servicii extra
    bool isPaid; // rezervarea e platita sau nu
    void copyGuestName(const char* src); // helper privat pt deep copy
    void copyExtras(int noExtras, float* extras); // helper privat pentru extras

public:
    Reservation(); // constructor default
    Reservation(int roomNumber, const char* guestName, int noNights, float pricePerNight); // constructorul cu parametri
    Reservation(const Reservation& obj); // copy constructor
    Reservation& operator=(const Reservation& obj); // operator=
    ~Reservation(); // destructor

    // getters
    int getReservationId() const;
    int getRoomNumber() const;
    const char* getGuestName() const;
    int getNoNights() const;
    float getPricePerNight() const;
    float getTotalCost() const; // costul total
    bool getIsPaid() const;
    int getNoExtras() const;
    static int getTotalReservations();

    // setters
    void setRoomNumber(int roomNumber);
    void setGuestName(const char* guestName);
    void setNoNights(int noNights);
    void setPricePerNight(float pricePerNight);

    // functiile externe
    friend std::ostream& operator<<(std::ostream& os, const Reservation& obj);
    friend std::istream& operator>>(std::istream& is, Reservation& obj);

    // functii publice
    void addExtraCharge(float amount); // adauga un serviciu extra la rezervare
    void pay(); // marchez rezervarea ca platita
    float calculateTotal() const; // calculeaza costul total care este nopti * pret + extra
};

#endif