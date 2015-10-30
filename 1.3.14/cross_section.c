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
 * \file cross_section.c
 * \brief Source file to define cross section structures and functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2015 Javier Burguete Tolosa.
 */
#include "cross_section.h"

/**
 * \fn void cross_section_print(CrossSection *cs, FILE *file)
 * \brief Function to print a CrossSection structure on a file.
 * \param cs
 * \brief CrossSection structure.
 * \param file
 * \brief file.
 */
#if !INLINE_CROSS_SECTION_PRINT
void
cross_section_print (CrossSection * cs, FILE * file)
{
  _cross_section_print (cs, file);
}
#endif

/**
 * \fn void cross_section_error(CrossSection *cs, char *m)
 * \brief Function to write an error message opening a CrossSection structure.
 * \param cs
 * \brief CrossSection structure.
 * \param m
 * \brief error message.
 */
#if !INLINE_CROSS_SECTION_ERROR
void
cross_section_error (CrossSection * cs, char *m)
{
  _cross_section_error (cs, m);
}
#endif

/**
 * \fn void cross_section_delete(CrossSection *cs)
 * \brief Function to delete a CrossSection structure.
 * \param cs
 * \brief CrossSection structure.
 */
#if !INLINE_CROSS_SECTION_DELETE
void
cross_section_delete (CrossSection * cs)
{
  _cross_section_delete (cs);
}
#endif

/**
 * \fn void cross_section_init_empty(CrossSection *cs)
 * \brief Function to init an empty CrossSection structure.
 * \param cs
 * \brief CrossSection structure.
 */
#if !INLINE_CROSS_SECTION_INIT_EMPTY
void
cross_section_init_empty (CrossSection * cs)
{
  _cross_section_init_empty (cs);
}
#endif

/**
 * \fn int cross_section_create(CrossSection *cs, int n, char *name)
 * \brief Function to create a CrossSection structure.
 * \param cs
 * \brief CrossSection structure.
 * \param n
 * \brief points number.
 * \param name
 * \brief name.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_CROSS_SECTION_CREATE
int
cross_section_create (CrossSection * cs, int n, char *name)
{
  return _cross_section_create (cs, n, name);
}
#endif

/**
 * \fn int cross_section_insert_transient(CrossSection *cs, int position)
 * \brief Function to insert a TransientSection in a CrossSection structure.
 * \param cs
 * \brief CrossSection structure.
 * \param position
 * \brief insertion position.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_CROSS_SECTION_INSERT_TRANSIENT
int
cross_section_insert_transient (CrossSection * cs, int position)
{
  return _cross_section_insert_transient (cs, position);
}
#endif

/**
 * \fn int cross_section_remove_transient(CrossSection *cs, int position)
 * \brief Function to remove a TransientSection in a CrossSection structure.
 * \param cs
 * \brief CrossSection structure.
 * \param position
 * \brief deletion position.
 */
#if !INLINE_CROSS_SECTION_REMOVE_TRANSIENT
void
cross_section_remove_transient (CrossSection * cs, int position)
{
  _cross_section_remove_transient (cs, position);
}
#endif

/**
 * \fn int cross_section_copy(CrossSection *cs, CrossSection *cs_copy)
 * \brief Function to copy two CrossSection structures.
 * \param cs
 * \brief CrossSection structure copy.
 * \param cs_copy
 * \brief CrossSection structure copied.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_CROSS_SECTION_COPY
int
cross_section_copy (CrossSection * cs, CrossSection * cs_copy)
{
  return _cross_section_copy (cs, cs_copy);
}
#endif

/**
 * \fn int cross_section_control_channel_set_up(CrossSection *cs, char **name, \
 *   unsigned int n)
 * \brief Function to set up the channel of a control cross section.
 * \param cs
 * \brief ControlSection structure.
 * \param name
 * \brief array of channel names.
 * \param n
 * \brief number of channel names.
 * \return 1 on succes, 0 on error.
 */
#if !INLINE_CROSS_SECTION_CONTROL_CHANNEL_SET_UP
int
cross_section_control_channel_set_up (CrossSection * cs, char **name, int n)
{
  return _cross_section_control_channel_set_up (cs, name, n);
}
#endif

/**
 * \fn int cross_section_control_section_set_up(CrossSection *cs, char **name, \
 *   unsigned int n)
 * \brief Function to set up the cross section of a control cross section.
 * \param cs
 * \brief ControlSection structure.
 * \param name
 * \brief array of cross section names.
 * \param n
 * \brief number of cross section names.
 * \return 1 on succes, 0 on error.
 */
#if !INLINE_CROSS_SECTION_CONTROL_SECTION_SET_UP
int cross_section_control_section_set_up
  (CrossSection * cs, char **name, unsigned int n)
{
  return _cross_section_control_section_set_up (cs, name, n);
}
#endif

/**
 * \fn int cross_section_check(CrossSection *cs)
 * \brief Function to check if the TransientSections are in correct order.
 * \return 1 on succes, 0 on error. 
 */
#if !INLINE_CROSS_SECTION_CHECK
int
cross_section_check (CrossSection * cs)
{
  return _cross_section_check (cs);
}
#endif

/**
 * \fn int cross_section_open_xml(CrossSection *cs, xmlNode *node)
 * \brief Function to open a CrossSection structure on a XML node.
 * \param cs
 * \brief CrossSection structure.
 * \param node
 * \brief XML node.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_CROSS_SECTION_OPEN_XML
int
cross_section_open_xml (CrossSection * cs, xmlNode * node)
{
  return _cross_section_open_xml (cs, node);
}
#endif

/**
 * \fn void cross_section_save_xml(CrossSection *cs, xmlNode *node)
 * \brief Function to save a CrossSection structure on a XML node.
 * \param cs
 * \brief CrossSection structure.
 * \param node
 * \brief XML node.
 */
#if !INLINE_CROSS_SECTION_SAVE_XML
void
cross_section_save_xml (CrossSection * cs, xmlNode * node)
{
  _cross_section_save_xml (cs, node);
}
#endif

/**
 * \fn TransientSection* cross_section_get_transient(CrossSection *cs, \
 *   JBDOUBLE t)
 * \brief Function to get a TransientSection structure from a CrossSection
 *   structure at a time.
 * \param cs
 * \brief CrossSection structure.
 * \param t
 * \brief time.
 * \return TransientSection structure.
 */
#if !INLINE_CROSS_SECTION_GET_TRANSIENT
TransientSection *
cross_section_get_transient (CrossSection * cs, JBDOUBLE t)
{
  return _cross_section_get_transient (cs, t);
}
#endif
