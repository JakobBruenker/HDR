# HDR

A program that generates HDR images, given a set of photographs of a scene.

# Instructions

To run the program, you need a config.cfg file in the directory you are running
it from, which contains the relative or absolute path to a hdrgen file that
specifies which images should be used.

The program will then generate an image in the OpenEXR format and save it with
the same name as the hdrgen file, replacing the file ending with .exr.

It will also open several windows to show a tonemapped version of the HDR image
as well as 3 simulated exposures: 0.01 seconds, 0.05 seconds, and 0.33 seconds.
It will also show the recovered response curve; the required light to reach a
certain pixel value for a certain color increases to the right, and the pixel
value reached increases to the top.

To exit the program, simply close one of the windows it opens.

# Sources
  - M. A. Robertson, S. Borman, and R. L. Stevenson. *Dynamic Range
    Improvement Through Multiple Exposures.* In: Proceedings of ICIP, 1999.
  - F. Drago, K. Myszkowski, T. Annen and N. Chiba. *Adaptive Logarithmic For
    Displaying High Contrast Scenes.* In: Eurographics 22(3), 2003.
  - CIE XYZ conversion values are taken from:
    Wikipedia. *CIE 1931 color space.*
    https://en.wikipedia.org/wiki/CIE_XYZ#Construction_of_the_CIE_XYZ_color_space_from_the_Wright.E2.80.93Guild_data
    (accessed April 30, 2016).
