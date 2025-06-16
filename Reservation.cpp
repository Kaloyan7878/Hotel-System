#include "Reservation.h"
#include <iostream>
#include <fstream>
//#include <vector>
#include <cstdlib>

static int charToIntLocal(char c) 
{
    if (c >= '0' && c <= '9') 
    {
        return c - '0';
    }
    return -1;
}

static int extractIntFromMyStringLocal(const MyString& str, size_t start, size_t length) {
    int result = 0;
    bool valid = false;
    for (size_t i = 0; i < length; i++) 
    {
        if (start + i < str.getSize()) 
        {
            char c = str.c_str()[start + i];
            int digit = charToIntLocal(c);
            if (digit != -1) 
            {
                result = result * 10 + digit;
                valid = true;
            }
            else 
            {
                return -1;
            }
        }
        else 
        {
            return -1;
        }
    }
    return valid ? result : -1;
}

time_t parseDateStringToTimeT(const MyString& dateString) 
{
    if (dateString.getSize() != 10 ||
        dateString.c_str()[2] != '.' || dateString.c_str()[5] != '.') 
    {
        std::cerr << "Error: Invalid date string format '" << dateString << "'. Expected DD.MM.YYYY." << std::endl;
        return INVALID_TIME_T_VALUE;
    }

    int day = extractIntFromMyStringLocal(dateString, 0, 2);
    int month = extractIntFromMyStringLocal(dateString, 3, 2);
    int year = extractIntFromMyStringLocal(dateString, 6, 4);

    if (day == -1 || month == -1 || year == -1) 
    {
        std::cerr << "Error: Invalid characters in date string '" << dateString << "'." << std::endl;
        return INVALID_TIME_T_VALUE;
    }
    if (year < 1900 || year > 2100 || month < 1 || month > 12 || day < 1 || day > 31) 
    {
        std::cerr << "Error: Date values out of range in '" << dateString << "'." << std::endl;
        return INVALID_TIME_T_VALUE;
    }


    tm t = {};
    t.tm_mday = day;
    t.tm_mon = month - 1;
    t.tm_year = year - 1900;
    t.tm_hour = 12;
    t.tm_min = 0;
    t.tm_sec = 0;
    t.tm_isdst = -1;

    time_t rawTimeResult = mktime(&t);
    if (rawTimeResult == INVALID_TIME_T_VALUE)
    {
        std::cerr << "Error: mktime failed for date string '" << dateString << "'." << std::endl;
    }
    return rawTimeResult;
}

MyString formatTimeTToDateString(time_t rawTime)
{
    if (rawTime == INVALID_TIME_T_VALUE) 
    {
        return "INVALID_DATE";
    }

    tm* local_tm = localtime(&rawTime);
    if (!local_tm) 
    {
        std::cerr << "Error: Failed to convert raw time to tm struct for formatting." << std::endl;
        return "FORMAT_ERROR";
    }

    char buffer[11]; // DD.MM.YYYY\0
    if (strftime(buffer, sizeof(buffer), "%d.%m.%Y", local_tm) == 0) {
        std::cerr << "Error: Failed to format date string using strftime." << std::endl;
        return "FORMAT_ERROR";
    }
    return MyString(buffer);
}


unsigned Reservation::nextReservationId = 1;

Reservation::Reservation()
    : reservationId(0), guestId(0), roomId(0),
    startDateRaw(INVALID_TIME_T_VALUE), endDateRaw(INVALID_TIME_T_VALUE), totalPrice(0.0)
{}

Reservation::Reservation(unsigned guestId, unsigned roomId, time_t startDate, time_t endDate, double totalPrice)
    : reservationId(nextReservationId++), guestId(guestId), roomId(roomId),
    startDateRaw(startDate), endDateRaw(endDate), totalPrice(totalPrice)
{
    if (startDateRaw == INVALID_TIME_T_VALUE || endDateRaw == INVALID_TIME_T_VALUE)
    {
        std::cerr << "Warning: Reservation " << reservationId << " created with invalid dates for Guest " << guestId << ", Room " << roomId << "." << std::endl;
    }
    if (startDateRaw != INVALID_TIME_T_VALUE && endDateRaw != INVALID_TIME_T_VALUE && startDateRaw > endDateRaw) 
    {
        std::cerr << "Warning: Start date is after end date for Reservation " << reservationId << "." << std::endl;
    }
}

Reservation::Reservation(unsigned resId, unsigned gId, unsigned rId, time_t startD, time_t endD, double tPrice)
    : reservationId(resId), guestId(gId), roomId(rId), startDateRaw(startD), endDateRaw(endD), totalPrice(tPrice)
{
    if (resId >= nextReservationId) {
        nextReservationId = resId + 1;
    }
}


void Reservation::setTotalPrice(double price) { totalPrice = price; }
void Reservation::setStartDateRaw(time_t date) { startDateRaw = date; }
void Reservation::setEndDateRaw(time_t date) { endDateRaw = date; }

unsigned Reservation::getReservationId() const { return reservationId; }
unsigned Reservation::getGuestId() const { return guestId; }
unsigned Reservation::getRoomNumber() const { return roomId; }
MyString Reservation::getStartDateString() const { return formatTimeTToDateString(startDateRaw); }
MyString Reservation::getEndDateString() const { return formatTimeTToDateString(endDateRaw); }
time_t Reservation::getStartDateRaw() const { return startDateRaw; }
time_t Reservation::getEndDateRaw() const { return endDateRaw; }
double Reservation::getTotalPrice() const { return totalPrice; }

int Reservation::getNumberOfNights() const
{
    if (startDateRaw == INVALID_TIME_T_VALUE || endDateRaw == INVALID_TIME_T_VALUE || startDateRaw > endDateRaw) 
    {
        return 0;
    }
    double secondsPerDay = 24 * 60 * 60;
    double differenceInSeconds = difftime(endDateRaw, startDateRaw);
    int nights = static_cast<int>((differenceInSeconds / secondsPerDay) + 0.5);
    return (nights > 0 ? nights : 0);
}


bool Reservation::overlapsWith(time_t otherStartDateRaw, time_t otherEndDateRaw) const 
{
    if (startDateRaw == INVALID_TIME_T_VALUE || endDateRaw == INVALID_TIME_T_VALUE ||
        otherStartDateRaw == INVALID_TIME_T_VALUE || otherEndDateRaw == INVALID_TIME_T_VALUE) {
        std::cerr << "Warning: Attempting to check overlap with invalid dates in Reservation " << reservationId << "." << std::endl;
        return true;
    }
    return (this->startDateRaw < otherEndDateRaw) && (this->endDateRaw > otherStartDateRaw);
}

unsigned Reservation::getNextReservationId() {
    return nextReservationId;
}

void Reservation::setNextReservationId(unsigned value) 
{
    if (value > nextReservationId) 
    {
        nextReservationId = value;
    }
}

void Reservation::serialize(std::ostream& os) const {
    os << reservationId << " "
        << guestId << " "
        << roomId << " "
        << startDateRaw << " "
        << endDateRaw << " "
        << totalPrice << "\n";
}
