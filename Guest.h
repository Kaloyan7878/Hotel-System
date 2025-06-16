#pragma once
#include "MyString.h"
#include <fstream>

enum GuestStatus
{
	NORMAL,
	GOLD,
	PLATINUM
};

class Guest
{
	MyString name;
	MyString mobileNumber;
	MyString mail;
	unsigned clientNumber;
	static unsigned nextClientNumber;

	GuestStatus status = NORMAL;
	size_t totalNights = 0;
	double totalSpent = 0.0;

	void updateLoyaltyStatus();

public:
	Guest() = default;
	Guest(const MyString& name, const MyString& mobileNumber, const MyString& mail);
	Guest(unsigned clientNumber, const MyString& name, const MyString& mobileNumber, const MyString& mail, GuestStatus status, size_t totalNights, double totalSpent);


	const MyString& getName() const;
	const MyString& getMail() const;
	const MyString& getMobileNumber() const;
	unsigned getClientNumber() const;
	GuestStatus getStatus() const;
	MyString getStatusString() const;
	size_t getTotalNights() const;
	double getTotalSpent() const;

	void setStatus(GuestStatus newStatus);
	void setMail(const MyString& newMail);
	void setMobileNumber(const MyString& newMobile);

	void addStay(size_t nights, double spent);

	static unsigned getNextClientNumber();
	static void setNextClientNumber(unsigned value);

	void serialize(std::ostream& ofs) const;
};
