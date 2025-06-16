#include "Accountant.h"

Accountant::Accountant(const MyString& userName, const MyString& password) : User(userName, password, UserRole::ROLE_ACCOUNTANT)
{}

bool Accountant::canAddRoom() const
{
    return false;
}
bool Accountant::canViewRevenue() const
{
    return true;
}
bool Accountant::canMakeReservation() const
{
    return false;
}