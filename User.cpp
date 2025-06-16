#include "User.h"

MyString userRoleToString(UserRole role)
{
    switch (role)
    {
    case ROLE_MANAGER: return "Manager";
    case ROLE_RECEPTIONIST: return "Receptionist";
    case ROLE_ACCOUNTANT: return "Accountant";
    default: return "Unknown Role";
    }
}

User::User(const MyString& userName, const MyString& password, UserRole userRole)
    : userName(userName), password(password), role(userRole)
{
}

UserRole User::getRole() const
{
    return role;
}

const MyString& User::getUsername() const
{
    return userName;
}

MyString User::getRoleString() const
{
    return userRoleToString(role);
}

bool User::checkPassword(const MyString& plainPassword) const
{
    return plainPassword == this->password;
}

void User::serialize(std::ostream& os) const 
{
    os << static_cast<int>(role) << "\n";
    os << userName << "\n";
    os << password << "\n";
}