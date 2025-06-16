#pragma once
#pragma once
#include "Room.h"
class LuxuryRoom : public Room
{
public:
	LuxuryRoom() = default;
	LuxuryRoom(int roomNumber, double basePrice, size_t capacity);

	MyString getRoomType() const override;
	RoomTypeEnum getRoomTypeEnum() const override;
	double calculatePrice(int nights, const PricingStrategy& strategy) const override;
	void serialize(std::ostream& os) const override;
};