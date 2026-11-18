#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
using namespace std;

const char DATA_FILE[] = "../data/books.dat";

// Forward declaration for friend function
class BookListing;
void inspectListingIntegrity(const BookListing &book);

string trim(string s)
{
    size_t start = s.find_first_not_of(" \t");
    if (start == string::npos)
        return "";
    size_t end = s.find_last_not_of(" \t");
    s.erase(end + 1);
    s.erase(0, start);
    return s;
}

string toLowerStr(string s)
{
    for (size_t i = 0; i < s.length(); i++)
    {
        if (s[i] >= 'A' && s[i] <= 'Z')
            s[i] = s[i] + 32;
    }
    return s;
}

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

string readText(const string &prompt, const string &fieldName, size_t minLen, size_t maxLen)
{
    string s = readLine(prompt);
    while (s.length() < minLen || s.length() > maxLen)
    {
        if (s.empty())
            cout << "[ERROR] " << fieldName << " cannot be empty.\n";
        else if (s.length() < minLen)
            cout << "[ERROR] " << fieldName << " must be at least "
                 << minLen << " characters.\n";
        else
            cout << "[ERROR] " << fieldName << " cannot exceed "
                 << maxLen << " characters.\n";
        s = readLine(prompt);
    }
    return s;
}

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

void copyToFixedBuffer(char *dest, const string &src, size_t maxSize)
{
    memset(dest, 0, maxSize);
    strncpy(dest, src.c_str(), maxSize - 1);
    dest[maxSize - 1] = '\0';
}


class BookListing
{
private:
    int  listingId;             // primary key (1 - 99999)
    char title[60];             // fixed-size char arrays for binary file storage
    char author[40];
    char genre[30];
    char donorResident[40];
    char dropLocation[40];
    int  conditionGrade;        // 1 (Poor) to 5 (Like New)
    bool isAvailable;

    static int activeListingsCount;

public:
    // 1. Default Constructor
    BookListing() : listingId(0), conditionGrade(0), isAvailable(false)
    {
        memset(title, 0, sizeof(title));
        memset(author, 0, sizeof(author));
        memset(genre, 0, sizeof(genre));
        memset(donorResident, 0, sizeof(donorResident));
        memset(dropLocation, 0, sizeof(dropLocation));
    }

    // 2. Parameterized Constructor
    BookListing(int listingId, const string &titleStr, const string &authorStr,
                const string &genreStr, const string &donorStr,
                const string &locationStr, int conditionGrade)
        : listingId(listingId), conditionGrade(conditionGrade), isAvailable(true)
    {
        copyToFixedBuffer(this->title, titleStr, sizeof(this->title));
        copyToFixedBuffer(this->author, authorStr, sizeof(this->author));
        copyToFixedBuffer(this->genre, genreStr, sizeof(this->genre));
        copyToFixedBuffer(this->donorResident, donorStr, sizeof(this->donorResident));
        copyToFixedBuffer(this->dropLocation, locationStr, sizeof(this->dropLocation));
    }

    // 3. Copy Constructor
    BookListing(const BookListing &other)
        : listingId(other.listingId),
          conditionGrade(other.conditionGrade),
          isAvailable(other.isAvailable)
    {
        memcpy(this->title, other.title, sizeof(this->title));
        memcpy(this->author, other.author, sizeof(this->author));
        memcpy(this->genre, other.genre, sizeof(this->genre));
        memcpy(this->donorResident, other.donorResident, sizeof(this->donorResident));
        memcpy(this->dropLocation, other.dropLocation, sizeof(this->dropLocation));
    }

    // 4. Destructor
    ~BookListing() {}

    // Setters for in-place updates (seekp)
    void setAvailability(bool status)          { this->isAvailable = status; }
    void setConditionGrade(int newGrade)       { this->conditionGrade = newGrade; }
    void setDropLocation(const string &newLoc) { copyToFixedBuffer(this->dropLocation, newLoc, sizeof(this->dropLocation)); }

    // Inline getters
    inline int         getListingId()      const { return listingId; }
    inline const char* getTitle()          const { return title; }
    inline const char* getDonor()          const { return donorResident; }
    inline int         getConditionGrade() const { return conditionGrade; }
    inline bool        getAvailability()   const { return isAvailable; }

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
             << (isAvailable ? "Available for Pickup" : "Claimed / Picked Up") << "\n";
        cout << "--------------------------------------------------------\n";
    }

    static void syncCountFromFile(int totalRecords) { activeListingsCount = totalRecords; }
    static void incrementCount()                    { activeListingsCount++; }

    static void displaySystemSummary()
    {
        cout << "\n[STATISTICS] Total persistent community listings in file: "
             << activeListingsCount << "\n";
        cout << "[STORAGE]    Fixed binary record size: "
             << sizeof(BookListing) << " bytes\n";
    }

    friend void inspectListingIntegrity(const BookListing &book);
};

int BookListing::activeListingsCount = 0;

// Friend function: quality audit
void inspectListingIntegrity(const BookListing &book)
{
    cout << "\n[AUDIT LOG] Quality audit for Listing #" << book.listingId << "\n";
    cout << " - Title length : " << strlen(book.title) << " characters\n";

    if (book.conditionGrade >= 4)
        cout << " - Condition    : Premium / Recommended for Community Spotlight\n";
    else if (book.conditionGrade >= 2)
        cout << " - Condition    : Acceptable Standard\n";
    else
        cout << " - Condition    : Flagged for Physical Repair / Re-binding\n";
}

class ExchangeCoordinator
{
private:
    const char *fileName;

public:
    ExchangeCoordinator(const char *fname) : fileName(fname)
    {
        // Open existing binary file or create it if it doesn't exist
        fstream file(fileName, ios::in | ios::binary);
        if (!file)
        {
            ofstream createFile(fileName, ios::out | ios::binary);
            createFile.close();
            BookListing::syncCountFromFile(0);
            return;
        }

        // Sync static counter with existing records on disk
        file.seekg(0, ios::end);
        int recordCount = file.tellg() / sizeof(BookListing);
        BookListing::syncCountFromFile(recordCount);
        file.close();
    }

    // Search using seekg: returns byte offset of matching record, or -1
    streampos findRecordOffsetById(int id, BookListing &outRecord)
    {
        ifstream inFile(fileName, ios::in | ios::binary);
        if (!inFile)
            return -1;

        inFile.seekg(0, ios::end);
        int totalRecords = inFile.tellg() / sizeof(BookListing);

        BookListing temp;
        for (int i = 0; i < totalRecords; i++)
        {
            streampos offset = i * sizeof(BookListing);
            inFile.seekg(offset, ios::beg);
            inFile.read((char *)&temp, sizeof(BookListing));

            if (inFile && temp.getListingId() == id)
            {
                outRecord = temp;
                inFile.close();
                return offset;
            }
        }

        inFile.close();
        return -1;
    }

    // Check if the same donor already deposited the same title
    bool isDuplicateDonorTitle(const string &title, const string &donor, int &existingId)
    {
        ifstream inFile(fileName, ios::in | ios::binary);
        if (!inFile)
            return false;

        BookListing temp;
        string targetTitle = toLowerStr(title);
        string targetDonor = toLowerStr(donor);

        while (inFile.read((char *)&temp, sizeof(BookListing)))
        {
            if (toLowerStr(temp.getTitle()) == targetTitle &&
                toLowerStr(temp.getDonor()) == targetDonor)
            {
                existingId = temp.getListingId();
                inFile.close();
                return true;
            }
        }

        inFile.close();
        return false;
    }

    // CREATE: Validate, construct object, and append to binary file
    void registerNewBook()
    {
        cout << "\n--- Register Book for Community Exchange ---\n";

        int id = readInt("Enter unique Listing ID (1 - 99999): ", 1, 99999);

        BookListing existing;
        if (findRecordOffsetById(id, existing) != -1)
        {
            cout << "[REJECTED] Duplicate key: Listing ID " << id
                 << " already exists!\n";
            return;
        }

        string title  = readText("Enter book title (2 - 58 chars): ", "Title", 2, 58);
        string author = readText("Enter author name (1 - 38 chars): ", "Author", 1, 38);
        string genre  = readText("Enter genre / category (1 - 28 chars): ", "Genre", 1, 28);
        string donor  = readText("Enter donor / depositor name (1 - 38 chars): ", "Donor name", 1, 38);

        int existingId = 0;
        if (isDuplicateDonorTitle(title, donor, existingId))
        {
            cout << "[REJECTED] " << donor << " has already deposited \""
                 << title << "\" (Listing ID " << existingId << ").\n";
            return;
        }

        string location = readText("Enter kiosk / shelf location (e.g. North-Wing-Shelf-A): ",
                                   "Location", 1, 38);
        int condition   = readInt("Enter condition grade (1:Poor 2:Fair 3:Good 4:Very Good 5:Like New): ",
                                  1, 5);

        BookListing newBook(id, title, author, genre, donor, location, condition);
        BookListing recordToWrite(newBook);     // uses copy constructor

        ofstream outFile(fileName, ios::out | ios::app | ios::binary);
        if (!outFile)
        {
            cout << "[ERROR] Could not open " << fileName << " for writing.\n";
            return;
        }

        outFile.write((char *)&recordToWrite, sizeof(BookListing));
        outFile.close();

        BookListing::incrementCount();
        cout << "\n[SUCCESS] Book persisted to " << fileName << " with ID: " << id << "\n";
    }

    // READ ALL: Sequential read from binary file
    void viewAllListings()
    {
        ifstream inFile(fileName, ios::in | ios::binary);
        if (!inFile)
        {
            cout << "\n[INFO] Data file not found.\n";
            return;
        }

        BookListing temp;
        int shown = 0;
        cout << "\n============= PERSISTENT COMMUNITY BOOK LISTINGS =============\n";
        while (inFile.read((char *)&temp, sizeof(BookListing)))
        {
            temp.displayCard();
            shown++;
        }
        inFile.close();

        if (shown == 0)
            cout << "[INFO] No community books stored yet.\n";
        else
            cout << "Total persistent records shown: " << shown << "\n";
    }

    // SEARCH WITH seekg: Direct random-access search by Listing ID + Friend Audit
    void searchAndAudit()
    {
        int searchId = readInt("\nEnter Listing ID to search: ", 1, 99999);

        BookListing found;
        streampos offset = findRecordOffsetById(searchId, found);

        if (offset != -1)
        {
            cout << "\n[MATCH FOUND at byte offset " << offset << "]\n";
            found.displayCard();
            inspectListingIntegrity(found);
        }
        else
        {
            cout << "\n[NOT FOUND] No record with Listing ID " << searchId << ".\n";
        }
    }

    // BASIC UPDATE WITH seekp: Modify record in-place at exact byte offset
    void updateBookRecord()
    {
        int targetId = readInt("\nEnter Listing ID to update: ", 1, 99999);

        BookListing record;
        streampos offset = findRecordOffsetById(targetId, record);

        if (offset == -1)
        {
            cout << "\n[NOT FOUND] Listing ID " << targetId << " does not exist.\n";
            return;
        }

        cout << "\n[CURRENT RECORD at byte offset " << offset << "]\n";
        record.displayCard();

        cout << "Select field to update:\n";
        cout << "1. Toggle Availability Status (Available <-> Claimed)\n";
        cout << "2. Update Kiosk / Shelf Location\n";
        cout << "3. Update Condition Grade (1 - 5)\n";
        cout << "4. Cancel Update\n";

        int choice = readInt("Enter choice (1-4): ", 1, 4);
        if (choice == 4)
        {
            cout << "[INFO] Update cancelled.\n";
            return;
        }

        if (choice == 1)
        {
            record.setAvailability(!record.getAvailability());
            cout << "[INFO] Status changed to: "
                 << (record.getAvailability() ? "Available for Pickup" : "Claimed / Picked Up") << "\n";
        }
        else if (choice == 2)
        {
            string newLoc = readText("Enter new kiosk / shelf location (1 - 38 chars): ",
                                     "Location", 1, 38);
            record.setDropLocation(newLoc);
        }
        else if (choice == 3)
        {
            int newGrade = readInt("Enter new condition grade (1 - 5): ", 1, 5);
            record.setConditionGrade(newGrade);
        }

        fstream ioFile(fileName, ios::in | ios::out | ios::binary);
        if (!ioFile)
        {
            cout << "[ERROR] Failed to open " << fileName << " for update.\n";
            return;
        }

        ioFile.seekp(offset, ios::beg);
        ioFile.write((char *)&record, sizeof(BookListing));
        ioFile.close();

        cout << "\n[SUCCESS] Record ID " << targetId
             << " updated via seekp() at byte offset " << offset << ".\n";
    }

    // READ: Partial case-insensitive title search
    void searchByTitle()
    {
        ifstream inFile(fileName, ios::in | ios::binary);
        if (!inFile)
            return;

        string key = toLowerStr(readText("\nEnter title or part of it: ", "Search text", 1, 58));
        int matches = 0;

        BookListing temp;
        while (inFile.read((char *)&temp, sizeof(BookListing)))
        {
            if (toLowerStr(temp.getTitle()).find(key) != string::npos)
            {
                temp.displayCard();
                matches++;
            }
        }
        inFile.close();

        if (matches == 0)
            cout << "\n[NOT FOUND] No title contains \"" << key << "\".\n";
        else
            cout << "Matches found: " << matches << "\n";
    }

    // READ: View all books deposited by one resident
    void viewByDonor()
    {
        ifstream inFile(fileName, ios::in | ios::binary);
        if (!inFile)
            return;

        string name = toLowerStr(readText("\nEnter donor name: ", "Donor name", 1, 38));
        int matches = 0;

        BookListing temp;
        while (inFile.read((char *)&temp, sizeof(BookListing)))
        {
            if (toLowerStr(temp.getDonor()) == name)
            {
                temp.displayCard();
                matches++;
            }
        }
        inFile.close();

        if (matches == 0)
            cout << "\n[NOT FOUND] No books deposited by that resident.\n";
        else
            cout << "Books deposited: " << matches << "\n";
    }
};

int main()
{
    ExchangeCoordinator coordinator(DATA_FILE);
    int choice = 0;

    do
    {
        cout << "\n====================================================\n";
        cout << "   COMMUNITY BOOK EXCHANGE COORDINATOR (CA2)\n";
        cout << "====================================================\n";
        cout << "1. Register new community book\n";
        cout << "2. View all books\n";
        cout << "3. Search by ID & run audit\n";
        cout << "4. Update book status/details\n";
        cout << "5. Search by title\n";
        cout << "6. View books of one donor\n";
        cout << "7. System statistics\n";
        cout << "8. Exit\n";

        choice = readInt("Enter your choice (1-8): ", 1, 8);

        switch (choice)
        {
            case 1: coordinator.registerNewBook();      
                    break;
            case 2: coordinator.viewAllListings();      
                    break;
            case 3: coordinator.searchAndAudit();       
                    break;
            case 4: coordinator.updateBookRecord();     
                    break;
            case 5: coordinator.searchByTitle();        
                    break;
            case 6: coordinator.viewByDonor();          
                    break;
            case 7: BookListing::displaySystemSummary();
                    break;
            case 8: cout << "\nExiting CA2 application. Data saved in "
                         << DATA_FILE << ".\n";         
                    break;
        }
    } while (choice != 8);


    return 0;
}