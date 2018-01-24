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
 * \file editor_boundary_flow.h
 * \brief Header file to define flow boundary conditions editor structures and
 *   functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2015 Javier Burguete Tolosa.
 */
#ifndef EDITOR_BOUNDARY_FLOW__H
#define EDITOR_BOUNDARY_FLOW__H 1

#include "boundary_flow.h"

/**
 * \struct EditorJunction
 * \brief Structure to define an editor of junctions data.
 */
typedef struct
{
/**
 * \var label_channel
 * \brief GtkLabel to show a label of the junction channel.
 * \var label_initial
 * \brief GtkLabel to show a label of the junction initial cross section.
 * \var label_final
 * \brief GtkLabel to show a label of the junction final cross section.
 * \var combo_channel
 * \brief GtkComboBoxText to set the junction channel.
 * \var combo_initial
 * \brief GtkComboBoxText to set the junction initial cross section.
 * \var combo_final
 * \brief GtkComboBoxText to set the junction final cross section.
 * \var grid
 * \brief GtkGrid to pack the junction widgets.
 * \var frame
 * \brief GtkFrame to group the junction widgets.
 * \var channel_name
 * \brief array of channel names.
 * \var section_name
 * \brief array of channel cross section names.
 * \var nchannels
 * \brief number of channels.
 * \var nsections
 * \brief array of channel cross section numbers.
 */
  JunctionData jd[1];
  GtkLabel *label_channel, *label_initial, *label_final;
  GtkComboBoxText *combo_channel, *combo_initial, *combo_final;
  GtkGrid *grid;
  GtkFrame *frame;
  char **channel_name, ***section_name;
  int nchannels, *nsections;
} EditorJunction;

/**
 * \struct EditorBoundaryFlow
 * \brief Structure to define an editor of flow boundary conditions.
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
 * \var label_type
 * \brief GtkLabel to show a label of the flow boundary condition type.
 * \var label_name
 * \brief GtkLabel to show a label of the flow boundary condition name.
 * \var label_initial
 * \brief GtkLabel to show a label of the initial cross section.
 * \var label_final
 * \brief GtkLabel to show a label of the final cross section.
 * \var label_delay
 * \brief GtkLabel to show a label of the delay.
 * \var label_discharge
 * \brief GtkLabel to show a label of the discharge.
 * \var label_depth
 * \brief GtkLabel to show a label of the depth.
 * \var label_level
 * \brief GtkLabel to show a label of the level.
 * \var label_roughness
 * \brief GtkLabel to show a label of the roughness coefficient.
 * \var label_height
 * \brief GtkLabel to show a label of the height.
 * \var label_width
 * \brief GtkLabel to show a label of the width.
 * \var label_junction
 * \brief GtkLabel to show a label of the junction data.
 * \var entry_name
 * \brief GtkEntry to set the flow boundary condition name.
 * \var entry_delay
 * \brief GtkSpinButton to set the delay to apply the boundary condition.
 * \var label_discharge
 * \brief GtkSpinButton to set the discharge.
 * \var label_depth
 * \brief GtkSpinButton to set the depth.
 * \var label_level
 * \brief GtkSpinButton to set the level.
 * \var label_roughness
 * \brief GtkSpinButton to set the roughness coefficient.
 * \var label_height
 * \brief GtkSpinButton to set the height.
 * \var label_width
 * \brief GtkSpinButton to set the width.
 * \var combo_initial
 * \brief GtkComboBoxText to set the initial cross section to apply the boundary
 *   condition.
 * \var combo_final
 * \brief GtkComboBoxText to set the final cross section to apply the boundary
 *   condition.
 * \var combo_junction
 * \brief GtkComboBoxText to set the junction.
 * \var button_insert
 * \brief GtkButton to insert a point.
 * \var button_remove
 * \brief GtkButton to remove a point.
 * \var button_plot
 * \brief GtkButton to update the flow boundary conditions graphic.
 * \var button_junction_insert
 * \brief GtkButton to insert a junction.
 * \var button_junction_remove
 * \brief GtkButton to remove a junction.
 * \var array_type
 * \brief array of GtkRadioButtons to set the flow boundary conditions type.
 * \var frame_type
 * \brief GtkFrame to group the array_type buttons.
 * \var grid
 * \brief GtkGrid to pack the widgets.
 * \var grid_type
 * \brief GtkGrid to pack the array_type buttons.
 * \var grid_junction
 * \brief GtkGrid to pack the junction buttons.
 * \var notebook
 * \brief GtkNotebook to put the widgets.
 * \var id_junction
 * \brief identifier number of the combo_junction signals handler.
 * \var ijunction
 * \brief active junction data index.
 */
  EditorJunction editor_junction[1];
  BoundaryFlow bf[1];
  JBWArrayEditor *array;
  JBWGraphic *graphic;
  GtkLabel *label_type, *label_name, *label_initial, *label_final,
    *label_delay, *label_discharge, *label_depth, *label_level,
    *label_roughness, *label_height, *label_width, *label_junction;
  GtkEntry *entry_name;
  GtkSpinButton *entry_delay, *entry_discharge, *entry_depth, *entry_level,
    *entry_roughness, *entry_height, *entry_width;
  GtkComboBoxText *combo_initial, *combo_final, *combo_junction;
  GtkButton *button_insert, *button_remove, *button_plot,
    *button_insert_junction, *button_remove_junction;
  GtkRadioButton *array_type[N_BOUNDARY_FLOW_TYPES];
  GtkFrame *frame_type;
  GtkGrid *grid, *grid_type, *grid_junction;
  GtkNotebook *notebook;
  int id_junction, ijunction;
} EditorBoundaryFlow;

void editor_junction_update (EditorJunction * junction);
void editor_junction_update_channels
  (EditorJunction * junction, char **channel_name, int nchannels);
void editor_junction_update_sections
  (EditorJunction * junction, int channel, char **section_name, int nsections);
int editor_junction_get (EditorJunction * editor);
void editor_junction_open (EditorJunction * editor);
void editor_junction_destroy (EditorJunction * editor);
void editor_junction_new (EditorJunction * junction, char **channel_name,
                          int nchannels, char ***section_name, int *nsections);

void editor_boundary_flow_update_junction (EditorBoundaryFlow * editor);
void editor_boundary_flow_update (EditorBoundaryFlow * editor);
int editor_boundary_flow_get_junction (EditorBoundaryFlow * editor);
int editor_boundary_flow_get (EditorBoundaryFlow * editor);
void editor_boundary_flow_open_junction (EditorBoundaryFlow * editor);
void editor_boundary_flow_open (EditorBoundaryFlow * editor);
void editor_boundary_flow_insert_point (EditorBoundaryFlow * editor);
void editor_boundary_flow_remove_point (EditorBoundaryFlow * editor);
void editor_boundary_flow_insert_junction (EditorBoundaryFlow * editor);
void editor_boundary_flow_remove_junction (EditorBoundaryFlow * editor);
void editor_boundary_flow_draw (EditorBoundaryFlow * editor);
void editor_boundary_flow_destroy (EditorBoundaryFlow * editor);
void editor_boundary_flow_new (EditorBoundaryFlow * editor,
                               GtkNotebook * notebook, int position,
                               int channel, char **channel_name, int nchannels,
                               char ***section_name, int *nsections);

extern void editor_draw ();

#endif
