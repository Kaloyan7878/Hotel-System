#pragma once
#include "PricingStrategy.h"

class SeasonalPricingStrategy : public PricingStrategy
{
	double seasonalMultiplier;
public:
	SeasonalPricingStrategy();
	SeasonalPricingStrategy(double multiplier);

	double calculatePrice(double basePricePerNight, int nights) const override;
	PricingStrategyType getStrategyType() const override;
	MyString getStrategyName() const override;
	void serialize(std::ostream& os) const override;

	double getMultiplier() const;
};