#include "HotelSystem.h"
#include "SingleRoom.h"
#include "DoubleRoom.h"
#include "LuxuryRoom.h"
#include "Apartment.h"
#include "ConferenceHall.h"
MyString intToMyString(int value) 
{
    if (value == 0)
    {
        return MyString("0");
    }
    char buffer[20];
    int i = 19;
    buffer[i--] = '\0';
    bool negative = false;
    if (value < 0) {
        negative = true;
        value = -value;
    }
    if (value == 0)
    {
        buffer[i--] = '0';
    }
    while (value > 0)
    {
        buffer[i--] = (value % 10) + '0';
        value /= 10;
    }
    if (negative)
    {
        buffer[i--] = '-';
    }
    return MyString(buffer + i + 1);
}

MyString doubleToMyString(double value, int precision) 
{
    if (precision < 0) precision = 0;
    if (precision > 6) precision = 6;

    char buffer[50];
    int idx = 0;

    if (value < 0) 
    {
        buffer[idx++] = '-';
        value = -value;
    }

    long long intPart = static_cast<long long>(value);
    MyString intStr = intToMyString(static_cast<int>(intPart));
    for (size_t i = 0; i < intStr.getSize(); i++) 
    {
        buffer[idx++] = intStr.c_str()[i];
    }

    if (precision > 0)
    {
        buffer[idx++] = '.';
        double fracPart = value - intPart;
        for (int i = 0; i < precision; i++)
        {
            fracPart *= 10;
            int digit = static_cast<int>(fracPart);
            buffer[idx++] = digit + '0';
            fracPart -= digit;
        }
    }
    buffer[idx] = '\0';
    return MyString(buffer);
}

HotelSystem::HotelSystem()
    : loggedInUser(nullptr), currentPricingStrategy(nullptr)
{
    currentPricingStrategy = new DefaultPricingStrategy();
}

HotelSystem::~HotelSystem()
{
    for (size_t i = 0; i < rooms.getSize(); i++)
    {
        delete rooms[i];
    }
    for (size_t i = 0; i < users.getSize(); i++)
    {
        delete users[i];
    }
    delete currentPricingStrategy;
}

//Find Methods
Room* HotelSystem::findRoom(unsigned roomNumber)
{
    for (size_t i = 0; i < rooms.getSize(); i++) 
    {
        if (rooms[i] != nullptr && rooms[i]->getRoomNumber() == roomNumber)
        {
            return rooms[i];
        }
    }
    return nullptr;
}
const Room* HotelSystem::findRoom(unsigned roomNumber) const
{

    for (size_t i = 0; i < rooms.getSize(); i++) 
    {
        if (rooms[i] != nullptr && rooms[i]->getRoomNumber() == roomNumber) 
        {
            return rooms[i];
        }
    }
    return nullptr;
}

Guest* HotelSystem::findGuest(unsigned guestId) 
{
    for (size_t i = 0; i < guests.getSize(); i++)
    {
        if (guests[i].getClientNumber() == guestId) 
        {
            return &guests[i];
        }
    }
    return nullptr;
}
const Guest* HotelSystem::findGuest(unsigned guestId) const 
{
    for (size_t i = 0; i < guests.getSize(); i++)
    {
        if (guests[i].getClientNumber() == guestId)
        {
            return &guests[i];
        }
    }
    return nullptr;
}

Guest* HotelSystem::findGuestByName(const MyString& guestName) 
{
    for (size_t i = 0; i < guests.getSize(); i++) 
    {
        if (guests[i].getName() == guestName)
        {
            return &guests[i];
        }
    }
    return nullptr;
}
const Guest* HotelSystem::findGuestByName(const MyString& guestName) const {
    for (size_t i = 0; i < guests.getSize(); i++)
    {
        if (guests[i].getName() == guestName)
        {
            return &guests[i];
        }
    }
    return nullptr;
}

Reservation* HotelSystem::findReservation(unsigned reservationId)
{
    for (size_t i = 0; i < reservations.getSize(); i++) 
    {
        if (reservations[i].getReservationId() == reservationId)
        {
            return &reservations[i];
        }
    }
    return nullptr;
}

User* HotelSystem::findUserByUsername(const MyString& username)
{
    for (size_t i = 0; i < users.getSize(); i++) 
    {
        if (users[i] != nullptr && users[i]->getUsername() == username)
        {
            return users[i];
        }
    }
    return nullptr;
}

//Room Management
void HotelSystem::addRoom(unsigned roomNumber, const MyString& roomTypeStr, double pricePerNight, size_t capacity) 
{
    if (!loggedInUser || !loggedInUser->canAddRoom()) 
    {
        std::cout << "Access Denied: You do not have permission to add rooms." << std::endl;
        return;
    }
    if (findRoom(roomNumber))
    {
        std::cout << "Error: Room number " << roomNumber << " already exists." << std::endl;
        return;
    }
    if (pricePerNight < 0) 
    {
        std::cout << "Error: Room price cannot be negative." << std::endl;
        return;
    }
    Room* newRoom = nullptr;
    if (roomTypeStr == "single" || roomTypeStr == "SingleRoom") 
    {
        newRoom = new SingleRoom(roomNumber, pricePerNight, (capacity > 0 ? capacity : 1));
    }
    else if (roomTypeStr == "double" || roomTypeStr == "DoubleRoom")
    {
        newRoom = new DoubleRoom(roomNumber, pricePerNight, (capacity > 0 ? capacity : 2));
    }
    else if (roomTypeStr == "luxury" || roomTypeStr == "LuxuryRoom")
    {
        newRoom = new LuxuryRoom(roomNumber, pricePerNight, (capacity > 0 ? capacity : 1));
    }
    else if (roomTypeStr == "apartment" || roomTypeStr == "Apartment") 
    {
        newRoom = new Apartment(roomNumber, pricePerNight, (capacity > 0 ? capacity : 2));
    }
    else if (roomTypeStr == "conference" || roomTypeStr == "ConferenceHall")
    {
        newRoom = new ConferenceHall(roomNumber, pricePerNight, capacity);
    }
    else
    {
        std::cout << "Error: Unknown room type '" << roomTypeStr << "' specified." << std::endl;
        return;
    }

    if (newRoom) 
    {
        rooms.push_back(newRoom);
        std::cout << "Room " << newRoom->getRoomNumber() << " (" << newRoom->getRoomType()
            << ") with capacity " << intToMyString(newRoom->getCapacity()).c_str() << " added successfully." << std::endl;
    }
}

void HotelSystem::updateRoomStatus(unsigned roomNumber, RoomStatus newStatus) 
{
    if (!loggedInUser || (!loggedInUser->canAddRoom() && !loggedInUser->canMakeReservation()))
    {
        std::cout << "Access Denied: You do not have permission to update room status." << std::endl;
        return;
    }
    Room* room = findRoom(roomNumber);
    if (room) 
    {
        room->setStatus(newStatus);
        std::cout << "Status for room " << intToMyString(roomNumber).c_str() << " updated to " << room->getStatusString() << "." << std::endl;
    }
    else 
    {
        std::cout << "Error: Room " << intToMyString(roomNumber).c_str() << " not found." << std::endl;
    }
}

void HotelSystem::updateRoomPrice(unsigned roomNumber, double newPrice) 
{
    if (!loggedInUser || !loggedInUser->canAddRoom()) {
        std::cout << "Access Denied: You do not have permission to update room prices." << std::endl;
        return;
    }
    Room* room = findRoom(roomNumber);
    if (room) 
    {
        if (newPrice >= 0)
        {
            room->setBasePricePerNight(newPrice);
            std::cout << "Base price for room " << intToMyString(roomNumber).c_str() << " updated to " << doubleToMyString(newPrice).c_str() << "." << std::endl;
        }
        else 
        {
            std::cout << "Error: New price cannot be negative." << std::endl;
        }
    }
    else 
    {
        std::cout << "Error: Room " << intToMyString(roomNumber).c_str() << " not found." << std::endl;
    }
}

void HotelSystem::listAvailableRooms(const MyString& startDateStr, const MyString& endDateStr, std::ostream& os) const 
{
    time_t reqStartDate = parseDateStringToTimeT(startDateStr);
    time_t reqEndDate = parseDateStringToTimeT(endDateStr);

    if (reqStartDate == INVALID_TIME_T_VALUE || reqEndDate == INVALID_TIME_T_VALUE) 
    {
        os << "Error: Invalid date format provided for availability check." << std::endl;
        return;
    }
    if (reqStartDate >= reqEndDate) 
    {
        os << "Error: Start date must be strictly before end date for availability check." << std::endl;
        return;
    }

    os << "--- Available Rooms from " << startDateStr << " to " << endDateStr << " ---" << std::endl;
    bool foundAnyAvailable = false;
    for (size_t i = 0; i < rooms.getSize(); i++) 
    {
        const Room* currentRoom = rooms[i];
        if (!currentRoom) continue;

        if (currentRoom->getStatus() == RoomStatus::MAINTENANCE)
        {
            continue;
        }

        bool isBookedInPeriod = false;
        for (size_t j = 0; j < reservations.getSize(); j++)
        {
            if (reservations[j].getRoomNumber() == currentRoom->getRoomNumber()) 
            {
                if (reservations[j].overlapsWith(reqStartDate, reqEndDate)) 
                {
                    isBookedInPeriod = true;
                    break;
                }
            }
        }

        if (!isBookedInPeriod) 
        {
            os << "- Room " << intToMyString(currentRoom->getRoomNumber()).c_str()
                << " (" << currentRoom->getRoomType() << ")"
                << ", Capacity: " << intToMyString(currentRoom->getCapacity()).c_str()
                << ", Price/Night: " << doubleToMyString(currentRoom->getBasePricePerNight()).c_str()
                << std::endl;
            foundAnyAvailable = true;
        }
    }
    if (!foundAnyAvailable) 
    {
        os << "No rooms are available for the selected period." << std::endl;
    }
    os << "-----------------------------------------------------" << std::endl;
}

void HotelSystem::printAllRooms(std::ostream& os) const 
{
    os << "--- All Registered Rooms ---" << std::endl;
    if (rooms.getSize() == 0) 
    {
        os << "No rooms are currently registered in the system." << std::endl;
    }
    else {
        for (size_t i = 0; i < rooms.getSize(); i++) 
        {
            const Room* room = rooms[i];
            if (!room) continue;
            os << "Room #: " << intToMyString(room->getRoomNumber()).c_str()
                << " | Type: " << room->getRoomType()
                << " | Capacity: " << intToMyString(room->getCapacity()).c_str()
                << " | Price: " << doubleToMyString(room->getBasePricePerNight()).c_str()
                << " | Status: " << room->getStatusString()
                << std::endl;
        }
    }
    os << "----------------------------" << std::endl;
}

//Guest Management
void HotelSystem::registerGuest(const MyString& name, const MyString& phone, const MyString& email) {
    if (!loggedInUser || !loggedInUser->canMakeReservation()) 
    {
        std::cout << "Access Denied: You do not have permission to register guests." << std::endl;
        return;
    }
    if (name.getSize() == 0 || phone.getSize() == 0 || email.getSize() == 0) 
    {
        std::cout << "Error: Guest name, phone, and email cannot be empty." << std::endl;
        return;
    }
    for (size_t i = 0; i < guests.getSize(); i++) 
    {
        if (guests[i].getMail() == email) 
        {
            std::cout << "Warning: A guest with email '" << email << "' already exists (Client ID: "
                << intToMyString(guests[i].getClientNumber()).c_str() << ", Name: " << guests[i].getName() << ")." << std::endl;
            std::cout << "Proceeding with registration of new guest. Ensure this is intended." << std::endl;
            break;
        }
    }
    guests.push_back(Guest(name, phone, email));
    std::cout << "Guest '" << name << "' registered successfully. Client ID: " << intToMyString(Guest::getNextClientNumber() - 1).c_str() << "." << std::endl;
}

void HotelSystem::printAllGuests(std::ostream& os) const {
    os << "--- All Registered Guests ---" << std::endl;
    if (guests.getSize() == 0) 
    {
        os << "No guests are currently registered in the system." << std::endl;
    }
    else 
    {
        for (size_t i = 0; i < guests.getSize(); i++)
        {
            const Guest& g = guests[i];
            os << "ID: " << intToMyString(g.getClientNumber()).c_str()
                << " | Name: " << g.getName()
                << " | Phone: " << g.getMobileNumber()
                << " | Email: " << g.getMail()
                << " | Status: " << g.getStatusString()
                << " | Total Nights: " << intToMyString(g.getTotalNights()).c_str()
                << " | Total Spent: " << doubleToMyString(g.getTotalSpent()).c_str()
                << std::endl;
        }
    }
    os << "-----------------------------" << std::endl;
}

void HotelSystem::printGuestActivity(const MyString& guestName, std::ostream& os) const
{
    const Guest* guest = findGuestByName(guestName);
    if (!guest) 
    {
        os << "Error: Guest '" << guestName << "' not found." << std::endl;
        return;
    }
    os << "--- Activity Report for Guest: " << guest->getName() << " (ID: " << intToMyString(guest->getClientNumber()).c_str() << ") ---" << std::endl;
    os << "Loyalty Status: " << guest->getStatusString() << std::endl;
    os << "Total Nights Stayed: " << intToMyString(guest->getTotalNights()).c_str() << std::endl;
    os << "Total Amount Spent: " << doubleToMyString(guest->getTotalSpent()).c_str() << std::endl;
    os << "Reservation History:" << std::endl;

    bool foundReservations = false;
    for (size_t i = 0; i < reservations.getSize(); i++)
    {
        if (reservations[i].getGuestId() == guest->getClientNumber()) 
        {
            const Reservation& res = reservations[i];
            const Room* room = findRoom(res.getRoomNumber());
            os << "  - Reservation ID: " << intToMyString(res.getReservationId()).c_str()
                << ", Room #: " << intToMyString(res.getRoomNumber()).c_str() << (room ? " (" + room->getRoomType() + ")" : " (Room info unavailable)")
                << ", Period: " << res.getStartDateString() << " to " << res.getEndDateString()
                << ", Price Paid: " << doubleToMyString(res.getTotalPrice()).c_str()
                << std::endl;
            foundReservations = true;
        }
    }
    if (!foundReservations) 
    {
        os << "  No reservations found for this guest." << std::endl;
    }
    os << "---------------------------------------------------------" << std::endl;
}

//Reservation Management
double HotelSystem::getDiscountFactorForGuest(GuestStatus status) const
{
    switch (status) 
    {
    case PLATINUM: return 0.85;
    case GOLD:     return 0.90;
    case NORMAL:
    default:       return 1.0;
    }
}

bool HotelSystem::createReservation(unsigned roomNumber, const MyString& guestName,
    const MyString& startDateStr, const MyString& endDateStr) 
{
    if (!loggedInUser || !loggedInUser->canMakeReservation()) 
    {
        std::cout << "Access Denied: You do not have permission to create reservations." << std::endl;
        return false;
    }
    Room* room = findRoom(roomNumber);
    if (!room) 
    {
        std::cout << "Error: Room " << intToMyString(roomNumber).c_str() << " does not exist." << std::endl;
        return false;
    }
    Guest* guest = findGuestByName(guestName);
    if (!guest) 
    {
        std::cout << "Error: Guest '" << guestName << "' not found. Please register the guest first." << std::endl;
        return false;
    }
    time_t reqStartDate = parseDateStringToTimeT(startDateStr);
    time_t reqEndDate = parseDateStringToTimeT(endDateStr);

    if (reqStartDate == INVALID_TIME_T_VALUE || reqEndDate == INVALID_TIME_T_VALUE) 
    {
        std::cout << "Error: Invalid date format used for reservation period." << std::endl;
        return false;
    }
    if (reqStartDate >= reqEndDate)
    {
        std::cout << "Error: Reservation start date must be strictly before the end date." << std::endl;
        return false;
    }
    time_t currentTime_val;
    time(&currentTime_val);
    tm* now_tm_val = localtime(&currentTime_val);
    if (now_tm_val) 
    {
        now_tm_val->tm_hour = 0; now_tm_val->tm_min = 0; now_tm_val->tm_sec = 0;
        time_t today_start_of_day = mktime(now_tm_val);
        if (reqStartDate < today_start_of_day) 
        {
            std::cout << "Error: Reservation start date cannot be in the past." << std::endl;
            return false;
        }
    }
    else 
    {
        std::cerr << "Error: Could not determine current time for date validation." << std::endl;
        return false;
    }

    if (room->getStatus() == RoomStatus::MAINTENANCE)
    {
        std::cout << "Error: Room " << intToMyString(roomNumber).c_str() << " is currently under maintenance and cannot be booked." << std::endl;
        return false;
    }
    for (size_t i = 0; i < reservations.getSize(); i++) 
    {
        if (reservations[i].getRoomNumber() == roomNumber) 
        {
            if (reservations[i].overlapsWith(reqStartDate, reqEndDate)) 
            {
                std::cout << "Error: Room " << intToMyString(roomNumber).c_str() << " is already booked for the selected period or part of it." << std::endl;
                std::cout << "  Conflict with Reservation ID: " << intToMyString(reservations[i].getReservationId()).c_str()
                    << " (Period: " << reservations[i].getStartDateString()
                    << " to " << reservations[i].getEndDateString() << ")" << std::endl;
                return false;
            }
        }
    }
    int nights = Reservation(0, 0, reqStartDate, reqEndDate, 0).getNumberOfNights();
    if (nights <= 0) 
    {
        std::cout << "Error: The reservation must be for at least 1 night." << std::endl;
        return false;
    }
    if (!currentPricingStrategy) {
        std::cerr << "Critical Error: No pricing strategy set. Cannot calculate price." << std::endl;
        currentPricingStrategy = new DefaultPricingStrategy();
    }
    double baseCalculatedPrice = room->calculatePrice(nights, *currentPricingStrategy);
    double discountFactor = getDiscountFactorForGuest(guest->getStatus());
    double finalPrice = baseCalculatedPrice * discountFactor;

    Reservation newReservation(guest->getClientNumber(), roomNumber, reqStartDate, reqEndDate, finalPrice);
    reservations.push_back(newReservation);
    guest->addStay(nights, finalPrice);

    std::cout << "Reservation " << intToMyString(newReservation.getReservationId()).c_str() << " created successfully for Room " << intToMyString(roomNumber).c_str()
        << " by Guest '" << guestName << "' (ID: " << intToMyString(guest->getClientNumber()).c_str() << ")." << std::endl;
    std::cout << "  Period: " << startDateStr << " to " << endDateStr << " (" << intToMyString(nights).c_str() << " nights)." << std::endl;
    std::cout << "  Total Price: " << doubleToMyString(finalPrice).c_str();
    if (discountFactor < 1.0) {
        int discount_percent_int = static_cast<int>((1.0 - discountFactor) * 100.0 + 0.5);
        std::cout << " (after " << intToMyString(discount_percent_int).c_str() << "% " << guest->getStatusString() << " discount)";
    }
    std::cout << "." << std::endl;
    return true;
}

bool HotelSystem::cancelReservation(unsigned reservationId) {
    if (!loggedInUser || !loggedInUser->canMakeReservation())
    {
        std::cout << "Access Denied: You do not have permission to cancel reservations." << std::endl;
        return false;
    }
    for (size_t i = 0; i < reservations.getSize(); +i++) 
    {
        if (reservations[i].getReservationId() == reservationId) 
        {
            time_t currentTime_val;
            time(&currentTime_val);
            if (reservations[i].getEndDateRaw() < currentTime_val) 
            {
                std::cout << "Warning: Cancelling a reservation (ID: " << intToMyString(reservationId).c_str()
                    << ") whose period has already ended." << std::endl;
            }
            else if (reservations[i].getStartDateRaw() < currentTime_val) {
                std::cout << "Warning: Cancelling a reservation (ID: " << intToMyString(reservationId).c_str()
                    << ") that is currently active or has partially passed." << std::endl;
            }
            reservations.erase(i);
            std::cout << "Reservation " << intToMyString(reservationId).c_str() << " has been cancelled." << std::endl;
            return true;
        }
    }
    std::cout << "Error: Reservation with ID " << intToMyString(reservationId).c_str() << " not found." << std::endl;
    return false;
}

void HotelSystem::viewReservationDetails(unsigned reservationId, std::ostream& os) const {
    const Reservation* res_ptr = nullptr;
    for (size_t i = 0; i < reservations.getSize(); i++) 
    {
        if (reservations[i].getReservationId() == reservationId) {
            res_ptr = &reservations[i];
            break;
        }
    }
    if (res_ptr) 
    {
        const Reservation& res = *res_ptr;
        const Guest* guest = findGuest(res.getGuestId());
        const Room* room = findRoom(res.getRoomNumber());
        os << "--- Reservation Details (ID: " << intToMyString(res.getReservationId()).c_str() << ") ---" << std::endl;
        os << "Room #: " << intToMyString(res.getRoomNumber()).c_str()
            << (room ? " (Type: " + room->getRoomType() + ")" : " (Room details unavailable)") << std::endl;
        os << "Guest: " << (guest ? guest->getName() : MyString("Unknown Guest"))
            << " (Client ID: " << intToMyString(res.getGuestId()).c_str() << ")" << std::endl;
        os << "Period: " << res.getStartDateString() << " to " << res.getEndDateString()
            << " (" << intToMyString(res.getNumberOfNights()).c_str() << " nights)" << std::endl;
        os << "Total Price: " << doubleToMyString(res.getTotalPrice()).c_str() << std::endl;
        os << "------------------------------------" << std::endl;
    }
    else
    {
        os << "Error: Reservation with ID " << intToMyString(reservationId).c_str() << " not found." << std::endl;
    }
}

void HotelSystem::printAllReservations(std::ostream& os) const
{
    os << "--- All Current Reservations ---" << std::endl;
    if (reservations.getSize() == 0) {
        os << "There are no reservations in the system." << std::endl;
    }
    else
    {
        for (size_t i = 0; i < reservations.getSize(); i++)
        {
            const Reservation& res = reservations[i];
            const Guest* guest = findGuest(res.getGuestId());
            const Room* room = findRoom(res.getRoomNumber());
            os << "ID: " << intToMyString(res.getReservationId()).c_str()
                << " | Room: " << intToMyString(res.getRoomNumber()).c_str() << (room ? " (" + room->getRoomType() + ")" : "")
                << " | Guest: " << (guest ? guest->getName() : MyString("ID:") + intToMyString(res.getGuestId()))
                << " | Period: " << res.getStartDateString() << " - " << res.getEndDateString()
                << " | Price: " << doubleToMyString(res.getTotalPrice()).c_str()
                << std::endl;
        }
    }
    os << "--------------------------------" << std::endl;
}

//User Management
void HotelSystem::registerUser(const MyString& username, const MyString& password, UserRole role) {
    if (users.getSize() > 0) 
    {
        if (!loggedInUser || loggedInUser->getRole() != ROLE_MANAGER) 
        {
            std::cout << "Access Denied: Only Managers can register new users once the system is set up." << std::endl;
            return;
        }
    }
    else 
    {
        if (role != ROLE_MANAGER) 
        {
            std::cout << "Error: The first user registered in the system must be a Manager." << std::endl;
            return;
        }
        std::cout << "Initial system setup: Registering the first user (Manager)." << std::endl;
    }
    if (findUserByUsername(username))
    {
        std::cout << "Error: Username '" << username << "' is already taken." << std::endl;
        return;
    }
    if (username.getSize() == 0 || password.getSize() == 0) 
    {
        std::cout << "Error: Username and password cannot be empty." << std::endl;
        return;
    }
    if (password.getSize() < 4) 
    {
        std::cout << "Error: Password must be at least 4 characters long for security." << std::endl;
        return;
    }
    User* newUser = nullptr;
    switch (role) 
    {
    case ROLE_MANAGER:    newUser = new Manager(username, password); break;
    case ROLE_RECEPTIONIST: newUser = new Receptionist(username, password); break;
    case ROLE_ACCOUNTANT: newUser = new Accountant(username, password); break;
    default:
        std::cout << "Error: Invalid or unknown user role specified. User not registered." << std::endl;
        return;
    }
    users.push_back(newUser);
    std::cout << "User '" << username << "' registered successfully as " << userRoleToString(role) << "." << std::endl;
}

bool HotelSystem::login(const MyString& username, const MyString& password) {
    if (loggedInUser) 
    {
        std::cout << "Error: User '" << loggedInUser->getUsername()
            << "' is already logged in. Please logout first." << std::endl;
        return false;
    }
    User* userToLogin = findUserByUsername(username);
    if (userToLogin) 
    {
        if (userToLogin->checkPassword(password)) 
        {
            loggedInUser = userToLogin;
            std::cout << "Login successful. Welcome, " << loggedInUser->getUsername()
                << " (" << loggedInUser->getRoleString() << ")." << std::endl;
            return true;
        }
        else 
        {
            std::cout << "Error: Invalid password for username '" << username << "'." << std::endl;
            return false;
        }
    }
    std::cout << "Error: Username '" << username << "' not found." << std::endl;
    return false;
}

void HotelSystem::logout() {
    if (loggedInUser) 
    {
        std::cout << "User '" << loggedInUser->getUsername() << "' has been logged out." << std::endl;
        loggedInUser = nullptr;
    }
    else
    {
        std::cout << "No user is currently logged in to logout." << std::endl;
    }
}

void HotelSystem::listAllUsers(std::ostream& os) const
{
    os << "--- All Registered Users ---" << std::endl;
    if (users.getSize() == 0)
    {
        os << "No users are currently registered in the system." << std::endl;
    }
    else 
    {
        for (size_t i = 0; i < users.getSize(); i++) 
        {
            const User* user = users[i];
            if (!user) continue;
            os << "Username: " << user->getUsername()
                << " | Role: " << user->getRoleString()
                << std::endl;
        }
    }
    os << "----------------------------" << std::endl;
}

//Pricing Strategy Management
void HotelSystem::setPricingStrategy(PricingStrategyType type, double arg) {
    if (!loggedInUser || loggedInUser->getRole() != ROLE_MANAGER) {
        std::cout << "Access Denied: Only Managers can change the pricing strategy." << std::endl;
        return;
    }
    delete currentPricingStrategy;
    currentPricingStrategy = nullptr;
    switch (type) {
    case DEFAULT_PRICING:
        currentPricingStrategy = new DefaultPricingStrategy();
        break;
    case SEASONAL_PRICING:
        currentPricingStrategy = new SeasonalPricingStrategy(arg > 0 ? arg : 1.15);
        break;
    case WEEKEND_PRICING:
        currentPricingStrategy = new WeekendPricingStrategy();
        break;
    default:
        std::cout << "Warning: Unknown pricing strategy type requested. Reverting to Default Pricing." << std::endl;
        currentPricingStrategy = new DefaultPricingStrategy();
        break;
    }
    std::cout << "Pricing strategy has been updated to: " << currentPricingStrategy->getStrategyName() << std::endl;
}

const PricingStrategy* HotelSystem::getCurrentPricingStrategy() const {
    return currentPricingStrategy;
}

//Reports and Analytics
double HotelSystem::calculateTotalRevenueForPeriod(const MyString& startDateStr, const MyString& endDateStr) const {
    if (!loggedInUser || !loggedInUser->canViewRevenue()) 
    {
        std::cout << "Access Denied: You do not have permission to calculate revenue." << std::endl;
        return 0.0;
    }
    time_t periodStart = parseDateStringToTimeT(startDateStr);
    time_t periodEnd = parseDateStringToTimeT(endDateStr);
    if (periodStart == INVALID_TIME_T_VALUE || periodEnd == INVALID_TIME_T_VALUE || periodStart >= periodEnd) 
    {
        std::cout << "Error: Invalid date period specified for revenue calculation." << std::endl;
        return 0.0;
    }
    double totalRevenue = 0.0;
    for (size_t i = 0; i < reservations.getSize(); i++)
    {
        const Reservation& res = reservations[i];
        if (res.getStartDateRaw() >= periodStart && res.getStartDateRaw() < periodEnd) 
        {
            totalRevenue += res.getTotalPrice();
        }
    }
    return totalRevenue;
}

void HotelSystem::generateRevenueReport(const MyString& startDateStr, const MyString& endDateStr, std::ostream& os) const {
    if (!loggedInUser || !loggedInUser->canViewRevenue())
    {
        os << "Access Denied: You do not have permission to generate this report." << std::endl;
        return;
    }
    os << "--- Revenue Report (Period: " << startDateStr << " - " << endDateStr << ") ---" << std::endl;
    time_t periodStart = parseDateStringToTimeT(startDateStr);
    time_t periodEnd = parseDateStringToTimeT(endDateStr);
    if (periodStart == INVALID_TIME_T_VALUE || periodEnd == INVALID_TIME_T_VALUE || periodStart >= periodEnd)
    {
        os << "Error: Invalid date period specified for the report." << std::endl;
        return;
    }
    double overallTotalRevenue = 0.0;
    bool foundAnyRevenue = false;
    for (size_t i = 0; i < reservations.getSize(); i++)
    {
        const Reservation& res = reservations[i];
        if (res.getStartDateRaw() >= periodStart && res.getStartDateRaw() < periodEnd)
        {
            const Room* room = findRoom(res.getRoomNumber());
            const Guest* guest = findGuest(res.getGuestId());
            os << "  Reservation ID: " << intToMyString(res.getReservationId()).c_str()
                << " | Room: " << intToMyString(res.getRoomNumber()).c_str() << (room ? " (" + room->getRoomType() + ")" : " (N/A)")
                << " | Guest: " << (guest ? guest->getName() : MyString("ID:") + intToMyString(res.getGuestId()))
                << " | Period: " << res.getStartDateString() << " - " << res.getEndDateString()
                << " | Amount: " << doubleToMyString(res.getTotalPrice()).c_str()
                << std::endl;
            overallTotalRevenue += res.getTotalPrice();
            foundAnyRevenue = true;
        }
    }
    if (!foundAnyRevenue) 
    {
        os << "No reservations contributing to revenue found within this period." << std::endl;
    }
    os << "--------------------------------------------------------" << std::endl;
    os << "Total Revenue for the Period: " << doubleToMyString(overallTotalRevenue).c_str() << std::endl;
    os << "--------------------------------------------------------" << std::endl;
}

void HotelSystem::generateOccupancyReport(const MyString& monthYearStr, std::ostream& os) const {
    if (!loggedInUser || !loggedInUser->canViewRevenue())
    {
        os << "Access Denied: You do not have permission to generate occupancy reports." << std::endl;
        return;
    }
    if (monthYearStr.getSize() != 7 || monthYearStr.c_str()[2] != '.')
    {
        os << "Error: Invalid month.year format (MM.YYYY expected)." << std::endl;
        return;
    }
    MyString monthStr = monthYearStr.substr(0, 2);
    MyString yearStr = monthYearStr.substr(3, 4);
    int targetMonth = (monthStr.c_str()[0] - '0') * 10 + (monthStr.c_str()[1] - '0');
    int targetYear = (yearStr.c_str()[0] - '0') * 1000 + (yearStr.c_str()[1] - '0') * 100 +
        (yearStr.c_str()[2] - '0') * 10 + (yearStr.c_str()[3] - '0');

    if (targetMonth < 1 || targetMonth > 12 || targetYear < 1900 || targetYear > 2200)
    {
        os << "Error: Invalid month or year parsed from input '" << monthYearStr << "'." << std::endl;
        return;
    }
    os << "--- Occupancy Report for " << monthStr << "." << yearStr << " ---" << std::endl;
    unsigned totalOccupiedRoomNightsInMonth = 0;
    int daysInMonthArray[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    bool isLeap = (targetYear % 4 == 0 && (targetYear % 100 != 0 || targetYear % 400 == 0));
    if (targetMonth == 2 && isLeap) daysInMonthArray[2] = 29;
    int daysInTargetMonth = daysInMonthArray[targetMonth];
    unsigned totalPossibleRoomNights = rooms.getSize() * daysInTargetMonth;

    for (size_t i = 0; i < reservations.getSize(); i++)
    {
        const Reservation& res = reservations[i];
        time_t resNight = res.getStartDateRaw();
        if (resNight == INVALID_TIME_T_VALUE) continue;
        for (int nightIdx = 0; nightIdx < res.getNumberOfNights(); ++nightIdx) 
        {
            tm currentNightTm = *localtime(&resNight);
            if ((currentNightTm.tm_mon + 1 == targetMonth) && (currentNightTm.tm_year + 1900 == targetYear))
            {
                totalOccupiedRoomNightsInMonth++;
            }
            currentNightTm.tm_mday += 1;
            resNight = mktime(&currentNightTm);
            if (resNight == INVALID_TIME_T_VALUE) break;
        }
    }
    os << "Total Rooms in Hotel: " << intToMyString(rooms.getSize()).c_str() << std::endl;
    os << "Days in " << monthStr << "." << yearStr << ": " << intToMyString(daysInTargetMonth).c_str() << std::endl;
    os << "Total Possible Room-Nights for the Month: " << intToMyString(totalPossibleRoomNights).c_str() << std::endl;
    os << "Total Occupied Room-Nights in the Month: " << intToMyString(totalOccupiedRoomNightsInMonth).c_str() << std::endl;
    double occupancyRatePercent = 0.0;
    if (totalPossibleRoomNights > 0) {
        occupancyRatePercent = (static_cast<double>(totalOccupiedRoomNightsInMonth) / totalPossibleRoomNights) * 100.0;
    }
    os << "Overall Occupancy Rate: " << doubleToMyString(occupancyRatePercent).c_str() << "%" << std::endl;
    os << "--------------------------------------------------------" << std::endl;
}

void HotelSystem::generateLoyaltyReport(std::ostream& os) const
{
    if (!loggedInUser || (!loggedInUser->canMakeReservation() && !loggedInUser->canViewRevenue())) 
    {
        os << "Access Denied: You do not have permission to generate loyalty reports." << std::endl;
        return;
    }
    os << "--- Guest Loyalty Report ---" << std::endl;
    if (guests.getSize() == 0)
    {
        os << "No guests registered to report on." << std::endl;
    }
    else 
    {
        os << "Listing all guests with their loyalty status and history:" << std::endl;
        for (size_t i = 0; i < guests.getSize(); i++)
        {
            const Guest& g = guests[i];
            os << "  Guest: " << g.getName() << " (ID: " << intToMyString(g.getClientNumber()).c_str() << ")"
                << " | Status: " << g.getStatusString()
                << " | Total Nights: " << intToMyString(g.getTotalNights()).c_str()
                << " | Total Spent: " << doubleToMyString(g.getTotalSpent()).c_str()
                << std::endl;
        }
    }
    os << "----------------------------" << std::endl;
}

void HotelSystem::generateRoomTypeRevenueReport(const MyString& startDateStr, const MyString& endDateStr, std::ostream& os) const {
    if (!loggedInUser || !loggedInUser->canViewRevenue()) 
    {
        os << "Access Denied: You do not have permission to generate this report." << std::endl;
        return;
    }
    os << "--- Room Type Revenue Report (Period: " << startDateStr << " - " << endDateStr << ") ---" << std::endl;
    time_t periodStart = parseDateStringToTimeT(startDateStr);
    time_t periodEnd = parseDateStringToTimeT(endDateStr);
    if (periodStart == INVALID_TIME_T_VALUE || periodEnd == INVALID_TIME_T_VALUE || periodStart >= periodEnd) 
    {
        os << "Error: Invalid date period specified for the report." << std::endl;
        return;
    }
    double revenuesByType[UNKNOWN_ROOM_TYPE] = { 0.0 };
    for (size_t i = 0; i < reservations.getSize(); i++)
    {
        const Reservation& res = reservations[i];
        if (res.getStartDateRaw() >= periodStart && res.getStartDateRaw() < periodEnd)
        {
            const Room* room = findRoom(res.getRoomNumber());
            if (room)
            {
                revenuesByType[room->getRoomTypeEnum()] += res.getTotalPrice();
            }
        }
    }
    bool foundData = false;
    for (int i = 0; i < UNKNOWN_ROOM_TYPE; i++) 
    {
        if (revenuesByType[i] > 0.009) 
        {
            RoomTypeEnum currentType = static_cast<RoomTypeEnum>(i);
            MyString typeName;
            switch (currentType) 
            {
            case SINGLE_ROOM:     typeName = "Single Room"; break;
            case DOUBLE_ROOM:     typeName = "Double Room"; break;
            case LUXURY_ROOM:     typeName = "Luxury Room"; break;
            case APARTMENT:       typeName = "Apartment"; break;
            case CONFERENCE_HALL: typeName = "Conference Hall"; break;
            default:              typeName = "Other/Unknown Type"; break;
            }
            os << "  Revenue from " << typeName << "s: " << doubleToMyString(revenuesByType[i]).c_str() << std::endl;
            foundData = true;
        }
    }
    if (!foundData) 
    {
        os << "No revenue generated by any room type in this period." << std::endl;
    }
    os << "------------------------------------------------------------------" << std::endl;
}

//Data Persistence
void HotelSystem::saveStaticIdsData() const {
    std::ofstream ofs(IDS_FILE_PATH);
    if (!ofs.is_open()) 
    {
        std::cerr << "Error: Could not open file '" << IDS_FILE_PATH << "' for saving static IDs." << std::endl;
        return;
    }
    ofs << Guest::getNextClientNumber() << "\n";
    ofs << Reservation::getNextReservationId() << "\n";
    ofs.close();
}
void HotelSystem::loadStaticIdsData() {
    std::ifstream ifs(IDS_FILE_PATH);
    if (!ifs.is_open()) 
    {
        std::cerr << "Warning: Static IDs file '" << IDS_FILE_PATH << "' not found. Using default starting IDs." << std::endl;
        Guest::setNextClientNumber(1);
        Reservation::setNextReservationId(1);
        return;
    }
    unsigned nextGuestId_val, nextResId_val;
    ifs >> nextGuestId_val;
    if (ifs.good()) Guest::setNextClientNumber(nextGuestId_val);
    else std::cerr << "Error reading next guest ID from '" << IDS_FILE_PATH << "'. Check file format." << std::endl;
    ifs >> nextResId_val;
    if (ifs.good()) Reservation::setNextReservationId(nextResId_val);
    else std::cerr << "Error reading next reservation ID from '" << IDS_FILE_PATH << "'. Check file format." << std::endl;
    ifs.close();
}

void HotelSystem::saveRoomsData() const {
    std::ofstream ofs(ROOMS_FILE_PATH);
    if (!ofs.is_open())
    {
        std::cerr << "Error: Could not open rooms data file '" << ROOMS_FILE_PATH << "' for saving." << std::endl;
        return;
    }
    ofs << rooms.getSize() << "\n";
    for (size_t i = 0; i < rooms.getSize(); i++)
    {
        if (rooms[i]) 
        {
            rooms[i]->serialize(ofs);
        }
    }
    ofs.close();
}
void HotelSystem::loadRoomsData() 
{
    std::ifstream ifs(ROOMS_FILE_PATH);
    if (!ifs.is_open()) 
    {
        std::cerr << "Warning: Rooms data file '" << ROOMS_FILE_PATH << "' not found. Starting with no rooms." << std::endl;
        return;
    }
    size_t count;
    ifs >> count;
    if (ifs.fail()) 
    {
        std::cerr << "Error reading room count from '" << ROOMS_FILE_PATH << "'. File might be corrupted." << std::endl;
        ifs.close(); return;
    }
    ifs.ignore();
    for (size_t i = 0; i < count; i++) 
    {
        Room* room = Room::deserialize(ifs);
        if (room)
        {
            rooms.push_back(room);
        }
        else
        {
            std::cerr << "Error deserializing room entry " << intToMyString(i + 1).c_str() << " from '" << ROOMS_FILE_PATH << "'. Skipping." << std::endl;
            if (ifs.eof()) break;
        }
    }
    ifs.close();
}

void HotelSystem::saveGuestsData() const
{
    std::ofstream ofs(GUESTS_FILE_PATH);
    if (!ofs.is_open()) 
    {
        std::cerr << "Error: Could not open guests data file '" << GUESTS_FILE_PATH << "' for saving." << std::endl;
        return;
    }
    ofs << guests.getSize() << "\n";
    for (size_t i = 0; i < guests.getSize(); i++)
    {
        guests[i].serialize(ofs);
    }
    ofs.close();
}
void HotelSystem::loadGuestsData() {
    std::ifstream ifs(GUESTS_FILE_PATH);
    if (!ifs.is_open())
    {
        std::cerr << "Warning: Guests data file '" << GUESTS_FILE_PATH << "' not found. Starting with no guests." << std::endl;
        return;
    }
    size_t count;
    ifs >> count;
    if (ifs.fail())
    {
        std::cerr << "Error reading guest count from '" << GUESTS_FILE_PATH << "'. File might be corrupted." << std::endl;
        ifs.close(); return;
    }
    ifs.ignore();
    for (size_t i = 0; i < count; i++)
    {
        unsigned loaded_clientId;
        MyString loaded_name, loaded_mobile, loaded_email;
        int loaded_statusInt;
        size_t loaded_nights_val;
        double loaded_spent_val;
        char buffer[256];
        ifs >> loaded_clientId;
        if (ifs.peek() == '\n') ifs.ignore();
        ifs.getline(buffer, sizeof(buffer)); loaded_name = buffer;
        ifs.getline(buffer, sizeof(buffer)); loaded_mobile = buffer;
        ifs.getline(buffer, sizeof(buffer)); loaded_email = buffer;
        ifs >> loaded_statusInt >> loaded_nights_val >> loaded_spent_val;
        if (ifs.peek() == '\n') ifs.ignore();
        if (ifs.fail()) 
        {
            std::cerr << "Error deserializing guest entry " << intToMyString(i + 1).c_str() << " from '" << GUESTS_FILE_PATH << "'. Skipping." << std::endl;
            if (ifs.eof()) break;
            ifs.clear();
            if (ifs.peek() != '\n' && !ifs.eof()) ifs.getline(buffer, sizeof(buffer));
            continue;
        }
        guests.push_back(Guest(loaded_clientId, loaded_name, loaded_mobile, loaded_email,
            static_cast<GuestStatus>(loaded_statusInt), loaded_nights_val, loaded_spent_val));
    }
    ifs.close();
}

void HotelSystem::saveReservationsData() const 
{
    std::ofstream ofs(RESERVATIONS_FILE_PATH);
    if (!ofs.is_open())
    {
        std::cerr << "Error: Could not open reservations data file '" << RESERVATIONS_FILE_PATH << "' for saving." << std::endl;
        return;
    }
    ofs << reservations.getSize() << "\n";
    for (size_t i = 0; i < reservations.getSize(); i++) 
    {
        reservations[i].serialize(ofs);
    }
    ofs.close();
}
void HotelSystem::loadReservationsData() {
    std::ifstream ifs(RESERVATIONS_FILE_PATH);
    if (!ifs.is_open())
    {
        std::cerr << "Warning: Reservations data file '" << RESERVATIONS_FILE_PATH << "' not found. Starting with no reservations." << std::endl;
        return;
    }
    size_t count;
    ifs >> count;
    if (ifs.fail()) 
    {
        std::cerr << "Error reading reservation count from '" << RESERVATIONS_FILE_PATH << "'. File might be corrupted." << std::endl;
        ifs.close(); return;
    }
    ifs.ignore();
    for (size_t i = 0; i < count; i++) 
    {
        unsigned loaded_resId, loaded_guestId, loaded_roomId;
        long long loaded_start_ll, loaded_end_ll;
        double loaded_price;
        ifs >> loaded_resId >> loaded_guestId >> loaded_roomId >> loaded_start_ll >> loaded_end_ll >> loaded_price;
        if (ifs.peek() == '\n') ifs.ignore();
        if (ifs.fail())
        {
            std::cerr << "Error deserializing reservation entry " << intToMyString(i + 1).c_str() << " from '" << RESERVATIONS_FILE_PATH << "'. Skipping." << std::endl;
            if (ifs.eof()) break;
            ifs.clear();
            char tempBuffer[256]; if (ifs.peek() != '\n' && !ifs.eof()) ifs.getline(tempBuffer, sizeof(tempBuffer));
            continue;
        }
        reservations.push_back(Reservation(loaded_resId, loaded_guestId, loaded_roomId,
            static_cast<time_t>(loaded_start_ll), static_cast<time_t>(loaded_end_ll), loaded_price));
    }
    ifs.close();
}

void HotelSystem::saveUsersData() const {
    std::ofstream ofs(USERS_FILE_PATH);
    if (!ofs.is_open()) 
    {
        std::cerr << "Error: Could not open users data file '" << USERS_FILE_PATH << "' for saving." << std::endl;
        return;
    }
    ofs << users.getSize() << "\n";
    for (size_t i = 0; i < users.getSize(); i++)
    {
        if (users[i]) 
        {
            users[i]->serialize(ofs);
        }
    }
    ofs.close();
}
void HotelSystem::loadUsersData() {
    std::ifstream ifs(USERS_FILE_PATH);
    if (!ifs.is_open()) 
    {
        std::cerr << "Warning: Users data file '" << USERS_FILE_PATH << "' not found. Starting with no users." << std::endl;
        return;
    }
    size_t count;
    ifs >> count;
    if (ifs.fail()) 
    {
        std::cerr << "Error reading user count from '" << USERS_FILE_PATH << "'. File might be corrupted." << std::endl;
        ifs.close(); return;
    }
    ifs.ignore();
    for (size_t i = 0; i < count; i++)
    {
        int loaded_roleInt;
        MyString loaded_username, loaded_password;
        char buffer[256];
        ifs >> loaded_roleInt;
        if (ifs.peek() == '\n') ifs.ignore();
        ifs.getline(buffer, sizeof(buffer)); loaded_username = buffer;
        ifs.getline(buffer, sizeof(buffer)); loaded_password = buffer;
        if (ifs.fail()) 
        {
            std::cerr << "Error deserializing user entry " << intToMyString(i + 1).c_str() << " from '" << USERS_FILE_PATH << "'. Skipping." << std::endl;
            if (ifs.eof()) break;
            ifs.clear();
            if (ifs.peek() != '\n' && !ifs.eof()) ifs.getline(buffer, sizeof(buffer));
            continue;
        }
        UserRole loaded_role = static_cast<UserRole>(loaded_roleInt);
        User* newUser = nullptr;
        switch (loaded_role)
        {
        case ROLE_MANAGER:    newUser = new Manager(loaded_username, loaded_password); break;
        case ROLE_RECEPTIONIST: newUser = new Receptionist(loaded_username, loaded_password); break;
        case ROLE_ACCOUNTANT: newUser = new Accountant(loaded_username, loaded_password); break;
        default:
            std::cerr << "Error: Unknown role ID (" << intToMyString(loaded_roleInt).c_str() << ") for user '"
                << loaded_username << "' during deserialization. Skipping user." << std::endl;
            continue;
        }
        users.push_back(newUser);
    }
    ifs.close();
}

void HotelSystem::savePricingStrategyData() const 
{
    std::ofstream ofs(STRATEGY_FILE_PATH);
    if (!ofs.is_open())
    {
        std::cerr << "Error: Could not open pricing strategy file '" << STRATEGY_FILE_PATH << "' for saving." << std::endl;
        return;
    }
    if (currentPricingStrategy)
    {
        currentPricingStrategy->serialize(ofs);
    }
    else 
    {
        std::cerr << "Warning: No current pricing strategy set. Saving a default strategy." << std::endl;
        DefaultPricingStrategy tempDefaultStrategy;
        tempDefaultStrategy.serialize(ofs);
    }
    ofs.close();
}
void HotelSystem::loadPricingStrategyData()
{
    std::ifstream ifs(STRATEGY_FILE_PATH);
    if (!ifs.is_open())
    {
        std::cerr << "Warning: Pricing strategy file '" << STRATEGY_FILE_PATH
            << "' not found. Using default pricing strategy." << std::endl;
        delete currentPricingStrategy;
        currentPricingStrategy = new DefaultPricingStrategy();
        return;
    }
    int typeInt;
    ifs >> typeInt;
    if (ifs.fail())
    {
        std::cerr << "Error reading strategy type from '" << STRATEGY_FILE_PATH
            << "'. File might be empty or corrupted. Using default strategy." << std::endl;
        delete currentPricingStrategy;
        currentPricingStrategy = new DefaultPricingStrategy();
        ifs.close();
        return;
    }
    PricingStrategyType loaded_type = static_cast<PricingStrategyType>(typeInt);
    delete currentPricingStrategy;
    currentPricingStrategy = nullptr;
    switch (loaded_type)
    {
    case DEFAULT_PRICING:
        currentPricingStrategy = new DefaultPricingStrategy();
        break;
    case SEASONAL_PRICING:
        {
        double multiplier;
        ifs >> multiplier;
        if (ifs.fail())
        {
            std::cerr << "Error reading seasonal multiplier for SeasonalPricingStrategy. Using default multiplier (1.15)." << std::endl;
            multiplier = 1.15;
        }
        currentPricingStrategy = new SeasonalPricingStrategy(multiplier);
        break;
    }
    case WEEKEND_PRICING:
        currentPricingStrategy = new WeekendPricingStrategy();
        break;
    default:
        std::cerr << "Warning: Unknown pricing strategy type (" << intToMyString(typeInt).c_str()
            << ") found in file. Using default strategy." << std::endl;
        currentPricingStrategy = new DefaultPricingStrategy();
        break;
    }
    if (ifs.peek() == '\n') ifs.ignore();
    ifs.close();
    if (!currentPricingStrategy) {
        std::cerr << "Critical failure in loading pricing strategy. Forcing default." << std::endl;
        currentPricingStrategy = new DefaultPricingStrategy();
    }
}

//Save/Load
void HotelSystem::saveData() const {
    std::cout << "Saving all system data..." << std::endl;
    saveStaticIdsData();
    saveRoomsData();
    saveGuestsData();
    saveReservationsData();
    saveUsersData();
    savePricingStrategyData();
    std::cout << "All data saved successfully." << std::endl;
}

void HotelSystem::loadData() {
    std::cout << "Loading all system data..." << std::endl;
    loadStaticIdsData();
    loadRoomsData();
    loadGuestsData();
    loadReservationsData();
    loadUsersData();
    loadPricingStrategyData();
    std::cout << "All data loaded." << std::endl;
    if (users.getSize() == 0) 
    {
        std::cout << "No users found in the system. This might be the first run." << std::endl;
        std::cout << "Please register the initial Administrator/Manager user." << std::endl;
    }
    if (!currentPricingStrategy) 
    {
        std::cerr << "Warning: No pricing strategy loaded, defaulting." << std::endl;
        currentPricingStrategy = new DefaultPricingStrategy();
    }
}

User* HotelSystem::getLoggedInUser() const 
{
    return loggedInUser;
}

size_t HotelSystem::getUsersCount() const 
{
    return users.getSize();
}