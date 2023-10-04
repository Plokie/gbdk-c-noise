# gbdk-c-noise
A simple header file that contains procedural noise functions for use with GBDK.
Comparable to basic simplex noise, and includes an experimental perlin noise / fbm noise implementation that I don't think works but might? I'm not sure tbh, I couldn't yield useful results.

Based on the C fbm header by nowl at https://gist.github.com/nowl/828013 and modified using some fixed point stuff i made up

For information on how it works, see the header file comments

Example usage:

```c++
//Loop through each x and y coordinate to-be generated
for(uint_fast8_t x=0; x<targetMap_Width; x++) {
    for(uint_fast8_t y=0; y<targetMap_Height; y++) {
    
        uint_fast8_t val = noise2d(x*45,y*45); //Sample simplex noise
        
        if(val<128) set_element(x,y,water); //Below ocean level
        else set_element(x,y,grass); //Above ocean level = land (grass)
    }
}
```

Demonstration

![Screenshot of a GameBoy game, demonstrating the usage of the noise](https://plokie.com/other/gb.png)
<3
