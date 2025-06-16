#pragma once
#include "Room.h"
class Apartment :public Room
{
public:
	Apartment() = default;
	Apartment(unsigned roomNumber, double basePrice, size_t capacity);

	MyString getRoomType() const override;
	RoomTypeEnum getRoomTypeEnum() const override;

	double calculatePrice(int nights, const PricingStrategy& strategy) const override;
	void serialize(std::ostream& os) const override;
};
