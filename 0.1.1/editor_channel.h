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

#ifndef EDITOR_CHANNEL__H
#define EDITOR_CHANNEL__H 1

#include "channel.h"

#define EDITOR_BF 1

#define N_SECTION_TYPE 4

typedef struct
{
	GtkLabel *label_name,*label_discharge,*label_depth,*label_level,
		*label_data1,*label_data2;
	GtkEntry *entry_name,*entry_data1,*entry_data2;
	JBWFloatEntry *entry_discharge,*entry_depth,*entry_level;
	GtkRadioButton **button_type;
	GtkFrame *frame;
	GtkButton *button_insert,*button_delete,*button_update;
	JBWArrayEditor *editor;
	JBWGraphic *graphic;
	GtkHBox *hbox,*hbox2;
	GtkVBox *vbox,*vbox2;
	GtkGrid *table,*table2;
	GtkVPaned *paned;
} EditorBF;

typedef enum
{
	EDITOR_BF_TYPE_IO=0,
	EDITOR_BF_TYPE_INNER=1
} EditorBFType;

enum EditorChannelBFPage
{
	EDITOR_CHANNEL_BF_PAGE_INLET=0,
	EDITOR_CHANNEL_BF_PAGE_OUTLET=1,
	EDITOR_CHANNEL_BF_PAGE_INNER=2
};

typedef struct
{
	int ics,its,ibf,ict,ibt,id_combo_transient,id_combo_section,id_combo_bf,
		id_combo_bf2,id_combo_bt,id_combo_bt2,id_combo_transport,
		id_combo_transport2,id_notebook,id_notebook_transport,id_entry_bf,
		id_entry_bt,id_entry_transport;
	char name[JB_BUFFER_SIZE];
	Channel c[1],*cv;
	GtkLabel *label_x,*label_y,*label_angle,*label_transient,*label_section,
		*label_variations,
		*label_control,*label_tolerance,*label_time,*label_parameter,
		*label_bf,*label_inner_initial,*label_inner_final,
		*label_transport,*label_solubility,*label_it,*label_bt,
		*label_bt_initial,*label_bt_final,*label_bt_time,*label_bt_mass;
	GtkEntry *entry_transport,*entry_bt;
	JBWIntEntry *entry_control,*entry_inner_initial,*entry_inner_final,
		*entry_bt_initial,*entry_bt_final;
	JBWFloatEntry *entry_x,*entry_y,*entry_angle,*entry_tolerance,*entry_time,
		*entry_parameter,*entry_solubility,*entry_bt_time,*entry_bt_mass;
	GtkButton *button_insert_if,*button_delete_if,*button_update_if,
		*button_geometry_up,*button_geometry_down,
		*button_new_geometry,*button_insert_geometry,*button_delete_geometry,
		*button_section_up,*button_section_down,
		*button_new_section,*button_insert_section,*button_delete_section,
		*button_transient_up,*button_transient_down,
		*button_insert_transient,*button_delete_transient,*button_update,
		*button_bf_up,*button_bf_down,*button_insert_bf,*button_delete_bf,
		*button_transport_up,*button_transport_down,
		*button_insert_transport,*button_delete_transport,
		*button_insert_it,*button_delete_it,*button_update_it,
		*button_insert_bts,*button_delete_bts,*button_bt_up,*button_bt_down,
		*button_insert_bt,*button_delete_bt,*button_update_bt;
	GtkRadioButton *button_section[N_SECTION_TYPE],*button_if[N_INITIAL_FLOW],
		*button_it[N_INITIAL_TRANSPORT],*button_bt[N_BOUNDARY_TRANSPORT];
	GtkFrame *frame_section,*frame_if,*frame_it,*frame_bt;
	GtkComboBoxText *combo_transient,*combo_section,*combo_bf,*combo_transport,
		*combo_bt;
	JBWArrayEditor *editor_transient,*editor_section,*editor_geometry,
		*editor_if,*editor_it,*editor_bt;
	JBWGraphic *graphic_transient,*graphic_section,*graphic_geometry,
		*graphic_if,*graphic_it,*graphic_bt;
	GtkHBox *hbox_transient,*hbox_section,*hbox_if,*hbox_bf,
		*hbox_transport,*hbox_transport2,*hbox_it,*hbox_bt,*hbox_bt2;
	GtkVBox *vbox_transient,*vbox_section,*vbox_section_type,*vbox_geometry,
		*vbox_if,*vbox_if2,*vbox_inner3,*vbox_transport,*vbox_it,*vbox_it2,
		*vbox_bt,*vbox_bt2;
	GtkGrid *table_transient,*table_section,*table_section_type,*table_geometry,
		*table_if,*table_it,*table_bt,*table_bt2;
	GtkVPaned *paned_transient,*paned_section,*paned_geometry,*paned_if,
		*paned_it,*paned_bt;
	GtkNotebook *notebook,*notebook_bf,*notebook_transport;
	GtkDialog *window;
	EditorBF editor_bf_inlet[1],editor_bf_outlet[1],editor_bf_inner[1];
} EditorChannel;

enum EditorChannelPage
{
	EDITOR_CHANNEL_PAGE_GEOMETRY=0,
	EDITOR_CHANNEL_PAGE_SECTION=1,
	EDITOR_CHANNEL_PAGE_TRANSIENT=2,
	EDITOR_CHANNEL_PAGE_IF=3,
	EDITOR_CHANNEL_PAGE_BF=4,
	EDITOR_CHANNEL_PAGE_TRANSPORT=5
};

enum EditorChannelPageTransport
{
	EDITOR_CHANNEL_PAGE_TRANSPORT_IT=0,
	EDITOR_CHANNEL_PAGE_TRANSPORT_BT=1
};

extern EditorChannel editor[1];

void editor_paned(GtkVPaned **paned,GtkGrid *table,JBWGraphic *graphic);

void editor_bf_get(EditorBF *editor,BoundaryFlow *bf,EditorBFType type);
void editor_bf_update(EditorBF *editor,BoundaryFlow *bf,EditorBFType type);
void editor_bf_open(EditorBF *editor,BoundaryFlow *bf,EditorBFType type);
void editor_bf_create(EditorBF *editor,EditorBFType type,
	void (*editor_update),void (*editor_draw),void *dlg);

void editor_channel_insert_point(JBWArrayEditor *dlg);
void editor_channel_delete_point(JBWArrayEditor *dlg);

void editor_channel_get_transient(EditorChannel *dlg);
void editor_channel_update_transient(EditorChannel *dlg);
void editor_channel_open_transient(EditorChannel *dlg,TransientSection *ts);
void editor_channel_set_transient(EditorChannel *dlg);
void editor_channel_up_transient(EditorChannel *dlg);
void editor_channel_down_transient(EditorChannel *dlg);
void editor_channel_insert_transient(EditorChannel *dlg);
void editor_channel_delete_transient(EditorChannel *dlg);
void editor_channel_draw_transient();

void editor_channel_get_section(EditorChannel *dlg);
void editor_channel_update_section(EditorChannel *dlg);
void editor_channel_open_section(EditorChannel *dlg,CrossSection *cs);
void editor_channel_set_section(EditorChannel *dlg);
void editor_channel_up_section(EditorChannel *dlg);
void editor_channel_down_section(EditorChannel *dlg);
void editor_channel_insert_section(EditorChannel *dlg);
void editor_channel_delete_section(EditorChannel *dlg);
void editor_channel_draw_section();

void editor_channel_get_geometry(EditorChannel *dlg);
void editor_channel_update_geometry(EditorChannel *dlg);
void editor_channel_open_geometry(EditorChannel *dlg,ChannelGeometry *cg);
void editor_channel_insert_geometry(EditorChannel *dlg);
void editor_channel_delete_geometry(EditorChannel *dlg);
void editor_channel_draw_geometry();

void editor_channel_get_if(EditorChannel *dlg);
void editor_channel_update_if(EditorChannel *dlg);
void editor_channel_open_if(EditorChannel *dlg,InitialFlow *fic);
void editor_channel_insert_if(EditorChannel *dlg);
void editor_channel_delete_if(EditorChannel *dlg);
void editor_channel_draw_if();

#if EDITOR_BF
void editor_channel_update_inlet(EditorChannel *dlg);
void editor_channel_open_inlet(EditorChannel *dlg,BoundaryFlow *bf);
void editor_channel_insert_inlet(EditorChannel *dlg);
void editor_channel_delete_inlet(EditorChannel *dlg);
void editor_channel_draw_inlet();

void editor_channel_update_outlet(EditorChannel *dlg);
void editor_channel_open_outlet(EditorChannel *dlg,BoundaryFlow *bf);
void editor_channel_insert_outlet(EditorChannel *dlg);
void editor_channel_delete_outlet(EditorChannel *dlg);
void editor_channel_draw_outlet();

void editor_channel_update_inner(EditorChannel *dlg);
void editor_channel_open_inner(EditorChannel *dlg,BoundaryFlow *bf);
void editor_channel_insert_inner(EditorChannel *dlg);
void editor_channel_delete_inner(EditorChannel *dlg);
void editor_channel_draw_inner();

void editor_channel_update_bf(EditorChannel *dlg);
void editor_channel_update_combo_bf(EditorChannel *dlg);
void editor_channel_update_entry_bf(EditorChannel *dlg);
void editor_channel_up_bf(EditorChannel *dlg);
void editor_channel_down_bf(EditorChannel *dlg);
void editor_channel_open_bf(EditorChannel *dlg);
void editor_channel_insert_bf(EditorChannel *dlg);
void editor_channel_delete_bf(EditorChannel *dlg);
#endif

void editor_channel_get_it(EditorChannel *dlg);
void editor_channel_update_it(EditorChannel *dlg);
void editor_channel_open_it(EditorChannel *dlg,InitialTransport *it);
void editor_channel_insert_it(EditorChannel *dlg);
void editor_channel_delete_it(EditorChannel *dlg);
void editor_channel_draw_it();

#if EDITOR_BF
void editor_channel_get_bt(EditorChannel *dlg);
void editor_channel_update_bt(EditorChannel *dlg);
void editor_channel_open_bt(EditorChannel *dlg,BoundaryTransport *bt);
void editor_channel_insert_bt(EditorChannel *dlg);
void editor_channel_delete_bt(EditorChannel *dlg);
void editor_channel_draw_bt();

void editor_channel_update_bts(EditorChannel *dlg);
void editor_channel_update_combo_bts(EditorChannel *dlg);
void editor_channel_update_entry_bts(EditorChannel *dlg);
void editor_channel_up_bts(EditorChannel *dlg);
void editor_channel_down_bts(EditorChannel *dlg);
void editor_channel_open_bts(EditorChannel *dlg);
void editor_channel_insert_bts(EditorChannel *dlg);
void editor_channel_delete_bts(EditorChannel *dlg);
#endif

void editor_channel_update_transport(EditorChannel *dlg);
void editor_channel_update_combo_transport(EditorChannel *dlg);
void editor_channel_update_entry_transport(EditorChannel *dlg);
void editor_channel_up_transport(EditorChannel *dlg);
void editor_channel_down_transport(EditorChannel *dlg);
void editor_channel_open_transport(EditorChannel *dlg);
void editor_channel_insert_transport(EditorChannel *dlg);
void editor_channel_delete_transport(EditorChannel *dlg);

void editor_channel_update_notebook_transport(EditorChannel *dlg);
void editor_channel_update_notebook(EditorChannel *dlg);
void editor_channel_update(EditorChannel *dlg);

void editor_channel_destroy(EditorChannel *dlg);

void editor_channel_hbox(GtkHBox **box,GtkLabel **label,GtkComboBoxText **combo,
	GtkButton **button_up,GtkButton **button_down,char *string);
void editor_channel_vbox
	(GtkVBox **box,GtkButton *button1,GtkButton *button2,GtkButton *button3);
void editor_channel_new(EditorChannel *dlg,Channel *c);
#endif
