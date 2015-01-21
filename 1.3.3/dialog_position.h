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

#ifndef DIALOG_POSITION__H
#define DIALOG_POSITION__H 1

#include "jb/jb_win.h"

typedef struct
{
	GtkLabel *label;
	JBWIntEntry *entry;
	GtkGrid *box;
	GtkDialog *window;
} DialogPosition;

static inline int _dialog_position_new(GtkWindow *parent, char *action, int n)
{
	register int i;
	char buffer[JB_BUFFER_SIZE];
	DialogPosition dlg;
	snprintf(buffer, JB_BUFFER_SIZE, gettext("Position (1-%d)"), n);
	dlg.label = (GtkLabel*)gtk_label_new(buffer);
	dlg.entry = jbw_int_entry_new();
	dlg.box = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach(dlg.box, GTK_WIDGET(dlg.label), 0, 0, 1, 1);
	gtk_grid_attach(dlg.box, GTK_WIDGET(dlg.entry), 1, 0, 1, 1);
	dlg.window = (GtkDialog*)gtk_dialog_new_with_buttons(action,parent,
		GTK_DIALOG_MODAL, gettext("_OK"), GTK_RESPONSE_OK, NULL);
	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(dlg.window)),
		GTK_WIDGET(dlg.box));
	gtk_widget_show_all(GTK_WIDGET(dlg.box));
	gtk_dialog_run(dlg.window);
	i = jbw_int_entry_get_value(dlg.entry);
	if (i < 1 || i > n)
	{
		i = 0;
		jbw_show_error(gettext("Bad position"));
	}
	gtk_widget_destroy(GTK_WIDGET(dlg.window));
	return i;
}

#if INLINE_DIALOG_POSITION_NEW
	#define dialog_position_new _dialog_position_new
#else
	int dialog_position_new(GtkWindow*, char*, int);
#endif

#endif
