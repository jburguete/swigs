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
 * \file initial_transport.c
 * \brief Source file to define transported solute initial conditions structures
 *   and functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2015 Javier Burguete Tolosa.
 */
#include "initial_transport.h"

/**
 * \fn void initial_transport_print(InitialTransport *it, FILE *file)
 * \brief Function to print an InitialTransport structure on a file.
 * \param it
 * \brief InitialTransport structure.
 * \param file
 * \brief file.
 */
#if !INLINE_INITIAL_TRANSPORT_PRINT
void initial_transport_print(InitialTransport *it, FILE *file)
{_initial_transport_print(it, file);}
#endif

/**
 * \fn void initial_transport_error(char *m)
 * \brief Function to write an error message opening an InitialTransport
 *   structure.
 * \param m
 * \brief error message.
 */
#if !INLINE_INITIAL_TRANSPORT_ERROR
void initial_transport_error(char *m) {_initial_transport_error(m);}
#endif

/**
 * \fn void initial_transport_delete(InitialTransport *it)
 * \brief Function to delete an InitialTransport structure.
 * \param it
 * \brief InitialTransport structure.
 */
#if !INLINE_INITIAL_TRANSPORT_DELETE
void initial_transport_delete(InitialTransport *it)
{_initial_transport_delete(it);}
#endif

/**
 * \fn void initial_transport_init_empty(InitialTransport *it)
 * \brief Function to init an empty InitialTransport structure.
 * \param it
 * \brief InitialTransport structure.
 */
#if !INLINE_INITIAL_TRANSPORT_INIT_EMPTY
void initial_transport_init_empty(InitialTransport *it) {_initial_transport_init_empty(it);}
#endif

/**
 * \fn int initial_transport_copy(InitialTransport *it, \
 *   InitialTransport *it_copy)
 * \brief Function to copy two InitialTransport structures.
 * \param it
 * \brief InitialTransport structure copy.
 * \param it_copy
 * \brief InitialTransport structure copied.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_INITIAL_TRANSPORT_COPY
int initial_transport_copy(InitialTransport *it, InitialTransport *it_copy)
{return _initial_transport_copy(it, it_copy);}
#endif

/**
 * \fn int initial_transport_check(InitialTransport *it)
 * \brief Function to check if the points of the transported solute initial
 *   conditions are in correct order.
 * \param it
 * \brief InitialTransport structure.
 * \return 1 on succes, 0 on error. 
 */
#if !INLINE_INITIAL_TRANSPORT_CHECK
int initial_transport_check(InitialTransport *it)
{return _initial_transport_check(it);}
#endif

/**
 * \fn int initial_transport_open_xml(InitialTransport *it, xmlNode *node)
 * \brief Function to open an InitialTransport structure on a XML node.
 * \param it
 * \brief InitialTransport structure.
 * \param node
 * \brief XML node.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_INITIAL_TRANSPORT_OPEN_XML
int initial_transport_open_xml(InitialTransport *it, xmlNode *node)
{return _initial_transport_open_xml(it, node);}
#endif

/**
 * \fn void initial_transport_save_xml(InitialTransport *it, xmlNode *node)
 * \brief Function to save an InitialTransport structure on a XML node.
 * \param it
 * \brief InitialTransport structure.
 * \param node
 * \brief XML node.
 */
#if !INLINE_INITIAL_TRANSPORT_SAVE_XML
void initial_transport_save_xml(InitialTransport *it, xmlNode *node)
{_initial_transport_save_xml(it, node);}
#endif
