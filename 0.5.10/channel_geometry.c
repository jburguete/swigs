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
 * \file channel_geometry.c
 * \brief Source file to define channel geometry structures and functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005 - 2014 Javier Burguete Tolosa.
 */
#include "channel_geometry.h"

/**
 * \fn void channel_geometry_print(ChannelGeometry *cg, FILE *file)
 * \brief Function to print a ChannelGeometry structure in a file.
 * \param cg
 * \brief ChannelGeometry structure.
 * \param file
 * \brief file.
 */
#if !INLINE_CHANNEL_GEOMETRY_PRINT
void channel_geometry_print(ChannelGeometry *cg, FILE *file)
{_channel_geometry_print(cg, file);}
#endif

/**
 * \fn void channel_geometry_error(char *m) {_channel_geometry_error(m);}
 * \brief Function to write an error message opening a ChannelGeometry structure.
 * \param m
 * \brief error message.
 */
#if !INLINE_CHANNEL_GEOMETRY_ERROR
void channel_geometry_error(char *m) {_channel_geometry_error(m);}
#endif

/**
 * \fn void channel_geometry_delete(ChannelGeometry *cg)
 * \brief Function to delete a ChannelGeometry structure.
 * \param cg
 * \brief ChannelGeometry structure.
 */
#if !INLINE_CHANNEL_GEOMETRY_DELETE
void channel_geometry_delete(ChannelGeometry *cg)
{_channel_geometry_delete(cg);}
#endif

/**
 * \fn void channel_geometry_init_empty(ChannelGeometry *cg)
 * \brief Function to init an empty ChannelGeometry structure.
 * \param cg
 * \brief ChannelGeometry structure.
 */
#if !INLINE_CHANNEL_GEOMETRY_INIT_EMPTY
void channel_geometry_init_empty(ChannelGeometry *cg)
{_channel_geometry_init_empty(cg);}
#endif

/**
 * \fn void channel_geometry_update_x(ChannelGeometry *cg, int i)
 * \brief Function to update the longitudinal distances to the inlet on a
 *   ChannelGeometry structure.
 * \param cg
 * \brief ChannelGeometry structure.
 * \param i
 * \brief cross section position to begin the update changes.
 */
#if !INLINE_CHANNEL_GEOMETRY_UPDATE_X
void channel_geometry_update_x(ChannelGeometry *cg, int i)
{_channel_geometry_update_x(cg, i);}
#endif

/**
 * \fn int channel_geometry_insert_section(ChannelGeometry *cg, int i)
 * \brief Function to insert a CrossSection in a ChannelGeometry structure.
 * \param cg
 * \brief ChannelGeometry structure.
 * \param i
 * \brief insertion position.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_CHANNEL_GEOMETRY_INSERT_SECTION
int channel_geometry_insert_section(ChannelGeometry *cg, int i)
{return _channel_geometry_insert_section(cg, i);}
#endif

/**
 * \fn void channel_geometry_remove_section(ChannelGeometry *cg, int i)
 * \brief Function to remove a CrossSection in a ChannelGeometry structure.
 * \param cg
 * \brief ChannelGeometry structure.
 * \param i
 * \brief deletion position.
 */
#if !INLINE_CHANNEL_GEOMETRY_REMOVE_SECTION
void channel_geometry_remove_section(ChannelGeometry *cg, int i)
{_channel_geometry_remove_section(cg, i);}
#endif

/**
 * \fn int channel_geometry_copy(ChannelGeometry *cg, ChannelGeometry *cg_copy)
 * \brief Function to copy two ChannelGeometry structures.
 * \param cg
 * \brief ChannelGeometry structure copy.
 * \param cg_copy
 * \brief CrossGeometry structure copied.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_CHANNEL_GEOMETRY_COPY
int channel_geometry_copy(ChannelGeometry *cg, ChannelGeometry *cg_copy)
{return _channel_geometry_copy(cg, cg_copy);}
#endif

/**
 * \fn int channel_geometry_open_xml(ChannelGeometry *cg, xmlNode *node)
 * \brief Function to open a ChannelGeometry structure on a XML node.
 * \param cg
 * \brief ChannelGeometry structure.
 * \param node
 * \brief XML node.
 * \return 0 on error, 1 on success.
 */
#if !INLINE_CHANNEL_GEOMETRY_OPEN_XML
int channel_geometry_open_xml(ChannelGeometry *cg, xmlNode *node)
{return _channel_geometry_open_xml(cg, node);}
#endif

/**
 * \fn int channel_geometry_save_xml(ChannelGeometry *cg, xmlNode *node)
 * \brief Function to save a ChannelGeometry structure on a XML node.
 * \param cg
 * \brief ChannelGeometry structure.
 * \param node
 * \brief XML node.
 */
#if !INLINE_CHANNEL_GEOMETRY_SAVE_XML
void channel_geometry_save_xml(ChannelGeometry *cg, xmlNode *node)
{_channel_geometry_save_xml(cg, node);}
#endif
