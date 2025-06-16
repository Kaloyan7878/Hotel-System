#pragma once
#include "MyString.h"
#include <iostream>

enum UserRole
{
	ROLE_MANAGER,
	ROLE_RECEPTIONIST,
	ROLE_ACCOUNTANT,
	UNKNOWN_ROLE
};

MyString userRoleToString(UserRole role);

class User
{
protected:

	MyString userName;
	MyString password;
	UserRole role;
public:
	User() : role(UNKNOWN_ROLE) {}
	User(const MyString& userName, const MyString& password, UserRole userRole);

	virtual ~User() = default;

	const MyString& getUsername() const;
	UserRole getRole() const;
	MyString getRoleString() const;

	bool checkPassword(const MyString& plainPassword) const;

	virtual bool canAddRoom() const = 0;
	virtual bool canViewRevenue() const = 0;
	virtual bool canMakeReservation() const = 0;

	virtual void serialize(std::ostream& os) const;
};
