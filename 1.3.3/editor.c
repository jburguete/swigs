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

#include "dialog_simulator.h"
#include "dialog_position.h"
#include "editor_channel.h"
#include "simulate.h"
#include "draw.h"

void editor_paned(GtkVPaned **paned,GtkGrid *table,JBWGraphic *graphic)
{
	GtkFrame *frame;
#if DEBUG_EDITOR_PANED
	fprintf(stderr,"Editor paned: start\n");
#endif
	graphic->zmax = graphic->zmin = 0.;
	*paned = (GtkVPaned*)gtk_vpaned_new();
	frame=(GtkFrame*)gtk_frame_new(0);
	gtk_frame_set_shadow_type(frame,GTK_SHADOW_IN);
	gtk_container_add((GtkContainer*)frame,(GtkWidget*)table);
	gtk_paned_pack1((GtkPaned*)*paned,(GtkWidget*)frame,1,0);
	frame=(GtkFrame*)gtk_frame_new(0);
	gtk_frame_set_shadow_type(frame,GTK_SHADOW_IN);
	gtk_paned_pack2((GtkPaned*)*paned,(GtkWidget*)frame,1,0);
#if DEBUG_EDITOR_PANED
	fprintf(stderr,"Editor paned: end\n");
#endif
}

#if EDITOR_BF

void editor_bf_get(EditorBF *editor,BoundaryFlow *bf,EditorBFType type)
{
	int n;
	int inner[]={BOUNDARY_FLOW_Q,BOUNDARY_FLOW_QT,BOUNDARY_FLOW_QH,
		BOUNDARY_FLOW_QZ,BOUNDARY_FLOW_DAM,BOUNDARY_FLOW_PIPE,
		BOUNDARY_FLOW_GATE};
	#if DEBUG_EDITOR_BF_GET
		fprintf(stderr,"Editor bf get: start\n");
		fprintf(stderr,"EBFG name=%s\n",bf->name);
	#endif
	n = jbw_array_radio_buttons_get_active(editor->button_type);
	if (type==EDITOR_BF_TYPE_INNER) 
	{
		jb_free_null((void**)&bf->name);
		bf->name = jb_strdup((char*)gtk_entry_get_text(editor->entry_name));
		if (!bf->name) goto exit2;
		n=inner[n];
	}
	bf->type = n;
	#if DEBUG_EDITOR_BF_GET
		fprintf(stderr,"EBFG type=%d\n",bf->type);
	#endif
	switch (bf->type)
	{
	case BOUNDARY_FLOW_Q:
	case BOUNDARY_FLOW_H:
	case BOUNDARY_FLOW_Z:
		bf->p1 = jb_try_realloc(bf->p1, 2 * sizeof(JBFLOAT));
		if (!bf->p1) goto exit2;
		bf->p2 = bf->p1 + 1;
		bf->n = 0;
		bf->p1[0] = 0.;
		break;
	case BOUNDARY_FLOW_Q_H:
	case BOUNDARY_FLOW_Q_Z:
		bf->p1 = jb_try_realloc(bf->p1, 3 * sizeof(JBFLOAT));
		if (!bf->p1) goto exit2;
		bf->p2 = bf->p1 + 1;
		bf->p3 = bf->p2 + 1;
		bf->n = 0;
		bf->p1[0] = 0.;
		bf->p2[0] = jbw_float_entry_get_value(editor->entry_discharge);
		bf->p3[0] = jbw_float_entry_get_value(editor->entry_depth);
		break;
	case BOUNDARY_FLOW_QT:
	case BOUNDARY_FLOW_HT:
	case BOUNDARY_FLOW_ZT:
	case BOUNDARY_FLOW_QH:
	case BOUNDARY_FLOW_QZ:
	case BOUNDARY_FLOW_DAM:
	case BOUNDARY_FLOW_PIPE:
		n = editor->editor->n;
		#if DEBUG_EDITOR_BF_GET
			fprintf(stderr,"EBFG n=%d\n",n);
		#endif
		bf->p1 = jb_try_realloc(bf->p1, 2 * sizeof(JBFLOAT) * n);
		if (!bf->p1) goto exit2;
		bf->p2 = bf->p1 + n;
		bf->p3 = 0;
		bf->n = n - 1;
		jbw_array_editor_get_column_float(editor->editor,0,bf->p1);
		jbw_array_editor_get_column_float(editor->editor,1,bf->p2);
		break;
	case BOUNDARY_FLOW_QT_HT:
	case BOUNDARY_FLOW_QT_ZT:
		n = editor->editor->n;
		#if DEBUG_EDITOR_BF_GET
			fprintf(stderr,"EBFG n=%d\n",n);
		#endif
		bf->p1 = jb_try_realloc(bf->p1, 3 * sizeof(JBFLOAT) * n);
		if (!bf->p1) goto exit2;
		bf->p2 = bf->p1 + n;
		bf->p3 = bf->p2 + n;
		bf->n = n - 1;
		jbw_array_editor_get_column_float(editor->editor,0,bf->p1);
		jbw_array_editor_get_column_float(editor->editor,1,bf->p2);
		jbw_array_editor_get_column_float(editor->editor,2,bf->p3);
		break;
	default:
		jb_free_null((void**)&bf->p1);
	}
	#if DEBUG_EDITOR_BF_GET
		fprintf(stderr,"EBFG parameters\n");
	#endif
	switch (bf->type)
	{
	case BOUNDARY_FLOW_Q:
		bf->p2[0] = jbw_float_entry_get_value(editor->entry_discharge);
		break;
	case BOUNDARY_FLOW_H:
		bf->p2[0] = jbw_float_entry_get_value(editor->entry_depth);
		break;
	case BOUNDARY_FLOW_Z:
		bf->p2[0] = jbw_float_entry_get_value(editor->entry_level);
		break;
	case BOUNDARY_FLOW_DAM:
		bf->data = jb_try_realloc(bf->data, 3 * sizeof(JBFLOAT));
		if (!bf->data) goto exit2;
		DAM_LEVEL(bf) =
			jbw_float_entry_get_value((JBWFloatEntry*)editor->entry_data1);
		DAM_ROUGHNESS(bf) =
			jbw_float_entry_get_value((JBWFloatEntry*)editor->entry_data2);
		break;
	case BOUNDARY_FLOW_GATE:
		bf->data = jb_try_realloc(bf->data, 3 * sizeof(JBFLOAT));
		if (!bf->data) goto exit2;
		GATE_HEIGHT(bf) =
			jbw_float_entry_get_value((JBWFloatEntry*)editor->entry_data1);
		GATE_WIDTH(bf) =
			jbw_float_entry_get_value((JBWFloatEntry*)editor->entry_data2);
	}
	#if DEBUG_EDITOR_BF_GET
		boundary_flow_print(bf,stderr);
		fprintf(stderr,"Editor bf get: end\n");
	#endif
	return;

exit2:
	jbw_show_error2
		(gettext("Flow boundary conditions"),gettext("Not enough memory"));
	#if DEBUG_EDITOR_BF_GET
		fprintf(stderr,"Editor bf get: end\n");
	#endif
	gtk_main_quit();
}

void editor_bf_update(EditorBF *editor,BoundaryFlow *bf,EditorBFType type)
{
	#if DEBUG_EDITOR_BF_UPDATE
		fprintf(stderr,"Editor bf update: start\n");
	#endif
	editor_bf_get(editor,bf,type);
	#if DEBUG_EDITOR_BF_UPDATE
		fprintf(stderr,"EBFA type=%d\n",bf->type);
	#endif
	gtk_widget_hide((GtkWidget*)editor->label_discharge);
	gtk_widget_hide((GtkWidget*)editor->entry_discharge);
	gtk_widget_hide((GtkWidget*)editor->label_depth);
	gtk_widget_hide((GtkWidget*)editor->entry_depth);
	gtk_widget_hide((GtkWidget*)editor->label_level);
	gtk_widget_hide((GtkWidget*)editor->entry_level);
	gtk_widget_hide((GtkWidget*)editor->label_data1);
	gtk_widget_hide((GtkWidget*)editor->entry_data1);
	gtk_widget_hide((GtkWidget*)editor->label_data2);
	gtk_widget_hide((GtkWidget*)editor->entry_data2);
	gtk_widget_hide((GtkWidget*)editor->paned);
	jb_free_null((void**)&editor->graphic->str_z);
	switch (bf->type)
	{
	case BOUNDARY_FLOW_Q_Z:
		gtk_widget_show((GtkWidget*)editor->label_discharge);
		gtk_widget_show((GtkWidget*)editor->entry_discharge);
		gtk_widget_show((GtkWidget*)editor->label_level);
		gtk_widget_show((GtkWidget*)editor->entry_level);
		jbw_graphic_set_xlabel(editor->graphic,gettext("Time"));
		jbw_graphic_set_ylabel(editor->graphic,gettext("Discharge"));
		jbw_graphic_set_zlabel(editor->graphic,gettext("Level"));
		goto exit2;
	case BOUNDARY_FLOW_Q_H:
		gtk_widget_show((GtkWidget*)editor->label_depth);
		gtk_widget_show((GtkWidget*)editor->entry_depth);
		jbw_graphic_set_zlabel(editor->graphic,gettext("Depth"));
	case BOUNDARY_FLOW_Q:
		gtk_widget_show((GtkWidget*)editor->label_discharge);
		gtk_widget_show((GtkWidget*)editor->entry_discharge);
		jbw_graphic_set_xlabel(editor->graphic,gettext("Time"));
		jbw_graphic_set_ylabel(editor->graphic,gettext("Discharge"));
		goto exit2;
	case BOUNDARY_FLOW_H:
		gtk_widget_show((GtkWidget*)editor->label_depth);
		gtk_widget_show((GtkWidget*)editor->entry_depth);
		jbw_graphic_set_xlabel(editor->graphic,gettext("Time"));
		jbw_graphic_set_ylabel(editor->graphic,gettext("Depth"));
		goto exit2;
	case BOUNDARY_FLOW_Z:
		gtk_widget_show((GtkWidget*)editor->label_level);
		gtk_widget_show((GtkWidget*)editor->entry_level);
		jbw_graphic_set_xlabel(editor->graphic,gettext("Time"));
		jbw_graphic_set_ylabel(editor->graphic,gettext("Level"));
		goto exit2;
	case BOUNDARY_FLOW_QT:
	case BOUNDARY_FLOW_DAM:
	case BOUNDARY_FLOW_PIPE:
		jbw_array_editor_set_title(editor->editor,0,gettext("Time"));
		jbw_array_editor_set_title(editor->editor,1,gettext("Discharge"));
		jbw_graphic_set_xlabel(editor->graphic,gettext("Time"));
		jbw_graphic_set_ylabel(editor->graphic,gettext("Discharge"));
		break;
	case BOUNDARY_FLOW_HT:
		jbw_array_editor_set_title(editor->editor,0,gettext("Time"));
		jbw_array_editor_set_title(editor->editor,1,gettext("Depth"));
		jbw_graphic_set_xlabel(editor->graphic,gettext("Time"));
		jbw_graphic_set_ylabel(editor->graphic,gettext("Depth"));
		break;
	case BOUNDARY_FLOW_ZT:
		jbw_array_editor_set_title(editor->editor,0,gettext("Time"));
		jbw_array_editor_set_title(editor->editor,1,gettext("Level"));
		jbw_graphic_set_xlabel(editor->graphic,gettext("Time"));
		jbw_graphic_set_ylabel(editor->graphic,gettext("Level"));
		break;
	case BOUNDARY_FLOW_QH:
		jbw_array_editor_set_title(editor->editor,0,gettext("Depth"));
		jbw_array_editor_set_title(editor->editor,1,gettext("Discharge"));
		jbw_graphic_set_xlabel(editor->graphic,gettext("Depth"));
		jbw_graphic_set_ylabel(editor->graphic,gettext("Discharge"));
		break;
	case BOUNDARY_FLOW_QZ:
		jbw_array_editor_set_title(editor->editor,0,gettext("Level"));
		jbw_array_editor_set_title(editor->editor,1,gettext("Discharge"));
		jbw_graphic_set_xlabel(editor->graphic,gettext("Level"));
		jbw_graphic_set_ylabel(editor->graphic,gettext("Discharge"));
		break;
	case BOUNDARY_FLOW_QT_HT:
		jbw_array_editor_set_title(editor->editor,0,gettext("Time"));
		jbw_array_editor_set_title(editor->editor,1,gettext("Discharge"));
		jbw_array_editor_set_title(editor->editor,2,gettext("Depth"));
		jbw_array_editor_show_column(editor->editor,2);
		jbw_graphic_set_xlabel(editor->graphic,gettext("Time"));
		jbw_graphic_set_ylabel(editor->graphic,gettext("Discharge"));
		jbw_graphic_set_zlabel(editor->graphic,gettext("Depth"));
		goto exit1;
	case BOUNDARY_FLOW_QT_ZT:
		jbw_array_editor_set_title(editor->editor,0,gettext("Time"));
		jbw_array_editor_set_title(editor->editor,1,gettext("Discharge"));
		jbw_array_editor_set_title(editor->editor,2,gettext("Level"));
		jbw_array_editor_show_column(editor->editor,2);
		jbw_graphic_set_xlabel(editor->graphic,gettext("Time"));
		jbw_graphic_set_ylabel(editor->graphic,gettext("Discharge"));
		jbw_graphic_set_zlabel(editor->graphic,gettext("Level"));
		goto exit1;
	case BOUNDARY_FLOW_GATE:
		break;
	default:
		goto exit2;
	}
	switch (bf->type)
	{
	case BOUNDARY_FLOW_DAM:
		gtk_label_set_text(editor->label_data1,gettext("Level"));
		gtk_label_set_text(editor->label_data2,gettext("Friction coefficient"));
		gtk_widget_show((GtkWidget*)editor->label_data1);
		gtk_widget_show((GtkWidget*)editor->entry_data1);
		gtk_widget_show((GtkWidget*)editor->label_data2);
		gtk_widget_show((GtkWidget*)editor->entry_data2);
		break;
	case BOUNDARY_FLOW_GATE:
		gtk_label_set_text(editor->label_data1,gettext("Height"));
		gtk_label_set_text(editor->label_data2,gettext("Width"));
		gtk_widget_show((GtkWidget*)editor->label_data1);
		gtk_widget_show((GtkWidget*)editor->entry_data1);
		gtk_widget_show((GtkWidget*)editor->label_data2);
		gtk_widget_show((GtkWidget*)editor->entry_data2);
		goto exit2;
	}
	jbw_array_editor_hide_column(editor->editor,2);

exit1:
	gtk_widget_set_sensitive((GtkWidget*)editor->button_delete, bf->n > 0);
	gtk_widget_show((GtkWidget*)editor->paned);
#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
	jbw_graphic_draw = editor->graphic->draw;
	jbw_graphic_expose_event();
#endif

exit2:
	#if DEBUG_EDITOR_BF_UPDATE
		fprintf(stderr,"Editor bf update: end\n");
	#endif
	return;
}

void editor_bf_open(EditorBF *editor,BoundaryFlow *bf,EditorBFType type)
{
	int k;
	int inner[]={0,-1,-1,-1,-1,1,-1,-1,-1,-1,2,3,-1,4,5};
	#if DEBUG_EDITOR_BF_OPEN
		fprintf(stderr,"Editor bf open: start\n");
	#endif
	jbw_float_entry_set_value(editor->entry_discharge,0.);
	jbw_float_entry_set_value(editor->entry_depth,0.);
	jbw_float_entry_set_value(editor->entry_level,0.);
	switch (bf->type)
	{
	case BOUNDARY_FLOW_Q_Z:
		jbw_float_entry_set_value(editor->entry_level,bf->p3[0]);
		jbw_float_entry_set_value(editor->entry_discharge,bf->p2[0]);
		break;
	case BOUNDARY_FLOW_Q_H:
		jbw_float_entry_set_value(editor->entry_depth,bf->p3[0]);
	case BOUNDARY_FLOW_Q:
		jbw_float_entry_set_value(editor->entry_discharge,bf->p2[0]);
		break;
	case BOUNDARY_FLOW_H:
		jbw_float_entry_set_value(editor->entry_depth,bf->p2[0]);
		break;
	case BOUNDARY_FLOW_Z:
		jbw_float_entry_set_value(editor->entry_level,bf->p2[0]);
		break;
	case BOUNDARY_FLOW_QT:
	case BOUNDARY_FLOW_HT:
	case BOUNDARY_FLOW_ZT:
	case BOUNDARY_FLOW_QH:
	case BOUNDARY_FLOW_QZ:
	case BOUNDARY_FLOW_DAM:
	case BOUNDARY_FLOW_PIPE:
		jbw_array_editor_set_rows(editor->editor,bf->n+1);
		jbw_array_editor_set_column_float(editor->editor,0,bf->p1);
		jbw_array_editor_set_column_float(editor->editor,1,bf->p2);
		break;
	case BOUNDARY_FLOW_QT_HT:
	case BOUNDARY_FLOW_QT_ZT:
		jbw_array_editor_set_rows(editor->editor,bf->n+1);
		jbw_array_editor_set_column_float(editor->editor,0,bf->p1);
		jbw_array_editor_set_column_float(editor->editor,1,bf->p2);
		jbw_array_editor_set_column_float(editor->editor,2,bf->p3);
	}
	switch (bf->type)
	{
	case BOUNDARY_FLOW_DAM:
		jbw_float_entry_set_value
			((JBWFloatEntry*)editor->entry_data1,DAM_LEVEL(bf));
		jbw_float_entry_set_value
			((JBWFloatEntry*)editor->entry_data2,DAM_ROUGHNESS(bf));
		break;
	case BOUNDARY_FLOW_GATE:
		jbw_float_entry_set_value
			((JBWFloatEntry*)editor->entry_data1,GATE_HEIGHT(bf));
		jbw_float_entry_set_value
			((JBWFloatEntry*)editor->entry_data2,GATE_WIDTH(bf));
	}
	k = bf->type;
	if (type==EDITOR_BF_TYPE_INNER)
	{
		gtk_entry_set_text(editor->entry_name,bf->name);
		k=inner[k];
	}
	jbw_array_radio_buttons_set_active(editor->button_type,k,1);
	#if DEBUG_EDITOR_BF_OPEN
		fprintf(stderr,"Editor bf open: end\n");
	#endif
}

void editor_bf_create(EditorBF *editor,EditorBFType type,
	void (*editor_update),void (*editor_draw),void *dlg)
{
	int i,n;
	char **str;
	char *label_bf[]={gettext("Steady disc_harge"),gettext("Stead_y depth"),
		gettext("S_teady level"),gettext("_Steady discharge and depth"),
		gettext("Ste_ady discharge and level"),
		gettext("Va_riable discharge Q(t)"),gettext("_Variable depth h(t)"),
		gettext("Variable level _z(t)"),
		gettext("Variab_le discharge and depth Q(t), h(t)"),
		gettext("Variable _discharge and level Q(t), z(t)"),
		gettext("_Gauge discharge Q(h)"),gettext("Gaug_e discharge Q(z)"),
		gettext("Su_percritical"),gettext("G_ate")};
	char *label_inner[]={gettext("Steady disc_harge"),
		gettext("Va_riable discharge Q(t)"),gettext("_Gauge discharge Q(h)"),
		gettext("Gaug_e discharge Q(z)"),gettext("Da_m"),gettext("_Pipe")};
	char *label_bf2[]={gettext("Time"),gettext("Discharge"),gettext("Depth")};
	#if DEBUG_EDITOR_BF_CREATE
		fprintf(stderr,"Editor bf create: start\n");
	#endif
	editor->hbox2 = (GtkHBox*)gtk_hbox_new(0,0);
	editor->vbox = (GtkVBox*)gtk_vbox_new(0,0);
	if (type==EDITOR_BF_TYPE_IO)
	{
		n=N_BOUNDARY_FLOW_EXTERN;
		str=label_bf;
	}
	else
	{
		editor->label_name =
			(GtkLabel*)gtk_label_new(gettext("Flow boundary condition"));
		editor->entry_name = (GtkEntry*)gtk_entry_new();
		gtk_box_pack_start((GtkBox*)editor->hbox2,
			(GtkWidget*)editor->label_name,0,1,0);
		gtk_box_pack_start_defaults((GtkBox*)editor->hbox2,
			(GtkWidget*)editor->entry_name);
		n=N_BOUNDARY_FLOW_INNER;
		str=label_inner;
	}
	editor->button_type =
		(GtkRadioButton**)g_malloc(n * sizeof(GtkRadioButton*));
	editor->button_type[0] = 0;
	for (i=0; i<n; ++i)
	{
		editor->button_type[i] = (GtkRadioButton*)
			gtk_radio_button_new_with_mnemonic_from_widget
				(editor->button_type[0],str[i]);
		gtk_box_pack_start_defaults((GtkBox*)editor->vbox,
			(GtkWidget*)editor->button_type[i]);
		g_signal_connect_swapped
			(editor->button_type[i],"clicked",editor_update,dlg);
	}
	editor->frame = (GtkFrame*)gtk_frame_new(gettext("Type"));
	gtk_container_add((GtkContainer*)editor->frame,(GtkWidget*)editor->vbox);
	editor->label_discharge = (GtkLabel*)gtk_label_new(gettext("Discharge"));
	editor->label_depth = (GtkLabel*)gtk_label_new(gettext("Depth"));
	editor->label_level = (GtkLabel*)gtk_label_new(gettext("Level"));
	editor->label_data1 = (GtkLabel*)gtk_label_new(0);
	editor->label_data2 = (GtkLabel*)gtk_label_new(0);
	editor->entry_discharge = jbw_float_entry_new();
	editor->entry_depth = jbw_float_entry_new();
	editor->entry_level = jbw_float_entry_new();
	editor->entry_data1 = (GtkEntry*)gtk_entry_new();
	editor->entry_data2 = (GtkEntry*)gtk_entry_new();
	editor->button_insert =
		(GtkButton*)gtk_button_new_with_mnemonic(gettext("Insert _point"));
	editor->button_delete =
		(GtkButton*)gtk_button_new_with_mnemonic(gettext("Delete poi_nt"));
	editor->button_update =
		(GtkButton*)gtk_button_new_with_mnemonic(gettext("Update p_lot"));
	editor_channel_vbox(&editor->vbox2,editor->button_insert,
		editor->button_delete,editor->button_update);
	editor->editor = jbw_array_editor_new(3,3,1,label_bf2);
	editor->table2 = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach(editor->table2,
		(GtkWidget*)editor->editor->scrolled,0,0,1,1);
	gtk_grid_attach(editor->table2,(GtkWidget*)editor->vbox2,1,0,1,1);
	editor->graphic = (JBWGraphic*)jbw_graphic_new(0,5,5,5,editor_draw);
	jbw_graphic_set_logo(editor->graphic, "swigs.png");
	editor_paned(&editor->paned,editor->table2,editor->graphic);
	editor->table = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach(editor->table,(GtkWidget*)editor->hbox2,
		0,0,3,1);
	gtk_grid_attach(editor->table,(GtkWidget*)editor->frame,
		0,1,1,7);
	gtk_grid_attach(editor->table,(GtkWidget*)editor->label_data1,
		1,1,1,1);
	gtk_grid_attach(editor->table,(GtkWidget*)editor->entry_data1,
		2,1,1,1);
	gtk_grid_attach(editor->table,(GtkWidget*)editor->label_data2,
		1,2,1,1);
	gtk_grid_attach(editor->table,(GtkWidget*)editor->entry_data2,
		2,2,1,1);
	gtk_grid_attach(editor->table,(GtkWidget*)editor->label_discharge,
		1,4,1,1);
	gtk_grid_attach(editor->table,(GtkWidget*)editor->entry_discharge,
		2,4,1,1);
	gtk_grid_attach(editor->table,(GtkWidget*)editor->label_depth,
		1,5,1,1);
	gtk_grid_attach(editor->table,(GtkWidget*)editor->entry_depth,
		2,5,1,1);
	gtk_grid_attach(editor->table,(GtkWidget*)editor->label_level,
		1,6,1,1);
	gtk_grid_attach(editor->table,(GtkWidget*)editor->entry_level,
		2,6,1,1);
	gtk_grid_attach(editor->table,(GtkWidget*)editor->paned,1,7,2,1);
	#if DEBUG_EDITOR_BF_CREATE
		fprintf(stderr,"Editor bf create: end\n");
	#endif
}

#endif

void editor_channel_insert_point(JBWArrayEditor *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_INSERT_POINT
		fprintf(stderr,"Editor channel insert point: start\n");
	#endif
	int i=dialog_position_new
		((GtkWindow*)editor->window,gettext("Insert point"),dlg->n+1);
	if (i) jbw_array_editor_insert_row(dlg,i-1);
	#if DEBUG_EDITOR_CHANNEL_INSERT_POINT
		fprintf(stderr,"Editor channel insert point: end\n");
	#endif
}

void editor_channel_delete_point(JBWArrayEditor *dlg)
{
	int i;
	#if DEBUG_EDITOR_CHANNEL_DELETE_POINT
		fprintf(stderr,"Editor channel delete point: start\n");
	#endif
	i=dialog_position_new
		((GtkWindow*)editor->window,gettext("Delete point"),dlg->n);
	if (i) jbw_array_editor_remove_row(dlg,i-1);
	#if DEBUG_EDITOR_CHANNEL_DELETE_POINT
		fprintf(stderr,"Editor channel delete point: end\n");
	#endif
}

void editor_channel_get_transient(EditorChannel *dlg)
{
	int i, j = dlg->editor_transient->n;
	JBFLOAT y[j],z[j],r[j];
	SectionPoint *sp;
	TransientSection *ts = dlg->c->cg->cs[dlg->ics].ts + dlg->its;
	#if DEBUG_EDITOR_CHANNEL_GET_TRANSIENT
		fprintf(stderr,"Editor channel get transient: start\n");
		fprintf(stderr,"ECGT j=%d ics=%d its=%d\n",j,dlg->ics,dlg->its);
	#endif
	sp = jb_try_realloc(ts->sp, j * sizeof(SectionPoint));
	if (!sp)
	{
		jbw_show_error2
			(gettext("Transient section"),gettext("Not enough memory"));
		#if DEBUG_EDITOR_CHANNEL_GET_TRANSIENT
			fprintf(stderr,"\nEditor channel get transient: end\n");
		#endif
		gtk_main_quit();
	}
	ts->sp = sp;
	jbw_array_editor_get_column_float(dlg->editor_transient,0,y);
	jbw_array_editor_get_column_float(dlg->editor_transient,1,z);
	jbw_array_editor_get_column_float(dlg->editor_transient,2,r);
	#if INTERFACE == INTERFACE_SCIENTIFIC
		int t[j];
		jbw_array_editor_get_column_int(dlg->editor_transient,3,t);
		for (i=j; --i>=0;)
			section_point_open(ts->sp + i, y[i], z[i], r[i], t[i]);
	#else
		for (i=j; --i>=0;) section_point_open(ts->sp + i, y[i], z[i], r[i], 0);
	#endif
	ts->n = --j;
	ts->zmin = jbm_varray_min(&(ts->sp->z),sizeof(SectionPoint),j);
	if (ts->hmax == 0.)
		ts->hmax = fminl(ts->sp[0].z, ts->sp[ts->n].z) - ts->zmin;
	#if DEBUG_EDITOR_CHANNEL_GET_TRANSIENT
		transient_section_print(ts,stderr);
		fprintf(stderr,"Editor channel get transient: end\n");
	#endif
}

void editor_channel_update_transient(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_UPDATE_TRANSIENT
		fprintf(stderr,"Editor channel update transient: start\n");
	#endif
	editor_channel_get_transient(dlg);
	gtk_widget_set_sensitive((GtkWidget*)dlg->button_transient_up,
		(dlg->ics!=dlg->c->cg->n) || (dlg->its!=dlg->c->cg->cs[dlg->ics].n));
	gtk_widget_set_sensitive((GtkWidget*)dlg->button_transient_down,
		dlg->ics || dlg->its);
	gtk_widget_set_sensitive((GtkWidget*)dlg->button_delete_transient,
		dlg->c->cg->cs[dlg->ics].ts[dlg->its].n > 1);
#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
	jbw_graphic_draw = editor_channel_draw_transient;
	jbw_graphic_expose_event();
#endif
	#if DEBUG_EDITOR_CHANNEL_UPDATE_TRANSIENT
		fprintf(stderr,"Editor channel update transient: end\n");
	#endif
}

void editor_channel_open_transient(EditorChannel *dlg,TransientSection *ts)
{
	int i, n = ts->n + 1;
	JBFLOAT y[n],z[n],r[n];
	#if INTERFACE == INTERFACE_SCIENTIFIC
		int t[n];
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

void editor_channel_up_transient(EditorChannel *dlg)
{
	int i;
	CrossSection *cs = dlg->c->cg->cs + dlg->ics;
	#if DEBUG_EDITOR_CHANNEL_UP_TRANSIENT
		fprintf(stderr,"Editor channel up transient: start\n");
		fprintf(stderr,"ECUT its=%d\n",dlg->its);
	#endif
	editor_channel_get_transient(dlg);
	i = dlg->its + 1;
	if (i > cs->n)
	{
		g_signal_handler_block(dlg->notebook,dlg->id_notebook);
		g_signal_handler_block(dlg->combo_section,dlg->id_combo_section);
		editor_channel_get_section(dlg);
		++dlg->ics;
		++cs;
		i = dlg->its = 0;
		editor_channel_open_section(dlg, cs);
		gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_section),dlg->ics);
		g_signal_handler_unblock(dlg->combo_section,dlg->id_combo_section);
		g_signal_handler_unblock(dlg->notebook,dlg->id_notebook);
	}
	gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_transient),i);
	gtk_notebook_set_current_page
		(dlg->notebook,EDITOR_CHANNEL_PAGE_TRANSIENT);
	#if DEBUG_EDITOR_CHANNEL_UP_TRANSIENT
		fprintf(stderr,"ECUT its=%d\n",dlg->its);
		fprintf(stderr,"Editor channel up transient: end\n");
	#endif
}

void editor_channel_down_transient(EditorChannel *dlg)
{
	int i;
	CrossSection *cs = dlg->c->cg->cs + dlg->ics;
	#if DEBUG_EDITOR_CHANNEL_DOWN_TRANSIENT
		fprintf(stderr,"Editor channel down transient: start\n");
	#endif
	editor_channel_get_transient(dlg);
	i = dlg->its - 1;
	if (i < 0)
	{
		g_signal_handler_block(dlg->notebook,dlg->id_notebook);
		g_signal_handler_block(dlg->combo_section,dlg->id_combo_section);
		editor_channel_get_section(dlg);
		--dlg->ics;
		--cs;
		i = dlg->its = cs->n;
		editor_channel_open_section(dlg,cs);
		gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_section),dlg->ics);
		g_signal_handler_unblock(dlg->combo_section,dlg->id_combo_section);
		g_signal_handler_unblock(dlg->notebook,dlg->id_notebook);
	}
	gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_transient),i);
	gtk_notebook_set_current_page
		(dlg->notebook,EDITOR_CHANNEL_PAGE_TRANSIENT);
	#if DEBUG_EDITOR_CHANNEL_DOWN_TRANSIENT
		fprintf(stderr,"Editor channel down transient: end\n");
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

void editor_channel_get_section(EditorChannel *dlg)
{
	int i, j = dlg->editor_section->n;
	char *name[j];
	JBFLOAT beta[j];
	CrossSection *cs = dlg->c->cg->cs + dlg->ics;
	#if DEBUG_EDITOR_CHANNEL_GET_SECTION
		fprintf(stderr,"Editor channel get section: start\n");
		fprintf(stderr,"ECGS n=%d ics=%d\n",j,dlg->ics);
	#endif
	jbw_array_editor_get_column_float(dlg->editor_section,0,cs->t);
	jbw_array_editor_get_column_float(dlg->editor_section,1,beta);
	jbw_array_editor_get_column(dlg->editor_section,2,name);
	for (i=j; --i>=0;)
	{
		cs->ts[i].contraction = beta[i];
		cs->ts[i].t = cs->t[i];
		g_free(cs->ts[i].name);
		cs->ts[i].name = jb_strdup(name[i]);
		if (!cs->ts[i].name)
		{
			jbw_show_error2
				(gettext("Cross section"),gettext("Not enough memory"));
			#if DEBUG_EDITOR_CHANNEL_GET_SECTION
				fprintf(stderr,"\nEditor channel get section: end\n");
			#endif
			gtk_main_quit();
		}
	}
	cs->x = jbw_float_entry_get_value(dlg->entry_x);
	cs->y = jbw_float_entry_get_value(dlg->entry_y);
	cs->angle = jbw_float_entry_get_value(dlg->entry_angle);
	cs->type = jbw_array_radio_buttons_get_active(dlg->button_section);
	cs->control = jbw_int_entry_get_value(dlg->entry_control);
	cs->tolerance = jbw_float_entry_get_value(dlg->entry_tolerance);
	cs->time = jbw_float_entry_get_value(dlg->entry_time);
	cs->parameter = jbw_float_entry_get_value(dlg->entry_parameter);
	channel_geometry_update_x(dlg->c->cg,0);
	jbw_array_editor_set_column_float(dlg->editor_geometry,0,dlg->c->cg->x);
	#if DEBUG_EDITOR_CHANNEL_GET_SECTION
		cross_section_print(cs,stderr);
		fprintf(stderr,"Editor channel get section: end\n");
	#endif
}

void editor_channel_update_section(EditorChannel *dlg)
{
	CrossSection *cs;
	#if DEBUG_EDITOR_CHANNEL_UPDATE_SECTION
		fprintf(stderr,"Editor channel update section: start\n");
	#endif
	editor_channel_get_section(dlg);
	gtk_widget_set_sensitive((GtkWidget*)dlg->button_section_up,
		dlg->ics != dlg->c->cg->n);
	gtk_widget_set_sensitive((GtkWidget*)dlg->button_section_down,dlg->ics);
	cs = dlg->c->cg->cs + dlg->ics;
	gtk_widget_set_sensitive((GtkWidget*)dlg->button_delete_section, cs->n > 0);
	switch (cs->type)
	{
	case CROSS_SECTION_TYPE_TIME:
		gtk_widget_hide((GtkWidget*)dlg->table_section_type);
		jbw_array_editor_show_column(dlg->editor_section,0);
		goto type_normal;
	case CROSS_SECTION_TYPE_CONTROL_Q:
		gtk_label_set_text(dlg->label_parameter,gettext("Discharge"));
		break;
	case CROSS_SECTION_TYPE_CONTROL_H:
		gtk_label_set_text(dlg->label_parameter,gettext("Depth"));
		break;
	default:
		gtk_label_set_text(dlg->label_parameter,gettext("Level"));
	}
	gtk_widget_show((GtkWidget*)dlg->table_section_type);
	jbw_array_editor_hide_column(dlg->editor_section,0);
type_normal:
#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
	jbw_graphic_draw = editor_channel_draw_section;
	jbw_graphic_expose_event();
#endif
	#if DEBUG_EDITOR_CHANNEL_UPDATE_SECTION
		fprintf(stderr,"Editor channel update section: end\n");
	#endif
}

void editor_channel_open_section(EditorChannel *dlg,CrossSection *cs)
{
	int i=cs->n+1;
	char *name[i];
	JBFLOAT beta[i];
	#if DEBUG_EDITOR_CHANNEL_OPEN_SECTION
		fprintf(stderr,"Editor channel open section: start\n");
		fprintf(stderr,"ECOS: i=%d ics %d\n",i,dlg->ics);
		cross_section_print(cs,stderr);
	#endif
	g_signal_handler_block(dlg->notebook,dlg->id_notebook);
	gtk_notebook_set_current_page(dlg->notebook,EDITOR_CHANNEL_PAGE_SECTION);
	#if DEBUG_EDITOR_CHANNEL_OPEN_SECTION
		cross_section_print(cs,stderr);
	#endif
	jbw_float_entry_set_value(dlg->entry_x,cs->x);
	jbw_float_entry_set_value(dlg->entry_y,cs->y);
	jbw_float_entry_set_value(dlg->entry_angle,cs->angle);
	jbw_int_entry_set_value(dlg->entry_control,cs->control);
	jbw_float_entry_set_value(dlg->entry_tolerance,cs->tolerance);
	jbw_float_entry_set_value(dlg->entry_time,cs->time);
	jbw_float_entry_set_value(dlg->entry_parameter,cs->parameter);
	jbw_array_editor_set_rows(dlg->editor_section,i);
	while (--i>=0)
	{
		beta[i] = cs->ts[i].contraction;
		name[i] = cs->ts[i].name;
	}
	jbw_array_editor_set_column_float(dlg->editor_section,0,cs->t);
	jbw_array_editor_set_column_float(dlg->editor_section,1,beta);
	jbw_array_editor_set_column(dlg->editor_section,2,name);
	dlg->its=0;
	editor_channel_open_transient(dlg,cs->ts);
	jbw_array_radio_buttons_set_active(dlg->button_section,cs->type,1);
	editor_channel_update_section(dlg);
	g_signal_handler_block(dlg->combo_transient,dlg->id_combo_transient);
	gtk_combo_box_text_remove_all(dlg->combo_transient);
	for (i=cs->n+1; --i>=0;)
		gtk_combo_box_text_prepend_text(dlg->combo_transient,cs->ts[i].name);
	g_signal_handler_unblock(dlg->combo_transient,dlg->id_combo_transient);
	gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_transient),0);
	gtk_notebook_set_current_page(dlg->notebook,EDITOR_CHANNEL_PAGE_SECTION);
	g_signal_handler_unblock(dlg->notebook,dlg->id_notebook);
	#if DEBUG_EDITOR_CHANNEL_OPEN_SECTION
		fprintf(stderr,"Editor channel open section: end\n");
	#endif
}

void editor_channel_set_section(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_SET_SECTION
		fprintf(stderr,"Editor channel set section: start\n");
	#endif
	int i=gtk_combo_box_get_active(GTK_COMBO_BOX(dlg->combo_section));
	CrossSection *cs = dlg->c->cg->cs;
	#if DEBUG_EDITOR_CHANNEL_SET_SECTION
		fprintf(stderr,"ECSS ics %d\n",i);
	#endif
	if (i>=0)
	{
		editor_channel_get_section(dlg);
		#if DEBUG_EDITOR_CHANNEL_SET_SECTION
			cross_section_print(cs+i,stderr);
		#endif
		dlg->ics = i;
		editor_channel_open_section(dlg,cs+i);
	}
	#if DEBUG_EDITOR_CHANNEL_SET_SECTION
		fprintf(stderr,"Editor channel set section: end\n");
	#endif
}

void editor_channel_up_section(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_UP_SECTION
		fprintf(stderr,"Editor channel up section: start\n");
	#endif
	gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_section),dlg->ics+1);
	#if DEBUG_EDITOR_CHANNEL_UP_SECTION
		fprintf(stderr,"Editor channel up section: end\n");
	#endif
}

void editor_channel_down_section(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_DOWN_SECTION
		fprintf(stderr,"Editor channel down section: start\n");
	#endif
	gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_section),dlg->ics-1);
	#if DEBUG_EDITOR_CHANNEL_DOWN_SECTION
		fprintf(stderr,"Editor channel down section: end\n");
	#endif
}

void editor_channel_insert_section(EditorChannel *dlg)
{
	int i;
	TransientSection *ts;
	CrossSection *cs;
	#if DEBUG_EDITOR_CHANNEL_INSERT_SECTION
		fprintf(stderr,"Editor channel insert section: start\n");
	#endif
	editor_channel_update_section(dlg);
	i=dialog_position_new((GtkWindow*)dlg->window,
		gettext("New transient section"),dlg->editor_section->n+1);
	#if DEBUG_EDITOR_CHANNEL_INSERT_SECTION
		fprintf(stderr,"ECNS i=%d\n",i);
	#endif
	if (!i) goto exit2;
	ts = g_try_malloc(sizeof(TransientSection));
	if (!ts)
	{
		jbw_show_error2
			(gettext("Transient section"),gettext("Not enough memory"));
		goto exit2;
	}
	if (!transient_section_copy(ts,tsnew))
	{
		g_free(ts);
		goto exit2;
	}
	cs = dlg->c->cg->cs + dlg->ics;
	if (!cross_section_insert_transient(cs,ts,--i))
	{
		transient_section_delete(ts);
		g_free(ts);
		goto exit2;
	}
	#if DEBUG_EDITOR_CHANNEL_INSERT_SECTION
		cross_section_print(cs,stderr);
	#endif
	editor_channel_open_section(dlg,cs);
exit2:
	#if DEBUG_EDITOR_CHANNEL_INSERT_SECTION
		fprintf(stderr,"Editor channel insert section: end\n");
	#endif
	return;
}

void editor_channel_delete_section(EditorChannel *dlg)
{
	int i;
	CrossSection *cs;
	#if DEBUG_EDITOR_CHANNEL_DELETE_SECTION
		fprintf(stderr,"Editor channel delete section: start\n");
	#endif
	editor_channel_update_section(dlg);
	i=dialog_position_new((GtkWindow*)dlg->window,
		gettext("Delete transient section"),dlg->editor_section->n);
	if (!i) return;
	--i;
	cs = dlg->c->cg->cs + dlg->ics;
	jbw_array_editor_remove_row(dlg->editor_section,i);
	cross_section_delete_transient(cs,i);
	editor_channel_open_section(dlg,cs);
	#if DEBUG_EDITOR_CHANNEL_DELETE_SECTION
		fprintf(stderr,"Editor channel delete section: end\n");
	#endif
}

void editor_channel_draw_section()
{
	#if DEBUG_EDITOR_CHANNEL_DRAW_SECTION
		fprintf(stderr,"Editor channel draw section: start\n");
	#endif
	int i,j,n,nmax;
	JBDOUBLE k1,k2;
	JBFLOAT *y,*z;
	CrossSection *cs = editor->c->cg->cs + editor->ics;
	TransientSection *ts = cs->ts;
	JBWGraphic *graphic = editor->graphic_section;
	nmax = ts->n;
	jbm_varray_maxmin
		(&(ts->sp->y),sizeof(SectionPoint),nmax,&graphic->xmax,&graphic->xmin);
	jbm_varray_maxmin
		(&(ts->sp->z),sizeof(SectionPoint),nmax,&graphic->ymax,&graphic->ymin);
	for (i=0; ++i<=cs->n;)
	{
		++ts;
		nmax = jbm_max(nmax, ts->n);
		jbm_varray_maxmin
			(&(ts->sp->y),sizeof(SectionPoint),ts->n,&k2,&k1);
		graphic->xmax = fmaxl(graphic->xmax, k2);
		graphic->xmin = fminl(graphic->xmin, k1);
		jbm_varray_maxmin
			(&(ts->sp->z),sizeof(SectionPoint),ts->n,&k2,&k1);
		graphic->ymax = fmaxl(graphic->ymax, k2);
		graphic->ymin = fminl(graphic->ymin, k1);
	}
	jbw_graphic_labels(graphic);
	y=(JBFLOAT*)g_malloc((++nmax)*2*sizeof(JBFLOAT));
	z=y+nmax;
	for (i=0, ts=cs->ts; i<=cs->n; ++i, ++ts)
	{
		for (j=n=ts->n+1; --j>=0;) y[j]=ts->sp[j].y, z[j]=ts->sp[j].z;
		k1 = ((float)i) / (cs->n + 1);
		jbw_graphic_draw_line(graphic,0.,k1,1.-k1,y,z,n);
	}
	g_free(y);
	jbw_graphic_draw_logo(graphic);
	#if DEBUG_EDITOR_CHANNEL_DRAW_SECTION
		fprintf(stderr,"Editor channel draw section: end\n");
	#endif
}

void editor_channel_get_geometry(EditorChannel *dlg)
{
	int i, j = dlg->editor_geometry->n;
	char *name[j];
	ChannelGeometry *cg = dlg->c->cg;
	#if DEBUG_EDITOR_CHANNEL_GET_GEOMETRY
		fprintf(stderr,"Editor channel get geometry: start\n");
	#endif
	channel_geometry_update_x(cg,0);
	jbw_array_editor_set_column_float(dlg->editor_geometry,0,cg->x);
	jbw_array_editor_get_column(dlg->editor_geometry,1,name);
	for (i=j; --i>=0;)
	{
		g_free(cg->cs[i].name);
		cg->cs[i].name = jb_strdup(name[i]);
		if (!cg->cs[i].name)
		{
			jbw_show_error2(gettext("Geometry"),gettext("Not enough memory"));
			#if DEBUG_EDITOR_CHANNEL_GET_GEOMETRY
				fprintf(stderr,"\nEditor channel get geometry: end\n");
			#endif
			gtk_main_quit();
		}
	}
	#if DEBUG_EDITOR_CHANNEL_GET_GEOMETRY
		channel_geometry_print(cg,stderr);
		fprintf(stderr,"Editor channel get geometry: end\n");
	#endif
}

void editor_channel_update_geometry(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_UPDATE_GEOMETRY
		fprintf(stderr,"Editor channel update geometry: start\n");
	#endif
	editor_channel_get_geometry(dlg);
	gtk_widget_set_sensitive((GtkWidget*)dlg->button_delete_geometry,
		dlg->c->cg->n > 1);
	jbw_array_editor_set_column_sensitive(dlg->editor_geometry,0,0);
#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
	#if DEBUG_EDITOR_CHANNEL_UPDATE_GEOMETRY
		fprintf(stderr,"ECUG: drawing\n");
	#endif
	jbw_graphic_draw = editor_channel_draw_geometry;
	jbw_graphic_expose_event();
#endif
	#if DEBUG_EDITOR_CHANNEL_UPDATE_GEOMETRY
		fprintf(stderr,"Editor channel update geometry: end\n");
	#endif
}

void editor_channel_open_geometry(EditorChannel *dlg,ChannelGeometry *cg)
{
	int i=cg->n+1;
	char *name[i];
	#if DEBUG_EDITOR_CHANNEL_OPEN_GEOMETRY
		fprintf(stderr,"Editor channel open geometry: start\n");
		channel_geometry_print(cg,stderr);
	#endif
	g_signal_handler_block(dlg->notebook,dlg->id_notebook);
	gtk_notebook_set_current_page(dlg->notebook,EDITOR_CHANNEL_PAGE_GEOMETRY);
	jbw_array_editor_set_rows(dlg->editor_geometry,i);
	while (--i>=0) name[i]=cg->cs[i].name;
	jbw_array_editor_set_column(dlg->editor_geometry,1,name);
	jbw_array_editor_set_column_float(dlg->editor_geometry,0,cg->x);
	dlg->ics = 0;
	editor_channel_open_section(dlg,cg->cs);
	editor_channel_update_geometry(dlg);
	for (i=0; i<cg->n+1; ++i)
		gtk_combo_box_text_append_text(dlg->combo_section,name[i]);
	gtk_notebook_set_current_page(dlg->notebook,EDITOR_CHANNEL_PAGE_GEOMETRY);
	g_signal_handler_unblock(dlg->notebook,dlg->id_notebook);
	#if DEBUG_EDITOR_CHANNEL_OPEN_GEOMETRY
		fprintf(stderr,"Editor channel open geometry: end\n");
	#endif
}

void editor_channel_insert_geometry(EditorChannel *dlg)
{
	int i;
	CrossSection *cs;
	ChannelGeometry *cg;
	#if DEBUG_EDITOR_CHANNEL_INSERT_GEOMETRY
		fprintf(stderr,"Editor channel insert geometry: start\n");
	#endif
	editor_channel_update_geometry(dlg);
	i=dialog_position_new((GtkWindow*)dlg->window,gettext("New cross section"),
		dlg->editor_geometry->n+1);
	#if DEBUG_EDITOR_CHANNEL_INSERT_GEOMETRY
		fprintf(stderr,"ECNG i=%d\n",i);
	#endif
	if (!i) goto exit2;
	cs = g_try_malloc(sizeof(CrossSection));
	if (!cs)
	{
		jbw_show_error2(gettext("Cross section"),gettext("Not enough memory"));
		goto exit2;
	}
	if (!cross_section_copy(cs,csnew))
	{
		g_free(cs);
		goto exit2;
	}
	cg = dlg->c->cg;
	channel_geometry_insert_section(cg,cs,--i);
	#if DEBUG_EDITOR_CHANNEL_INSERT_GEOMETRY
		channel_geometry_print(cg,stderr);
	#endif
	editor_channel_open_geometry(dlg,cg);
exit2:
	#if DEBUG_EDITOR_CHANNEL_INSERT_GEOMETRY
		fprintf(stderr,"Editor channel insert geometry: end\n");
	#endif
	return;
}

void editor_channel_delete_geometry(EditorChannel *dlg)
{
	int i;
	ChannelGeometry *cg;
	#if DEBUG_EDITOR_CHANNEL_DELETE_GEOMETRY
		fprintf(stderr,"Editor channel delete geometry: start\n");
	#endif
	editor_channel_update_geometry(dlg);
	i=dialog_position_new((GtkWindow*)dlg->window,
		gettext("Delete cross section"),dlg->editor_geometry->n);
	if (!i) return;
	--i;
	cg = dlg->c->cg;
	jbw_array_editor_remove_row(dlg->editor_geometry,i);
	channel_geometry_delete_section(cg,i);
	editor_channel_open_geometry(dlg,cg);
	#if DEBUG_EDITOR_CHANNEL_DELETE_GEOMETRY
		fprintf(stderr,"Editor channel delete geometry: end\n");
	#endif
}

void editor_channel_draw_geometry()
{
	#if DEBUG_EDITOR_CHANNEL_DRAW_GEOMETRY
		fprintf(stderr,"Editor channel draw geometry: start\n");
	#endif
	int i, j, n=editor->c->cg->n;
	TransientSection *ts;
	CrossSection *cs;
	JBWGraphic *graphic;
	JBDOUBLE k1,k2,xmax,xmin;
	#if DEBUG_EDITOR_CHANNEL_DRAW_GEOMETRY
		fprintf(stderr,"ECDG cg=%ld\n",(size_t)editor->c->cg);
		channel_geometry_print(editor->c->cg,stderr);
		fprintf(stderr,"ECDG n=%d\n",n);
	#endif
	JBFLOAT x[n+n+2],y[n+n+2],x2[n+1],y2[n+1],*xx,*yy;
	graphic = editor->graphic_geometry;
	cs=editor->c->cg->cs;
	for (i=0, cs=editor->c->cg->cs, xx=x, yy=y; i<=n; ++i, ++cs, ++xx, ++yy)
	{
		ts = cs->ts;
		jbm_varray_maxmin
			(&(ts->sp->y),sizeof(SectionPoint),ts->n,&xmax,&xmin);
		for (j=0; ++j<=cs->n;)
		{
			++ts;
			jbm_varray_maxmin
				(&(ts->sp->y),sizeof(SectionPoint),ts->n,&k2,&k1);
			xmax=fmaxl(k2,xmax);
			xmin=fminl(k1,xmin);
		}
		x2[i]=cs->x;
		*xx = cs->x + xmax * cos(cs->angle * M_PI / 180.);
		y2[i]=cs->y;
		*yy = cs->y + xmax * sin(cs->angle * M_PI / 180.);
		++xx, ++yy;
		*xx = cs->x + xmin * cos(cs->angle * M_PI / 180.);
		*yy = cs->y + xmin * sin(cs->angle * M_PI / 180.);
	}
	jbm_farray_maxmin(x,n+n+1,&graphic->xmax,&graphic->xmin);
	jbm_farray_maxmin(y,n+n+1,&graphic->ymax,&graphic->ymin);
	jbw_graphic_map_resize(graphic);
	jbw_graphic_labels(graphic);
#if JBW_GRAPHIC==JBW_GRAPHIC_GLUT
	glLoadIdentity();
	glOrtho(graphic->xmin,graphic->xmax,graphic->ymin,graphic->ymax,-1.,1.);
	#if PRECISION==1
		jbw_draw_segmentsf(0.,0.,1.,x,y,n+n+2);
		jbw_draw_linesf(1.,0.,0.,x2,y2,n+1);
	#else
		jbw_draw_segmentsd(0.,0.,1.,x,y,n+n+2);
		jbw_draw_linesd(1.,0.,0.,x2,y2,n+1);
	#endif
#else
//	jbw_draw_segment(graphic->cr,0.,0.,1.,x,y,n+n+2);
	jbw_graphic_draw_lines(graphic,x2,y2,0,0,0,n+1);
#endif
	jbw_graphic_draw_logo(graphic);
	#if DEBUG_EDITOR_CHANNEL_DRAW_GEOMETRY
		fprintf(stderr,"Editor channel draw geometry: end\n");
	#endif
}

void editor_channel_get_if(EditorChannel *dlg)
{
	int i, j = dlg->editor_if->n;
	JBFLOAT x[j],y[j],z[j];
	Point3 *p;
	InitialFlow *fic = dlg->c->fic;
	#if DEBUG_EDITOR_CHANNEL_GET_IF
		fprintf(stderr,"Editor channel get if: start\n");
	#endif
	fic->type = jbw_array_radio_buttons_get_active(dlg->button_if);
	#if DEBUG_EDITOR_CHANNEL_GET_IF
		fprintf(stderr,"ECGIF type=%d\n",fic->type);
	#endif
	switch (fic->type)
	{
	case INITIAL_FLOW_DRY:
	case INITIAL_FLOW_STEADY:
		jb_free_null((void**)&fic->p);
		#if DEBUG_EDITOR_CHANNEL_GET_IF
			fprintf(stderr,"Editor channel get if: end\n");
		#endif
		return;
	}
	p=jb_try_realloc(fic->p,j*sizeof(Point3));
	if (!p) goto exit2;
	fic->p = p;
	jbw_array_editor_get_column_float(dlg->editor_if,0,x);
	jbw_array_editor_get_column_float(dlg->editor_if,1,y);
	jbw_array_editor_get_column_float(dlg->editor_if,2,z);
	for (i=j; --i>=0;) point3_open(p+i,x[i],y[i],z[i]);
	fic->n = --j;
	#if DEBUG_EDITOR_CHANNEL_GET_IF
		fprintf(stderr,"Editor channel get if: end\n");
	#endif
	return;

exit2:
	jbw_show_error2
		(gettext("Flow initial conditions"),gettext("Not enough memory"));
	#if DEBUG_EDITOR_CHANNEL_GET_IF
		fprintf(stderr,"\nEditor channel get if: end\n");
	#endif
	gtk_main_quit();
}

void editor_channel_update_if(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_UPDATE_IF
		fprintf(stderr,"Editor channel update if: start\n");
	#endif
	editor_channel_get_if(dlg);
	#if DEBUG_EDITOR_CHANNEL_UPDATE_IF
		fprintf(stderr,"ECUIF type=%d\n",dlg->c->fic->type);
	#endif
	switch (dlg->c->fic->type)
	{
	case INITIAL_FLOW_DRY:
	case INITIAL_FLOW_STEADY:
		gtk_widget_hide((GtkWidget*)dlg->paned_if);
		#if DEBUG_EDITOR_CHANNEL_UPDATE_IF
			fprintf(stderr,"Editor channel update if: end\n");
		#endif
		return;
	case INITIAL_FLOW_XQH:
		jbw_array_editor_set_title(dlg->editor_if,2,gettext("Depth"));
		jbw_graphic_set_zlabel(dlg->graphic_if,gettext("Depth"));
		break;
	case INITIAL_FLOW_XQZ:
		jbw_array_editor_set_title(dlg->editor_if,2,gettext("Level"));
		jbw_graphic_set_zlabel(dlg->graphic_if,gettext("Level"));
	}
	gtk_widget_show((GtkWidget*)dlg->paned_if);
	gtk_widget_set_sensitive((GtkWidget*)dlg->button_delete_if,
		dlg->c->fic->n > 0);
#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
	jbw_graphic_draw = editor_channel_draw_if;
	jbw_graphic_expose_event();
#endif
	#if DEBUG_EDITOR_CHANNEL_UPDATE_IF
		fprintf(stderr,"Editor channel update if: end\n");
	#endif
}

void editor_channel_open_if(EditorChannel *dlg,InitialFlow *fic)
{
	int i = fic->n + 1;
	JBFLOAT x[i],Q[i],h[i];
	#if DEBUG_EDITOR_CHANNEL_OPEN_IF
		fprintf(stderr,"Editor channel open if: start\n");
	#endif
	g_signal_handler_block(dlg->notebook,dlg->id_notebook);
	gtk_notebook_set_current_page(dlg->notebook,EDITOR_CHANNEL_PAGE_IF);
	g_signal_handler_unblock(dlg->notebook,dlg->id_notebook);
	jbw_array_radio_buttons_set_active(dlg->button_if,fic->type,1);
	switch (fic->type)
	{
	case INITIAL_FLOW_XQH:
	case INITIAL_FLOW_XQZ:
		jbw_array_editor_set_rows(dlg->editor_if,i);
		while (--i>=0) x[i]=fic->p[i].x, Q[i]=fic->p[i].y, h[i]=fic->p[i].z;
		jbw_array_editor_set_column_float(dlg->editor_if,0,x);
		jbw_array_editor_set_column_float(dlg->editor_if,1,Q);
		jbw_array_editor_set_column_float(dlg->editor_if,2,h);
	}
	editor_channel_update_if(dlg);
	#if DEBUG_EDITOR_CHANNEL_OPEN_IF
		fprintf(stderr,"Editor channel open if: end\n");
	#endif
}

void editor_channel_insert_if(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_INSERT_IF
		fprintf(stderr,"Editor channel insert if: start\n");
	#endif
	editor_channel_insert_point(dlg->editor_if);
	editor_channel_update_if(dlg);
	#if DEBUG_EDITOR_CHANNEL_INSERT_IF
		fprintf(stderr,"Editor channel insert if: end\n");
	#endif
}

void editor_channel_delete_if(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_DELETE_IF
		fprintf(stderr,"Editor channel delete if: start\n");
	#endif
	editor_channel_delete_point(dlg->editor_if);
	editor_channel_update_if(dlg);
	#if DEBUG_EDITOR_CHANNEL_DELETE_IF
		fprintf(stderr,"Editor channel delete if: end\n");
	#endif
}

void editor_channel_draw_if()
{
	InitialFlow *fic = editor->c->fic;
	int i, n = fic->n + 1;
	JBFLOAT x[n],y[n],z[n];
	#if DEBUG_EDITOR_CHANNEL_DRAW_IF
		fprintf(stderr,"Editor channel draw if: start\n");
	#endif
	for (i=n; --i>=0;) x[i]=fic->p[i].x, y[i]=fic->p[i].y, z[i]=fic->p[i].z;
	jbw_graphic_draw_lines(editor->graphic_if,x,y,0,z,0,n);
	jbw_graphic_draw_logo(editor->graphic_if);
	#if DEBUG_EDITOR_CHANNEL_DRAW_IF
		fprintf(stderr,"Editor channel draw if: end\n");
	#endif
}

#if EDITOR_BF

void editor_channel_update_inlet(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_UPDATE_INLET
		fprintf(stderr,"Editor channel update inlet: start\n");
		boundary_flow_print(dlg->c->bf, stderr);
	#endif
	editor_bf_update(dlg->editor_bf_inlet,dlg->c->bf,EDITOR_BF_TYPE_IO);
	#if DEBUG_EDITOR_CHANNEL_UPDATE_INLET
		fprintf(stderr,"Editor channel update inlet: end\n");
	#endif
}

void editor_channel_open_inlet(EditorChannel *dlg,BoundaryFlow *bf)
{
	#if DEBUG_EDITOR_CHANNEL_OPEN_INLET
		fprintf(stderr,"Editor channel open inlet: start\n");
	#endif
	g_signal_handler_block(dlg->notebook,dlg->id_notebook);
	gtk_notebook_set_current_page(dlg->notebook,EDITOR_CHANNEL_PAGE_BF);
	g_signal_handler_unblock(dlg->notebook,dlg->id_notebook);
	gtk_notebook_set_current_page
		(dlg->notebook_bf,EDITOR_CHANNEL_BF_PAGE_INLET);
	editor_bf_open(dlg->editor_bf_inlet,bf,EDITOR_BF_TYPE_IO);
	editor_channel_update_inlet(dlg);
	#if DEBUG_EDITOR_CHANNEL_OPEN_INLET
		fprintf(stderr,"Editor channel open inlet: end\n");
	#endif
}

void editor_channel_insert_inlet(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_INSERT_INLET
		fprintf(stderr,"Editor channel insert inlet: start\n");
	#endif
	editor_channel_insert_point(dlg->editor_bf_inlet->editor);
	editor_channel_update_inlet(dlg);
	#if DEBUG_EDITOR_CHANNEL_INSERT_INLET
		fprintf(stderr,"Editor channel insert inlet: end\n");
	#endif
}

void editor_channel_delete_inlet(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_DELETE_INLET
		fprintf(stderr,"Editor channel delete inlet: start\n");
	#endif
	editor_channel_delete_point(dlg->editor_bf_inlet->editor);
	editor_channel_update_inlet(dlg);
	#if DEBUG_EDITOR_CHANNEL_DELETE_INLET
		fprintf(stderr,"Editor channel delete inlet: end\n");
	#endif
}

void editor_channel_draw_inlet()
{
	BoundaryFlow *bf = editor->c->bf;
	#if DEBUG_EDITOR_CHANNEL_DRAW_INLET
		fprintf(stderr,"Editor channel draw inlet: start\n");
	#endif
	jbw_graphic_draw_lines(editor->editor_bf_inlet->graphic,
		bf->p1,bf->p2,0,bf->p3,0,bf->n+1);
	jbw_graphic_draw_logo(editor->editor_bf_inlet->graphic);
	#if DEBUG_EDITOR_CHANNEL_DRAW_INLET
		fprintf(stderr,"Editor channel draw inlet: end\n");
	#endif
}

void editor_channel_update_outlet(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_UPDATE_OUTLET
		fprintf(stderr,"Editor channel update outlet: start\n");
		boundary_flow_print(dlg->c->bf + dlg->c->n, stderr);
	#endif
	editor_bf_update
		(dlg->editor_bf_outlet, dlg->c->bf + dlg->c->n, EDITOR_BF_TYPE_IO);
	#if DEBUG_EDITOR_CHANNEL_UPDATE_OUTLET
		fprintf(stderr,"Editor channel update outlet: end\n");
	#endif
}

void editor_channel_open_outlet(EditorChannel *dlg,BoundaryFlow *bf)
{
	#if DEBUG_EDITOR_CHANNEL_OPEN_OUTLET
		fprintf(stderr,"Editor channel open outlet: start\n");
	#endif
	g_signal_handler_block(dlg->notebook,dlg->id_notebook);
	gtk_notebook_set_current_page(dlg->notebook,EDITOR_CHANNEL_PAGE_BF);
	g_signal_handler_unblock(dlg->notebook,dlg->id_notebook);
	gtk_notebook_set_current_page
		(dlg->notebook_bf,EDITOR_CHANNEL_BF_PAGE_OUTLET);
	editor_bf_open(dlg->editor_bf_outlet,bf,EDITOR_BF_TYPE_IO);
	editor_channel_update_outlet(dlg);
	#if DEBUG_EDITOR_CHANNEL_OPEN_OUTLET
		fprintf(stderr,"Editor channel open outlet: end\n");
	#endif
}

void editor_channel_insert_outlet(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_INSERT_OUTLET
		fprintf(stderr,"Editor channel insert outlet: start\n");
	#endif
	editor_channel_insert_point(dlg->editor_bf_outlet->editor);
	editor_channel_update_outlet(dlg);
	#if DEBUG_EDITOR_CHANNEL_INSERT_OUTLET
		fprintf(stderr,"Editor channel insert outlet: end\n");
	#endif
}

void editor_channel_delete_outlet(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_DELETE_OUTLET
		fprintf(stderr,"Editor channel delete outlet: start\n");
	#endif
	editor_channel_delete_point(dlg->editor_bf_outlet->editor);
	editor_channel_update_outlet(dlg);
	#if DEBUG_EDITOR_CHANNEL_DELETE_OUTLET
		fprintf(stderr,"Editor channel delete outlet: end\n");
	#endif
}

void editor_channel_draw_outlet()
{
	BoundaryFlow *bf = editor->c->bf + editor->c->n;
	#if DEBUG_EDITOR_CHANNEL_DRAW_OUTLET
		fprintf(stderr,"Editor channel draw outlet: start\n");
	#endif
	jbw_graphic_draw_lines(editor->editor_bf_outlet->graphic,
		bf->p1,bf->p2,0,bf->p3,0,bf->n+1);
	jbw_graphic_draw_logo(editor->editor_bf_outlet->graphic);
	#if DEBUG_EDITOR_CHANNEL_DRAW_OUTLET
		fprintf(stderr,"Editor channel draw outlet: end\n");
	#endif
}

void editor_channel_update_inner(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_UPDATE_INNER
		fprintf(stderr,"Editor channel update inner: start\n");
		fprintf(stderr,"ECUI ibf=%d\n",dlg->ibf);
	#endif
	BoundaryFlow *bf = dlg->c->bf + dlg->ibf + 1;
	bf->pos = jbw_int_entry_get_value(dlg->entry_inner_initial);
	bf->pos2 = jbw_int_entry_get_value(dlg->entry_inner_final);
	editor_bf_update(dlg->editor_bf_inner, bf,EDITOR_BF_TYPE_INNER);
	#if DEBUG_EDITOR_CHANNEL_UPDATE_INNER
		fprintf(stderr,"Editor channel update inner: end\n");
	#endif
}

void editor_channel_open_inner(EditorChannel *dlg,BoundaryFlow *bf)
{
	#if DEBUG_EDITOR_CHANNEL_OPEN_INNER
		fprintf(stderr,"Editor channel open inner: start\n");
	#endif
	if (!bf)
	{
		dlg->ibf = 0;
		goto exit2;
	}
	g_signal_handler_block(dlg->notebook,dlg->id_notebook);
	gtk_notebook_set_current_page(dlg->notebook,EDITOR_CHANNEL_PAGE_BF);
	g_signal_handler_unblock(dlg->notebook,dlg->id_notebook);
	gtk_notebook_set_current_page
		(dlg->notebook_bf,EDITOR_CHANNEL_BF_PAGE_INNER);
	#if DEBUG_EDITOR_CHANNEL_OPEN_INNER
		boundary_flow_print(bf,stderr);
	#endif
	jbw_int_entry_set_value(dlg->entry_inner_initial,bf->pos);
	jbw_int_entry_set_value(dlg->entry_inner_final,bf->pos2);
	editor_bf_open(dlg->editor_bf_inner,bf,EDITOR_BF_TYPE_INNER);
	editor_channel_update_inner(dlg);
	#if DEBUG_EDITOR_CHANNEL_OPEN_INNER
		boundary_flow_print(bf,stderr);
	#endif

exit2:
	#if DEBUG_EDITOR_CHANNEL_OPEN_INNER
		channel_print(dlg->c,stderr);
		fprintf(stderr,"Editor channel open inner: end\n");
	#endif
	return;
}

void editor_channel_insert_inner(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_INSERT_INNER
		fprintf(stderr,"Editor channel insert inner: start\n");
	#endif
	editor_channel_insert_point(dlg->editor_bf_inner->editor);
	editor_channel_update_inner(dlg);
	#if DEBUG_EDITOR_CHANNEL_INSERT_INNER
		fprintf(stderr,"Editor channel insert inner: end\n");
	#endif
}

void editor_channel_delete_inner(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_DELETE_INNER
		fprintf(stderr,"Editor channel delete inner: start\n");
	#endif
	editor_channel_delete_point(dlg->editor_bf_inner->editor);
	editor_channel_update_inner(dlg);
	#if DEBUG_EDITOR_CHANNEL_DELETE_INNER
		fprintf(stderr,"Editor channel delete inner: end\n");
	#endif
}

void editor_channel_draw_inner()
{
	BoundaryFlow *bf = editor->c->bf + editor->ibf + 1;
	#if DEBUG_EDITOR_CHANNEL_DRAW_INNER
		fprintf(stderr,"Editor channel draw inner: start\n");
	#endif
	jbw_graphic_draw_lines(editor->editor_bf_inner->graphic,
		bf->p1,bf->p2,0,0,0,bf->n+1);
	jbw_graphic_draw_logo(editor->editor_bf_inner->graphic);
	#if DEBUG_EDITOR_CHANNEL_DRAW_INNER
		fprintf(stderr,"Editor channel draw inner: end\n");
	#endif
}

void editor_channel_update_bf(EditorChannel *dlg)
{
	BoundaryFlow *bf = dlg->c->bf + dlg->ibf + 1;
	#if DEBUG_EDITOR_CHANNEL_UPDATE_BF
		fprintf(stderr,"Editor channel update bf: start\n");
		fprintf(stderr,"ECUBF n=%d ibf=%d\n",dlg->c->n,dlg->ibf);
		boundary_flow_print(bf,stderr);
	#endif
	if (dlg->c->n > 1)
	{
		#if DEBUG_EDITOR_CHANNEL_UPDATE_BF
			boundary_flow_print(bf,stderr);
		#endif
		editor_bf_update(dlg->editor_bf_inner,bf,EDITOR_BF_TYPE_INNER);
		gtk_widget_set_sensitive((GtkWidget*)dlg->button_delete_bf,1);
		gtk_widget_show((GtkWidget*)dlg->editor_bf_inner->table);
	}
	else
	{
		gtk_widget_hide((GtkWidget*)dlg->editor_bf_inner->table);
		gtk_widget_set_sensitive((GtkWidget*)dlg->button_delete_bf,0);
	}
	gtk_widget_set_sensitive((GtkWidget*)dlg->button_bf_up,
		dlg->ibf < dlg->c->n - 2);
	gtk_widget_set_sensitive((GtkWidget*)dlg->button_bf_down,dlg->ibf);
	#if DEBUG_EDITOR_CHANNEL_UPDATE_BF
		boundary_flow_print(bf,stderr);
		fprintf(stderr,"Editor channel update bf: end\n");
	#endif
}

void editor_channel_update_combo_bf(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_UPDATE_COMBO_BF
		fprintf(stderr,"Editor channel update combo bf: start\n");
		fprintf(stderr,"ECUCBF: n=%d\n",dlg->c->n);
	#endif
	char *name;
	BoundaryFlow *bf = dlg->c->bf + dlg->ibf + 1;
	if (dlg->c->n < 2) goto exit1;
	g_signal_handler_block(dlg->combo_bf,dlg->id_combo_bf);
	g_signal_handler_block(dlg->combo_bf,dlg->id_combo_bf2);
	gtk_combo_box_text_remove(dlg->combo_bf,dlg->ibf);
	name = (char*)gtk_entry_get_text(dlg->editor_bf_inner->entry_name);
	gtk_combo_box_text_insert_text(dlg->combo_bf,dlg->ibf,name);
	g_free(bf->name);
	bf->name = jb_strdup(name);
	gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_bf),dlg->ibf);
	g_signal_handler_unblock(dlg->combo_bf,dlg->id_combo_bf2);
	g_signal_handler_unblock(dlg->combo_bf,dlg->id_combo_bf);
exit1:
	#if DEBUG_EDITOR_CHANNEL_UPDATE_COMBO_BF
		fprintf(stderr,"Editor channel update combo bf: end\n");
	#endif
	return;
}

void editor_channel_update_entry_bf(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_UPDATE_ENTRY_BF
		fprintf(stderr,"Editor channel update entry bf: start\n");
		fprintf(stderr,"ECUEBF: n=%d\n",dlg->c->n);
	#endif
	g_signal_handler_block(dlg->editor_bf_inner->entry_name,dlg->id_entry_bf);
	dlg->ibf = gtk_combo_box_get_active(GTK_COMBO_BOX(dlg->combo_bf));
	gtk_entry_set_text(dlg->editor_bf_inner->entry_name,
		(char*)gtk_combo_box_text_get_active_text(dlg->combo_bf));
	editor_channel_open_inner(dlg, dlg->c->bf + dlg->ibf + 1);
	g_signal_handler_unblock(dlg->editor_bf_inner->entry_name,dlg->id_entry_bf);
	editor_channel_update_bf(dlg);
	#if DEBUG_EDITOR_CHANNEL_UPDATE_ENTRY_BF
		fprintf(stderr,"Editor channel update entry bf: end\n");
	#endif
	return;
}

void editor_channel_up_bf(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_UP_BF
		fprintf(stderr,"Editor channel up bf: start\n");
		fprintf(stderr,"ECUBF: ibf=%d\n",dlg->ibf);
	#endif
	gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_bf),dlg->ibf+1);
	#if DEBUG_EDITOR_CHANNEL_UP_BF
		fprintf(stderr,"ECUBF: ibf=%d\n",dlg->ibf);
		fprintf(stderr,"Editor channel up bf: end\n");
	#endif
}

void editor_channel_down_bf(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_DOWN_BF
		fprintf(stderr,"Editor channel down bf: start\n");
		fprintf(stderr,"ECDBF: ibf=%d\n",dlg->ibf);
	#endif
	gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_bf),dlg->ibf-1);
	#if DEBUG_EDITOR_CHANNEL_DOWN_BF
		fprintf(stderr,"ECDBF: ibf=%d\n",dlg->ibf);
		fprintf(stderr,"Editor channel down bf: end\n");
	#endif
}

void editor_channel_open_bf(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_OPEN_BF
		fprintf(stderr,"Editor channel open bf: start\n");
	#endif
	int i, n = dlg->c->n - 1;
	BoundaryFlow *bf = dlg->c->bf + 1;
	g_signal_handler_block(dlg->combo_bf,dlg->id_combo_bf);
	g_signal_handler_block(dlg->combo_bf,dlg->id_combo_bf2);
	g_signal_handler_block(dlg->editor_bf_inner->entry_name,dlg->id_entry_bf);
	for (i=n; --i>=0;)
		gtk_combo_box_text_prepend_text(dlg->combo_bf,bf[i].name);
	gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_bf),0);
	g_signal_handler_unblock(dlg->editor_bf_inner->entry_name,dlg->id_entry_bf);
	g_signal_handler_unblock(dlg->combo_bf,dlg->id_combo_bf2);
	g_signal_handler_unblock(dlg->combo_bf,dlg->id_combo_bf);
	dlg->ibf = 0;
	if (!n) bf=0;
	editor_channel_open_inner(dlg,bf);
	editor_channel_update_bf(dlg);
	#if DEBUG_EDITOR_CHANNEL_OPEN_BF
		fprintf(stderr,"Editor channel open bf: end\n");
	#endif
}

void editor_channel_insert_bf(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_INSERT_BF
		fprintf(stderr,"Editor channel insert bf: start\n");
		channel_print(dlg->c,stderr);
	#endif
	BoundaryFlow *bf;
	bf = (BoundaryFlow*)g_try_malloc(sizeof(BoundaryFlow));
	if (!bf)
	{
		jbw_show_error2
			(gettext("Flow boundary condition"),gettext("Not enough memory"));
		goto exit2;
	}
	if (!boundary_flow_copy(bf,bfnew))
	{
		g_free(bf);
		goto exit2;
	}
	editor_channel_update_bf(dlg);
	channel_insert_bf(dlg->c, bf, dlg->ibf + 1);
	#if DEBUG_EDITOR_CHANNEL_INSERT_BF
		channel_print(dlg->c,stderr);
	#endif
	g_signal_handler_block(dlg->editor_bf_inner->entry_name,dlg->id_entry_bf);
	g_signal_handler_block(dlg->combo_bf,dlg->id_combo_bf2);
	gtk_combo_box_text_insert_text(dlg->combo_bf,dlg->ibf,bf->name);
	gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_bf),dlg->ibf);
	g_signal_handler_unblock(dlg->combo_bf,dlg->id_combo_bf2);
	g_signal_handler_unblock(dlg->editor_bf_inner->entry_name,dlg->id_entry_bf);
	editor_channel_open_inner(dlg,bf);

exit2:
	#if DEBUG_EDITOR_CHANNEL_INSERT_BF
		channel_print(dlg->c,stderr);
		fprintf(stderr,"Editor channel insert bf: end\n");
	#endif
	return;
}

void editor_channel_delete_bf(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_DELETE_BF
		fprintf(stderr,"Editor channel delete bf: start\n");
		channel_print(dlg->c,stderr);
	#endif
	channel_delete_bf(dlg->c, dlg->ibf + 1);
	#if DEBUG_EDITOR_CHANNEL_DELETE_BF
		channel_print(dlg->c,stderr);
	#endif
	gtk_combo_box_text_remove(dlg->combo_bf,dlg->ibf);
	if (dlg->c->n > 1)
	{
		if (dlg->ibf == dlg->c->n - 1) --dlg->ibf;
		g_signal_handler_block(dlg->combo_bf,dlg->id_combo_bf2);
		gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_bf),dlg->ibf);
		g_signal_handler_unblock(dlg->combo_bf,dlg->id_combo_bf2);
	}
	editor_channel_update_bf(dlg);
	#if DEBUG_EDITOR_CHANNEL_DELETE_BF
		channel_print(dlg->c,stderr);
		fprintf(stderr,"ECDBF: ibf=%d\n",dlg->ibf);
		fprintf(stderr,"Editor channel delete bf: end\n");
	#endif
}

#endif

void editor_channel_get_it(EditorChannel *dlg)
{
	int i, j = dlg->editor_it->n;
	JBFLOAT x[j],y[j];
	Point2 *p;
	InitialTransport *it = dlg->c->ct[dlg->ict].it;
	#if DEBUG_EDITOR_CHANNEL_GET_IT
		fprintf(stderr,"Editor channel get it: start\n");
	#endif
	it->type = jbw_array_radio_buttons_get_active(dlg->button_it);
	#if DEBUG_EDITOR_CHANNEL_GET_IT
		fprintf(stderr,"ECGIT type=%d\n",it->type);
	#endif
	switch (it->type)
	{
	case INITIAL_TRANSPORT_DRY:
	case INITIAL_TRANSPORT_STEADY:
		jb_free_null((void**)&it->p);
		#if DEBUG_EDITOR_CHANNEL_GET_IT
			fprintf(stderr,"Editor channel get it: end\n");
		#endif
		return;
	}
	p=jb_try_realloc(it->p,j*sizeof(Point2));
	if (!p) goto exit2;
	it->p = p;
	jbw_array_editor_get_column_float(dlg->editor_it,0,x);
	jbw_array_editor_get_column_float(dlg->editor_it,1,y);
	for (i=j; --i>=0;) point2_open(p+i,x[i],y[i]);
	it->n = --j;
	#if DEBUG_EDITOR_CHANNEL_GET_IT
		fprintf(stderr,"Editor channel get it: end\n");
	#endif
	return;

exit2:
	jbw_show_error2
		(gettext("Transport initial conditions"),gettext("Not enough memory"));
	#if DEBUG_EDITOR_CHANNEL_GET_IT
		fprintf(stderr,"\nEditor channel get it: end\n");
	#endif
	gtk_main_quit();
}

void editor_channel_update_it(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_UPDATE_IT
		fprintf(stderr,"Editor channel update it: start\n");
	#endif
	InitialTransport *it = dlg->c->ct[dlg->ict].it;
	editor_channel_get_it(dlg);
	#if DEBUG_EDITOR_CHANNEL_UPDATE_IT
		fprintf(stderr,"ECUIT type=%d\n",it->type);
	#endif
	switch (it->type)
	{
	case INITIAL_TRANSPORT_DRY:
	case INITIAL_TRANSPORT_STEADY:
		gtk_widget_hide((GtkWidget*)dlg->paned_it);
		#if DEBUG_EDITOR_CHANNEL_UPDATE_IT
			fprintf(stderr,"Editor channel update it: end\n");
		#endif
		return;
	}
	gtk_widget_show((GtkWidget*)dlg->paned_it);
	gtk_widget_set_sensitive((GtkWidget*)dlg->button_delete_it,
		it->n > 0);
#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
	jbw_graphic_draw = dlg->graphic_it->draw;
	jbw_graphic_expose_event();
#endif
	#if DEBUG_EDITOR_CHANNEL_UPDATE_IT
		fprintf(stderr,"Editor channel update it: end\n");
	#endif
}

void editor_channel_open_it(EditorChannel *dlg,InitialTransport *it)
{
	int i = it->n + 1;
	JBFLOAT x[i],c[i];
	#if DEBUG_EDITOR_CHANNEL_OPEN_IT
		fprintf(stderr,"Editor channel open it: start\n");
		initial_transport_print(it,stderr);
	#endif
	g_signal_handler_block(dlg->notebook_transport,dlg->id_notebook_transport);
	gtk_notebook_set_current_page(dlg->notebook_transport,
		EDITOR_CHANNEL_PAGE_TRANSPORT_IT);
	#if DEBUG_EDITOR_CHANNEL_OPEN_IT
		initial_transport_print(it,stderr);
	#endif
	g_signal_handler_unblock
		(dlg->notebook_transport,dlg->id_notebook_transport);
	#if DEBUG_EDITOR_CHANNEL_OPEN_IT
		initial_transport_print(it,stderr);
	#endif
	switch (it->type)
	{
	case INITIAL_TRANSPORT_XC:
		jbw_array_editor_set_rows(dlg->editor_it,i);
		while (--i>=0) x[i]=it->p[i].x, c[i]=it->p[i].y;
		jbw_array_editor_set_column_float(dlg->editor_it,0,x);
		jbw_array_editor_set_column_float(dlg->editor_it,1,c);
	}
	jbw_array_radio_buttons_set_active(dlg->button_it,it->type,1);
	#if DEBUG_EDITOR_CHANNEL_OPEN_IT
		initial_transport_print(it,stderr);
		fprintf(stderr,"Editor channel open it: end\n");
	#endif
}

void editor_channel_insert_it(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_INSERT_IT
		fprintf(stderr,"Editor channel insert it: start\n");
	#endif
	editor_channel_insert_point(dlg->editor_it);
	editor_channel_update_it(dlg);
	#if DEBUG_EDITOR_CHANNEL_INSERT_IT
		fprintf(stderr,"Editor channel insert it: end\n");
	#endif
}

void editor_channel_delete_it(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_DELETE_IT
		fprintf(stderr,"Editor channel delete it: start\n");
	#endif
	editor_channel_delete_point(dlg->editor_it);
	editor_channel_update_it(dlg);
	#if DEBUG_EDITOR_CHANNEL_DELETE_IT
		fprintf(stderr,"Editor channel delete transport: end\n");
	#endif
}

void editor_channel_draw_it()
{
	#if DEBUG_EDITOR_CHANNEL_DRAW_IT
		fprintf(stderr,"Editor channel draw it: start\n");
	#endif
	InitialTransport *it = editor->c->ct[editor->ict].it;
	int i, n = it->n + 1;
	JBFLOAT x[n],y[n];
	for (i=n; --i>=0;) x[i]=it->p[i].x, y[i]=it->p[i].y;
	jbw_graphic_draw_lines(editor->graphic_it,x,y,0,0,0,n);
	jbw_graphic_draw_logo(editor->graphic_it);
	#if DEBUG_EDITOR_CHANNEL_DRAW_IT
		fprintf(stderr,"Editor channel draw it: end\n");
	#endif
}

#if EDITOR_BF

void editor_channel_get_bt(EditorChannel *dlg)
{
	int n;
	BoundaryTransport *bt = dlg->c->ct[dlg->ict].bt + dlg->ibt;
	#if DEBUG_EDITOR_CHANNEL_GET_BT
		fprintf(stderr,"Editor channel get bt: start\n");
		fprintf(stderr,"ECGBT name=%s\n",bt->name);
	#endif
	jb_free_null((void**)&bt->name);
	bt->name = jb_strdup((char*)gtk_entry_get_text(dlg->entry_bt));
	if (!bt->name) goto exit2;
	bt->pos = jbw_int_entry_get_value(dlg->entry_bt_initial);
	bt->pos2 = jbw_int_entry_get_value(dlg->entry_bt_final);
	bt->type = jbw_array_radio_buttons_get_active(dlg->button_bt);
	#if DEBUG_EDITOR_CHANNEL_GET_BT
		fprintf(stderr,"ECGBT type=%d\n",bt->type);
	#endif
	switch (bt->type)
	{
	case BOUNDARY_TRANSPORT_M:
		bt->p1 = jb_try_realloc(bt->p1, 2 * sizeof(JBFLOAT));
		if (!bt->p1) goto exit2;
		bt->p2 = bt->p1 + 1;
		bt->n = 0;
		bt->p1[0] = jbw_float_entry_get_value(dlg->entry_bt_time);
		bt->p2[0] = jbw_float_entry_get_value(dlg->entry_bt_mass);
		break;
	case BOUNDARY_TRANSPORT_Q:
		n = dlg->editor_bt->n;
		#if DEBUG_EDITOR_CHANNEL_GET_BT
			fprintf(stderr,"ECGBT n=%d\n",n);
		#endif
		bt->p1 = jb_try_realloc(bt->p1, 2 * sizeof(JBFLOAT) * n);
		if (!bt->p1) goto exit2;
		bt->p2 = bt->p1 + n;
		bt->n = n - 1;
		jbw_array_editor_get_column_float(dlg->editor_bt,0,bt->p1);
		jbw_array_editor_get_column_float(dlg->editor_bt,1,bt->p2);
	}
	#if DEBUG_EDITOR_CHANNEL_GET_BT
		fprintf(stderr,"ECGBT parameters\n");
		boundary_transport_print(bt,stderr);
		fprintf(stderr,"Editor channel get bt: end\n");
	#endif
	return;

exit2:
	jbw_show_error2
		(gettext("Transport boundary condition"),gettext("Not enough memory"));
	#if DEBUG_EDITOR_CHANNEL_GET_BT
		fprintf(stderr,"Editor channel get bt: end\n");
	#endif
	gtk_main_quit();
}

void editor_channel_update_bt(EditorChannel *dlg)
{
	BoundaryTransport *bt = dlg->c->ct[dlg->ict].bt + dlg->ibt;
	#if DEBUG_EDITOR_CHANNEL_UPDATE_BT
		fprintf(stderr,"Editor channel update bt: start\n");
	#endif
	editor_channel_get_bt(dlg);
	#if DEBUG_EDITOR_CHANNEL_UPDATE_BT
		fprintf(stderr,"ECABT type=%d\n",bt->type);
	#endif
	switch (bt->type)
	{
	case BOUNDARY_TRANSPORT_M:
		gtk_widget_show((GtkWidget*)dlg->label_bt_time);
		gtk_widget_show((GtkWidget*)dlg->entry_bt_time);
		gtk_widget_show((GtkWidget*)dlg->label_bt_mass);
		gtk_widget_show((GtkWidget*)dlg->entry_bt_mass);
		gtk_widget_hide((GtkWidget*)dlg->paned_bt);
		break;
	case BOUNDARY_TRANSPORT_Q:
		gtk_widget_hide((GtkWidget*)dlg->label_bt_time);
		gtk_widget_hide((GtkWidget*)dlg->entry_bt_time);
		gtk_widget_hide((GtkWidget*)dlg->label_bt_mass);
		gtk_widget_hide((GtkWidget*)dlg->entry_bt_mass);

		jbw_array_editor_set_title(dlg->editor_bt,0,gettext("Time"));
		jbw_array_editor_set_title(dlg->editor_bt,1,gettext("Discharge"));
		jbw_graphic_set_xlabel(dlg->graphic_bt,gettext("Time"));
		jbw_graphic_set_ylabel(dlg->graphic_bt,gettext("Discharge"));

		gtk_widget_set_sensitive((GtkWidget*)dlg->button_delete_bt,
			bt->n > 0);
		gtk_widget_show((GtkWidget*)dlg->paned_bt);
#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
		jbw_graphic_draw = dlg->graphic_bt->draw;
		jbw_graphic_expose_event();
#endif
	}
	#if DEBUG_EDITOR_CHANNEL_UPDATE_BT
		fprintf(stderr,"Editor channel update bt: end\n");
	#endif
	return;
}

void editor_channel_open_bt(EditorChannel *dlg,BoundaryTransport *bt)
{
	#if DEBUG_EDITOR_CHANNEL_OPEN_BT
		fprintf(stderr,"Editor channel open bt: start\n");
		fprintf(stderr,"ECOBT name=%s\n",bt->name);
	#endif
	g_signal_handler_block(dlg->notebook_transport,dlg->id_notebook_transport);
	gtk_notebook_set_current_page(dlg->notebook_transport,
		EDITOR_CHANNEL_PAGE_TRANSPORT_BT);
	g_signal_handler_unblock
		(dlg->notebook_transport,dlg->id_notebook_transport);
	jbw_int_entry_set_value(dlg->entry_bt_initial,bt->pos);
	jbw_int_entry_set_value(dlg->entry_bt_final,bt->pos2);
	switch (bt->type)
	{
	case BOUNDARY_TRANSPORT_M:
		jbw_float_entry_set_value(dlg->entry_bt_time,bt->p1[0]);
		jbw_float_entry_set_value(dlg->entry_bt_mass,bt->p2[0]);
		break;
	case BOUNDARY_TRANSPORT_Q:
		jbw_array_editor_set_rows(dlg->editor_bt,bt->n+1);
		jbw_array_editor_set_column_float(dlg->editor_bt,0,bt->p1);
		jbw_array_editor_set_column_float(dlg->editor_bt,1,bt->p2);
	}
	#if DEBUG_EDITOR_CHANNEL_OPEN_BT
		fprintf(stderr,"ECOBT name=%s\n",bt->name);
	#endif
	g_signal_handler_block(dlg->entry_bt,dlg->id_entry_bt);
	gtk_entry_set_text(dlg->entry_bt,bt->name);
	g_signal_handler_unblock(dlg->entry_bt,dlg->id_entry_bt);
	#if DEBUG_EDITOR_CHANNEL_OPEN_BT
		fprintf(stderr,"ECOBT name=%s\n",bt->name);
	#endif
	jbw_array_radio_buttons_set_active(dlg->button_bt,bt->type,1);
	#if DEBUG_EDITOR_CHANNEL_OPEN_BT
		fprintf(stderr,"ECOBT name=%s\n",bt->name);
		fprintf(stderr,"Editor channel open bt: end\n");
	#endif
}

void editor_channel_insert_bt(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_INSERT_BT
		fprintf(stderr,"Editor channel insert bt: start\n");
	#endif
	editor_channel_insert_point(dlg->editor_bt);
	editor_channel_update_bt(dlg);
	#if DEBUG_EDITOR_CHANNEL_INSERT_BT
		fprintf(stderr,"Editor channel insert bt: end\n");
	#endif
}

void editor_channel_delete_bt(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_DELETE_BT
		fprintf(stderr,"Editor channel delete bt: start\n");
	#endif
	editor_channel_delete_point(dlg->editor_bt);
	editor_channel_update_bt(dlg);
	#if DEBUG_EDITOR_CHANNEL_DELETE_BT
		fprintf(stderr,"Editor channel delete bt: end\n");
	#endif
}

void editor_channel_draw_bt()
{
	BoundaryTransport *bt = editor->c->ct[editor->ict].bt + editor->ibt;
	#if DEBUG_EDITOR_CHANNEL_DRAW_BT
		fprintf(stderr,"Editor channel draw bt: start\n");
	#endif
	jbw_graphic_draw_lines(editor->graphic_bt,bt->p1,bt->p2,0,0,0,bt->n+1);
	jbw_graphic_draw_logo(editor->graphic_bt);
	#if DEBUG_EDITOR_CHANNEL_DRAW_BT
		fprintf(stderr,"Editor channel draw bt: end\n");
	#endif
}

void editor_channel_update_bts(EditorChannel *dlg)
{
	ChannelTransport *ct = dlg->c->ct + dlg->ict;
	#if DEBUG_EDITOR_CHANNEL_UPDATE_BTS
		fprintf(stderr,"Editor channel update bts: start\n");
		fprintf(stderr,"ECUBTS n=%d ict=%d\n",ct->n,dlg->ict);
		channel_transport_print(ct,stderr);
	#endif
	if (ct->n >= 0)
	{
		editor_channel_update_bt(dlg);
		gtk_widget_set_sensitive((GtkWidget*)dlg->button_delete_bts,1);
		gtk_widget_show((GtkWidget*)dlg->table_bt);
	}
	else
	{
		gtk_widget_hide((GtkWidget*)dlg->table_bt);
		gtk_widget_set_sensitive((GtkWidget*)dlg->button_delete_bts,0);
	}
	gtk_widget_set_sensitive((GtkWidget*)dlg->button_bt_up, dlg->ibt < ct->n);
	gtk_widget_set_sensitive((GtkWidget*)dlg->button_bt_down, dlg->ibt);
	#if DEBUG_EDITOR_CHANNEL_UPDATE_BTS
		channel_transport_print(ct,stderr);
		fprintf(stderr,"ECUBTS %s\n",gtk_entry_get_text(dlg->entry_transport));
		fprintf(stderr,"Editor channel update bts: end\n");
	#endif
}

void editor_channel_update_combo_bts(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_UPDATE_COMBO_BTS
		fprintf(stderr,"Editor channel update combo bts: start\n");
		fprintf(stderr,"ECUCBTS: n=%d\n",dlg->c->ct[dlg->ict].n);
	#endif
	char *name;
	ChannelTransport *ct = dlg->c->ct + dlg->ict;
	BoundaryTransport *bt = ct->bt + dlg->ibt;
	if (ct->n < 0) goto exit1;
	g_signal_handler_block(dlg->combo_bt,dlg->id_combo_bt);
	g_signal_handler_block(dlg->combo_bt,dlg->id_combo_bt2);
	gtk_combo_box_text_remove(dlg->combo_bt,dlg->ibt);
	name = (char*)gtk_entry_get_text(dlg->entry_bt);
	gtk_combo_box_text_insert_text(dlg->combo_bt,dlg->ibt,name);
	g_free(bt->name);
	bt->name = jb_strdup(name);
	gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_bt),dlg->ibt);
	g_signal_handler_unblock(dlg->combo_bt,dlg->id_combo_bt2);
	g_signal_handler_unblock(dlg->combo_bt,dlg->id_combo_bt);
exit1:
	#if DEBUG_EDITOR_CHANNEL_UPDATE_COMBO_BTS
		fprintf(stderr,"ECUCBTS %s\n",gtk_entry_get_text(dlg->entry_transport));
		fprintf(stderr,"Editor channel update combo bts: end\n");
	#endif
	return;
}

void editor_channel_update_entry_bts(EditorChannel *dlg)
{
	ChannelTransport *ct = dlg->c->ct + dlg->ict;
	#if DEBUG_EDITOR_CHANNEL_UPDATE_ENTRY_BTS
		fprintf(stderr,"Editor channel update entry bts: start\n");
		fprintf(stderr,"ECUEBTS: n=%d name=%s\n",ct->n,ct->name);
	#endif
	if (ct->n < 0) goto exit1;
	g_signal_handler_block(dlg->entry_bt,dlg->id_entry_bt);
	dlg->ibt = gtk_combo_box_get_active(GTK_COMBO_BOX(dlg->combo_bt));
	gtk_entry_set_text(dlg->entry_bt,
		(char*)gtk_combo_box_text_get_active_text(dlg->combo_bt));
	editor_channel_open_bt(dlg, dlg->c->ct[dlg->ict].bt + dlg->ibt);
	g_signal_handler_unblock(dlg->entry_bt,dlg->id_entry_bt);
exit1:
	editor_channel_update_bts(dlg);
	#if DEBUG_EDITOR_CHANNEL_UPDATE_ENTRY_BTS
		fprintf(stderr,"ECUEBTS %s\n",gtk_entry_get_text(dlg->entry_transport));
		fprintf(stderr,"Editor channel update entry bts: end\n");
	#endif
	return;
}

void editor_channel_up_bts(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_UP_BTS
		fprintf(stderr,"Editor channel up bts: start\n");
		fprintf(stderr,"ECUBTS: ibt=%d\n",dlg->ibt);
	#endif
	gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_bt),dlg->ibt+1);
	#if DEBUG_EDITOR_CHANNEL_UP_BTS
		fprintf(stderr,"ECUBTS: ibt=%d\n",dlg->ibt);
		fprintf(stderr,"ECUBTS %s\n",gtk_entry_get_text(dlg->entry_transport));
		fprintf(stderr,"Editor channel up bts: end\n");
	#endif
}

void editor_channel_down_bts(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_DOWN_BTS
		fprintf(stderr,"Editor channel down bts: start\n");
		fprintf(stderr,"ECDBTS: ibt=%d\n",dlg->ibt);
	#endif
	gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_bt),dlg->ibt-1);
	#if DEBUG_EDITOR_CHANNEL_DOWN_BTS
		fprintf(stderr,"ECDBTS: ibt=%d\n",dlg->ibt);
		fprintf(stderr,"ECDBTS %s\n",gtk_entry_get_text(dlg->entry_transport));
		fprintf(stderr,"Editor channel down bts: end\n");
	#endif
}

void editor_channel_open_bts(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_OPEN_BTS
		fprintf(stderr,"Editor channel open bts: start\n");
	#endif
	int i, n = dlg->c->ct[dlg->ict].n + 1;
	BoundaryTransport *bt = dlg->c->ct[dlg->ict].bt;
	g_signal_handler_block(dlg->combo_bt,dlg->id_combo_bt);
	g_signal_handler_block(dlg->combo_bt,dlg->id_combo_bt2);
	g_signal_handler_block(dlg->entry_bt,dlg->id_entry_bt);
	for (i=n; --i>=0;)
		gtk_combo_box_text_prepend_text(dlg->combo_bt,bt[i].name);
	gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_bt),0);
	g_signal_handler_unblock(dlg->entry_bt,dlg->id_entry_bt);
	g_signal_handler_unblock(dlg->combo_bt,dlg->id_combo_bt2);
	g_signal_handler_unblock(dlg->combo_bt,dlg->id_combo_bt);
	dlg->ibt = 0;
	if (!n) bt=0; else editor_channel_open_bt(dlg,bt);
	editor_channel_update_bts(dlg);
	#if DEBUG_EDITOR_CHANNEL_OPEN_BTS
		fprintf(stderr,"ECOBTS %s\n",gtk_entry_get_text(dlg->entry_transport));
		fprintf(stderr,"Editor channel open bts: end\n");
	#endif
}

void editor_channel_insert_bts(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_INSERT_BTS
		fprintf(stderr,"Editor channel insert bts: start\n");
		channel_transport_print(dlg->c->ct, stderr);
	#endif
	BoundaryTransport *bt;
	bt = (BoundaryTransport*)g_try_malloc(sizeof(BoundaryTransport));
	if (!bt)
	{
		jbw_show_error2(gettext("Transport boundary condition"),
			gettext("Not enough memory"));
		goto exit2;
	}
	if (!boundary_transport_copy(bt,btnew))
	{
		g_free(bt);
		goto exit2;
	}
	editor_channel_update_bts(dlg);
	channel_transport_insert_bt(dlg->c->ct + dlg->ict, bt, dlg->ibt);
	#if DEBUG_EDITOR_CHANNEL_INSERT_BTS
		channel_transport_print(dlg->c->ct + dlg->ict, stderr);
	#endif
	g_signal_handler_block(dlg->entry_bt,dlg->id_entry_bt);
	g_signal_handler_block(dlg->combo_bt,dlg->id_combo_bt2);
	gtk_combo_box_text_insert_text(dlg->combo_bt,dlg->ibt,bt->name);
	gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_bt),dlg->ibt);
	g_signal_handler_unblock(dlg->combo_bt,dlg->id_combo_bt2);
	g_signal_handler_unblock(dlg->entry_bt,dlg->id_entry_bt);
	editor_channel_open_bt(dlg,bt);

exit2:
	#if DEBUG_EDITOR_CHANNEL_INSERT_BTS
		channel_transport_print(dlg->c->ct + dlg->ict, stderr);
		fprintf(stderr,"ECIBTS %s\n",gtk_entry_get_text(dlg->entry_transport));
		fprintf(stderr,"Editor channel insert bts: end\n");
	#endif
	return;
}

void editor_channel_delete_bts(EditorChannel *dlg)
{
	ChannelTransport *ct = dlg->c->ct + dlg->ict;
	#if DEBUG_EDITOR_CHANNEL_DELETE_BTS
		fprintf(stderr,"Editor channel delete bts: start\n");
		channel_transport_print(ct,stderr);
	#endif
	channel_transport_delete_bt(ct,dlg->ibt);
	#if DEBUG_EDITOR_CHANNEL_DELETE_BTS
		channel_transport_print(ct,stderr);
	#endif
	gtk_combo_box_text_remove(dlg->combo_bt,dlg->ibt);
	if (ct->n >= 0)
	{
		if (dlg->ibt > ct->n) --dlg->ibt;
		g_signal_handler_block(dlg->combo_bt,dlg->id_combo_bt2);
		gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_bt),dlg->ibt);
		g_signal_handler_unblock(dlg->combo_bt,dlg->id_combo_bt2);
	}
	editor_channel_update_bts(dlg);
	#if DEBUG_EDITOR_CHANNEL_DELETE_BTS
		channel_transport_print(ct,stderr);
		fprintf(stderr,"ECDBTS: ibt=%d\n",dlg->ibt);
		fprintf(stderr,"ECDBTS %s\n",gtk_entry_get_text(dlg->entry_transport));
		fprintf(stderr,"Editor channel delete bts: end\n");
	#endif
}

#endif

void editor_channel_update_transport(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_UPDATE_TRANSPORT
		fprintf(stderr,"Editor channel update transport: start\n");
		fprintf(stderr,"ECUT n=%d ict=%d\n",dlg->c->nt,dlg->ict);
		channel_print(dlg->c, stderr);
	#endif
	if (dlg->c->nt >= 0)
	{
		#if DEBUG_EDITOR_CHANNEL_UPDATE_TRANSPORT
			channel_print(dlg->c,stderr);
		#endif
		dlg->c->ct[dlg->ict].solubility =
			jbw_float_entry_get_value(dlg->entry_solubility);
#if EDITOR_BF
		editor_channel_update_bts(dlg);
#endif
		editor_channel_update_it(dlg);
		gtk_widget_set_sensitive((GtkWidget*)dlg->button_delete_transport,1);
		gtk_widget_show((GtkWidget*)dlg->notebook_transport);
		gtk_widget_show((GtkWidget*)dlg->hbox_transport2);
	}
	else
	{
		gtk_widget_set_sensitive((GtkWidget*)dlg->button_delete_transport,0);
		gtk_widget_hide((GtkWidget*)dlg->notebook_transport);
		gtk_widget_hide((GtkWidget*)dlg->hbox_transport2);
	}
	gtk_widget_set_sensitive
		((GtkWidget*)dlg->button_transport_up, dlg->ict < dlg->c->nt);
	gtk_widget_set_sensitive((GtkWidget*)dlg->button_transport_down, dlg->ict);
	#if DEBUG_EDITOR_CHANNEL_UPDATE_TRANSPORT
		channel_print(dlg->c,stderr);
		fprintf(stderr,"ECUT %s\n",gtk_entry_get_text(dlg->entry_transport));
		fprintf(stderr,"Editor channel update transport: end\n");
	#endif
}

void editor_channel_update_combo_transport(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_UPDATE_COMBO_TRANSPORT
		fprintf(stderr,"Editor channel update combo transport: start\n");
		fprintf(stderr,"ECUCT: n=%d\n",dlg->c->nt);
	#endif
	char *name;
	ChannelTransport *ct = dlg->c->ct + dlg->ict;
	if (dlg->c->nt < 0) goto exit1;
	#if DEBUG_EDITOR_CHANNEL_UPDATE_COMBO_TRANSPORT
		fprintf(stderr,"ECUCT: solubility="FWF"\n",ct->solubility);
	#endif
	g_signal_handler_block(dlg->combo_transport,dlg->id_combo_transport);
	g_signal_handler_block(dlg->combo_transport,dlg->id_combo_transport2);
	gtk_combo_box_text_remove(dlg->combo_transport,dlg->ict);
	name = (char*)gtk_entry_get_text(dlg->entry_transport);
	gtk_combo_box_text_insert_text(dlg->combo_transport,dlg->ict,name);
	g_free(ct->name);
	ct->name = jb_strdup(name);
	gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_transport),dlg->ict);
	g_signal_handler_unblock(dlg->combo_transport,dlg->id_combo_transport2);
	g_signal_handler_unblock(dlg->combo_transport,dlg->id_combo_transport);
exit1:
	#if DEBUG_EDITOR_CHANNEL_UPDATE_COMBO_TRANSPORT
		fprintf(stderr,"ECUCT %s\n",gtk_entry_get_text(dlg->entry_transport));
		fprintf(stderr,"Editor channel update combo transport: end\n");
	#endif
	return;
}

void editor_channel_update_entry_transport(EditorChannel *dlg)
{
	ChannelTransport *ct;
	#if DEBUG_EDITOR_CHANNEL_UPDATE_ENTRY_TRANSPORT
		fprintf(stderr,"Editor channel update entry transport: start\n");
		fprintf(stderr,"ECUET: n=%d\n",dlg->c->nt);
	#endif
	g_signal_handler_block(dlg->entry_transport,dlg->id_entry_transport);
	dlg->ict = gtk_combo_box_get_active(GTK_COMBO_BOX(dlg->combo_transport));
	ct = dlg->c->ct + dlg->ict;
	#if DEBUG_EDITOR_CHANNEL_UPDATE_ENTRY_TRANSPORT
		fprintf(stderr,"ECUET: active=%s\n",
			gtk_combo_box_get_active_text(dlg->combo_transport));
	#endif
	gtk_entry_set_text(dlg->entry_transport,
		(char*)gtk_combo_box_text_get_active_text(dlg->combo_transport));
	jbw_float_entry_set_value(dlg->entry_solubility,ct->solubility);
#if EDITOR_BF
	editor_channel_open_bts(dlg);
#endif
	editor_channel_open_it(dlg,ct->it);
	g_signal_handler_unblock(dlg->entry_transport,dlg->id_entry_transport);
	editor_channel_update_transport(dlg);
	#if DEBUG_EDITOR_CHANNEL_UPDATE_ENTRY_TRANSPORT
		fprintf(stderr,"ECUET %s\n",gtk_entry_get_text(dlg->entry_transport));
		fprintf(stderr,"Editor channel update entry transport: end\n");
	#endif
	return;
}

void editor_channel_up_transport(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_UP_TRANSPORT
		fprintf(stderr,"Editor channel up transport: start\n");
		fprintf(stderr,"ECUT: ict=%d\n",dlg->ict);
	#endif
	gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_transport),dlg->ict+1);
	#if DEBUG_EDITOR_CHANNEL_UP_TRANSPORT
		fprintf(stderr,"ECUT: ict=%d\n",dlg->ict);
		fprintf(stderr,"ECUT %s\n",gtk_entry_get_text(dlg->entry_transport));
		fprintf(stderr,"Editor channel up transport: end\n");
	#endif
}

void editor_channel_down_transport(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_DOWN_TRANSPORT
		fprintf(stderr,"Editor channel down transport: start\n");
		fprintf(stderr,"ECDT: ict=%d\n",dlg->ict);
	#endif
	gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_transport),dlg->ict-1);
	#if DEBUG_EDITOR_CHANNEL_DOWN_TRANSPORT
		fprintf(stderr,"ECDT: ict=%d\n",dlg->ict);
		fprintf(stderr,"ECDT %s\n",gtk_entry_get_text(dlg->entry_transport));
		fprintf(stderr,"Editor channel down transport: end\n");
	#endif
}

void editor_channel_open_transport(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_OPEN_TRANSPORT
		fprintf(stderr,"Editor channel open transport: start\n");
	#endif
	int i, n = dlg->c->nt + 1;
	ChannelTransport *ct = dlg->c->ct;
	g_signal_handler_block(dlg->combo_transport,dlg->id_combo_transport);
	g_signal_handler_block(dlg->combo_transport,dlg->id_combo_transport2);
	g_signal_handler_block(dlg->entry_transport,dlg->id_entry_transport);
	for (i=n; --i>=0;)
		gtk_combo_box_text_prepend_text(dlg->combo_transport,ct[i].name);
	gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_transport),0);
	g_signal_handler_unblock(dlg->entry_transport,dlg->id_entry_transport);
	g_signal_handler_unblock(dlg->combo_transport,dlg->id_combo_transport2);
	g_signal_handler_unblock(dlg->combo_transport,dlg->id_combo_transport);
	dlg->ict = 0;
	if (!n) ct=0;
	else jbw_float_entry_set_value(dlg->entry_solubility,ct->solubility);
	editor_channel_update_entry_transport(dlg);
	#if DEBUG_EDITOR_CHANNEL_OPEN_TRANSPORT
		fprintf(stderr,"ECOT %s\n",gtk_entry_get_text(dlg->entry_transport));
		fprintf(stderr,"Editor channel open transport: end\n");
	#endif
}

void editor_channel_insert_transport(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_INSERT_TRANSPORT
		fprintf(stderr,"Editor channel insert transport: start\n");
		channel_print(dlg->c, stderr);
	#endif
	ChannelTransport *ct;
	ct = (ChannelTransport*)g_try_malloc(sizeof(ChannelTransport));
	if (!ct)
	{
		jbw_show_error2
			(gettext("Channel transport"),gettext("Not enough memory"));
		goto exit2;
	}
	if (!channel_transport_copy(ct,ctnew))
	{
		g_free(ct);
		goto exit2;
	}
	editor_channel_update_transport(dlg);
	channel_insert_ct(dlg->c, ct, dlg->ict);
	#if DEBUG_EDITOR_CHANNEL_INSERT_TRANSPORT
		channel_print(dlg->c, stderr);
	#endif
	g_signal_handler_block(dlg->entry_transport,dlg->id_entry_transport);
	g_signal_handler_block(dlg->combo_transport,dlg->id_combo_transport2);
	gtk_combo_box_text_insert_text(dlg->combo_transport,dlg->ict,ct->name);
	gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_transport),dlg->ict);
	jbw_float_entry_set_value(dlg->entry_solubility,ct->solubility);
	g_signal_handler_unblock(dlg->combo_transport,dlg->id_combo_transport2);
	g_signal_handler_unblock(dlg->entry_transport,dlg->id_entry_transport);
	editor_channel_open_it(dlg, ct->it);
#if EDITOR_BF
	editor_channel_open_bts(dlg);
#endif

exit2:
	#if DEBUG_EDITOR_CHANNEL_INSERT_TRANSPORT
		channel_print(dlg->c, stderr);
		fprintf(stderr,"ECIT %s\n",gtk_entry_get_text(dlg->entry_transport));
		fprintf(stderr,"Editor channel insert transport: end\n");
	#endif
	return;
}

void editor_channel_delete_transport(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_DELETE_TRANSPORT
		fprintf(stderr,"Editor channel delete transport: start\n");
		fprintf(stderr,"ECDT ict=%d nt=%d\n",dlg->ict,dlg->c->nt);
		channel_print(dlg->c,stderr);
	#endif
	channel_delete_ct(dlg->c, dlg->ict);
	#if DEBUG_EDITOR_CHANNEL_DELETE_TRANSPORT
		fprintf(stderr,"ECDT ict=%d nt=%d\n",dlg->ict,dlg->c->nt);
	#endif
	dlg->ibt = 0;
	#if DEBUG_EDITOR_CHANNEL_DELETE_TRANSPORT
		channel_print(dlg->c,stderr);
	#endif
	gtk_combo_box_text_remove(dlg->combo_transport,dlg->ict);
	if (dlg->c->nt >= 0)
	{
		#if DEBUG_EDITOR_CHANNEL_DELETE_TRANSPORT
			fprintf(stderr,"ECDT ict=%d nt=%d\n",dlg->ict,dlg->c->nt);
		#endif
		if (dlg->ict > dlg->c->nt) --dlg->ict;
		g_signal_handler_block(dlg->combo_transport,dlg->id_combo_transport2);
		gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_transport),dlg->ict);
		g_signal_handler_unblock(dlg->combo_transport,dlg->id_combo_transport2);
	}
	editor_channel_update_transport(dlg);
	#if DEBUG_EDITOR_CHANNEL_DELETE_TRANSPORT
		channel_print(dlg->c, stderr);
		fprintf(stderr,"ECDT: ict=%d\n",dlg->ict);
		fprintf(stderr,"ECDT %s\n",gtk_entry_get_text(dlg->entry_transport));
		fprintf(stderr,"Editor channel delete transport: end\n");
	#endif
}

void editor_channel_update_notebook_transport(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_UPDATE_NOTEBOOK_TRANSPORT
		fprintf(stderr,"Editor channel update notebook transport: start\n");
	#endif
#if EDITOR_BF
	switch (gtk_notebook_get_current_page(dlg->notebook_transport))
	{
	case EDITOR_CHANNEL_PAGE_TRANSPORT_BT:
		#if DEBUG_EDITOR_CHANNEL_UPDATE_NOTEBOOK_TRANSPORT
			fprintf(stderr,"ECUNT: boundary condition\n");
		#endif
		editor_channel_update_bts(dlg);
		break;
	}
#endif
	#if DEBUG_EDITOR_CHANNEL_UPDATE_NOTEBOOK_TRANSPORT
		channel_print(dlg->c,stderr);
		fprintf(stderr,"Editor channel update notebook transport: end\n");
	#endif
}

void editor_channel_update_notebook(EditorChannel *dlg)
{
	int i;
	#if DEBUG_EDITOR_CHANNEL_UPDATE_NOTEBOOK
		fprintf(stderr,"Editor channel update notebook: start\n");
	#endif
	switch (gtk_notebook_get_current_page(dlg->notebook))
	{
	case EDITOR_CHANNEL_PAGE_GEOMETRY:
		#if DEBUG_EDITOR_CHANNEL_UPDATE_NOTEBOOK
			fprintf(stderr,"ECUN: geometry\n");
		#endif
		editor_channel_get_section(dlg);
		editor_channel_update_geometry(dlg);
		break;
	case EDITOR_CHANNEL_PAGE_SECTION:
	{
		#if DEBUG_EDITOR_CHANNEL_UPDATE_NOTEBOOK
			fprintf(stderr,"ECUN: section\n");
		#endif
		ChannelGeometry *cg = dlg->c->cg;
		char *name[cg->n + 1];
		g_signal_handler_block(dlg->combo_section,dlg->id_combo_section);
		jbw_array_editor_get_column(dlg->editor_geometry,1,name);
		for (i=cg->n+1; --i>=0;)
			gtk_combo_box_text_prepend_text(dlg->combo_section,name[i]);
		g_signal_handler_unblock(dlg->combo_section,dlg->id_combo_section);
		gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_section),dlg->ics);
		editor_channel_update_section(dlg);
		break;
	}
	case EDITOR_CHANNEL_PAGE_TRANSIENT:
	{
		#if DEBUG_EDITOR_CHANNEL_UPDATE_NOTEBOOK
			fprintf(stderr,"ECUN: transient\n");
		#endif
		CrossSection *cs = dlg->c->cg->cs + dlg->ics;
		char *name[cs->n + 1];
		g_signal_handler_block(dlg->combo_transient,dlg->id_combo_transient);
		jbw_array_editor_get_column(dlg->editor_section,2,name);
		for (i=cs->n+1; --i>=0;)
			gtk_combo_box_text_prepend_text(dlg->combo_transient,name[i]);
		g_signal_handler_unblock(dlg->combo_transient,dlg->id_combo_transient);
		gtk_combo_box_set_active(GTK_COMBO_BOX(dlg->combo_transient),dlg->its);
		editor_channel_update_transient(dlg);
		break;
	}
	case EDITOR_CHANNEL_PAGE_IF:
		#if DEBUG_EDITOR_CHANNEL_UPDATE_NOTEBOOK
			fprintf(stderr,"ECUN: flow initial condition\n");
		#endif
		editor_channel_update_if(dlg);
		break;
	case EDITOR_CHANNEL_PAGE_BF:
#if EDITOR_BF
		#if DEBUG_EDITOR_CHANNEL_UPDATE_NOTEBOOK
			fprintf(stderr,"ECUN: boundary condition\n");
		#endif
		editor_channel_update_bf(dlg);
#endif
		break;
	case EDITOR_CHANNEL_PAGE_TRANSPORT:
		#if DEBUG_EDITOR_CHANNEL_UPDATE_NOTEBOOK
			fprintf(stderr,"ECUN: transport\n");
		#endif
		editor_channel_update_transport(dlg);
	}
	#if DEBUG_EDITOR_CHANNEL_UPDATE_NOTEBOOK
		channel_print(dlg->c,stderr);
		fprintf(stderr,"Editor channel update notebook: end\n");
	#endif
}

void editor_channel_update(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_UPDATE
		fprintf(stderr,"Editor channel update: start\n");
		channel_print(dlg->c,stderr);
	#endif
	editor_channel_update_transient(dlg);
	editor_channel_update_section(dlg);
	editor_channel_update_geometry(dlg);
	editor_channel_update_if(dlg);
#if EDITOR_BF
	editor_channel_update_inlet(dlg);
	editor_channel_update_outlet(dlg);
	editor_channel_update_bf(dlg);
#endif
	editor_channel_update_transport(dlg);
	editor_channel_update_notebook(dlg);
	#if DEBUG_EDITOR_CHANNEL_UPDATE
		channel_print(dlg->c,stderr);
		fprintf(stderr,"Editor channel update: end\n");
	#endif
}

void editor_channel_destroy(EditorChannel *dlg)
{
	#if DEBUG_EDITOR_CHANNEL_DESTROY
		fprintf(stderr,"Editor channel destroy: start\n");
		fprintf(stderr,"ECD: freeing boundary data\n");
	#endif
	g_free(dlg->editor_bf_inlet->button_type);
	g_free(dlg->editor_bf_outlet->button_type);
	g_free(dlg->editor_bf_inner->button_type);
	#if DEBUG_EDITOR_CHANNEL_DESTROY
		fprintf(stderr,"ECD: destroying graphics\n");
	#endif
#if EDITOR_BF
	jbw_graphic_destroy(dlg->graphic_bt);
	jbw_graphic_destroy(dlg->editor_bf_inner->graphic);
	jbw_graphic_destroy(dlg->editor_bf_outlet->graphic);
	jbw_graphic_destroy(dlg->editor_bf_inlet->graphic);
#endif
	jbw_graphic_destroy(dlg->graphic_it);
	jbw_graphic_destroy(dlg->graphic_if);
	jbw_graphic_destroy(dlg->graphic_transient);
	jbw_graphic_destroy(dlg->graphic_section);
	jbw_graphic_destroy(dlg->graphic_geometry);
	#if DEBUG_EDITOR_CHANNEL_DESTROY
		fprintf(stderr,"ECD: destroying editor arrays\n");
	#endif
	jbw_array_editor_destroy(dlg->editor_transient);
	jbw_array_editor_destroy(dlg->editor_section);
	jbw_array_editor_destroy(dlg->editor_geometry);
	jbw_array_editor_destroy(dlg->editor_if);
#if EDITOR_BF
	jbw_array_editor_destroy(dlg->editor_bf_inlet->editor);
	jbw_array_editor_destroy(dlg->editor_bf_outlet->editor);
	jbw_array_editor_destroy(dlg->editor_bf_inner->editor);
#endif
	#if DEBUG_EDITOR_CHANNEL_DESTROY
		fprintf(stderr,"ECD: deleting channel\n");
	#endif
	channel_delete(dlg->c);
	window_parent = dialog_simulator->window;
	#if DEBUG_EDITOR_CHANNEL_DESTROY
		fprintf(stderr,"Editor channel destroy: end\n");
	#endif
}

void editor_channel_hbox(GtkHBox **box,GtkLabel **label,GtkComboBoxText **combo,
	GtkButton **button_up,GtkButton **button_down,char *string)
{
	GtkArrow *arrow;
	*box = (GtkHBox*)gtk_hbox_new(0,5);
	*label = (GtkLabel*)gtk_label_new(string);
	gtk_box_pack_start((GtkBox*)*box,(GtkWidget*)*label,0,0,0);
	*combo = (GtkComboBoxText*)gtk_combo_box_text_new();
	gtk_box_pack_start_defaults((GtkBox*)*box,(GtkWidget*)*combo);
	arrow = (GtkArrow*)gtk_arrow_new(GTK_ARROW_UP,GTK_SHADOW_OUT);
	*button_up = (GtkButton*)gtk_button_new();
	gtk_container_add((GtkContainer*)*button_up,(GtkWidget*)arrow);
	gtk_box_pack_start((GtkBox*)*box,(GtkWidget*)*button_up,0,1,0);
	arrow = (GtkArrow*)gtk_arrow_new(GTK_ARROW_DOWN,GTK_SHADOW_OUT);
	*button_down = (GtkButton*)gtk_button_new();
	gtk_container_add((GtkContainer*)*button_down,(GtkWidget*)arrow);
	gtk_box_pack_start((GtkBox*)*box,(GtkWidget*)*button_down,0,1,0);
}

void editor_channel_vbox
	(GtkVBox **box,GtkButton *button1,GtkButton *button2,GtkButton *button3)
{
	*box = (GtkVBox*)gtk_vbox_new(0,5);
	gtk_box_pack_start_defaults((GtkBox*)*box,(GtkWidget*)gtk_label_new(0));
	gtk_box_pack_start((GtkBox*)*box,(GtkWidget*)button1,0,1,5);
	gtk_box_pack_start((GtkBox*)*box,(GtkWidget*)button2,0,1,5);
	if (button3) gtk_box_pack_start((GtkBox*)*box,(GtkWidget*)button3,0,1,5);
}

void editor_channel_new(EditorChannel *dlg,Channel *c)
{
	int i;
	char buffer[JB_BUFFER_SIZE];
	char *label_geometry[]={gettext("Position"),gettext("Cross section")};
	char *label_section[]={gettext("t"),
		gettext("Expansion/contraction coefficient"),
		gettext("Transient section")};
	char *label_section_type[]={gettext("_Normal"),gettext("Discharge _gauge"),
		gettext("De_pth gauge"),gettext("Le_vel gauge")};
	char *label_if[]={gettext("x"),gettext("Discharge"),gettext("Depth")};
	char *label_button_if[]={gettext("_Dry channel"),gettext("_Steady"),
		gettext("Variables (x, Q, _h)"),gettext("Variables (x, Q, _z)")};
	char *label_it[]={gettext("x"),gettext("Concentration")};
	char *label_button_it[]=
		{gettext("_Dry channel"),gettext("_Steady"),gettext("Variables (_x, c)")};
	char *label_bt[]={gettext("_Pointwise"),gettext("_Discharge")};
	char *label_bt2[]={gettext("Time"),gettext("Discharge")};
	#if INTERFACE == INTERFACE_SCIENTIFIC
		char *label_transient[]={gettext("y"),gettext("z"),
			gettext("Friction coefficient"),gettext("Friction model")};
	#else
		char *label_transient[]={gettext("y"),gettext("z"),
			gettext("Friction coefficient")};
	#endif
	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr,"Editor channel new: start\n");
		channel_print(c,stderr);
	#endif
	if (!channel_copy(dlg->c,c)) return;
	dlg->cv = c;
	#if DEBUG_EDITOR_CHANNEL_NEW
		channel_print(dlg->c,stderr);
	#endif

	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr,"ECN: geometry\n");
	#endif
	dlg->button_insert_geometry = (GtkButton*)gtk_button_new_with_mnemonic
		(gettext("_Insert cross section"));
	dlg->button_delete_geometry = (GtkButton*)gtk_button_new_with_mnemonic
		(gettext("_Delete cross section"));
	editor_channel_vbox(&dlg->vbox_geometry,dlg->button_insert_geometry,
		dlg->button_delete_geometry,0);
	dlg->editor_geometry = jbw_array_editor_new(2,2,1,label_geometry);
	dlg->graphic_geometry =
		(JBWGraphic*)jbw_graphic_new(0,5,5,5,editor_channel_draw_geometry);
	jbw_graphic_set_xlabel(dlg->graphic_geometry,gettext("x"));
	jbw_graphic_set_ylabel(dlg->graphic_geometry,gettext("y"));
	jbw_graphic_set_logo(dlg->graphic_geometry, "swigs.png");
	dlg->table_geometry = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach(dlg->table_geometry,
		(GtkWidget*)dlg->editor_geometry->scrolled,0,0,1,1);
	gtk_grid_attach(dlg->table_geometry,(GtkWidget*)dlg->vbox_geometry,
		1,0,1,1);
	editor_paned
		(&dlg->paned_geometry,dlg->table_geometry,dlg->graphic_geometry);

	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr,"ECN: section\n");
	#endif
	editor_channel_hbox(&dlg->hbox_section,&dlg->label_section,
		&dlg->combo_section,&dlg->button_section_up,
		&dlg->button_section_down,gettext("Cross section"));
	dlg->button_insert_section = (GtkButton*)gtk_button_new_with_mnemonic
		(gettext("_Insert transient section"));
	dlg->button_delete_section = (GtkButton*)gtk_button_new_with_mnemonic
		(gettext("_Delete transient section"));
	editor_channel_vbox(&dlg->vbox_section,dlg->button_insert_section,
		dlg->button_delete_section,0);
	dlg->label_x = (GtkLabel*)gtk_label_new(gettext("x"));
	dlg->entry_x = jbw_float_entry_new();
	dlg->label_y = (GtkLabel*)gtk_label_new(gettext("y"));
	dlg->entry_y = jbw_float_entry_new();
	dlg->label_angle = (GtkLabel*)gtk_label_new(gettext("Angle (°)"));
	dlg->entry_angle = jbw_float_entry_new();
	dlg->vbox_section_type = (GtkVBox*)gtk_vbox_new(0,0);
	dlg->button_section[0] = 0;
	for (i=0; i<N_SECTION_TYPE; ++i)
	{
		dlg->button_section[i] = (GtkRadioButton*)
			gtk_radio_button_new_with_mnemonic_from_widget(
				dlg->button_section[0],label_section_type[i]);
		gtk_box_pack_start_defaults((GtkBox*)dlg->vbox_section_type,
			(GtkWidget*)dlg->button_section[i]);
		g_signal_connect_swapped(dlg->button_section[i],"clicked",
			(void(*)())editor_channel_update_section,dlg);
	}
	dlg->frame_section = (GtkFrame*)gtk_frame_new(gettext("Type"));
	gtk_container_add
		((GtkContainer*)dlg->frame_section,(GtkWidget*)dlg->vbox_section_type);
	dlg->label_control = (GtkLabel*)gtk_label_new(gettext("Gauge section"));
	dlg->entry_control = (JBWIntEntry*)gtk_entry_new();
	dlg->label_tolerance = (GtkLabel*)gtk_label_new(gettext("Tolerance"));
	dlg->entry_tolerance = (JBWFloatEntry*)gtk_entry_new();
	dlg->label_time = (GtkLabel*)gtk_label_new(gettext("Time"));
	dlg->entry_time = (JBWFloatEntry*)gtk_entry_new();
	dlg->label_parameter = (GtkLabel*)gtk_label_new(0);
	dlg->entry_parameter = (JBWFloatEntry*)gtk_entry_new();
	dlg->table_section_type = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach(dlg->table_section_type,(GtkWidget*)dlg->label_control,
		0,0,1,1);
	gtk_grid_attach(dlg->table_section_type,(GtkWidget*)dlg->entry_control,
		1,0,1,1);
	gtk_grid_attach(dlg->table_section_type,(GtkWidget*)dlg->label_tolerance,
		2,0,1,1);
	gtk_grid_attach(dlg->table_section_type,(GtkWidget*)dlg->entry_tolerance,
		3,0,1,1);
	gtk_grid_attach(dlg->table_section_type,(GtkWidget*)dlg->label_time,
		0,1,1,1);
	gtk_grid_attach(dlg->table_section_type,(GtkWidget*)dlg->entry_time,
		1,1,1,1);
	gtk_grid_attach(dlg->table_section_type,(GtkWidget*)dlg->label_parameter,
		2,1,1,1);
	gtk_grid_attach(dlg->table_section_type,(GtkWidget*)dlg->entry_parameter,
		3,1,1,1);
	dlg->label_variations =
		(GtkLabel*)gtk_label_new(gettext("Cross section variations"));
	dlg->label_variations =
		(GtkLabel*)gtk_label_new(gettext("Cross section variations"));
	dlg->editor_section = jbw_array_editor_new(3,3,1,label_section);
	dlg->graphic_section =
		(JBWGraphic*)jbw_graphic_new(0,5,5,5,editor_channel_draw_section);
	jbw_graphic_set_xlabel(dlg->graphic_section,gettext("y"));
	jbw_graphic_set_ylabel(dlg->graphic_section,gettext("z initial"));
	jbw_graphic_set_yylabel(dlg->graphic_section,gettext("z final"));
	jbw_graphic_set_logo(dlg->graphic_section, "swigs.png");
	dlg->table_section = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach(dlg->table_section,(GtkWidget*)dlg->hbox_section,
		1,0,7,1);
	gtk_grid_attach(dlg->table_section,(GtkWidget*)dlg->label_x,
		1,1,1,1);
	gtk_grid_attach(dlg->table_section,(GtkWidget*)dlg->entry_x,
		2,1,1,1);
	gtk_grid_attach(dlg->table_section,(GtkWidget*)dlg->label_y,
		3,1,1,1);
	gtk_grid_attach(dlg->table_section,(GtkWidget*)dlg->entry_y,
		4,1,1,1);
	gtk_grid_attach(dlg->table_section,(GtkWidget*)dlg->label_angle,
		5,1,1,1);
	gtk_grid_attach(dlg->table_section,(GtkWidget*)dlg->entry_angle,
		7,1,1,1);
	gtk_grid_attach(dlg->table_section,(GtkWidget*)dlg->frame_section,
		0,0,1,3);
	gtk_grid_attach(dlg->table_section,(GtkWidget*)dlg->table_section_type,
		1,2,7,1);
	gtk_grid_attach(dlg->table_section,(GtkWidget*)dlg->label_variations,
		0,3,7,1);
	gtk_grid_attach_defaults(dlg->table_section,
		(GtkWidget*)dlg->editor_section->scrolled,
		0,4,7,1);
	gtk_grid_attach(dlg->table_section,(GtkWidget*)dlg->vbox_section,
		7,4,1,1);
	editor_paned
		(&dlg->paned_section,dlg->table_section,dlg->graphic_section);

	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr,"ECN: transient section\n");
	#endif
	editor_channel_hbox(&dlg->hbox_transient,&dlg->label_transient,
		&dlg->combo_transient,&dlg->button_transient_up,
		&dlg->button_transient_down,gettext("Transient section"));
	dlg->button_insert_transient =
		(GtkButton*)gtk_button_new_with_mnemonic(gettext("Insert _point"));
	dlg->button_delete_transient =
		(GtkButton*)gtk_button_new_with_mnemonic(gettext("Delete poi_nt"));
	dlg->button_update =
		(GtkButton*)gtk_button_new_with_mnemonic(gettext("Update p_lot"));
	editor_channel_vbox(&dlg->vbox_transient,dlg->button_insert_transient,
		dlg->button_delete_transient,dlg->button_update);
	#if INTERFACE == INTERFACE_SCIENTIFIC
		dlg->editor_transient=(JBWArrayEditor*)jbw_array_editor_new
			(4,2,1,label_transient);
	#else
		dlg->editor_transient=(JBWArrayEditor*)jbw_array_editor_new
			(3,2,1,label_transient);
	#endif
	dlg->graphic_transient =
		(JBWGraphic*)jbw_graphic_new(0,5,5,5,editor_channel_draw_transient);
	jbw_graphic_set_xlabel(dlg->graphic_transient,gettext("y"));
	jbw_graphic_set_ylabel(dlg->graphic_transient,gettext("z"));
	jbw_graphic_set_logo(dlg->graphic_transient, "swigs.png");
	dlg->table_transient = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach(dlg->table_transient,(GtkWidget*)dlg->hbox_transient,
		0,0,2,1);
	gtk_grid_attach(dlg->table_transient,
		(GtkWidget*)dlg->editor_transient->scrolled,
		0,1,1,1);
	gtk_grid_attach(dlg->table_transient,(GtkWidget*)dlg->vbox_transient,
		1,1,1,1);
	editor_paned(&dlg->paned_transient,dlg->table_transient,
		dlg->graphic_transient);

	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr,"ECN: flow initial conditions\n");
	#endif
	dlg->hbox_if = (GtkHBox*)gtk_hbox_new(0,0);
	dlg->button_if[0] = 0;
	for (i=0; i<N_INITIAL_FLOW; ++i)
	{
		dlg->button_if[i] = (GtkRadioButton*)
			gtk_radio_button_new_with_mnemonic_from_widget(dlg->button_if[0],
			label_button_if[i]);
		gtk_box_pack_start_defaults((GtkBox*)dlg->hbox_if,
			(GtkWidget*)dlg->button_if[i]);
		g_signal_connect_swapped(dlg->button_if[i],"clicked",
			(void(*)())editor_channel_update_if,dlg);
	}
	dlg->frame_if = (GtkFrame*)gtk_frame_new(gettext("Type"));
	gtk_container_add((GtkContainer*)dlg->frame_if,(GtkWidget*)dlg->hbox_if);
	dlg->button_insert_if =
		(GtkButton*)gtk_button_new_with_mnemonic(gettext("Insert _point"));
	dlg->button_delete_if =
		(GtkButton*)gtk_button_new_with_mnemonic(gettext("Delete poi_nt"));
	dlg->button_update_if =
		(GtkButton*)gtk_button_new_with_mnemonic(gettext("Update p_lot"));
	editor_channel_vbox(&dlg->vbox_if,dlg->button_insert_if,
		dlg->button_delete_if,dlg->button_update_if);
	dlg->editor_if = jbw_array_editor_new(3,3,1,label_if);
	dlg->table_if = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach(dlg->table_if,
		(GtkWidget*)dlg->editor_if->scrolled,
		0,0,1,1);
	gtk_grid_attach(dlg->table_if,(GtkWidget*)dlg->vbox_if,
		1,0,1,1);
	dlg->graphic_if =
		(JBWGraphic*)jbw_graphic_new(0,5,5,5,editor_channel_draw_if);
	jbw_graphic_set_xlabel(dlg->graphic_if,gettext("x"));
	jbw_graphic_set_ylabel(dlg->graphic_if,gettext("Discharge"));
	jbw_graphic_set_logo(dlg->graphic_if, "swigs.png");
	editor_paned(&dlg->paned_if,dlg->table_if,dlg->graphic_if);
	dlg->vbox_if2 = (GtkVBox*)gtk_vbox_new(0,0);
	gtk_box_pack_start((GtkBox*)dlg->vbox_if2,(GtkWidget*)dlg->frame_if,0,1,0);
	gtk_box_pack_start_defaults((GtkBox*)dlg->vbox_if2,
		(GtkWidget*)dlg->paned_if);

	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr,"ECN: flow boundary conditions\n");
	#endif
#if EDITOR_BF
	editor_bf_create(dlg->editor_bf_inlet,EDITOR_BF_TYPE_IO,
		(void(*)())editor_channel_update_inlet,editor_channel_draw_inlet,
		(void*)dlg);
	editor_bf_create(dlg->editor_bf_outlet,EDITOR_BF_TYPE_IO,
		(void(*)())editor_channel_update_outlet,editor_channel_draw_outlet,
		(void*)dlg);
	editor_channel_hbox(&dlg->hbox_bf,&dlg->label_bf,
		&dlg->combo_bf,&dlg->button_bf_up,&dlg->button_bf_down,0);
	dlg->button_insert_bf =
		(GtkButton*)gtk_button_new_with_mnemonic(gettext("_Insert"));
	dlg->button_delete_bf =
		(GtkButton*)gtk_button_new_with_mnemonic(gettext("_Delete"));
	gtk_box_pack_start((GtkBox*)dlg->hbox_bf,
		(GtkWidget*)dlg->button_insert_bf,0,1,0);
	gtk_box_pack_start((GtkBox*)dlg->hbox_bf,
		(GtkWidget*)dlg->button_delete_bf,0,1,0);
	editor_bf_create(dlg->editor_bf_inner,EDITOR_BF_TYPE_INNER,
		(void(*)())editor_channel_update_inner,editor_channel_draw_inner,
		(void*)dlg);
	dlg->label_inner_initial =
		(GtkLabel*)gtk_label_new(gettext("Initial section"));
	dlg->entry_inner_initial = jbw_int_entry_new();
	dlg->label_inner_final = (GtkLabel*)gtk_label_new(gettext("Final section"));
	dlg->entry_inner_final = jbw_int_entry_new();
	gtk_box_pack_start((GtkBox*)dlg->editor_bf_inner->hbox2,
		(GtkWidget*)dlg->label_inner_initial,0,0,0);
	gtk_box_pack_start((GtkBox*)dlg->editor_bf_inner->hbox2,
		(GtkWidget*)dlg->entry_inner_initial,0,1,0);
	gtk_box_pack_start((GtkBox*)dlg->editor_bf_inner->hbox2,
		(GtkWidget*)dlg->label_inner_final,0,0,0);
	gtk_box_pack_start((GtkBox*)dlg->editor_bf_inner->hbox2,
		(GtkWidget*)dlg->entry_inner_final,0,1,0);
	dlg->vbox_inner3 = (GtkVBox*)gtk_vbox_new(0,0);
	gtk_box_pack_start((GtkBox*)dlg->vbox_inner3,(GtkWidget*)dlg->hbox_bf,
		0,1,0);
	gtk_box_pack_start_defaults((GtkBox*)dlg->vbox_inner3,
		(GtkWidget*)dlg->editor_bf_inner->table);

	dlg->notebook_bf = (GtkNotebook*)gtk_notebook_new();
	gtk_notebook_append_page(dlg->notebook_bf,
		(GtkWidget*)dlg->editor_bf_inlet->table,
		(GtkWidget*)gtk_label_new(gettext("Inlet")));
	gtk_notebook_append_page(dlg->notebook_bf,
		(GtkWidget*)dlg->editor_bf_outlet->table,
		(GtkWidget*)gtk_label_new(gettext("Outlet")));
	gtk_notebook_append_page(dlg->notebook_bf,(GtkWidget*)dlg->vbox_inner3,
		(GtkWidget*)gtk_label_new(gettext("Inner conditions")));
#endif

	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr,"ECN: transport initial conditions\n");
	#endif
	dlg->hbox_it = (GtkHBox*)gtk_hbox_new(0,0);
	dlg->button_it[0] = 0;
	for (i=0; i<N_INITIAL_TRANSPORT; ++i)
	{
		dlg->button_it[i] = (GtkRadioButton*)
			gtk_radio_button_new_with_mnemonic_from_widget(dlg->button_it[0],
			label_button_it[i]);
		gtk_box_pack_start_defaults((GtkBox*)dlg->hbox_it,
			(GtkWidget*)dlg->button_it[i]);
		g_signal_connect_swapped(dlg->button_it[i],"clicked",
			(void(*)())editor_channel_update_it,dlg);
	}
	dlg->frame_it = (GtkFrame*)gtk_frame_new(gettext("Type"));
	gtk_container_add((GtkContainer*)dlg->frame_it,(GtkWidget*)dlg->hbox_it);
	dlg->button_insert_it =
		(GtkButton*)gtk_button_new_with_mnemonic(gettext("Insert _point"));
	dlg->button_delete_it =
		(GtkButton*)gtk_button_new_with_mnemonic(gettext("Delete poi_nt"));
	dlg->button_update_it =
		(GtkButton*)gtk_button_new_with_mnemonic(gettext("Update p_lot"));
	editor_channel_vbox(&dlg->vbox_it,dlg->button_insert_it,
		dlg->button_delete_it,dlg->button_update_it);
	dlg->editor_it = jbw_array_editor_new(2,2,1,label_it);
	dlg->table_it = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach(dlg->table_it,
		(GtkWidget*)dlg->editor_it->scrolled,
		0,0,1,1);
	gtk_grid_attach(dlg->table_it,(GtkWidget*)dlg->vbox_it,
		1,0,1,1);
	dlg->graphic_it =
		(JBWGraphic*)jbw_graphic_new(0,5,5,5,editor_channel_draw_it);
	jbw_graphic_set_xlabel(dlg->graphic_it,gettext("x"));
	jbw_graphic_set_ylabel(dlg->graphic_it,gettext("Concentration"));
	jbw_graphic_set_logo(dlg->graphic_it, "swigs.png");
	editor_paned(&dlg->paned_it,dlg->table_it,dlg->graphic_it);
	dlg->vbox_it2 = (GtkVBox*)gtk_vbox_new(0,0);
	gtk_box_pack_start((GtkBox*)dlg->vbox_it2,(GtkWidget*)dlg->frame_it,0,1,0);
	gtk_box_pack_start_defaults((GtkBox*)dlg->vbox_it2,
		(GtkWidget*)dlg->paned_it);

	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr,"ECN: transport boundary conditions\n");
	#endif
#if EDITOR_BF
	editor_channel_hbox(&dlg->hbox_bt,&dlg->label_bt,&dlg->combo_bt,
		&dlg->button_bt_up,&dlg->button_bt_down,
		gettext("Transport boundary condition"));
	dlg->button_insert_bts =
		(GtkButton*)gtk_button_new_with_mnemonic(gettext("_Insert"));
	dlg->button_delete_bts =
		(GtkButton*)gtk_button_new_with_mnemonic(gettext("_Delete"));
	gtk_box_pack_start((GtkBox*)dlg->hbox_bt,
		(GtkWidget*)dlg->button_insert_bts,0,1,0);
	gtk_box_pack_start((GtkBox*)dlg->hbox_bt,
		(GtkWidget*)dlg->button_delete_bts,0,1,0);
	dlg->hbox_bt2 = (GtkHBox*)gtk_hbox_new(0,0);
	dlg->vbox_bt = (GtkVBox*)gtk_vbox_new(0,0);
	dlg->label_bt =
		(GtkLabel*)gtk_label_new(gettext("Flow boundary condition"));
	dlg->entry_bt = (GtkEntry*)gtk_entry_new();
	gtk_box_pack_start((GtkBox*)dlg->hbox_bt2,(GtkWidget*)dlg->label_bt,
		0,1,0);
	gtk_box_pack_start_defaults((GtkBox*)dlg->hbox_bt2,
		(GtkWidget*)dlg->entry_bt);
	dlg->button_bt[0] = 0;
	for (i=0; i<N_BOUNDARY_TRANSPORT; ++i)
	{
		dlg->button_bt[i] = (GtkRadioButton*)
			gtk_radio_button_new_with_mnemonic_from_widget
				(dlg->button_bt[0],label_bt[i]);
		gtk_box_pack_start_defaults
			((GtkBox*)dlg->vbox_bt,(GtkWidget*)dlg->button_bt[i]);
		g_signal_connect_swapped(dlg->button_bt[i],"clicked",
			(void(*))editor_channel_update_bt,dlg);
	}
	dlg->frame_bt = (GtkFrame*)gtk_frame_new(gettext("Type"));
	gtk_container_add((GtkContainer*)dlg->frame_bt,(GtkWidget*)dlg->vbox_bt);
	dlg->label_bt_time = (GtkLabel*)gtk_label_new(gettext("Time"));
	dlg->label_bt_mass = (GtkLabel*)gtk_label_new(gettext("Mass"));
	dlg->entry_bt_time = jbw_float_entry_new();
	dlg->entry_bt_mass = jbw_float_entry_new();
	dlg->button_insert_bt = 
		(GtkButton*)gtk_button_new_with_mnemonic(gettext("Insert _point"));
	dlg->button_delete_bt =
		(GtkButton*)gtk_button_new_with_mnemonic(gettext("Delete poi_nt"));
	dlg->button_update_bt =
		(GtkButton*)gtk_button_new_with_mnemonic(gettext("Update p_lot"));
	editor_channel_vbox(&dlg->vbox_bt2,dlg->button_insert_bt,
		dlg->button_delete_bt,dlg->button_update_bt);
	dlg->editor_bt = jbw_array_editor_new(2,2,1,label_bt2);
	dlg->table_bt2 = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach(dlg->table_bt2,
		(GtkWidget*)dlg->editor_bt->scrolled,
		0,0,1,1);
	gtk_grid_attach(dlg->table_bt2,(GtkWidget*)dlg->vbox_bt2,
		1,0,1,1);
	dlg->graphic_bt =
		(JBWGraphic*)jbw_graphic_new(0,5,5,5,editor_channel_draw_bt);
	jbw_graphic_set_logo(dlg->graphic_bt, "swigs.png");
	editor_paned(&dlg->paned_bt,dlg->table_bt2,dlg->graphic_bt);
	dlg->table_bt = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach(dlg->table_bt,(GtkWidget*)dlg->hbox_bt2,
		0,0,3,1);
	gtk_grid_attach(dlg->table_bt,(GtkWidget*)dlg->frame_bt,
		0,1,1,7);
	gtk_grid_attach(dlg->table_bt,(GtkWidget*)dlg->label_bt_time,
		1,1,1,1);
	gtk_grid_attach(dlg->table_bt,(GtkWidget*)dlg->entry_bt_time,
		2,1,1,1);
	gtk_grid_attach(dlg->table_bt,(GtkWidget*)dlg->label_bt_mass,
		1,2,1,1);
	gtk_grid_attach(dlg->table_bt,(GtkWidget*)dlg->entry_bt_mass,
		2,2,1,1);
	gtk_grid_attach(dlg->table_bt,(GtkWidget*)dlg->paned_bt,
		1,3,2,1);
	dlg->label_bt_initial =
		(GtkLabel*)gtk_label_new(gettext("Initial section"));
	dlg->entry_bt_initial = jbw_int_entry_new();
	dlg->label_bt_final = (GtkLabel*)gtk_label_new(gettext("Final section"));
	dlg->entry_bt_final = jbw_int_entry_new();
	gtk_box_pack_start
		((GtkBox*)dlg->hbox_bt2,(GtkWidget*)dlg->label_bt_initial,0,0,0);
	gtk_box_pack_start
		((GtkBox*)dlg->hbox_bt2,(GtkWidget*)dlg->entry_bt_initial,0,1,0);
	gtk_box_pack_start
		((GtkBox*)dlg->hbox_bt2,(GtkWidget*)dlg->label_bt_final,0,0,0);
	gtk_box_pack_start
		((GtkBox*)dlg->hbox_bt2,(GtkWidget*)dlg->entry_bt_final,0,1,0);
	dlg->vbox_bt2 = (GtkVBox*)gtk_vbox_new(0,0);
	gtk_box_pack_start((GtkBox*)dlg->vbox_bt2,(GtkWidget*)dlg->hbox_bt,
		0,1,0);
	gtk_box_pack_start_defaults((GtkBox*)dlg->vbox_bt2,
		(GtkWidget*)dlg->table_bt);
#endif

	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr,"ECN: transport\n");
	#endif
	editor_channel_hbox(&dlg->hbox_transport,&dlg->label_transport,
		&dlg->combo_transport,&dlg->button_transport_up,
		&dlg->button_transport_down,gettext("Transport"));
	dlg->button_insert_transport =
		(GtkButton*)gtk_button_new_with_mnemonic(gettext("_Insert"));
	dlg->button_delete_transport =
		(GtkButton*)gtk_button_new_with_mnemonic(gettext("_Delete"));
	gtk_box_pack_start((GtkBox*)dlg->hbox_transport,
		(GtkWidget*)dlg->button_insert_transport,0,1,0);
	gtk_box_pack_start((GtkBox*)dlg->hbox_transport,
		(GtkWidget*)dlg->button_delete_transport,0,1,0);
	dlg->label_transport = (GtkLabel*)gtk_label_new(gettext("Transport"));
	dlg->entry_transport = (GtkEntry*)gtk_entry_new();
	dlg->label_solubility = (GtkLabel*)gtk_label_new(gettext("Solubility"));
	dlg->entry_solubility = jbw_float_entry_new();
	dlg->hbox_transport2 = (GtkHBox*)gtk_hbox_new(0,0);
	gtk_box_pack_start((GtkBox*)dlg->hbox_transport2,
		(GtkWidget*)dlg->label_transport,0,1,0);
	gtk_box_pack_start_defaults((GtkBox*)dlg->hbox_transport2,
		(GtkWidget*)dlg->entry_transport);
	gtk_box_pack_start((GtkBox*)dlg->hbox_transport2,
		(GtkWidget*)dlg->label_solubility,0,1,0);
	gtk_box_pack_start_defaults((GtkBox*)dlg->hbox_transport2,
		(GtkWidget*)dlg->entry_solubility);
	dlg->notebook_transport = (GtkNotebook*)gtk_notebook_new();
	gtk_notebook_append_page(dlg->notebook_transport,(GtkWidget*)dlg->vbox_it2,
		(GtkWidget*)gtk_label_new(gettext("Transport initial conditions")));
	gtk_notebook_append_page(dlg->notebook_transport,(GtkWidget*)dlg->vbox_bt2,
		(GtkWidget*)gtk_label_new(gettext("Transport boundary condition")));
	dlg->vbox_transport = (GtkVBox*)gtk_vbox_new(0,0);
	gtk_box_pack_start((GtkBox*)dlg->vbox_transport,
		(GtkWidget*)dlg->hbox_transport,0,1,0);
	gtk_box_pack_start((GtkBox*)dlg->vbox_transport,
		(GtkWidget*)dlg->hbox_transport2,0,1,0);
	gtk_box_pack_start((GtkBox*)dlg->vbox_transport,
		(GtkWidget*)dlg->notebook_transport,1,1,0);

	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr,"ECN: notebook\n");
	#endif
	dlg->notebook = (GtkNotebook*)gtk_notebook_new();
	gtk_notebook_append_page(dlg->notebook,(GtkWidget*)dlg->paned_geometry,
		(GtkWidget*)gtk_label_new(gettext("Geometry")));
	gtk_notebook_append_page(dlg->notebook,(GtkWidget*)dlg->paned_section,
		(GtkWidget*)gtk_label_new(gettext("Cross sections")));
	gtk_notebook_append_page(dlg->notebook,(GtkWidget*)dlg->paned_transient,
		(GtkWidget*)gtk_label_new(gettext("Transient sections")));
	gtk_notebook_append_page(dlg->notebook,(GtkWidget*)dlg->vbox_if2,
		(GtkWidget*)gtk_label_new(gettext("Flow initial conditions")));
	gtk_notebook_append_page(dlg->notebook,(GtkWidget*)dlg->notebook_bf,
		(GtkWidget*)gtk_label_new(gettext("Flow boundary conditions")));
	gtk_notebook_append_page(dlg->notebook,(GtkWidget*)dlg->vbox_transport,
		(GtkWidget*)gtk_label_new(gettext("Transport")));	

	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr,"ECN: window\n");
	#endif
	snprintf(buffer,JB_BUFFER_SIZE,gettext("Channel editor: %s/%s"),
		c->directory,c->name);
	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr,"ECN: buffer=%s\n", buffer);
	#endif
	dlg->window = (GtkDialog*)gtk_dialog_new_with_buttons(buffer,
		dialog_simulator->window,GTK_DIALOG_MODAL,
		GTK_STOCK_OK,GTK_RESPONSE_OK,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,0);
	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr,"ECN: packing notebook\n");
	#endif
	gtk_box_pack_start_defaults
		((GtkBox*)dlg->window->vbox,(GtkWidget*)dlg->notebook);
	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr,"ECN: showing all\n");
	#endif
	gtk_widget_show_all((GtkWidget*)dlg->notebook);

	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr,"ECN: signals\n");
	#endif
	g_signal_connect_swapped
		(dlg->window,"destroy",(void(*)())editor_channel_destroy,dlg);

	dlg->id_combo_transient=g_signal_connect_swapped(dlg->combo_transient,
		"changed",(void(*)())editor_channel_set_transient,dlg);
	dlg->id_combo_section=g_signal_connect_swapped(dlg->combo_section,"changed",
		(void(*)())editor_channel_set_section,dlg);
#if EDITOR_BF
	dlg->id_combo_bf = jbw_signal_connect_swapped_after(dlg->combo_bf,"changed",
		(void(*)())editor_channel_update_entry_bf,dlg);
	dlg->id_combo_bf2 = g_signal_connect_swapped(dlg->combo_bf,"changed",
		(void(*)())editor_channel_update_inner,dlg);
	dlg->id_entry_bf = jbw_signal_connect_swapped_after
		(dlg->editor_bf_inner->entry_name,"changed",
			(void(*)())editor_channel_update_combo_bf,dlg);
	dlg->id_combo_bt = jbw_signal_connect_swapped_after(dlg->combo_bt,"changed",
		(void(*)())editor_channel_update_entry_bts,dlg);
	dlg->id_combo_bt2 = g_signal_connect_swapped(dlg->combo_bt,"changed",
		(void(*)())editor_channel_update_bts,dlg);
	dlg->id_entry_bt = jbw_signal_connect_swapped_after(dlg->entry_bt,"changed",
		(void(*)())editor_channel_update_combo_bts,dlg);
#endif
	dlg->id_combo_transport=jbw_signal_connect_swapped_after
		(dlg->combo_transport,"changed",
			(void(*)())editor_channel_update_entry_transport,dlg);
	dlg->id_combo_transport2 = g_signal_connect_swapped(dlg->combo_transport,
		"changed",(void(*)())editor_channel_update_transport,dlg);
	dlg->id_entry_transport=jbw_signal_connect_swapped_after
		(dlg->entry_transport,"changed",
			(void(*)())editor_channel_update_combo_transport,dlg);
	dlg->id_notebook_transport=jbw_signal_connect_swapped_after
		(dlg->notebook_transport,"switch-page",
			(void(*)())editor_channel_update_notebook_transport,dlg);
	dlg->id_notebook=jbw_signal_connect_swapped_after(dlg->notebook,
		"switch-page",(void(*)())editor_channel_update_notebook,dlg);

	dlg->ibf = 0;
	dlg->ict = 0;
	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr,"ECN nt=%d\n",c->nt);
	#endif
	if (c->nt >= 0) editor_channel_open_transport(dlg);

#if EDITOR_BF
	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr,"ECN opening boundary conditions\n");
	#endif
	editor_channel_open_bf(dlg);
	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr,"ECN opening outlet\n");
	#endif
	editor_channel_open_outlet(dlg,c->bf+c->n);
	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr,"ECN opening inlet\n");
	#endif
	editor_channel_open_inlet(dlg,c->bf);
	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr,"ECN opening initial conditions\n");
	#endif
#endif

	editor_channel_open_if(dlg,c->fic);
	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr,"ECN opening geometry\n");
	#endif
	editor_channel_open_geometry(dlg,c->cg);

	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr,"ECN connecting\n");
	#endif
	g_signal_connect_swapped(dlg->button_delete_geometry,"clicked",
		(void(*)())editor_channel_delete_geometry,dlg);
	g_signal_connect_swapped(dlg->button_insert_geometry,"clicked",
		(void(*)())editor_channel_insert_geometry,dlg);

	g_signal_connect_swapped(dlg->button_insert_section,"clicked",
		(void(*)())editor_channel_insert_section,dlg);
	g_signal_connect_swapped(dlg->button_delete_section,"clicked",
		(void(*)())editor_channel_delete_section,dlg);
	g_signal_connect_swapped(dlg->button_section_up,"clicked",
		(void(*)())editor_channel_up_section,dlg);
	g_signal_connect_swapped(dlg->button_section_down,"clicked",
		(void(*)())editor_channel_down_section,dlg);

	g_signal_connect_swapped(dlg->button_insert_transient,"clicked",
		(void(*)())editor_channel_insert_transient,dlg);
	g_signal_connect_swapped(dlg->button_delete_transient,"clicked",
		(void(*)())editor_channel_delete_transient,dlg);
	g_signal_connect_swapped(dlg->button_transient_up,"clicked",
		(void(*)())editor_channel_up_transient,dlg);
	g_signal_connect_swapped(dlg->button_transient_down,"clicked",
		(void(*)())editor_channel_down_transient,dlg);

	g_signal_connect_swapped(dlg->button_update,"clicked",
		(void(*)())editor_channel_update,dlg);

	g_signal_connect_swapped(dlg->button_insert_if,"clicked",
		(void(*)())editor_channel_insert_if,dlg);
	g_signal_connect_swapped(dlg->button_delete_if,"clicked",
		(void(*)())editor_channel_delete_if,dlg);
	g_signal_connect_swapped(dlg->button_update_if,"clicked",
		(void(*)())editor_channel_update,dlg);

#if EDITOR_BF
	g_signal_connect_swapped(dlg->editor_bf_inlet->button_insert,"clicked",
		(void(*)())editor_channel_insert_inlet,dlg);
	g_signal_connect_swapped(dlg->editor_bf_inlet->button_delete,"clicked",
		(void(*)())editor_channel_delete_inlet,dlg);
	g_signal_connect_swapped(dlg->editor_bf_inlet->button_update,"clicked",
		(void(*)())editor_channel_update,dlg);

	g_signal_connect_swapped(dlg->editor_bf_outlet->button_insert,"clicked",
		(void(*)())editor_channel_insert_outlet,dlg);
	g_signal_connect_swapped(dlg->editor_bf_outlet->button_delete,"clicked",
		(void(*)())editor_channel_delete_outlet,dlg);
	g_signal_connect_swapped(dlg->editor_bf_outlet->button_update,"clicked",
		(void(*)())editor_channel_update,dlg);

	g_signal_connect_swapped(dlg->editor_bf_inner->button_insert,"clicked",
		(void(*)())editor_channel_insert_inner,dlg);
	g_signal_connect_swapped(dlg->editor_bf_inner->button_delete,"clicked",
		(void(*)())editor_channel_delete_inner,dlg);
	g_signal_connect_swapped(dlg->editor_bf_inner->button_update,"clicked",
		(void(*)())editor_channel_update,dlg);

	g_signal_connect_swapped(dlg->button_bf_up,"clicked",
		(void(*)())editor_channel_up_bf,dlg);
	g_signal_connect_swapped(dlg->button_bf_down,"clicked",
		(void(*)())editor_channel_down_bf,dlg);
	g_signal_connect_swapped(dlg->button_insert_bf,"clicked",
		(void(*)())editor_channel_insert_bf,dlg);
	g_signal_connect_swapped(dlg->button_delete_bf,"clicked",
		(void(*)())editor_channel_delete_bf,dlg);
#endif

	g_signal_connect_swapped(dlg->button_insert_it,"clicked",
		(void(*)())editor_channel_insert_it,dlg);
	g_signal_connect_swapped(dlg->button_delete_it,"clicked",
		(void(*)())editor_channel_delete_it,dlg);
	g_signal_connect_swapped(dlg->button_update_it,"clicked",
		(void(*)())editor_channel_update,dlg);

#if EDITOR_BF
	g_signal_connect_swapped(dlg->button_bt_up,"clicked",
		(void(*)())editor_channel_up_bts,dlg);
	g_signal_connect_swapped(dlg->button_bt_down,"clicked",
		(void(*)())editor_channel_down_bts,dlg);
	g_signal_connect_swapped(dlg->button_insert_bts,"clicked",
		(void(*)())editor_channel_insert_bts,dlg);
	g_signal_connect_swapped(dlg->button_delete_bts,"clicked",
		(void(*)())editor_channel_delete_bts,dlg);
	g_signal_connect_swapped(dlg->button_insert_bt,"clicked",
		(void(*)())editor_channel_insert_bt,dlg);
	g_signal_connect_swapped(dlg->button_delete_bt,"clicked",
		(void(*)())editor_channel_delete_bt,dlg);
	g_signal_connect_swapped(dlg->button_update_bt,"clicked",
		(void(*)())editor_channel_update_bt,dlg);
#endif

	g_signal_connect_swapped(dlg->button_transport_up,"clicked",
		(void(*)())editor_channel_up_transport,dlg);
	g_signal_connect_swapped(dlg->button_transport_down,"clicked",
		(void(*)())editor_channel_down_transport,dlg);
	g_signal_connect_swapped(dlg->button_insert_transport,"clicked",
		(void(*)())editor_channel_insert_transport,dlg);
	g_signal_connect_swapped(dlg->button_delete_transport,"clicked",
		(void(*)())editor_channel_delete_transport,dlg);

	window_parent = (GtkWindow*)dlg->window;

error1:
	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr,"ECN connected\n");
	#endif
	if (gtk_dialog_run(dlg->window)==GTK_RESPONSE_OK)
	{
		#if DEBUG_EDITOR_CHANNEL_NEW
			fprintf(stderr,"ECN actualizing\n");
		#endif
		editor_channel_update(dlg);
		dlg->c->bf[0].pos = 1;
		dlg->c->bf[c->n].pos = dlg->c->cg->n + 1;
		if (!channel_boundary_test(dlg->c)) goto error1;
		channel_delete(dlg->cv);
		channel_copy(dlg->cv,dlg->c);
		gtk_widget_destroy((GtkWidget*)dlg->window);
		opening=1;
		simulate_start();
		opening=0;
	}
	else gtk_widget_destroy((GtkWidget*)dlg->window);

	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr,"Editor channel new: end\n");
	#endif
}
