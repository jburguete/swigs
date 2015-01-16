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
 * \file editor_system.h
 * \brief Header file to define channels system editor structures and functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2014 Javier Burguete Tolosa.
 */
#ifndef EDITOR_SYSTEM__H
#define EDITOR_SYSTEM__H 1

#include "editor_transport.h"
#include "editor_channel.h"

/**
 * \struct EditorSystem
 * \brief Structure to define an editor of channel systems.
 */ 
typedef struct
{
/**
 * \var editor_channel
 * \brief EditorChannel to define the channels editor.
 * \var editor_transport
 * \brief EditorTransport to define the transported solutes editor.
 * \var system
 * \brief System to define the channels system data.
 * \var graphic
 * \brief JBWGraphic to show the channels system graphic.
 * \var label_name
 * \brief GtkLabel to show a label of the system name.
 * \var label_section
 * \brief GtkLabel to show a label of the active cross section.
 * \var entry_name
 * \brief GtkEntry to set the system name.
 * \var button_insert
 * \brief GtkButton to insert a cross section.
 * \var button_remove
 * \brief GtkButton to remove a cross section.
 * \var button_plot
 * \brief GtkButton to update the system graphic.
 * \var combo_section
 * \brief GtkComboBoxText to set the active cross section.
 * \var grid
 * \brief GtkGrid to pack the widgets.
 * \var notebook
 * \brief GtkNotebook to put the widgets.
 * \var id_section
 * \brief identifier number of the combo_section signals handler.
 */
	EditorChannel editor_channel[1];
	EditorTransport editor_transport[1];
	System system[1];
	JBWGraphic *graphic;
	GtkLabel *label_name, *label_initial_time, *label_observation_time,
		*label_final_time, *label_measured_interval, *label_cfl,
		*label_implicit, *label_steady_error, *label_max_steady_time,
		*label_section_width_min, *label_depth_min,
		*label_granulometric_coefficient, *label_transport, *label_channel,
		*label_section, *label_transient, *label_initial_flow,
		*label_boundary_flow, *label_initial_transport,
		*label_boundary_transport;
	GtkEntry *entry_name, *entry_initial_time, *entry_observation_time,
		*entry_final_time;
	GtkSpinButton *entry_measured_interval, *entry_cfl, *entry_implicit,
		*entry_steady_error, *entry_max_steady_time, *entry_section_width_min,
		*entry_depth_min, *entry_granulometric_coefficient;
	GtkButton *button_insert_transport, *button_remove_transport,
		*button_insert_channel, *button_remove_channel, *button_plot,
		*button_transport_up, *button_channel_up,
		*button_section_up, *button_transient_up,
		*button_initial_flow_up, *button_boundary_flow_up,
		*button_initial_transport_up, *button_boundary_transport_up,
		*button_transport_down, *button_channel_down,
		*button_section_down, *button_transient_down,
		*button_initial_flow_down, *button_boundary_flow_down,
		*button_initial_transport_down, *button_boundary_transport_down;
	GtkComboBoxText *combo_transport, *combo_channel, *combo_section,
		*combo_transient, *combo_initial_flow, *combo_boundary_flow,
		*combo_initial_transport, *combo_boundary_transport;
	GtkGrid *grid;
	GtkNotebook *notebook;
	int id_transport, id_channel, id_section, id_transient, id_initial_flow,
		id_boundary_flow, id_initial_transport, id_boundary_transport;
} EditorSystem;

void editor_system_update(EditorSystem *editor);
int editor_system_get_transport(EditorSystem *editor);
int editor_system_get_channel(EditorSystem *editor);
int editor_system_get(EditorSystem *editor);
void editor_system_open_transport(EditorSystem *editor);
void editor_system_open_channel(EditorSystem *editor);
void editor_system_open(EditorSystem *editor);
void editor_system_insert_transport(EditorSystem *editor);
void editor_system_remove_transport(EditorSystem *editor);
void editor_system_insert_channel(EditorSystem *editor);
void editor_system_remove_channel(EditorSystem *editor);
void editor_system_draw(EditorSystem *editor);
int editor_system_check(EditorSystem *editor);
void editor_system_destroy(EditorSystem *editor);
void editor_system_new(EditorSystem *editor);
void editor_system_change_current_page(EditorSystem *editor);
void editor_system_ok(EditorSystem *editor);

#endif
