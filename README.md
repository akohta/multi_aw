# multi_aw
This is the sound pressure analysis program for focused beams. 
This program can analyze plane waves, Bessel beams, focused beams and those superpositions.


![analysis model](multi_aw_fb.png "analysis model")

## Usage of example code

1. type 'make' command to compile. libpng is required.  
   The executable example1.out, example2.out, example3.out are created.
   The example1.out is the executable of source code example1.c, it shows a simplest example and verification results using "multi_aw". 
   The example2.out is the executable of source code example2.c, it shows a example of sound pressure and particle velocity intensity analysis.
   The example3.out is the executable of source code example3.c, 
   it shows a example of outputting the instantaneous value of the sound pressure and particle velocity as an image.  
   
2. type './example1.out'.  
   This executable calculates sound pressure and particle velocity. 
   The beam datafile "mfb.txt" is used. 
   This program searches for a beam datafile in the current directory using the default beam datafile name and reads the found files. 
   The default beam datafile name is defined for plane wave as "mpw.txt", for Bessel beam as "mbb.txt", for focused beam as "mfb.txt". 
   The samples of beam datafile are in the folder beam_sample, please copy and use.
   
3. type './example2.out'.  
   This executable calculates sound pressure and particle velocity intensity distributions, outputs them to text files.
   The I_example2.png is the visualization result of the intensity distributions, 
   created by using Gnuplot script gscritp.plt (converted eps to png by using ImageMagick).  
   
4. type './example3.out'.  
   This executable calculates instantaneous value of the sound pressure and particle velocity, outputs them to png image files.
   The image files are output to the folder "images" that is automatically created at runtime.
   Each image file has a name that indicates the cross section, field component, and number of time steps (ex. xz_p_014.png).
   The color bar is output as color_bar.png in the same folder.
   The range of color bar in each cross section is output to the info.txt file (xy_info.txt for z=0 plane).
   The xz_p.gif and the xy_p.gif are animated gifs that concatenate the png files created by using the shell script gif_animation.sh.   
   
Please see maw_src/multi_aw.h for detail of functions.
The main parts of the code is parallelized by using OpenMP. 
The number of threads is controlled by the environment variable OMP_NUM_THREADS.

![intensity distributions](I_example2.png "intensity distributions (I_example2.png)")  
![xz_p](xz_p.gif "instantaneous value of the p on y=0 plane (xz_p.gif)")![xy_p](xy_p.gif "instantaneous value of the p on z=0 plane (xy_p.gif)")  


## Analysis sample 1 (in the folder analysis_sample1)  

This is the analysis result of two focused beams cross at the origin. 
It is simply the placement of the wave sources and does not take into account the reflections between the transducers.

![intensity distributions 1](analysis_sample1/I_example2.png "intensity distributions (analysis_sample1/I_example2.png)")  

![xz_p 1](analysis_sample1/xz_p.gif "instantaneous value of the p on y=0 plane (analysis_sample1/xz_p.gif)")![yz_p 1](analysis_sample1/yz_p.gif "instantaneous value of the p on x=0 plane (analysis_sample1/yz_p.gif)")  
![xy_p 1](analysis_sample1/xy_p.gif "instantaneous value of the p on z=0 plane (analysis_sample1/xy_p.gif)")  


## References  

1. The command-line driven graphing utility [gnuplot](http://www.gnuplot.info/)  
2. The utilities for manipulating images [ImageMagick](https://imagemagick.org/)  
3. The official PNG reference library [libpng](http://www.libpng.org/pub/png/libpng.html)  
