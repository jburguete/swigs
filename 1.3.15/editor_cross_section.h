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
 * \file editor_cross_section.h
 * \brief Header file to define cross section editor structures and functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2015 Javier Burguete Tolosa.
 */
#ifndef EDITOR_CROSS_SECTION__H
#define EDITOR_CROSS_SECTION__H 1

#include "cross_section.h"
#include "editor_transient_section.h"

/**
 * \def N_CONTROL_TYPES
 * \brief Macro to set the number of cross section controlled types.
 */
#define N_CONTROL_TYPES 3

/**
 * \struct EditorControl
 * \brief Structure to define the data of a controlled cross section.
 */
typedef struct
{
/**
 * \var label_channel
 * \brief GtkLabel to show a label of the channel.
 * \var label_section
 * \brief GtkLabel to show a label of the cross section.
 * \var label_parameter
 * \brief GtkLabel to show a label of the parameter value.
 * \var label_time
 * \brief GtkLabel to show a label of the integration time.
 * \var label_tolerance
 * \brief GtkLabel to show a label of the tolerance.
 * \var combo_channel
 * \brief GtkComboBoxText to set the channel where is the cross section to
 *   control.
 * \var combo_section
 * \brief GtkComboBoxText to set the cross section to control.
 * \var entry_parameter
 * \brief GtkSpinButton to set the parameter value.
 * \var entry_time
 * \brief GtkSpinButton to set the integration time.
 * \var entry_tolerance
 * \brief GtkSpinButton to set the tolerance.
 * \var array_type
 * \brief array of GtkRadioButton widgets to set the control type.
 * \var grid
 * \brief GtkGrid to pack the widgets.
 * \var grid_type
 * \brief GtkGrid to pack the array_type widgets.
 * \var frame
 * \brief GtkFrame to group the widgets.
 * \var frame_type
 * \brief GtkFrame to group the array_type widgets.
 * \var channel_name
 * \brief array of channel names.
 * \var section_name
 * \brief array of channel cross section names.
 * \var nchannels
 * \brief number of channels.
 * \var nsections
 * \brief array of channel cross section numbers.
 */
  GtkLabel *label_channel, *label_section, *label_parameter, *label_time,
    *label_tolerance;
  GtkComboBoxText *combo_channel, *combo_section;
  GtkSpinButton *entry_parameter, *entry_time, *entry_tolerance;
  GtkRadioButton *array_type[N_CONTROL_TYPES];
  GtkGrid *grid, *grid_type;
  GtkFrame *frame, *frame_type;
  char **channel_name, ***section_name;
  int nchannels, *nsections;
} EditorControl;

/**
 * \struct EditorCrossSection
 * \brief Structure to define an editor of cross sections.
 */
typedef struct
{
/**
 * \var editor_transient
 * \brief EditorTransientSection to define the transient sections editor.
 * \var control
 * \brief EditorControl to define the control parameters.
 * \var cs
 * \brief CrossSection to define the cross section data.
 * \var graphic
 * \brief JBWGraphic to show the cross section graphic.
 * \var label_name
 * \brief GtkLabel to show a label of the cross section name.
 * \var label_x
 * \brief GtkLabel to show a label of the cross section x-coordinate.
 * \var label_y
 * \brief GtkLabel to show a label of the cross section y-coordinate.
 * \var label_angle
 * \brief GtkLabel to show a label of the cross section angle of orientation.
 * \var label_transient
 * \brief GtkLabel to show a label of the active transient section.
 * \var entry_name
 * \brief GtkEntry to set the cross section name.
 * \var entry_x
 * \brief GtkSpinButton to set the cross section x-coordinate.
 * \var entry_y
 * \brief GtkSpinButton to set the cross section y-coordinate.
 * \var entry_angle
 * \brief GtkSpinButton to set the cross section angle of orientation.
 * \var button_insert
 * \brief GtkButton to insert a transient section.
 * \var button_remove
 * \brief GtkButton to remove a transient section.
 * \var button_plot
 * \brief GtkButton to update the cross section graphic.
 * \var array_type
 * \brief array of GtkRadioButton widgets to set the cross section type.
 * \var combo_transient
 * \brief GtkComboBoxText to set the active transient section.
 * \var frame_type
 * \brief GtkFrame to group the array_type buttons.
 * \var grid
 * \brief GtkGrid to pack the widgets.
 * \var grid_type
 * \brief GtkGrid to pack the array_type buttons.
 * \var notebook
 * \brief GtkNotebook to put the widgets.
 * \var id_transient
 * \brief identifier number of the combo_transient signals handler.
 */
  EditorTransientSection editor_transient[1];
  EditorControl control[1];
  CrossSection cs[1];
  JBWGraphic *graphic;
  GtkLabel *label_name, *label_x, *label_y, *label_angle, *label_transient;
  GtkEntry *entry_name;
  GtkSpinButton *entry_x, *entry_y, *entry_angle;
  GtkButton *button_insert, *button_remove, *button_plot;
  GtkRadioButton *array_type[2];
  GtkComboBoxText *combo_transient;
  GtkFrame *frame_type;
  GtkGrid *grid, *grid_type;
  GtkNotebook *notebook;
  int id_transient;
} EditorCrossSection;

void editor_control_update (EditorControl * control);
void editor_control_update_channels
  (EditorControl * control, char **channel_name, int channels);
void editor_control_update_sections
  (EditorControl * control, int channel, char **section_name, int nsections);
void editor_control_new (EditorControl * control, char **channel_name,
                         int nchannels, char ***section_name, int *nsections);

void editor_cross_section_update (EditorCrossSection * editor);
int editor_cross_section_get_transient (EditorCrossSection * editor);
int editor_cross_section_get (EditorCrossSection * editor);
void editor_cross_section_open_transient (EditorCrossSection * editor);
void editor_cross_section_open (EditorCrossSection * editor);
void editor_cross_section_insert_transient (EditorCrossSection * editor);
void editor_cross_section_remove_transient (EditorCrossSection * editor);
void editor_cross_section_draw (EditorCrossSection * editor);
int editor_cross_section_check (EditorCrossSection * editor);
void editor_cross_section_destroy (EditorCrossSection * editor);
void editor_cross_section_new (EditorCrossSection * editor,
                               GtkNotebook * notebook, char **channel_name,
                               int nchannels, char ***section_name,
                               int *nsections);

#if TEST_EDITOR_CROSS_SECTION
void editor_draw ();
#endif

#endif
