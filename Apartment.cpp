#include "Apartment.h"

Apartment::Apartment(unsigned roomNumber, double basePrice, size_t capacity) : Room(roomNumber, basePrice, capacity)
{
}

MyString Apartment::getRoomType() const
{
    return "Apartment";
}

RoomTypeEnum Apartment::getRoomTypeEnum() const
{
    return APARTMENT;
}

double Apartment::calculatePrice(int nights, const PricingStrategy& strategy) const
{
    return strategy.calculatePrice(getBasePricePerNight(), nights) * 1.5;
}

void Apartment::serialize(std::ostream& os) const
{
    Room::serialize(os);
}
