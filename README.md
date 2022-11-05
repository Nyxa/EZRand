# EsoRand
Simple RNG library using Perlin Noise.
All it does is add some functionality around a noise library to make it nice for obtaining random values given a specified range.

This being noise-based will mean that given the same parameters (seed, octave, spread, range, index), it will always output the same thing. **This is intentional**. I created this library as a helper for game RNG where randomness needs to be replicable. Still, given an arbitrary seed, you can have seemingly random numbers each time the program is run. The default seed does this - it simply uses the time at which the application was launched to determine the seed, producing random values each time the program is ran.

Note that this library uses C++20 concepts to restrict the randomizer to floating point types only. I may one day do a version that won't compile without numeric literals, but for now this library only works with C++20.

This is a pretty lightweight header-only file, so you can just drop it into your project without any issues.

# Usage
```cpp    
// Create a float randomizer with a range of -100/100
// Additional parameters may also be added, such as a custom seed, octave, and spread. What they do is explained below.
eso::Rand Randomizer(-100.f, 100.f);

// Returns random value using internal counter as the index, and progresses said counter.
Randomizer.GetNext();
  
// Returns random value using internal counter without progressing it.
Randomizer.Get();

// Returns random value at specific index. Doesn't affect counter.
Randomizer.Get(42);

// Access internal counter directly.
// The internal counter is just a simple integer that may or may not increment.
// It allows you to repeatedly call the randomizer without having to worry about what index you're at.
Randomizer.Counter = 42;

// Modify Octave parameter. This affects the resolution of the output, meaning a higher octave will give a smoother distribution.
// Note that this has diminishing returns. Anything above an octave of 4 shouldn't really be necessary.
Randomizer.Octave = 2;

// Modify Spread parameter. This adjusts how close to the center of the range the values are (lower spread = more centered)
// It's best to mess around with the spread using different ranges and numbers of iteration to get a feel for how it works.
// It's recommended to avoid a spread greater than 5.0f, as this would lead to extremely biased outputs.
Randomizer.Spread = 1.35f; 

// Modifies the range that output values can be within.
Randomizer.UpdateRange(12.f, 57.f);

// Reseed the randomizer with a positive integer. Note that given the same seed and other parameters, the output will always be the same.
Randomizer.UpdateSeed(12345u);

// Returns an eso::Range object which contains the min/max values currently being used by the randomizer.
// Also contains an ERangeType variable, which tracks whether the range is positive, negative, or mixed.
Randomizer.GetValueRange();    

// Only really useful if you want to output the RangeType to console for some reason.
Randomizer.GetRangeTypeAsText(); 
    
// The randomizer only accepts floating-point types.
eso::Rand IntRandomizer(1, 100); // Won't compile.

// If you want random integers, you can simply cast the output to an int.
