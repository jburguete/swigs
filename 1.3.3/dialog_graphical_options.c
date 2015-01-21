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

#include "simulate.h"
#include "dialog_simulator.h"
#include "dialog_graphical_options.h"

void string_graphical_options(char **string, System *sys)
{
	register int i;
	string[0] = gettext("Discharge");
	string[1] = gettext("Depth");
	string[2] = gettext("Level");
	string[3] = gettext("Velocity");
	string[4] = gettext("Froude number");
	string[5] = gettext("Area");
	string[6] = gettext("Top width");
	for (i = 0; i <= sys->nt; ++i) string[7 + i] = sys->transport[i].name;
}

void dialog_graphical_options_actualize(DialogGraphicalOptions *dlg)
{
	register int k;
	k = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(dlg->button_adjust));
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->table_profile_x), k);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->label_profile_ymin), k);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->entry_profile_ymin), k);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->label_profile_ymax), k);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->entry_profile_ymax), k);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->label_profile_zmin), k);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->entry_profile_zmin), k);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->label_profile_zmax), k);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->entry_profile_zmax), k);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->table_evolution_x), k);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->label_evolution_ymin), k);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->entry_evolution_ymin), k);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->label_evolution_ymax), k);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->entry_evolution_ymax), k);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->label_evolution_zmin), k);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->entry_evolution_zmin), k);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->label_evolution_zmax), k);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->entry_evolution_zmax), k);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->table_section_x), k);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->label_section_ymin), k);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->entry_section_ymin), k);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->label_section_ymax), k);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->entry_section_ymax), k);
}

void dialog_graphical_options_sections(DialogGraphicalOptions *dlg)
{
	int i, n;
	char *name;
	n = sys->channel[channel].cg->n;
	for (i = 0; i <= n; ++i)
	{
		name = sys->channel[channel].cg->cs[i].name;
		gtk_combo_box_text_append_text(dlg->combo_evolution_section, name);
		gtk_combo_box_text_append_text(dlg->combo_section, name);
	}
	if (evolution_section > n) evolution_section = 0;
	if (section > n) section = 0;
}

void dialog_graphical_options_channel(DialogGraphicalOptions *dlg)
{
	int i;
	for (i = 0; i <= sys->channel[channel].cg->n; ++i)
	{
		gtk_combo_box_text_remove(dlg->combo_section, 0);
		gtk_combo_box_text_remove(dlg->combo_evolution_section, 0);
	}
	channel = gtk_combo_box_get_active(GTK_COMBO_BOX(dlg->combo_channel));
	dialog_graphical_options_sections(dlg);
	gtk_combo_box_set_active
		(GTK_COMBO_BOX(dlg->combo_evolution_section), evolution_section);
	gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_section), section);
}

void dialog_graphical_options_new(GtkWindow *parent)
{
	int i;
	DialogGraphicalOptions dlg;
	
	char *string_animation[3]
		= {gettext("_No animation"), gettext("_Fast"), gettext("_Detailed")};
	char *string_options[sys->nt + 8];
	char *string_channels[sys->n + 1];

	for (i = 0; i <= sys->n; ++i) string_channels[i] = sys->channel[i].name;
	
	string_graphical_options(string_options, sys);

	dlg.button_adjust = (GtkCheckButton*)gtk_check_button_new_with_mnemonic
		(gettext("_Manual adjust"));
	g_signal_connect_swapped(dlg.button_adjust, "clicked",
		(void(*)())dialog_graphical_options_actualize, &dlg);

	dlg.button_grid = (GtkCheckButton*)gtk_check_button_new_with_mnemonic
		(gettext("_Show grid"));

	dlg.label_profile_xmax = (GtkLabel*)gtk_label_new(gettext("Maximum"));
	dlg.entry_profile_xmax = (JBWFloatEntry*)jbw_float_entry_new();

	dlg.label_profile_xmin = (GtkLabel*)gtk_label_new(gettext("Minimum"));
	dlg.entry_profile_xmin = (JBWFloatEntry*)jbw_float_entry_new();

	dlg.table_profile_x = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach
		(dlg.table_profile_x, GTK_WIDGET(dlg.label_profile_xmax), 0, 0, 1, 1);
	gtk_grid_attach
		(dlg.table_profile_x, GTK_WIDGET(dlg.entry_profile_xmax), 1, 0, 1, 1);
	gtk_grid_attach
		(dlg.table_profile_x, GTK_WIDGET(dlg.label_profile_xmin), 0, 1, 1, 1);
	gtk_grid_attach
		(dlg.table_profile_x, GTK_WIDGET(dlg.entry_profile_xmin), 1, 1, 1, 1);

	dlg.frame_profile_x = (GtkFrame*)gtk_frame_new(gettext("Position"));
	gtk_container_add
		(GTK_CONTAINER(dlg.frame_profile_x), GTK_WIDGET(dlg.table_profile_x));

	dlg.combo_profile_y  =
		jbw_combo_box_new_with_strings(string_options, 8 + sys->nt);

	dlg.label_profile_ymax = (GtkLabel*)gtk_label_new(gettext("Maximum"));
	dlg.entry_profile_ymax = (JBWFloatEntry*)jbw_float_entry_new();

	dlg.label_profile_ymin = (GtkLabel*)gtk_label_new(gettext("Minimum"));
	dlg.entry_profile_ymin = (JBWFloatEntry*)jbw_float_entry_new();

	dlg.table_profile_y = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach
		(dlg.table_profile_y, GTK_WIDGET(dlg.combo_profile_y), 0, 0, 2, 1);
	gtk_grid_attach
		(dlg.table_profile_y, GTK_WIDGET(dlg.label_profile_ymax), 0, 1, 1, 1);
	gtk_grid_attach
		(dlg.table_profile_y, GTK_WIDGET(dlg.entry_profile_ymax), 1, 1, 1, 1);
	gtk_grid_attach
		(dlg.table_profile_y, GTK_WIDGET(dlg.label_profile_ymin), 0, 2, 1, 1);
	gtk_grid_attach
		(dlg.table_profile_y, GTK_WIDGET(dlg.entry_profile_ymin), 1, 2, 1, 1);

	dlg.frame_profile_y = (GtkFrame*)gtk_frame_new(gettext("Parameter 1"));
	gtk_container_add
		(GTK_CONTAINER(dlg.frame_profile_y), GTK_WIDGET(dlg.table_profile_y));

	dlg.combo_profile_z  =
		jbw_combo_box_new_with_strings(string_options, 8 + sys->nt);

	dlg.label_profile_zmax = (GtkLabel*)gtk_label_new(gettext("Maximum"));
	dlg.entry_profile_zmax = (JBWFloatEntry*)jbw_float_entry_new();

	dlg.label_profile_zmin = (GtkLabel*)gtk_label_new(gettext("Minimum"));
	dlg.entry_profile_zmin = (JBWFloatEntry*)jbw_float_entry_new();

	dlg.table_profile_z = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach
		(dlg.table_profile_z, GTK_WIDGET(dlg.combo_profile_z), 0, 0, 2, 1);
	gtk_grid_attach
		(dlg.table_profile_z, GTK_WIDGET(dlg.label_profile_zmax), 0, 1, 1, 1);
	gtk_grid_attach
		(dlg.table_profile_z, GTK_WIDGET(dlg.entry_profile_zmax), 1, 1, 1, 1);
	gtk_grid_attach
		(dlg.table_profile_z, GTK_WIDGET(dlg.label_profile_zmin), 0, 2, 1, 1);
	gtk_grid_attach
		(dlg.table_profile_z, GTK_WIDGET(dlg.entry_profile_zmin), 1, 2, 1, 1);

	dlg.frame_profile_z = (GtkFrame*)gtk_frame_new(gettext("Parameter 2"));
	gtk_container_add
		(GTK_CONTAINER(dlg.frame_profile_z), GTK_WIDGET(dlg.table_profile_z));

	dlg.table_profile = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach
		(dlg.table_profile, GTK_WIDGET(dlg.frame_profile_x), 0, 1, 1, 1);
	gtk_grid_attach
		(dlg.table_profile, GTK_WIDGET(dlg.frame_profile_y), 0, 2, 1, 1);
	gtk_grid_attach
		(dlg.table_profile, GTK_WIDGET(dlg.frame_profile_z), 0, 3, 1, 1);

	if (simulated)
	{
		char *string_profile[nstep];
		for (i = 0; i<nstep; ++i)
		{
			string_profile[i] = (char*)g_malloc(32*sizeof(char));
			snprintf(string_profile[i], 32, FGL, (JBDOUBLE)fmin(i*ti, tf));
		}
		dlg.combo_profile = jbw_combo_box_new_with_strings(string_profile, i);
		dlg.label_profile_time = (GtkLabel*)gtk_label_new(gettext("Time"));
		dlg.box_profile = (GtkGrid*)gtk_grid_new();
		gtk_grid_attach(dlg.box_profile, GTK_WIDGET(dlg.label_profile_time),
			0, 0, 1, 1);
		gtk_grid_attach(dlg.box_profile, GTK_WIDGET(dlg.combo_profile),
			1, 0, 1, 1);
		gtk_grid_attach(dlg.table_profile, GTK_WIDGET(dlg.box_profile),
			0, 0, 1, 1);
	}

	dlg.label_evolution_xmax = (GtkLabel*)gtk_label_new(gettext("Maximum"));
	dlg.entry_evolution_xmax = (JBWFloatEntry*)jbw_float_entry_new();

	dlg.label_evolution_xmin = (GtkLabel*)gtk_label_new(gettext("Minimum"));
	dlg.entry_evolution_xmin = (JBWFloatEntry*)jbw_float_entry_new();

	dlg.table_evolution_x = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach(dlg.table_evolution_x, GTK_WIDGET(dlg.label_evolution_xmax),
		0, 0, 1, 1);
	gtk_grid_attach(dlg.table_evolution_x, GTK_WIDGET(dlg.entry_evolution_xmax),
		1, 0, 2, 1);
	gtk_grid_attach(dlg.table_evolution_x, GTK_WIDGET(dlg.label_evolution_xmin),
		0, 1, 1, 1);
	gtk_grid_attach(dlg.table_evolution_x, GTK_WIDGET(dlg.entry_evolution_xmin),
		1, 1, 2, 1);

	dlg.frame_evolution_x = (GtkFrame*)gtk_frame_new(gettext("Time"));
	gtk_container_add(GTK_CONTAINER(dlg.frame_evolution_x),
		GTK_WIDGET(dlg.table_evolution_x));

	dlg.combo_evolution_y  =
		jbw_combo_box_new_with_strings(string_options, 8 + sys->nt);

	dlg.label_evolution_ymax = (GtkLabel*)gtk_label_new(gettext("Maximum"));
	dlg.entry_evolution_ymax = (JBWFloatEntry*)jbw_float_entry_new();

	dlg.label_evolution_ymin = (GtkLabel*)gtk_label_new(gettext("Minimum"));
	dlg.entry_evolution_ymin = (JBWFloatEntry*)jbw_float_entry_new();

	dlg.table_evolution_y = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach(dlg.table_evolution_y, GTK_WIDGET(dlg.combo_evolution_y),
		0, 0, 2, 1);
	gtk_grid_attach(dlg.table_evolution_y, GTK_WIDGET(dlg.label_evolution_ymax),
		0, 1, 1, 1);
	gtk_grid_attach(dlg.table_evolution_y, GTK_WIDGET(dlg.entry_evolution_ymax),
		1, 1, 1, 1);
	gtk_grid_attach(dlg.table_evolution_y, GTK_WIDGET(dlg.label_evolution_ymin),
		0, 2, 1, 1);
	gtk_grid_attach(dlg.table_evolution_y, GTK_WIDGET(dlg.entry_evolution_ymin),
		1, 2, 1, 1);

	dlg.frame_evolution_y = (GtkFrame*)gtk_frame_new(gettext("Parameter 1"));
	gtk_container_add(GTK_CONTAINER(dlg.frame_evolution_y),
		GTK_WIDGET(dlg.table_evolution_y));

	dlg.combo_evolution_z  =
		jbw_combo_box_new_with_strings(string_options, 8 + sys->nt);

	dlg.label_evolution_zmax = (GtkLabel*)gtk_label_new(gettext("Maximum"));
	dlg.entry_evolution_zmax = (JBWFloatEntry*)jbw_float_entry_new();

	dlg.label_evolution_zmin = (GtkLabel*)gtk_label_new(gettext("Minimum"));
	dlg.entry_evolution_zmin = (JBWFloatEntry*)jbw_float_entry_new();

	dlg.table_evolution_z = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach(dlg.table_evolution_z, GTK_WIDGET(dlg.combo_evolution_z),
		0, 0, 2, 1);
	gtk_grid_attach(dlg.table_evolution_z, GTK_WIDGET(dlg.label_evolution_zmax),
		0, 1, 1, 1);
	gtk_grid_attach(dlg.table_evolution_z, GTK_WIDGET(dlg.entry_evolution_zmax),
		1, 1, 1, 1);
	gtk_grid_attach(dlg.table_evolution_z, GTK_WIDGET(dlg.label_evolution_zmin),
		0, 2, 1, 1);
	gtk_grid_attach(dlg.table_evolution_z, GTK_WIDGET(dlg.entry_evolution_zmin),
		1, 2, 1, 1);

	dlg.frame_evolution_z = (GtkFrame*)gtk_frame_new(gettext("Parameter 2"));
	gtk_container_add(GTK_CONTAINER(dlg.frame_evolution_z),
		GTK_WIDGET(dlg.table_evolution_z));

	dlg.label_evolution_section =
		(GtkLabel*)gtk_label_new(gettext("Transient section"));

	dlg.combo_evolution_section = (GtkComboBoxText*)gtk_combo_box_text_new();
	dlg.combo_section = (GtkComboBoxText*)gtk_combo_box_text_new();
	dialog_graphical_options_sections(&dlg);

	dlg.table_evolution = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach(dlg.table_evolution,
		GTK_WIDGET(dlg.label_evolution_section), 0, 0, 1, 1);
	gtk_grid_attach(dlg.table_evolution,
		GTK_WIDGET(dlg.combo_evolution_section), 0, 1, 1, 1);
	gtk_grid_attach(dlg.table_evolution,
		GTK_WIDGET(dlg.frame_evolution_x), 0, 2, 1, 1);
	gtk_grid_attach(dlg.table_evolution,
		GTK_WIDGET(dlg.frame_evolution_y), 0, 3, 1, 1);
	gtk_grid_attach(dlg.table_evolution,
		GTK_WIDGET(dlg.frame_evolution_z), 0, 4, 1, 1);

	dlg.label_section_xmax = (GtkLabel*)gtk_label_new(gettext("Maximum"));
	dlg.entry_section_xmax = (JBWFloatEntry*)jbw_float_entry_new();

	dlg.label_section_xmin = (GtkLabel*)gtk_label_new(gettext("Minimum"));
	dlg.entry_section_xmin = (JBWFloatEntry*)jbw_float_entry_new();

	dlg.table_section_x = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach
		(dlg.table_section_x, GTK_WIDGET(dlg.label_section_xmax), 0, 0, 1, 1);
	gtk_grid_attach
		(dlg.table_section_x, GTK_WIDGET(dlg.entry_section_xmax), 1, 0, 1, 1);
	gtk_grid_attach
		(dlg.table_section_x, GTK_WIDGET(dlg.label_section_xmin), 0, 1, 1, 1);
	gtk_grid_attach
		(dlg.table_section_x, GTK_WIDGET(dlg.entry_section_xmin), 1, 1, 1, 1);

	dlg.frame_section_x = (GtkFrame*)gtk_frame_new(gettext("Position"));
	gtk_container_add(GTK_CONTAINER(dlg.frame_section_x),
		GTK_WIDGET(dlg.table_section_x));

	dlg.label_section_ymax = (GtkLabel*)gtk_label_new(gettext("Maximum"));
	dlg.entry_section_ymax = (JBWFloatEntry*)jbw_float_entry_new();

	dlg.label_section_ymin = (GtkLabel*)gtk_label_new(gettext("Minimum"));
	dlg.entry_section_ymin = (JBWFloatEntry*)jbw_float_entry_new();

	dlg.table_section_y = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach
		(dlg.table_section_y, GTK_WIDGET(dlg.label_section_ymax), 0, 0, 1, 1);
	gtk_grid_attach
		(dlg.table_section_y, GTK_WIDGET(dlg.entry_section_ymax), 1, 0, 1, 1);
	gtk_grid_attach
		(dlg.table_section_y, GTK_WIDGET(dlg.label_section_ymin), 0, 1, 1, 1);
	gtk_grid_attach
		(dlg.table_section_y, GTK_WIDGET(dlg.entry_section_ymin), 1, 1, 1, 1);

	dlg.frame_section_y = (GtkFrame*)gtk_frame_new(gettext("Level"));
	gtk_container_add
		(GTK_CONTAINER(dlg.frame_section_y), GTK_WIDGET(dlg.table_section_y));


	dlg.label_section_section =
		(GtkLabel*)gtk_label_new(gettext("Transient section"));

	dlg.table_section = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach
		(dlg.table_section, GTK_WIDGET(dlg.label_section_section), 0, 0, 1, 1);
	gtk_grid_attach
		(dlg.table_section, GTK_WIDGET(dlg.combo_section), 0, 1, 1, 1);
	gtk_grid_attach
		(dlg.table_section, GTK_WIDGET(dlg.frame_section_x), 0, 2, 1, 1);
	gtk_grid_attach
		(dlg.table_section, GTK_WIDGET(dlg.frame_section_y), 0, 3, 1, 1);

	dlg.label_profile
		= (GtkLabel*)gtk_label_new(gettext("Longitudinal profile"));
	dlg.label_evolution = (GtkLabel*)gtk_label_new(gettext("Time evolution"));
	dlg.label_section = (GtkLabel*)gtk_label_new(gettext("Transient section"));
	
	dlg.notebook = (GtkNotebook*)gtk_notebook_new();
	gtk_notebook_append_page(dlg.notebook,
		GTK_WIDGET(dlg.table_profile), GTK_WIDGET(dlg.label_profile));
	gtk_notebook_append_page(dlg.notebook,
		GTK_WIDGET(dlg.table_evolution), GTK_WIDGET(dlg.label_evolution));
	gtk_notebook_append_page(dlg.notebook,
		GTK_WIDGET(dlg.table_section), GTK_WIDGET(dlg.label_section));

	dlg.array_animation[0] = 0;
	dlg.box_animation = (GtkGrid*)gtk_grid_new();
	for (i = 0; i < 3; ++i)
	{
		dlg.array_animation[i] = (GtkRadioButton*)
			gtk_radio_button_new_with_mnemonic_from_widget
				(dlg.array_animation[0], string_animation[i]);
		gtk_grid_attach(dlg.box_animation, GTK_WIDGET(dlg.array_animation[i]),
			0, i, 1, 1);
	}

	dlg.frame_animation = (GtkFrame*)gtk_frame_new(gettext("Animation"));
	gtk_container_add(GTK_CONTAINER(dlg.frame_animation),
		GTK_WIDGET(dlg.box_animation));

	dlg.combo_channel  =
		jbw_combo_box_new_with_strings(string_channels, sys->n + 1);

	dlg.vbox = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach(dlg.vbox, GTK_WIDGET(dlg.combo_channel), 0, 0, 1, 1);
	gtk_grid_attach(dlg.vbox, GTK_WIDGET(dlg.frame_animation), 0, 1, 1, 1);
	gtk_grid_attach(dlg.vbox, GTK_WIDGET(dlg.button_adjust), 0, 2, 1, 1);
	gtk_grid_attach(dlg.vbox, GTK_WIDGET(dlg.button_grid), 0, 3, 1, 1);

	dlg.box = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach(dlg.box, GTK_WIDGET(dlg.notebook), 0, 0, 1, 1);
	gtk_grid_attach(dlg.box, GTK_WIDGET(dlg.vbox), 1, 0, 1, 1);

	dlg.window = (GtkDialog*)gtk_dialog_new_with_buttons(
		gettext("Graphical options"), parent, GTK_DIALOG_MODAL,
		gettext("_OK"), GTK_RESPONSE_OK,
		gettext("_Cancel"), GTK_RESPONSE_CANCEL,
		NULL);
	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(dlg.window)),
		GTK_WIDGET(dlg.box));
	gtk_widget_show_all(GTK_WIDGET(dlg.box));

	jbw_float_entry_set_value(dlg.entry_profile_xmax, profile_xmax);
	jbw_float_entry_set_value(dlg.entry_profile_xmin, profile_xmin);
	jbw_float_entry_set_value(dlg.entry_profile_ymax, profile_ymax);
	jbw_float_entry_set_value(dlg.entry_profile_ymin, profile_ymin);
	jbw_float_entry_set_value(dlg.entry_profile_zmax, profile_zmax);
	jbw_float_entry_set_value(dlg.entry_profile_zmin, profile_zmin);
	if (simulated)
		gtk_combo_box_set_active(GTK_COMBO_BOX(dlg.combo_profile), istep);
	gtk_combo_box_set_active
		(GTK_COMBO_BOX(dlg.combo_profile_y), profile_parameter1);
	gtk_combo_box_set_active
		(GTK_COMBO_BOX(dlg.combo_profile_z), profile_parameter2);

	jbw_float_entry_set_value(dlg.entry_evolution_xmax, evolution_xmax);
	jbw_float_entry_set_value(dlg.entry_evolution_xmin, evolution_xmin);
	jbw_float_entry_set_value(dlg.entry_evolution_ymax, evolution_ymax);
	jbw_float_entry_set_value(dlg.entry_evolution_ymin, evolution_ymin);
	jbw_float_entry_set_value(dlg.entry_evolution_zmax, evolution_zmax);
	jbw_float_entry_set_value(dlg.entry_evolution_zmin, evolution_zmin);
	gtk_combo_box_set_active
		(GTK_COMBO_BOX(dlg.combo_evolution_y), evolution_parameter1);
	gtk_combo_box_set_active
		(GTK_COMBO_BOX(dlg.combo_evolution_z), evolution_parameter2);
	gtk_combo_box_set_active
		(GTK_COMBO_BOX(dlg.combo_evolution_section), evolution_section);

	gtk_toggle_button_set_active
		((GtkToggleButton*)dlg.button_adjust, draw_adjust);

	gtk_toggle_button_set_active
		((GtkToggleButton*)dlg.button_grid, draw_grid);

	jbw_float_entry_set_value(dlg.entry_section_xmax, section_xmax);
	jbw_float_entry_set_value(dlg.entry_section_xmin, section_xmin);
	jbw_float_entry_set_value(dlg.entry_section_ymax, section_ymax);
	jbw_float_entry_set_value(dlg.entry_section_ymin, section_ymin);
	gtk_combo_box_set_active(GTK_COMBO_BOX(dlg.combo_section), section);

	jbw_array_radio_buttons_set_active(dlg.array_animation, type_animation, 1);
	gtk_notebook_set_current_page(dlg.notebook, type_draw);

	gtk_combo_box_set_active(GTK_COMBO_BOX(dlg.combo_channel), channel);

	dialog_graphical_options_actualize(&dlg);

	g_signal_connect_swapped(dlg.combo_channel, "changed",
		(void(*))dialog_graphical_options_channel, &dlg);

	if (gtk_dialog_run(dlg.window) == GTK_RESPONSE_OK)
	{
		if (simulated)
		{
			istep = gtk_combo_box_get_active(GTK_COMBO_BOX(dlg.combo_profile));
			t = fmin(t0+istep*ti, tf);
			dialog_simulator_actualize_bar(dialog_simulator);
		}
		channel = gtk_combo_box_get_active(GTK_COMBO_BOX(dlg.combo_channel));
		draw_adjust =
			gtk_toggle_button_get_active((GtkToggleButton*)dlg.button_adjust);
		draw_grid =
			gtk_toggle_button_get_active((GtkToggleButton*)dlg.button_grid);
		profile_parameter1 =
			gtk_combo_box_get_active(GTK_COMBO_BOX(dlg.combo_profile_y));
		profile_parameter2 =
			gtk_combo_box_get_active(GTK_COMBO_BOX(dlg.combo_profile_z));
		profile_xmin = jbw_float_entry_get_value(dlg.entry_profile_xmin);
		profile_xmax = jbw_float_entry_get_value(dlg.entry_profile_xmax);
		profile_ymin = jbw_float_entry_get_value(dlg.entry_profile_ymin);
		profile_ymax = jbw_float_entry_get_value(dlg.entry_profile_ymax);
		profile_zmin = jbw_float_entry_get_value(dlg.entry_profile_zmin);
		profile_zmax = jbw_float_entry_get_value(dlg.entry_profile_zmax);
		evolution_parameter1 =
			gtk_combo_box_get_active(GTK_COMBO_BOX(dlg.combo_evolution_y));
		evolution_parameter2 =
			gtk_combo_box_get_active(GTK_COMBO_BOX(dlg.combo_evolution_z));
		evolution_xmin = jbw_float_entry_get_value(dlg.entry_evolution_xmin);
		evolution_xmax = jbw_float_entry_get_value(dlg.entry_evolution_xmax);
		evolution_ymin = jbw_float_entry_get_value(dlg.entry_evolution_ymin);
		evolution_ymax = jbw_float_entry_get_value(dlg.entry_evolution_ymax);
		evolution_zmin = jbw_float_entry_get_value(dlg.entry_evolution_zmin);
		evolution_zmax = jbw_float_entry_get_value(dlg.entry_evolution_zmax);
		evolution_section = gtk_combo_box_get_active
			(GTK_COMBO_BOX(dlg.combo_evolution_section));
		section_xmin = jbw_float_entry_get_value(dlg.entry_section_xmin);
		section_xmax = jbw_float_entry_get_value(dlg.entry_section_xmax);
		section_ymin = jbw_float_entry_get_value(dlg.entry_section_ymin);
		section_ymax = jbw_float_entry_get_value(dlg.entry_section_ymax);
		section = gtk_combo_box_get_active(GTK_COMBO_BOX(dlg.combo_section));
		type_draw = gtk_notebook_get_current_page(dlg.notebook);
		type_animation
			= jbw_array_radio_buttons_get_active(dlg.array_animation);
		dialog_simulator_actualize(dialog_simulator);
	}
	gtk_widget_destroy(GTK_WIDGET(dlg.window));
	draw();
}
