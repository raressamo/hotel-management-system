#include <iostream>
#include <string.h>
#include <cstdlib>

class Room {
private:
    static int totalRooms; // static = apartine de clasa, nu de un obiect
    const int id; // const = se seteaza o singura data si nu mai poate fi modificat dupa
    int roomNumber; // numarul camerei
    char* type; // tipul camerei
    float pricePerNight; // pretul pe noapte
    int capacity; // cate persoane incap in camera
    bool isAvailable; // true e libera, false e luata
    void copyType(const char* src); // va aloca memorie noua si va copia sirul src in this->type pe viitor, are rol de functie helper

public:
    Room(); // constructor default, il chemi cu Room r1;
    Room(int roomNumber, const char* type, float price, int cap); // constructor cu parametrii, il chemi cu Room r1(101, "Single", 150.0f, 2)p
    Room(const Room& obj); // copy constructor, il chemi cu Room r2(r1);
    Room& operator=(const Room& obj); // operator= , atribuire intre obiecte deja existente
    ~Room(); // destructor

    // getters pentru a obtine valoarea unui membru privat
    int getId() const;
    int getRoomNumber() const;
    const char* getType() const; // const ca sa nu poata fi modificat
    float getPricePerNight() const;
    int getCapacity() const;
    bool getIsAvailable() const;
    static int getTotalRooms(); // static ca sa poata sa fie apelat fara vreun obiect

    // setters pentru a modifica valoarea unui membru privat
    void setRoomNumber(int roomNumber);
    void setType(const char* type);
    void setPricePerNight(float price);
    void setCapacity(int capacity);
    // pentru isAvailable se schimba prin book sau release

    // functii externe cu acces la membrii private
    friend std::ostream& operator<<(std::ostream& os, const Room& obj);
    friend std::istream& operator>>(std::istream& is, Room& obj);

    // functii publice
    void book(); // marcheaza camera ca ocupata
    void release(); // marcheaza camera ca libera
    bool isAffordable(float budget) const; // verifica daca pretul este in buget
};

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

class Guest {
private:
    static int totalGuests; // echivalentul totalRooms
    const int guestId; // id unic care nu se schimba
    char* name; // numele alocat dinamic pe heap
    int age; // varsta oaspetelui
    char idCard[20]; // seria array fix pe stack (AB123456)
    double totalSpent; // suma totala cheltuita
    bool isCheckedIn; // true e cazat false e plecat
    void copyName(const char* src); // functie helper

public:
    Guest(); // constructor default
    Guest(const char* name, int age, const char* idCard); // constructor cu parametri
    Guest(const Guest& obj); // copy constructor
    Guest& operator=(const Guest& obj); // operator=
    ~Guest(); // destructor

    // getters
    int getGuestId() const;
    const char* getName() const;
    int getAge() const;
    const char* getIdCard() const;
    double getTotalSpent() const;
    bool getIsCheckedIn() const;
    static int getTotalGuests(); // static adica e apelabil fara obiect

    // setters
    void setName(const char* name);
    void setAge(int age);
    void setIdCard(const char* idCard);

    // functii externe cu acces la membri private
    friend std::ostream& operator<<(std::ostream& os, const Guest& obj);
    friend std::istream& operator>>(std::istream& is, Guest& obj);

    // functii publice
    void addSpending(double amount); // adauga la suma cheltuita
    bool isAdult() const; // true daca e major
    void checkIn(); // marcheaza guestul ca si cazat
    void checkOut(); // marcheaza oaspetele ca plecat
};

// static se defineste in afara clasei
int Guest::totalGuests = 0;

// functie helper
void Guest::copyName(const char* src) {
    this->name = new char[strlen(src) + 1];
    strcpy(this->name, src);
}

// constructor default
Guest::Guest() : guestId(++totalGuests) {
    copyName("-");
    this->age = 0;
    strcpy(this->idCard, "-");
    this->totalSpent = 0.0;
    this->isCheckedIn = false; // oaspetele nu e cazat by default
}

// constructor cu parametri
Guest::Guest(const char* name, int age, const char* idCard) : guestId(++totalGuests) {
    copyName(name);
    this->age = age;
    strcpy(this->idCard, idCard);// idCard e array nu pointer
    this->totalSpent = 0.0;
    this->isCheckedIn = false;
}

// copy constructor
Guest::Guest(const Guest& obj) : guestId(++totalGuests) {
    copyName(obj.name);
    this->age = obj.age;
    strcpy(this->idCard, obj.idCard);
    this->totalSpent = obj.totalSpent;
    this->isCheckedIn = obj.isCheckedIn;
}

// operator=
Guest& Guest::operator=(const Guest& obj) {
    // verificare
    if (this == &obj)
        return *this;

    delete[] this->name;//prevenire memory leak
    copyName(obj.name);
    this->age = obj.age;
    strcpy(this->idCard, obj.idCard);
    this->totalSpent = obj.totalSpent;
    this->isCheckedIn = obj.isCheckedIn;
    return *this; // returneaza referinta la obiectul curent
}

// destructor
Guest::~Guest() {
    delete[] this->name;
}

// getters
int Guest::getGuestId() const { return guestId; }
const char* Guest::getName() const { return name; }
int Guest::getAge() const { return age; }
const char* Guest::getIdCard() const { return idCard; }
double Guest::getTotalSpent() const { return totalSpent; }
bool Guest::getIsCheckedIn() const { return isCheckedIn; }
int Guest::getTotalGuests() { return totalGuests; } // static

// setters
void Guest::setName(const char* name) {
    delete[] this->name;
    copyName(name);
}

void Guest::setAge(int age) {
    if (age <= 0) {
        std::cout << "Varsta trebuie sa fie pozitiva!\n";
        return;
    }
    this->age = age;
}

void Guest::setIdCard(const char* idCard) {
    strcpy(this->idCard, idCard);
}

// operatori
// operator<<
std::ostream& operator<<(std::ostream& os, const Guest& obj) {
    os << "  [Oaspete #" << obj.guestId << "]\n";
    os << "  Nume         : " << obj.name << "\n";
    os << "  Varsta       : " << obj.age << " ani\n";
    os << "  Buletin      : " << obj.idCard << "\n";
    os << "  Total cheltuit: " << obj.totalSpent << " RON\n";
    os << "  Status       : " << (obj.isCheckedIn ? "Cazat" : "Plecat") << "\n";
    return os;
}

// operator>>
std::istream& operator>>(std::istream& is, Guest& obj) {
    std::cout << "  Nume: ";
    char buf[100];
    is >> buf;
    obj.setName(buf);
    std::cout << "  Varsta: ";
    is >> obj.age;
    std::cout << "  Serie buletin: ";
    is >> obj.idCard;
    // totalSpent si isCheckedIn sunt setate automat de program
    obj.totalSpent = 0.0;
    obj.isCheckedIn = false;
    return is;
}

// functii publice
void Guest::addSpending(double amount) {
    if (amount <= 0) {
        std::cout << "Suma trebuie sa fie pozitiva!\n";
        return;
    }
    this->totalSpent += amount;
}

bool Guest::isAdult() const {
    return this->age >= 18;
}

void Guest::checkIn() {
    if (isCheckedIn) {
        std::cout << "Oaspetele " << name << " este deja cazat!\n";
        return;
    }
    isCheckedIn = true;
}

void Guest::checkOut() {
    if (!isCheckedIn) {
        std::cout << "Oaspetele " << name << " nu este cazat!\n";
        return;
    }
    isCheckedIn = false;
}

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

void clearScreen() {
    system("cls");
}

void roomMenu(Hotel& hotel) {
    int option;
    while (true) {
        std::cout << "\n--- Meniu Camere ---\n";
        std::cout << "1. Adauga camera\n";
        std::cout << "2. Afiseaza toate camerele\n";
        std::cout << "3. Afiseaza camere disponibile\n";
        std::cout << "4. Cea mai ieftina camera\n";
        std::cout << "0. Inapoi\n";
        std::cout << "Optiune: ";
        std::cin >> option;
        clearScreen();

        switch (option) {
            case 1: {
                // cream o camera goala si citim datele de la tastatura cu operator>>
                Room r;
                std::cin >> r;
                hotel.addRoom(r);
                std::cout << "Camera adaugata cu succes!\n";
                break;
            }
            case 2: {
                // afisam informatiile hotelului care include numarul de camere
                std::cout << hotel;
                break;
            }
            case 3: {
                // apelam functia care afiseaza camerele disponibile
                hotel.getAvailableRooms();
                break;
            }
            case 4: {
                // apelam functia care gaseste cea mai ieftina camera
                hotel.findCheapestRoom();
                break;
            }
            case 0:
                return;
            default:
                std::cout << "Optiune invalida!\n";
        }
    }
}

void guestMenu(Hotel& hotel) {
    int option;
    while (true) {
        std::cout << "\n--- Meniu Oaspeti ---\n";
        std::cout << "1. Adauga oaspete\n";
        std::cout << "2. Afiseaza informatii hotel\n";
        std::cout << "0. Inapoi\n";
        std::cout << "Optiune: ";
        std::cin >> option;
        clearScreen();

        switch (option) {
            case 1: {
                // cream un oaspete gol si citim datele de la tastatura cu operator>>
                Guest g;
                std::cin >> g;
                hotel.addGuest(g);
                std::cout << "Oaspete adaugat cu succes!\n";
                break;
            }
            case 2: {
                hotel.printGuests();
                break;
            }
            case 0:
                return;
            default:
                std::cout << "Optiune invalida!\n";
        }
    }
}

void reservationMenu(Hotel& hotel) {
    int option;
    while (true) {
        std::cout << "\n--- Meniu Rezervari ---\n";
        std::cout << "1. Check-in\n";
        std::cout << "2. Check-out\n";
        std::cout << "3. Adauga serviciu extra la rezervare\n";
        std::cout << "4. Venit total hotel\n";
        std::cout << "5. Afiseaza toate rezervarile\n";
        std::cout << "0. Inapoi\n";
        std::cout << "Optiune: ";
        std::cin >> option;
        clearScreen();

        switch (option) {
            case 1: {
                // checkIn are nevoie de ID-ul camerei si ID-ul oaspetelui
                int roomId, guestId;
                std::cout << "ID camera: ";
                std::cin >> roomId;
                std::cout << "ID oaspete: ";
                std::cin >> guestId;
                hotel.checkIn(roomId, guestId);
                break;
            }
            case 2: {
                // checkOut are nevoie de ID-ul rezervarii
                int reservationId;
                std::cout << "ID rezervare: ";
                std::cin >> reservationId;
                hotel.checkOut(reservationId);
                break;
            }
            case 3: {
                // adaugam un serviciu extra la o rezervare
                int reservationId;
                float amount;
                std::cout << "ID rezervare: ";
                std::cin >> reservationId;
                std::cout << "Suma serviciu extra (RON): ";
                std::cin >> amount;
                std::cout << "Serviciu extra adaugat!\n";
                break;
            }
            case 4: {
                // apelam functia care calculeaza venitul total
                std::cout << "Venit total: " << hotel.getTotalRevenue() << " RON\n";
                break;
            }
            case 5: {
                hotel.printReservations();
                break;
            }
            case 0:
                return;
            default:
                std::cout << "Optiune invalida!\n";
        }
    }
}

int main() {
    // cream hotelul principal al aplicatiei
    Hotel hotel("PBT Hotel", 4.5);

    int option;
    while (true) {
        std::cout << "\n=== Hotel Management System ===\n";
        std::cout << hotel; // afisam informatiile hotelului cu operator<<
        std::cout << "\n1. Gestionare camere\n";
        std::cout << "2. Gestionare oaspeti\n";
        std::cout << "3. Rezervari\n";
        std::cout << "0. Exit\n";
        std::cout << "Optiune: ";
        std::cin >> option;
        clearScreen();

        switch (option) {
            case 1:
                roomMenu(hotel);
                break;
            case 2:
                guestMenu(hotel);
                break;
            case 3:
                reservationMenu(hotel);
                break;
            case 0:
                std::cout << "La revedere!\n";
                return 0;
            default:
                std::cout << "Optiune invalida!\n";
        }
    }
}
