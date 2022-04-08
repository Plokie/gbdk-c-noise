/*
This entire conversion project thing is HEAVILY based on the implementation at https://gist.github.com/nowl/828013 by Nowl
Conversion to using fixed integers intended for GBDK by Plokie
*/
#include <gb/gb.h>

/*
    How fixed numbers should be handled:
        - Defined by 16 bit integer
        
        - The decimal part is defined from 0 to 256 repeating, where 0 is 0.0 and 256 is 1.0
        - The integer part is incremented as the decimal increases every 256
        
        - The decimal part can be found with x % 256 (0 = 0.0, 256=1.0)
        - The integer part can be found with x / 256
*/
/* Lerps a to b by x where x is a fixed point decimal part (0-256). r*/
uint_fast8_t fixed_lerp(uint_fast8_t a, uint_fast8_t b, uint_fast8_t x ) { return ( a * (256 - x) + b * x ) >> 8; }
/* Returns the integer part of a fixed number */
uint_fast8_t fixed_integer_part(uint_fast16_t x) { return x / 256; }
/* Returns the decimal part of a fixed number (0-256 = 0.0-1.0) */
uint_fast8_t fixed_decimal_part(uint_fast16_t x) { return x % 256; }

//Seed to use for the noise
static uint_fast8_t noise_seed = 26u;
//Hash map to base noise values from. Can be changed if you like.
static uint_fast8_t hash[] = {208,34,231,213,32,248,233,56,161,78,24,140,71,48,140,254,245,255,247,247,40,
                     185,248,251,245,28,124,204,204,76,36,1,107,28,234,163,202,224,245,128,167,204,
                     9,92,217,54,239,174,173,102,193,189,190,121,100,108,167,44,43,77,180,204,8,81,
                     70,223,11,38,24,254,210,210,177,32,81,195,243,125,8,169,112,32,97,53,195,13,
                     203,9,47,104,125,117,114,124,165,203,181,235,193,206,70,180,174,0,167,181,41,
                     164,30,116,127,198,245,146,87,224,149,206,57,4,192,210,65,210,129,240,178,105,
                     228,108,245,148,140,40,35,195,38,58,65,207,215,253,65,85,208,76,62,3,237,55,89,
                     232,50,217,64,244,157,199,121,252,90,17,212,203,149,152,140,187,234,177,73,174,
                     193,100,192,143,97,53,145,135,19,103,13,90,135,151,199,91,239,247,33,39,145,
                     101,120,99,3,186,86,99,41,237,203,111,79,220,135,158,42,30,154,120,67,87,167,
                     135,176,183,191,253,115,184,21,233,58,129,233,142,39,128,211,118,137,139,255,
                     114,20,218,113,154,27,127,246,250,1,8,198,250,209,92,222,173,21,88,102,219};
/*
Returns an 8 bit uinteger corresponding to x and y from the hash map. Probably not unique.
*/
uint_fast8_t noise2(uint8_t x, uint8_t y)
{
    uint_fast8_t tmp = hash[(y + noise_seed) % 256];
    return hash[(tmp + x) % 256];
}

//In most cases, this is the function you wanna use vvvv
/*
    Takes x and y unsigned integers as inputs and return a noise value from 0 to 255
    X and Y are 16 bit unsigned integers and so are in the range 0-65536

    I generally recommend to multiply the input x and y by 50 for good-looking noise
*/
uint_fast8_t noise2d(uint16_t x, uint16_t y)
{
    uint_fast8_t x_int = fixed_integer_part(x);
    uint_fast8_t y_int = fixed_integer_part(y);
    uint_fast8_t x_dec = fixed_decimal_part(x);
    uint_fast8_t y_dec = fixed_decimal_part(y);

    uint_fast8_t s = noise2(x_int, y_int);
    uint_fast8_t t = noise2(x_int+1, y_int);
    uint_fast8_t u = noise2(x_int, y_int+1);
    uint_fast8_t v = noise2(x_int+1, y_int+1);

    uint_fast8_t low = fixed_lerp(s, t, x_dec);
    uint_fast8_t high = fixed_lerp(u, v, x_dec);
    return fixed_lerp(low, high, y_dec);
}
/*
To be honest this is kind of experimental and im not sure if it works or not, its just sort of here if you
want to try to use it in whatever you may want to use it for. Can't promise it works though. Sorry!
*/
uint_fast8_t fbm(uint16_t x, uint16_t y, uint8_t freq, uint8_t depth)
{
    uint_fast16_t xa = fixed_integer_part(x) * freq;
    uint_fast16_t ya = fixed_integer_part(y) * freq;
    uint_fast16_t amp = 256;
    uint_fast16_t fin = 0;
    uint_fast16_t div = 0;

    for(int i=0; i<depth; i++)
    {
        div+=256*fixed_integer_part(amp);
        fin += noise2d(xa,ya) * amp;
        amp/=2;
        xa*=2; ya*=2;
    }

    return fixed_integer_part(fin) / fixed_integer_part(div);
}

//<3
