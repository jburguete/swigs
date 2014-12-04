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

#include "simulate.h"
#include "dialog_simulator.h"
#include "dialog_system.h"

void dialog_open_system_new(DialogOpenSystem *dlg)
{
	int i;
	char *name;
	GtkFileFilter *filter;
	System s[1];

	filter = gtk_file_filter_new();
	gtk_file_filter_add_pattern(filter, "*.xml");
	gtk_file_filter_add_pattern(filter, "*.XML");
	gtk_file_filter_set_name
		(filter, gettext("XML system file (*.xml, *.XML)"));
	dlg->button_open = (GtkFileChooserButton*)gtk_file_chooser_button_new
		(gettext("Open system"), GTK_FILE_CHOOSER_ACTION_OPEN);
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dlg->button_open), filter);
	gtk_file_chooser_set_filename
		(GTK_FILE_CHOOSER(dlg->button_open), string_system);

	dlg->window=(GtkDialog*)gtk_dialog_new_with_buttons(gettext("Open system"),
		dialog_simulator->window, GTK_DIALOG_MODAL,
		gettext("_OK"), GTK_RESPONSE_OK,
		gettext("_Cancel"), GTK_RESPONSE_CANCEL,
		NULL);
	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(dlg->window)),
		GTK_WIDGET(dlg->button_open));
	gtk_widget_show(GTK_WIDGET(dlg->button_open));

	window_parent = (GtkWindow*)dlg->window;

	do
	{
		i = gtk_dialog_run(dlg->window);
		if (i == GTK_RESPONSE_OK)
		{
			name = gtk_file_chooser_get_filename
				(GTK_FILE_CHOOSER(dlg->button_open));
			if (name)
			{
				system_init(s);
				i = system_open_xml(s, g_path_get_basename(name),
					g_path_get_dirname(name));
			}
			else
			{
				i = 0;
				jbw_show_error2(gettext("Open system"),
					gettext("Unable to open the file"));
			}
			if (i)
			{
				g_free(string_system);
				string_system = jb_strdup(name);
				g_free(name);
				system_delete(sys);
				if (!system_copy(sys, s))
				{
					jbw_show_error(message);
					exit(0);
				}
				system_delete(s);
				if (!simulate_start()) continue;
				dialog_simulator_actualize_bar(dialog_simulator);
				dialog_simulator_actualize(dialog_simulator);
				break;
			}
			else g_free(name);
		}
		else break;
	}
	while (1);
	gtk_widget_destroy(GTK_WIDGET(dlg->window));

	window_parent = dialog_simulator->window;
}
