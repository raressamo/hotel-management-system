#ifndef ROOM_H
#define ROOM_H
#include <iostream>

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

#endif