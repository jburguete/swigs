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
 * \file editor_channel.c
 * \brief Source file to define channel editor structures and functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2015 Javier Burguete Tolosa.
 */
#include "editor_channel.h"

/**
 * \def EDITOR_CHANNEL
 * \brief Macro to define the position of the channel editor in the notebook.
 * \def EDITOR_CROSS_SECTION
 * \brief Macro to define the position of the cross section editor in the
 *   notebook.
 * \def EDITOR_TRANSIENT_SECTION
 * \brief Macro to define the position of the transient section editor in the
 *   notebook.
 * \def EDITOR_INITIAL_FLOW
 * \brief Macro to define the position of the flow initial conditions editor in
 *   the notebook.
 * \def EDITOR_INLET
 * \brief Macro to define the position of the inlet flow boundary condition
 *   editor in the notebook.
 * \def EDITOR_OUTLET
 * \brief Macro to define the position of the outlet flow boundary condition
 *   editor in the notebook.
 * \def EDITOR_INNER
 * \brief Macro to define the position of the inner flow boundary conditions
 *   editor in the notebook.
 */
#if TEST_EDITOR_CHANNEL
	#define EDITOR_CHANNEL 0
	char *message;
	EditorChannel editor[1];
#else
	#define EDITOR_CHANNEL 1
#endif
#define EDITOR_CROSS_SECTION (EDITOR_CHANNEL + 1)
#define EDITOR_TRANSIENT_SECTION (EDITOR_CROSS_SECTION + 1)
#define EDITOR_INITIAL_FLOW (EDITOR_TRANSIENT_SECTION + 1)
#define EDITOR_INLET (EDITOR_INITIAL_FLOW + 1)
#define EDITOR_OUTLET (EDITOR_INLET + 1)
#define EDITOR_INNER (EDITOR_OUTLET + 1)

/**
 * \fn void editor_channel_update(EditorChannel *editor)
 * \brief Function to update the view of a channel editor.
 * \param editor
 * \brief channel geometry editor.
 */
void editor_channel_update(EditorChannel *editor)
{
	#if DEBUG_EDITOR_CHANNEL_UPDATE
		fprintf(stderr, "editor_channel_update: start\n");
	#endif
	gtk_widget_set_sensitive
		(GTK_WIDGET(editor->button_remove), editor->channel->cg->n > 0);
	gtk_widget_set_sensitive
		(GTK_WIDGET(editor->button_remove_inner), editor->channel->n > 1);
	gtk_widget_set_sensitive
		(GTK_WIDGET(editor->editor_inner->grid), editor->channel->n > 1);
	#if DEBUG_EDITOR_CHANNEL_UPDATE
		fprintf(stderr, "editor_channel_update: end\n");
	#endif
}

/**
 * \fn int editor_channel_get_section(EditorChannel *editor)
 * \brief Function to get the actual cross section data from the editor.
 * \param editor
 * \brief channel editor.
 * \return 1 on valid cross section, 0 on error.
 */
int editor_channel_get_section(EditorChannel *editor)
{
	int i;
	CrossSection *cs;
	#if DEBUG_EDITOR_CHANNEL_GET_SECTION
		fprintf(stderr, "editor_channel_get_section: start\n");
	#endif
	i = gtk_combo_box_get_active(GTK_COMBO_BOX(editor->combo_section));
	cs = editor->channel->cg->cs + i;
	if (!editor_cross_section_get(editor->editor_section))
	{
		#if DEBUG_EDITOR_CHANNEL_GET_SECTION
			fprintf(stderr, "editor_channel_get_section: end\n");
		#endif
		return 0;
	}
	cross_section_delete(cs);
	cross_section_copy(cs, editor->editor_section->cs);
	#if DEBUG_EDITOR_CHANNEL_GET_SECTION
		fprintf(stderr, "editor_channel_get_section: end\n");
	#endif
	return 1;
}

/**
 * \fn int editor_channel_get_inlet(EditorChannel *editor)
 * \brief Function to get the actual inlet flow boundary condtion data from the
 *   editor.
 * \param editor
 * \brief channel editor.
 * \return 1 on valid boundary condition, 0 on error.
 */
int editor_channel_get_inlet(EditorChannel *editor)
{
	BoundaryFlow *bf;
	#if DEBUG_EDITOR_CHANNEL_GET_INLET
		fprintf(stderr, "editor_channel_get_inlet: start\n");
	#endif
	bf = editor->channel->bf;
	if (!editor_boundary_flow_get(editor->editor_inlet))
	{
		#if DEBUG_EDITOR_CHANNEL_GET_INLET
			fprintf(stderr, "editor_channel_get_inlet: end\n");
		#endif
		return 0;
	}
	boundary_flow_delete(bf);
	boundary_flow_copy(bf, editor->editor_inlet->bf);
	#if DEBUG_EDITOR_CHANNEL_GET_INLET
		fprintf(stderr, "editor_channel_get_inlet: end\n");
	#endif
	return 1;
}

/**
 * \fn int editor_channel_get_outlet(EditorChannel *editor)
 * \brief Function to get the actual outlet flow boundary condtion data from the
 *   editor.
 * \param editor
 * \brief channel editor.
 * \return 1 on valid boundary condition, 0 on error.
 */
int editor_channel_get_outlet(EditorChannel *editor)
{
	BoundaryFlow *bf;
	Channel *channel;
	#if DEBUG_EDITOR_CHANNEL_GET_OUTLET
		fprintf(stderr, "editor_channel_get_outlet: start\n");
	#endif
	channel = editor->channel;
	bf = channel->bf + channel->n;
	if (!editor_boundary_flow_get(editor->editor_outlet))
	{
		#if DEBUG_EDITOR_CHANNEL_GET_OUTLET
			fprintf(stderr, "editor_channel_get_outlet: end\n");
		#endif
		return 0;
	}
	boundary_flow_delete(bf);
	boundary_flow_copy(bf, editor->editor_outlet->bf);
	#if DEBUG_EDITOR_CHANNEL_GET_OUTLET
		fprintf(stderr, "editor_channel_get_outlet: end\n");
	#endif
	return 1;
}

/**
 * \fn int editor_channel_get_inner(EditorChannel *editor)
 * \brief Function to get the actual inner flow boundary condtion data from the
 *   editor.
 * \param editor
 * \brief channel editor.
 * \return 1 on valid boundary condition, 0 on error.
 */
int editor_channel_get_inner(EditorChannel *editor)
{
	int i;
	BoundaryFlow *bf;
	#if DEBUG_EDITOR_CHANNEL_GET_INNER
		fprintf(stderr, "editor_channel_get_inner: start\n");
	#endif
	i = gtk_combo_box_get_active(GTK_COMBO_BOX(editor->combo_inner));
	bf = editor->channel->bf + i + 1;
	if (!editor_boundary_flow_get(editor->editor_inner))
	{
		#if DEBUG_EDITOR_CHANNEL_GET_INNER
			fprintf(stderr, "editor_channel_get_inner: end\n");
		#endif
		return 0;
	}
	boundary_flow_delete(bf);
	boundary_flow_copy(bf, editor->editor_inner->bf);
	#if DEBUG_EDITOR_CHANNEL_GET_INNER
		fprintf(stderr, "editor_channel_get_inner: end\n");
	#endif
	return 1;
}

/**
 * \fn int editor_channel_get(EditorChannel *editor)
 * \brief Function to get the actual channel data from the editor.
 * \param editor
 * \brief channel editor.
 * \return 1 on valid channel, 0 on error.
 */
int editor_channel_get(EditorChannel *editor)
{
	Channel *channel = editor->channel;
	#if DEBUG_EDITOR_CHANNEL_GET
		fprintf(stderr, "editor_channel_get: start\n");
	#endif
	g_free(channel->name);
	channel->name = g_strdup(gtk_entry_get_text(editor->entry_name));
	if (!channel->name)
	{
		channel_error(channel, gettext("Not enough memory"));
		#if DEBUG_EDITOR_CHANNEL_GET
			fprintf(stderr, "editor_channel_get: end\n");
		#endif
		return 0;
	}
	jbw_graphic_set_title(editor->graphic, channel->name);
	#if DEBUG_EDITOR_CHANNEL_GET
		fprintf(stderr, "editor_channel_get: end\n");
	#endif
	return 1;
}

/**
 * \fn void editor_channel_open_section(EditorChannel *editor)
 * \brief Function to open a cross section in the editor.
 * \param editor
 * \brief channel editor.
 */
void editor_channel_open_section(EditorChannel *editor)
{
	int i;
	#if DEBUG_EDITOR_CHANNEL_OPEN_SECTION
		fprintf(stderr, "editor_channel_open_section: start\n");
	#endif
	i = gtk_combo_box_get_active(GTK_COMBO_BOX(editor->combo_section));
	cross_section_delete(editor->editor_section->cs);
	cross_section_copy(editor->editor_section->cs, editor->channel->cg->cs + i);
	editor_cross_section_open(editor->editor_section);
	#if DEBUG_EDITOR_CHANNEL_OPEN_SECTION
		fprintf(stderr, "editor_channel_open_section: end\n");
	#endif
}

/**
 * \fn void editor_channel_open_inlet(EditorChannel *editor)
 * \brief Function to open an inlet flow boundary condition in the editor.
 * \param editor
 * \brief channel editor.
 */
void editor_channel_open_inlet(EditorChannel *editor)
{
	#if DEBUG_EDITOR_CHANNEL_OPEN_INLET
		fprintf(stderr, "editor_channel_open_inlet: start\n");
	#endif
	boundary_flow_delete(editor->editor_inlet->bf);
	boundary_flow_copy(editor->editor_inlet->bf, editor->channel->bf);
	editor_boundary_flow_open(editor->editor_inlet);
	#if DEBUG_EDITOR_CHANNEL_OPEN_INLET
		fprintf(stderr, "editor_channel_open_inlet: end\n");
	#endif
}

/**
 * \fn void editor_channel_open_outlet(EditorChannel *editor)
 * \brief Function to open an outlet flow boundary condition in the editor.
 * \param editor
 * \brief channel editor.
 */
void editor_channel_open_outlet(EditorChannel *editor)
{
	#if DEBUG_EDITOR_CHANNEL_OPEN_OUTLET
		fprintf(stderr, "editor_channel_open_outlet: start\n");
	#endif
	boundary_flow_delete(editor->editor_outlet->bf);
	boundary_flow_copy
		(editor->editor_outlet->bf, editor->channel->bf + editor->channel->n);
	editor_boundary_flow_open(editor->editor_outlet);
	#if DEBUG_EDITOR_CHANNEL_OPEN_OUTLET
		fprintf(stderr, "editor_channel_open_outlet: end\n");
	#endif
}

/**
 * \fn void editor_channel_open_inner(EditorChannel *editor)
 * \brief Function to open an inner flow boundary condition in the editor.
 * \param editor
 * \brief channel editor.
 */
void editor_channel_open_inner(EditorChannel *editor)
{
	int i;
	#if DEBUG_EDITOR_CHANNEL_OPEN_INNER
		fprintf(stderr, "editor_channel_open_inner: start\n");
	#endif
	i = gtk_combo_box_get_active(GTK_COMBO_BOX(editor->combo_inner));
	boundary_flow_delete(editor->editor_inner->bf);
	boundary_flow_copy(editor->editor_inner->bf, editor->channel->bf + i + 1);
	editor_boundary_flow_open(editor->editor_inner);
	#if DEBUG_EDITOR_CHANNEL_OPEN_INNER
		fprintf(stderr, "editor_channel_open_inner: end\n");
	#endif
}

/**
 * \fn void editor_channel_open(EditorChannel *editor)
 * \brief Function to open a channel in the editor.
 * \param editor
 * \brief channel editor.
 */
void editor_channel_open(EditorChannel *editor)
{
	int i;
	Channel *channel;
	ChannelGeometry *cg;
	InitialFlow *ifc;
	EditorInitialFlow *editor_initial;
	EditorBoundaryFlow *editor_boundary;
	#if DEBUG_EDITOR_CHANNEL_OPEN
		fprintf(stderr, "editor_channel_open: start\n");
		channel_print(editor->channel, stderr);
	#endif
	channel = editor->channel;
	g_signal_handler_block(editor->combo_section, editor->id_section);
	gtk_combo_box_text_remove_all(editor->combo_section);
	cg = channel->cg;
	for (i = 0; i <= cg->n; ++i)
		gtk_combo_box_text_append_text(editor->combo_section, cg->cs[i].name);
	g_signal_handler_unblock(editor->combo_section, editor->id_section);
	gtk_combo_box_set_active(GTK_COMBO_BOX(editor->combo_section), 0);
	g_signal_handler_block(editor->combo_inner, editor->id_inner);
	gtk_combo_box_text_remove_all(editor->combo_inner);
	for (i = 1; i < channel->n; ++i)
		gtk_combo_box_text_append_text
			(editor->combo_inner, channel->bf[i].name);
	g_signal_handler_unblock(editor->combo_inner, editor->id_inner);
	gtk_combo_box_set_active(GTK_COMBO_BOX(editor->combo_inner), 0);
	gtk_entry_set_text(editor->entry_name, channel->name);
	ifc = channel->ifc;
	editor_initial = editor->editor_initial;
	initial_flow_delete(editor_initial->ifc);
	initial_flow_copy(editor_initial->ifc, ifc);
	editor_initial_flow_open(editor_initial);
	editor_boundary = editor->editor_inlet;
	boundary_flow_delete(editor_boundary->bf);
	boundary_flow_copy(editor_boundary->bf, channel->bf);
	editor_boundary_flow_open(editor_boundary);
	editor_boundary = editor->editor_outlet;
	boundary_flow_delete(editor_boundary->bf);
	boundary_flow_copy(editor_boundary->bf, channel->bf + channel->n);
	editor_boundary_flow_open(editor_boundary);
	editor_channel_update(editor);
	#if DEBUG_EDITOR_CHANNEL_OPEN
		fprintf(stderr, "editor_channel_open: end\n");
	#endif
}

/**
 * \fn void editor_channel_insert_section(EditorChannel *editor)
 * \brief Function to insert a cross section in the actual channel data of the
 *   editor.
 * \param editor
 * \brief channel editor.
 */
void editor_channel_insert_section(EditorChannel *editor)
{
	int i;
	Channel *channel;
	ChannelGeometry *cg;
	CrossSection *cs;
	EditorCrossSection *editor_section;
	#if DEBUG_EDITOR_CHANNEL_INSERT_SECTION
		fprintf(stderr, "editor_channel_insert_section: start\n");
	#endif
	channel = editor->channel;
	cg = channel->cg;
	i = 1 + gtk_combo_box_get_active(GTK_COMBO_BOX(editor->combo_section));
	channel_geometry_insert_section(cg, i);
	cs = cg->cs + i;
	gtk_combo_box_text_insert_text(editor->combo_section, i, cs->name);
	editor_channel_update(editor);
	editor_section = editor->editor_section;
	cross_section_delete(editor_section->cs);
	cross_section_copy(editor_section->cs, cs);
	editor_cross_section_open(editor_section);
	gtk_combo_box_set_active(GTK_COMBO_BOX(editor->combo_section), i);
	gtk_notebook_set_current_page(editor->notebook, EDITOR_CROSS_SECTION);
	#if DEBUG_EDITOR_CHANNEL_INSERT_SECTION
		fprintf(stderr, "editor_channel_insert_section: end\n");
	#endif
}

/**
 * \fn void editor_channel_remove_section(EditorChannel *editor)
 * \brief Function to remove a cross section in the actual channel data
 *   of the editor.
 * \param editor
 * \brief channel editor.
 */
void editor_channel_remove_section(EditorChannel *editor)
{
	int i;
	ChannelGeometry *cg;
	#if DEBUG_EDITOR_CHANNEL_REMOVE_SECTION
		fprintf(stderr, "editor_channel_remove_section: start\n");
	#endif
	cg = editor->channel->cg;
	i = gtk_combo_box_get_active(GTK_COMBO_BOX(editor->combo_section));
	channel_geometry_remove_section(cg, i);
	g_signal_handler_block(editor->combo_section, editor->id_section);
	gtk_combo_box_text_remove(editor->combo_section, i);
	g_signal_handler_unblock(editor->combo_section, editor->id_section);
	gtk_combo_box_set_active
		(GTK_COMBO_BOX(editor->combo_section), jbm_min(i, cg->n));
	editor_channel_update(editor);
	#if DEBUG_EDITOR_CHANNEL_REMOVE_SECTION
		fprintf(stderr, "editor_channel_remove_section: end\n");
	#endif
}

/**
 * \fn void editor_channel_insert_inner(EditorChannel *editor)
 * \brief Function to insert an inner flow boundary condition in the actual
 *   channel data of the editor.
 * \param editor
 * \brief channel editor.
 */
void editor_channel_insert_inner(EditorChannel *editor)
{
	int i;
	Channel *channel;
	BoundaryFlow *bf;
	EditorBoundaryFlow *editor_inner;
	#if DEBUG_EDITOR_CHANNEL_INSERT_INNER
		fprintf(stderr, "editor_channel_insert_inner: start\n");
	#endif
	channel = editor->channel;
	i = 2 + gtk_combo_box_get_active(GTK_COMBO_BOX(editor->combo_inner));
	channel_insert_bf(channel, i);
	bf = channel->bf + i;
	gtk_combo_box_text_insert_text(editor->combo_inner, i - 1, bf->name);
	editor_channel_update(editor);
	editor_inner = editor->editor_inner;
	boundary_flow_delete(editor_inner->bf);
	boundary_flow_copy(editor_inner->bf, bf);
	editor_boundary_flow_open(editor_inner);
	gtk_combo_box_set_active(GTK_COMBO_BOX(editor->combo_inner), i - 1);
	gtk_notebook_set_current_page(editor->notebook, EDITOR_INNER);
	#if DEBUG_EDITOR_CHANNEL_INSERT_INNER
		fprintf(stderr, "editor_channel_insert_inner: end\n");
	#endif
}

/**
 * \fn void editor_channel_remove_inner(EditorChannel *editor)
 * \brief Function to remove an inner flow boundary condition in the actual
 *   channel data of the editor.
 * \param editor
 * \brief channel editor.
 */
void editor_channel_remove_inner(EditorChannel *editor)
{
	int i;
	Channel *channel;
	#if DEBUG_EDITOR_CHANNEL_REMOVE_INNER
		fprintf(stderr, "editor_channel_remove_inner: start\n");
	#endif
	channel = editor->channel;
	i = gtk_combo_box_get_active(GTK_COMBO_BOX(editor->combo_inner));
	channel_remove_bf(channel, i + 1);
	g_signal_handler_block(editor->combo_inner, editor->id_inner);
	gtk_combo_box_text_remove(editor->combo_inner, i);
	g_signal_handler_unblock(editor->combo_inner, editor->id_inner);
	gtk_combo_box_set_active
		(GTK_COMBO_BOX(editor->combo_inner), jbm_min(i, channel->n));
	editor_channel_update(editor);
	#if DEBUG_EDITOR_CHANNEL_REMOVE_INNER
		fprintf(stderr, "editor_channel_remove_inner: end\n");
	#endif
}

/**
 * \fn void editor_channel_draw(EditorChannel *editor)
 * \brief Function to draw a channel in a channel editor.
 * \param editor
 * \brief channel editor.
 */
void editor_channel_draw(EditorChannel *editor)
{
	#if DEBUG_EDITOR_CHANNEL_DRAW
		fprintf(stderr,"editor_channel_draw: start\n");
	#endif

	editor_channel_get(editor);
	int i, j, n = editor->channel->cg->n;
	TransientSection *ts;
	CrossSection *cs;
	JBDOUBLE k1, k2, xmax, xmin;
	JBWGraphic *graphic;
	JBFLOAT x[n + n + 2], y[n + n + 2], x2[n + 1], y2[n + 1], *xx, *yy;

	graphic = editor->graphic;
	for (i = 0, cs = editor->channel->cg->cs, xx = x, yy = y; i <= n;
		++i, ++cs, ++xx, ++yy)
	{
		ts = cs->ts;
		jbm_varray_maxmin
			(&(ts->sp->y), sizeof(SectionPoint2), ts->n, &xmax, &xmin);
		for (j = 0; ++j <= cs->n;)
		{
			++ts;
			jbm_varray_maxmin
				(&(ts->sp->y), sizeof(SectionPoint2), ts->n, &k2, &k1);
			xmax = fmaxl(k2, xmax);
			xmin = fminl(k1, xmin);
		}
		x2[i] = cs->x;
		*xx = cs->x + xmax * cos(cs->angle * M_PI / 180.);
		y2[i] = cs->y;
		*yy = cs->y + xmax * sin(cs->angle * M_PI / 180.);
		++xx, ++yy;
		*xx = cs->x + xmin * cos(cs->angle * M_PI / 180.);
		*yy = cs->y + xmin * sin(cs->angle * M_PI / 180.);
	}
	jbm_farray_maxmin(x, n + n + 1, &graphic->xmax, &graphic->xmin);
	jbm_farray_maxmin(y, n + n + 1, &graphic->ymax, &graphic->ymin);
	#if DEBUG_EDITOR_CHANNEL_DRAW
		fprintf(stderr, "ECGD: xmin="FWL" xmax="FWL" ymin="FWL" ymax="FWL"\n",
			graphic->xmin, graphic->xmax, graphic->ymin, graphic->ymax);
	#endif
	jbw_graphic_labels(graphic);
	#if DEBUG_EDITOR_CHANNEL_DRAW
		fprintf(stderr, "ECGD: xmin="FWL" xmax="FWL" ymin="FWL" ymax="FWL"\n",
			graphic->xmin, graphic->xmax, graphic->ymin, graphic->ymax);
	#endif
#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
	glLoadIdentity();
	glOrtho
		(graphic->xmin, graphic->xmax, graphic->ymin, graphic->ymax, -1., 1.);
	#if JB_PRECISION < 3
		jbw_draw_segmentsf(0., 0., 1., x, y, n + n + 2);
		jbw_draw_linesf(1., 0., 0., x2, y2, n + 1);
	#else
		jbw_draw_segmentsd(0., 0., 1., x, y, n + n + 2);
		jbw_draw_linesd(1., 0., 0., x2, y2, n + 1);
	#endif
#else
//	jbw_draw_segments(graphic->cr, 0., 0., 1., x, y, n + n + 2);
	jbw_graphic_draw_lines(graphic, x2, y2, NULL, NULL, NULL, n + 1);
#endif

	// Drawing logo
	jbw_graphic_draw_logo(graphic);

	// Displaying
#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
	glutSwapBuffers();
#elif JBW_GRAPHIC == JBW_GRAPHIC_CLUTTER
	glFlush();
#endif

	// End
	#if DEBUG_EDITOR_CHANNEL_DRAW
		fprintf(stderr,"editor_channel_draw: end\n");
	#endif
}

/**
 * \fn int editor_channel_check(EditorChannel *editor)
 * \brief Function to check if the channel is correct.
 * \param editor
 * \brief channel editor.
 * \return 1 on valid channel, 0 on error.
 */
int editor_channel_check(EditorChannel *editor)
{
	Channel *channel;
	#if DEBUG_EDITOR_CHANNEL_CHECK
		fprintf(stderr,"editor_channel_check: start\n");
	#endif
	if (!editor_channel_get(editor)) goto error_check;
	channel = editor->channel;
	if (!editor_cross_section_check(editor->editor_section)
		|| !editor_initial_flow_check(editor->editor_initial)
		|| !channel_flow_boundaries_set_up(channel)
		|| !channel_transport_boundaries_set_up(channel))
	{
		channel_error(channel, message);
		goto error_check;
	}
	#if DEBUG_EDITOR_CHANNEL_CHECK
		fprintf(stderr,"editor_channel_check: end\n");
	#endif
	return 1;

error_check:
	#if DEBUG_EDITOR_CHANNEL_CHECK
		fprintf(stderr,"editor_channel_check: end\n");
	#endif
	return 0;
}

/**
 * \fn void editor_channel_destroy(EditorChannel *editor)
 * \brief Function to destroy a channel editor.
 * \param editor
 * \brief channel editor.
 */
void editor_channel_destroy(EditorChannel *editor)
{
	#if DEBUG_EDITOR_CHANNEL_DESTROY
		fprintf(stderr, "editor_channel_destroy: start\n");
	#endif
	editor_cross_section_destroy(editor->editor_section);
	editor_initial_flow_destroy(editor->editor_initial);
	editor_boundary_flow_destroy(editor->editor_inlet);
	editor_boundary_flow_destroy(editor->editor_outlet);
	editor_boundary_flow_destroy(editor->editor_inner);
	gtk_widget_destroy(GTK_WIDGET(editor->grid));
	channel_delete(editor->channel);
	#if DEBUG_EDITOR_CHANNEL_DESTROY
		fprintf(stderr, "editor_channel_destroy: end\n");
	#endif
}

/**
 * \fn void editor_channel_new(EditorChannel *editor, GtkNotebook *notebook, \
 *   char **channel_name, int nchannels, char ***section_name, int *nsections)
 * \brief Function to create a new channel editor.
 * \param editor
 * \param notebook
 * \brief GtkNotebook to pack the widgets.
 * \param channel_name
 * \brief array of channel names.
 * \param nchannels
 * \brief number of channels.
 * \param section_name
 * \brief array of arrays of cross section names.
 * \param nsections
 * \brief array of number of cross sections.
 */
void editor_channel_new(EditorChannel *editor,
	GtkNotebook *notebook, char **channel_name, int nchannels,
	char ***section_name, int *nsections)
{
	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr, "editor_channel_new: start\n");
	#endif
	channel_init_empty(editor->channel);
	editor->notebook = notebook;
	editor->grid = (GtkGrid*)gtk_grid_new();
	gtk_notebook_append_page(notebook, GTK_WIDGET(editor->grid),
		gtk_label_new(gettext("Channel")));
	editor_cross_section_new(editor->editor_section, notebook, channel_name,
		nchannels, section_name, nsections);
	editor_initial_flow_new(editor->editor_initial, notebook);
	editor_boundary_flow_new(editor->editor_inlet, notebook, -1, 0,
		channel_name, nchannels, section_name, nsections);
	editor_boundary_flow_new(editor->editor_outlet, notebook, 1, 0,
		channel_name, nchannels, section_name, nsections);
	editor_boundary_flow_new(editor->editor_inner, notebook, 0, 0,
		channel_name, nchannels, section_name, nsections);
	editor->label_name = (GtkLabel*)gtk_label_new(gettext("Name"));
	gtk_grid_attach(editor->grid, GTK_WIDGET(editor->label_name), 0, 0, 1, 1);
	editor->entry_name = (GtkEntry*)gtk_entry_new();
	gtk_grid_attach(editor->grid, GTK_WIDGET(editor->entry_name), 1, 0, 2, 1);
	editor->button_insert = (GtkButton*)gtk_button_new_with_label
		(gettext("Insert cross section"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->button_insert), 0, 1, 1, 1);
	g_signal_connect_swapped(editor->button_insert, "clicked",
		(void(*))&editor_channel_insert_section, editor);
	editor->button_remove = (GtkButton*)gtk_button_new_with_label
		(gettext("Remove cross section"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->button_remove), 1, 1, 1, 1);
	g_signal_connect_swapped(editor->button_remove, "clicked",
		(void(*))&editor_channel_remove_section, editor);
	editor->button_plot
		= (GtkButton*)gtk_button_new_with_label(gettext("Update plot"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->button_plot), 2, 1, 1, 1);
	g_signal_connect(editor->button_plot, "clicked", &editor_draw, NULL);
	editor->label_section
		= (GtkLabel*)gtk_label_new(gettext("Cross section"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->label_section), 0, 2, 1, 1);
	editor->combo_section = (GtkComboBoxText*)gtk_combo_box_text_new();
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->combo_section), 1, 2, 2, 1);
	editor->id_section = g_signal_connect_swapped(editor->combo_section,
		"changed", (void(*))&editor_channel_open_section, editor);
	editor->button_insert_inner = (GtkButton*)gtk_button_new_with_label
		(gettext("Insert boundary condition"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->button_insert_inner), 0, 3, 1, 1);
	g_signal_connect_swapped(editor->button_insert_inner, "clicked",
		(void(*))&editor_channel_insert_inner, editor);
	editor->button_remove_inner = (GtkButton*)gtk_button_new_with_label
		(gettext("Remove boundary condition"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->button_remove_inner), 1, 3, 1, 1);
	g_signal_connect_swapped(editor->button_remove_inner, "clicked",
		(void(*))&editor_channel_remove_inner, editor);
	editor->label_inner
		= (GtkLabel*)gtk_label_new(gettext("Boundary condition"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->label_inner), 0, 4, 1, 1);
	editor->combo_inner = (GtkComboBoxText*)gtk_combo_box_text_new();
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->combo_inner), 1, 4, 2, 1);
	editor->id_inner = g_signal_connect_swapped(editor->combo_inner,
		"changed", (void(*))&editor_channel_open_inner, editor);
	editor->graphic = jbw_graphic_new(NULL, 6, 6, 0, &editor_draw);
	jbw_graphic_set_logo(editor->graphic, "swigs.png");
	jbw_graphic_set_xlabel(editor->graphic, "x (m)");
	jbw_graphic_set_ylabel(editor->graphic, "y (m)");
	editor->graphic->map = 1;
	#if DEBUG_EDITOR_CHANNEL_NEW
		fprintf(stderr, "editor_channel_new: end\n");
	#endif
}

#if TEST_EDITOR_CHANNEL

void editor_draw()
{
	switch (gtk_notebook_get_current_page(editor->notebook))
	{
		case EDITOR_CHANNEL:
			editor_channel_draw(editor);
			break;
		case EDITOR_CROSS_SECTION:
			editor_cross_section_draw(editor->editor_section);
			break;
		case EDITOR_TRANSIENT_SECTION:
			editor_transient_section_draw
				(editor->editor_section->editor_transient);
			break;
		case EDITOR_INITIAL_FLOW:
			editor_initial_flow_draw(editor->editor_initial);
			break;
		case EDITOR_INLET:
			editor_boundary_flow_draw(editor->editor_inlet);
			break;
		case EDITOR_OUTLET:
			editor_boundary_flow_draw(editor->editor_outlet);
			break;
		case EDITOR_INNER:
			editor_boundary_flow_draw(editor->editor_inner);
			break;
	}
}

void change_current_page(EditorChannel *editor)
{
	int is, it, ib;
	CrossSection *cs;
	TransientSection *ts;
	InitialFlow *ifc;
	BoundaryFlow *bf;
	EditorCrossSection *editor_section;
	EditorTransientSection *editor_transient;
	EditorInitialFlow *editor_initial;
	EditorBoundaryFlow *editor_boundary;
	editor_section = editor->editor_section;
	editor_boundary = editor->editor_inner;
	is = gtk_combo_box_get_active(GTK_COMBO_BOX(editor->combo_section));
	it = gtk_combo_box_get_active
		(GTK_COMBO_BOX(editor_section->combo_transient));
	ib = gtk_combo_box_get_active(GTK_COMBO_BOX(editor->combo_inner));
printf("ib=%d\n", ib);
	switch (gtk_notebook_get_current_page(editor->notebook))
	{
		case EDITOR_CROSS_SECTION:
			if (!editor_cross_section_get(editor_section))
			{
				jbw_show_error(message);
				break;
			}
			cs = editor->channel->cg->cs + is;
			cross_section_delete(cs);
			cross_section_copy(cs, editor_section->cs);
			editor_channel_open(editor);
			break;
		case EDITOR_TRANSIENT_SECTION:
			editor_transient = editor_section->editor_transient;
			if (!editor_transient_section_get(editor_transient))
			{
				jbw_show_error(message);
				break;
			}
			ts = editor_section->cs->ts + it;
			transient_section_delete(ts);
			transient_section_copy(ts, editor_transient->ts);
			editor_cross_section_get(editor_section);
			cs = editor->channel->cg->cs + gtk_combo_box_get_active
				(GTK_COMBO_BOX(editor->combo_section));
			cross_section_delete(cs);
			cross_section_copy(cs, editor_section->cs);
			editor_cross_section_open(editor_section);
			break;
		case EDITOR_INITIAL_FLOW:
			editor_initial = editor->editor_initial;
			editor_initial_flow_get(editor_initial);
			ifc = editor->channel->ifc;
			initial_flow_delete(ifc);
			initial_flow_copy(ifc, editor_initial->ifc);
			editor_channel_open(editor);
			break;
		case EDITOR_INLET:
			editor_boundary = editor->editor_inlet;
			if (!editor_boundary_flow_get(editor_boundary))
			{
				jbw_show_error(message);
				break;
			}
			bf = editor->channel->bf;
			boundary_flow_delete(bf);
			boundary_flow_copy(bf, editor_boundary->bf);
			editor_channel_open(editor);
			break;
		case EDITOR_OUTLET:
			editor_boundary = editor->editor_outlet;
			if (!editor_boundary_flow_get(editor_boundary))
			{
				jbw_show_error(message);
				break;
			}
			bf = editor->channel->bf + editor->channel->n;
			boundary_flow_delete(bf);
			boundary_flow_copy(bf, editor_boundary->bf);
			editor_channel_open(editor);
			break;
		case EDITOR_INNER:
			if (editor->channel->n < 2) break;
			if (!editor_boundary_flow_get(editor_boundary))
			{
				jbw_show_error(message);
				break;
			}
			bf = editor->channel->bf + ib + 1;
			boundary_flow_delete(bf);
			boundary_flow_copy(bf, editor_boundary->bf);
			editor_channel_open(editor);
			break;
	}
	gtk_combo_box_set_active(GTK_COMBO_BOX(editor->combo_section), is);
	gtk_combo_box_set_active
		(GTK_COMBO_BOX(editor_section->combo_transient), it);
printf("ib=%d\n", ib);
	gtk_combo_box_set_active(GTK_COMBO_BOX(editor->combo_inner), ib);
}

void ok(char *name)
{
	xmlNode *node;
	xmlDoc *doc;
	if (!editor_channel_check(editor))
	{
		jbw_show_error(message);
		return;
	}
	doc = xmlNewDoc((const xmlChar*)"1.0");
	node = xmlNewDocNode(doc, 0, XML_CHANNEL, 0);
	xmlDocSetRootElement(doc, node);
	channel_save_xml(editor->channel, node);
	xmlSaveFormatFile(name, doc, 1);
	xmlFree(doc);
	glutLeaveMainLoop();
}

int main(int argn, char **argc)
{
	int nsections[2] = {3, 2};
	char *channel_name[2] = {"Channel1", "Channel2"};
	char *section_name1[3] = {"Section1", "Section2", "Section3"},
		*section_name2[2] = {"SectionA", "SectionB"};
	char **section_name[2] = {section_name1, section_name2};
	xmlNode *node;
	xmlDoc *doc;
	GtkNotebook *notebook;
	GtkButton *button_ok, *button_cancel;
	GtkDialog *dlg;
	xmlKeepBlanksDefault(0);
	if (!jbw_graphic_init(&argn, &argc)) return 1;
	glutIdleFunc((void(*))&gtk_main_iteration);
	notebook = (GtkNotebook*)gtk_notebook_new();
	gtk_notebook_set_tab_pos(notebook, GTK_POS_LEFT);
	g_signal_connect_swapped(notebook, "switch-page",
		(void(*))&change_current_page, editor);
	g_signal_connect_after(notebook, "switch-page", &editor_draw, NULL);
	editor_channel_new
		(editor, notebook, channel_name, 2, section_name, nsections);
	doc = xmlParseFile(argc[1]);
	if (!doc) return 2;
	node = xmlDocGetRootElement(doc);
	if (!channel_open_xml(editor->channel, -1, node)) return 3;
	xmlFreeDoc(doc);
	dlg = (GtkDialog*)gtk_dialog_new();
	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(dlg)),
		GTK_WIDGET(notebook));
	gtk_window_set_title(GTK_WINDOW(dlg), "Test editor channel");
	button_ok = (GtkButton*)gtk_dialog_add_button
		(dlg, gettext("_OK"), GTK_RESPONSE_OK);
	g_signal_connect_swapped(button_ok, "clicked", (void(*))&ok, argc[2]);
	button_cancel = (GtkButton*)gtk_dialog_add_button
		(dlg, gettext("_Cancel"), GTK_RESPONSE_CANCEL);
	g_signal_connect(button_cancel, "clicked", &glutLeaveMainLoop, NULL);
	editor_channel_open(editor);
	gtk_widget_show_all(GTK_WIDGET(dlg));
	glutMainLoop();
	editor_channel_destroy(editor);
	gtk_widget_destroy(GTK_WIDGET(dlg));
	return 0;
}

#endif
