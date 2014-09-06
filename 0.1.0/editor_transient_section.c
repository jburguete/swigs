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

#include "editor_transient_section.h"

void editor_transient_section_get
	(EditorTransientSection *editor, TransientSection *ts)
{
	int i, j = editor->array->n;
	JBFLOAT y[j], z[j], r[j];
	SectionPoint *sp;
	#if DEBUG_EDITOR_TRANSIENT_SECTION_GET
		fprintf(stderr, "editor_transient_section_get: start\n");
		fprintf(stderr, "ETSG n=%d\n", j);
	#endif
	sp = jb_try_realloc(ts->sp, j * sizeof(SectionPoint));
	if (!sp)
	{
		jbw_show_error2
			(gettext("Transient section"), gettext("Not enough memory"));
		#if DEBUG_EDITOR_TRANSIENT_SECTION_GET
			fprintf(stderr, "editor_transient_section_get: end\n");
		#endif
		gtk_main_quit();
	}
	ts->sp = sp;
	jbw_array_editor_get_column_float(editor->array, 0, y);
	jbw_array_editor_get_column_float(editor->array, 1, z);
	jbw_array_editor_get_column_float(editor->array, 2, r);
	#if INTERFACE == INTERFACE_SCIENTIFIC
		int t[j];
		jbw_array_editor_get_column_int(editor->array, 3, t);
		for (i = j; --i>=0;)
			section_point_open(ts->sp + i, y[i], z[i], r[i], t[i]);
	#else
		for (i = j; --i>=0;)
			section_point_open(ts->sp + i, y[i], z[i], r[i], 0);
	#endif
	ts->n = --j;
	ts->zmin = jbm_varray_min(&(ts->sp->z), sizeof(SectionPoint), j);
	if (ts->hmax == 0.)
		ts->hmax = fminl(ts->sp[0].z, ts->sp[ts->n].z) - ts->zmin;
	#if DEBUG_EDITOR_TRANSIENT_SECTION_GET
		transient_section_print(ts,stderr);
		fprintf(stderr,"editor_transient_section_get: end\n");
	#endif
}

void editor_channel_open_transient(EditorChannel *dlg, TransientSection *ts)
{
	int i, n = ts->n + 1;
	JBFLOAT y[n],z[n],r[n];
	#if INTERFACE == INTERFACE_SCIENTIFIC
		unsigned int t[n];
	#endif
	#if DEBUG_EDITOR_CHANNEL_OPEN_TRANSIENT
		fprintf(stderr,"Editor channel open transient: start\n");
		transient_section_print(ts,stderr);
	#endif
	g_signal_handler_block(dlg->notebook,dlg->id_notebook);
	gtk_notebook_set_current_page(dlg->notebook,EDITOR_CHANNEL_PAGE_TRANSIENT);
	g_signal_handler_unblock(dlg->notebook,dlg->id_notebook);
	for (i=n; --i>=0;)
	{
		y[i] = ts->sp[i].y;
		z[i] = ts->sp[i].z;
		r[i] = ts->sp[i].r;
		#if INTERFACE == INTERFACE_SCIENTIFIC
			t[i] = ts->sp[i].t;
		#endif
	}
	jbw_array_editor_set_rows(dlg->editor_transient,n);
	jbw_array_editor_set_column_float(dlg->editor_transient,0,y);
	jbw_array_editor_set_column_float(dlg->editor_transient,1,z);
	jbw_array_editor_set_column_float(dlg->editor_transient,2,r);
	#if INTERFACE == INTERFACE_SCIENTIFIC
		jbw_array_editor_set_column_int(dlg->editor_transient,3,t);
	#endif
	editor_channel_update_transient(dlg);
	#if DEBUG_EDITOR_CHANNEL_OPEN_TRANSIENT
		fprintf(stderr,"Editor channel open transient: end\n");
	#endif
}

void editor_channel_set_transient(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_SET_TRANSIENT
		fprintf(stderr,"Editor channel set transient: start\n");
	#endif
	int i=gtk_combo_box_get_active(GTK_COMBO_BOX(dlg->combo_transient));
	CrossSection *cs = dlg->c->cg->cs + dlg->ics;
	#if DEBUG_EDITOR_CHANNEL_SET_TRANSIENT
		fprintf(stderr,"ECST i %d\n",i);
	#endif
	if (i>=0)
	{
		editor_channel_update_transient(dlg);
		dlg->its = i;
		editor_channel_open_transient(dlg,cs->ts + i);
	}
	#if DEBUG_EDITOR_CHANNEL_SET_TRANSIENT
		fprintf(stderr,"Editor channel set transient: end\n");
	#endif
}

void editor_channel_insert_transient(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_INSERT_TRANSIENT
		fprintf(stderr,"Editor channel insert transient: start\n");
	#endif
	editor_channel_insert_point(dlg->editor_transient);
	editor_channel_update_transient(dlg);
	#if DEBUG_EDITOR_CHANNEL_INSERT_TRANSIENT
		fprintf(stderr,"Editor channel insert transient: end\n");
	#endif
}

void editor_channel_delete_transient(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_DELETE_TRANSIENT
		fprintf(stderr,"Editor channel delete transient: start\n");
	#endif
	editor_channel_delete_point(dlg->editor_transient);
	editor_channel_update_transient(dlg);
	#if DEBUG_EDITOR_CHANNEL_DELETE_TRANSIENT
		fprintf(stderr,"Editor channel delete transient: end\n");
	#endif
}

void editor_channel_draw_transient()
{
	TransientSection *ts = editor->c->cg->cs[editor->ics].ts + editor->its;
	int i, n = ts->n + 1;
	JBFLOAT y[n],z[n];
	#if DEBUG_EDITOR_CHANNEL_DRAW_TRANSIENT
		fprintf(stderr,"Editor channel draw transient: start\n");
	#endif
	for (i=n; --i>=0;) y[i] = ts->sp[i].y, z[i] = ts->sp[i].z;
	jbw_graphic_draw_lines(editor->graphic_transient,y,z,0,0,0,n);
	jbw_graphic_draw_logo(editor->graphic_transient);
	#if DEBUG_EDITOR_CHANNEL_DRAW_TRANSIENT
		fprintf(stderr,"Editor channel draw transient: end\n");
	#endif
}

