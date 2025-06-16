#pragma once
#include "PricingStrategy.h"

class WeekendPricingStrategy : public PricingStrategy
{
public:
    WeekendPricingStrategy() = default;

    double calculatePrice(double basePricePerNight, int nights) const override;
    PricingStrategyType getStrategyType() const override;
    MyString getStrategyName() const override;
    void serialize(std::ostream& os) const override;
};
