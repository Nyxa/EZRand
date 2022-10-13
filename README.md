# EsoRand
Simple RNG library using Perlin Noise.
All it does is add some functionality around a noise library to make it nice for obtaining random values given a specified range.

# Usage
```cpp
// NOTE: C++20 required.
// This is a header-only library, so simply include the EsoRand folder in your project dependencies to use it.
    
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
// Note that, if all other parameters remain unchanged, 
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

// If you want random integers, you can simply cast the output to an int. ```
