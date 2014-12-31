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
 * \file junction_data.c
 * \brief Source file to define junction structures and functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2014 Javier Burguete Tolosa.
 */
#include "junction_data.h"

/**
 * \fn void junction_data_print(JunctionData *data, FILE *file)
 * \brief Function to print a JunctionData structure on a file.
 * \param data
 * \brief JunctionData structure.
 * \param file
 * \brief file.
 */
#if !INLINE_JUNCTION_DATA_PRINT
void junction_data_print(JunctionData *data, FILE *file)
{_junction_data_print(data, file);}
#endif

/**
 * \fn void junction_data_error(char *m)
 * \brief Function to write an error message opening a JunctionData structure.
 * \param m
 * \brief error message.
 */
#if !INLINE_JUNCTION_DATA_ERROR
void junction_data_error(char *m) {_junction_data_error(m);}
#endif

/**
 * \fn void junction_data_delete(JunctionData *data)
 * \brief Function to delete a JunctionData structure.
 * \param data
 * \brief JunctionData structure.
 */
#if !INLINE_JUNCTION_DATA_DELETE
void junction_data_delete(JunctionData *data) {_junction_data_delete(data);}
#endif

/**
 * \fn void junction_data_init_empty(JunctionData *data)
 * \brief Function to init an empty JunctionData structure.
 * \param data
 * \brief JunctionData structure.
 */
#if !INLINE_JUNCTION_DATA_INIT_EMPTY
void junction_data_init_empty(JunctionData *data)
{_junction_data_init_empty(data);}
#endif

/**
 * \fn int junction_data_copy(JunctionData *data, JunctionData *data_copy)
 * \brief Function to copy two JunctionData structures.
 * \param data
 * \brief JunctionData structure copy.
 * \param data_copy
 * \brief JunctionData structure copied.
 * \return 1 on success, 0 on error.
 */
#if !INLINE_JUNCTION_DATA_COPY
int junction_data_copy(JunctionData *data, JunctionData *data_copy)
{return _junction_data_copy(data, data_copy);}
#endif

/**
 * \fn int junction_data_open_xml(JunctionData *data, xmlNode *node)
 * \brief Function to open a JunctionData structure on a XML node.
 * \param data
 * \brief JunctionData structure.
 * \param node
 * \brief XML node.
 * \return 1 on succes, 0 on error.
 */
#if !INLINE_JUNCTION_DATA_OPEN_XML
int junction_data_open_xml(JunctionData *data, xmlNode *node)
{return _junction_data_open_xml(data, node);}
#endif

/**
 * \fn void junction_data_save_xml(JunctionData *data, xmlNode *node)
 * \brief Function to save a JunctionData structure on a XML node.
 * \param data
 * \brief JunctionData structure.
 * \param node
 * \brief XML node.
 */
#if !INLINE_JUNCTION_DATA_SAVE_XML
void junction_data_save_xml(JunctionData *data, xmlNode *node)
{_junction_data_save_xml(data, node);}
#endif
