#include "WeekendPricingStrategy.h"

double WeekendPricingStrategy::calculatePrice(double basePricePerNight, int nights) const
{
    if (nights <= 0)
    {
        return 0.0;
    }
    const double WEEKEND_PRICE_MULTIPLIER = 1.25;
    return (basePricePerNight * WEEKEND_PRICE_MULTIPLIER) * nights;
}

PricingStrategyType WeekendPricingStrategy::getStrategyType() const
{
    return WEEKEND_PRICING;
}

MyString WeekendPricingStrategy::getStrategyName() const
{
    return "Weekend Pricing";
}

void WeekendPricingStrategy::serialize(std::ostream& os) const
{
    os << static_cast<int>(getStrategyType()) << "\n";
}
