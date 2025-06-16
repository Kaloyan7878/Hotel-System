#include "SeasonalPricingStrategy.h"

SeasonalPricingStrategy::SeasonalPricingStrategy() : seasonalMultiplier(1.15) {}

SeasonalPricingStrategy::SeasonalPricingStrategy(double multiplier) : seasonalMultiplier(multiplier > 0 ? multiplier : 1.0)
{
}

double SeasonalPricingStrategy::calculatePrice(double basePricePerNight, int nights) const
{
    if (nights <= 0)
    {
        return 0.0;
    }
    return basePricePerNight * nights * seasonalMultiplier;
}

PricingStrategyType SeasonalPricingStrategy::getStrategyType() const
{
    return SEASONAL_PRICING;
}

MyString SeasonalPricingStrategy::getStrategyName() const
{
    char buffer[50];
    return "Seasonal Pricing";
}


void SeasonalPricingStrategy::serialize(std::ostream& os) const
{
    os << static_cast<int>(getStrategyType()) << " " << seasonalMultiplier << "\n";
}

double SeasonalPricingStrategy::getMultiplier() const {
    return seasonalMultiplier;
}