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
 * \file editor_channel_geometry.h
 * \brief Header file to define channel geometry editor structures and methods.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2014 Javier Burguete Tolosa.
 */
#ifndef EDITOR_CHANNEL_GEOMETRY__H
#define EDITOR_CHANNEL_GEOMETRY__H 1

#include "channel_geometry.h"
#include "editor_cross_section.h"

/**
 * \struct EditorChannelGeometry
 * \brief Structure to define an editor of channel geometries.
 */ 
typedef struct
{
/**
 * \var editor_section
 * \brief EditorCrossSection to define the cross sections editor.
 * \var cg
 * \brief ChannelGeometry to define the channel geometry data.
 * \var graphic
 * \brief JBWGraphic to show the channel geometry graphic.
 * \var label_section
 * \brief GtkLabel to show a label of the active cross section.
 * \var button_insert
 * \brief GtkButton to insert a cross section.
 * \var button_remove
 * \brief GtkButton to remove a cross section.
 * \var button_plot
 * \brief GtkButton to update the channel geometry graphic.
 * \var combo_section
 * \brief GtkComboBoxText to set the active cross section.
 * \var grid
 * \brief GtkGrid to pack the widgets.
 * \var notebook
 * \brief GtkNotebook to put the widgets.
 * \var id_section
 * \brief identifier number of the combo_section signals handler.
 */
	EditorCrossSection editor_section[1];
	ChannelGeometry cg[1];
	JBWGraphic *graphic;
	GtkLabel *label_section;
	GtkButton *button_insert, *button_remove, *button_plot;
	GtkComboBoxText *combo_section;
	GtkGrid *grid;
	GtkNotebook *notebook;
	int id_section;
} EditorChannelGeometry;

void editor_channel_geometry_update(EditorChannelGeometry *editor);
void editor_channel_geometry_get_section(EditorChannelGeometry *editor);
void editor_channel_geometry_open_section(EditorChannelGeometry *editor);
void editor_channel_geometry_open(EditorChannelGeometry *editor);
void editor_channel_geometry_insert_section(EditorChannelGeometry *editor);
void editor_channel_geometry_remove_section(EditorChannelGeometry *editor);
void editor_channel_geometry_draw(EditorChannelGeometry *editor);
void editor_channel_geometry_destroy(EditorChannelGeometry *editor);
void editor_channel_geometry_new(EditorChannelGeometry *editor,
	GtkNotebook *notebook, char **channel_name, int nchannels,
	char ***section_name, int *nsections);

#if TEST_EDITOR_CHANNEL_GEOMETRY
	void editor_draw();
#endif

#endif
