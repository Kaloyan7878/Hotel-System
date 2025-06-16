#include "Room.h"
#include "SingleRoom.h"
#include "DoubleRoom.h"
#include "LuxuryRoom.h"
#include "Apartment.h"
#include "ConferenceHall.h"
#include "DefaultPricingStrategy.h"

Room::Room(unsigned roomNumber, double basePrice, size_t capacity)
    : roomNumber(roomNumber), basePrice(basePrice), capacity(capacity), status(RoomStatus::AVAILABLE)
{
}

unsigned Room::getRoomNumber() const
{
    return roomNumber;
}

RoomStatus Room::getStatus() const
{
    return status;
}

MyString Room::getStatusString() const
{
    switch (status) 
    {
    case AVAILABLE: return "Available";
    case RESERVED: return "Reserved";
    case MAINTENANCE: return "Maintenance";
    default: return "Unknown";
    }
}

double Room::getBasePricePerNight() const
{
    return basePrice;
}

size_t Room::getCapacity() const
{
    return capacity;
}

void Room::setStatus(RoomStatus newStatus)
{
    status = newStatus;
}

void Room::setBasePricePerNight(double newPrice)
{
    basePrice = newPrice;
}

void Room::setCapacity(size_t newCapacity)
{
    capacity = newCapacity;
}

void Room::serialize(std::ostream& os) const
{
    os << static_cast<int>(getRoomTypeEnum()) << " ";
    os << roomNumber << " " << static_cast<int>(status) << " " << basePrice << " " << capacity << "\n";
}

Room* Room::deserialize(std::istream& is)
{
    int typeInt;
    is >> typeInt;
    if (is.fail()) return nullptr;

    RoomTypeEnum type = static_cast<RoomTypeEnum>(typeInt);

    unsigned roomNum;
    int statusInt;
    double price;
    size_t cap;

    is >> roomNum >> statusInt >> price >> cap;
    if (is.fail()) return nullptr;

    Room* newRoom = nullptr;
    RoomStatus room_status = static_cast<RoomStatus>(statusInt);

    switch (type) 
    {
    case SINGLE_ROOM: newRoom = new SingleRoom(roomNum, price, cap); break;
    case DOUBLE_ROOM: newRoom = new DoubleRoom(roomNum, price, cap); break;
    case LUXURY_ROOM: newRoom = new LuxuryRoom(roomNum, price, cap); break;
    case APARTMENT: newRoom = new Apartment(roomNum, price, cap); break;
    case CONFERENCE_HALL: newRoom = new ConferenceHall(roomNum, price, cap); break;
    default:
        std::cerr << "Error: Unknown room type (" << typeInt << ") during deserialization." << std::endl;
        char tempBuffer[256];
        is.getline(tempBuffer, sizeof(tempBuffer));
        return nullptr;
    }

    if (newRoom) 
    {
        newRoom->setStatus(room_status);
    }
    return newRoom;
}
