#include "Receptionist.h"

Receptionist::Receptionist(const MyString& userName, const MyString& password) : User(userName, password, UserRole::ROLE_RECEPTIONIST)
{}

bool Receptionist::canAddRoom() const
{
    return false;
}
bool Receptionist::canViewRevenue() const
{
    return false;
}
bool Receptionist::canMakeReservation() const
{
    return true;
}