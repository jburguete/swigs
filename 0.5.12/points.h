/*
SWIGS (Shallow Water in Irregular Geometries Simulator): a software to simulate
transient or steady flows with solute transport in channels, channel networks
and rivers.

Copyright 2005-2014 Javier Burguete Tolosa.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice,
		this list of conditions and the following disclaimer.

	2. Redistributions in binary form must reproduce the above copyright notice,
		this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY Javier Burguete Tolosa ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL Javier Burguete Tolosa OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * \file points.h
 * \brief Header file to define point structures and functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2014 Javier Burguete Tolosa.
 */
#ifndef POINTS__H
#define POINTS__H 1

#include "def.h"
#include "jb/jb_win.h"
#include "jb/jb_xml.h"

/**
 * \struct Point2
 * \brief Structure to define a 2D point.
 */
typedef struct
{
/**
 * \var x
 * \brief x-coordinate.
 * \var y
 * \brief y-coordinate.
 */
	JBFLOAT x, y;
} Point2;

/**
 * \struct Point3
 * \brief Structure to define a 3D point.
 */
typedef struct
{
/**
 * \var x
 * \brief x-coordinate.
 * \var y
 * \brief y-coordinate.
 * \var z
 * \brief z-coordinate.
 */
	JBFLOAT x, y, z;
} Point3;

/**
 * \struct SectionPoint2
 * \brief Structure to define a 2D cross section point.
 */
typedef struct
{
/**
 * \var y
 * \brief transversal coordinate.
 * \var z
 * \brief vertical coordinate.
 * \var r
 * \brief friction coefficient.
 * \var t
 * \brief friction type.
 */
	JBFLOAT y, z, r;
	unsigned int t;
} SectionPoint2;

/**
 * \struct SectionPoint3
 * \brief Structure to define a 3D cross section point.
 */
typedef struct
{
/**
 * \var x
 * \brief x-coordinate.
 * \var y
 * \brief y-coordinate.
 * \var z
 * \brief z-coordinate.
 * \var r
 * \brief friction coefficient.
 * \var t
 * \brief friction type.
 */
	JBFLOAT x, y, z, r;
	unsigned int t;
} SectionPoint3;

static inline void _point2_print(Point2 *p, FILE *file)
{
	fprintf(file, "Point 2: x = "FWF" y = "FWF"\n", p->x, p->y);
}

#if INLINE_POINT2_PRINT
	#define point2_print _point2_print
#else
	void point2_print(Point2*, FILE*);
#endif

static inline void _point2_copy(Point2 *p, Point2 *p_copy)
{
	#if DEBUG_POINT2_COPY
		fprintf(stderr, "point_2_copy: start\n");
	#endif
	memcpy(p, p_copy, sizeof(Point2));
	#if DEBUG_POINT2_COPY
		fprintf(stderr, "point2_copy: end\n");
	#endif
}

#if INLINE_POINT2_COPY
	#define point2_copy _point2_copy
#else
	void point2_copy(Point2*, Point2*);
#endif

static inline void _point2_open(Point2 *p, JBDOUBLE x, JBDOUBLE y)
{
	#if DEBUG_POINT2_OPEN
		fprintf(stderr, "point2_open: start\n");
	#endif
	p->x = x, p->y = y;
	#if DEBUG_POINT2_OPEN
		fprintf(stderr, "point2_open: end\n");
	#endif
}

#if INLINE_POINT2_OPEN
	#define point2_open _point2_open
#else
	void point2_open(Point2*, JBDOUBLE, JBDOUBLE);
#endif

static inline void _point3_print(Point3 *p, FILE *file)
{
	fprintf(file, "Point 3: x = "FWF" y = "FWF" z = "FWF"\n", p->x, p->y, p->z);
}

#if INLINE_POINT3_PRINT
	#define point3_print _point3_print
#else
	void point3_print(Point3*, FILE*);
#endif

static inline void _point3_copy(Point3 *p, Point3 *p_copy)
{
	#if DEBUG_POINT3_COPY
		fprintf(stderr, "point3_copy: start\n");
	#endif
	memcpy(p, p_copy, sizeof(Point3));
	#if DEBUG_POINT3_COPY
		fprintf(stderr, "point3_copy: end\n");
	#endif
}

#if INLINE_POINT3_COPY
	#define point3_copy _point3_copy
#else
	void point3_copy(Point3*, Point3*);
#endif

static inline void _point3_open(Point3 *p, JBDOUBLE x, JBDOUBLE y, JBDOUBLE z)
{
	#if DEBUG_POINT3_OPEN
		fprintf(stderr, "point3_open: start\n");
	#endif
	p->x = x, p->y = y, p->z = z;
	#if DEBUG_POINT3_OPEN
		fprintf(stderr, "point3_open: end\n");
	#endif
}

#if INLINE_POINT3_OPEN
	#define point3_open _point3_open
#else
	void point3_open(Point3*, JBDOUBLE, JBDOUBLE, JBDOUBLE);
#endif

static inline void _section_point2_print(SectionPoint2 *sp, FILE *file)
{
	fprintf(file, "Section point: y = "FWF" z = "FWF" r = "FWF" t = %u\n", 
		sp->y, sp->z, sp->r, sp->t);
}

#if INLINE_SECTION_POINT2_PRINT
	#define section_point2_print _section_point2_print
#else
	void section_point2_print(SectionPoint2*, FILE*);
#endif

static inline void _section_point2_copy
	(SectionPoint2 *sp, SectionPoint2 *sp_copy)
{
	#if DEBUG_SECTION_POINT2_COPY
		fprintf(stderr, "section_point2_copy: start\n");
	#endif
	memcpy(sp, sp_copy, sizeof(SectionPoint2));
	#if DEBUG_SECTION_POINT2_COPY
		fprintf(stderr, "section_point2_copy: end\n");
	#endif
}

#if INLINE_SECTION_POINT2_COPY
	#define section_point2_copy _section_point2_copy
#else
	void section_point2_copy(SectionPoint2*, SectionPoint2*);
#endif

static inline void _section_point2_open
	(SectionPoint2 *sp, JBDOUBLE y, JBDOUBLE z, JBDOUBLE r, unsigned int t)
{
	#if DEBUG_SECTION_POINT2_OPEN
		fprintf(stderr, "section_point2_open: start\n");
	#endif
	sp->y = y; sp->z = z; sp->r = r; sp->t = t;
	#if DEBUG_SECTION_POINT2_OPEN
		fprintf(stderr, "section_point2_open: end\n");
	#endif
}

#if INLINE_SECTION_POINT2_OPEN
	#define section_point2_open _section_point2_open
#else
	void section_point2_open(SectionPoint2*, JBDOUBLE, JBDOUBLE, JBDOUBLE, int);
#endif

static inline void _section_point3_print(SectionPoint3 *sp, FILE *file)
{
	fprintf(file,
		"Section point 2: x = "FWF" y = "FWF" z = "FWF" r = "FWF" t = %u\n", 
		sp->x, sp->y, sp->z, sp->r, sp->t);
}

#if INLINE_SECTION_POINT3_PRINT
	#define section_point3_print _section_point3_print
#else
	void section_point3_print(SectionPoint3*, FILE*);
#endif

static inline void
	_section_point3_copy(SectionPoint3 *sp, SectionPoint3 *sp_copy)
{
	#if DEBUG_SECTION_POINT3_COPY
		fprintf(stderr, "section_point3_copy: start\n");
	#endif
	memcpy(sp, sp_copy, sizeof(SectionPoint3));
	#if DEBUG_SECTION_POINT3_COPY
		fprintf(stderr, "section_point3_copy: end\n");
	#endif
}

#if INLINE_SECTION_POINT3_COPY
	#define section_point3_copy _section_point3_copy
#else
	void section_point3_copy(SectionPoint3*, SectionPoint3*);
#endif

static inline void _section_point3_open
	(SectionPoint3 *sp, JBDOUBLE x, JBDOUBLE y, JBDOUBLE z, JBDOUBLE r, int t)
{
	#if DEBUG_SECTION_POINT3_OPEN
		fprintf(stderr, "section_point3_open: start\n");
	#endif
	sp->x = x; sp->y = y; sp->z = z; sp->r = r; sp->t = t;
	#if DEBUG_SECTION_POINT3_OPEN
		fprintf(stderr, "section_point3_open: end\n");
	#endif
}

#if INLINE_SECTION_POINT3_OPEN
	#define section_point3_open _section_point3_open
#else
	void section_point3_open(SectionPoint3*, JBDOUBLE, JBDOUBLE, JBDOUBLE,
		JBDOUBLE, unsigned int);
#endif

static inline void _section_point2_transform(SectionPoint2 *sp,
	SectionPoint3 *sp2, JBDOUBLE x, JBDOUBLE y, JBDOUBLE angle)
{
	register JBDOUBLE s;
	s = (sp2->x - x) * cosl(angle) + (sp2->y - y) * sinl(angle);
	sp->y = jbm_v2_length(sp2->x, sp2->y, x, y);
	if (s < 0.) sp->y *=  -1.;
	sp->z = sp2->z;
	sp->r = sp2->r;
	sp->t = sp2->t;
}

#if INLINE_SECTION_POINT2_TRANSFORM
	#define section_point2_transform _section_point2_transform
#else
	void section_point2_transform
		(SectionPoint2*, SectionPoint3*, JBDOUBLE, JBDOUBLE, JBDOUBLE);
#endif

#endif
