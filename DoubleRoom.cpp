#pragma once
#include "DoubleRoom.h"

DoubleRoom::DoubleRoom(int roomNumber, double basePrice, size_t capacity) : Room(roomNumber, basePrice, capacity)
{
    if (capacity == 0) this->capacity = 2;
}

MyString DoubleRoom::getRoomType() const
{
    return "DoubleRoom";
}

RoomTypeEnum DoubleRoom::getRoomTypeEnum() const
{
    return DOUBLE_ROOM;
}

double DoubleRoom::calculatePrice(int nights, const PricingStrategy& strategy) const
{
    return strategy.calculatePrice(getBasePricePerNight(), nights) * 1.2;
}

void DoubleRoom::serialize(std::ostream& os) const
{
    Room::serialize(os);
}