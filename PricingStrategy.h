#pragma once
#include "MyString.h"
#include <iostream>

enum PricingStrategyType 
{
    DEFAULT_PRICING,
    SEASONAL_PRICING,
    WEEKEND_PRICING,
    UNKNOWN_PRICING_STRATEGY
};

class PricingStrategy
{
public:
    virtual ~PricingStrategy() = default;
    virtual double calculatePrice(double basePricePerNight, int nights) const = 0;

    virtual PricingStrategyType getStrategyType() const = 0;
    virtual MyString getStrategyName() const = 0;
    virtual void serialize(std::ostream& os) const = 0;
};
