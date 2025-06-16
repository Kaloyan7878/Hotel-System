#pragma once
#include "User.h"

class Receptionist :public User
{
public:
    Receptionist(const MyString& userName, const MyString& password);

    bool canAddRoom() const override;
    bool canViewRevenue() const override;
    bool canMakeReservation() const override;
};