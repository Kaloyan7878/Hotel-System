#pragma once
#include "User.h"

class Manager :public User
{
public:
    Manager(const MyString& userName, const MyString& password);

    bool canAddRoom() const override;
    bool canViewRevenue() const override;
    bool canMakeReservation() const override;
};