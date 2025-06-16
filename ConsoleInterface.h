#pragma once
#include "HotelSystem.h"

enum class ReportType {
    REVENUE,
    OCCUPANCY,
    LOYALTY,
    ROOM_TYPE_REVENUE
};

class ConsoleInterface
{
    HotelSystem& hotelSystem;

    void displayLoginMenu();
    void displayMainMenu();
    void displayRoomManagementMenu();
    void displayGuestManagementMenu();
    void displayReservationManagementMenu();
    void displayUserManagementMenu();
    void displayReportAndAnalyticsMenu();
    void displayPricingStrategyMenu();

    void handleLogin();
    void handleLogout();

    void handleAddRoom();
    void handleSetRoomStatus();
    void handleUpdateRoomPrice();
    void handleViewAvailableRooms();
    void handleRegisterGuest();
    void handleViewClientActivity();

    void handleCreateReservation();
    void handleCancelReservation();
    void handleViewReservationDetails();

    void handleRegisterNewUser();

    void handleCalculateAndDisplayRevenue();
    void handleGenerateReport(ReportType type);

public:
    ConsoleInterface(HotelSystem& system);
    void run();
};