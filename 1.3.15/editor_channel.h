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
 * \file editor_channel.h
 * \brief Header file to define channel editor structures and functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2015 Javier Burguete Tolosa.
 */
#ifndef EDITOR_CHANNEL__H
#define EDITOR_CHANNEL__H 1

#include "channel.h"
#include "editor_cross_section.h"
#include "editor_initial_flow.h"
#include "editor_boundary_flow.h"

/**
 * \struct EditorChannel
 * \brief Structure to define an editor of channels.
 */
typedef struct
{
/**
 * \var editor_section
 * \brief EditorCrossSection to define the cross sections editor.
 * \var editor_initial
 * \brief EditorInitialFlow to define the flow initial conditions editor.
 * \var editor_inlet
 * \brief EditorBoundaryFlow to define the inlet flow boundary condition editor.
 * \var editor_outlet
 * \brief EditorBoundaryFlow to define the outlet flow boundary condition
 *   editor.
 * \var editor_inner
 * \brief EditorBoundaryFlow to define the inner flow boundary conditions
 *   editor.
 * \var channel
 * \brief Channel to define the channel data.
 * \var graphic
 * \brief JBWGraphic to show the channel graphic.
 * \var label_name
 * \brief GtkLabel to show a label of the channel name.
 * \var label_section
 * \brief GtkLabel to show a label of the active cross section.
 * \var label_inner
 * \brief GtkLabel to show a label of the active inner flow boundary condition.
 * \var entry_name
 * \brief GtkEntry to set the channel name.
 * \var button_insert
 * \brief GtkButton to insert a cross section.
 * \var button_remove
 * \brief GtkButton to remove a cross section.
 * \var button_plot
 * \brief GtkButton to update the channel graphic.
 * \var button_insert_inner
 * \brief GtkButton to insert an inner flow boundary condition.
 * \var button_remove_inner
 * \brief GtkButton to remove an inner flow boundary condition.
 * \var combo_section
 * \brief GtkComboBoxText to set the active cross section.
 * \var combo_inner
 * \brief GtkComboBoxText to set the active inner flow boundary condition.
 * \var grid
 * \brief GtkGrid to pack the widgets.
 * \var notebook
 * \brief GtkNotebook to put the widgets.
 * \var id_section
 * \brief identifier number of the combo_section signals handler.
 * \var id_section
 * \brief identifier number of the combo_inner signals handler.
 */
  EditorCrossSection editor_section[1];
  EditorInitialFlow editor_initial[1];
  EditorBoundaryFlow editor_inlet[1], editor_outlet[1], editor_inner[1];
  Channel channel[1];
  JBWGraphic *graphic;
  GtkLabel *label_name, *label_section, *label_inner;
  GtkEntry *entry_name;
  GtkButton *button_insert, *button_remove, *button_plot,
    *button_insert_inner, *button_remove_inner;
  GtkComboBoxText *combo_section, *combo_inner;
  GtkGrid *grid;
  GtkNotebook *notebook;
  int id_section, id_inner;
} EditorChannel;

void editor_channel_update (EditorChannel * editor);
int editor_channel_get_section (EditorChannel * editor);
int editor_channel_get_inlet (EditorChannel * editor);
int editor_channel_get_outlet (EditorChannel * editor);
int editor_channel_get_inner (EditorChannel * editor);
int editor_channel_get (EditorChannel * editor);
void editor_channel_open_section (EditorChannel * editor);
void editor_channel_open_inlet (EditorChannel * editor);
void editor_channel_open_outlet (EditorChannel * editor);
void editor_channel_open_inner (EditorChannel * editor);
void editor_channel_open (EditorChannel * editor);
void editor_channel_insert_section (EditorChannel * editor);
void editor_channel_remove_section (EditorChannel * editor);
void editor_channel_insert_inner (EditorChannel * editor);
void editor_channel_remove_inner (EditorChannel * editor);
void editor_channel_draw (EditorChannel * editor);
int editor_channel_check (EditorChannel * editor);
void editor_channel_destroy (EditorChannel * editor);
void editor_channel_new (EditorChannel * editor, GtkNotebook * notebook,
                         char **channel_name, int nchannels,
                         char ***section_name, int *nsections);

#if TEST_EDITOR_CHANNEL
void editor_draw ();
#endif

#endif
