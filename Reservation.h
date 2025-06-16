#pragma once
#include <iostream> 
#include <ctime>    
#include "DateUtils.h"
#pragma warning(disable : 4996) 

const time_t INVALID_TIME_T_VALUE = (time_t)-1;

class Reservation
{

private:
	unsigned reservationId;
	unsigned guestId;
	unsigned roomId;
	time_t startDateRaw;
	time_t endDateRaw;
	double totalPrice;
	static unsigned nextReservationId;
public:
	Reservation();
	Reservation(unsigned guestId, unsigned roomId,
		time_t startDate, time_t endDate, double totalPrice);
	Reservation(unsigned reservationId, unsigned guestId, unsigned roomId,
		time_t startDate, time_t endDate, double totalPrice);
	unsigned getReservationId() const;
	unsigned getGuestId() const;
	unsigned getRoomNumber() const;
	MyString getStartDateString() const;
	MyString getEndDateString() const;
	time_t getStartDateRaw() const;
	time_t getEndDateRaw() const;
	double getTotalPrice() const;
	int getNumberOfNights() const;
	void setTotalPrice(double price);
	void setStartDateRaw(time_t date);
	void setEndDateRaw(time_t date);
	bool overlapsWith(time_t otherStartDateRaw, time_t otherEndDateRaw) const;
	static unsigned getNextReservationId();
	static void setNextReservationId(unsigned value);
	void serialize(std::ostream& os) const;
};