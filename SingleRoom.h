#pragma once
#include "Room.h"
#include <iostream>
#include <fstream>
class SingleRoom : public Room
{
public:
	SingleRoom() = default;
	SingleRoom(int roomNumber, double basePrice, size_t capacity);
	MyString getRoomType() const override;

	RoomTypeEnum getRoomTypeEnum() const override;
	double calculatePrice(int nights, const PricingStrategy& strategy) const override;
	void serialize(std::ostream& os) const override;
};
