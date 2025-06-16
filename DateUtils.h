#pragma once
#include "MyString.h"
#include <ctime> 


time_t parseDateStringToTimeT(const MyString& dateString);
MyString formatTimeTToDateString(time_t rawTime);