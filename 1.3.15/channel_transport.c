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
 * \file channel_transport.c
 * \brief Source file to define channel transported solute structures and
 *   functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2015 Javier Burguete Tolosa.
 */
#include "channel_transport.h"

/**
 * \fn void channel_transport_print(ChannelTransport *ct, FILE *file)
 * \brief Function to print a ChannelTransport structure in a file.
 * \param ct
 * \brief ChannelTransport structure.
 * \param file
 * \brief file.
 */
#if !INLINE_CHANNEL_TRANSPORT_PRINT
void
channel_transport_print (ChannelTransport * ct, FILE * file)
{
  _channel_transport_print (ct, file);
}
#endif

/**
 * \fn void channel_transport_error(char *m)
 * \brief Function to write an error message opening a ChannelTransport
 *   structure.
 * \param m
 * \brief error message.
 */
#if !INLINE_CHANNEL_TRANSPORT_ERROR
void
channel_transport_error (char *m)
{
  _channel_transport_error (m);
}
#endif

/**
 * \fn void channel_transport_delete(ChannelTransport *ct)
 * \brief Function to delete a ChannelTransport structure.
 * \param ct
 * \brief ChannelTransport structure.
 */
#if !INLINE_CHANNEL_TRANSPORT_DELETE
void
channel_transport_delete (ChannelTransport * ct)
{
  _channel_transport_delete (ct);
}
#endif

/**
 * \fn void channel_transport_init_empty(ChannelTransport *ct)
 * \brief Function to init an empty ChannelTransport structure.
 * \param ct
 * \brief ChannelTransport structure.
 */
#if !INLINE_CHANNEL_TRANSPORT_INIT_EMPTY
void
channel_transport_init_empty (ChannelTransport * ct)
{
  _channel_transport_init_empty (ct);
}
#endif

/**
 * \fn int channel_transport_insert_bt(ChannelTransport *ct, int i)
 * \brief Function to insert a boundary codition in a ChannelTransport
 *   structure.
 * \param ct
 * \brief ChannelTransport structure.
 * \param i
 * \brief insertion position.
 * \return 1 on success, 0 on error.
 */
#if !INLINE_CHANNEL_TRANSPORT_INSERT_BT
int
channel_transport_insert_bt (ChannelTransport * ct, int i)
{
  return _channel_transport_insert_bt (ct, i);
}
#endif

/**
 * \fn void channel_transport_remove_bt(ChannelTransport *ct, int i)
 * \brief Function to remove a boundary codition in a ChannelTransport
 *   structure.
 * \param ct
 * \brief ChannelTransport structure.
 * \param i
 * \brief deletion position.
 */
#if !INLINE_CHANNEL_TRANSPORT_REMOVE_BT
void
channel_transport_remove_bt (ChannelTransport * ct, int i)
{
  _channel_transport_remove_bt (ct, i);
}
#endif

/**
 * \fn int channel_transport_copy(ChannelTransport *ct, \
 *   ChannelTransport *ct_copy)
 * \brief Function to copy two ChannelTransport structures.
 * \param ct
 * \brief ChannelTransport structure copy.
 * \param ct_copy
 * \brief ChannelTransport structure copied.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_CHANNEL_TRANSPORT_COPY
int
channel_transport_copy (ChannelTransport * ct, ChannelTransport * ct_copy)
{
  return _channel_transport_copy (ct, ct_copy);
}
#endif

/**
 * \fn int channel_transport_parse_node(ChannelTransport *ct, xmlNode *node)
 * \brief Function to parse a XML child node defining a channel_transport.
 * \param ct
 * \brief ChannelTransport structure.
 * \param node
 * \brief XML child node.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_CHANNEL_TRANSPORT_PARSE_NODE
int
channel_transport_parse_node (ChannelTransport * ct, xmlNode * node)
{
  return _channel_transport_parse_node (ct, node);
}
#endif

/**
 * \fn int channel_transport_open_xml(ChannelTransport *ct, xmlNode *node)
 * \brief Function to open a ChannelTransport structure on a XML node.
 * \param ct
 * \brief ChannelTransport structure.
 * \param node
 * \brief XML node.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_CHANNEL_TRANSPORT_OPEN_XML
int
channel_transport_open_xml (ChannelTransport * ct, xmlNode * node)
{
  return _channel_transport_open_xml (ct, node);
}
#endif

/**
 * \fn void channel_transport_save_xml(ChannelTransport *ct, xmlNode *node)
 * \brief Function to save a ChannelTransport structure on a XML node.
 * \param ct
 * \brief ChannelTransport structure.
 * \param node
 * \brief XML node.
 */
#if !INLINE_CHANNEL_TRANSPORT_SAVE_XML
void
channel_transport_save_xml (ChannelTransport * ct, xmlNode * node)
{
  _channel_transport_save_xml (ct, node);
}
#endif
