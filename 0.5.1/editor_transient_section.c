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
 * \file editor_transient_section.c
 * \brief Source file to define transient section editor structures and methods.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2014 Javier Burguete Tolosa.
 */
#include "editor_transient_section.h"

/**
 * \fn void editor_transient_section_get(EditorTransientSection *editor, \
 *   TransientSection *ts)
 * \brief Function to get actual transient section data from the editor.
 * \param editor
 * \brief transient section editor.
 * \param ts
 * \brief transient section.
 */
void editor_transient_section_get
	(EditorTransientSection *editor, TransientSection *ts)
{
	int i, j = editor->array->n;
	JBFLOAT x[j], y[j], z[j];
	#if DEBUG_EDITOR_TRANSIENT_SECTION_GET
		fprintf(stderr, "editor_transient_section_get: start\n");
	#endif
	transient_section_delete(ts);
	if (!transient_section_create(ts,
		editor->array->n,
		jbw_array_radio_buttons_get_active(editor->array_type),
		gtk_entry_get_text(editor->entry_name)))
	{
		jbw_show_error(message);
		#if DEBUG_EDITOR_TRANSIENT_SECTION_GET
			fprintf(stderr, "editor_transient_section_get: end\n");
		#endif
		gtk_main_quit();
	}
	if (ts->type == TRANSIENT_SECTION_TYPE_STRAIGHT)
	{
		jbw_array_editor_get_column_float(editor->array, 0, y);
		jbw_array_editor_get_column_float(editor->array, 1, z);
		jbw_array_editor_get_column_float(editor->array, 2, r);
		#if INTERFACE == INTERFACE_SCIENTIFIC
			int t[j];
			jbw_array_editor_get_column_int(editor->array, 3, t);
			for (i = j; --i>=0;)
				section_point2_open(ts->sp + i, y[i], z[i], r[i], t[i]);
		#else
			for (i = j; --i>=0;)
				section_point2_open(ts->sp + i, y[i], z[i], r[i], 0);
		#endif
	}
	else
	{
		jbw_array_editor_get_column_float(editor->array, 0, x);
		jbw_array_editor_get_column_float(editor->array, 1, y);
		jbw_array_editor_get_column_float(editor->array, 2, z);
		jbw_array_editor_get_column_float(editor->array, 3, r);
		#if INTERFACE == INTERFACE_SCIENTIFIC
			int t[j];
			jbw_array_editor_get_column_int(editor->array, 4, t);
			for (i = j; --i>=0;)
				section_point3_open(TRANSIENT_SECTION_POINT3(ts) + i,
					x[i], y[i], z[i], r[i], t[i]);
		#else
			for (i = j; --i>=0;)
				section_point3_open(TRANSIENT_SECTION_POINT3(ts) + i,
					x[i], y[i], z[i], r[i], 0);
		#endif
	}
	ts->n = --j;
	ts->t = gtk_spin_button_get_value(editor->entry_t);
	ts->u = gtk_spin_button_get_value(editor->entry_u);
	ts->contraction = gtk_spin_button_get_value(editor->entry_contraction);
	ts->dz = gtk_spin_button_get_value(editor->entry_dz);
	ts->zmin = jbm_varray_min(&(ts->sp->z), sizeof(SectionPoint), j);
	ts->hmax = gtk_spin_button_get_value(editor->entry_hmax);
	if (ts->hmax == 0.)
		ts->hmax = fminl(ts->sp[0].z, ts->sp[ts->n].z) - ts->zmin;
	#if DEBUG_EDITOR_TRANSIENT_SECTION_GET
		transient_section_print(ts,stderr);
		fprintf(stderr,"editor_transient_section_get: end\n");
	#endif
}

/**
 * \fn void editor_transient_section_open(EditorTransientSection *editor, \
 *   TransientSection *ts)
 * \brief Function to open a transient section in the editor.
 * \param editor
 * \brief transient section editor.
 * \param ts
 * \brief transient section.
 */
void editor_transient_section_open
	(EditorTransientSection *editor, TransientSection *ts)
{
	int i, n = ts->n + 1;
	JBFLOAT x[n], y[n], z[n], r[n];
	#if INTERFACE == INTERFACE_SCIENTIFIC
		unsigned int t[n];
	#endif
	#if DEBUG_EDITOR_TRANSIENT_SECTION_OPEN
		fprintf(stderr,"editor_transient_section_open: start\n");
		transient_section_print(ts, stderr);
	#endif
	gtk_entry_set_text(editor->entry_name, ts->name);
	jbw_array_radio_buttons_set_active(editor->array_type, ts->type, 1);
	jbw_array_editor_set_rows(editor->array, n);
	if (ts->type == TRANSIENT_SECTION_TYPE_STRAIGHT)
	{
		for (i = n; --i >= 0;)
		{
			y[i] = ts->sp[i].y;
			z[i] = ts->sp[i].z;
			r[i] = ts->sp[i].r;
			#if INTERFACE == INTERFACE_SCIENTIFIC
				t[i] = ts->sp[i].t;
			#endif
		}
		jbw_array_editor_set_column_float(dlg->editor_transient, 0, y);
		jbw_array_editor_set_column_float(dlg->editor_transient, 1, z);
		jbw_array_editor_set_column_float(dlg->editor_transient, 2, r);
		#if INTERFACE == INTERFACE_SCIENTIFIC
			jbw_array_editor_set_column_int(dlg->editor_transient, 3, t);
		#endif
	}
	else
	{
		for (i = n; --i >= 0;)
		{
			x[i] = TRANSIENT_SECTION_POINT3(ts)[i].x;
			y[i] = TRANSIENT_SECTION_POINT3(ts)[i].y;
			z[i] = TRANSIENT_SECTION_POINT3(ts)[i].z;
			r[i] = TRANSIENT_SECTION_POINT3(ts)[i].r;
			#if INTERFACE == INTERFACE_SCIENTIFIC
				t[i] = TRANSIENT_SECTION_POINT3(ts)[i].t;
			#endif
		}
		jbw_array_editor_set_column_float(dlg->editor_transient, 0, y);
		jbw_array_editor_set_column_float(dlg->editor_transient, 1, z);
		jbw_array_editor_set_column_float(dlg->editor_transient, 2, r);
		#if INTERFACE == INTERFACE_SCIENTIFIC
			jbw_array_editor_set_column_int(dlg->editor_transient, 3, t);
		#endif
	}
	gtk_spin_button_set_value(editor->entry_t, ts->t);
	gtk_spin_button_set_value(editor->entry_u, ts->u);
	gtk_spin_button_set_value(editor->entry_contraction, ts->contraction);
	gtk_spin_button_set_value(editor->entry_dz, ts->dz);
	if (ts->hmax == fminl(ts->sp[0].z, ts->sp[ts->n].z) - ts->zmin)
		gtk_spin_button_set_value(editor->entry_hmax, 0.);
	else gtk_spin_button_set_value(editor->entry_hmax, ts->hmax);
	#if DEBUG_EDITOR_TRANSIENT_SECTION_OPEN
		fprintf(stderr,"editor_transient_section_open: end\n");
	#endif
}

/**
 * \fn void editor_transient_section_update(EditorTransientSection *editor)
 * \brief Function to update the view of a transient section editor.
 * \param editor
 * \brief transient section editor.
 */
void editor_transient_section_update(EditorTransientSection *editor)
{
	int i;
	i = jbw_array_radio_buttons_get_active(editor->array_type);
	if (i == TRANSIENT_SECTION_TYPE_STRAIGHT)
		jbw_array_editor_hide_column(editor->array, 0);
	else jbw_array_editor_show_column(editor->array, 0);
	i = editor->array->n;
	gtk_widget_set_sensitive(GTK_WIDGET(editor->button_remove), n < 3);
}

/**
 * \fn void editor_transient_section_insert_point \
 *   (EditorTransientSection *editor)
 * \brief Function to insert a point in a transient section editor.
 * \param editor
 * \brief transient section editor.
 */
void editor_transient_section_insert_point(EditorTransientSection *editor)
{
	#if DEBUG_EDITOR_TRANSIENT_SECTION_INSERT_POINT
		fprintf(stderr, "editor_transient_section_insert_point: start\n");
	#endif
	jbw_array_editor_insert(editor->array);
	editor_transient_section_update(editor);
	#if DEBUG_EDITOR_TRANSIENT_SECTION_INSERT_POINT
		fprintf(stderr, "editor_transient_section_insert_point: end\n");
	#endif
}

/**
 * \fn void editor_transient_section_remove_point \
 *   (EditorTransientSection *editor)
 * \brief Function to remove a point in a transient section editor.
 * \param editor
 * \brief transient section editor.
 */
void editor_transient_section_remove_point(EditorTransientSection *editor)
{
	#if DEBUG_EDITOR_TRANSIENT_SECTION_REMOVE_POINT
		fprintf(stderr, "editor_transient_section_remove_point: start\n");
	#endif
	jbw_array_editor_remove(editor->array);
	editor_transient_section_update(editor);
	#if DEBUG_EDITOR_TRANSIENT_SECTION_REMOVE_POINT
		fprintf(stderr, "editor_transient_section_remove_point: end\n");
	#endif
}

/**
 * \fn void editor_transient_section_draw(EditorTransientSection *editor, \
 *   TransientSection *ts)
 * \brief Function to draw a transient section in a transient section editor.
 * \param editor
 * \brief transient section editor.
 * \param ts
 * \brief transient section.
 */
void editor_transient_section_draw
	(EditorTransientSection *editor, TransientSection *ts)
{
	int i, n = ts->n + 1;
	JBFLOAT y[n], z[n];
	#if DEBUG_EDITOR_TRANSIENT_SECTION_DRAW
		fprintf(stderr,"editor_transient_section_draw: start\n");
	#endif
	for (i = n; --i >= 0;) y[i] = ts->sp[i].y, z[i] = ts->sp[i].z;
	jbw_graphic_draw_lines(editor->graphic, y, z, 0, 0, 0, n);
	jbw_graphic_draw_logo(editor->graphic);
	#if DEBUG_EDITOR_TRANSIENT_SECTION_DRAW
		fprintf(stderr,"editor_transient_section_draw: end\n");
	#endif
}
