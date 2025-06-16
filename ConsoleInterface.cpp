#include "ConsoleInterface.h"
#include "HotelSystem.h"
#include "MyString.h"
#include <iostream>
#include <ctime>
#include <fstream>

MyString readLine() 
{
    char buffer[256];
    if (std::cin.peek() == '\n') {
        std::cin.ignore();
    }
    std::cin.getline(buffer, sizeof(buffer));
    return MyString(buffer);
}

int readInt() 
{
    int value;
    std::cin >> value;
    while (std::cin.fail() || std::cin.get() != '\n') 
    {
        std::cout << "Invalid input. Please enter an integer: ";
        std::cin.clear();
        while (std::cin.get() != '\n' && std::cin.good());
        std::cin >> value;
    }
    return value;
}

double readDouble() {
    double value;
    std::cin >> value;
    while (std::cin.fail() || std::cin.get() != '\n') {
        std::cout << "Invalid input. Please enter a number: ";
        std::cin.clear();
        while (std::cin.get() != '\n' && std::cin.good());
        std::cin >> value;
    }
    return value;
}

ConsoleInterface::ConsoleInterface(HotelSystem& system)
    : hotelSystem(system)
{
    hotelSystem.loadData();
}

void ConsoleInterface::run() {
    if (hotelSystem.getUsersCount() == 0) 
    {
        std::cout << "No users found in the system. Initial setup required." << std::endl;
        std::cout << "Please register the first user (Manager role recommended)." << std::endl;
        handleRegisterNewUser();
        if (hotelSystem.getUsersCount() == 0) 
        {
            std::cout << "Failed to register initial user. Exiting." << std::endl;
            return;
        }
    }


    displayLoginMenu();

    if (hotelSystem.getLoggedInUser()) 
    {
        displayMainMenu();
    }

    hotelSystem.saveData();
    std::cout << "Exiting Hotel Management System. Goodbye!" << std::endl;
}

void ConsoleInterface::displayLoginMenu() 
{
    int choice;
    do 
    {
        std::cout << "\n--- Login Menu ---" << std::endl;
        std::cout << "1. Login" << std::endl;
        if (hotelSystem.getUsersCount() == 0) 
        {
            std::cout << "2. Register Initial User" << std::endl;
        }
        std::cout << "0. Exit System" << std::endl;
        std::cout << "Enter your choice: ";

        choice = readInt();

        switch (choice) 
        {
        case 1: handleLogin(); break;
        case 2:
            if (hotelSystem.getUsersCount() == 0) handleRegisterNewUser();
            else std::cout << "Registration from login menu is for initial setup only." << std::endl;
            break;
        case 0:
            std::cout << "Exiting login..." << std::endl;
            return;
        default: std::cout << "Invalid choice. Please try again." << std::endl; break;
        }
        if (hotelSystem.getLoggedInUser()) 
        {
            break;
        }
    } while (choice != 0);
}

void ConsoleInterface::displayMainMenu() {
    int choice;
    User* currentUser = hotelSystem.getLoggedInUser();
    if (!currentUser) 
    {
        std::cout << "Critical Error: No user logged in to access main menu." << std::endl;
        return;
    }

    do 
    {
        std::cout << "\n--- Main Menu (Logged in as: " << currentUser->getUsername()
            << " (" << currentUser->getRoleString() << ")) ---" << std::endl;
        std::cout << "1. Room Management" << std::endl;
        std::cout << "2. Guest Management" << std::endl;
        std::cout << "3. Reservation Management" << std::endl;
        if (currentUser->getRole() == ROLE_MANAGER) 
        {
            std::cout << "4. User Management" << std::endl;
            std::cout << "5. Pricing Strategy Management" << std::endl;
        }
        std::cout << "6. Reports and Analytics" << std::endl;
        std::cout << "7. Logout" << std::endl;
        std::cout << "0. Save and Exit System" << std::endl;
        std::cout << "Enter your choice: ";

        choice = readInt();

        switch (choice) 
        {
        case 1: displayRoomManagementMenu(); break;
        case 2: displayGuestManagementMenu(); break;
        case 3: displayReservationManagementMenu(); break;
        case 4:
            if (currentUser->getRole() == ROLE_MANAGER) displayUserManagementMenu();
            else std::cout << "Access denied." << std::endl;
            break;
        case 5:
            if (currentUser->getRole() == ROLE_MANAGER) displayPricingStrategyMenu();
            else std::cout << "Access denied." << std::endl;
            break;
        case 6:
            if (currentUser->canViewRevenue()) displayReportAndAnalyticsMenu();
            else std::cout << "Access denied." << std::endl;
            break;
        case 7: handleLogout(); break;
        case 0: break;
        default: std::cout << "Invalid choice. Please try again." << std::endl; break;
        }
    } while (choice != 0 && hotelSystem.getLoggedInUser());
}

void ConsoleInterface::displayRoomManagementMenu() 
{
    User* currentUser = hotelSystem.getLoggedInUser();
    if (!currentUser) return;

    int choice;
    do {
        std::cout << "\n--- Room Management Menu ---" << std::endl;
        if (currentUser->canAddRoom()) std::cout << "1. Add Room" << std::endl;
        std::cout << "2. Update Room Status" << std::endl;
        if (currentUser->canAddRoom()) std::cout << "3. Update Room Base Price" << std::endl;
        std::cout << "4. List Available Rooms for Period" << std::endl;
        std::cout << "5. View All Rooms" << std::endl;
        std::cout << "0. Back to Main Menu" << std::endl;
        std::cout << "Enter your choice: ";
        choice = readInt();

        switch (choice) 
        {
        case 1: if (currentUser->canAddRoom()) handleAddRoom(); else std::cout << "Access denied." << std::endl; break;
        case 2: handleSetRoomStatus(); break;
        case 3: if (currentUser->canAddRoom()) handleUpdateRoomPrice(); else std::cout << "Access denied." << std::endl; break;
        case 4: handleViewAvailableRooms(); break;
        case 5: hotelSystem.printAllRooms(std::cout); break;
        case 0: break;
        default: std::cout << "Invalid choice. Please try again." << std::endl; break;
        }
    } while (choice != 0);
}

void ConsoleInterface::displayGuestManagementMenu() 
{
    User* currentUser = hotelSystem.getLoggedInUser();
    if (!currentUser) return;

    int choice;
    do 
    {
        std::cout << "\n--- Guest Management Menu ---" << std::endl;
        if (currentUser->canMakeReservation()) std::cout << "1. Register Guest" << std::endl;
        std::cout << "2. View All Guests" << std::endl;
        std::cout << "3. View Guest Activity" << std::endl;
        std::cout << "0. Back to Main Menu" << std::endl;
        std::cout << "Enter your choice: ";
        choice = readInt();
        switch (choice) 
        {
        case 1: if (currentUser->canMakeReservation()) handleRegisterGuest(); else std::cout << "Access denied." << std::endl; break;
        case 2: hotelSystem.printAllGuests(std::cout); break;
        case 3: handleViewClientActivity(); break;
        case 0: break;
        default: std::cout << "Invalid choice. Please try again." << std::endl; break;
        }
    } while (choice != 0);
}

void ConsoleInterface::displayReservationManagementMenu() 
{
    User* currentUser = hotelSystem.getLoggedInUser();
    if (!currentUser) return;

    int choice;
    do
    {
        std::cout << "\n--- Reservation Management Menu ---" << std::endl;
        if (currentUser->canMakeReservation()) std::cout << "1. Create Reservation" << std::endl;
        if (currentUser->canMakeReservation()) std::cout << "2. Cancel Reservation" << std::endl;
        std::cout << "3. View Reservation Details" << std::endl;
        std::cout << "4. View All Reservations" << std::endl;
        std::cout << "0. Back to Main Menu" << std::endl;
        std::cout << "Enter your choice: ";
        choice = readInt();
        switch (choice) 
        {
        case 1: if (currentUser->canMakeReservation()) handleCreateReservation(); else std::cout << "Access denied." << std::endl; break;
        case 2: if (currentUser->canMakeReservation()) handleCancelReservation(); else std::cout << "Access denied." << std::endl; break;
        case 3: handleViewReservationDetails(); break;
        case 4: hotelSystem.printAllReservations(std::cout); break;
        case 0: break;
        default: std::cout << "Invalid choice. Please try again." << std::endl; break;
        }
    } while (choice != 0);
}

void ConsoleInterface::displayUserManagementMenu() {
    User* currentUser = hotelSystem.getLoggedInUser();
    if (!currentUser || currentUser->getRole() != ROLE_MANAGER)
    {
        std::cout << "Access Denied." << std::endl;
        return;
    }
    int choice;
    do 
    {
        std::cout << "\n--- User Management Menu (Manager) ---" << std::endl;
        std::cout << "1. Register New User" << std::endl;
        std::cout << "2. List All Users" << std::endl;
        std::cout << "0. Back to Main Menu" << std::endl;
        std::cout << "Enter your choice: ";
        choice = readInt();
        switch (choice) {
        case 1: handleRegisterNewUser(); break;
        case 2: hotelSystem.listAllUsers(std::cout); break;
        case 0: break;
        default: std::cout << "Invalid choice. Please try again." << std::endl; break;
        }
    } while (choice != 0);
}

void ConsoleInterface::displayReportAndAnalyticsMenu() 
{
    User* currentUser = hotelSystem.getLoggedInUser();
    if (!currentUser || !currentUser->canViewRevenue())
    {
        std::cout << "Access Denied." << std::endl;
        return;
    }
    int choice;
    do 
    {
        std::cout << "\n--- Reports and Analytics Menu ---" << std::endl;
        std::cout << "1. Calculate Total Revenue for Period" << std::endl;
        std::cout << "2. Generate Revenue Report (Console/File)" << std::endl;
        std::cout << "3. Generate Occupancy Report (Console/File)" << std::endl;
        std::cout << "4. Generate Guest Loyalty Report (Console/File)" << std::endl;
        std::cout << "5. Generate Room Type Revenue Report (Console/File)" << std::endl;
        std::cout << "0. Back to Main Menu" << std::endl;
        std::cout << "Enter your choice: ";
        choice = readInt();
        switch (choice) {
        case 1: handleCalculateAndDisplayRevenue(); break;
        case 2: handleGenerateReport(ReportType::REVENUE); break;
        case 3: handleGenerateReport(ReportType::OCCUPANCY); break;
        case 4: handleGenerateReport(ReportType::LOYALTY); break;
        case 5: handleGenerateReport(ReportType::ROOM_TYPE_REVENUE); break;
        case 0: break;
        default: std::cout << "Invalid choice. Please try again." << std::endl; break;
        }
    } while (choice != 0);
}

void ConsoleInterface::displayPricingStrategyMenu() {
    User* currentUser = hotelSystem.getLoggedInUser();
    if (!currentUser || currentUser->getRole() != ROLE_MANAGER)
    {
        std::cout << "Access Denied." << std::endl;
        return;
    }
    int choice;
    do 
    {
        std::cout << "\n--- Pricing Strategy Menu (Manager) ---" << std::endl;
        std::cout << "Current Strategy: " << hotelSystem.getCurrentPricingStrategy()->getStrategyName() << std::endl;
        std::cout << "1. Set Default Pricing" << std::endl;
        std::cout << "2. Set Seasonal Pricing" << std::endl;
        std::cout << "3. Set Weekend Pricing" << std::endl;
        std::cout << "0. Back to Main Menu" << std::endl;
        std::cout << "Enter your choice: ";
        choice = readInt();
        switch (choice)
        {
        case 1: hotelSystem.setPricingStrategy(DEFAULT_PRICING); break;
        case 2:
        {
            std::cout << "Enter seasonal multiplier (e.g., 1.15 for +15%): ";
            double multiplier = readDouble();
            hotelSystem.setPricingStrategy(SEASONAL_PRICING, multiplier);
            break;
        }
        case 3: hotelSystem.setPricingStrategy(WEEKEND_PRICING); break;
        case 0: break;
        default: std::cout << "Invalid choice. Please try again." << std::endl; break;
        }
    } while (choice != 0);
}


// Handler Methods
void ConsoleInterface::handleLogin() {
    std::cout << "Enter username: ";
    MyString username = readLine();
    std::cout << "Enter password: ";
    MyString password = readLine();
    hotelSystem.login(username, password);
}

void ConsoleInterface::handleLogout() 
{
    hotelSystem.logout();
    std::cout << "Logged out successfully." << std::endl;
}

void ConsoleInterface::handleAddRoom() 
{
    std::cout << "Enter room number: ";
    unsigned roomNumber = readInt();
    std::cout << "Enter room type (single, double, luxury, apartment, conference): ";
    MyString roomTypeStr = readLine();
    std::cout << "Enter base price per night: ";
    double price = readDouble();
    std::cout << "Enter room capacity: ";
    size_t capacity = readInt();

    hotelSystem.addRoom(roomNumber, roomTypeStr, price, capacity);
}

void ConsoleInterface::handleUpdateRoomPrice() 
{
    std::cout << "Enter room number to update price: ";
    unsigned roomNumber = readInt();
    std::cout << "Enter new base price per night: ";
    double newPrice = readDouble();
    hotelSystem.updateRoomPrice(roomNumber, newPrice);
}


void ConsoleInterface::handleSetRoomStatus()
{
    std::cout << "Enter room number: ";
    unsigned roomNumber = readInt();
    std::cout << "Enter new status (0 for Available, 1 for Reserved, 2 for Maintenance): ";
    int statusChoice = readInt();
    RoomStatus newStatus;
    switch (statusChoice)
    {
    case 0: newStatus = RoomStatus::AVAILABLE; break;
    case 1: newStatus = RoomStatus::RESERVED; break;
    case 2: newStatus = RoomStatus::MAINTENANCE; break;
    default:
        std::cout << "Invalid status choice. No change made." << std::endl;
        return;
    }
    hotelSystem.updateRoomStatus(roomNumber, newStatus);
}

void ConsoleInterface::handleViewAvailableRooms()
{
    std::cout << "Enter start date (DD.MM.YYYY): ";
    MyString startDate = readLine();
    std::cout << "Enter end date (DD.MM.YYYY): ";
    MyString endDate = readLine();
    hotelSystem.listAvailableRooms(startDate, endDate);
}


void ConsoleInterface::handleRegisterGuest()
{
    std::cout << "Enter guest name: ";
    MyString name = readLine();
    std::cout << "Enter guest phone: ";
    MyString phone = readLine();
    std::cout << "Enter guest email: ";
    MyString email = readLine();

    hotelSystem.registerGuest(name, phone, email);
}

void ConsoleInterface::handleViewClientActivity() 
{
    std::cout << "Enter guest name to view activity: ";
    MyString guestName = readLine();
    hotelSystem.printGuestActivity(guestName, std::cout);
}


void ConsoleInterface::handleCreateReservation() 
{
    std::cout << "Enter room number for reservation: ";
    unsigned roomNumber = readInt();
    std::cout << "Enter guest name: ";
    MyString guestName = readLine();
    std::cout << "Enter start date (DD.MM.YYYY): ";
    MyString startDate = readLine();
    std::cout << "Enter end date (DD.MM.YYYY): ";
    MyString endDate = readLine();

    hotelSystem.createReservation(roomNumber, guestName, startDate, endDate);
}

void ConsoleInterface::handleCancelReservation() 
{
    std::cout << "Enter reservation ID to cancel: ";
    unsigned reservationId = readInt();
    hotelSystem.cancelReservation(reservationId);
}

void ConsoleInterface::handleViewReservationDetails()
{
    std::cout << "Enter reservation ID to view details: ";
    unsigned reservationId = readInt();
    hotelSystem.viewReservationDetails(reservationId, std::cout);
}


void ConsoleInterface::handleRegisterNewUser()
{
    std::cout << "Enter desired username: ";
    MyString username = readLine();
    std::cout << "Enter password: ";
    MyString password = readLine();
    std::cout << "Enter role (0 for Manager, 1 for Receptionist, 2 for Accountant): ";
    int roleChoice = readInt();
    UserRole role;
    switch (roleChoice)
    {
    case 0: role = ROLE_MANAGER; break;
    case 1: role = ROLE_RECEPTIONIST; break;
    case 2: role = ROLE_ACCOUNTANT; break;
    default:
        std::cout << "Invalid role choice. User not registered." << std::endl;
        role = UNKNOWN_ROLE;
        return;
    }
    hotelSystem.registerUser(username, password, role);
}


void ConsoleInterface::handleCalculateAndDisplayRevenue() 
{
    std::cout << "Enter start date for revenue calculation (DD.MM.YYYY): ";
    MyString startDate = readLine();
    std::cout << "Enter end date (DD.MM.YYYY): ";
    MyString endDate = readLine();
    double revenue = hotelSystem.calculateTotalRevenueForPeriod(startDate, endDate);
    std::cout << "Total revenue for period " << startDate << " to " << endDate << ": " << revenue << std::endl;
}

void ConsoleInterface::handleGenerateReport(ReportType type)
{
    MyString startDate, endDate, monthYear, fileName;
    bool toFile = false;
    std::cout << "Output to (1. Console, 2. File): ";
    int outputChoice = readInt();
    if (outputChoice == 2)
    {
        toFile = true;
        std::cout << "Enter filename for report (e.g., report.txt): ";
        fileName = readLine();
    }

    std::ostream* outStream = &std::cout;
    std::ofstream fileStream;
    if (toFile)
    {
        fileStream.open(fileName.c_str());
        if (!fileStream.is_open())
        {
            std::cout << "Error: Could not open file " << fileName << " for writing. Outputting to console." << std::endl;
            outStream = &std::cout;
        }
        else
        {
            outStream = &fileStream;
        }
    }

    switch (type)
    {
    case ReportType::REVENUE:
    case ReportType::ROOM_TYPE_REVENUE:
        std::cout << "Enter start date (DD.MM.YYYY): "; startDate = readLine();
        std::cout << "Enter end date (DD.MM.YYYY): "; endDate = readLine();
        if (type == ReportType::REVENUE) hotelSystem.generateRevenueReport(startDate, endDate, *outStream);
        else hotelSystem.generateRoomTypeRevenueReport(startDate, endDate, *outStream);
        break;
    case ReportType::OCCUPANCY:
        std::cout << "Enter month and year (MM.YYYY): "; monthYear = readLine();
        hotelSystem.generateOccupancyReport(monthYear, *outStream);
        break;
    case ReportType::LOYALTY:
        hotelSystem.generateLoyaltyReport(*outStream);
        break;
    }

    if (toFile && fileStream.is_open())
    {
        fileStream.close();
        std::cout << "Report generated to file: " << fileName << std::endl;
    }
}