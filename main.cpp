#include <iostream>
#include <string>
#include <vector>
#include <limits>
using namespace std;

// Forward declarations
class Movie;
class Customer;
class Ticket;
class Transaction;

// Base class demonstrating virtual functions
class Entity {
protected:
    int id;
    string name;

public:
    Entity(int i = 0, string n = ""): id(i), name(n) {}
    virtual void display() = 0;
    virtual ~Entity() {} // Virtual destructor
};

// Movie class with static members and operator overloading
class Movie : public Entity {
private:
    static int movieCount;
    string genre;
    double rating;
    vector<int> showtimes;
    double basePrice;

public:
    // Default constructor
    Movie(): Entity(), genre(""), rating(0.0), basePrice(0.0) {
        movieCount++;
    }

    // Overloaded constructor
    Movie(int id, string name, string g, double r = 0.0, double price = 10.0)
        : Entity(id, name), genre(g), rating(r), basePrice(price) {
        movieCount++;
    }

    // Copy constructor
    Movie(const Movie& other): Entity(other.id, other.name) {
        genre = other.genre;
        rating = other.rating;
        showtimes = other.showtimes;
        basePrice = other.basePrice;
        movieCount++;
    }

    ~Movie() {
        movieCount--;
    }

    // Static function
    static int getMovieCount() { return movieCount; }

    // Function overloading
    void setRating(double r) { rating = r; }
    void setRating(int r) { rating = static_cast<double>(r); }

    // Operator overloading
    Movie& operator++() {
        rating += 0.1;
        return *this;
    }

    Movie operator+(const Movie& other) const {
        Movie result = *this;
        result.rating = (this->rating + other.rating) / 2;
        return result;
    }

    const vector<int>& getShowtimes() const { return showtimes; }
    void addShowtime(int time) { showtimes.push_back(time); }
    double getBasePrice() const { return basePrice; }

    void display() override {
        cout << "Movie ID: " << id << "\nTitle: " << name
             << "\nGenre: " << genre << "\nRating: " << rating
             << "\nBase Price: $" << basePrice << endl;
    }
};

int Movie::movieCount = 0;

// Customer class
class Customer : public Entity {
private:
    string email;
    string phone;
    bool isVIP;

public:
    Customer(int id = 0, string name = "", string email = "", string phone = "", bool vip = false)
        : Entity(id, name), email(email), phone(phone), isVIP(vip) {}

    void display() override {
        cout << "Customer ID: " << id << "\nName: " << name
             << "\nEmail: " << email << "\nPhone: " << phone
             << "\nVIP Status: " << (isVIP ? "Yes" : "No") << endl;
    }

    bool getVIPStatus() const { return isVIP; }
    void setVIPStatus(bool status) { isVIP = status; }
};

// Base Ticket class
class Ticket : public Entity {
protected:
    Movie* movie;
    int seatNumber;
    double price;
    string ticketType;

public:
    Ticket(int id, Movie* m, int seat, string type = "Standard")
        : Entity(id, "Ticket-" + to_string(id)),
          movie(m), seatNumber(seat),
          price(m ? m->getBasePrice() : 0.0),
          ticketType(type) {}

    virtual void calculatePrice() {
        if (movie) {
            price = movie->getBasePrice();
        }
    }

    double getPrice() const { return price; }  // Add getter for price

    void display() override {
        cout << "Ticket ID: " << id << "\nType: " << ticketType
             << "\nSeat Number: " << seatNumber << "\nPrice: $" << price << endl;
        if (movie) {
            movie->display();
        }
    }
};

// VIP Ticket class demonstrating inheritance
class VIPTicket : public Ticket {
private:
    string loungeAccess;
    static const double VIP_MULTIPLIER;

public:
    VIPTicket(int id, Movie* m, int seat, string lounge = "Premium")
        : Ticket(id, m, seat, "VIP"), loungeAccess(lounge) {
        calculatePrice();
    }

    void calculatePrice() override {
        Ticket::calculatePrice();
        price *= VIP_MULTIPLIER;
    }

    void display() override {
        Ticket::display();
        cout << "Lounge Access: " << loungeAccess << endl;
    }
};

const double VIPTicket::VIP_MULTIPLIER = 1.5;

// Transaction class
class Transaction {
private:
    int transactionId;
    vector<Ticket*> tickets;
    Customer* customer;
    string paymentMethod;
    double totalAmount;
    string dateTime;

public:
    Transaction(int id, Customer* c, string payment, string date)
        : transactionId(id), customer(c), paymentMethod(payment),
          dateTime(date), totalAmount(0.0) {}

    void addTicket(Ticket* ticket) {
        tickets.push_back(ticket);
        totalAmount += ticket->getPrice();  // Use getter instead of direct access
    }

    void display() {
        cout << "\n=== Transaction Details ===" << endl;
        cout << "Transaction ID: " << transactionId << endl;
        cout << "Date/Time: " << dateTime << endl;
        cout << "Payment Method: " << paymentMethod << endl;
        cout << "Total Amount: $" << totalAmount << endl;

        if (customer) {
            cout << "\nCustomer Details:" << endl;
            customer->display();
        }

        cout << "\nTickets:" << endl;
        for (auto ticket : tickets) {
            ticket->display();
            cout << "------------------------" << endl;
        }
    }
};

// Ticket System class to manage everything
class TicketSystem {
private:
    vector<Movie*> movies;
    vector<Customer*> customers;
    vector<Transaction*> transactions;
    int nextTicketId;
    int nextTransactionId;

public:
    TicketSystem(): nextTicketId(1), nextTransactionId(1) {}

    ~TicketSystem() {
        for (auto m : movies) delete m;
        for (auto c : customers) delete c;
        for (auto t : transactions) delete t;
    }

    void addMovie(Movie* movie) { movies.push_back(movie); }
    void addCustomer(Customer* customer) { customers.push_back(customer); }

    Ticket* createTicket(Movie* movie, int seatNumber, Customer* customer) {
        Ticket* ticket;
        if (customer && customer->getVIPStatus()) {
            ticket = new VIPTicket(nextTicketId++, movie, seatNumber);
        } else {
            ticket = new Ticket(nextTicketId++, movie, seatNumber);
        }
        return ticket;
    }

    Transaction* createTransaction(Customer* customer, vector<Ticket*>& tickets) {
        Transaction* trans = new Transaction(nextTransactionId++, customer, "Credit Card", "2024-10-22");
        for (auto ticket : tickets) {
            trans->addTicket(ticket);
        }
        transactions.push_back(trans);
        return trans;
    }

    void displayMovies() {
        cout << "\n=== Available Movies ===" << endl;
        for (auto movie : movies) {
            movie->display();
            cout << "------------------------" << endl;
        }
    }

    // Helper function to clear input buffer
    static void clearInputBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    void run() {
        while (true) {
            cout << "\n=== Movie Ticket Management System ===" << endl;
            cout << "1. Add Movie" << endl;
            cout << "2. Add Customer" << endl;
            cout << "3. Book Tickets" << endl;
            cout << "4. Display Movies" << endl;
            cout << "5. Exit" << endl;
            cout << "Enter choice: ";

            int choice;
            cin >> choice;
            clearInputBuffer();

            switch (choice) {
                case 1: {
                    string title, genre;
                    double rating, price;

                    cout << "Enter movie title: ";
                    getline(cin, title);
                    cout << "Enter genre: ";
                    getline(cin, genre);
                    cout << "Enter rating: ";
                    cin >> rating;
                    cout << "Enter base price: ";
                    cin >> price;

                    addMovie(new Movie(movies.size() + 1, title, genre, rating, price));
                    cout << "Movie added successfully!" << endl;
                    break;
                }
                case 2: {
                    string name, email, phone;
                    char vipChoice;

                    cout << "Enter customer name: ";
                    getline(cin, name);
                    cout << "Enter email: ";
                    getline(cin, email);
                    cout << "Enter phone: ";
                    getline(cin, phone);
                    cout << "VIP customer? (y/n): ";
                    cin >> vipChoice;

                    addCustomer(new Customer(customers.size() + 1, name, email, phone,
                                          tolower(vipChoice) == 'y'));
                    cout << "Customer added successfully!" << endl;
                    break;
                }
                case 3: {
                    if (movies.empty() || customers.empty()) {
                        cout << "Please add movies and customers first!" << endl;
                        break;
                    }

                    displayMovies();
                    cout << "Enter movie ID: ";
                    int movieId;
                    cin >> movieId;

                    if (movieId < 1 || movieId > movies.size()) {
                        cout << "Invalid movie ID!" << endl;
                        break;
                    }

                    cout << "Enter customer ID: ";
                    int customerId;
                    cin >> customerId;

                    if (customerId < 1 || customerId > customers.size()) {
                        cout << "Invalid customer ID!" << endl;
                        break;
                    }

                    cout << "Enter seat number: ";
                    int seatNumber;
                    cin >> seatNumber;

                    vector<Ticket*> tickets;
                    Ticket* ticket = createTicket(movies[movieId-1], seatNumber,
                                               customers[customerId-1]);
                    tickets.push_back(ticket);

                    Transaction* trans = createTransaction(customers[customerId-1], tickets);
                    trans->display();
                    break;
                }
                case 4:
                    displayMovies();
                    break;
                case 5:
                    cout << "Thank you for using the Movie Ticket System!" << endl;
                    return;
                default:
                    cout << "Invalid choice!" << endl;
            }
        }
    }
};

int main() {
    TicketSystem system;
    system.run();
    return 0;
}