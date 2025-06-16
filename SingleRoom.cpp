#include "SingleRoom.h"
#include "DefaultPricingStrategy.h"

SingleRoom::SingleRoom(int roomNumber, double basePrice, size_t capacity) : Room(roomNumber, basePrice, capacity)
{
    if (capacity == 0) this->capacity = 1;
}
MyString SingleRoom::getRoomType() const
{
    return "SingleRoom";
}

RoomTypeEnum SingleRoom::getRoomTypeEnum() const
{
    return SINGLE_ROOM;
}

double SingleRoom::calculatePrice(int nights, const PricingStrategy& strategy) const
{
    return strategy.calculatePrice(getBasePricePerNight(), nights);
}

void SingleRoom::serialize(std::ostream& os) const
{
    Room::serialize(os);
}
