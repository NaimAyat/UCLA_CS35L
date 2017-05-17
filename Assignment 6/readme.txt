Naim Ayat
CS 35L Assignment 6
Homework: Multithreaded performance
April 13, 2017

This is a brief report discussing my multithreaded implementation of SRT.

As per the spec, the only file which required modification was main.c.
Thus, I looked through the code to find four nested four loops which
served to iterate through an image's pixels. This system of loops is 
suboptimal on its own; I had to find a way to divide it into multiple 
threads. 

My implementation of SRT contains multiple threads which each generate 
a vertical segment of an image. Upon termination of the program, these 
segments are merged into a single output image.

I created a function to be called by pthread_create and put the nested 
loops mentioned above inside it. Of course, this yeilded some errors 
regarding the the new function's access to certain main() variables, so 
I made them global.

Then, I created an array for storing pixels of the image so that they
could be properly reordered after being split into separate threads.
Therefore, each thread outputs its pixels to their corresponding location
in the array. When all the threads have finished processing, the program
outputs the array. 

As in the lab, I ran four tests of 1, 2, 4, and 8 threads, respectively.

______________________________________________________________________________

One thread:
	time ./srt 1-test.ppm >1-test.ppm.tmp
Output:
	real	0m49.151s
	user	0m49.221s
	sys	0m0.001s


Two threads: 
	time ./srt 2-test.ppm >2-test.ppm.tmp
Output:
	real	0m24.412s
	user	0m48.678s
	sys	0m0.002s


Four threads:
	time ./srt 4-test.ppm >4-test.ppm.tmp
Output:
	real	0m14.500s
	user	0m48.117s
	sys	0m0.005s


Eight threads:
	time ./srt 8-test.ppm >8-test.ppm.tmp
Output:
	real	0m10.005s
	user	0m49.299s
	sys	0m0.004s
______________________________________________________________________________


Again, we see that increasing the number of threads drastically improves
the real-time performance of SRT. The duration of real-time processing is 
almost halved by each test of increasing thread count. Once more, as in 
the lab, user- and system-times remain relatively constant.