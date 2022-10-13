#include "EsoRand.hpp"

#include <format>
#include <iostream>

#define OUTPUT_VALUES 1

int main()
{
    constexpr double RangeMin = -500.0;
    constexpr double RangeMax = -50.0;
    
    eso::Rand<double> Randomizer(RangeMin, RangeMax);

    constexpr int IterCount = 60;
    std::cout << std::format("Computing {} random values with Min: {} | Max: {}\n", IterCount, RangeMin, RangeMax);
    std::cout << "Range type is: " << Randomizer.GetRangeType() << "\n";

    double MaxOutput = RangeMin;
    double MinOutput = RangeMax;
    for (int i = 0; i < IterCount; ++i)
    {
        const double Output = Randomizer.Get(i);
        if (MaxOutput < Output)
            MaxOutput = Output;
        if (MinOutput > Output)
            MinOutput = Output;

#if OUTPUT_VALUES == 1
        if (i > 0 && i % 6 == 0)
            std::cout << "\n";
        
        std::cout << " | " << Output;
#endif
    }

    std::cout << std::format("\nMin output: {} | Max output: {}", MinOutput, MaxOutput);
}
