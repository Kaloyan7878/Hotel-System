#include "DefaultPricingStrategy.h"

double DefaultPricingStrategy::calculatePrice(double basePricePerNight, int nights) const
{
    if (nights <= 0)
    {
        return 0.0;
    }
    return basePricePerNight * nights;
}

PricingStrategyType DefaultPricingStrategy::getStrategyType() const
{
    return DEFAULT_PRICING;
}

MyString DefaultPricingStrategy::getStrategyName() const
{
    return "Default Pricing";
}

void DefaultPricingStrategy::serialize(std::ostream& os) const
{
    os << static_cast<int>(getStrategyType()) << "\n";
}
