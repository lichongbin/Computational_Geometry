#ifndef _CONST_H_
#define _CONST_H_

#pragma once

#define PLATFORM_SIZE   1000     // unit: mm
#define EPS             0.000000001
#define PI              3.14159265f

#define ZOOM_STEP       0.01
#define ZOOM_MIN        0.001
#define ZOOM_MAX        100

#define DOT_SIZE        4	        // unit: mm

#define MIN(a, b)       ( (a) > (b) ? (b) : (a) )
#define MAX(a, b)       ( (a) > (b) ? (a) : (b) )

#define EQUAL(a, b)     ( fabs((a) - (b)) < EPS )
#define LEQUAL(a, b)    ( (a) - (b) < EPS       )
#define LESS(a, b)      ( (a) - (b) <= -EPS     )
#define GREATER(a, b)	( (a) - (b) >= EPS      )
#define GEQUAL(a, b)	( (a) - (b) > -EPS      )
#define UNEQUAL(a, b)	( fabs((a) - (b)) >= EPS)

#endif // _CONST_H_