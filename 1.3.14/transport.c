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
 * \file transport.c
 * \brief Source file to define transported solute structures and functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2015 Javier Burguete Tolosa.
 */
#include "transport.h"

/**
 * \fn void transport_print(Transport *t, FILE *file)
 * \brief Function to print a Transport structure in a file.
 * \param t
 * \brief Transport structure.
 * \param file
 * \brief file.
 */
#if !INLINE_TRANSPORT_PRINT
void
transport_print (Transport * t, FILE * file)
{
  _transport_print (t, file);
}
#endif

/**
 * \fn void transport_error(Transport *t, char *m)
 * \brief Function to write an error message opening a Transport structure.
 * \param t
 * \brief Transport structure.
 * \param m
 * \brief error message.
 */
#if !INLINE_TRANSPORT_ERROR
void
transport_error (Transport * t, char *m)
{
  _transport_error (t, m);
}
#endif

/**
 * \fn void transport_delete(Transport *t)
 * \brief Function to delete a Transport structure.
 * \param t
 * \brief Transport structure.
 */
#if !INLINE_TRANSPORT_DELETE
void
transport_delete (Transport * t)
{
  _transport_delete (t);
}
#endif

/**
 * \fn void transport_init_empty(Transport *t)
 * \brief Function to init an empty Transport structure.
 * \param t
 * \brief Transport structure.
 */
#if !INLINE_TRANSPORT_INIT_EMPTY
void
transport_init_empty (Transport * t)
{
  _transport_init_empty (t);
}
#endif

/**
 * \fn int transport_create(Transport *t, char *name)
 * \brief Function to create a Transport structure.
 * \param t
 * \brief Transport structure.
 * \param name
 * \brief name.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_TRANSPORT_CREATE
int
transport_create (Transport * t, char *name)
{
  return _transport_create (t, name);
}
#endif

/**
 * \fn int transport_copy(Transport *t, Transport *t_copy)
 * \brief Function to copy two Transport structures.
 * \param t
 * \brief Transport structure copy.
 * \param t_copy
 * \brief Transport structure copied.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_TRANSPORT_COPY
int
transport_copy (Transport * t, Transport * t_copy)
{
  return _transport_copy (t, t_copy);
}
#endif

/**
 * \fn int transport_open_xml(Transport *t, xmlNode *node)
 * \brief Function to open a Transport structure on a XML node.
 * \param t
 * \brief Transport structure.
 * \param node
 * \brief XML node.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_TRANSPORT_OPEN_XML
int
transport_open_xml (Transport * t, xmlNode * node)
{
  return _transport_open_xml (t, node);
}
#endif

/**
 * \fn void transport_save_xml(Transport *t, xmlNode *node)
 * \brief Function to save a Transport structure on a XML node.
 * \param t
 * \brief Transport structure.
 * \param node
 * \brief XML node.
 */
#if !INLINE_TRANSPORT_SAVE_XML
void
transport_save_xml (Transport * t, xmlNode * node)
{
  _transport_save_xml (t, node);
}
#endif
