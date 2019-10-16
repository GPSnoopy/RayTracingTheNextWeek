# Ray Tracing The Next Week

<img align="center" src="https://github.com/GPSnoopy/RayTracingTheNextWeek/blob/master/Book2.jpg">

My take on [Peter Shirley's Ray Tracing The Next Week](https://github.com/petershirley/raytracingthenextweek) book. This is the second book in his *Ray Tracing In One Weekend* series. My take on his first book is available [here](https://github.com/GPSnoopy/RayTracingInOneWeekend).

On top of my own programming style, I have added:
* Multi-threading support (using a separate random number generator for each thread).
* Optimizing AABB ray intersection by removing conditional branches (this runs faster on any modern processor).
* Building the BVH tree using `depth%3` splitting rather than random one (this seems to run faster in my case).
* Using SSE and AVX instructions for implementing the Vec3 class.

The Earth texture was found on [NASA's The Blue Marbe](https://visibleearth.nasa.gov/view.php?id=57735).

## Building

Visual Studio 2019 project files provided as-is. Use `build_linux.sh` for a GCC build, be sure to change the `-march=skylake` to match your  processor.

## Performance

The cover picture was rendered in about 26h running 14 threads on a Core i9-9900K @ 4.7GHz at 3840x2160 with 65536 samples per pixel. I've used [GIMP](https://www.gimp.org/) to open the resulting PPM image and convert it to more standard formats such as PNG and JPEG.

For best performance, I recommend the Linux build even on Windows (I've used Ubuntu 18.04 running via [WSL on Windows 10 1809](https://en.wikipedia.org/wiki/Windows_Subsystem_for_Linux)). For some reason, GCC (7.3) seems better at generating AVX FMA instructions than Visual Studio 2019 (16.3) on this code.
