#include "ConferenceHall.h"

ConferenceHall::ConferenceHall(unsigned roomNumber, double basePrice, size_t capacity) : Room(roomNumber, basePrice, capacity)
{
}

MyString ConferenceHall::getRoomType() const
{
    return "ConferenceHall";
}

RoomTypeEnum ConferenceHall::getRoomTypeEnum() const
{
    return CONFERENCE_HALL;
}

double ConferenceHall::calculatePrice(int nights, const PricingStrategy& strategy) const
{
    return strategy.calculatePrice(getBasePricePerNight(), nights) * 3.0;
}

void ConferenceHall::serialize(std::ostream& os) const
{
    Room::serialize(os);
}
