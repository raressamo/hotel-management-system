#include "Guest.h"
#include <string.h>

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