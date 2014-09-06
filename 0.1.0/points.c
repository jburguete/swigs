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
 * \file points.c
 * \brief Source file to define point methods.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2014 Javier Burguete Tolosa.
 */
#include "points.h"

/**
 * \fn void point2_print(Point2 *p, FILE *file)
 * \brief Method to print a Point2 structure on a file.
 * \param p
 * \brief Point2 structure.
 * \param file
 * \brief file.
 */
#if !INLINE_POINT2_PRINT
void point2_print(Point2 *p, FILE *file) {_point2_print(p, file);}
#endif

/**
 * \fn void point2_copy(Point2 *p, Point2 *p_copy)
 * \brief Method to copy two Point2 structures.
 * \param p
 * \brief Point2 structure copy.
 * \param p_copy
 * \brief Point2 structure copied.
 */
#if !INLINE_POINT2_COPY
void point2_copy(Point2 *p, Point2 *p_copy) {_point2_copy(p, p_copy);}
#endif

/**
 * \fn void point2_open(Point2 *p, JBDOUBLE x, JBDOUBLE y)
 * \brief Method to open a Point2 structure.
 * \param p
 * \brief Point2 structure.
 * \param x
 * \brief x-coordinate.
 * \param y
 * \brief y-coordinate.
 */
#if !INLINE_POINT2_OPEN
void point2_open(Point2 *p, JBDOUBLE x, JBDOUBLE y) {_point2_open(p, x, y);}
#endif

/**
 * \fn void point3_print(Point3 *p, FILE *file)
 * \brief Method to print a Point3 structure on a file.
 * \param p
 * \brief Point3 structure.
 * \param file
 * \brief file.
 */
#if !INLINE_POINT3_PRINT
void point3_print(Point3 *p, FILE *file) {_point3_print(p, file);}
#endif

/**
 * \fn void point3_copy(Point3 *p, Point3 *p_copy)
 * \brief Method to copy two Point3 structures.
 * \param p
 * \brief Point3 structure copy.
 * \param p_copy
 * \brief Point3 structure copied.
 */
#if !INLINE_POINT3_COPY
void point3_copy(Point3 *p, Point3 *p_copy) {_point3_copy(p, p_copy);}
#endif

/**
 * \fn void point3_open(Point3 *p, JBDOUBLE x, JBDOUBLE y, JBDOUBLE z)
 * \brief Method to open a Point3 structure.
 * \param p
 * \brief Point3 structure.
 * \param x
 * \brief x-coordinate.
 * \param y
 * \brief y-coordinate.
 * \param z
 * \brief z-coordinate.
 */
#if !INLINE_POINT3_OPEN
void point3_open(Point3 *p, JBDOUBLE x, JBDOUBLE y, JBDOUBLE z)
{_point3_open(p, x, y, z);}
#endif

/**
 * \fn void section_point2_print(SectionPoint2 *sp, FILE *file)
 * \brief Method to print a SectionPoint2 structure on a file.
 * \param sp
 * \brief SectionPoint2 structure.
 * \param file
 * \brief file.
 */
#if !INLINE_SECTION_POINT2_PRINT
void section_point2_print(SectionPoint2 *sp, FILE *file)
{_section_point2_print(sp, file);}
#endif

/**
 * \fn void section_point2_copy(SectionPoint2 *sp, SectionPoint2 *sp_copy)
 * \brief Method to copy two SectionPoint2 structures.
 * \param sp
 * \brief SectionPoint2 structure copy.
 * \param sp_copy
 * \brief SectionPoint2 structure copied.
 */
#if !INLINE_SECTION_POINT2_COPY
void section_point2_copy(SectionPoint2 *sp, SectionPoint2 *sp_copy)
{_section_point2_copy(sp, sp_copy);}
#endif

/**
 * \fn void section_point2_open(SectionPoint2 *sp, JBDOUBLE y, JBDOUBLE z, \
 *   JBDOUBLE r, unsigned int t)
 * \brief Method to open a Point2 structure.
 * \param sp
 * \brief SectionPoint2 structure.
 * \param y
 * \brief transversal coordinate.
 * \param z
 * \brief vertical coordinate.
 * \param r
 * \brief Friction coefficient.
 * \param t
 * \brief Friction type.
 */
#if !INLINE_SECTION_POINT2_OPEN
void section_point2_open
	(SectionPoint2 *sp, JBDOUBLE y, JBDOUBLE z, JBDOUBLE r, unsigned int t)
{_section_point2_open(sp, y, z, r, t);}
#endif

/**
 * \fn void section_point3_print(SectionPoint3 *sp, FILE *file)
 * \brief Method to print a SectionPoint3 structure on a file.
 * \param sp
 * \brief SectionPoint3 structure.
 * \param file
 * \brief file.
 */
#if !INLINE_SECTION_POINT3_PRINT
void section_point3_print(SectionPoint3 *sp, FILE *file)
{_section_point3_print(sp, file);}
#endif

/**
 * \fn void section_point3_copy(SectionPoint3 *sp, SectionPoint3 *sp_copy)
 * \brief Method to copy two SectionPoint3 structures.
 * \param sp
 * \brief SectionPoint3 structure copy.
 * \param sp_copy
 * \brief SectionPoint3 structure copied.
 */
#if !INLINE_SECTION_POINT3_COPY
void section_point3_copy(SectionPoint3 *sp, SectionPoint3 *sp_copy)
{_section_point3_copy(sp, sp_copy);}
#endif

/**
 * \fn void section_point3_open(SectionPoint3 *sp, JBDOUBLE x, JBDOUBLE y, \
 *   JBDOUBLE z, JBDOUBLE r, unsigned int t)
 * \brief Method to open a Point3 structure.
 * \param sp
 * \brief SectionPoint3 structure.
 * \param x
 * \brief x-coordinate.
 * \param y
 * \brief y-coordinate.
 * \param z
 * \brief z-coordinate.
 * \param r
 * \brief Friction coefficient.
 * \param t
 * \brief Friction type.
 */
#if !INLINE_SECTION_POINT3_OPEN
void section_point3_open(SectionPoint3 *sp, JBDOUBLE x, JBDOUBLE y, JBDOUBLE z,
	JBDOUBLE r, unsigned int t)
{_section_point3_open(sp, x, y, z, r, t);}
#endif

/**
 * \fn void section_point2_transform(SectionPoint2 *sp, SectionPoint3 *sp2, \
 *   JBDOUBLE x, JBDOUBLE y, JBDOUBLE angle)
 * \brief Method to transform a Point3 structure to a Point2 structure.
 * \param sp
 * \brief SectionPoint2 structure transformed.
 * \param sp2
 * \brief SectionPoint3 structure to transform.
 * \param x
 * \brief point x-coordinate.
 * \param y
 * \brief point y-coordinate.
 * \param angle
 * \brief angle.
 */
#if !INLINE_SECTION_POINT2_TRANSFORM
void section_point2_transform(SectionPoint2 *sp, SectionPoint3 *sp2, JBDOUBLE x,
	JBDOUBLE y, JBDOUBLE angle)
{_section_point2_transform(sp, sp2, x, y, angle);}
#endif
