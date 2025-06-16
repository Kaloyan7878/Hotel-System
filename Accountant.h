#pragma once
#include "User.h"

class Accountant :public User
{
public:
    Accountant(const MyString& userName, const MyString& password);

    bool canAddRoom() const override;
    bool canViewRevenue() const override;
    bool canMakeReservation() const override;
};