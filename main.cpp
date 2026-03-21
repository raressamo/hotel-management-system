#include <iostream>
#include "Hotel.h"
#include "Room.h"
#include "Guest.h"
#include "Reservation.h"

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
                std::cout << hotel;
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
        std::cout << "0. Inapoi\n";
        std::cout << "Optiune: ";
        std::cin >> option;

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