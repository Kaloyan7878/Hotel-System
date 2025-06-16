#include "Guest.h"
#include <iostream>

unsigned Guest::nextClientNumber = 1;

Guest::Guest(const MyString& name, const MyString& mobileNumber, const MyString& mail)
    : name(name), mobileNumber(mobileNumber), mail(mail), clientNumber(nextClientNumber++), status(NORMAL), totalNights(0), totalSpent(0.0)
{
}

Guest::Guest(unsigned clientNum, const MyString& name, const MyString& mobileNumber, const MyString& mail, GuestStatus status, size_t totalNights, double totalSpent)
    : name(name), mobileNumber(mobileNumber), mail(mail), clientNumber(clientNum), status(status), totalNights(totalNights), totalSpent(totalSpent)
{
    if (clientNum >= nextClientNumber) 
    {
        nextClientNumber = clientNum + 1;
    }
}


const MyString& Guest::getName() const
{
    return name;
}

const MyString& Guest::getMail() const
{
    return mail;
}

const MyString& Guest::getMobileNumber() const
{
    return mobileNumber;
}

unsigned Guest::getClientNumber() const
{
    return clientNumber;
}

GuestStatus Guest::getStatus() const
{
    return status;
}

MyString Guest::getStatusString() const {
    switch (status) 
    {
    case NORMAL: return "Normal";
    case GOLD: return "Gold";
    case PLATINUM: return "Platinum";
    default: return "Unknown";
    }
}

size_t Guest::getTotalNights() const
{
    return totalNights;
}

double Guest::getTotalSpent() const
{
    return totalSpent;
}

void Guest::setStatus(GuestStatus newStatus)
{
    status = newStatus;
}

void Guest::setMail(const MyString& newMail)
{
    mail = newMail;
}

void Guest::setMobileNumber(const MyString& newMobile)
{
    mobileNumber = newMobile;
}

void Guest::addStay(size_t nights, double spent) {
    totalNights += nights;
    totalSpent += spent;
    updateLoyaltyStatus();
}

void Guest::updateLoyaltyStatus()
{
    if (totalSpent >= 1000.0 && totalNights >= 50)
    {
        status = PLATINUM;
    }
    else if (totalSpent >= 300.0 && totalNights >= 10)
    {
        status = GOLD;
    }
    else
    {
        status = NORMAL;
    }
}

unsigned Guest::getNextClientNumber()
{
    return nextClientNumber;
}

void Guest::setNextClientNumber(unsigned value)
{
    if (value > nextClientNumber)
    {
        nextClientNumber = value;
    }
}


void Guest::serialize(std::ostream& ofs) const
{
    ofs << clientNumber << "\n";
    ofs << name << "\n";
    ofs << mobileNumber << "\n";
    ofs << mail << "\n";
    ofs << static_cast<int>(status) << " ";
    ofs << totalNights << " ";
    ofs << totalSpent << "\n";
}