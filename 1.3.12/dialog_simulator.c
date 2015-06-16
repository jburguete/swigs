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

#define _GNU_SOURCE
#include <time.h>
#include "simulate.h"
//#include "editor_channel.h"
#include "dialog_system.h"
#include "dialog_graphical_options.h"
#include "dialog_save_solution.h"
#include "dialog_mass.h"
#include "dialog_save_graphical.h"
#include "draw.h"
#include "dialog_simulator.h"

void dialog_simulator_new_system()
{
	system_delete(sys);
	system_copy(sys, sysnew);
	dialog_simulator_edit();
}

void dialog_simulator_edit()
{
/*
	editor_channel_new(editor,channel);
*/
}

void dialog_simulator_save_as()
{
	char *name;
	char buffer[JB_BUFFER_SIZE];
	GtkFileChooserDialog *dlg;

	dlg = (GtkFileChooserDialog*)gtk_file_chooser_dialog_new(
		gettext("Save system as"), dialog_simulator->window,
		GTK_FILE_CHOOSER_ACTION_SAVE,
		gettext("_Cancel"), GTK_RESPONSE_CANCEL,
		gettext("_OK"), GTK_RESPONSE_OK,
		NULL);
	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dlg), 1);
	gtk_file_chooser_set_local_only(GTK_FILE_CHOOSER(dlg), 0);
	snprintf(buffer, JB_BUFFER_SIZE, "%s/%s", sys->directory, sys->name);
	if (!gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dlg), buffer))
	{
		gtk_file_chooser_set_current_folder
			(GTK_FILE_CHOOSER(dlg), sys->directory);
		gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dlg), sys->name);
	}
	window_parent = (GtkWindow*)dlg;
	if (gtk_dialog_run(GTK_DIALOG(dlg)) == GTK_RESPONSE_OK)
	{
		name = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dlg));
		jb_free_null((void**)&sys->name);
		sys->name = g_path_get_basename(name);
		jb_free_null((void**)&sys->directory);
		sys->directory = g_path_get_dirname(name);
		g_free(name);
		system_save_xml(sys);
	}
	gtk_widget_destroy(GTK_WIDGET(dlg));
	window_parent = dialog_simulator->window;
}

void dialog_simulator_start(DialogSimulator *dlg)
{
	dlg->time0 = time(0);
	simulate();
}

void dialog_simulator_stop()
{
	simulating = animating = 0;
}

void dialog_simulator_mass()
{
	dialog_mass_show(m, p, tp, n, sys, dialog_simulator->window);
}

void dialog_simulator_animate(DialogSimulator *dlg)
{
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->item_graphic[3]), 1);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->button[16]), 1);
	draw_animation();
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->item_graphic[3]), 0);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->button[16]), 0);
}	

void dialog_simulator_about(DialogSimulator *dlg)
{
	gchar *authors[] =
	{
		"Javier Burguete Tolosa (jburguete@eead.csic.es)",
		NULL
	};
	gtk_show_about_dialog(dlg->window,
		"program_name",
		"SWIGS",
		"comments",
		gettext("SWIGS (Shallow Water in Irregular Geometries Simulator): "
		"a software to simulate transient or steady flows with solute "
		"transport in channels, channel networks and rivers"),
		"authors",
		authors,
		"version",
		"1.3.12",
		"copyright",
		"Copyright 2011-2015 Javier Burguete Tolosa",
		"logo",
		dlg->pixbuf,
		"website-label",
		"Website",
		"website",
		"https://github.com/jburguete/swigs",
		NULL);
}

void dialog_simulator_actualize(DialogSimulator *dlg)
{
	register int i;
	char *array_options1[sys->nt + 8], *array_options2[sys->nt + 8];

	jbw_graphic_set_title(dlg->graphic, sys->name);

	string_graphical_options(array_options1, sys);

	array_options2[0] = array_options2[1] = NULL;
	array_options2[2] = gettext("Bed level");
	array_options2[3] = gettext("Critical velocity");
	for (i = 0; i <= sys->nt + 3; ++i) array_options2[4 + i] = NULL;

	if (channel > sys->n) channel = 0;
	if (channel_write > sys->n) channel_write = 0;

	i = sys->nt + 7;
	if (profile_parameter1 > i) profile_parameter1 = 0;
	if (profile_parameter2 > i) profile_parameter2 = 1;
	if (evolution_parameter1 > i) evolution_parameter1 = 0;
	if (evolution_parameter2 > i) evolution_parameter2 = 1;

	jbw_graphic_set_xlabel(dlg->graphic, gettext("Position"));
	switch (type_draw)
	{
	case 0:
		jbw_graphic_set_ylabel
			(dlg->graphic, array_options1[profile_parameter1]);
		jbw_graphic_set_yylabel
			(dlg->graphic, array_options2[profile_parameter1]);
		jbw_graphic_set_zlabel
			(dlg->graphic, array_options1[profile_parameter2]);
		jbw_graphic_set_zzlabel
			(dlg->graphic, array_options2[profile_parameter2]);
		dlg->graphic->xmin = profile_xmin;
		dlg->graphic->xmax = profile_xmax;
		dlg->graphic->ymin = profile_ymin;
		dlg->graphic->ymax = profile_ymax;
		dlg->graphic->zmin = profile_zmin;
		dlg->graphic->zmax = profile_zmax;
		break;
	case 1:
		jbw_graphic_set_xlabel(dlg->graphic, gettext("Time"));
		jbw_graphic_set_ylabel
			(dlg->graphic,array_options1[evolution_parameter1]);
		jbw_graphic_set_zlabel
			(dlg->graphic,array_options1[evolution_parameter2]);
		jb_free_null((void**)&dlg->graphic->str_yy);
		jb_free_null((void**)&dlg->graphic->str_zz);
		dlg->graphic->xmin = evolution_xmin;
		dlg->graphic->xmax = evolution_xmax;
		dlg->graphic->ymin = evolution_ymin;
		dlg->graphic->ymax = evolution_ymax;
		dlg->graphic->zmin = evolution_zmin;
		dlg->graphic->zmax = evolution_zmax;
		break;
	default:
		jbw_graphic_set_ylabel(dlg->graphic, gettext("Level"));
		jbw_graphic_set_yylabel(dlg->graphic, gettext("Bed level"));
		jb_free_null((void**)&dlg->graphic->str_z);
		jb_free_null((void**)&dlg->graphic->str_zz);
		dlg->graphic->xmin = section_xmin;
		dlg->graphic->xmax = section_xmax;
		dlg->graphic->ymin = section_ymin;
		dlg->graphic->ymax = section_ymax;
		dlg->graphic->zmin = dlg->graphic->zmax = 0.;
	}
	dlg->graphic->resize = !draw_adjust;
#if JBW_GRAPHIC==JBW_GRAPHIC_GLUT
	glutPostRedisplay();
#endif
}

void dialog_simulator_actualize_bar(DialogSimulator *dlg)
{
	register JBDOUBLE k;
	k = tf - t0;
	if (k > 0.) k = fmin(t - t0, k) / k; else k = 1.;
	gtk_progress_bar_set_fraction(dlg->progress_simulation, k);
	snprintf(dlg->string_progress, 32, FGL" s", t - t0);
	gtk_progress_bar_set_text(dlg->progress_simulation, dlg->string_progress);
	if (simulating) jbw_int_entry_set_value_with_format(dlg->entry_cpu, "%ld s",
		time(0) - dlg->time0);
	jbw_float_entry_set_value_with_format
		(dlg->entry_error, FGL" %%", flow_mass_error(m, p, n));
	gtk_widget_queue_draw(GTK_WIDGET(dlg->box_bar));
}

void dialog_simulator_actualize_menu(DialogSimulator *dlg, int active)
{
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->item_system[0]), active);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->item_system[1]), active);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->item_system[2]), active);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->item_simulate[0]), active);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->button[2]), active);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->button[3]), active);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->button[4]), active);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->button[8]), active);
	active = !active;
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->item_simulate[1]), active);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->button[9]), active);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->item_simulate[3]), simulated);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->button[10]), simulated);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->item_simulate[5]), simulated);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->button[11]), simulated);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->item_graphic[2]), simulated);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->button[15]), simulated);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->item_graphic[3]), 0);
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->button[16]), 0);
}

DialogSimulator* dialog_simulator_new()
{
	register int i;
	register DialogSimulator* dlg;
	char *str_item_bar[N_DIALOG_SIMULATOR_ITEM_BAR] = {gettext("_Channel"),
		gettext("_Simulate"), gettext("_Graphical"), gettext("_Help")},
	*str_item_system[N_DIALOG_SIMULATOR_ITEM_SYSTEM] = {gettext("_New"),
		gettext("_Open"), gettext("_Edit"), gettext("_Save"),
		gettext("S_ave as"), NULL, gettext("E_xit")},
	*str_item_simulate[N_DIALOG_SIMULATOR_ITEM_SIMULATE] =
		{gettext("_Start"),gettext("Sto_p"), NULL,
		gettext("Conse_rvation error"), NULL,
		gettext("Sa_ve solution"), gettext("Save _graphical")},
	*str_item_graphic[N_DIALOG_SIMULATOR_ITEM_GRAPHIC] = {gettext("_Options"),
		NULL, gettext("_View animation"), gettext("_Stop animation")},
	*str_item_help[N_DIALOG_SIMULATOR_ITEM_HELP] = {gettext("_About")},
	*str_buttons[N_DIALOG_SIMULATOR_BUTTONS] = {gettext("Exit"), NULL,
		gettext("New system"), gettext("Open system"), gettext("Edit system"),
		gettext("Save system"), gettext("Save system as"), NULL,
		gettext("Start simulation"), gettext("Stop simulation"),
		gettext("Conservation error"), gettext("Save solution"),
		gettext("Save graphical"), NULL,
		gettext("Graphical options"), gettext("View animation"),
		gettext("Stop animation"), NULL, gettext("Help")},
	*image_buttons[N_DIALOG_SIMULATOR_BUTTONS] = {"application-exit", NULL,
		"document-new", "document-open", "document-properties",
		"document-save", "document-save-as", NULL,
		"system-run", "process-stop",
		"dialog-information", "document-revert",
		"document-print", NULL,
		"preferences-system", "call-start",
		"call-stop", NULL, "help-about"}; 

	dlg = (DialogSimulator*)g_try_malloc(sizeof(DialogSimulator));
	if (!dlg) return 0;

	dlg->menu = (GtkMenuBar*)gtk_menu_bar_new();
	for (i = 0; i < N_DIALOG_SIMULATOR_ITEM_BAR; ++i)
	{
		if (str_item_bar[i])
			dlg->item_bar[i] = (GtkMenuItem*)gtk_menu_item_new_with_mnemonic
				(str_item_bar[i]);
		else dlg->item_bar[i] = (GtkMenuItem*)gtk_separator_menu_item_new();
		gtk_menu_shell_append(GTK_MENU_SHELL(dlg->menu),
			GTK_WIDGET(dlg->item_bar[i]));
	}

	dlg->menu_system = (GtkMenu*)gtk_menu_new();
	for (i = 0; i < N_DIALOG_SIMULATOR_ITEM_SYSTEM; ++i)
	{
		if (str_item_system[i])
			dlg->item_system[i] = (GtkMenuItem*)gtk_menu_item_new_with_mnemonic
				(str_item_system[i]);
		else dlg->item_system[i] = (GtkMenuItem*)gtk_separator_menu_item_new();
		gtk_menu_shell_append(GTK_MENU_SHELL(dlg->menu_system),
			GTK_WIDGET(dlg->item_system[i]));
	}
	gtk_menu_item_set_submenu(dlg->item_bar[0], GTK_WIDGET(dlg->menu_system));

	dlg->menu_simulate = (GtkMenu*)gtk_menu_new();
	for (i = 0; i < N_DIALOG_SIMULATOR_ITEM_SIMULATE; ++i)
	{
		if (str_item_simulate[i])
			dlg->item_simulate[i] = (GtkMenuItem*)
				gtk_menu_item_new_with_mnemonic(str_item_simulate[i]);
		else dlg->item_simulate[i]
			= (GtkMenuItem*)gtk_separator_menu_item_new();
		gtk_menu_shell_append(GTK_MENU_SHELL(dlg->menu_simulate),
			GTK_WIDGET(dlg->item_simulate[i]));
	}
	gtk_menu_item_set_submenu(dlg->item_bar[1], GTK_WIDGET(dlg->menu_simulate));

	dlg->menu_graphic = (GtkMenu*)gtk_menu_new();
	for (i = 0; i < N_DIALOG_SIMULATOR_ITEM_GRAPHIC; ++i)
	{
		if (str_item_graphic[i])
			dlg->item_graphic[i] = (GtkMenuItem*)
				gtk_menu_item_new_with_mnemonic(str_item_graphic[i]);
		else dlg->item_graphic[i] = (GtkMenuItem*)gtk_separator_menu_item_new();
		gtk_menu_shell_append(GTK_MENU_SHELL(dlg->menu_graphic),
			GTK_WIDGET(dlg->item_graphic[i]));
	}
	gtk_menu_item_set_submenu(dlg->item_bar[2], GTK_WIDGET(dlg->menu_graphic));

	dlg->menu_help = (GtkMenu*)gtk_menu_new();
	for (i=0; i<N_DIALOG_SIMULATOR_ITEM_HELP; ++i)
	{
		if (str_item_help[i])
			dlg->item_help[i] = (GtkMenuItem*)gtk_menu_item_new_with_mnemonic
				(str_item_help[i]);
		else dlg->item_help[i] = (GtkMenuItem*)gtk_separator_menu_item_new();
		gtk_menu_shell_append(GTK_MENU_SHELL(dlg->menu_help),
			GTK_WIDGET(dlg->item_help[i]));
	}
	gtk_menu_item_set_submenu(dlg->item_bar[3], GTK_WIDGET(dlg->menu_help));

	dlg->toolbar = (GtkToolbar*)gtk_toolbar_new();
	for (i = 0; i < N_DIALOG_SIMULATOR_BUTTONS; ++i)
	{
		if (str_buttons[i])
		{
			dlg->button[i] = (GtkToolItem*)gtk_tool_button_new
				(
					gtk_image_new_from_icon_name
						(image_buttons[i], GTK_ICON_SIZE_SMALL_TOOLBAR),
					str_buttons[i]
				);
			gtk_widget_set_tooltip_text
				(GTK_WIDGET(dlg->button[i]), str_buttons[i]);
		}
		else dlg->button[i] = (GtkToolItem*)gtk_separator_tool_item_new();
		gtk_toolbar_insert(dlg->toolbar, dlg->button[i], -1);
	}

	dlg->graphic = jbw_graphic_new(0, 6, 6, 6, draw_graphic);
	jbw_graphic_set_logo(dlg->graphic, "swigs.png");
	dlg->label_simulation
		= (GtkLabel*)gtk_label_new(gettext("Simulation time"));
	dlg->progress_simulation = (GtkProgressBar*)gtk_progress_bar_new();

	dlg->label_cpu = (GtkLabel*)gtk_label_new(gettext("Computational time"));
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->label_cpu), 0);
	dlg->entry_cpu = (JBWFloatEntry*)jbw_float_entry_new();

	dlg->label_error = (GtkLabel*)gtk_label_new(gettext("Conservation error"));
	dlg->entry_error = (JBWFloatEntry*)jbw_float_entry_new();

	dlg->box_bar = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach(dlg->box_bar, GTK_WIDGET(dlg->label_simulation),
		0, 0, 1, 1);
	gtk_grid_attach(dlg->box_bar, GTK_WIDGET(dlg->progress_simulation),
		1, 0, 1, 1);
	gtk_grid_attach(dlg->box_bar, GTK_WIDGET(dlg->label_cpu),
		2, 0, 1, 1);
	gtk_grid_attach(dlg->box_bar, GTK_WIDGET(dlg->entry_cpu),
		3, 0, 1, 1);
	gtk_grid_attach(dlg->box_bar, GTK_WIDGET(dlg->label_error),
		4, 0, 1, 1);
	gtk_grid_attach(dlg->box_bar, GTK_WIDGET(dlg->entry_error),
		5, 0, 1, 1);

	dlg->button_bar = (GtkButton*)gtk_button_new();
	gtk_container_add(GTK_CONTAINER(dlg->button_bar), GTK_WIDGET(dlg->box_bar));
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->button_bar), 0);

	dlg->box = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach(dlg->box, GTK_WIDGET(dlg->menu), 0, 0, 1, 1);
	gtk_grid_attach(dlg->box, GTK_WIDGET(dlg->toolbar), 0, 1, 1, 1);
	gtk_grid_attach(dlg->box, GTK_WIDGET(dlg->button_bar), 0, 2, 1, 1);

	dlg->pixbuf = gtk_image_get_pixbuf
		(GTK_IMAGE(gtk_image_new_from_file("logo.png")));

	dlg->window = (GtkWindow*)gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(dlg->window, "SWIGS");
	gtk_window_set_icon(dlg->window, dlg->pixbuf);
	gtk_container_add(GTK_CONTAINER(dlg->window), GTK_WIDGET(dlg->box));
	gtk_widget_show_all(GTK_WIDGET(dlg->window));
	#if JBW_GRAPHIC==JBW_GRAPHIC_GLUT
		g_signal_connect(dlg->window, "destroy", glutLeaveMainLoop, NULL);
	#else
		g_signal_connect(dlg->window, "destroy", gtk_main_quit, NULL);
	#endif

	g_signal_connect
		(dlg->item_system[0], "activate", dialog_simulator_new_system, 0);
	g_signal_connect_swapped(dlg->item_system[1], "activate",
		(void(*))dialog_open_system_new, dialog_open_system);
	g_signal_connect(dlg->item_system[2], "activate", dialog_simulator_edit, 0);
	g_signal_connect_swapped
		(dlg->item_system[3], "activate", (void(*))system_save_xml, sys);
	g_signal_connect
		(dlg->item_system[4], "activate", dialog_simulator_save_as, 0);
	g_signal_connect(dlg->item_system[6], "activate", (void(*))exit, 0);
	g_signal_connect_swapped(dlg->item_simulate[0], "activate",
		(void(*))dialog_simulator_start, dlg);
	g_signal_connect
		(dlg->item_simulate[1], "activate", dialog_simulator_stop, 0);
	g_signal_connect
		(dlg->item_simulate[3], "activate", dialog_simulator_mass, 0);
	g_signal_connect
		(dlg->item_simulate[5], "activate", dialog_save_solution_new, 0);
	g_signal_connect
		(dlg->item_simulate[6], "activate", dialog_save_graphical_new, 0);
	g_signal_connect_swapped(dlg->item_graphic[0], "activate",
		(void(*))dialog_graphical_options_new, dlg->window);
	g_signal_connect_swapped(dlg->item_graphic[2], "activate",
		(void*)dialog_simulator_animate, dlg);
	g_signal_connect
		(dlg->item_graphic[3], "activate", dialog_simulator_stop, 0);
	g_signal_connect_swapped
		(dlg->item_help[0], "activate", (void(*))dialog_simulator_about, dlg);

	g_signal_connect(dlg->button[0], "clicked", (void(*))exit, 0);
	g_signal_connect(dlg->button[2], "clicked", dialog_simulator_new_system, 0);
	g_signal_connect_swapped(dlg->button[3], "clicked",
		(void(*))dialog_open_system_new, dialog_open_system);
	g_signal_connect(dlg->button[4], "clicked", dialog_simulator_edit, 0);
	g_signal_connect_swapped
		(dlg->button[5], "clicked", (void(*))system_save_xml, sys);
	g_signal_connect(dlg->button[6], "clicked", dialog_simulator_save_as, 0);
	g_signal_connect_swapped
		(dlg->button[8], "clicked", (void(*))dialog_simulator_start, dlg);
	g_signal_connect(dlg->button[9], "clicked", dialog_simulator_stop, 0);
	g_signal_connect(dlg->button[10], "clicked", dialog_simulator_mass, 0);
	g_signal_connect(dlg->button[11], "clicked", dialog_save_solution_new, 0);
	g_signal_connect(dlg->button[12], "clicked", dialog_save_graphical_new, 0);
	g_signal_connect_swapped(dlg->button[14], "clicked",
		(void(*))dialog_graphical_options_new, dlg->window);
	g_signal_connect_swapped
		(dlg->button[15], "clicked", (void*)dialog_simulator_animate, dlg);
	g_signal_connect(dlg->button[16], "clicked", dialog_simulator_stop, 0);
	g_signal_connect_swapped
		(dlg->button[18], "clicked", (void(*))dialog_simulator_about, dlg);

	dialog_simulator_actualize(dlg);

	window_parent = dlg->window;

	return dlg;
}
