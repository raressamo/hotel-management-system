#include "Hotel.h"
#include <string.h>

int Hotel::totalHotels = 0;

void Hotel::copyName(const char* src) {
    this->name = new char[strlen(src) + 1];
    strcpy(this->name, src);
}

void Hotel::copyRooms(int n, Room* r) {
    this->noRooms = n; // 
    if (n > 0) {
        this->rooms = new Room[n];
        for (int i = 0; i < n; i++)
            this->rooms[i] = r[i];
    } else {
        this->rooms = nullptr;
    }
}

void Hotel::copyGuests(int n, Guest* g) {
    this->noGuests = n;
    if (n > 0) {
        this->guests = new Guest[n];
        for (int i = 0; i < n; i++)
            this->guests[i] = g[i];
    } else {
        this->guests = nullptr;
    }
}

void Hotel::copyReservations(int n, Reservation* r) {
    this->noReservations = n;
    if (n > 0) {
        this->reservations = new Reservation[n];
        for (int i = 0; i < n; i++)
            this->reservations[i] = r[i];
    } else {
        this->reservations = nullptr;
    }
}

int Hotel::findRoomIndex(int roomId) const {
    for (int i = 0; i < noRooms; i++)
        if (rooms[i].getId() == roomId)
            return i;
    return -1;
}

int Hotel::findGuestIndex(int guestId) const {
    for (int i = 0; i < noGuests; i++)
        if (guests[i].getGuestId() == guestId)
            return i;
    return -1;
}

int Hotel::findReservationIndex(int reservationId) const {
    for (int i = 0; i < noReservations; i++)
        if (reservations[i].getReservationId() == reservationId)
            return i;
    return -1;
}

// constructor default
Hotel::Hotel() : hotelId(++totalHotels) {
    copyName("-");
    this->noRooms = 0;
    this->rooms = nullptr; // nu sunt camere by default
    this->noGuests = 0;
    this->guests = nullptr; // nu sunt oaspeti by default
    this->noReservations = 0;
    this->reservations = nullptr; // nu sunt nici rezervari by default
    this->rating = 0.0;
}

// constructor cu parametri
Hotel::Hotel(const char* name, double rating) : hotelId(++totalHotels) {
    copyName(name);
    this->noRooms = 0;
    this->rooms = nullptr;
    this->noGuests = 0;
    this->guests = nullptr;
    this->noReservations = 0;
    this->reservations = nullptr;
    this->rating = rating;
}

// copy constructor
Hotel::Hotel(const Hotel& obj) : hotelId(++totalHotels) {
    copyName(obj.name);
    this->rating = obj.rating;
    copyRooms(obj.noRooms, obj.rooms);
    copyGuests(obj.noGuests, obj.guests);
    copyReservations(obj.noReservations, obj.reservations);
}

// operator=
Hotel& Hotel::operator=(const Hotel& obj) {
    if (this == &obj)
        return *this;

    // anti memory leak
    delete[] this->name;
    delete[] this->rooms;
    delete[] this->guests;
    delete[] this->reservations;

    copyName(obj.name);
    this->rating = obj.rating;
    copyRooms(obj.noRooms, obj.rooms);
    copyGuests(obj.noGuests, obj.guests);
    copyReservations(obj.noReservations, obj.reservations);
    return *this;
}

// destructor
Hotel::~Hotel() {
    delete[] this->name;
    delete[] this->rooms;
    delete[] this->guests;
    delete[] this->reservations;
}

// getters
int Hotel::getHotelId() const { return hotelId; }
const char* Hotel::getName() const { return name; }
int Hotel::getNoRooms() const { return noRooms; }
int Hotel::getNoGuests() const { return noGuests; }
int Hotel::getNoReservations() const { return noReservations; }
double Hotel::getRating() const { return rating; }
int Hotel::getTotalHotels() { return totalHotels; }

// setters
void Hotel::setName(const char* name) {
    delete[] this->name;
    copyName(name);
}

void Hotel::setRating(double rating) {
    // rating intre 1 si 5 (pe google maps minim o stea)
    if (rating < 1.0 || rating > 5.0) {
        std::cout << "Rating-ul trebuie sa fie intre 1.0 si 5.0!\n";
        return;
    }
    this->rating = rating;
}

// operator<<
std::ostream& operator<<(std::ostream& os, const Hotel& obj) {
    os << "  [Hotel #" << obj.hotelId << "]\n";
    os << "  Nume         : " << obj.name << "\n";
    os << "  Rating       : " << obj.rating << "/5.0\n";
    os << "  Camere       : " << obj.noRooms << "\n";
    os << "  Oaspeti      : " << obj.noGuests << "\n";
    os << "  Rezervari    : " << obj.noReservations << "\n";
    return os;
}

// operator>>
std::istream& operator>>(std::istream& is, Hotel& obj) {
    std::cout << "  Nume hotel: ";
    char buf[100];
    is >> buf;
    obj.setName(buf);
    std::cout << "  Rating (1.0 - 5.0): ";
    is >> obj.rating;
    return is;
}

// adauga o camera noua in hotel
void Hotel::addRoom(const Room& room) {
    // array nou cu elem +1
    Room* newRooms = new Room[noRooms + 1];
    // ce e deja existent
    for (int i = 0; i < noRooms; i++)
        newRooms[i] = rooms[i];
    // camera noua
    newRooms[noRooms] = room;
    // stergem array vechi
    delete[] rooms;
    // inlocuim pointerul cu noul array
    rooms = newRooms;
    noRooms++;
}

// adauga oaspete nou in hotel
void Hotel::addGuest(const Guest& guest) {
    Guest* newGuests = new Guest[noGuests + 1];
    for (int i = 0; i < noGuests; i++)
        newGuests[i] = guests[i];
    newGuests[noGuests] = guest;
    delete[] guests;
    guests = newGuests;
    noGuests++;
}

// asociaza oaspete cu o camera si creeaza rezervare
void Hotel::checkIn(int roomId, int guestId) {
    int roomIdx = findRoomIndex(roomId);
    if (roomIdx == -1) {
        std::cout << "Camera cu ID " << roomId << " nu exista!\n";
        return;
    }

    int guestIdx = findGuestIndex(guestId);
    if (guestIdx == -1) {
        std::cout << "Oaspetele cu ID " << guestId << " nu exista!\n";
        return;
    }

    if (!rooms[roomIdx].getIsAvailable()) {
        std::cout << "Camera " << roomId << " nu este disponibila!\n";
        return;
    }

    if (guests[guestIdx].getIsCheckedIn()) {
        std::cout << "Oaspetele " << guests[guestIdx].getName() << " este deja cazat!\n";
        return;
    }

    int noNights;
    std::cout << "Numar de nopti: ";
    std::cin >> noNights;

    // marcam camera ca ocupata si oaspetele ca cazat
    rooms[roomIdx].book();
    guests[guestIdx].checkIn();

    // cream rezervarea
    Reservation newRes(
        rooms[roomIdx].getRoomNumber(),
        guests[guestIdx].getName(),
        noNights,
        rooms[roomIdx].getPricePerNight()
    );

    // adaugam rezervarea in array
    Reservation* newReservations = new Reservation[noReservations + 1];
    for (int i = 0; i < noReservations; i++)
        newReservations[i] = reservations[i];
    newReservations[noReservations] = newRes;
    delete[] reservations;
    reservations = newReservations;
    noReservations++;

    std::cout << "Check-in realizat cu succes!\n";
    std::cout << newRes;
}

// elibereaza camera si calculeaza costul final
void Hotel::checkOut(int reservationId) {
    int resIdx = findReservationIndex(reservationId);
    if (resIdx == -1) {
        std::cout << "Rezervarea cu ID " << reservationId << " nu exista!\n";
        return;
    }

    // cautam camera rezervarii dupa numar
    int roomIdx = -1;
    for (int i = 0; i < noRooms; i++)
        if (rooms[i].getRoomNumber() == reservations[resIdx].getRoomNumber()) {
            roomIdx = i;
            break;
        }

    // cautam oaspetele rezervarii dupa nume
    int guestIdx = -1;
    for (int i = 0; i < noGuests; i++)
        if (strcmp(guests[i].getName(), reservations[resIdx].getGuestName()) == 0) {
            guestIdx = i;
            break;
        }

    // calculam costul total
    float total = reservations[resIdx].calculateTotal();

    // eliberam camera si marcam oaspetele ca plecat
    if (roomIdx != -1) rooms[roomIdx].release();
    if (guestIdx != -1) {
        guests[guestIdx].checkOut();
        // adaugam suma cheltuita la totalul oaspetelui
        guests[guestIdx].addSpending(total);
    }

    // marcam rezervarea ca platita
    reservations[resIdx].pay();

    std::cout << "Check-out realizat cu succes!\n";
    std::cout << "Total de plata: " << total << " RON\n";
}

// toate camerele disponibile
void Hotel::getAvailableRooms() const {
    std::cout << "\n  Camere disponibile:\n";
    bool found = false;
    for (int i = 0; i < noRooms; i++) {
        if (rooms[i].getIsAvailable()) {
            std::cout << rooms[i];
            found = true;
        }
    }
    if (!found)
        std::cout << "  Nu exista camere disponibile.\n";
}

// venitul total din toate rezervarile platite
double Hotel::getTotalRevenue() const {
    double total = 0.0;
    for (int i = 0; i < noReservations; i++)
        if (reservations[i].getIsPaid())
            total += reservations[i].calculateTotal();
    return total;
}

// cea mai ieftina camera disponibila din hotel
void Hotel::findCheapestRoom() const {
    // verificam daca exista camere
    if (noRooms == 0) {
        std::cout << "Nu exista camere in hotel.\n";
        return;
    }

    // pornim cu prima camera disponibila ca referinta
    int cheapestIdx = -1;
    for (int i = 0; i < noRooms; i++) {
        if (rooms[i].getIsAvailable()) {
            if (cheapestIdx == -1 ||
                rooms[i].getPricePerNight() < rooms[cheapestIdx].getPricePerNight())
                cheapestIdx = i;
        }
    }

    if (cheapestIdx == -1) {
        std::cout << "Nu exista camere disponibile.\n";
        return;
    }

    std::cout << "\n  Cea mai ieftina camera disponibila:\n";
    std::cout << rooms[cheapestIdx];
}

void Hotel::printGuests() const {
    if (noGuests == 0) {
        std::cout << " Nu exista oaspeti inregistrati.\n";
        return;
    }
    std::cout << "\n  Oaspeti inregistrati:\n";
    for (int i = 0; i < noGuests; i++)
      std::cout << guests[i];
}

void Hotel::printReservations() const {
    if (noReservations == 0) {
        std::cout << " Nu exista rezervari.\n";
        return;
    }
    std::cout << "\n  Rezervari:\n";
    for (int i = 0; i < noReservations; i++)
        std::cout << reservations[i];
}