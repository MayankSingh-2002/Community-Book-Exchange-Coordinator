# Community Book Exchange Coordinator

**Course:** CAP455 – Object Oriented Programming using C++ (MCA, Lovely Professional University)
**Session:** 2026–27
**Student:** Mayank Singh | **Roll No:** RD1P2635B66 | **Reg. No:** 12612197
**Faculty:** Adarsh Kumar Singh
**Current phase:** CA1 – Design & Create

---

## 1. About the Project

Residents of a community deposit books on a shared shelf or kiosk. This console application is the digital register for that shelf. Every deposited book is stored as a `BookListing` object with a unique listing ID, and an `ExchangeCoordinator` object manages all listings.

It is a donation-and-pickup register, not a lending library: there are no members, due dates or fines.

In CA1 all data lives **in memory only**. Data is lost when the program exits. File handling comes in CA2.

## 2. Features by Phase

| Phase                            | Status  | Features                                                                                                                                                                         |
| -------------------------------- | ------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **CA1 – Design & Create** | Done    | Entity class with static, inline and friend members; array of objects; pointers; `this`; `std::string`; in-memory Create and Read; input validation; duplicate-key rejection |
| CA2 – Persist & Search          | Planned | Constructors and destructor, binary file storage (`fstream`), `seekg` / `seekp`, data survives restart                                                                     |
| CA3 – Reuse & Delete            | Planned | Inheritance, operator overloading, `new` / `delete`, virtual functions, safe Update and Delete                                                                               |
| CA4 – Integrate & Defend        | Planned | Full CRUD, exceptions, templates, STL, advanced feature                                                                                                                          |

### CA1 menu options

1. Register new community book (Create)
2. View all books (Read all)
3. Search by ID and run audit (Read one, friend-function audit)
4. Search by title (partial, case-insensitive)
5. View books of one donor
6. System statistics (static member)
7. Exit

## 3. Project Structure

```
Community Book Exchange Coordinator/
├── docs/        Proposal, data dictionary, UML, flowcharts, CA1 report (.docx)
├── src/         RD1P2635B66_CAP455_CA1_CommunityBookExchangeCoordinator.cpp
├── data/        Sample data files (empty in CA1; used from CA2 onwards)
├── tests/       Test table and screenshots
└── README.md    This file
```

## 4. Requirements

- A C++ compiler with standard C++ support (developed with **g++ / GCC**)
- Windows 11 with VS Code and the PowerShell terminal (also works on Linux and macOS)
- Only standard headers are used: `<iostream>`, `<string>`, `<cstdlib>`

## 5. Build and Run

### Windows (PowerShell, as used in development)

```powershell
cd "d:\Coding Journey\C++ Project\Community Book Exchange Coordinator\src"
g++ RD1P2635B66_CAP455_CA1_CommunityBookExchangeCoordinator.cpp -o RD1P2635B66_CAP455_CA1_CommunityBookExchangeCoordinator
.\RD1P2635B66_CAP455_CA1_CommunityBookExchangeCoordinator
```

### Linux / macOS

```bash
cd src
g++ RD1P2635B66_CAP455_CA1_CommunityBookExchangeCoordinator.cpp -o exchange
./exchange
```

Optional: add `-Wall -Wextra` to check for warnings. The code compiles with none.

## 6. How to Use

The program shows a menu. Type a number from 1 to 7 and press Enter.

**Registering a book (option 1)** asks for, in order:

| Field                  | Rule                                    |
| ---------------------- | --------------------------------------- |
| Listing ID             | Whole number 1 – 99999, must be unique |
| Title                  | At least 2 characters                   |
| Author                 | Not empty                               |
| Genre                  | Not empty                               |
| Donor name             | Not empty                               |
| Kiosk / shelf location | Not empty                               |
| Condition grade        | Whole number 1 (Poor) to 5 (Like New)   |

Invalid input shows an `[ERROR]` message and asks again.

### Other rules

- A duplicate Listing ID is rejected.
- The same donor cannot deposit the same title twice (case-insensitive).
- The registry holds at most **50** books.
- Pressing Ctrl+Z then Enter (Windows) or Ctrl+D (Linux / macOS) ends the program cleanly.

### Sample session

```
Enter your choice (1-7): 1
--- Register Book for Community Exchange ---
Enter unique Listing ID (1 - 99999): 101
Enter book title (min 2 chars): The Alchemist
Enter author name: Paulo Coelho
Enter genre / category: Fiction
Enter donor / depositor name: Rahul Sharma
Enter kiosk / shelf location (e.g. North-Wing-Shelf-A): North-Wing-Shelf-A
Enter condition grade (1:Poor 2:Fair 3:Good 4:Very Good 5:Like New): 4

[SUCCESS] Book registered with ID: 101
```

## 7. Design Overview

| Class / function                                                                    | Role                                                                          |
| ----------------------------------------------------------------------------------- | ----------------------------------------------------------------------------- |
| `BookListing`                                                                     | Entity class: one deposited book (8 private attributes plus a static counter) |
| `ExchangeCoordinator`                                                             | Manager class: owns `BookListing registry[50]`, implements Create and Read  |
| `inspectListingIntegrity()`                                                       | Friend function: quality audit using private members                          |
| `trim`, `toLowerStr`, `isDigitsOnly`, `readLine`, `readText`, `readInt` | Helper functions for string handling and safe input                           |

**Unique key:** `listingId`

### CA1 concepts demonstrated

Static data member and static function · inline functions · friend function · `this` pointer · default constructor · array of objects · pointers and pointer arithmetic · `std::string` handling · const correctness · forward declaration.

UML, flowcharts and the full explanation are in the CA1 report in the `docs` folder.

## 8. Testing

20 test cases cover normal, invalid and boundary inputs: duplicate IDs, short titles, empty fields, out-of-range numbers, capacity limit (50), empty registry and end-of-input. The test table and screenshots are in the `tests` folder and in Chapter 6 of the report.

## 9. Known Limitations (CA1)

- Data is not saved; everything is lost on exit (fixed in CA2).
- No Update or Delete yet (planned for CA3).
- Fixed capacity of 50 listings.
- Case conversion handles English letters A–Z only.

## 10. Academic Integrity

This is individual work for CAP455. Books, online documentation and AI tools were used only for learning, clarification and report preparation, and are disclosed in the CA1 report.
