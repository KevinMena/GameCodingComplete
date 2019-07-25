/* Include this file instead of including <Windows.h> directly. */


#ifdef NOMINMAX
#define MINMAX_HPP 0 
#else // !NOMINMAX
#define MINMAX_HPP 1 
#define NOMINMAX 1
#endif // NOMINMAX

#ifdef STRICT
#define STRICT_HPP 0 
#else // !STRICT 
#define STRICT_HPP 1 
#define STRICT 1
#endif // STRICT

#include <Windows.h>

#if MINMAX_HPP
#undef NOMINMAX
#endif // MINMAX_HPP

#if STRICT_HPP
#undef STRICT
#endif // MINMAX_HPP

#undef MINMAX_HPP
#undef STRICT_HPP