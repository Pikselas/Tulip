#pragma once
 
/*
  Linear interpolation
    a: start value
    b: end value
    t: interpolation factor
  returns: value on the line between a and b at t
*/
float lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

/*
  fade function for perlin noise based on
  equation 6 * t^5 - 15 * t^4 + 10 * t^3
*/

float fade(float t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

/*
  Perlin noise permutation table
  This is randomly shuffled numbers from 0 to 255
    This is used to generate a random gradient vector
*/


// This method is faster than using std::floor

int fastfloor(float x)
{
    return x > 0 ? static_cast<int>(x) : static_cast<int>(x) - 1;
}

/*
  Perlin noise gradient function
    hash: hash value
    x: x coordinate
    y: y coordinate
    z: z coordinate
  returns: value between -1 and 1
  This function is used for calculating the dot product between the gradient vector and the distance vector

  picks a random vector from the following 12 vectors:

  (1,1,0) , (-1,1,0) , (1,-1,0) , (-1,-1,0),
  (1,0,1) , (-1,0,1) , (1,0,-1) , (-1,0,-1),
  (0,1,1) , (0,-1,1) , (0,1,-1) , (0,-1,-1)

  this is the same as the following code
       0x0  ->    x  +  y
       0x1  ->  - x  +  y
       0x2  ->    x  -  y
       0x3  ->  - x  -  y
       0x4  ->    x  +  z
       0x5  ->  - x  +  z
       0x6  ->    x  -  z
       0x7  ->  - x  -  z
       0x8  ->    y  +  z
       0x9  ->  - y  +  z
       0xA  ->    y  -  z
       0xB  ->  - y  -  z
       0xC  ->    y  +  x
       0xD  ->  - y  +  z
       0xE  ->    y  -  x
       0xF  ->  - y  -  z

*/
float grad(int hash, float x, float y, float z)
{
    const auto h = hash & 15; // takes the last 4 bits of the hash
    const auto u = h < 8 ? x : y;
    const auto v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

constexpr uint8_t perlin_permutation[] =
{
    151, 160, 137, 91, 90, 15,
    131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
    190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
    88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
    77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
    102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
    135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
    5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
    223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
    129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
    251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
    49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
    138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
};

/*
  Perlin noise
    x: x coordinate
    y: y coordinate
    z: z coordinate
  returns: value between -1 and 1
*/

float perlin(float x, float y, float z)
{
    const auto xint = fastfloor(x);
    const auto yint = fastfloor(y);
    const auto zint = fastfloor(z);

    /*
      Unit cube that contains the point
       the cube is defined by the lower left corner (i.e the integer part of the coordinates)
       for a point (x, y, z) the cube is defined by (x0, y0, z0) to (x1, y1, z1)
       and the fractional part of the coordinates is inside the cube

     (x0 , y1) ----------- (x1 , y1)
               |         |
               |         |
               |     (.) |   -> (xfrac, yfrac)
               |         |
     (x0 , y0) ----------- (x1 , y0)

    */

    const auto x0 = xint & 255;
    const auto y0 = yint & 255;
    const auto z0 = zint & 255;

    const auto x1 = x0 + 1;
    const auto y1 = y0 + 1;
    const auto z1 = z0 + 1;

    /*
      Fractional part of the coordinates
    */

    const auto xfrac = x - xint;
    const auto yfrac = y - yint;
    const auto zfrac = z - zint;

    /*
      Fade curves
        Fade curves are used to smooth the interpolation
        where t is the fractional part of the coordinates
    */

    const auto u = fade(xfrac);
    const auto v = fade(yfrac);
    const auto w = fade(zfrac);

    /*
      Hash coordinates of the 8 cube corners
        The hash function is used to generate a random number from the coordinates
        The random number is used to index the gradient table
    */

    const auto aaa = perlin_permutation[perlin_permutation[perlin_permutation[x0] + y0] + z0];
    const auto aba = perlin_permutation[perlin_permutation[perlin_permutation[x0] + y1] + z0];
    const auto aab = perlin_permutation[perlin_permutation[perlin_permutation[x0] + y0] + z1];
    const auto abb = perlin_permutation[perlin_permutation[perlin_permutation[x0] + y1] + z1];
    const auto baa = perlin_permutation[perlin_permutation[perlin_permutation[x1] + y0] + z0];
    const auto bba = perlin_permutation[perlin_permutation[perlin_permutation[x1] + y1] + z0];
    const auto bab = perlin_permutation[perlin_permutation[perlin_permutation[x1] + y0] + z1];
    const auto bbb = perlin_permutation[perlin_permutation[perlin_permutation[x1] + y1] + z1];

    // Interpolate between the gradients at the corners
    const auto x1y1z1 = lerp(grad(aaa, xfrac, yfrac, zfrac), grad(baa, xfrac - 1, yfrac, zfrac), u);
    const auto x0y1z1 = lerp(grad(aba, xfrac, yfrac - 1, zfrac), grad(bba, xfrac - 1, yfrac - 1, zfrac), u);
    const auto x1y0z1 = lerp(grad(aab, xfrac, yfrac, zfrac - 1), grad(bab, xfrac - 1, yfrac, zfrac - 1), u);
    const auto x0y0z1 = lerp(grad(abb, xfrac, yfrac - 1, zfrac - 1), grad(bbb, xfrac - 1, yfrac - 1, zfrac - 1), u);

    // Interpolate between the results above
    const auto x1y1z0 = lerp(x1y1z1, x1y0z1, v);
    const auto x0y1z0 = lerp(x0y1z1, x0y0z1, v);

    return lerp(x1y1z0, x0y1z0, w);
}

float OctavePerlin(float x, float y, float z, int octaves, float persistence) {
    double total = 0;
    double frequency = 1;
    double amplitude = 1;
    double maxValue = 0;  // Used for normalizing result to 0.0 - 1.0
    for (int i = 0; i < octaves; i++) {
        total += perlin(x * frequency, y * frequency, z * frequency) * amplitude;

        maxValue += amplitude;

        amplitude *= persistence;
        frequency *= 2;
    }

    return total / maxValue;
}