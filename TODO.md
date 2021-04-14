#Features in progress
* Handle saturating arithmetic
* Handle arbitrary sized fixed-point numbers (not equal to storage-type dimensions)
* Add trigonometric functions (sin, cos, sinh, cosh, asin, acos, asinh, acosh, tan, atan, atan2)

#Features to be added
* Add fixed-point type meta-functions (type from range/resolution, etc.)
* Use expression templates to optimize computations
* Add ability to calculate optimal calculation type automatically and remove from template arguments
* Add ability to use multi-precision (at least 128-bit) calculation type
* Implement "fast" versions of elementary functions use IAMB_FAST_MATH preprocessor define
