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
 * \file editor_transport.c
 * \brief Source file to define transported solute editor structures and
 *   functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2015 Javier Burguete Tolosa.
 */
#include "editor_transport.h"

/**
 * \def EDITOR_TRANSPORT
 * \brief Macro to get the position of the transported solute editor in the
 *   notebook.
 * \def EDITOR_CHANNEL
 * \brief Macro to get the position of the channel editor in the notebook.
 * \def EDITOR_CROSS_SECTION
 * \brief Macro to get the position of the cross section editor in the notebook.
 * \def EDITOR_TRANSIENT_SECTION
 * \brief Macro to get the position of the transient section editor in the
 *   notebook.
 * \def EDITOR_INITIAL_FLOW
 * \brief Macro to get the position of the flow initial conditions editor in the
 *   notebook.
 */
#if TEST_EDITOR_TRANSPORT
	#define EDITOR_TRANSPORT 0
	char *message;
	EditorTransport editor[1];
#else
	#define EDITOR_TRANSPORT 1
#endif
#define EDITOR_CHANNEL (EDITOR_TRANSPORT + 1)
#define EDITOR_CROSS_SECTION (EDITOR_CHANNEL + 1)
#define EDITOR_TRANSIENT_SECTION (EDITOR_CROSS_SECTION + 1)
#define EDITOR_INITIAL_FLOW (EDITOR_TRANSIENT_SECTION + 1)

/**
 * \fn int editor_transport_get(EditorTransport *editor)
 * \brief Function to get the actual transported solute data from the editor.
 * \param editor
 * \brief transport editor.
 * \return 1 on valid transport, 0 on error.
 */
int editor_transport_get(EditorTransport *editor)
{
	Transport *transport = editor->transport;
	#if DEBUG_EDITOR_TRANSPORT_GET
		fprintf(stderr, "editor_transport_get: start\n");
	#endif
	g_free(transport->name);
	transport->name = g_strdup(gtk_entry_get_text(editor->entry_name));
	if (!transport->name)
	{
		transport_error(transport, gettext("Not enough memory"));
		#if DEBUG_EDITOR_TRANSPORT_GET
			fprintf(stderr, "editor_transport_get: end\n");
		#endif
		return 0;
	}
	transport->solubility = gtk_spin_button_get_value(editor->entry_solubility);
	transport->danger = gtk_spin_button_get_value(editor->entry_danger);
	#if DEBUG_EDITOR_TRANSPORT_GET
		fprintf(stderr, "editor_transport_get: end\n");
	#endif
	return 1;
}

/**
 * \fn void editor_transport_open(EditorTransport *editor)
 * \brief Function to open a transported solute in the editor.
 * \param editor
 * \brief transport editor.
 */
void editor_transport_open(EditorTransport *editor)
{
	Transport *transport;
	#if DEBUG_EDITOR_TRANSPORT_OPEN
		fprintf(stderr, "editor_transport_open: start\n");
	#endif
	transport = editor->transport;
	gtk_entry_set_text(editor->entry_name, transport->name);
	gtk_spin_button_set_value(editor->entry_solubility, transport->solubility);
	gtk_spin_button_set_value(editor->entry_danger, transport->danger);
	#if DEBUG_EDITOR_TRANSPORT_OPEN
		fprintf(stderr, "editor_transport_open: end\n");
	#endif
}

/**
 * \fn void editor_transport_destroy(EditorTransport *editor)
 * \brief Function to destroy a transported solute editor.
 * \param editor
 * \brief transport editor.
 */
void editor_transport_destroy(EditorTransport *editor)
{
	#if DEBUG_EDITOR_TRANSPORT_DESTROY
		fprintf(stderr, "editor_transport_destroy: start\n");
	#endif
	gtk_widget_destroy(GTK_WIDGET(editor->grid));
	transport_delete(editor->transport);
	#if DEBUG_EDITOR_TRANSPORT_DESTROY
		fprintf(stderr, "editor_transport_destroy: end\n");
	#endif
}

/**
 * \fn void editor_transport_new(EditorTransport *editor, \
 *   GtkNotebook *notebook)
 * \brief Function to create a new transport editor.
 * \param editor
 * \param notebook
 * \brief GtkNotebook to pack the widgets.
 */
void editor_transport_new(EditorTransport *editor, GtkNotebook *notebook)
{
	#if DEBUG_EDITOR_TRANSPORT_NEW
		fprintf(stderr, "editor_transport_new: start\n");
	#endif
	transport_init_empty(editor->transport);
	editor->notebook = notebook;
	editor->grid = (GtkGrid*)gtk_grid_new();
	gtk_notebook_append_page(notebook, GTK_WIDGET(editor->grid),
		gtk_label_new(gettext("Transport")));
	editor->label_name = (GtkLabel*)gtk_label_new(gettext("Name"));
	gtk_grid_attach(editor->grid, GTK_WIDGET(editor->label_name), 0, 0, 1, 1);
	editor->entry_name = (GtkEntry*)gtk_entry_new();
	gtk_grid_attach(editor->grid, GTK_WIDGET(editor->entry_name), 1, 0, 1, 1);
	editor->label_solubility = (GtkLabel*)gtk_label_new(gettext("Solubility"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->label_solubility), 0, 1, 1, 1);
	editor->entry_solubility
		= (GtkSpinButton*)gtk_spin_button_new_with_range(0., 1000., 1e-6);
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->entry_solubility), 1, 1, 1, 1);
	editor->label_danger = (GtkLabel*)gtk_label_new(gettext("Danger"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->label_danger), 0, 2, 1, 1);
	editor->entry_danger
		= (GtkSpinButton*)gtk_spin_button_new_with_range(0., 1000., 1e-9);
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->entry_danger), 1, 2, 1, 1);
	#if DEBUG_EDITOR_TRANSPORT_NEW
		fprintf(stderr, "editor_transport_new: end\n");
	#endif
}

#if TEST_EDITOR_TRANSPORT

void ok(char *name)
{
	xmlNode *node;
	xmlDoc *doc;
	editor_transport_get(editor);
	doc = xmlNewDoc((const xmlChar*)"1.0");
	node = xmlNewDocNode(doc, 0, XML_CHANNEL_TRANSPORT, 0);
	xmlDocSetRootElement(doc, node);
	transport_save_xml(editor->transport, node);
	xmlSaveFormatFile(name, doc, 1);
	xmlFree(doc);
	gtk_main_quit();
}

int main(int argn, char **argc)
{
	xmlNode *node;
	xmlDoc *doc;
	GtkNotebook *notebook;
	GtkButton *button_ok, *button_cancel;
	GtkDialog *dlg;
	xmlKeepBlanksDefault(0);
	if (!jbw_graphic_init(&argn, &argc)) return 1;
	notebook = (GtkNotebook*)gtk_notebook_new();
	editor_transport_new(editor, notebook);
	doc = xmlParseFile(argc[1]);
	if (!doc) return 2;
	node = xmlDocGetRootElement(doc);
	if (!transport_open_xml(editor->transport, node)) return 3;
	xmlFreeDoc(doc);
	dlg = (GtkDialog*)gtk_dialog_new();
	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(dlg)),
		GTK_WIDGET(notebook));
	gtk_window_set_title(GTK_WINDOW(dlg), "Test transported solute editor");
	button_ok = (GtkButton*)gtk_dialog_add_button
		(dlg, gettext("_OK"), GTK_RESPONSE_OK);
	g_signal_connect_swapped(button_ok, "clicked", (void(*))&ok, argc[2]);
	button_cancel = (GtkButton*)gtk_dialog_add_button
		(dlg, gettext("_Cancel"), GTK_RESPONSE_CANCEL);
	g_signal_connect(button_cancel, "clicked", &glutLeaveMainLoop, NULL);
	gtk_widget_show_all(GTK_WIDGET(dlg));
	editor_transport_open(editor);
	gtk_main();
	editor_transport_destroy(editor);
	gtk_widget_destroy(GTK_WIDGET(dlg));
	return 0;
}

#endif
