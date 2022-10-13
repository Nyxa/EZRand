# EsoRand
Simple RNG library using Perlin Noise.
All it does is add some functionality around a noise library to make it nice for obtaining random values given a specified range.

# Usage
```cpp
    // Create a float randomizer with a range of -100/100
    eso::Rand FloatRandomizer(-100.f, 100.f);

    FloatRandomizer.GetNext();      // Returns random value using internal counter, and progresses it
    FloatRandomizer.Get();          // Returns random value using internal counter without progressing
    FloatRandomizer.Get(42);   // Returns random value at specific index. Doesn't affect counter.
    FloatRandomizer.Counter = 42;   // Access internal counter directly.
    FloatRandomizer.Octave = 2;     // Modify Octave parameter. This affects the resolution of the output (higher octave = smoother distribution, slower processing)
    FloatRandomizer.Spread = 1.35f; // Modify Spread parameter. This adjusts how close to the center of the range the values are (lower spread = more centered)
    
    FloatRandomizer.UpdateRange(12.f, 57.f); // Modifies the specified range. Be careful with changing this as it will give entirely different results.
    FloatRandomizer.UpdateSeed(12345u); // Reseed the randomizer.
    FloatRandomizer.GetValueRange();    // Returns an eso::Range object which contains the min/max values currently being used by the randomizer. Also contains RangeType.
    
    FloatRandomizer.GetRangeTypeAsText(); // Only really useful if you want to output the RangeType for some reason.


    // Doubles are also fine, but integers will not be accepted.
    eso::Rand IntRandomizer(1, 100); // Won't compile.

    // If you want random integers, you can simply cast the output to an int.```
