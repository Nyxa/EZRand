#pragma once

#include "PerlinNoise.hpp"
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

    template<std::floating_point T>
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

            constexpr T Zero = static_cast<T>(0.0);

            if (Min >= Zero)
                RangeType = ERangeType::Positive;
            else if (Min < Zero && Max < Zero)
                RangeType = ERangeType::Negative;
            else
                RangeType = ERangeType::Balanced;
        }
    };

    template<std::floating_point T>
    constexpr T Abs(const T x)
    {
        return (x < 0 ? x * -1 : x);
    }

    template<std::floating_point Ty>
    class Rand
    {
        static constexpr ui8 DEFAULT_OCTAVE = 2;
        static constexpr float DEFAULT_SPREAD = 1.35f;
        static constexpr ui32 DEFAULT_SEED = 69u;
        
    public:
        
        explicit Rand(const Range<Ty> InRange = {static_cast<Ty>(0), static_cast<Ty>(1)},
                      const ui8 InOctave = DEFAULT_OCTAVE,
                      const float InSpread = DEFAULT_SPREAD,
                      const ui32 InSeed = DEFAULT_SEED)
            : Counter(0)
            , Octave(InOctave)
            , Spread(InSpread)
            , PerlinNoise({InSeed})
            , ValueRange(InRange)
            , AbsMin(Abs(ValueRange.Min))
            , AbsMax(Abs(ValueRange.Max))
        {
            InitNoiseDelegate();
        }
        
        explicit Rand(const Ty InMin = 0, const Ty InMax = 1,
                      const ui8 InOctave = DEFAULT_OCTAVE,
                      const float InSpread = DEFAULT_SPREAD,
                      const ui32 InSeed = DEFAULT_SEED)
            : Counter(0)
            , Octave(InOctave)
            , Spread(InSpread)
            , PerlinNoise({InSeed})
            , ValueRange(InMin, InMax)
            , AbsMin(Abs(ValueRange.Min))
            , AbsMax(Abs(ValueRange.Max))
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
        const char* GetRangeTypeAsText() const
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

        // Sets a new value range
        void UpdateRange(const Ty RangeMin, const Ty RangeMax)
        {
            ValueRange = Range(RangeMin, RangeMax);
            AbsMin = Abs(ValueRange.Min);
            AbsMax = Abs(ValueRange.Max);
        }

        void UpdateRange(const Range<Ty> InRange)
        {
            ValueRange = InRange;
            AbsMin = Abs(ValueRange.Min);
            AbsMax = Abs(ValueRange.Max);
        }

        void UpdateSeed(const ui32 InSeed)
        {
            PerlinNoise.reseed(InSeed);
        }

        Range<Ty> GetValueRange() const
        {
            return ValueRange;
        }

    private:

#pragma warning(push)
#pragma warning(disable:4244)
        // Narrowing conversion literally doesn't matter here as the Spread value's precision isn't very important
        void InitNoiseDelegate()
        {
            switch(ValueRange.RangeType)
            {
                using enum ERangeType;

            case Positive:
            case Negative:
                NoiseDelegate = [this](const i32 a, const i32 b, const float c) { return PerlinNoise.octave1D_01(a, b, c); };  // NOLINT(clang-diagnostic-double-promotion)
                break;
            case Balanced:
                NoiseDelegate = [this](const i32 a, const i32 b, const float c) { return PerlinNoise.octave1D_11(a, b, c); };  // NOLINT(clang-diagnostic-double-promotion)
            }
        }
#pragma warning(pop)
        
        [[nodiscard]]
        Ty ComputeOutput(const Ty Input) const
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

    public:
        ui32 Counter;
        ui8 Octave;
        float Spread;
        
    private:
        siv::PerlinNoise PerlinNoise;
        Range<Ty> ValueRange;
        Ty AbsMin;
        Ty AbsMax;
        std::function<Ty(i32, i32, float)> NoiseDelegate;
    };
}
