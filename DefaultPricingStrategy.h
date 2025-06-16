#pragma once
#include "PricingStrategy.h"

class DefaultPricingStrategy : public PricingStrategy
{
public:
    DefaultPricingStrategy() = default;

    double calculatePrice(double basePricePerNight, int nights) const override;
    PricingStrategyType getStrategyType() const override;
    MyString getStrategyName() const override;
    void serialize(std::ostream& os) const override;
};