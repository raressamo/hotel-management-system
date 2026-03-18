#include "Room.h"
#include <string.h>

// membrul static
int Room::totalRooms = 0;

// functie helper
void Room::copyType(const char* src) {
    this->type = new char[strlen(src) + 1]; // adaug +1 pentru caracterul '\0', aloc memorie cu new char
    strcpy(this->type, src); // strcpy pentru a copia continutul sirului
}

// constructor default
Room::Room() : id(++totalRooms) {
    this->roomNumber = 0;
    copyType("-"); // folosesc functia helper pentru DRY ca sa nu dau strcpy de fiecare data
    this->pricePerNight = 0.0f;
    this->capacity = 0;
    this->isAvailable = true; // camera abia creata este disponibila by default
}

// constructor cu parametri
Room::Room(int roomNumber, const char* type, float price, int cap) : id(++totalRooms) {
    this->roomNumber = roomNumber; // cu this->roomNumber accesezi membrul clasei, nu parametrul functiei
    copyType(type);
    this->pricePerNight = price;
    this->capacity = cap;
    this->isAvailable = true;
}

// copy constructor
Room::Room(const Room& obj) : id(++totalRooms) {
    this->roomNumber = obj.roomNumber;
    copyType(obj.type);
    this->pricePerNight = obj.pricePerNight;
    this->capacity = obj.capacity;
    this->isAvailable = obj.isAvailable;
}

// operator=
Room& Room::operator=(const Room& obj) {
    // verificam daca obiectul curent este acelasi cu sursa
    if (this == &obj)
        return *this;

    delete[] this->type; // eliberam memoria veche a lui type inainte sa alocam alta ca sa previn memory leak

    // copiem toate datele din sursa
    this->roomNumber = obj.roomNumber;
    copyType(obj.type);
    this->pricePerNight = obj.pricePerNight;
    this->capacity = obj.capacity;
    this->isAvailable = obj.isAvailable;

    return *this; // returnam referinta la obiectul curent
}

// destructor
Room::~Room() {
    // type a fost alocat cu new char[] deci trebuie eliberat cu delete[]
    delete[] this->type;
}

// getters
// fiecare getter returneaza valoarea membrului corespunzator
int Room::getId() const { return id; }
int Room::getRoomNumber() const { return roomNumber; }
const char* Room::getType() const { return type; }
float Room::getPricePerNight() const { return pricePerNight; }
int Room::getCapacity() const { return capacity; }
bool Room::getIsAvailable() const { return isAvailable; }
int Room::getTotalRooms() { return totalRooms; } // getter static

// setters
void Room::setRoomNumber(int roomNumber) {
    this->roomNumber = roomNumber; // membrul = parametrul
}

void Room::setType(const char* type) {
    delete[] this->type; // eliberam memoria veche
    copyType(type); // alocam memorie noua si copiem continutul
}

void Room::setPricePerNight(float price) {
    // validare (pretul >0)
    if (price <= 0) {
        std::cout << "Pretul trebuie sa fie pozitiv!\n";
        return;
    }
    this->pricePerNight = price;
}

void Room::setCapacity(int capacity) {
    // validare (capacitate >0)
    if (capacity <= 0) {
        std::cout << "Capacitatea trebuie sa fie pozitiva!\n";
        return;
    }
    this->capacity = capacity;
}

// operatori
// operator<<
std::ostream& operator<<(std::ostream& os, const Room& obj) {
    os << "  [Camera       #" << obj.id << "]\n";
    os << "  Numar camera : " << obj.roomNumber << "\n";
    os << "  Tip          : " << obj.type << "\n";
    os << "  Pret/noapte  : " << obj.pricePerNight << " RON\n";
    os << "  Capacitate   : " << obj.capacity << " persoane\n";
    os << "  Status       : " << (obj.isAvailable ? "Disponibila" : "Ocupata") << "\n"; // daca isAvailabl e true afiseaza disponibila, alfel afiseaza ocupata
    return os;
}

// operator>>
std::istream& operator>>(std::istream& is, Room& obj) {
    std::cout << "Numar camera: ";
    is >> obj.roomNumber;

    // folosim un buffer temporar, ca sa facem deepcopy in this->type
    std::cout << "  Tip (Single/Double/Suite): ";
    char buf[50];
    is >> buf;
    obj.setType(buf);

    std::cout << "  Pret pe noapte: ";
    is >> obj.pricePerNight;

    std::cout << "  Capacitate: ";
    is >> obj.capacity;

    obj.isAvailable = true; // o camera citita e disponibila by default
    return is;
}

// functii publice
// book , marcheaza camera ca ocupata
void Room::book() {
    // validare
    if (!isAvailable) {
        std::cout << "Camera " << roomNumber << " este deja ocupata!\n";
        return;
    }
    isAvailable = false;
}

// release , marcheaza camera ca disponibila
void Room::release() {
    if (isAvailable) {
        std::cout << "Camera " << roomNumber << " este deja disponibila!\n";
        return;
    }
    isAvailable = true;
}

// isAffordable , returneaza daca este in buget
bool Room::isAffordable(float budget) const {
    return pricePerNight <= budget;
}