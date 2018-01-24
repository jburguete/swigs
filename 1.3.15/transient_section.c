/*
SWIGS (Shallow Water in Irregular Geometries Simulator): a software to simulate
transient or steady flows with solute transport in channels, channel networks
and rivers.

Copyright 2005-2015 Javier Burguete Tolosa.

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
 * \file transient_section.c
 * \brief Source file to define transient section functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2015 Javier Burguete Tolosa.
 */
#include "transient_section.h"

/**
 * \var friction_dz
 * \brief accuracy of the height in friction calculation.
 */
JBFLOAT friction_dz = FRICTION_DZ;

/**
 * \fn void transient_section_print(TransientSection *ts, FILE *file)
 * \brief Function to print a TransientSection structure on a file.
 * \param ts
 * \brief TransientSection structure.
 * \param file
 * \brief file.
 */
#if !INLINE_TRANSIENT_SECTION_PRINT
void
transient_section_print (TransientSection * ts, FILE * file)
{
  _transient_section_print (ts, file);
}
#endif

/**
 * \fn void transient_section_error(TransientSection *ts, char *m)
 * \brief Function to write an error message opening a TransientSection structure.
 * \param ts
 * \brief TransientSection structure.
 * \param m
 * \brief error message.
 */
#if !INLINE_TRANSIENT_SECTION_ERROR
void
transient_section_error (TransientSection * ts, char *m)
{
  _transient_section_error (ts, m);
}
#endif

/**
 * \fn void transient_section_delete(TransientSection *ts)
 * \brief Function to delete a TransientSection structure.
 * \param ts
 * \brief TransientSection structure.
 */
#if !INLINE_TRANSIENT_SECTION_DELETE
void
transient_section_delete (TransientSection * ts)
{
  _transient_section_delete (ts);
}
#endif

/**
 * \fn void transient_section_init_empty(TransientSection *ts)
 * \brief Function to init an empty TransientSection structure.
 * \param ts
 * \brief TransientSection structure.
 */
#if !INLINE_TRANSIENT_SECTION_INIT_EMPTY
void
transient_section_init_empty (TransientSection * ts)
{
  _transient_section_init_empty (ts);
}
#endif

/**
 * \fn int transient_section_create(TransientSection *ts, int n, int type, \
 *   char *name)
 * \brief Function to create a TransientSection structure.
 * \param ts
 * \brief TransientSection structure.
 * \param n
 * \brief points number.
 * \param type
 * \brief type
 * \param name
 * \brief name.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_TRANSIENT_SECTION_CREATE
int
transient_section_create (TransientSection * ts, int n, int type, char *name)
{
  return _transient_section_create (ts, n, type, name);
}
#endif

/**
 * \fn int transient_section_copy(TransientSection *ts, \
 *   TransientSection *ts_copy)
 * \brief Function to copy two TransientSection structures.
 * \param ts
 * \brief TransientSection structure copy.
 * \param ts_copy
 * \brief TransientSection structure copied.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_TRANSIENT_SECTION_COPY
int
transient_section_copy (TransientSection * ts, TransientSection * ts_copy)
{
  return _transient_section_copy (ts, ts_copy);
}
#endif

/**
 * \fn int transient_section_transform(TransientSection *ts, JBFLOAT x, \
 *   JBFLOAT y, JBFLOAT angle)
 * \brief Function to transform from a polilyne to a cross section.
 * \param ts
 * \brief TransientSection structure.
 * \param x
 * \brief x-coordinate of the axis point.
 * \param y
 * \brief y-coordinate of the axis point.
 * \param angle
 * \brief orientation angle of the transient section.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_TRANSIENT_SECTION_TRANSFORM
void transient_section_transform
  (TransientSection * ts, JBFLOAT x, JBFLOAT y, JBFLOAT angle)
{
  _transient_section_transform (ts, x, y, angle);
}
#endif

/**
 * \fn int transient_section_open_xml(TransientSection *ts, xmlNode *node, \
 *   JBFLOAT x, JBFLOAT y, JBFLOAT angle)
 * \brief Function to open a TransientSection structure on a XML node.
 * \param ts
 * \brief TransientSection structure.
 * \param node
 * \brief XML node.
 * \param x
 * \brief x-coordinate of the axis point.
 * \param y
 * \brief y-coordinate of the axis point.
 * \param angle
 * \brief orientation angle of the transient section.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_TRANSIENT_SECTION_OPEN_XML
int transient_section_open_xml
  (TransientSection * ts, xmlNode * node, JBFLOAT x, JBFLOAT y, JBFLOAT angle)
{
  return _transient_section_open_xml (ts, node, x, y, angle);
}
#endif

/**
 * \fn void transient_section_save_xml(TransientSection *ts, xmlNode *node)
 * \brief Function to save a TransientSection structure on a XML node.
 * \param ts
 * \brief TransientSection structure.
 * \param node
 * \brief XML node.
 */
#if !INLINE_TRANSIENT_SECTION_SAVE_XML
void
transient_section_save_xml (TransientSection * ts, xmlNode * node)
{
  _transient_section_save_xml (ts, node);
}
#endif
