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
 * \file editor_boundary_transport.c
 * \brief Source file to define transported solute boundary conditions editor
 *   structures and functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2014 Javier Burguete Tolosa.
 */
#include "editor_boundary_transport.h"

#if TEST_EDITOR_BOUNDARY_TRANSPORT
	char *message;
	EditorBoundaryTransport editor[1];
	void editor_draw() {editor_boundary_transport_draw(editor);}
#endif

/**
 * \fn void editor_boundary_transport_update(EditorBoundaryTransport *editor)
 * \brief Function to update the view of a transported solute boundary
 *   conditions editor.
 * \param editor
 * \brief transported solute boundary conditions editor.
 */
void editor_boundary_transport_update(EditorBoundaryTransport *editor)
{
	#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_UPDATE
		fprintf(stderr, "editor_boundary_transport_update: start\n");
	#endif
	switch (jbw_array_radio_buttons_get_active(editor->array_type))
	{
		case BOUNDARY_TRANSPORT_TYPE_M:
			jbw_array_editor_set_rows(editor->array, 1);
			jbw_array_editor_set_title
				(editor->array, 1, gettext("Solute mass"));
			jbw_graphic_set_ylabel
				(editor->graphic, gettext("Solute mass"));
			gtk_widget_set_sensitive(GTK_WIDGET(editor->button_insert), 0);
			gtk_widget_set_sensitive(GTK_WIDGET(editor->button_remove), 0);
			break;
		default:
			jbw_array_editor_set_title
				(editor->array, 1, gettext("Solute discharge"));
			jbw_graphic_set_ylabel
				(editor->graphic, gettext("Solute discharge"));
			gtk_widget_set_sensitive(GTK_WIDGET(editor->button_insert), 1);
			gtk_widget_set_sensitive
				(GTK_WIDGET(editor->button_remove), editor->array->n > 1);
	}
	#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_UPDATE
		fprintf(stderr, "editor_boundary_transport_update: end\n");
	#endif
}

/**
 * \fn void editor_boundary_transport_get(EditorBoundaryTransport *editor)
 * \brief Function to get the actual transported solute boundary conditions data
 *   from the editor.
 * \param editor
 * \brief transported solute boundary conditions editor.
 */
void editor_boundary_transport_get(EditorBoundaryTransport *editor)
{
	int j = editor->array->n;
	JBDOUBLE t[j];
	JBFLOAT p[j];
	BoundaryTransport *bt = editor->bt;
	#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_GET
		fprintf(stderr, "editor_boundary_transport_get: start\n");
	#endif
	boundary_transport_delete(bt);
	g_free(bt->name);
	bt->name = g_strdup(gtk_entry_get_text(editor->entry_name));
	if (!bt->name)
	{
		boundary_condition_error(bt, gettext("Not enough memory"));
		jbw_show_error(message);
		gtk_main_quit();
	}
	bt->type = jbw_array_radio_buttons_get_active(editor->array_type);
	#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_GET
		fprintf(stderr, "EIFG: type=%d\n", bt->type);
		fprintf(stderr, "EIFG: rows=%d\n", j);
	#endif
	bt->delay = gtk_spin_button_get_value(editor->entry_delay);
	bt->t = g_realloc(bt->t, j * sizeof(JBDOUBLE));
	bt->p = g_realloc(bt->p, j * sizeof(JBFLOAT));
	if (!bt->t || !bt->p)
	{
		boundary_condition_error(bt, gettext("Not enough memory"));
		jbw_show_error(message);
		gtk_main_quit();
	}
	#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_GET
		fprintf(stderr, "EIFG: getting columns\n");
	#endif
	jbw_array_editor_get_column_time(editor->array, 0, t);
	jbw_array_editor_get_column_float(editor->array, 1, p);
	#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_GET
		fprintf(stderr, "EIFG: setting points\n");
	#endif
	memcpy(bt->t, t, j * sizeof(JBDOUBLE)); 
	memcpy(bt->p, p, j * sizeof(JBFLOAT)); 
	bt->n = --j;
	#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_GET
		boundary_transport_print(bt, stderr);
		fprintf(stderr, "editor_boundary_transport_get: end\n");
	#endif
}

/**
 * \fn void editor_boundary_transport_open(EditorBoundaryTransport *editor)
 * \brief Function to open a transported solute boundary conditions data in the
 *   editor.
 * \param editor
 * \brief transported solute boundary conditions editor.
 */
void editor_boundary_transport_open(EditorBoundaryTransport *editor)
{
	BoundaryTransport *bt = editor->bt;
	#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_OPEN
		fprintf(stderr, "editor_boundary_transport_open: start\n");
	#endif
	gtk_entry_set_text(editor->entry_name, bt->name);
	jbw_array_radio_buttons_set_active(editor->array_type, bt->type, 1);
	gtk_spin_button_set_value(editor->entry_delay, bt->delay);
	jbw_array_editor_set_rows(editor->array, bt->n + 1);
	jbw_array_editor_set_column_time(editor->array, 0, bt->t);
	jbw_array_editor_set_column_float(editor->array, 1, bt->p);
	editor_boundary_transport_update(editor);
	#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_OPEN
		fprintf(stderr, "editor_boundary_transport_open: end\n");
	#endif
}
