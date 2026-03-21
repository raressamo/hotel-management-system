# Hotel Management System

## Informații

| | |
|---|---|
| **Student** | Samoilă Rareș |
| **Laborant** | Gavrilă Florin Alexandru |
| **Disciplina** | Programare Orientată pe Obiecte |
| **Facultatea** | Matematică și Informatică, UniBuc |
| **An / Semestru** | Anul I, Semestrul II |

---

## Descriere

Aplicație de gestiune a unui hotel implementată în C++, care demonstrează principiile de bază ale programării orientate pe obiecte: **încapsulare**, **abstractizare**, **gestionarea memoriei dinamice** și **supraîncărcarea operatorilor**.

Aplicația permite gestionarea camerelor, oaspeților și rezervărilor unui hotel printr-un meniu interactiv în linia de comandă.

---

## Clase implementate

### `Room` — Camera
Modelează o cameră de hotel.

| Membru | Tip | Descriere |
|--------|-----|-----------|
| `totalRooms` | `static int` | Numărul total de camere create |
| `id` | `const int` | ID unic, setat la creare |
| `roomNumber` | `int` | Numărul camerei |
| `type` | `char*` | Tipul camerei (Single / Double / Suite) |
| `pricePerNight` | `float` | Prețul pe noapte |
| `capacity` | `int` | Capacitatea (nr. persoane) |
| `isAvailable` | `bool` | Disponibilitatea camerei |

**Funcții publice:** `book()`, `release()`, `isAffordable(budget)`

---

### `Guest` — Oaspetele
Modelează un oaspete al hotelului.

| Membru | Tip | Descriere |
|--------|-----|-----------|
| `totalGuests` | `static int` | Numărul total de oaspeți creați |
| `guestId` | `const int` | ID unic, setat la creare |
| `name` | `char*` | Numele oaspetelui |
| `age` | `int` | Vârsta |
| `idCard` | `char[20]` | Seria buletinului |
| `totalSpent` | `double` | Suma totală cheltuită |
| `isCheckedIn` | `bool` | Status cazare |

**Funcții publice:** `addSpending(amount)`, `isAdult()`, `checkIn()`, `checkOut()`

---

### `Reservation` — Rezervarea
Modelează o rezervare asociată unui oaspete și unei camere.

| Membru | Tip | Descriere |
|--------|-----|-----------|
| `totalReservations` | `static int` | Numărul total de rezervări |
| `reservationId` | `const int` | ID unic, setat la creare |
| `roomNumber` | `int` | Numărul camerei rezervate |
| `guestName` | `char*` | Numele oaspetelui |
| `noNights` | `int` | Numărul de nopți |
| `pricePerNight` | `float` | Prețul pe noapte |
| `extraCharges` | `float*` | Array de servicii extra |
| `noExtras` | `int` | Numărul de servicii extra |
| `isPaid` | `bool` | Status plată |

**Funcții publice:** `addExtraCharge(amount)`, `pay()`, `calculateTotal()`

---

### `Hotel` — Hotelul
Clasa principală care agregă camere, oaspeți și rezervări.

| Membru | Tip | Descriere |
|--------|-----|-----------|
| `totalHotels` | `static int` | Numărul total de hoteluri |
| `hotelId` | `const int` | ID unic |
| `name` | `char*` | Numele hotelului |
| `rooms` | `Room*` | Array de camere |
| `guests` | `Guest*` | Array de oaspeți |
| `reservations` | `Reservation*` | Array de rezervări |
| `rating` | `double` | Ratingul hotelului (1.0 – 5.0) |

**Funcții publice:** `addRoom()`, `addGuest()`, `checkIn()`, `checkOut()`, `getAvailableRooms()`, `getTotalRevenue()`, `findCheapestRoom()`

---

## Funcționalități demonstrare în meniu

```
=== Hotel Management System ===
1. Gestionare camere
   - Adaugă cameră
   - Afișează camere disponibile
   - Cea mai ieftină cameră disponibilă
2. Gestionare oaspeți
   - Adaugă oaspete
   - Afișează oaspeți înregistrați
3. Rezervări
   - Check-in (creare rezervare)
   - Check-out (eliberare cameră + calcul cost)
   - Adaugă serviciu extra
   - Venit total hotel
   - Afișează toate rezervările
```

---

## Cerințe tehnice acoperite

- `static` — contoare globale per clasă (`totalRooms`, `totalGuests` etc.)
- `const` — ID-uri unice nemodificabile (`const int id`)
- `char*` — șiruri alocate dinamic cu deep copy
- `char[]` — seria buletinului (`idCard[20]`)
- `float` / `float*` — prețuri și servicii extra
- `double` — suma cheltuită, rating, venit total
- `int` / `bool` — diverse atribute
- Constructor de inițializare, copy constructor, `operator=`, destructor pentru toate clasele
- `operator<<` și `operator>>` pentru toate clasele
- Getters și setters cu validare
- Funcții private helper (`copyName`, `copyType`, `copyGuestName`, `copyExtras`)

---

## Compilare și rulare

```bash
g++ main2.cpp -o hotel
./hotel
```

---

## Structura repository

```
hotel-management-system/
├── main2.cpp        # Tot codul intr-un singur fisier
├── main.cpp         # Varianta cu fisiere separate
├── Room.h / Room.cpp
├── Guest.h / Guest.cpp
├── Reservation.h / Reservation.cpp
├── Hotel.h / Hotel.cpp
└── README.md
```
