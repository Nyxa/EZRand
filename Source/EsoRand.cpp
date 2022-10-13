#include "EsoRand.hpp"

#include <format>
#include <iostream>
#include <chrono>

#define OUTPUT_VALUES 0

int main()
{
    using RngType = float;
    constexpr RngType RangeMin = -500.0;
    constexpr RngType RangeMax = -50.0;
    
    eso::Rand<RngType> Randomizer(RangeMin, RangeMax);

    constexpr int IterCount = 600000;
    std::cout << std::format("Computing {} random values with Min: {} | Max: {}\n", IterCount, RangeMin, RangeMax);
    std::cout << "Range type is: " << Randomizer.GetRangeType() << "\n";

    RngType MaxOutput = RangeMin;
    RngType MinOutput = RangeMax;

    const auto ClockStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < IterCount; ++i)
    {
        const RngType Output = Randomizer.GetNext();
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
    const auto ClockEnd = std::chrono::high_resolution_clock::now();
    const auto ExecutionTime = ClockEnd - ClockStart;
    const auto ExecutionTimeMS = std::chrono::duration_cast<std::chrono::milliseconds>(ExecutionTime);
    const auto ExecutionTimeUS = std::chrono::duration_cast<std::chrono::microseconds>(ExecutionTime);

    std::cout << std::format("\nMin output: {} | Max output: {}", MinOutput, MaxOutput);
    std::cout << std::format("\nExecuted in: {} | {}", ExecutionTimeMS, ExecutionTimeUS);
}
