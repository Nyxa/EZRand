#pragma once

#include "PerlinNoise/PerlinNoise.hpp"
#include <functional>

namespace eso
{
    using ui8  = uint8_t;
    using ui16 = uint16_t;
    using ui32 = uint32_t;
    using ui64 = uint64_t;

    using i8  = int8_t;
    using i16 = int16_t;
    using i32 = int32_t;
    using i64 = int64_t;

    enum class ERangeType
    {
        Negative,
        Balanced,
        Positive,
    };

    template<typename T>
    concept Number = std::integral<T> || std::floating_point<T>;

    template<Number T>
    struct Range
    {
        T Min;
        T Max;
        ERangeType RangeType;
        
        Range(T InMin, T InMax)
            : Min(InMin)
            , Max(InMax)
        {
            if (Min > Max)
            {
                const T MinTemp = Max;
                const T MaxTemp = Min;
                Min = MinTemp;
                Max = MaxTemp;
            }

            if (Min >= 0.0)
                RangeType = ERangeType::Positive;
            else if (Min < 0.0 && Max < 0.0)
                RangeType = ERangeType::Negative;
            else
                RangeType = ERangeType::Balanced;
        }
    };

    template<Number T>
    constexpr T Abs(const T x)
    {
        return (x < 0.0 ? x * -1.0 : x);
    }

    template<Number Ty>
    class Rand
    {
    public:
        
        explicit Rand(const Range<Ty> InRange = Range(static_cast<Ty>(0.0), static_cast<Ty>(1.0)),
                      const ui8 InOctave = 2,
                      const double InSpread = 1.35,
                      const ui32 InSeed = 69u)
            : Seed(InSeed)
            , PerlinNoise(Seed)
            , Counter(0)
            , ValueRange(InRange)
            , AbsMin(Abs(ValueRange.Min))
            , AbsMax(Abs(ValueRange.Max))
            , Octave(InOctave)
            , Spread(InSpread)
        {
            InitNoiseDelegate();
        }
        
        explicit Rand(const double InMin = 0.0, const double InMax = 1.0,
                      const ui8 InOctave = 2,
                      const double InSpread = 1.35,
                      const ui32 InSeed = 69u)
            : Seed(InSeed)
            , PerlinNoise(Seed)
            , Counter(0)
            , ValueRange(InMin, InMax)
            , AbsMin(Abs(ValueRange.Min))
            , AbsMax(Abs(ValueRange.Max))
            , Octave(InOctave)
            , Spread(InSpread)
        {
            InitNoiseDelegate();
        }

        // Returns random value at current index, and progresses to next value
        [[nodiscard]]
        Ty GetNext()
        {
            return static_cast<Ty>(ComputeOutput(NoiseDelegate(Counter++, Octave, Spread)));
        }

        // Returns value at current index without affecting internal state
        [[nodiscard]]
        Ty Get() const
        {
            return static_cast<Ty>(ComputeOutput(NoiseDelegate(Counter, Octave, Spread)));
        }

        // Returns value at specified index
        [[nodiscard]]
        Ty Get(const i32 Index) const
        {
            return static_cast<Ty>(ComputeOutput(NoiseDelegate(Index, Octave, Spread)));
        }

        [[nodiscard]]
        const char* GetRangeType() const
        {
            switch (ValueRange.RangeType)
            {
                using enum ERangeType;
            case Balanced:
                return "Balanced";
            case Positive:
                return "Positive";
            case Negative:
                return "Negative";
            }
            return "fuck";
        }

    private:
        void InitNoiseDelegate()
        {
            switch(ValueRange.RangeType)
            {
                using enum ERangeType;

            case Positive:
            case Negative:
                NoiseDelegate = [this](const i32 a, const i32 b, const double c) { return PerlinNoise.octave1D_01(a, b, c); };
                break;
            case Balanced:
                NoiseDelegate = [this](const i32 a, const i32 b, const double c) { return PerlinNoise.octave1D_11(a, b, c); };
            }
        }
        
        [[nodiscard]]
        Ty ComputeOutput(const double Input) const
        {
            Ty Output{};
            switch (ValueRange.RangeType)
            {
                using enum ERangeType;
            case Balanced:
                Output = (Input < 0 ? Input * AbsMin : Input * AbsMax);
                break;
            case Negative:
                Output = -Input * AbsMin; // Multiply by value most distant to 0
                if (Output > ValueRange.Max) // Clamp range
                { Output -= AbsMax; } 
                break;
            case Positive:
                Output = Input * AbsMax;
                if (Output < AbsMin)
                { Output += AbsMin; }
                break;
            }
            return Output;
        }
        
    private:
        const siv::PerlinNoise::seed_type Seed;
        const siv::PerlinNoise PerlinNoise;
        
        ui32 Counter;
        const Range<Ty> ValueRange;
        const Ty AbsMin;
        const Ty AbsMax;
        ui8 Octave;
        double Spread;
        
        std::function<Ty(i32, i32, double)> NoiseDelegate;
    };
}
