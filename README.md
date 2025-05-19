# Drunk Drivers 🍻🚗

**Drunk Drivers** is a 2D racing game developed in C++ using the **SFML** library. The game includes both single-player and two-player modes, dynamic sound effects, and resolution settings from a main menu interface.

## 🎮 Features

- Single-player and Two-player modes
- Adjustable screen resolution from the main menu
- 2D graphics rendered using **SFML**
- Simple but fun gameplay mechanics

## 🛠️ Built With

- **C++**
- [**SFML (Simple and Fast Multimedia Library)**](https://www.sfml-dev.org/)
- Visual Studio Code (or your preferred IDE)


### Prerequisites

- A C++17 (or later) compatible compiler

- You must set up SFML before running the code in code/main.cpp.
If SFML is not installed/configured, the game will not compile or run.

- Link SFML to Your Project:g++ code/main.cpp -o DrunkDrivers -lsfml-graphics -lsfml-window -lsfml-system

### Build Instructions
Το παιχνίδι λειτουργεί με τη βοήθεια της βιβλιοθήκης SFML.
Για να εκτελεστεί σωστά μέσω του Visual Studio, θα χρειαστεί να γίνουν οι κατάλληλες ρυθμίσεις στις Configuration Properties, τόσο στον C/C++ compiler όσο και στον Linker, ώστε να περιλαμβάνουν τα paths προς τους φακέλους της SFML.
Τα απαιτούμενα assets βρίσκονται μέσα στον φάκελο assets και οργανώνονται στους υποφακέλους:
audio για τους ήχους (π.χ. background και εφέ)
images για τα γραφικά του παίκτη και των εμποδίων
font για τη γραμματοσειρά του παιχνιδιού.
Κατά συνέπεια, θα πρέπει να τροποποιηθούν τα paths στον κώδικα, ώστε να αντιστοιχούν στα σωστά directories (π.χ. για τον παίκτη, τα εμπόδια, τους ήχους και το background), χρησιμοποιώντας τα κατάλληλα αρχεία από τους παραπάνω
υποφακέλους.



