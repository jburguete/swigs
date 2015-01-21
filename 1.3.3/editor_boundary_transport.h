/*
SWIGS (Shallow Water in Irregular Geometries Simulator): a software to simulate
initial or steady transports with solute transport in channels, channel networks
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
 * \file editor_boundary_transport.h
 * \brief Header file to define transported solute boundary conditions editor
 *   structures and functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2015 Javier Burguete Tolosa.
 */
#ifndef EDITOR_BOUNDARY_TRANSPORT__H
#define EDITOR_BOUNDARY_TRANSPORT__H 1

#include "boundary_transport.h"

/**
 * \struct EditorBoundaryTransport
 * \brief Structure to define an editor of transported solute boundary
 *   conditions.
 */
typedef struct
{
/**
 * \var bt
 * \brief BoundaryTransport to define the transported solute boundary conditions
 *   data.
 * \var array
 * \brief JBWArrayEditor to define the data points.
 * \var graphic
 * \brief JBWGraphic to show the transported solute boundary condition graphic.
 * \var label_name
 * \brief GtkLabel to show a label of the transported solute boundary condition
 *   name.
 * \var label_initial
 * \brief GtkLabel to show a label of the initial cross section.
 * \var label_final
 * \brief GtkLabel to show a label of the final cross section.
 * \var label_delay
 * \brief GtkLable to show a label of the delay.
 * \var entry_name
 * \brief GtkEntry to set the transported solute boundary condition name.
 * \var entry_delay
 * \brief GtkSpinButton to set the delay to apply the boundary condition.
 * \var combo_initial
 * \brief GtkComboBoxText to set the initial cross section to apply the boundary
 *   condition.
 * \var combo_final
 * \brief GtkComboBoxText to set the final cross section to apply the boundary
 *   condition.
 * \var button_insert
 * \brief GtkButton to insert a point.
 * \var button_remove
 * \brief GtkButton to remove a point.
 * \var button_plot
 * \brief GtkButton to update the transported solute boundary conditions
 *   graphic.
 * \var array_type
 * \brief array of GtkRadioButtons to set the transported solute boundary
 *   conditions type.
 * \var frame_type
 * \brief GtkFrame to group the array_type buttons.
 * \var grid
 * \brief GtkGrid to pack the widgets.
 * \var grid_type
 * \brief GtkGrid to pack the array_type buttons.
 * \var notebook
 * \brief GtkNotebook to put the widgets.
 */
	BoundaryTransport bt[1];
	JBWArrayEditor *array;
	JBWGraphic *graphic;
	GtkLabel *label_name, *label_initial, *label_final, *label_delay;
	GtkEntry *entry_name;
	GtkSpinButton *entry_delay;
	GtkComboBoxText *combo_initial, *combo_final;
	GtkButton *button_insert, *button_remove, *button_plot;
	GtkRadioButton *array_type[N_BOUNDARY_TRANSPORT_TYPES];
	GtkFrame *frame_type;
	GtkGrid *grid, *grid_type;
	GtkNotebook *notebook;
} EditorBoundaryTransport;

void editor_boundary_transport_update(EditorBoundaryTransport *editor);
int editor_boundary_transport_get(EditorBoundaryTransport *editor);
void editor_boundary_transport_open(EditorBoundaryTransport *editor);
void editor_boundary_transport_insert_point(EditorBoundaryTransport *editor);
void editor_boundary_transport_remove_point(EditorBoundaryTransport *editor);
void editor_boundary_transport_draw(EditorBoundaryTransport *editor);
void editor_boundary_transport_destroy(EditorBoundaryTransport *editor);
void editor_boundary_transport_new(EditorBoundaryTransport *editor,
	GtkNotebook *notebook, char **section_name, int nsections);

extern void editor_draw();

#endif
