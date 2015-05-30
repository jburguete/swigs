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
 * \file channel.c
 * \brief Source file to define channel structures and functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2015 Javier Burguete Tolosa.
 */
#include "channel.h"

/**
 * \fn void channel_print(Channel *c, FILE *file)
 * \brief Function to print a Channel structure in a file.
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
 * \brief Function to write an error message opening a Channel structure.
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
 * \brief Function to delete a Channel structure.
 * \param c
 * \brief Channel structure.
 */
#if !INLINE_CHANNEL_DELETE
void channel_delete(Channel *c) {_channel_delete(c);}
#endif

/**
 * \fn void channel_init_empty(Channel *c)
 * \brief Function to init an empty Channel structure.
 * \param c
 * \brief Channel structure.
 */
#if !INLINE_CHANNEL_INIT_EMPTY
void channel_init_empty(Channel *c) {_channel_init_empty(c);}
#endif

/**
 * \fn int channel_insert_bf(Channel *c, int i)
 * \brief Function to insert a flow boundary condition in a channel.
 * \param c
 * \brief Channel structure.
 * \param i
 * \brief insertion position.
 * \return 1 on success, 0 on error.
 */
#if !INLINE_CHANNEL_INSERT_BF
int channel_insert_bf(Channel *c, int i) {return _channel_insert_bf(c, i);}
#endif

/**
 * \fn void channel_remove_bf(Channel *c, int i)
 * \brief Function to remove a flow boundary condition in a channel.
 * \param c
 * \brief Channel structure.
 * \param i
 * \brief deletion position.
 */
#if !INLINE_CHANNEL_REMOVE_BF
void channel_remove_bf(Channel *c, int i) {_channel_remove_bf(c, i);}
#endif

/**
 * \fn int channel_insert_ct(Channel *c, int i)
 * \brief Function to insert a transported solute in a channel.
 * \param c
 * \brief Channel structure.
 * \param i
 * \brief insertion position.
 */
#if !INLINE_CHANNEL_INSERT_CT
int channel_insert_ct(Channel *c, int i) {return _channel_insert_ct(c, i);}
#endif

/**
 * \fn void channel_remove_ct(Channel *c, int i)
 * \brief Function to remove a transported solute in a channel.
 * \param c
 * \brief Channel structure.
 * \param i
 * \brief deletion position.
 */
#if !INLINE_CHANNEL_REMOVE_CT
void channel_remove_ct(Channel *c, int i) {_channel_remove_ct(c, i);}
#endif

/**
 * \fn int channel_create(Channel *c, char *name)
 * \brief Function to create a Channel structure.
 * \param c
 * \brief Channel structure.
 * \param name
 * \brief name.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_CHANNEL_CREATE
int channel_create(Channel *c, char *name) {return _channel_create(c, name);}
#endif

/**
 * \fn int channel_copy(Channel *c, Channel *c_copy)
 * \brief Function to copy two Channel structures.
 * \param c
 * \brief Channel structure copy.
 * \param c_copy
 * \brief Channel structure copied.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_CHANNEL_COPY
int channel_copy(Channel *c, Channel *c_copy) {return _channel_copy(c, c_copy);}
#endif

/**
 * \fn int channel_parse_node(Channel *c, xmlNode *node, int *hasof)
 * \brief Function to parse a XML child node defining a channel.
 * \param c
 * \brief Channel structure.
 * \param node
 * \brief XML child node.
 * \param hasof
 * \brief 1 if flow outlet condition is defined, 0 otherwise.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_CHANNEL_PARSE_NODE
int channel_parse_node(Channel *c, xmlNode *node, int *hasof)
{return _channel_parse_node(c, node, hasof);}
#endif

/**
 * \fn int channel_flow_boundaries_set_up(Channel *c)
 * \brief Function to set up the flow boundary conditions of a channel.
 * \param c
 * \brief Channel structure.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_CHANNEL_FLOW_BOUNDARIES_SET_UP
int channel_flow_boundaries_set_up(Channel *c)
{return _channel_flow_boundaries_set_up(c);}
#endif

/**
 * \fn int channel_transport_boundaries_set_up(Channel *c)
 * \brief Function to set up the transported solute boundary conditions of a
 *   channel.
 * \param c
 * \brief Channel structure.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_CHANNEL_TRANSPORT_BOUNDARIES_SET_UP
int channel_transport_boundaries_set_up(Channel *c)
{return _channel_transport_boundaries_set_up(c);}
#endif

/**
 * \fn int channel_open_xml(Channel *c, int nt, xmlNode *node)
 * \brief Function to open a Channel structure on a XML node.
 * \param c
 * \brief Channel structure.
 * \param nt
 * \brief number of transported solutes.
 * \param node
 * \brief XML node.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_CHANNEL_OPEN_XML
int channel_open_xml(Channel *c, int nt, xmlNode *node)
{return _channel_open_xml(c, nt, node);}
#endif

/**
 * \fn void channel_save_xml(Channel *c, xmlNode *node)
 * \brief Function to save a Channel structure on a XML node.
 * \param c
 * \brief Channel structure.
 * \param node
 * \brief XML node.
 */
#if !INLINE_CHANNEL_SAVE_XML
void channel_save_xml(Channel *c, xmlNode *node) {_channel_save_xml(c, node);}
#endif
