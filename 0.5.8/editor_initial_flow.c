/*
SWIGS (Shallow Water in Irregular Geometries Simulator): a software to simulate
initial or steady flows with solute transport in channels, channel networks
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
 * \file editor_initial_flow.c
 * \brief Source file to define initial flow conditions editor structures and
 *   methods.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2014 Javier Burguete Tolosa.
 */
#include "editor_initial_flow.h"

#if TEST_EDITOR_INITIAL_FLOW
	char *message;
	EditorInitialFlow editor[1];
	void editor_draw() {editor_initial_flow_draw(editor);}
#endif

/**
 * \fn void editor_initial_flow_update(EditorInitialFlow *editor)
 * \brief Function to update the view of a initial flow conditions editor.
 * \param editor
 * \brief initial flow conditions editor.
 */
void editor_initial_flow_update(EditorInitialFlow *editor)
{
	#if DEBUG_EDITOR_INITIAL_FLOW_UPDATE
		fprintf(stderr, "editor_initial_flow_update: start\n");
	#endif
	switch (jbw_array_radio_buttons_get_active(editor->array_type))
	{
		case INITIAL_FLOW_TYPE_XQH:
			jbw_array_editor_set_title(editor->array, 1, gettext("Discharge"));
			jbw_array_editor_set_title(editor->array, 2, gettext("Depth"));
			jbw_graphic_set_ylabel(editor->graphic, gettext("Discharge"));
			jbw_graphic_set_zlabel(editor->graphic, gettext("Depth"));
			gtk_widget_show(GTK_WIDGET(editor->grid_parameters));
			break;
		case INITIAL_FLOW_TYPE_XQZ:
			jbw_array_editor_set_title(editor->array, 1, gettext("Discharge"));
			jbw_array_editor_set_title(editor->array, 2, gettext("Level"));
			jbw_graphic_set_ylabel(editor->graphic, gettext("Discharge"));
			jbw_graphic_set_zlabel(editor->graphic, gettext("Level"));
			gtk_widget_show(GTK_WIDGET(editor->grid_parameters));
			break;
		case INITIAL_FLOW_TYPE_XUH:
			jbw_array_editor_set_title(editor->array, 1, gettext("Velocity"));
			jbw_array_editor_set_title(editor->array, 2, gettext("Depth"));
			jbw_graphic_set_ylabel(editor->graphic, gettext("Velocity"));
			jbw_graphic_set_zlabel(editor->graphic, gettext("Depth"));
			gtk_widget_show(GTK_WIDGET(editor->grid_parameters));
			break;
		case INITIAL_FLOW_TYPE_XUZ:
			jbw_array_editor_set_title(editor->array, 1, gettext("Velocity"));
			jbw_array_editor_set_title(editor->array, 2, gettext("Level"));
			jbw_graphic_set_ylabel(editor->graphic, gettext("Velocity"));
			jbw_graphic_set_zlabel(editor->graphic, gettext("Level"));
			gtk_widget_show(GTK_WIDGET(editor->grid_parameters));
			break;
		default:
			gtk_widget_hide(GTK_WIDGET(editor->grid_parameters));
	}
	gtk_widget_set_sensitive
		(GTK_WIDGET(editor->button_remove), editor->array->n > 0);
	#if DEBUG_EDITOR_INITIAL_FLOW_UPDATE
		fprintf(stderr, "editor_initial_flow_update: end\n");
	#endif
}

/**
 * \fn void editor_initial_flow_get(EditorInitialFlow *editor)
 * \brief Function to get the actual initial flow conditions data from the
 *   editor.
 * \param editor
 * \brief initial flow conditions editor.
 */
void editor_initial_flow_get(EditorInitialFlow *editor)
{
	int i, j = editor->array->n;
	JBFLOAT x[j], p1[j], p2[j];
	InitialFlow *ifc = editor->ifc;
	#if DEBUG_EDITOR_INITIAL_FLOW_GET
		fprintf(stderr, "editor_initial_flow_get: start\n");
	#endif
	initial_flow_delete(ifc);
	ifc->type = jbw_array_radio_buttons_get_active(editor->array_type);
	switch (ifc->type)
	{
		case INITIAL_FLOW_TYPE_DRY:
		case INITIAL_FLOW_TYPE_STEADY:
			break;
		default:
			jbw_array_editor_get_column_float(editor->array, 1, x);
			jbw_array_editor_get_column_float(editor->array, 2, p1);
			jbw_array_editor_get_column_float(editor->array, 3, p2);
			for (i = j; --i>=0;) point3_open(ifc->p + i, x[i], p1[i], p2[i]);
	}
	ifc->n = --j;
	#if DEBUG_EDITOR_INITIAL_FLOW_GET
		initial_flow_print(ifc, stderr);
		fprintf(stderr, "editor_initial_flow_get: end\n");
	#endif
}

/**
 * \fn void editor_initial_flow_open(EditorInitialFlow *editor)
 * \brief Function to open initial flow conditions in the editor.
 * \param editor
 * \brief initial flow conditions editor.
 */
void editor_initial_flow_open(EditorInitialFlow *editor)
{
	InitialFlow *ifc = editor->ifc;
	int i, n = ifc->n + 1;
	JBFLOAT x[n], p1[n], p2[n];
	#if DEBUG_EDITOR_INITIAL_FLOW_OPEN
		fprintf(stderr, "editor_initial_flow_open: start\n");
		initial_flow_print(ifc, stderr);
	#endif
	jbw_array_radio_buttons_set_active(editor->array_type, ifc->type, 1);
	switch (ifc->type)
	{
		case INITIAL_FLOW_TYPE_DRY:
		case INITIAL_FLOW_TYPE_STEADY:
			jbw_array_editor_set_rows(editor->array, 0);
			break;
		default:
			for (i = n; --i >= 0;)
			{
				x[i] = ifc->p[i].x;
				p1[i] = ifc->p[i].y;
				p2[i] = ifc->p[i].z;
			}
			jbw_array_editor_set_column_float(editor->array, 0, x);
			jbw_array_editor_set_column_float(editor->array, 1, p1);
			jbw_array_editor_set_column_float(editor->array, 2, p2);
	}
	editor_initial_flow_update(editor);
	#if DEBUG_EDITOR_INITIAL_FLOW_OPEN
		fprintf(stderr, "editor_initial_flow_open: end\n");
	#endif
}

/**
 * \fn void editor_initial_flow_insert_point (EditorInitialFlow *editor)
 * \brief Function to insert a point in an initial flow conditions editor.
 * \param editor
 * \brief initial flow conditions editor.
 */
void editor_initial_flow_insert_point(EditorInitialFlow *editor)
{
	#if DEBUG_EDITOR_INITIAL_FLOW_INSERT_POINT
		fprintf(stderr, "editor_initial_flow_insert_point: start\n");
	#endif
	jbw_array_editor_insert(editor->array);
	editor_initial_flow_update(editor);
	#if DEBUG_EDITOR_INITIAL_FLOW_INSERT_POINT
		fprintf(stderr, "editor_initial_flow_insert_point: end\n");
	#endif
}

/**
 * \fn void editor_initial_flow_remove_point (EditorInitialFlow *editor)
 * \brief Function to remove a point in an initial flow conditions editor.
 * \param editor
 * \brief initial flow conditions editor.
 */
void editor_initial_flow_remove_point(EditorInitialFlow *editor)
{
	#if DEBUG_EDITOR_INITIAL_FLOW_REMOVE_POINT
		fprintf(stderr, "editor_initial_flow_remove_point: start\n");
	#endif
	jbw_array_editor_remove(editor->array);
	editor_initial_flow_update(editor);
	#if DEBUG_EDITOR_INITIAL_FLOW_REMOVE_POINT
		fprintf(stderr, "editor_initial_flow_remove_point: end\n");
	#endif
}

/**
 * \fn void editor_initial_flow_draw(EditorInitialFlow *editor)
 * \brief Function to draw a initial flow conditions in an initial flow
 *   conditions editor.
 * \param editor
 * \brief initial flow conditions editor.
 */
void editor_initial_flow_draw(EditorInitialFlow *editor)
{
	#if DEBUG_EDITOR_INITIAL_FLOW_DRAW
		fprintf(stderr, "editor_initial_flow_draw: start\n");
	#endif
	editor_initial_flow_get(editor);
	InitialFlow *ifc = editor->ifc;
	int i, n = ifc->n + 1;
	JBFLOAT x[n], y[n], z[n];
	for (i = n; --i >= 0;)
		x[i] = ifc->p[i].x, y[i] = ifc->p[i].y, z[i] = ifc->p[i].z;
	jbw_graphic_draw_lines(editor->graphic, x, y, z, NULL, NULL, n);
	jbw_graphic_draw_logo(editor->graphic);
#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
	glutSwapBuffers();
#elif JBW_GRAPHIC == JBW_GRAPHIC_CLUTTER
	glFlush();
#endif
	#if DEBUG_EDITOR_INITIAL_FLOW_DRAW
		fprintf(stderr, "editor_initial_flow_draw: end\n");
	#endif
}

/**
 * \fn void editor_initial_flow_destroy(EditorInitialFlow *editor)
 * \brief Function to destroy a initial flow conditions editor.
 * \param editor
 * \brief initial flow conditions editor.
 */
void editor_initial_flow_destroy(EditorInitialFlow *editor)
{
	#if DEBUG_EDITOR_INITIAL_FLOW_DESTROY
		fprintf(stderr, "editor_initial_flow_destroy: start\n");
	#endif
	gtk_widget_destroy(GTK_WIDGET(editor->grid));
	initial_flow_delete(editor->ifc);
	#if DEBUG_EDITOR_INITIAL_FLOW_DESTROY
		fprintf(stderr, "editor_initial_flow_destroy: end\n");
	#endif
}

/**
 * \fn void editor_initial_flow_new(EditorInitialFlow *editor, \
 *   GtkNotebook *notebook)
 * \brief Function to create a new initial flow conditions editor.
 * \param editor
 * \brief initial flow conditions editor.
 * \param notebook
 * \brief GtkNotebook to pack the widgets.
 */
void editor_initial_flow_new(EditorInitialFlow *editor, GtkNotebook *notebook)
{
	int i;
	const char *label_type[N_INITIAL_FLOW_TYPES] =
		{gettext("Dry"), gettext("Steady"),
		gettext("Discharge and depth profile"),
		gettext("Discharge and level profile"),
		gettext("Velocity and depth profile"),
		gettext("Velocity and level profile")};
	const char *label_array[3]
		= {"Position", "Discharge", gettext("Depth")};
	#if DEBUG_EDITOR_INITIAL_FLOW_NEW
		fprintf(stderr, "editor_initial_flow_new: start\n");
	#endif
	initial_flow_init_empty(editor->ifc);
	editor->notebook = notebook;
	editor->grid = (GtkGrid*)gtk_grid_new();
	gtk_notebook_append_page(notebook, GTK_WIDGET(editor->grid),
		gtk_label_new(gettext("Initial flow")));
	editor->frame_type = (GtkFrame*)gtk_frame_new(gettext("Type"));
	editor->grid_type = (GtkGrid*)gtk_grid_new();
	editor->array_type[0] = NULL;
	for (i = 0; i < N_INITIAL_FLOW_TYPES; ++i)
	{
		editor->array_type[i]
			= (GtkRadioButton*)gtk_radio_button_new_with_label_from_widget
				(editor->array_type[0], label_type[i]);
		gtk_grid_attach
			(editor->grid_type, GTK_WIDGET(editor->array_type[i]), 0, i, 1, 1);
		g_signal_connect_swapped(editor->array_type[i], "clicked",
			(void(*))&editor_initial_flow_update, editor);
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
		(void(*))&editor_initial_flow_insert_point, editor);
	editor->button_remove
		= (GtkButton*)gtk_button_new_with_label(gettext("Remove point"));
	gtk_grid_attach(editor->grid_parameters, GTK_WIDGET(editor->button_remove),
		1, 0, 1, 1);
	g_signal_connect_swapped(editor->button_remove, "clicked",
		(void(*))&editor_initial_flow_remove_point, editor);
	editor->button_plot
		= (GtkButton*)gtk_button_new_with_label(gettext("Update plot"));
	gtk_grid_attach
		(editor->grid_parameters, GTK_WIDGET(editor->button_plot), 2, 0, 1, 1);
	g_signal_connect(editor->button_plot, "clicked", &editor_draw, NULL);
	editor->array = jbw_array_editor_new(3, 3, 0, label_array);
	gtk_grid_attach(editor->grid_parameters,
		GTK_WIDGET(editor->array->scrolled), 0, 1, 3, 1);
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->grid_parameters), 0, 1, 1, 1);
	editor->graphic = jbw_graphic_new(NULL, 6, 6, 0, &editor_draw);
	jbw_graphic_set_title(editor->graphic, gettext("Initial flow"));
	jbw_graphic_set_logo(editor->graphic, "swigs.png");
	gtk_widget_show_all(GTK_WIDGET(editor->grid));
	#if DEBUG_EDITOR_INITIAL_FLOW_NEW
		fprintf(stderr, "editor_initial_flow_new: end\n");
	#endif
}

#if TEST_EDITOR_INITIAL_FLOW

void ok(char *name)
{
	xmlNode *node;
	xmlDoc *doc;
	editor_initial_flow_get(editor);
	doc = xmlNewDoc((const xmlChar*)"1.0");
	node = xmlNewDocNode(doc, 0, XML_INITIAL_FLOW, 0);
	xmlDocSetRootElement(doc, node);
	initial_flow_save_xml(editor->ifc, node);
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
	editor_initial_flow_new(editor, notebook);
	doc = xmlParseFile(argc[1]);
	if (!doc) return 2;
	node = xmlDocGetRootElement(doc);
	if (!initial_flow_open_xml(editor->ifc, node)) return 3;
	xmlFreeDoc(doc);
	dlg = (GtkDialog*)gtk_dialog_new();
	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(dlg)),
		GTK_WIDGET(notebook));
	gtk_window_set_title
		(GTK_WINDOW(dlg), "Test editor initial flow conditions");
	button_ok = (GtkButton*)gtk_dialog_add_button
		(dlg, gettext("_OK"), GTK_RESPONSE_OK);
	g_signal_connect_swapped(button_ok, "clicked", (void(*))&ok, argc[2]);
	button_cancel = (GtkButton*)gtk_dialog_add_button
		(dlg, gettext("_Cancel"), GTK_RESPONSE_CANCEL);
	g_signal_connect(button_cancel, "clicked", &glutLeaveMainLoop, NULL);
	gtk_widget_show_all(GTK_WIDGET(dlg));
	editor_initial_flow_open(editor);
	glutMainLoop();
	editor_initial_flow_destroy(editor);
	gtk_widget_destroy(GTK_WIDGET(dlg));
	return 0;
}

#endif

