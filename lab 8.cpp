#include <iostream>
using namespace std;

class TicketSystem {
public:
    int ticketID;       // ID of the ticket
    string ticketName;  // Name of the ticket
    int ticketPrice;    // Price of the ticket

    // Function to display the ticket menu
    void displayMenu() {
        cout << "TICKET MENU \n";
        cout << "1   Concert Ticket   500 \n";
        cout << "2   Movie Ticket      300\n";
        cout << "3   Sports Ticket     400\n";
    }

    // Friend function declaration
    // This allows orderTicket to access private members of TicketSystem
    friend void orderTicket(TicketSystem &system, int ticketID, int ticketPrice);
};

// Friend function definition
// This function can access private members of TicketSystem
void orderTicket(TicketSystem &system, int ticketID, int ticketPrice) {
    // Determine the ticket name and price based on the ticketID
    switch (ticketID) {
        case 1:
            system.ticketName = "Concert Ticket";
            system.ticketPrice = 500;
            break;
        case 2:
            system.ticketName = "Movie Ticket";
            system.ticketPrice = 300;
            break;
        case 3:
            system.ticketName = "Sports Ticket";
            system.ticketPrice = 400;
            break;
        default:
            system.ticketName = "Unknown Ticket";
            system.ticketPrice = 0;
            break;
    }

    // Print the ticket details or an error message if the ID is invalid
    if (system.ticketPrice == 0) {
        cout << "Invalid ticket ID entered." << endl;
    } else {
        cout << "You have ordered: " << system.ticketName << " with ID " << ticketID << " and price " << system.ticketPrice << endl;
        cout << "THANK YOU FOR YOUR ORDER!" << endl;
    }
}

int main() {
    TicketSystem system;  // Create an instance of TicketSystem
    system.displayMenu(); // Display the ticket menu

    int ticketID;         // Variable to store user input for ticket ID
    cout << "Enter the ticket ID number you would like to order: ";
    cin >> ticketID;      // Get the ticket ID from the user

    // Check if the ticket ID is valid (1 to 3)
    if (ticketID >= 1 && ticketID <= 3) {
        orderTicket(system, ticketID, 300);  // Call the friend function to process the order
    } else {
        cout << "Enter a valid ticket ID." << endl;  // Error message for invalid ID
    }

    return 0;  // End of program
}

