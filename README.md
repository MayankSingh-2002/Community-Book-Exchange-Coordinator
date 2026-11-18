# Community Book Exchange Coordinator

**Course:** CAP455 – Object Oriented Programming using C++ (MCA, Lovely Professional University)  
**Session:** 2026–27  
**Student:** Mayank Singh | **Roll No:** RD1P2635B66 | **Reg. No:** 12612197  
**Faculty:** Adarsh Kumar Singh  
**Current Phase:** CA2 – Persist & Search (Unit III)  

---

## 1. About the Project

Residents of a community deposit books on a shared shelf or kiosk. This console application is the digital register for that shelf. Every deposited book is stored as a `BookListing` object with a unique listing ID, and an `ExchangeCoordinator` object manages all listings and binary file operations.

It is a donation-and-pickup register, not a lending library: there are no members, due dates or fines.

In **CA1**, all data lived in volatile memory only. In **CA2**, records are persisted into a binary disk file (`../data/books.dat`) using fixed-size records so that data and static statistics survive program restarts.

---

## 2. Features by Phase

| Phase | Status | Key Features & Syllabus Coverage |
| :--- | :--- | :--- |
| **CA1 – Design & Create** | Completed | Entity class with static, inline and friend members; array of objects; pointers; `this`; `std::string`; in-memory Create & Read; validation & duplicate rejection |
| **CA2 – Persist & Search** | **Current** | Constructors & destructor; fixed-size char buffers (`224` bytes); binary file storage (`fstream`); random-access search (`seekg`); in-place update (`seekp`); data survives restart |
| CA3 – Reuse & Delete | Planned | Inheritance, operator overloading, `new` / `delete`, virtual functions, safe Update and Delete |
| CA4 – Integrate & Defend | Planned | Full CRUD, exceptions, templates, STL, advanced feature |

### CA2 Menu Options

1. Register new community book (Append to binary file)
2. View all books (Sequential binary read)
3. Search by ID & run audit (Random-access `seekg` + friend audit)
4. Update book status/details (In-place binary overwrite via `seekp`)
5. Search by title (Partial match across binary records)
6. View books of one donor (Filter persistent records)
7. System statistics (Static counter sync & record size)
8. Exit (Safe closure)

---

## 3. Project Structure

```
Community Book Exchange Coordinator/
├── docs/        CA1 & CA2 Project Reports (.docx), UML diagrams, flowcharts
├── src/         RD1P2635B66_CAP455_CA2_CommunityBookExchangeCoordinator.cpp
├── data/        books.dat (persistent 224-byte binary database file)
├── tests/       Automated test scripts (test_inputs.txt, test_restart.txt) & screenshots
└── README.md    This file
```

---

## 4. Requirements

* A C++ compiler with standard C++ support (developed with **g++ / GCC**)
* Windows 11 with VS Code and the PowerShell terminal (also compatible with Linux and macOS)
* Standard C++ libraries used: `<iostream>`, `<fstream>`, `<string>`, `<cstring>`, `<cstdlib>`

---

## 5. Build and Run

### Windows (PowerShell)

```powershell
cd "d:\Coding Journey\C++ Project\Community Book Exchange Coordinator\src"
g++ RD1P2635B66_CAP455_CA2_CommunityBookExchangeCoordinator.cpp -o RD1P2635B66_CAP455_CA2_CommunityBookExchangeCoordinator
.\RD1P2635B66_CAP455_CA2_CommunityBookExchangeCoordinator
```

### Linux / macOS

```bash
cd src
g++ RD1P2635B66_CAP455_CA2_CommunityBookExchangeCoordinator.cpp -o exchange
./exchange
```

---

## 6. How to Use & Validation Rules

The program presents an 8-option interactive menu. 

| Field | Rule & Constraint |
| :--- | :--- |
| Listing ID | Whole number 1 – 99999, must be unique across disk records |
| Title | 2 – 58 characters (fits fixed `char title[60]` buffer) |
| Author | 1 – 38 characters (fits fixed `char author[40]` buffer) |
| Genre | 1 – 28 characters (fits fixed `char genre[30]` buffer) |
| Donor name | 1 – 38 characters (fits fixed `char donorResident[40]` buffer) |
| Kiosk location | 1 – 38 characters (fits fixed `char dropLocation[40]` buffer) |
| Condition grade | Whole number 1 (Poor) to 5 (Like New) |

### Key Business Rules

* Duplicate Listing IDs are rejected by scanning `../data/books.dat` via `seekg`.
* The same donor cannot deposit the same title twice (case-insensitive check across disk records).
* Invalid inputs trigger an `[ERROR]` message and prompt again without crashing.
* Pressing `Ctrl+Z` (Windows) or `Ctrl+D` (Linux/macOS) ends the program cleanly.

---

## 7. Design Overview

| Class / Component | Role & Architecture |
| :--- | :--- |
| `BookListing` | Entity class: 224-byte fixed-size record containing default, parameterized, and copy constructors, destructor, fixed `char[]` buffers, inline getters, and static file-sync methods. |
| `ExchangeCoordinator` | Manager class: handles binary file streams (`../data/books.dat`), `seekg` random-access lookups, and `seekp` in-place record overwrites. |
| `inspectListingIntegrity()` | Friend function: performs quality audit using private members. |
| Helper Functions | `trim`, `toLowerStr`, `isDigitsOnly`, `readLine`, `readText`, `readInt`, `copyToFixedBuffer`. |

### CA2 Concepts Demonstrated

* Default, parameterized, and copy constructors with member initializer lists
* Destructor lifecycle
* Binary file streams (`fstream`, `ifstream`, `ofstream`) with fixed-size records (`sizeof(BookListing) = 224` bytes)
* Stream positioning pointers: `seekg()` for random-access reads and `seekp()` for in-place updates
* Startup counter synchronization via `file.tellg()`

UML class diagrams, file offset layouts, flowcharts, and full technical explanations are available in the CA2 report inside the `docs` folder.

---

## 8. Testing

Testing is verified using automated test scripts (`test_inputs.txt` and `test_restart.txt`) in the `tests` folder covering:
1. Empty file initialization & binary record size (`224` bytes)
2. Multi-record creation via parameterized and copy constructors
3. **Restart persistence proof** (`activeListingsCount` auto-syncs from disk on startup)
4. Random-access search via `seekg()` reporting exact byte offsets (`0`, `224`, `448`, etc.)
5. In-place record modification via `seekp()` (toggling status, updating location or grade)
6. Duplicate key and duplicate donor+title rejections on disk
7. Fixed-buffer max-length string overflow protection

---

## 9. Known Limitations (CA2)

* Record deletion and dynamic re-sorting are not yet implemented (planned for CA3).
* Fixed-size character array buffers enforce maximum character length limits per field.
* Case conversion handles standard English letters A–Z.

---

## 10. Academic Integrity

This is individual work for CAP455 at Lovely Professional University. Reference books, official C++ documentation, and AI tools were used solely for learning, structural formatting, and report preparation, and are formally disclosed in the CA2 project report.