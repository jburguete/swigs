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
 * \file editor_initial_transport.c
 * \brief Source file to define transported solute initial conditions editor
 *   structures and functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2014 Javier Burguete Tolosa.
 */
#include "editor_initial_transport.h"

#if TEST_EDITOR_INITIAL_TRANSPORT
	char *message;
	EditorInitialTransport editor[1];
	void editor_draw() {editor_initial_transport_draw(editor);}
#endif

/**
 * \fn void editor_initial_transport_update(EditorInitialTransport *editor)
 * \brief Function to update the view of a transported solute initial conditions
 *   editor.
 * \param editor
 * \brief transported solute initial conditions editor.
 */
void editor_initial_transport_update(EditorInitialTransport *editor)
{
	#if DEBUG_EDITOR_INITIAL_TRANSPORT_UPDATE
		fprintf(stderr, "editor_initial_transport_update: start\n");
	#endif
	switch (jbw_array_radio_buttons_get_active(editor->array_type))
	{
		case INITIAL_TRANSPORT_TYPE_XC:
			if (editor->array->n < 1)
				jbw_array_editor_set_rows(editor->array, 1);
			jbw_array_editor_set_title
				(editor->array, 1, gettext("Solute concentration"));
			jbw_graphic_set_ylabel
				(editor->graphic, gettext("Solute concentration"));
			gtk_widget_show(GTK_WIDGET(editor->grid_parameters));
			break;
		default:
			gtk_widget_hide(GTK_WIDGET(editor->grid_parameters));
	}
	gtk_widget_set_sensitive
		(GTK_WIDGET(editor->button_remove), editor->array->n > 1);
	#if DEBUG_EDITOR_INITIAL_TRANSPORT_UPDATE
		fprintf(stderr, "editor_initial_transport_update: end\n");
	#endif
}

/**
 * \fn void editor_initial_transport_get(EditorInitialTransport *editor)
 * \brief Function to get the actual transported solute initial conditions data
 *   from the editor.
 * \param editor
 * \brief transported solute initial conditions editor.
 */
void editor_initial_transport_get(EditorInitialTransport *editor)
{
	int i, j = editor->array->n;
	JBFLOAT x[j], p[j];
	InitialTransport *it = editor->it;
	#if DEBUG_EDITOR_INITIAL_TRANSPORT_GET
		fprintf(stderr, "editor_initial_transport_get: start\n");
	#endif
	initial_transport_delete(it);
	it->type = jbw_array_radio_buttons_get_active(editor->array_type);
	#if DEBUG_EDITOR_INITIAL_TRANSPORT_GET
		fprintf(stderr, "EIFG: type=%d\n", it->type);
	#endif
	switch (it->type)
	{
		case INITIAL_TRANSPORT_TYPE_DRY:
		case INITIAL_TRANSPORT_TYPE_STEADY:
			break;
		default:
			#if DEBUG_EDITOR_INITIAL_TRANSPORT_GET
				fprintf(stderr, "EIFG: rows=%d\n", j);
			#endif
			it->p = g_realloc(it->p, 2 * j * sizeof(JBFLOAT));
			#if DEBUG_EDITOR_INITIAL_TRANSPORT_GET
				fprintf(stderr, "EIFG: getting columns\n");
			#endif
			jbw_array_editor_get_column_float(editor->array, 0, x);
			jbw_array_editor_get_column_float(editor->array, 1, p);
			#if DEBUG_EDITOR_INITIAL_TRANSPORT_GET
				fprintf(stderr, "EIFG: setting points\n");
			#endif
			for (i = j; --i >= 0;) point2_open(it->p + i, x[i], p[i]);
	}
	it->n = --j;
	#if DEBUG_EDITOR_INITIAL_TRANSPORT_GET
		initial_transport_print(it, stderr);
		fprintf(stderr, "editor_initial_transport_get: end\n");
	#endif
}

/**
 * \fn void editor_initial_transport_open(EditorInitialTransport *editor)
 * \brief Function to open transported solute initial conditions in the editor.
 * \param editor
 * \brief transported solute initial conditions editor.
 */
void editor_initial_transport_open(EditorInitialTransport *editor)
{
	InitialTransport *it = editor->it;
	int i, n = it->n + 1;
	JBFLOAT x[n], p[n];
	#if DEBUG_EDITOR_INITIAL_TRANSPORT_OPEN
		fprintf(stderr, "editor_initial_transport_open: start\n");
		initial_transport_print(it, stderr);
	#endif
	jbw_array_radio_buttons_set_active(editor->array_type, it->type, 1);
	switch (it->type)
	{
		case INITIAL_TRANSPORT_TYPE_DRY:
		case INITIAL_TRANSPORT_TYPE_STEADY:
			jbw_array_editor_set_rows(editor->array, 0);
			break;
		default:
			for (i = n; --i >= 0;)
			{
				x[i] = it->p[i].x;
				p[i] = it->p[i].y;
			}
			jbw_array_editor_set_column_float(editor->array, 0, x);
			jbw_array_editor_set_column_float(editor->array, 1, p);
	}
	editor_initial_transport_update(editor);
	#if DEBUG_EDITOR_INITIAL_TRANSPORT_OPEN
		fprintf(stderr, "editor_initial_transport_open: end\n");
	#endif
}

/**
 * \fn void editor_initial_transport_insert_point \
 *   (EditorInitialTransport *editor)
 * \brief Function to insert a point in an transported solute initial conditions
 *   editor.
 * \param editor
 * \brief transported solute initial conditions editor.
 */
void editor_initial_transport_insert_point(EditorInitialTransport *editor)
{
	#if DEBUG_EDITOR_INITIAL_TRANSPORT_INSERT_POINT
		fprintf(stderr, "editor_initial_transport_insert_point: start\n");
	#endif
	jbw_array_editor_insert(editor->array);
	editor_initial_transport_update(editor);
	#if DEBUG_EDITOR_INITIAL_TRANSPORT_INSERT_POINT
		fprintf(stderr, "editor_initial_transport_insert_point: end\n");
	#endif
}

/**
 * \fn void editor_initial_transport_remove_point \
 *   (EditorInitialTransport *editor)
 * \brief Function to remove a point in a transported solute initial conditions
 *   editor.
 * \param editor
 * \brief transported solute initial conditions editor.
 */
void editor_initial_transport_remove_point(EditorInitialTransport *editor)
{
	#if DEBUG_EDITOR_INITIAL_TRANSPORT_REMOVE_POINT
		fprintf(stderr, "editor_initial_transport_remove_point: start\n");
	#endif
	jbw_array_editor_remove(editor->array);
	editor_initial_transport_update(editor);
	#if DEBUG_EDITOR_INITIAL_TRANSPORT_REMOVE_POINT
		fprintf(stderr, "editor_initial_transport_remove_point: end\n");
	#endif
}

/**
 * \fn void editor_initial_transport_draw(EditorInitialTransport *editor)
 * \brief Function to draw a transported solute initial conditions in a
 *  transported solute initial conditions editor.
 * \param editor
 * \brief transported solute initial conditions editor.
 */
void editor_initial_transport_draw(EditorInitialTransport *editor)
{
	#if DEBUG_EDITOR_INITIAL_TRANSPORT_DRAW
		fprintf(stderr, "editor_initial_transport_draw: start\n");
	#endif
	editor_initial_transport_get(editor);
	InitialTransport *it = editor->it;
	int i, n = it->n + 1;
	JBFLOAT x[n], y[n];
	for (i = n; --i >= 0;) x[i] = it->p[i].x, y[i] = it->p[i].y;
	jbw_graphic_draw_lines(editor->graphic, x, y, NULL, NULL, NULL, n);
	jbw_graphic_draw_logo(editor->graphic);
#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
	glutSwapBuffers();
#elif JBW_GRAPHIC == JBW_GRAPHIC_CLUTTER
	glFlush();
#endif
	#if DEBUG_EDITOR_INITIAL_TRANSPORT_DRAW
		fprintf(stderr, "editor_initial_transport_draw: end\n");
	#endif
}

/**
 * \fn int editor_initial_transport_check(EditorInitialTransport *editor)
 * \brief Function to check if the transport initial conditions are correct.
 * \param editor
 * \brief transported solute initial conditions editor.
 * \return 1 on correct transport initial conditions, 0 on error.
 */
int editor_initial_transport_check(EditorInitialTransport *editor)
{
	InitialTransport *it;
	#if DEBUG_EDITOR_INITIAL_TRANSPORT_CHECK
		fprintf(stderr, "editor_initial_transport_check: start\n");
	#endif
	editor_initial_transport_get(editor);
	it = editor->it;
	switch (it->type)
	{
		case INITIAL_TRANSPORT_TYPE_DRY:
		case INITIAL_TRANSPORT_TYPE_STEADY:
			break;
		default:
			if (!initial_transport_check(it)) goto check_error;
	}
	#if DEBUG_EDITOR_INITIAL_TRANSPORT_CHECK
		fprintf(stderr, "editor_initial_transport_check: end\n");
	#endif
	return 1;

check_error:
	#if DEBUG_EDITOR_INITIAL_TRANSPORT_CHECK
		fprintf(stderr, "editor_initial_transport_check: end\n");
	#endif
	return 0;
}

/**
 * \fn void editor_initial_transport_destroy(EditorInitialTransport *editor)
 * \brief Function to destroy a transported solute initial conditions editor.
 * \param editor
 * \brief transported solute initial conditions editor.
 */
void editor_initial_transport_destroy(EditorInitialTransport *editor)
{
	#if DEBUG_EDITOR_INITIAL_TRANSPORT_DESTROY
		fprintf(stderr, "editor_initial_transport_destroy: start\n");
	#endif
	gtk_widget_destroy(GTK_WIDGET(editor->grid));
	initial_transport_delete(editor->it);
	#if DEBUG_EDITOR_INITIAL_TRANSPORT_DESTROY
		fprintf(stderr, "editor_initial_transport_destroy: end\n");
	#endif
}

/**
 * \fn void editor_initial_transport_new(EditorInitialTransport *editor, \
 *   GtkNotebook *notebook)
 * \brief Function to create a new transported solute initial conditions editor.
 * \param editor
 * \brief transported solute initial conditions editor.
 * \param notebook
 * \brief GtkNotebook to pack the widgets.
 */
void editor_initial_transport_new(EditorInitialTransport *editor, GtkNotebook *notebook)
{
	int i;
	const char *label_type[N_INITIAL_TRANSPORT_TYPES] =
		{gettext("Dry"), gettext("Steady"),
		gettext("Solute concentration profile")};
	const char *label_array[2]
		= {gettext("Position"), gettext("Solute concentration")};
	#if DEBUG_EDITOR_INITIAL_TRANSPORT_NEW
		fprintf(stderr, "editor_initial_transport_new: start\n");
	#endif
	initial_transport_init_empty(editor->it);
	editor->notebook = notebook;
	editor->grid = (GtkGrid*)gtk_grid_new();
	gtk_notebook_append_page(notebook, GTK_WIDGET(editor->grid),
		gtk_label_new(gettext("Initial transport")));
	editor->frame_type = (GtkFrame*)gtk_frame_new(gettext("Type"));
	editor->grid_type = (GtkGrid*)gtk_grid_new();
	editor->array_type[0] = NULL;
	for (i = 0; i < N_INITIAL_TRANSPORT_TYPES; ++i)
	{
		editor->array_type[i]
			= (GtkRadioButton*)gtk_radio_button_new_with_label_from_widget
				(editor->array_type[0], label_type[i]);
		gtk_grid_attach
			(editor->grid_type, GTK_WIDGET(editor->array_type[i]), 0, i, 1, 1);
		g_signal_connect_swapped(editor->array_type[i], "clicked",
			(void(*))&editor_initial_transport_update, editor);
	}
	gtk_container_add
		(GTK_CONTAINER(editor->frame_type), GTK_WIDGET(editor->grid_type));
	gtk_grid_attach(editor->grid, GTK_WIDGET(editor->frame_type), 0, 0, 1, 1);
	editor->grid_parameters = (GtkGrid*)gtk_grid_new();
	editor->button_insert
		= (GtkButton*)gtk_button_new_with_label(gettext("Insert point"));
	gtk_grid_attach(editor->grid_parameters, GTK_WIDGET(editor->button_insert),
		0, 0, 1, 1);
	g_signal_connect_swapped(editor->button_insert, "clicked",
		(void(*))&editor_initial_transport_insert_point, editor);
	editor->button_remove
		= (GtkButton*)gtk_button_new_with_label(gettext("Remove point"));
	gtk_grid_attach(editor->grid_parameters, GTK_WIDGET(editor->button_remove),
		1, 0, 1, 1);
	g_signal_connect_swapped(editor->button_remove, "clicked",
		(void(*))&editor_initial_transport_remove_point, editor);
	editor->button_plot
		= (GtkButton*)gtk_button_new_with_label(gettext("Update plot"));
	gtk_grid_attach
		(editor->grid_parameters, GTK_WIDGET(editor->button_plot), 2, 0, 1, 1);
	g_signal_connect(editor->button_plot, "clicked", &editor_draw, NULL);
	editor->array = jbw_array_editor_new(2, 2, 0, label_array);
	gtk_grid_attach(editor->grid_parameters,
		GTK_WIDGET(editor->array->scrolled), 0, 1, 3, 1);
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->grid_parameters), 0, 1, 1, 1);
	editor->graphic = jbw_graphic_new(NULL, 6, 6, 0, &editor_draw);
	jbw_graphic_set_title(editor->graphic, gettext("Initial transport"));
	jbw_graphic_set_xlabel(editor->graphic, (char*)label_array[0]);
	jbw_graphic_set_ylabel(editor->graphic, (char*)label_array[1]);
	jbw_graphic_set_logo(editor->graphic, "swigs.png");
	gtk_widget_show_all(GTK_WIDGET(editor->grid));
	#if DEBUG_EDITOR_INITIAL_TRANSPORT_NEW
		fprintf(stderr, "editor_initial_transport_new: end\n");
	#endif
}

#if TEST_EDITOR_INITIAL_TRANSPORT

void ok(char *name)
{
	xmlNode *node;
	xmlDoc *doc;
	if (!editor_initial_transport_check(editor))
	{
		jbw_show_error(message);
		return;
	}
	doc = xmlNewDoc((const xmlChar*)"1.0");
	node = xmlNewDocNode(doc, 0, XML_INITIAL_TRANSPORT, 0);
	xmlDocSetRootElement(doc, node);
	initial_transport_save_xml(editor->it, node);
	xmlSaveFormatFile(name, doc, 1);
	xmlFree(doc);
	glutLeaveMainLoop();
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
	glutIdleFunc((void(*))&gtk_main_iteration);
	notebook = (GtkNotebook*)gtk_notebook_new();
	editor_initial_transport_new(editor, notebook);
	doc = xmlParseFile(argc[1]);
	if (!doc) return 2;
	node = xmlDocGetRootElement(doc);
	if (!initial_transport_open_xml(editor->it, node)) return 3;
	xmlFreeDoc(doc);
	dlg = (GtkDialog*)gtk_dialog_new();
	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(dlg)),
		GTK_WIDGET(notebook));
	gtk_window_set_title(GTK_WINDOW(dlg),
		"Test editor of transported solute initial conditions");
	button_ok = (GtkButton*)gtk_dialog_add_button
		(dlg, gettext("_OK"), GTK_RESPONSE_OK);
	g_signal_connect_swapped(button_ok, "clicked", (void(*))&ok, argc[2]);
	button_cancel = (GtkButton*)gtk_dialog_add_button
		(dlg, gettext("_Cancel"), GTK_RESPONSE_CANCEL);
	g_signal_connect(button_cancel, "clicked", &glutLeaveMainLoop, NULL);
	gtk_widget_show_all(GTK_WIDGET(dlg));
	editor_initial_transport_open(editor);
	glutMainLoop();
	editor_initial_transport_destroy(editor);
	gtk_widget_destroy(GTK_WIDGET(dlg));
	return 0;
}

#endif

