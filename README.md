### simplify-cpp

simplify-cpp is a simple port of simplify.js by Vladimir Agafonkin (https://github.com/mourner/simplify-js)

### Usage

```c++
#include "simplify.hpp"

using namespace simplify_cpp;

Points<double> points = {{224.55, 250.15},...,{866.36, 480.77}};
double tolerance = ...;
bool highQuality = ...;
Points<double> result = simplify(points, tolerance, highQuality);
```

- `points`: A struct containing x, y coordinates.

- `tolerance`: Affects the amount of simplification that occurs (the smaller, the less simplification).

- `highestQuality (optional, true by default)`: Flag to exclude the distance pre-processing. Produces higher quality results, but runs slower.
