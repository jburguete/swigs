/*
SWIGS (Shallow Water in Irregular Geometries Simulator): a software to simulate
initial or steady transports with solute transport in channels, channel networks
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
 * \file editor_boundary_flow.h
 * \brief Header file to define flow boundary conditions editor structures and
 *   functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2014 Javier Burguete Tolosa.
 */
#ifndef EDITOR_BOUNDARY_FLOW__H
#define EDITOR_BOUNDARY_FLOW__H 1

#include "boundary_flow.h"

/**
 * \struct EditorBoundaryFlow
 * \brief Structure to define an editor of flow boundary
 *   conditions.
 */
typedef struct
{
/**
 * \var bf
 * \brief BoundaryFlow to define the flow boundary conditions data.
 * \var array
 * \brief JBWArrayEditor to define the data points.
 * \var graphic
 * \brief JBWGraphic to show the flow boundary condition graphic.
 * \var label_name
 * \brief GtkLabel to show a label of the flow boundary condition name.
 * \var label_initial
 * \brief GtkLabel to show a label of the initial cross section.
 * \var label_final
 * \brief GtkLabel to show a label of the final cross section.
 * \var label_delay
 * \brief GtkLable to show a label of the delay.
 * \var entry_name
 * \brief GtkEntry to set the flow boundary condition name.
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
 * \brief GtkButton to update the flow boundary conditions graphic.
 * \var array_type
 * \brief array of GtkRadioButtons to set the flow boundary conditions type.
 * \var frame_type
 * \brief GtkFrame to group the array_type buttons.
 * \var grid
 * \brief GtkGrid to pack the widgets.
 * \var grid_type
 * \brief GtkGrid to pack the array_type buttons.
 * \var notebook
 * \brief GtkNotebook to put the widgets.
 */
	BoundaryFlow bf[1];
	JBWArrayEditor *array;
	JBWGraphic *graphic;
	GtkLabel *label_name, *label_initial, *label_final, *label_delay;
	GtkEntry *entry_name;
	GtkSpinButton *entry_delay;
	GtkComboBoxText *combo_initial, *combo_final;
	GtkButton *button_insert, *button_remove, *button_plot;
	GtkRadioButton *array_type[N_BOUNDARY_FLOW_TYPES];
	GtkFrame *frame_type;
	GtkGrid *grid, *grid_type;
	GtkNotebook *notebook;
} EditorBoundaryFlow;

void editor_boundary_flow_update(EditorBoundaryFlow *editor);
int editor_boundary_flow_get(EditorBoundaryFlow *editor);
void editor_boundary_flow_open(EditorBoundaryFlow *editor);
void editor_boundary_flow_insert_point(EditorBoundaryFlow *editor);
void editor_boundary_flow_remove_point(EditorBoundaryFlow *editor);
void editor_boundary_flow_draw(EditorBoundaryFlow *editor);
void editor_boundary_flow_destroy(EditorBoundaryFlow *editor);
void editor_boundary_flow_new(EditorBoundaryFlow *editor,
	GtkNotebook *notebook, char **section_name, int nsections);

extern void editor_draw();

#endif
