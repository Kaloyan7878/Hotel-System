#include "Manager.h"

Manager::Manager(const MyString& userName, const MyString& password) : User(userName, password, UserRole::ROLE_MANAGER)
{}

bool Manager::canAddRoom() const
{
    return true;
}
bool Manager::canViewRevenue() const
{
    return true;
}
bool Manager::canMakeReservation() const
{
    return true;
}