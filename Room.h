#pragma once
#include <iostream>
#include "MyString.h"
#include "PricingStrategy.h"

#pragma warning(disable : 4996)

enum RoomStatus
{
	AVAILABLE,
	RESERVED,
	MAINTENANCE
};

enum RoomTypeEnum
{
	SINGLE_ROOM,
	DOUBLE_ROOM,
	LUXURY_ROOM,
	APARTMENT,
	CONFERENCE_HALL,
	UNKNOWN_ROOM_TYPE
};

class Room
{
protected:
	unsigned roomNumber = 0;
	RoomStatus status = AVAILABLE;
	double basePrice = 0;
	size_t capacity = 0;
public:
	Room() = default;
	Room(unsigned roomNumber, double basePrice, size_t capacity);
	virtual ~Room() = default;

	unsigned getRoomNumber() const;
	RoomStatus getStatus() const;
	MyString getStatusString() const;
	double getBasePricePerNight() const;
	size_t getCapacity() const;

	void setStatus(RoomStatus newStatus);
	void setBasePricePerNight(double newPrice);
	void setCapacity(size_t newCapacity);

	virtual MyString getRoomType() const = 0;
	virtual RoomTypeEnum getRoomTypeEnum() const = 0;

	virtual void serialize(std::ostream& os) const;
	static Room* deserialize(std::istream& is);

	virtual double calculatePrice(int nights, const PricingStrategy& strategy) const = 0;
};
