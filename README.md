# Iamb
Iamb was created primarily with an eye toward implementing flight control systems on ARM Cortex M0, M0+, and M3 microcontrollers.  These particular processors are able to process 32-bit and 64-bit integer values rapidly, but do not possess a floating-point unit that would make the use of float or double efficient.  Even so, Iamb is designed for accuracy rather than absolute speed.  The calculations are done in a 64-bit space to avoid overflow and the elementary functions (trigonometric and transcendental) use implementations that lead to maximum accuracy.

Construction of fixed-point numbers is supported from integers, floating-point numbers, and as the result of integer divisions.  The first two methods are self-explanatory.  The integer division construction, however, is implemented so as to avoid any floating-point operations and still avoid the errors inherent in integer division.  The goal of this construction method is to provide compile-time constant construction as the entire operation can then be optimized out at compile-time.  Nevertheless, the method is available at run-time as well for minimal cost.
 
# Development
Iamb is currently in a functional but basic state.  General arithmetic operations are fully functional.  In addition, elementary functions such as log2/log10/ln, exp2/exp10/exp, and reciprocal are functional.  The only trigonometric function currently implemented is acos (though, additional trigonometric functions could easily be implemented).  Future development can follow three main paths.  First, the expansion of the implemented elementary functions (especially in the area of trigonometric functions) should be completed.  Secondly, Error tracking and handling should be added.  To this end, an error type has been created and various forms of overflow handling (saturation in addition to simple wrapping) is being considered.  Finally, rewriting the core to return an intermediate 64-bit value would allow for expression templates to be used in the core.  This could easily lead to code optimization as well as reduced rounding error in calculations.
