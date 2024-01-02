#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

// A class to represent a contact
class Contact {
private:    // Private data members
    std::string name;        // Name of the contact
    std::string phoneNumber;    // Phone number of the contact

public:
    Contact(const std::string& n, const std::string& num) : name(n), phoneNumber(num) {}    // Constructor

    void displayContact() const {
        std::cout << "Name: " << name << "\tPhone Number: " << phoneNumber << std::endl;    // Display contact information
    }

    const std::string& getName() const {
        return name;    // Return name
    }

    const std::string& getPhoneNumber() const {
        return phoneNumber;   // Return phone number
    }

    void setName(const std::string& newName) {
        name = newName;     // Set name
    }

    void setPhoneNumber(const std::string& newPhoneNumber) {
        phoneNumber = newPhoneNumber;   // Set phone number
    }

    std::string toCSV() const {
        return name + "," + phoneNumber;    // Return contact information in CSV format
    }
};

class PhoneDirectory {
private:
    std::vector<Contact> contacts;  // A vector to store contacts
    const std::string defaultCsvFilename = "contacts.csv";  // Default CSV filename

public:
    void addContact() {
        std::string name, phoneNumber;  // Temporary variables to store contact information

        std::cout << "Enter name: ";    // Prompt the user for contact information  
        std::cin.ignore(); // ignore any newline character left in the stream
        std::getline(std::cin, name);   // Read name

        std::cout << "Enter phone number: ";    // Prompt the user for contact information
        std::getline(std::cin, phoneNumber);    // Read phone number

        contacts.push_back(Contact(name, phoneNumber));     // Add contact to the vector
        std::cout << "Contact added successfully." << std::endl;    // Display success message
        saveToCSV();
    }
    void readFromCSV(const std::string& filename = "") {
    std::string csvFilename = filename.empty() ? defaultCsvFilename : filename;

    std::ifstream file(csvFilename);
    if (file.is_open()) {
        contacts.clear(); // Clear existing contacts before reading from CSV

        std::string line;
        std::getline(file, line); // Skip header row

        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string name, phoneNumber;

            std::getline(ss, name, ',');
            std::getline(ss, phoneNumber, ',');

            contacts.push_back(Contact(name, phoneNumber));
        }

        std::cout << "Contacts loaded from " << csvFilename << " successfully." << std::endl;
        file.close();
    } else {
        std::cerr << "Unable to open file: " << csvFilename << std::endl;
    }
}

    void displayContacts() const {  // Display all contacts
        if (contacts.empty()) {    // Check if the vector is empty
            std::cout << "Phone directory is empty." << std::endl;      // Display message
        } else {        // Otherwise
            std::cout << "Phone Directory:" << std::endl;   // Display the phone directory
            for (const auto& contact : contacts) {  // Iterate over the vector
                contact.displayContact();  // Display contact information
        
            }
        }
    }   

    void searchContact() const {   // Search for a contact
        int searchOption;       // Search option
        std::cout << "Search by:\n";    // Display search options
        std::cout << "1. Name\n";   
        std::cout << "2. Phone Number\n";   // Display search options
        std::cout << "Enter your choice: "; // Prompt the user for search option
        std::cin >> searchOption;   // Read search option

        std::string searchTerm;    // Search term
        if (searchOption == 1) {    // If search by name
            std::cout << "Enter name to search: ";  // Prompt the user for name to search
            std::cin.ignore(); // ignore any newline character left in the stream
            std::getline(std::cin, searchTerm); // Read name to search
        } else if (searchOption == 2) { // If search by phone number
            std::cout << "Enter phone number to search: ";      // Prompt the user for phone number to search
            std::cin.ignore(); // ignore any newline character left in the stream
            std::getline(std::cin, searchTerm); // Read phone number to search
        } else {
            std::cout << "Invalid choice. Aborting search.\n";  // Display error message
            return; // Exit function
        }   

        bool found = false; // A flag to indicate whether the contact was found
        for (const auto& contact : contacts) {      // Iterate over the vector
            if ((searchOption == 1 && contact.getName() == searchTerm) ||   // If the contact is found
                (searchOption == 2 && contact.getPhoneNumber() == searchTerm)) {    // If the contact is found
                contact.displayContact();       // Display contact information
                found = true;   // Set the flag to true
                break;  // Exit the loop
            }
        }

        if (!found) {   // If the contact was not found
            std::cout << "Contact not found." << std::endl; // Display error message
        } else {
            saveSearchResultsToCSV(searchTerm); // Save search results to CSV
        }
    }


    void deleteContact() {  // Delete a contact
        std::string nameToDelete;   // Name of the contact to delete
        std::cout << "Enter the name of the contact to delete: ";   // Prompt the user for name of the contact to delete
        std::cin.ignore(); // ignore any newline character left in the stream
        std::getline(std::cin, nameToDelete);   // Read name of the contact to delete

        auto it = std::remove_if(contacts.begin(), contacts.end(),  // Find the contact
                                 [nameToDelete](const Contact& contact) { return contact.getName() == nameToDelete; }); // Find the contact

        if (it != contacts.end()) {     // If the contact is found
            contacts.erase(it, contacts.end()); // Delete the contact
            std::cout << "Contact deleted successfully." << std::endl;  // Display success message
            saveToCSV();    // Save contacts to CSV
        } else {    // Otherwise
            std::cout << "Contact not found." << std::endl; // Display error message
        }
    }

    void saveToCSV(const std::string& filename = "") const {    // Save contacts to CSV
    std::string csvFilename = filename.empty() ? defaultCsvFilename : filename; // Use default filename if no filename is provided

    std::ofstream file(csvFilename);    // Open the file for writing
    if (file.is_open()) {   // If the file is open
        // Write header row with field names
        file << std::setw(20) << std::left << "Name" << ","     // Write header row with field names
             << std::setw(15) << std::left << "Phone Number" << "\n";   // Write header row with field names

        // Write data rows
        for (const auto& contact : contacts) {  // Iterate over the vector
            file << std::setw(20) << std::left << contact.getName() << ","  // Write data rows
                 << std::setw(15) << std::left << contact.getPhoneNumber() << "\n";   // Write data rows
        }

        std::cout << "Contacts saved to " << csvFilename << " successfully." << std::endl;  // Display success message
        file.close();   // Close the file
    } else {    // Otherwise
        std::cerr << "Unable to open file: " << csvFilename << std::endl;   // Display error message
    }
}


    void saveSearchResultsToCSV(const std::string& searchTerm) const {  // Save search results to CSV
        std::ofstream file("results.csv", std::ios_base::app);  // Open the file for writing
        if (file.is_open()) {  // If the file is open
        file << std::setw(20) << std::left << "Name" << "," << std::setw(15) << std::left << "Phone Number" << "\n";

            for (const auto& contact : contacts) {  // Iterate over the vector
                if (contact.getName() == searchTerm || contact.getPhoneNumber() == searchTerm) {     // If the contact matches the search term
                    file << contact.toCSV() << "\n";    // Write contact information to the file
                }
            }
            std::cout << "Search results saved to results.csv successfully." << std::endl;  // Display success message
            file.close();   // Close the file
        } else {
            std::cerr << "Unable to open file: results.csv" << std::endl;   // Display error message
        }
    }

    void editContact() {    // Edit a contact
        std::string nameToEdit; // Name of the contact to edit
        std::cout << "Enter the name of the contact to edit: ";  // Prompt the user for name of the contact to edit
        std::cin.ignore();  // ignore any newline character left in the stream
        std::getline(std::cin, nameToEdit); // Read name of the contact to edit

        auto it = std::find_if(contacts.begin(), contacts.end(),    // Find the contact
                               [nameToEdit](const Contact& contact) { return contact.getName() == nameToEdit; });   // Find the contact

        if (it != contacts.end()) { // If the contact is found
            // Prompt the user for new information
            std::string newName, newPhoneNumber;    // Temporary variables to store new information

            std::cout << "Enter new name (press Enter to keep current): ";  // Prompt the user for new information
            std::getline(std::cin, newName);    // Read new name

            std::cout << "Enter new phone number (press Enter to keep current): ";  // Prompt the user for new information
            std::getline(std::cin, newPhoneNumber);   // Read new phone number

            // Update contact information if new information is provided    
            if (!newName.empty()) {     // If new name is provided
                it->setName(newName);       // Update name  
            }

            if (!newPhoneNumber.empty()) {  // If new phone number is provided
                it->setPhoneNumber(newPhoneNumber);  // Update phone number
            }

            std::cout << "Contact edited successfully." << std::endl;   // Display success message 
            saveToCSV();    // Save contacts to CSV
        } else {    // Otherwise
            std::cout << "Contact not found." << std::endl; // Display error message
        }
    }
};

int main() {    // Test program  
    PhoneDirectory phoneDirectory;  // Create a PhoneDirectory object
     phoneDirectory.readFromCSV();

    int choice; // User's choice
    do {    // Display menu and get user's choice
        std::cout << "\nMenu:\n";   // Display menu
        std::cout << "1. Add Contact\n";    // Display menu
        std::cout << "2. Display Contacts\n";   // Display menu
        std::cout << "3. Search Contact\n"; // Display menu
        std::cout << "4. Delete Contact\n";   // Display menu
        std::cout << "5. Edit Contact\n";   // Display menu
        std::cout << "6. Exit\n";   // Display menu
        std::cout << "Enter your choice: "; // Prompt the user for choice
        std::cin >> choice; // Read choice

        switch (choice) {   // Perform the selected operation
            case 1:     // Add a contact
                phoneDirectory.addContact();    // Add a contact

                break;
            case 2:
                phoneDirectory.displayContacts();   // Display all contacts
                break;
            case 3:
                phoneDirectory.searchContact(); // Search for a contact
                break;
            
            case 4:
                phoneDirectory.deleteContact(); // Delete a contact
                break;
            
            case 5:
                phoneDirectory.editContact();   // Edit a contact
                break;
            case 6:
                std::cout << "Exiting program. Goodbye!\n"; // Display exit message
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n"; // Display error message
        }

    } while (choice != 6);  // Exit if the user enters 8

    return 0;   // Exit program
}