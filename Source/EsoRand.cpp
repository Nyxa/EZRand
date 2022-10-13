#include "EsoRand.hpp"

#include <format>
#include <iostream>
#include <chrono>

#define OUTPUT_VALUES 1

template<typename T>
void Test(eso::Rand<T> Randomizer, const eso::i32 IterCount = 60)
{
    const auto Range = Randomizer.GetValueRange();
    std::cout << std::format("Computing {} random values with Min: {} | Max: {}\n", IterCount, Range.Min, Range.Max);
    std::cout << "Range type is: " << Randomizer.GetRangeTypeAsText() << "\n";

    T MaxOutput = Range.Min;
    T MinOutput = Range.Max;

    const auto ClockStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < IterCount; ++i)
    {
        const T Output = Randomizer.GetNext();
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

#if OUTPUT_VALUES == 0
    std::cout << std::format("\nExecuted in: {} | {}", ExecutionTimeMS, ExecutionTimeUS);
#endif
}

template<typename T>
void TestInt(eso::Rand<T> Randomizer, const eso::i32 IterCount = 60)
{
    const auto Range = Randomizer.GetValueRange();
    std::cout << std::format("Computing {} random values with Min: {} | Max: {}\n", IterCount, Range.Min, Range.Max);
    std::cout << "Range type is: " << Randomizer.GetRangeTypeAsText() << "\n";

    eso::i32 MaxOutput = static_cast<int>(Range.Min);
    eso::i32 MinOutput = static_cast<int>(Range.Max);

    const auto ClockStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < IterCount; ++i)
    {
        const eso::i32 Output = static_cast<eso::i32>(Randomizer.GetNext());
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

#if OUTPUT_VALUES == 0
    std::cout << std::format("\nExecuted in: {} | {}", ExecutionTimeMS, ExecutionTimeUS);
#endif
}

int main()
{
    eso::Rand RandomFloatsNegative(-500.f, -50.f);
    eso::Rand RandomFloatsPositive(1.f, 100.f);
    eso::Rand RandomFloatsBalanced(-250.f, 250.f);

    constexpr eso::i32 IterCount{24};

    std::cout << "Testing Integer outputs\n";
    TestInt(RandomFloatsPositive, IterCount);
    std::cout << "\nUpdating Spread\n";
    RandomFloatsPositive.Spread = 0.15f;
    TestInt(RandomFloatsPositive, IterCount);
}
