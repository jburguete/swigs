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
 * \file channel.c
 * \brief Source file to define channel structures and methods.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005 - 2014 Javier Burguete Tolosa.
 */
#include "channel.h"

/**
 * \fn void channel_print(Channel *c, FILE *file)
 * \brief Method to print a Channel structure in a file.
 * \param c
 * \brief Channel structure.
 * \param file
 * \brief file.
 */
#if !INLINE_CHANNEL_PRINT
void channel_print(Channel *c, FILE *file) {_channel_print(c, file);}
#endif

/**
 * \fn void channel_error(Channel *c, char *m)
 * \brief Method to write an error message opening a Channel structure.
 * \param c
 * \brief Channel structure.
 * \param m
 * \brief error message.
 */
#if !INLINE_CHANNEL_ERROR
void channel_error(Channel *c, char *m) {_channel_error(c, m);}
#endif

/**
 * \fn void channel_delete(Channel *c)
 * \brief Method to delete a Channel structure.
 * \param c
 * \brief Channel structure.
 */
#if !INLINE_CHANNEL_DELETE
void channel_delete(Channel *c) {_channel_delete(c);}
#endif

/**
 * \fn void channel_init_empty(Channel *c)
 * \brief Method to init an empty Channel structure.
 * \param c
 * \brief Channel structure.
 */
#if !INLINE_CHANNEL_INIT_EMPTY
void channel_init_empty(Channel *c) {_channel_init_empty(c);}
#endif

#if !INLINE_CHANNEL_INSERT_BF
int channel_insert_bf(Channel *channel, int i)
{return _channel_insert_bf(channel, i);}
#endif

#if !INLINE_CHANNEL_REMOVE_BF
void channel_remove_bf(Channel *channel, int i)
{_channel_remove_bf(channel, i);}
#endif

#if !INLINE_CHANNEL_INSERT_CT
int channel_insert_ct(Channel *channel, int i)
{return _channel_insert_ct(channel, i);}
#endif

#if !INLINE_CHANNEL_REMOVE_CT
void channel_remove_ct(Channel *channel, int i)
{_channel_remove_ct(channel, i);}
#endif

#if !INLINE_CHANNEL_CREATE
int channel_create(Channel *c, char *name)
{return _channel_create(c, name);}
#endif

#if !INLINE_CHANNEL_COPY
int channel_copy(Channel *c, Channel *c_copy) {return _channel_copy(c, c_copy);}
#endif

#if !INLINE_CHANNEL_BOUNDARY_TEST
int channel_boundary_test(Channel *c) {return _channel_boundary_test(c);}
#endif

#if !INLINE_CHANNEL_PARSE_NODE
int channel_parse_node(Channel *c, xmlNode *node, int *hasof)
{return _channel_parse_node(c, node, hasof);}
#endif

#if !INLINE_CHANNEL_OPEN_XML
int channel_open_xml(Channel *c, int nt, xmlNode *node)
{return _channel_open_xml(c, nt, node);}
#endif

#if !INLINE_CHANNEL_SAVE_XML
void channel_save_xml(Channel *c, xmlNode *node) {_channel_save_xml(c, node);}
#endif

