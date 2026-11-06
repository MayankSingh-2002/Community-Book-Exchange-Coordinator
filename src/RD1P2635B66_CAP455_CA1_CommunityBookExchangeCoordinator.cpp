#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

const int MAX_CAPACITY = 50;    // size of the array of objects

// Forward declaration, needed for the friend function
class BookListing;
void inspectListingIntegrity(const BookListing &book);

// Remove spaces/tabs from both ends. Uses the string modifier erase().
string trim(string s)
{
    size_t start = s.find_first_not_of(" \t");
    if (start == string::npos)
        return "";                          // only spaces
    size_t end = s.find_last_not_of(" \t");
    s.erase(end + 1);                       // remove from the right
    s.erase(0, start);                      // remove from the left
    return s;
}

// Convert a string to lower case (for case-insensitive searching)
string toLowerStr(string s)
{
    for (size_t i = 0; i < s.length(); i++)
    {
        if (s[i] >= 'A' && s[i] <= 'Z')
            s[i] = s[i] + 32;
    }
    return s;
}

// true if the string has only the characters 0-9
bool isDigitsOnly(const string &s)
{
    if (s.empty())
        return false;
    for (size_t i = 0; i < s.length(); i++)
    {
        if (s[i] < '0' || s[i] > '9')
            return false;
    }
    return true;
}

// Read one full line. If the input has ended (Ctrl+Z / Ctrl+D)
// the program exits cleanly instead of looping forever.
string readLine(const string &prompt)
{
    string line;
    cout << prompt;
    if (!getline(cin, line))
    {
        cout << "\n[INFO] Input ended. Exiting program.\n";
        exit(0);
    }
    return trim(line);
}

// Read a text field that must have at least minLen characters
string readText(const string &prompt, const string &fieldName, size_t minLen)
{
    string s = readLine(prompt);
    while (s.length() < minLen)
    {
        if (s.empty())
            cout << "[ERROR] " << fieldName << " cannot be empty.\n";
        else
            cout << "[ERROR] " << fieldName << " must be at least "
                 << minLen << " characters.\n";
        s = readLine(prompt);
    }
    return s;
}

// Read a whole number between low and high (letters, signs, blanks rejected)
int readInt(const string &prompt, int low, int high)
{
    while (true)
    {
        string line = readLine(prompt);
        if (isDigitsOnly(line) && line.length() <= 9)
        {
            int value = 0;
            for (size_t i = 0; i < line.length(); i++)
                value = value * 10 + (line[i] - '0');
            if (value >= low && value <= high)
                return value;
        }
        cout << "[ERROR] Enter a whole number between "
             << low << " and " << high << ".\n";
    }
}

// ENTITY CLASS: BookListing
// One book deposited on the community shelf
class BookListing
{
private:
    int    listingId;           // unique key
    string title;
    string author;
    string genre;
    string donorResident;
    string dropLocation;        // e.g. "Room 3 Shelf 4"
    int    conditionGrade;      // 1 (Poor) to 5 (Like New)
    bool   isAvailable;

    // static data member: shared by all objects
    static int activeListingsCount;

public:
    // default constructor
    BookListing()
    {
        listingId      = 0;
        title          = "";
        author         = "";
        genre          = "";
        donorResident  = "";
        dropLocation   = "";
        conditionGrade = 0;
        isAvailable    = false;
    }

    // setter: parameter names equal member names, so 'this' is needed
    void setListingDetails(int listingId, string title, string author,
                           string genre, string donorResident,
                           string dropLocation, int conditionGrade)
    {
        this->listingId      = listingId;
        this->title          = title;
        this->author         = author;
        this->genre          = genre;
        this->donorResident  = donorResident;
        this->dropLocation   = dropLocation;
        this->conditionGrade = conditionGrade;
        this->isAvailable    = true;

        activeListingsCount++;
    }

    // inline member functions (defined inside the class)
    inline int    getListingId()      const { return listingId; }
    inline string getTitle()          const { return title; }
    inline string getDonor()          const { return donorResident; }
    inline int    getConditionGrade() const { return conditionGrade; }
    inline bool   getAvailability()   const { return isAvailable; }

    // show one listing as a card
    void displayCard() const
    {
        cout << "--------------------------------------------------------\n";
        cout << " Listing ID      : " << listingId << "\n";
        cout << " Title           : " << title << "\n";
        cout << " Author          : " << author << "\n";
        cout << " Genre           : " << genre << "\n";
        cout << " Deposited By    : " << donorResident << "\n";
        cout << " Kiosk Location  : " << dropLocation << "\n";
        cout << " Condition (1-5) : " << conditionGrade << " / 5\n";
        cout << " Status          : "
             << (isAvailable ? "Available for Pickup" : "Claimed") << "\n";
        cout << "--------------------------------------------------------\n";
    }

    // static member function: can be called without an object
    static void displaySystemSummary()
    {
        cout << "\n[STATISTICS] Total active community listings in memory: "
             << activeListingsCount << "\n";
    }

    // friend function: allowed to read the private members
    friend void inspectListingIntegrity(const BookListing &book);
};

// definition of the static data member
int BookListing::activeListingsCount = 0;

// FRIEND FUNCTION: quality audit of one listing
void inspectListingIntegrity(const BookListing &book)
{
    cout << "\n[AUDIT LOG] Quality audit for Listing #" << book.listingId << "\n";
    cout << " - Title length : " << book.title.length() << " characters\n";

    if (book.conditionGrade >= 4)
        cout << " - Condition    : Premium / Recommended for Community Spotlight\n";
    else if (book.conditionGrade >= 2)
        cout << " - Condition    : Acceptable Standard\n";
    else
        cout << " - Condition    : Flagged for Physical Repair / Re-binding\n";
}

// COORDINATOR CLASS: manages the array of objects
class ExchangeCoordinator
{
private:
    BookListing registry[MAX_CAPACITY];
    int currentCount;

public:
    ExchangeCoordinator()
    {
        currentCount = 0;
    }

    // Returns a pointer to the matching object, or NULL if not found.
    // Walks through the array using pointer arithmetic.
    BookListing* findListingById(int id)
    {
        BookListing *ptr = registry;
        for (int i = 0; i < currentCount; i++, ptr++)
        {
            if (ptr->getListingId() == id)
                return ptr;
        }
        return NULL;
    }

    // CREATE: validation + duplicate key rejection
    void registerNewBook()
    {
        if (currentCount >= MAX_CAPACITY)
        {
            cout << "\n[ERROR] Registry is full. Cannot add more books.\n";
            return;
        }

        cout << "\n--- Register Book for Community Exchange ---\n";

        int id = readInt("Enter unique Listing ID (1 - 99999): ", 1, 99999);

        // no duplicate keys
        if (findListingById(id) != NULL)
        {
            cout << "[REJECTED] Duplicate key: Listing ID " << id
                 << " already exists!\n";
            return;
        }

        string title  = readText("Enter book title (min 2 chars): ", "Title", 2);
        string author = readText("Enter author name: ", "Author", 1);
        string genre  = readText("Enter genre / category: ", "Genre", 1);
        string donor  = readText("Enter donor / depositor name: ", "Donor name", 1);

        // the same donor cannot deposit the same title twice
        BookListing *ptr = registry;
        for (int i = 0; i < currentCount; i++, ptr++)
        {
            if (toLowerStr(ptr->getTitle()) == toLowerStr(title) &&
                toLowerStr(ptr->getDonor()) == toLowerStr(donor))
            {
                cout << "[REJECTED] " << donor << " has already deposited \""
                     << title << "\" (Listing ID " << ptr->getListingId() << ").\n";
                return;
            }
        }

        string location = readText("Enter kiosk / shelf location (e.g. North-Wing-Shelf-A): ",
                                   "Location", 1);

        int condition = readInt("Enter condition grade (1:Poor 2:Fair 3:Good 4:Very Good 5:Like New): ",
                                1, 5);

        // store the new object in the next free slot of the array
        BookListing *slot = &registry[currentCount];
        slot->setListingDetails(id, title, author, genre, donor, location, condition);
        currentCount++;

        cout << "\n[SUCCESS] Book registered with ID: " << id << "\n";
    }

    // READ: show all listings
    void viewAllListings()
    {
        if (currentCount == 0)
        {
            cout << "\n[INFO] No community books registered yet.\n";
            return;
        }

        cout << "\n============= CURRENT COMMUNITY BOOK LISTINGS =============\n";
        BookListing *ptr = registry;
        for (int i = 0; i < currentCount; i++, ptr++)
            ptr->displayCard();
        cout << "Total books shown: " << currentCount << "\n";
    }

    // READ: search by ID, then run the friend-function audit
    void searchAndAudit()
    {
        if (currentCount == 0)
        {
            cout << "\n[INFO] Registry is empty.\n";
            return;
        }

        int searchId = readInt("\nEnter Listing ID to search: ", 1, 99999);

        BookListing *found = findListingById(searchId);
        if (found != NULL)
        {
            cout << "\n[MATCH FOUND]\n";
            found->displayCard();
            inspectListingIntegrity(*found);    // friend function call
        }
        else
        {
            cout << "\n[NOT FOUND] No record with Listing ID " << searchId << "\n";
        }
    }

    // READ: search by title (partial match, ignores upper/lower case)
    void searchByTitle()
    {
        if (currentCount == 0)
        {
            cout << "\n[INFO] Registry is empty.\n";
            return;
        }

        string key = toLowerStr(readText("\nEnter title or part of it: ", "Search text", 1));
        int matches = 0;

        BookListing *ptr = registry;
        for (int i = 0; i < currentCount; i++, ptr++)
        {
            if (toLowerStr(ptr->getTitle()).find(key) != string::npos)
            {
                ptr->displayCard();
                matches++;
            }
        }

        if (matches == 0)
            cout << "\n[NOT FOUND] No title contains \"" << key << "\".\n";
        else
            cout << "Matches found: " << matches << "\n";
    }

    // READ: all books deposited by one resident
    void viewByDonor()
    {
        if (currentCount == 0)
        {
            cout << "\n[INFO] Registry is empty.\n";
            return;
        }

        string name = toLowerStr(readText("\nEnter donor name: ", "Donor name", 1));
        int matches = 0;

        BookListing *ptr = registry;
        for (int i = 0; i < currentCount; i++, ptr++)
        {
            if (toLowerStr(ptr->getDonor()) == name)
            {
                ptr->displayCard();
                matches++;
            }
        }

        if (matches == 0)
            cout << "\n[NOT FOUND] No books deposited by that resident.\n";
        else
            cout << "Books deposited: " << matches << "\n";
    }
};

int main()
{
    ExchangeCoordinator coordinator;
    int choice = 0;

    do
    {
        cout << "\n====================================================\n";
        cout << "   COMMUNITY BOOK EXCHANGE COORDINATOR (CA1)\n";
        cout << "====================================================\n";
        cout << "1. Register new community book\n";
        cout << "2. View all books\n";
        cout << "3. Search by ID and run audit\n";
        cout << "4. Search by title\n";
        cout << "5. View books of one donor\n";
        cout << "6. System statistics\n";
        cout << "7. Exit\n";

        choice = readInt("Enter your choice (1-7): ", 1, 7);

        switch (choice)
        {
            case 1: coordinator.registerNewBook();      
                    break;
            case 2: coordinator.viewAllListings();      
                    break;
            case 3: coordinator.searchAndAudit();       
                    break;
            case 4: coordinator.searchByTitle();        
                    break;
            case 5: coordinator.viewByDonor();          
                    break;
            case 6: BookListing::displaySystemSummary();
                    break;
            case 7: cout << "\nExiting CA1 in-memory application. Goodbye!\n"; 
                    break;
        }
    } while (choice != 7);

    return 0;
}