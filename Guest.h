#ifndef GUEST_H
#define GUEST_H
#include <iostream>

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

#endif