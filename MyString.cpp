#include "MyString.h"
#include <cstring>y
#include <stdexcept>
#include <iostream>
#pragma warning(disable:4996)

void MyString::resize(unsigned newCapacity) {
    char* newData = new char[newCapacity + 1];
    if (data) {
        strncpy(newData, data, size < newCapacity ? size : newCapacity);
    }
    newData[size] = '\0';
    delete[] data;
    data = newData;
    capacity = newCapacity;
}

void MyString::free() {
    delete[] data;
    data = nullptr;
    size = 0;
    capacity = 0;
}

void MyString::copyFrom(const MyString& other) {
    size = other.size;
    capacity = other.capacity;
    data = new char[capacity + 1];
    if (other.data) 
    {
        strcpy(data, other.data);
    }
    else 
    {
        data[0] = '\0';
    }
}

void MyString::moveFrom(MyString&& other) {
    data = other.data;
    size = other.size;
    capacity = other.capacity;

    other.data = nullptr;
    other.size = 0;
    other.capacity = 0;
}

MyString::MyString() : data(nullptr), size(0), capacity(15) 
{

    data = new char[capacity + 1];
    data[0] = '\0';
}

MyString::MyString(const char* str) : data(nullptr), size(0), capacity(0)
{
    if (str) 
    {
        size = strlen(str);
        capacity = (size > 15 ? size : 15);
        data = new char[capacity + 1];
        strcpy(data, str);
    }
    else 
    {
        capacity = 15;
        data = new char[capacity + 1];
        data[0] = '\0';
    }
}

MyString::MyString(const MyString& other)
{
    copyFrom(other);
}

MyString& MyString::operator=(const MyString& other)
{
    if (this != &other)
    {
        free();
        copyFrom(other);
    }
    return *this;
}

MyString::MyString(MyString&& other) noexcept
{
    moveFrom(std::move(other));
}

MyString& MyString::operator=(MyString&& other) noexcept
{
    if (this != &other)
    {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

MyString::~MyString() 
{
    free();
}

size_t MyString::getSize() const
{
    return size;
}

size_t MyString::getCapacity() const
{
    return capacity;
}

const char* MyString::c_str() const
{
    return data ? data : "";
}

char& MyString::operator[](unsigned index) 
{
    if (index >= size)
    {
        throw std::out_of_range("Index out of bounds in MyString::operator[]");
    }
    return data[index];
}

const char& MyString::operator[](unsigned index) const
{
    if (index >= size)
    {
        throw std::out_of_range("Index out of bounds in MyString::operator[] const");
    }
    return data[index];
}

MyString& MyString::operator+=(const MyString& other)
{
    if (other.size == 0) return *this;
    if (size + other.size > capacity) 
    {
        resize((size + other.size) > (capacity * 2) ? (size + other.size) : (capacity * 2));
    }
    strcat(data, other.data);
    size += other.size;
    return *this;
}

MyString operator+(const MyString& lhs, const MyString& rhs) 
{
    MyString temp(lhs);
    temp += rhs;
    return temp;
}

std::ostream& operator<<(std::ostream& os, const MyString& str)
{
    if (str.data)
    {
        os << str.data;
    }
    return os;
}

std::istream& operator>>(std::istream& is, MyString& str) 
{
    char buffer[1024];
    is >> buffer;

    str.free();
    if (is.good() || is.eof()) 
    {
        str.size = strlen(buffer);
        str.capacity = str.size > 15 ? str.size : 15;
        str.data = new char[str.capacity + 1];
        strcpy(str.data, buffer);
    }
    else 
    {
        str.size = 0;
        str.capacity = 15;
        str.data = new char[str.capacity + 1];
        str.data[0] = '\0';
    }
    return is;
}

MyString MyString::substr(unsigned begin, unsigned howMany) const {
    if (begin > size) 
    {
        return MyString("");
    }
    if (begin + howMany > size)
    {
        howMany = size - begin;
    }
    if (howMany == 0)
    {
        return MyString("");
    }

    char* subBuffer = new char[howMany + 1];
    strncpy(subBuffer, data + begin, howMany);
    subBuffer[howMany] = '\0';

    MyString result(subBuffer);
    delete[] subBuffer;
    return result;
}

bool operator==(const MyString& lhs, const MyString& rhs) {
    if (lhs.getSize() != rhs.getSize()) return false;
    return strcmp(lhs.c_str(), rhs.c_str()) == 0;
}

bool operator!=(const MyString& lhs, const MyString& rhs) {
    return !(lhs == rhs);
}

bool operator>(const MyString& lhs, const MyString& rhs) {
    return strcmp(lhs.c_str(), rhs.c_str()) > 0;
}

bool operator>=(const MyString& lhs, const MyString& rhs) {
    return strcmp(lhs.c_str(), rhs.c_str()) >= 0;
}

bool operator<(const MyString& lhs, const MyString& rhs) {
    return strcmp(lhs.c_str(), rhs.c_str()) < 0;
}

bool operator<=(const MyString& lhs, const MyString& rhs) {
    return strcmp(lhs.c_str(), rhs.c_str()) <= 0;
}