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

#include "dialog_mass.h"

void table_mass_create(TableMass *table, Mass *m, JBDOUBLE e)
{
	char buffer[JB_BUFFER_SIZE];
	table->label_initial = (GtkLabel*)gtk_label_new(gettext("Initial mass"));
	table->label_final = (GtkLabel*)gtk_label_new(gettext("Final mass"));
	table->label_inlet =
		(GtkLabel*)gtk_label_new(gettext("Inlet contributions"));
	table->label_inner =
		(GtkLabel*)gtk_label_new(gettext("Inner contributions"));
	table->label_outlet =
		(GtkLabel*)gtk_label_new(gettext("Oulet contributions"));
	table->label_error =
		(GtkLabel*)gtk_label_new(gettext("Conservation error"));
	table->entry_initial = (GtkEntry*)gtk_entry_new();
	gtk_widget_set_sensitive(GTK_WIDGET(table->entry_initial), 0);
	jbw_float_entry_set_value(table->entry_initial, m->m0);
	table->entry_final = (GtkEntry*)gtk_entry_new();
	gtk_widget_set_sensitive(GTK_WIDGET(table->entry_final), 0);
	jbw_float_entry_set_value(table->entry_final, m->m);
	table->entry_inlet = (GtkEntry*)gtk_entry_new();
	gtk_widget_set_sensitive(GTK_WIDGET(table->entry_inlet), 0);
	jbw_float_entry_set_value(table->entry_inlet, m->inlet);
	table->entry_inner = (GtkEntry*)gtk_entry_new();
	gtk_widget_set_sensitive(GTK_WIDGET(table->entry_inner), 0);
	jbw_float_entry_set_value(table->entry_inner, m->inner);
	table->entry_outlet = (GtkEntry*)gtk_entry_new();
	gtk_widget_set_sensitive(GTK_WIDGET(table->entry_outlet), 0);
	jbw_float_entry_set_value(table->entry_outlet, m->outlet);
	table->entry_error = (GtkEntry*)gtk_entry_new();
	gtk_widget_set_sensitive(GTK_WIDGET(table->entry_error), 0);
	snprintf(buffer, JB_BUFFER_SIZE, FGL"%%", e);
	gtk_entry_set_text(table->entry_error, buffer);
	table->table = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach(table->table, GTK_WIDGET(table->label_initial),
		0, 0, 1, 1);
	gtk_grid_attach(table->table, GTK_WIDGET(table->entry_initial),
		1, 0, 1, 1);
	gtk_grid_attach(table->table, GTK_WIDGET(table->label_final),
		0, 1, 1, 1);
	gtk_grid_attach(table->table, GTK_WIDGET(table->entry_final),
		1, 1, 1, 1);
	gtk_grid_attach(table->table, GTK_WIDGET(table->label_inlet),
		0, 2, 1, 1);
	gtk_grid_attach(table->table, GTK_WIDGET(table->entry_inlet),
		1, 2, 1, 1);
	gtk_grid_attach(table->table, GTK_WIDGET(table->label_inner),
		0, 3, 1, 1);
	gtk_grid_attach(table->table, GTK_WIDGET(table->entry_inner),
		1, 3, 1, 1);
	gtk_grid_attach(table->table, GTK_WIDGET(table->label_outlet),
		0, 4, 1, 1);
	gtk_grid_attach(table->table, GTK_WIDGET(table->entry_outlet),
		1, 4, 1, 1);
	gtk_grid_attach(table->table, GTK_WIDGET(table->label_error),
		0, 5, 1, 1);
	gtk_grid_attach(table->table, GTK_WIDGET(table->entry_error),
		1, 5, 1, 1);
}

void dialog_mass_show(Mass *m, Parameters *p, TransportParameters *tp, int n,
	System *sys, GtkWindow *parent)
{
	int i;
	TableMass table[sys->nt + 2];
	DialogMass dlg[1];
	dlg->notebook = (GtkNotebook*)gtk_notebook_new();
	table_mass_create(table, m, flow_mass_error(m, p, n));
	gtk_notebook_append_page(dlg->notebook, GTK_WIDGET(table->table),
		gtk_label_new(gettext("Flow")));
	for (i = 0; i <= sys->nt; ++i, tp += n + 1)
	{
		++m;
		table_mass_create(table + i + 1, m, transport_mass_error(m, tp, n));
		gtk_notebook_append_page(dlg->notebook, GTK_WIDGET(table[i + 1].table),
			gtk_label_new(sys->transport[i].name));
	}
	gtk_widget_show_all(GTK_WIDGET(dlg->notebook));
	dlg->window = (GtkDialog*)gtk_dialog_new_with_buttons(
		gettext("Conservation error"), parent, GTK_DIALOG_MODAL,
		gettext("_OK"), GTK_RESPONSE_OK, NULL);
	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(dlg->window)),
		GTK_WIDGET(dlg->notebook));
	gtk_dialog_run(dlg->window);
	gtk_widget_destroy(GTK_WIDGET(dlg->window));
}
