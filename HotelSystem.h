#pragma once
#include "DateUtils.h"
#include "MyVector.hpp"     
#include "User.h"           
#include "Manager.h"        
#include "Receptionist.h"
#include "Accountant.h"
#include "Room.h"           
#include "Guest.h"
#include "Reservation.h"
#include "PricingStrategy.h"        
#include "DefaultPricingStrategy.h" 
#include "SeasonalPricingStrategy.h"
#include "WeekendPricingStrategy.h"

#include <iostream> 
#include <fstream>  

const char* const ROOMS_FILE_PATH = "rooms.dat";
const char* const GUESTS_FILE_PATH = "guests.dat";
const char* const RESERVATIONS_FILE_PATH = "reservations.dat";
const char* const USERS_FILE_PATH = "users.dat";
const char* const STRATEGY_FILE_PATH = "strategy.dat";
const char* const IDS_FILE_PATH = "ids.dat";


MyString intToMyString(int value);
MyString doubleToMyString(double value, int precision = 2);

class HotelSystem
{
private:
    MyVector<Room*> rooms;
    MyVector<Guest> guests;
    MyVector<Reservation> reservations;
    MyVector<User*> users;
    User* loggedInUser;
    PricingStrategy* currentPricingStrategy;
    Room* findRoom(unsigned roomNumber);
    Guest* findGuest(unsigned guestId);
    Guest* findGuestByName(const MyString& guestName);
    Reservation* findReservation(unsigned reservationId);
    User* findUserByUsername(const MyString& username);
    const Room* findRoom(unsigned roomNumber) const;
    const Guest* findGuest(unsigned guestId) const;
    const Guest* findGuestByName(const MyString& guestName) const;
    void saveRoomsData() const;
    void saveGuestsData() const;
    void saveReservationsData() const;
    void saveUsersData() const;
    void savePricingStrategyData() const;
    void saveStaticIdsData() const;
    void loadRoomsData();
    void loadGuestsData();
    void loadReservationsData();
    void loadUsersData();
    void loadPricingStrategyData();
    void loadStaticIdsData();
    double getDiscountFactorForGuest(GuestStatus status) const;
public:
    HotelSystem();
    ~HotelSystem();
    HotelSystem(const HotelSystem&) = delete;
    HotelSystem& operator=(const HotelSystem&) = delete;
    void addRoom(unsigned roomNumber, const MyString& roomTypeStr, double pricePerNight, size_t capacity);
    void updateRoomStatus(unsigned roomNumber, RoomStatus newStatus);
    void updateRoomPrice(unsigned roomNumber, double newPrice);
    void listAvailableRooms(const MyString& startDateStr, const MyString& endDateStr, std::ostream& os = std::cout) const;
    void printAllRooms(std::ostream& os = std::cout) const;
    void registerGuest(const MyString& name, const MyString& phone, const MyString& email);
    void printAllGuests(std::ostream& os = std::cout) const;
    void printGuestActivity(const MyString& guestName, std::ostream& os = std::cout) const;
    bool createReservation(unsigned roomNumber, const MyString& guestName,
        const MyString& startDateStr, const MyString& endDateStr);
    bool cancelReservation(unsigned reservationId);
    void viewReservationDetails(unsigned reservationId, std::ostream& os = std::cout) const;
    void printAllReservations(std::ostream& os = std::cout) const;
    void registerUser(const MyString& username, const MyString& password, UserRole role);
    bool login(const MyString& username, const MyString& password);
    void logout();
    void listAllUsers(std::ostream& os = std::cout) const;
    void setPricingStrategy(PricingStrategyType type, double arg = 0.0);
    const PricingStrategy* getCurrentPricingStrategy() const;
    double calculateTotalRevenueForPeriod(const MyString& startDateStr, const MyString& endDateStr) const;
    void generateRevenueReport(const MyString& startDateStr, const MyString& endDateStr, std::ostream& os = std::cout) const;
    void generateOccupancyReport(const MyString& monthYearStr, std::ostream& os = std::cout) const;
    void generateLoyaltyReport(std::ostream& os = std::cout) const;
    void generateRoomTypeRevenueReport(const MyString& startDateStr, const MyString& endDateStr, std::ostream& os = std::cout) const;
    void saveData() const;
    void loadData();
    User* getLoggedInUser() const;
    size_t getUsersCount() const;
};