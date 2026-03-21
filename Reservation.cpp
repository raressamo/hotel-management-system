#include "Reservation.h"
#include <string.h>

int Reservation::totalReservations = 0;

// aloca memorie noua si copiaza sirul src in this->guestName
void Reservation::copyGuestName(const char* src) {
    this->guestName = new char[strlen(src) + 1];
    strcpy(this->guestName, src);
}

// aloca memorie noua si copiaza array-ul de servicii extra
void Reservation::copyExtras(int noExtras, float* extras) {
    this->noExtras = noExtras;
    if (noExtras > 0) {
        // alocam un array nou de aceeasi dimensiune
        this->extraCharges = new float[noExtras];
        // copiem fiecare element in parte
        for (int i = 0; i < noExtras; i++)
            this->extraCharges[i] = extras[i];
    } else {
        // daca nu sunt extras inseamna ca pointerul e null
        this->extraCharges = nullptr;
    }
}

// constructor default
Reservation::Reservation() : reservationId(++totalReservations) {
    this->roomNumber = 0;
    copyGuestName("-");
    this->noNights = 0;
    this->pricePerNight = 0.0f;
    this->extraCharges = nullptr; // la inceput nu sunt servicii extra
    this->noExtras = 0;
    this->isPaid = false; // by default nu e platit
}

// constructor cu parametri
Reservation::Reservation(int roomNumber, const char* guestName, int noNights, float pricePerNight)
    : reservationId(++totalReservations) {
    this->roomNumber = roomNumber;
    copyGuestName(guestName); // deep copy numele oaspetelui
    this->noNights = noNights;
    this->pricePerNight = pricePerNight;
    this->extraCharges = nullptr; // la creare nu sunt servicii extra
    this->noExtras = 0;
    this->isPaid = false;
}

// copy constructor
Reservation::Reservation(const Reservation& obj) : reservationId(++totalReservations) {
    this->roomNumber = obj.roomNumber;
    copyGuestName(obj.guestName);
    this->noNights = obj.noNights;
    this->pricePerNight = obj.pricePerNight;
    this->isPaid = obj.isPaid;
    copyExtras(obj.noExtras, obj.extraCharges);
}

// operator=
Reservation& Reservation::operator=(const Reservation& obj) {
    if (this == &obj)
        return *this;

    // prevenim memory leak-ul
    delete[] this->guestName;
    delete[] this->extraCharges;

    // copy toate datele
    this->roomNumber = obj.roomNumber;
    copyGuestName(obj.guestName);
    this->noNights = obj.noNights;
    this->pricePerNight = obj.pricePerNight;
    this->isPaid = obj.isPaid;
    copyExtras(obj.noExtras, obj.extraCharges);
    return *this; // ret referinta
}

// destructor
Reservation::~Reservation() {
    delete[] this->guestName;
    delete[] this->extraCharges; // delete[] pe nullptr e safe, in cazul in care nu exista extra
}

// getters
int Reservation::getReservationId() const { return reservationId; }
int Reservation::getRoomNumber() const { return roomNumber; }
const char* Reservation::getGuestName() const { return guestName; }
int Reservation::getNoNights() const { return noNights; }
float Reservation::getPricePerNight() const { return pricePerNight; }
bool Reservation::getIsPaid() const { return isPaid; }
int Reservation::getNoExtras() const { return noExtras; }

// getter static
int Reservation::getTotalReservations() { return totalReservations; }

// calculam totalul la cerere
float Reservation::getTotalCost() const {
    return calculateTotal();
}

// setters
void Reservation::setRoomNumber(int roomNumber) {
    this->roomNumber = roomNumber;
}

void Reservation::setGuestName(const char* guestName) {
    delete[] this->guestName;
    copyGuestName(guestName);
}

void Reservation::setNoNights(int noNights) {
    if (noNights <= 0) {
        std::cout << "Numarul de nopti trebuie sa fie pozitiv!\n";
        return;
    }
    this->noNights = noNights;
}

void Reservation::setPricePerNight(float pricePerNight) {
    if (pricePerNight <= 0) {
        std::cout << "Pretul trebuie sa fie pozitiv!\n";
        return;
    }
    this->pricePerNight = pricePerNight;
}

// operator<<
std::ostream& operator<<(std::ostream& os, const Reservation& obj) {
    os << "  [Rezervare #" << obj.reservationId << "]\n";
    os << "  Camera       : " << obj.roomNumber << "\n";
    os << "  Oaspete      : " << obj.guestName << "\n";
    os << "  Nopti        : " << obj.noNights << "\n";
    os << "  Pret/noapte  : " << obj.pricePerNight << " RON\n";

    // afisam serviciile extra daca exista
    if (obj.noExtras > 0) {
        os << "  Servicii extra: ";
        for (int i = 0; i < obj.noExtras; i++)
            os << obj.extraCharges[i] << " RON  ";
        os << "\n";
    } else {
        os << "  Servicii extra: niciunul\n";
    }
    os << "  Total        : " << obj.calculateTotal() << " RON\n";
    os << "  Status plata : " << (obj.isPaid ? "Platita" : "Neplatita") << "\n";
    return os;
}

// operator>>
std::istream& operator>>(std::istream& is, Reservation& obj) {
    std::cout << "  Numar camera: ";
    is >> obj.roomNumber;
    std::cout << "  Nume oaspete: ";
    char buf[100];//buf temporar
    is >> buf;
    obj.setGuestName(buf);

    std::cout << "  Numar nopti: ";
    is >> obj.noNights;

    std::cout << "  Pret pe noapte: ";
    is >> obj.pricePerNight;

    // isPaid si extraCharges sunt setate automat
    obj.isPaid = false;
    obj.noExtras = 0;
    obj.extraCharges = nullptr;
    return is;
}

// functii publice
// addExtraCharge citeste array dinamic cu element nou (addDeposit)
void Reservation::addExtraCharge(float amount) {
    if (amount <= 0) {
        std::cout << "Suma trebuie sa fie pozitiva!\n";
        return;
    }
    // array cu elem +1
    float* newExtras = new float[noExtras + 1];

    // array vechi
    for (int i = 0; i < noExtras; i++)
        newExtras[i] = extraCharges[i];

    // elementul +1
    newExtras[noExtras] = amount;
    // eliberam anti memory leak arrayul vechi
    delete[] extraCharges;

    //inlocuire
    extraCharges = newExtras;
    noExtras++;
}

// marcheaza rezervarea ca platita
void Reservation::pay() {
    if (isPaid) {
        std::cout << "Rezervarea #" << reservationId << " este deja platita!\n";
        return;
    }
    isPaid = true;
    std::cout << "Rezervarea #" << reservationId << " a fost platita. Total: "
              << calculateTotal() << " RON\n";
}

// costul total al rezervarii
float Reservation::calculateTotal() const {
    // costul de baza
    float total = noNights * pricePerNight;

    // servicii extra
    for (int i = 0; i < noExtras; i++)
        total += extraCharges[i];

    return total; // doar calcul, nu modific rezervarea
}