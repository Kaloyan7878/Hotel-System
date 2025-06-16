#include "LuxuryRoom.h"

LuxuryRoom::LuxuryRoom(int roomNumber, double basePrice, size_t capacity) : Room(roomNumber, basePrice, capacity)
{
}

MyString LuxuryRoom::getRoomType() const
{
    return "LuxuryRoom";
}

RoomTypeEnum LuxuryRoom::getRoomTypeEnum() const
{
    return LUXURY_ROOM;
}

double LuxuryRoom::calculatePrice(int nights, const PricingStrategy& strategy) const
{
    return strategy.calculatePrice(getBasePricePerNight(), nights) * 2.0;
}

void LuxuryRoom::serialize(std::ostream& os) const
{
    Room::serialize(os);
}
