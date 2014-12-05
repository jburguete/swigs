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
 * \file editor_cross_section.c
 * \brief Source file to define cross section editor structures and methods.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2014 Javier Burguete Tolosa.
 */
#include "editor_cross_section.h"

#if TEST_EDITOR_CROSS_SECTION
	char *message;
	CrossSection cs[1];
	EditorCrossSection editor[1];
#endif

void editor_control_update(EditorControl *control)
{
	int i;
	i = gtk_combo_box_get_active(GTK_COMBO_BOX(control->combo_channel));
	jbw_combo_box_set_strings
		(control->combo_section, control->section_name[i], nsections[i]);
}

void editor_control_new(EditorControl *control, char **channel_name,
	int nchannels, char ***section_name, int *nsections)
{
	int i;
	const char *array_label[N_CONTROL_TYPES] = {
		gettext("Controlled by discharge"),
		gettext("Controlled by depth"),
		gettext("Controlled by level")};
	control->nchannels = nchannels;
	control->nsections = nsections;
	control->channel_name = channel_name;
	control->section_name = section_name;
	control->grid = (GtkGrid*)gtk_grid_new();
	control->grid_type = (GtkGrid*)gtk_grid_new();
	control->array_type[0] = NULL;
	for (i = 0; i < N_CONTROL_TYPES; ++i)
	{
		control->array_type[i]
			= (GtkRadioButton*)gtk_radio_button_new_with_label_from_widget
				(control->array_type[0], array_label[i]);
		gtk_grid_attach(control->grid_type, GTK_WIDGET(control->array_type[i]),
			0, i, 1, 1);
	}
	control->frame_type = (GtkFrame*)gtk_frame_new(gettext("Type"));
	gtk_container_add
		(GTK_CONTAINER(control->frame_type), GTK_WIDGET(control->grid_type));
	gtk_grid_attach(control->grid, GTK_WIDGET(control->frame_type), 0, 0, 2, 1);
	control->label_channel = (GtkLabel*)gtk_label_new(gettext("Channel"));
	gtk_grid_attach
		(control->grid, GTK_WIDGET(control->label_channel), 0, 1, 1, 1);
	control->combo_channel
		= jbw_combo_box_new_with_strings(channel_name, nchannels);
	gtk_grid_attach
		(control->grid, GTK_WIDGET(control->combo_channel), 1, 1, 1, 1);
	control->label_section = (GtkLabel*)gtk_label_new(gettext("Cross section"));
	gtk_grid_attach
		(control->grid, GTK_WIDGET(control->label_section), 0, 2, 1, 1);
	control->combo_section
		= jbw_combo_box_new_with_strings(section_name[0], nsections[0]);
	gtk_grid_attach
		(control->grid, GTK_WIDGET(control->combo_section), 1, 2, 1, 1);
	control->label_parameter
		= (GtkLabel*)gtk_label_new(gettext("Parameter value"));
	gtk_grid_attach
		(control->grid, GTK_WIDGET(control->label_parameter), 0, 3, 1, 1);
	control->entry_parameter
		= (GtkSpinButton*)gtk_spin_button_new_with_range(-1e6, 1e6, 1e-6);
	gtk_grid_attach
		(control->grid, GTK_WIDGET(control->entry_parameter), 1, 3, 1, 1);
	control->label_time = (GtkLabel*)gtk_label_new(gettext("Integration time"));
	gtk_grid_attach
		(control->grid, GTK_WIDGET(control->label_time), 0, 4, 1, 1);
	control->entry_time
		= (GtkSpinButton*)gtk_spin_button_new_with_range(0, 1e6, 1e-3);
	gtk_grid_attach
		(control->grid, GTK_WIDGET(control->entry_time), 1, 4, 1, 1);
	control->label_tolerance
		= (GtkLabel*)gtk_label_new(gettext("Tolerance"));
	gtk_grid_attach
		(control->grid, GTK_WIDGET(control->label_tolerance), 0, 5, 1, 1);
	control->entry_tolerance
		= (GtkSpinButton*)gtk_spin_button_new_with_range(-1e4, 1e4, 1e-6);
	gtk_grid_attach
		(control->grid, GTK_WIDGET(control->entry_tolerance), 1, 5, 1, 1);
	control->frame = (GtkFrame*)gtk_frame_new(gettext("Control"));
	gtk_container_add(GTK_CONTAINER(control->frame), GTK_WIDGET(control->grid));
	g_signal_connect_swapped(control->combo_channel, "changed",
		(void(*))&editor_control_update, control);
}

/**
 * \fn void editor_cross_section_update(EditorCrossSection *editor)
 * \brief Function to update the view of a cross section editor.
 * \param editor
 * \brief cross section editor.
 */
void editor_cross_section_update(EditorCrossSection *editor)
{
	int i;
	editor_control_update(editor->control);
	i = jbw_array_radio_buttons_get_active(editor->array_type);
	if (i) gtk_widget_show(GTK_WIDGET(editor->control->frame));
	else gtk_widget_hide(GTK_WIDGET(editor->control->frame));
}

void editor_cross_section_new(EditorCrossSection *editor, char **channel_name,
	int nchannels, char ***section_name, int *nsections)
{
	int i;
	const char *label_type[2] = {gettext("Normal"), gettext("Control")};
	editor->grid = (GtkGrid*)gtk_grid_new();
	editor->label_name = (GtkLabel*)gtk_label_new(gettext("Name"));
	gtk_grid_attach(editor->grid, GTK_WIDGET(editor->label_name), 0, 0, 1, 1);
	editor->entry_name = (GtkEntry*)gtk_entry_new();
	gtk_grid_attach(editor->grid, GTK_WIDGET(editor->entry_name), 1, 0, 2, 1);
	editor->grid_type = (GtkGrid*)gtk_grid_new();
	editor->array_type[0] = NULL;
	for (i = 0; i < 2; ++i)
	{
		control->array_type[i]
			= (GtkRadioButton*)gtk_radio_button_new_with_label_from_widget
				(control->array_type[0], array_label[i]);
		gtk_grid_attach(control->grid_type, GTK_WIDGET(control->array_type[i]),
			0, i, 1, 1);
		g_signal_connect_swapped(control->array_type[i], "toggled",
			(void(*))&editor_channel_update, editor);
	}
	control->frame_type = (GtkFrame*)gtk_frame_new(gettext("Type"));
	gtk_container_add
		(GTK_CONTAINER(control->frame_type), GTK_WIDGET(control->grid_type));
	gtk_grid_attach(control->grid, GTK_WIDGET(control->frame_type), 0, 1, 3, 1);
	editor->label_x = (GtkLabel*)gtk_label_new("x");
	gtk_grid_attach(editor->grid, GTK_WIDGET(editor->label_x), 0, 2, 1, 1);
	editor->entry_x
		= (GtkSpinButton*)gtk_spin_button_new_with_range(-1e6, 1e6, 0.001);
	gtk_grid_attach(editor->grid, GTK_WIDGET(editor->entry_x), 1, 2, 2, 1);
	editor->label_y = (GtkLabel*)gtk_label_new("y");
	gtk_grid_attach(editor->grid, GTK_WIDGET(editor->label_y), 0, 3, 1, 1);
	editor->entry_y
		= (GtkSpinButton*)gtk_spin_button_new_with_range(-1e6, 1e6, 0.001);
	gtk_grid_attach(editor->grid, GTK_WIDGET(editor->entry_y), 1, 3, 2, 1);
	editor->label_angle = (GtkLabel*)gtk_label_new(gettext("Angle"));
	gtk_grid_attach(editor->grid, GTK_WIDGET(editor->label_angle), 0, 4, 1, 1);
	editor->entry_angle
		= (GtkSpinButton*)gtk_spin_button_new_with_range(-360., 360., 0.001);
	gtk_grid_attach(editor->grid, GTK_WIDGET(editor->entry_angle), 1, 4, 2, 1);
	editor->button_insert = (GtkButton*)gtk_button_new_with_label
		(gettext("Insert transient section"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->button_insert), 0, 5, 1, 1);
	editor->button_remove = (GtkButton*)gtk_button_new_with_label
		(gettext("Remove transient section"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->button_remove), 1, 5, 1, 1);
	editor->button_plot
		= (GtkButton*)gtk_button_new_with_label(gettext("Update plot"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->button_plot), 2, 5, 1, 1);
	editor->label_transient
		= (GtkLabel*)gtk_label_new(gettext("Transient section"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->label_transient), 0, 6, 1, 1);
	editor->combo_transient = (GtkComboBoxText*)gtk_combo_box_text_new();
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->combo_transient), 1, 6, 2, 1);
	editor_control_new
		(editor->control, channel_name, nchannels, section_name, nsections);
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->control->frame), 0, 7, 3, 1);
}
