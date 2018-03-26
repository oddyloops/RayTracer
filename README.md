# G-TRACER

# Motivation
The most popular techniques used in image renderers have been hardware Z-buffering, and ray tracing. The earlier is mostly used in video games, and movie
CGIs for real time generation of high quality images. Some of these modern games/movies (including Virtual/Augmented
Reality) require over 120 of this images to be generated within fractions of a second. The hardware renders are able to keep up because they have an army
of dumb GPU cores crunching elementary school math to create these visual "illusions". They are illusions, because in hardware rendering, a shadow is not really a shadow,
a reflection isnt what you think. They are bunch of sleight-of-hand tricks by crafty programmers. That is not the case with ray-tracers. In hardware z-buffers, WYSIWYG (What is
what you get), as for ray-tracers we go by WYGIWYG (What you give is what you get). That is a much fairer deal.

The ray-tracers are slower, sometimes taking hours to render a single scene. As a result, their use has been limited to making animated movies 
like Pixar Cars. Once a mistake is made in  a scene, it has to be re-rendered to correct such mistakes which are mostly only visibile in the high-res images
It could take several months to animate an entire movie. It surprised me as to why this slow and delicate technique is still in use. However, understanding the reasons require us
to know why they are slow in the first place. 

"Level of details". Ray-Tracers are built to emulate real life optics and how they are related to our visibility. It is an 
abstraction of the world around you from the perspective of how light sends information to the human eye. This makes them more intuitive, extendable, realistic, and more fun to play with.
However, complex calculations beyond elementary math are required. Fortunately, the CPU cores are Phd level smart (unlike their GPU counterparts) with an almost 
infinite IQ (technically, only qunatum processors can have near infinite IQ). Unfortunately, just like Phd employees,it cost a lot to hire them literally. 
A CPU core can buy hundreds of GPU cores. Secondly, they like to multitask and hate taking orders. 

They execute code in their own time because they feel they are smarter than the actual programmers who wrote the codes in the first place. We could speed things up a bit by throwing  multiple cores because image rendering
at a very basic level is done per-pixel, with each pixel being independent of the other. That screams "parallelization". However, only a maximum of four cores (or 8 with hyperthreading)
are available on commodity PCs. We could optimize further through distributed ray-tracing by rendering different fragments of the scene across an entire network of computers, but then you 
will have a lot of idle GPU cores wasting away and laughing at how overwhelmed the superior ego-centric CPUs are. That is the entire point of this project. To foster cooperation 
between the Phd level CPUs, and elementary grade GPUs as it has been done in machine learning and other data intensive computer science fields. The CPU does all the complicated tasks
of interfacing with the program I/O to load scene information, while the GPUs take it from there and crush those vector algebrae like happy 7th graders.



# Description
G-Tracer stands for GPU tracer. It is a noble attempt in running CPU based ray-tracing algorithms on GPUs. It utilizes Microsoft C++ AMP framework to abstract the GPU dependent code into 
plain old C++. There are other GPU frameworks out there that might be simpler but C++ AMP takes the cake because it is hardware independent (doesnt care if you are on AMD, NVIDEA, etc), or
even a windows phone, X-Box, whatever, and also because of Visual Studio.


# Build
- Requires a minimum of 64-bit windows 8, and VS 2017 community edition or higher, and it does not matter if your computer has a GPU or not. 
- The AMP framework knows how to fallback to whatever accelerator is available.
- Clone the source code through the URL: https://github.com/oddyloops/RayTracer.git. This pulls a working copy in the Master branch. 
- Open the AMPRayTracer folder, and use Visual Studio to open the solution file.
- Build the solution in Release x64 Mode, and you can execute the JsonParser using the sample json_test.json file as input.
- You can also just reference the RTEngine dll in your C++ Application, and use the JsonParser source to guide you through its usage.


More of the usage details will be explained in the Wiki.


This is still a work in progress, as the current Renderer does not handle meshes and texture maps yet. I will also add a wiki page to 
document each classes, their relationships once my stress levels go down.





