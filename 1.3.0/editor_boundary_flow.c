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
 * \file editor_boundary_flow.c
 * \brief Source file to define flow boundary conditions editor
 *   structures and functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2014 Javier Burguete Tolosa.
 */
#include "editor_boundary_flow.h"

#if TEST_EDITOR_BOUNDARY_FLOW
	char *message;
	EditorBoundaryFlow editor[1];
	void editor_draw() {editor_boundary_flow_draw(editor);}
#endif

/**
 * \fn void editor_junction_update(EditorJunction *editor)
 * \brief Function to update the view of a junction editor.
 * \param editor
 * \brief editor parameters of a junction.
 */
void editor_junction_update(EditorJunction *editor)
{
	int i;
	#if DEBUG_EDITOR_JUNCTION_UPDATE
		fprintf(stderr, "editor_junction_update: start\n");
	#endif
	i = gtk_combo_box_get_active(GTK_COMBO_BOX(editor->combo_channel));
	jbw_combo_box_set_strings(editor->combo_initial,
		editor->section_name[i], editor->nsections[i]);
	gtk_combo_box_set_active(GTK_COMBO_BOX(editor->combo_initial), 0);
	jbw_combo_box_set_strings(editor->combo_final,
		editor->section_name[i], editor->nsections[i]);
	gtk_combo_box_set_active(GTK_COMBO_BOX(editor->combo_final), 0);
	#if DEBUG_EDITOR_JUNCTION_UPDATE
		fprintf(stderr, "editor_junction_update: end\n");
	#endif
}

/**
 * \fn void editor_junction_update_channels(EditorJunction *editor, \
 *   char **channel_name, int nchannels)
 * \brief Function to update the channel names in a junction editor.
 * \param editor
 * \brief editor parameters of a junction.
 * \param channel_name
 * \brief new array of channel names.
 * \param nchannels
 * \brief new number of channels.
 */
void editor_junction_update_channels
	(EditorJunction *editor, char **channel_name, int nchannels)
{
	#if DEBUG_EDITOR_JUNCTION_UPDATE_CHANNELS
		fprintf(stderr, "editor_junction_update_channels: start\n");
	#endif
	editor->channel_name = channel_name;
	editor->nchannels = nchannels;
	editor_junction_update(editor);
	#if DEBUG_EDITOR_JUNCTION_UPDATE_CHANNELS
		fprintf(stderr, "editor_junction_update_channels: end\n");
	#endif
}

/**
 * \fn void editor_junction_update_sections(EditorJunction *editor, \
 *   int channel, char **section_name, int nsections)
 * \brief Function to update the cross section names of a channel in a junction
 *   editor.
 * \param editor
 * \brief editor parameters of a junction.
 * \param channel
 * \brief number of channel.
 * \param section_name
 * \brief new array of cross section names.
 * \param nsections
 * \brief new number of cross sections.
 */
void editor_junction_update_sections
	(EditorJunction *editor, int channel, char **section_name, int nsections)
{
	#if DEBUG_EDITOR_JUNCTION_UPDATE_SECTIONS
		fprintf(stderr, "editor_junction_update_sections: start\n");
	#endif
	editor->section_name[channel] = section_name;
	editor->nsections[channel] = nsections;
	editor_junction_update(editor);
	#if DEBUG_EDITOR_JUNCTION_UPDATE_SECTIONS
		fprintf(stderr, "editor_junction_update_sections: end\n");
	#endif
}

/**
 * \fn int editor_junction_get(EditorJunction *editor)
 * \brief Function to get the actual junction data from the editor.
 * \param editor
 * \brief junction editor.
 * \return 1 on valid junction, 0 on error.
 */
int editor_junction_get(EditorJunction *editor)
{
	JunctionData *jd;
	#if DEBUG_EDITOR_JUNCTION_GET
		fprintf(stderr, "editor_junction_get: start\n");
	#endif
	jd = editor->jd;
	junction_data_delete(jd);
	jd->channel
		= gtk_combo_box_get_active (GTK_COMBO_BOX(editor->combo_channel));
	jd->pos = gtk_combo_box_get_active(GTK_COMBO_BOX(editor->combo_initial));
	jd->pos2 = gtk_combo_box_get_active(GTK_COMBO_BOX(editor->combo_final));
	if (jd->pos2 < jd->pos)
	{
		junction_data_error(gettext("Bad order"));
		#if DEBUG_EDITOR_JUNCTION_GET
			fprintf(stderr, "editor_junction_get: end\n");
		#endif
		return 0;
	}
	jd->channel_name
		= gtk_combo_box_text_get_active_text(editor->combo_channel);
	jd->section = gtk_combo_box_text_get_active_text(editor->combo_initial);
	jd->section2 = gtk_combo_box_text_get_active_text(editor->combo_final);
	#if DEBUG_EDITOR_JUNCTION_GET
		junction_data_print(jd, stderr);
		fprintf(stderr, "editor_junction_get: end\n");
	#endif
	return 1;
}

/**
 * \fn void editor_junction_open(EditorJunction *editor)
 * \brief Function to open a junction in the editor.
 * \param editor
 * \brief junction editor.
 */
void editor_junction_open(EditorJunction *editor)
{
	JunctionData *jd;
	#if DEBUG_EDITOR_JUNCTION_OPEN
		fprintf(stderr, "editor_junction_open: start\n");
		junction_data_print(editor->jd, stderr);
	#endif
	jd = editor->jd;
	gtk_combo_box_set_active(GTK_COMBO_BOX(editor->combo_channel), jd->channel);
	editor_junction_update(editor);
	gtk_combo_box_set_active(GTK_COMBO_BOX(editor->combo_initial), jd->pos);
	gtk_combo_box_set_active(GTK_COMBO_BOX(editor->combo_final), jd->pos2);
	#if DEBUG_EDITOR_JUNCTION_OPEN
		fprintf(stderr, "editor_junction_open: end\n");
	#endif
}

/**
 * \fn void editor_junction_destroy(EditorJunction *editor)
 * \brief Function to destroy an editor parameters of a junction.
 * \param editor
 * \brief editor parameters of a junction.
 */
void editor_junction_destroy(EditorJunction *editor)
{
	#if DEBUG_EDITOR_JUNCTION_DESTROY
		fprintf(stderr, "editor_junction_destroy: open\n");
	#endif
	gtk_widget_destroy(GTK_WIDGET(editor->frame));
	junction_data_delete(editor->jd);
	#if DEBUG_EDITOR_JUNCTION_DESTROY
		fprintf(stderr, "editor_junction_destroy: end\n");
	#endif
}

/**
 * \fn void editor_junction_new(EditorJunction *editor, char **channel_name, \
 *   int nchannels, char ***section_name, int *nsections)
 * \brief Function to create the editor parameters of a editor.
 * \param editor
 * \brief editor parameters of a junction.
 * \param channel_name
 * \brief array of channel names.
 * \param nchannels
 * \brief number of channels.
 * \param section_name
 * \brief array of arrays of cross section names.
 * \param nsections
 * \brief array of number of cross sections.
 */
void editor_junction_new(EditorJunction *editor, char **channel_name,
	int nchannels, char ***section_name, int *nsections)
{
	#if DEBUG_EDITOR_JUNCTION_NEW
		fprintf(stderr, "editor_junction_new: start\n");
	#endif
	editor->nchannels = nchannels;
	editor->nsections = nsections;
	editor->channel_name = channel_name;
	editor->section_name = section_name;
	editor->grid = (GtkGrid*)gtk_grid_new();
	editor->label_channel = (GtkLabel*)gtk_label_new(gettext("Channel"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->label_channel), 0, 1, 1, 1);
	editor->combo_channel
		= jbw_combo_box_new_with_strings(channel_name, nchannels);
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->combo_channel), 1, 1, 1, 1);
	editor->label_initial
		= (GtkLabel*)gtk_label_new(gettext("Initial cross section"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->label_initial), 0, 2, 1, 1);
	editor->combo_initial
		= jbw_combo_box_new_with_strings(section_name[0], nsections[0]);
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->combo_initial), 1, 2, 1, 1);
	editor->label_final
		= (GtkLabel*)gtk_label_new(gettext("Final cross section"));
	gtk_grid_attach(editor->grid, GTK_WIDGET(editor->label_final), 0, 3, 1, 1);
	editor->combo_final
		= jbw_combo_box_new_with_strings(section_name[0], nsections[0]);
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->combo_final), 1, 3, 1, 1);
	editor->frame = (GtkFrame*)gtk_frame_new(gettext("Junction"));
	gtk_container_add
		(GTK_CONTAINER(editor->frame), GTK_WIDGET(editor->grid));
	#if DEBUG_EDITOR_JUNCTION_NEW
		fprintf(stderr, "editor_junction_new: end\n");
	#endif
}

/**
 * \fn void editor_boundary_flow_update_junction(EditorBoundaryFlow *editor)
 * \brief Function to update the junction data in the editor.
 * \param editor
 * \brief flow boundary conditions editor.
 */
void editor_boundary_flow_update_junction(EditorBoundaryFlow *editor)
{
	int i;
	EditorJunction *editor_junction;
	#if DEBUG_EDITOR_BOUNDARY_FLOW_UPDATE_JUNCTION
		fprintf(stderr, "editor_boundary_flow_update_junction: start\n");
	#endif
	editor_junction = editor->editor_junction;
	editor_junction_update(editor_junction);
	i = gtk_combo_box_get_active(GTK_COMBO_BOX(editor->combo_junction));
	g_signal_handler_block(editor->combo_junction, editor->id_junction);
	gtk_combo_box_text_remove(editor->combo_junction, i);
	gtk_combo_box_text_insert_text(editor->combo_junction, i,
		gtk_combo_box_text_get_active_text(editor_junction->combo_channel));
	gtk_combo_box_set_active(GTK_COMBO_BOX(editor->combo_junction), i);
	g_signal_handler_unblock(editor->combo_junction, editor->id_junction);
	#if DEBUG_EDITOR_BOUNDARY_FLOW_UPDATE_JUNCTION
		fprintf(stderr, "editor_boundary_flow_update_junction: end\n");
	#endif
}

/**
 * \fn void editor_boundary_flow_update(EditorBoundaryFlow *editor)
 * \brief Function to update the view of a flow boundary conditions editor.
 * \param editor
 * \brief flow boundary conditions editor.
 */
void editor_boundary_flow_update(EditorBoundaryFlow *editor)
{
	int type;
	char *buffer;
	JunctionData *jd;
	BoundaryFlow *bf;
	EditorJunction *editor_junction;
	static int show_discharge[N_BOUNDARY_FLOW_TYPES] =
		{1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	show_depth[N_BOUNDARY_FLOW_TYPES] =
		{0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	show_level[N_BOUNDARY_FLOW_TYPES] =
		{0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
	show_array[N_BOUNDARY_FLOW_TYPES] =
		{0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0};
	#if DEBUG_EDITOR_BOUNDARY_FLOW_UPDATE
		fprintf(stderr, "editor_boundary_flow_update: start\n");
	#endif
	bf = editor->bf;
	type = boundary_flow_type(bf,
		jbw_array_radio_buttons_get_active(editor->array_type));
	#if DEBUG_EDITOR_BOUNDARY_FLOW_UPDATE
		fprintf(stderr, "EBFU type=%d\n", type);
	#endif
	if (show_discharge[type])
	{
		gtk_widget_show(GTK_WIDGET(editor->label_discharge));
		gtk_widget_show(GTK_WIDGET(editor->entry_discharge));
	}
	else
	{
		gtk_widget_hide(GTK_WIDGET(editor->label_discharge));
		gtk_widget_hide(GTK_WIDGET(editor->entry_discharge));
	}
	if (show_depth[type])
	{
		gtk_widget_show(GTK_WIDGET(editor->label_depth));
		gtk_widget_show(GTK_WIDGET(editor->entry_depth));
	}
	else
	{
		gtk_widget_hide(GTK_WIDGET(editor->label_depth));
		gtk_widget_hide(GTK_WIDGET(editor->entry_depth));
	}
	if (show_level[type])
	{
		gtk_widget_show(GTK_WIDGET(editor->label_level));
		gtk_widget_show(GTK_WIDGET(editor->entry_level));
	}
	else
	{
		gtk_widget_hide(GTK_WIDGET(editor->label_level));
		gtk_widget_hide(GTK_WIDGET(editor->entry_level));
	}
	if (show_array[type])
	{
		gtk_widget_show(GTK_WIDGET(editor->array->scrolled));
		gtk_widget_show(GTK_WIDGET(editor->button_insert));
		gtk_widget_show(GTK_WIDGET(editor->button_remove));
		gtk_widget_show(GTK_WIDGET(editor->button_plot));
		gtk_widget_set_sensitive
			(GTK_WIDGET(editor->button_remove), editor->array->n > 1);
	}
	else
	{
		gtk_widget_hide(GTK_WIDGET(editor->array->scrolled));
		gtk_widget_hide(GTK_WIDGET(editor->button_insert));
		gtk_widget_hide(GTK_WIDGET(editor->button_remove));
		gtk_widget_hide(GTK_WIDGET(editor->button_plot));
	}
	if (type == BOUNDARY_FLOW_TYPE_DAM)
	{
		gtk_widget_show(GTK_WIDGET(editor->label_roughness));
		gtk_widget_show(GTK_WIDGET(editor->entry_roughness));
	}
	else
	{
		gtk_widget_hide(GTK_WIDGET(editor->label_roughness));
		gtk_widget_hide(GTK_WIDGET(editor->entry_roughness));
	}
	if (type == BOUNDARY_FLOW_TYPE_GATE)
	{
		gtk_widget_show(GTK_WIDGET(editor->label_width));
		gtk_widget_show(GTK_WIDGET(editor->entry_width));
		gtk_widget_show(GTK_WIDGET(editor->label_height));
		gtk_widget_show(GTK_WIDGET(editor->entry_height));
	}
	else
	{
		gtk_widget_hide(GTK_WIDGET(editor->label_width));
		gtk_widget_hide(GTK_WIDGET(editor->entry_width));
		gtk_widget_hide(GTK_WIDGET(editor->label_height));
		gtk_widget_hide(GTK_WIDGET(editor->entry_height));
	}
	if (type == BOUNDARY_FLOW_TYPE_JUNCTION)
	{
		editor_junction = editor->editor_junction;
		gtk_widget_show(GTK_WIDGET(editor->grid_junction));
		buffer = gtk_combo_box_text_get_active_text(editor->combo_junction);
		if (!buffer)
		{
			bf->data
				= jb_realloc(bf->data, 2 * sizeof(int) + sizeof(JunctionData));
			JUNCTION_N(bf) = 0;
			jd = JUNCTION_DATA(bf, 0);
			jd->channel_name = jb_strdup(editor_junction->channel_name[0]);
			jd->section = jb_strdup(editor_junction->section_name[0][0]);
			jd->section2 = NULL;
			jd->channel = jd->pos = jd->pos2 = 0;
			g_signal_handler_block(editor->combo_junction, editor->id_junction);
			gtk_combo_box_text_insert_text
				(editor->combo_junction, 0, jd->channel_name);
			gtk_combo_box_set_active(GTK_COMBO_BOX(editor->combo_junction), 0);
			g_signal_handler_unblock
				(editor->combo_junction, editor->id_junction);
			gtk_widget_set_sensitive(GTK_WIDGET(editor_junction->grid), 0);
			gtk_widget_set_sensitive
				(GTK_WIDGET(editor->button_remove_junction), 0);
		}
		else
		{
			gtk_widget_set_sensitive(GTK_WIDGET(editor_junction->grid), 1);
			gtk_widget_set_sensitive(GTK_WIDGET(editor->button_remove_junction),
				JUNCTION_N(editor->bf) > 0);
			g_free(buffer);
		}
	}
	else
	{
		g_signal_handler_block(editor->combo_junction, editor->id_junction);
		gtk_combo_box_text_remove_all(editor->combo_junction);
		g_signal_handler_unblock(editor->combo_junction, editor->id_junction);
		gtk_widget_hide(GTK_WIDGET(editor->grid_junction));
	}
	jbw_array_editor_set_title(editor->array, 0, gettext("Time"));
	jbw_graphic_set_xlabel(editor->graphic, gettext("Time"));
	jbw_array_editor_set_title(editor->array, 1, gettext("Discharge"));
	jbw_graphic_set_ylabel(editor->graphic, gettext("Discharge"));
	switch (type)
	{
		case BOUNDARY_FLOW_TYPE_QT_HT:
			jbw_array_editor_set_title(editor->array, 2, gettext("Depth"));
			jbw_graphic_set_zlabel(editor->graphic, gettext("Depth"));
			break;
		case BOUNDARY_FLOW_TYPE_QT_ZT:
			jbw_array_editor_set_title(editor->array, 2, gettext("Level"));
			jbw_graphic_set_zlabel(editor->graphic, gettext("Level"));
			break;
		case BOUNDARY_FLOW_TYPE_HT:
			jbw_array_editor_set_title(editor->array, 1, gettext("Depth"));
			jbw_graphic_set_ylabel(editor->graphic, gettext("Depth"));
			break;
		case BOUNDARY_FLOW_TYPE_ZT:
			jbw_array_editor_set_title(editor->array, 1, gettext("Level"));
			jbw_graphic_set_ylabel(editor->graphic, gettext("Level"));
			break;
		case BOUNDARY_FLOW_TYPE_QH:
			jbw_array_editor_set_title(editor->array, 0, gettext("Depth"));
			jbw_graphic_set_xlabel(editor->graphic, gettext("Depth"));
			break;
		case BOUNDARY_FLOW_TYPE_QZ:
			jbw_array_editor_set_title(editor->array, 0, gettext("Level"));
			jbw_graphic_set_xlabel(editor->graphic, gettext("Level"));
			break;
		case BOUNDARY_FLOW_TYPE_GATE:
			jbw_array_editor_set_title(editor->array, 1, gettext("Opening"));
			jbw_graphic_set_ylabel(editor->graphic, gettext("Opening"));
		case BOUNDARY_FLOW_TYPE_DAM:
			bf->data = jb_realloc(bf->data, 2 * sizeof(JBFLOAT));
	}
	bf->type = type;
	#if DEBUG_EDITOR_BOUNDARY_FLOW_UPDATE
		boundary_flow_print(bf, stderr);
		fprintf(stderr, "editor_boundary_flow_update: end\n");
	#endif
}

/**
 * \fn int editor_boundary_flow_get_junction(EditorBoundaryFlow *editor)
 * \brief Function to get the actual junction data from the editor.
 * \param editor
 * \brief flow boundary conditions editor.
 * \return 1 on valid junctio, 0 on error.
 */
int editor_boundary_flow_get_junction(EditorBoundaryFlow *editor)
{
	int i;
	char *buffer;
	JunctionData *jd;
	#if DEBUG_EDITOR_BOUNDARY_FLOW_GET_JUNCTION
		fprintf(stderr, "editor_boundary_flow_get_junction: start\n");
	#endif
	buffer = gtk_combo_box_text_get_active_text(editor->combo_junction);
	if (!buffer)
	{
		boundary_flow_error(editor->bf, gettext("No junctions"));
		goto error_get_junction;
	}
	g_free(buffer);
	i = editor->ijunction;
	#if DEBUG_EDITOR_BOUNDARY_FLOW_GET_JUNCTION
		boundary_flow_print(editor->bf, stderr);
	#endif
	jd = JUNCTION_DATA(editor->bf, i);
	#if DEBUG_EDITOR_BOUNDARY_FLOW_GET_JUNCTION
		junction_data_print(jd, stderr);
	#endif
	junction_data_delete(jd);
	if (!editor_junction_get(editor->editor_junction)
		|| !junction_data_copy(jd, editor->editor_junction->jd))
			goto error_get_junction;
	#if DEBUG_EDITOR_BOUNDARY_FLOW_GET_JUNCTION
		boundary_flow_print(editor->bf, stderr);
		fprintf(stderr, "editor_boundary_flow_get_junction: end\n");
	#endif
	return 1;

error_get_junction:
	#if DEBUG_EDITOR_BOUNDARY_FLOW_GET_JUNCTION
		fprintf(stderr, "editor_boundary_flow_get_junction: end\n");
	#endif
	return 0;
}

/**
 * \fn int editor_boundary_flow_get(EditorBoundaryFlow *editor)
 * \brief Function to get the actual flow boundary conditions data from the
 *   editor.
 * \param editor
 * \brief flow boundary conditions editor.
 * \return 1 on valid boundary condition, 0 on error.
 */
int editor_boundary_flow_get(EditorBoundaryFlow *editor)
{
	int i, j;
	JBWArrayEditor *array;
	BoundaryFlow *bf, bf_copy[1];
	#if DEBUG_EDITOR_BOUNDARY_FLOW_GET
		fprintf(stderr, "editor_boundary_flow_get: start\n");
	#endif
	editor_boundary_flow_update(editor);
	bf = editor->bf;
	bf->type = boundary_flow_type(bf,
		jbw_array_radio_buttons_get_active(editor->array_type));
	if (bf->type == BOUNDARY_FLOW_TYPE_JUNCTION)
	{
		editor->ijunction
			= gtk_combo_box_get_active(GTK_COMBO_BOX(editor->combo_junction));
		if (!editor_boundary_flow_get_junction(editor))
		{
			boundary_flow_error(bf, message);
			goto error_get;
		}
		boundary_flow_copy(bf_copy, bf);
	}
	boundary_flow_delete(bf);
	bf->name = g_strdup(gtk_entry_get_text(editor->entry_name));
	if (!bf->name) goto error_memory;
	if (!strcmp(bf->name, ""))
	{
		boundary_flow_error(bf, gettext("Not name"));
		goto error_get;
	}
	if (!bf->position)
	{
		i = gtk_combo_box_get_active(GTK_COMBO_BOX(editor->combo_initial));
		j = gtk_combo_box_get_active(GTK_COMBO_BOX(editor->combo_final));
		if (j < i)
		{
			boundary_flow_error(bf, gettext("Bad order"));
			goto error_get;
		}
		bf->section = gtk_combo_box_text_get_active_text(editor->combo_initial);
		if (!bf->section) goto error_memory;
		if (j > i)
		{
			bf->section2
				= gtk_combo_box_text_get_active_text(editor->combo_final);
			if (!bf->section2) goto error_memory;
		}
	}
	bf->delay = gtk_spin_button_get_value(editor->entry_delay);
	array = editor->array;
	switch (bf->type)
	{
		case BOUNDARY_FLOW_TYPE_Q_H:
		case BOUNDARY_FLOW_TYPE_Q_Z:
			bf->p2 = (JBFLOAT*)g_try_malloc(sizeof(JBFLOAT));
			if (!bf->p2) goto error_memory;
		case BOUNDARY_FLOW_TYPE_Q:
		case BOUNDARY_FLOW_TYPE_H:
		case BOUNDARY_FLOW_TYPE_Z:
			bf->t = (JBDOUBLE*)g_try_malloc(sizeof(JBDOUBLE));
			bf->p1 = (JBFLOAT*)g_try_malloc(sizeof(JBFLOAT));
			if (!bf->t || !bf->p1) goto error_memory;
			bf->t[0] = 0.;
			bf->n = 0;
			break;
		case BOUNDARY_FLOW_TYPE_QT_HT:
		case BOUNDARY_FLOW_TYPE_QT_ZT:
			bf->p2 = (JBFLOAT*)g_try_malloc(array->n * sizeof(JBFLOAT));
			if (!bf->p2) goto error_memory;
			jbw_array_editor_get_column_float(array, 2, bf->p2);
		case BOUNDARY_FLOW_TYPE_QT:
		case BOUNDARY_FLOW_TYPE_HT:
		case BOUNDARY_FLOW_TYPE_ZT:
		case BOUNDARY_FLOW_TYPE_DAM:
		case BOUNDARY_FLOW_TYPE_PIPE:
		case BOUNDARY_FLOW_TYPE_GATE:
			bf->t = (JBDOUBLE*)g_try_malloc(array->n * sizeof(JBDOUBLE));
			bf->p1 = (JBFLOAT*)g_try_malloc(array->n * sizeof(JBFLOAT));
			if (!bf->t || !bf->p1) goto error_memory;
			jbw_array_editor_get_column_time(array, 0, bf->t);
			jbw_array_editor_get_column_float(array, 1, bf->p1);
			bf->n = array->n - 1;
			break;
		case BOUNDARY_FLOW_TYPE_QH:
		case BOUNDARY_FLOW_TYPE_QZ:
			bf->p1 = (JBFLOAT*)g_try_malloc(array->n * sizeof(JBFLOAT));
			bf->p2 = (JBFLOAT*)g_try_malloc(array->n * sizeof(JBFLOAT));
			if (!bf->p1 || !bf->p2) goto error_memory;
			jbw_array_editor_get_column_float(array, 0, bf->p1);
			jbw_array_editor_get_column_float(array, 1, bf->p2);
			bf->n = array->n - 1;
			break;
		case BOUNDARY_FLOW_TYPE_JUNCTION:
			bf->data = g_try_malloc(2 * sizeof(int)
				+ (1 + JUNCTION_N(bf_copy)) * sizeof(JunctionData));
			if (!bf->data) goto error_memory;
			JUNCTION_N(bf) = JUNCTION_N(bf_copy);
			for (i = 0; i <= JUNCTION_N(bf); ++i)
				if (!junction_data_copy
					(JUNCTION_DATA(bf, i), JUNCTION_DATA(bf_copy, i)))
				{
					boundary_flow_error(bf, message);
					goto error_get;
				}
			boundary_flow_delete(bf_copy);
	}
	switch (bf->type)
	{
		case BOUNDARY_FLOW_TYPE_Q_H:
			bf->p2[0] = gtk_spin_button_get_value(editor->entry_depth);
		case BOUNDARY_FLOW_TYPE_Q:
			bf->p1[0] = gtk_spin_button_get_value(editor->entry_discharge);
			break;
		case BOUNDARY_FLOW_TYPE_H:
			bf->p1[0] = gtk_spin_button_get_value(editor->entry_depth);
			break;
		case BOUNDARY_FLOW_TYPE_Z:
			bf->p1[0] = gtk_spin_button_get_value(editor->entry_level);
			break;
		case BOUNDARY_FLOW_TYPE_Q_Z:
			bf->p1[0] = gtk_spin_button_get_value(editor->entry_discharge);
			bf->p2[0] = gtk_spin_button_get_value(editor->entry_level);
			break;
		case BOUNDARY_FLOW_TYPE_DAM:
			bf->data = g_try_malloc(2 * sizeof(JBFLOAT));
			if (!bf->data) goto error_memory;
			DAM_LEVEL(bf) = gtk_spin_button_get_value(editor->entry_level);
			DAM_ROUGHNESS(bf)
				= gtk_spin_button_get_value(editor->entry_roughness);
			break;
		case BOUNDARY_FLOW_TYPE_GATE:
			bf->data = g_try_malloc(2 * sizeof(JBFLOAT));
			if (!bf->data) goto error_memory;
			GATE_HEIGHT(bf) = gtk_spin_button_get_value(editor->entry_height);
			GATE_WIDTH(bf)
				= gtk_spin_button_get_value(editor->entry_width);
			break;
	}
	#if DEBUG_EDITOR_BOUNDARY_FLOW_GET
		boundary_flow_print(bf, stderr);
		fprintf(stderr, "editor_boundary_flow_get: end\n");
	#endif
	return 1;

error_memory:
	boundary_flow_error(bf, gettext("Not enough memory"));

error_get:
	#if DEBUG_EDITOR_BOUNDARY_FLOW_GET
		fprintf(stderr, "editor_boundary_flow_get: end\n");
	#endif
	return 0;
}

/**
 * \fn void editor_boundary_flow_open_junction(EditorBoundaryFlow *editor)
 * \brief Function to open a junction data in the editor.
 * \param editor
 * \brief flow boundary conditions editor.
 */
void editor_boundary_flow_open_junction(EditorBoundaryFlow *editor)
{
	int i;
	EditorJunction *editor_junction;
	#if DEBUG_EDITOR_BOUNDARY_FLOW_OPEN_JUNCTION
		fprintf(stderr, "editor_boundary_flow_open_junction: start\n");
	#endif
	editor_boundary_flow_get_junction(editor);
	editor_junction = editor->editor_junction;
	i = gtk_combo_box_get_active(GTK_COMBO_BOX(editor->combo_junction));
	junction_data_delete(editor_junction->jd);
	junction_data_copy(editor_junction->jd, JUNCTION_DATA(editor->bf, i));
	editor_junction_open(editor_junction);
	editor_boundary_flow_update(editor);
	editor->ijunction = i;
	#if DEBUG_EDITOR_BOUNDARY_FLOW_OPEN_JUNCTION
		fprintf(stderr, "editor_boundary_flow_open_junction: end\n");
	#endif
}

/**
 * \fn void editor_boundary_flow_open(EditorBoundaryFlow *editor)
 * \brief Function to open the flow boundary conditions data in the editor.
 * \param editor
 * \brief flow boundary conditions editor.
 */
void editor_boundary_flow_open(EditorBoundaryFlow *editor)
{
	int i;
	JunctionData *jd;
	BoundaryFlow *bf;
	#if DEBUG_EDITOR_BOUNDARY_FLOW_OPEN
		fprintf(stderr, "editor_boundary_flow_open: start\n");
	#endif
	bf = editor->bf;
	gtk_entry_set_text(editor->entry_name, bf->name);
	if (!bf->position)
	{
		gtk_combo_box_set_active(GTK_COMBO_BOX(editor->combo_initial), bf->pos);
		gtk_combo_box_set_active(GTK_COMBO_BOX(editor->combo_final), bf->pos2);
	}
	gtk_spin_button_set_value(editor->entry_delay, bf->delay);
	for (i = 0; i < N_BOUNDARY_FLOW_EXTERN_TYPES; ++i)
		if (boundary_flow_type(bf, i) == bf->type) break;
	jbw_array_radio_buttons_set_active(editor->array_type, i, 1);
	switch (bf->type)
	{
		case BOUNDARY_FLOW_TYPE_Q_H:
			gtk_spin_button_set_value(editor->entry_depth, bf->p2[0]);
		case BOUNDARY_FLOW_TYPE_Q:
			gtk_spin_button_set_value(editor->entry_discharge, bf->p1[0]);
			break;
		case BOUNDARY_FLOW_TYPE_H:
			gtk_spin_button_set_value(editor->entry_depth, bf->p1[0]);
			break;
		case BOUNDARY_FLOW_TYPE_Z:
			gtk_spin_button_set_value(editor->entry_level, bf->p1[0]);
			break;
		case BOUNDARY_FLOW_TYPE_Q_Z:
			gtk_spin_button_set_value(editor->entry_level, bf->p2[0]);
			gtk_spin_button_set_value(editor->entry_discharge, bf->p1[0]);
			break;
		case BOUNDARY_FLOW_TYPE_DAM:
			gtk_spin_button_set_value(editor->entry_level, DAM_LEVEL(bf));
			gtk_spin_button_set_value
				(editor->entry_roughness, DAM_ROUGHNESS(bf));
		case BOUNDARY_FLOW_TYPE_QT:
		case BOUNDARY_FLOW_TYPE_HT:
		case BOUNDARY_FLOW_TYPE_ZT:
		case BOUNDARY_FLOW_TYPE_PIPE:
		case BOUNDARY_FLOW_TYPE_GATE:
			jbw_array_editor_set_rows(editor->array, bf->n + 1);
			jbw_array_editor_set_column_time(editor->array, 0, bf->t);
			jbw_array_editor_set_column_float(editor->array, 1, bf->p1);
			break;
		case BOUNDARY_FLOW_TYPE_QT_HT:
		case BOUNDARY_FLOW_TYPE_QT_ZT:
			jbw_array_editor_set_rows(editor->array, bf->n + 1);
			jbw_array_editor_set_column_time(editor->array, 0, bf->t);
			jbw_array_editor_set_column_float(editor->array, 1, bf->p1);
			jbw_array_editor_set_column_float(editor->array, 2, bf->p2);
			break;
		case BOUNDARY_FLOW_TYPE_QH:
		case BOUNDARY_FLOW_TYPE_QZ:
			jbw_array_editor_set_rows(editor->array, bf->n + 1);
			jbw_array_editor_set_column_float(editor->array, 0, bf->p1);
			jbw_array_editor_set_column_float(editor->array, 1, bf->p2);
			break;
		case BOUNDARY_FLOW_TYPE_JUNCTION:
			gtk_combo_box_text_remove_all(editor->combo_junction);
			for (i = 0; i <= JUNCTION_N(bf); ++i)
			{
				jd = JUNCTION_DATA(bf, i);
				gtk_combo_box_text_append_text
					(editor->combo_junction, jd->channel_name);
			}
			editor_junction_open(editor->editor_junction);
	}
	switch (bf->type)
	{
		case BOUNDARY_FLOW_TYPE_GATE:
			gtk_spin_button_set_value(editor->entry_height, GATE_HEIGHT(bf));
			gtk_spin_button_set_value(editor->entry_width, GATE_WIDTH(bf));
	}
	editor_boundary_flow_update(editor);
	#if DEBUG_EDITOR_BOUNDARY_FLOW_OPEN
		fprintf(stderr, "editor_boundary_flow_open: end\n");
	#endif
}

/**
 * \fn void editor_boundary_flow_insert_point \
 *   (EditorBoundaryFlow *editor)
 * \brief Function to insert a point in a flow boundary
 *   conditions editor.
 * \param editor
 * \brief flow boundary conditions editor.
 */
void editor_boundary_flow_insert_point(EditorBoundaryFlow *editor)
{
	#if DEBUG_EDITOR_BOUNDARY_FLOW_INSERT_POINT
		fprintf(stderr, "editor_boundary_flow_insert_point: start\n");
	#endif
	jbw_array_editor_insert(editor->array);
	editor_boundary_flow_update(editor);
	#if DEBUG_EDITOR_BOUNDARY_FLOW_INSERT_POINT
		fprintf(stderr, "editor_boundary_flow_insert_point: end\n");
	#endif
}

/**
 * \fn void editor_boundary_flow_remove_point \
 *   (EditorBoundaryFlow *editor)
 * \brief Function to remove a point in a flow boundary
 *   conditions editor.
 * \param editor
 * \brief flow boundary conditions editor.
 */
void editor_boundary_flow_remove_point(EditorBoundaryFlow *editor)
{
	#if DEBUG_EDITOR_BOUNDARY_FLOW_REMOVE_POINT
		fprintf(stderr, "editor_boundary_flow_remove_point: start\n");
	#endif
	jbw_array_editor_remove(editor->array);
	editor_boundary_flow_update(editor);
	#if DEBUG_EDITOR_BOUNDARY_FLOW_REMOVE_POINT
		fprintf(stderr, "editor_boundary_flow_remove_point: end\n");
	#endif
}

/**
 * \fn void editor_boundary_flow_insert_junction(EditorBoundaryFlow *editor)
 * \brief Function to insert a junction data in the actual flow boundary
 *   condition data of the editor.
 * \param editor
 * \brief flow boundary condition editor.
 */
void editor_boundary_flow_insert_junction(EditorBoundaryFlow *editor)
{
	int i;
	JunctionData *jd;
	BoundaryFlow *bf;
	EditorJunction *editor_junction;
	#if DEBUG_EDITOR_BOUNDARY_FLOW_INSERT_JUNCTION
		fprintf(stderr, "editor_boundary_flow_insert_junction: start\n");
	#endif
	bf = editor->bf;
	editor_junction = editor->editor_junction;
	editor->ijunction
		= gtk_combo_box_get_active(GTK_COMBO_BOX(editor->combo_junction));
	editor_boundary_flow_get_junction(editor);
	i = 1 + editor->ijunction;
	boundary_flow_insert_junction(bf, i);
	jd = JUNCTION_DATA(bf, i);
	g_signal_handler_block(editor->combo_junction, editor->id_junction);
	gtk_combo_box_text_insert_text(editor->combo_junction, i, jd->channel_name);
	gtk_combo_box_set_active(GTK_COMBO_BOX(editor->combo_junction), i);
	g_signal_handler_unblock(editor->combo_junction, editor->id_junction);
	editor_boundary_flow_update(editor);
	junction_data_delete(editor_junction->jd);
	junction_data_copy(editor_junction->jd, jd);
	editor_junction_open(editor_junction);
	#if DEBUG_EDITOR_BOUNDARY_FLOW_INSERT_JUNCTION
		boundary_flow_print(bf, stderr);
		fprintf(stderr, "editor_boundary_flow_insert_junction: end\n");
	#endif
	return;
}

/**
 * \fn void editor_boundary_flow_remove_junction(EditorBoundaryFlow *editor)
 * \brief Function to remove a junction data in the actual flow boundary
 *   condition data of the editor.
 * \param editor
 * \brief flow boundary condition editor.
 */
void editor_boundary_flow_remove_junction(EditorBoundaryFlow *editor)
{
	int i;
	#if DEBUG_EDITOR_BOUNDARY_FLOW_REMOVE_JUNCTION
		fprintf(stderr, "editor_boundary_flow_remove_junction: start\n");
	#endif
	i = gtk_combo_box_get_active(GTK_COMBO_BOX(editor->combo_junction));
	boundary_flow_remove_junction(editor->bf, i);
	g_signal_handler_block(editor->combo_junction, editor->id_junction);
	gtk_combo_box_text_remove(editor->combo_junction, i);
	g_signal_handler_unblock(editor->combo_junction, editor->id_junction);
	gtk_combo_box_set_active(GTK_COMBO_BOX(editor->combo_junction),
		jbm_min(i, JUNCTION_N(editor->bf)));
	editor_boundary_flow_update(editor);
	#if DEBUG_EDITOR_BOUNDARY_FLOW_REMOVE_JUNCTION
		fprintf(stderr, "editor_boundary_flow_remove_junction: end\n");
	#endif
}

/**
 * \fn void editor_boundary_flow_draw(EditorBoundaryFlow *editor)
 * \brief Function to draw a flow boundary conditions in a flow boundary
 *   conditions editor.
 * \param editor
 * \brief flow boundary conditions editor.
 */
void editor_boundary_flow_draw(EditorBoundaryFlow *editor)
{
	int i, n;
	JBFLOAT *t, *p1, *p2;
	BoundaryFlow *bf;
	#if DEBUG_EDITOR_BOUNDARY_FLOW_DRAW
		fprintf(stderr, "editor_boundary_flow_draw: start\n");
	#endif
	editor_boundary_flow_get(editor);
	bf = editor->bf;
	n = bf->n + 1;
	t = p1 = p2 = NULL;
	switch (bf->type)
	{
		case BOUNDARY_FLOW_TYPE_Q_H:
		case BOUNDARY_FLOW_TYPE_Q_Z:
			p1 = g_alloca(n * sizeof(JBFLOAT));
			memcpy(p1, bf->p1, n * sizeof(JBFLOAT));
			p2 = g_alloca(n * sizeof(JBFLOAT));
			memcpy(p2, bf->p2, n * sizeof(JBFLOAT));
			jbw_graphic_draw_lines
				(editor->graphic, p1, p2, NULL, NULL, NULL, n);
			break;
		case BOUNDARY_FLOW_TYPE_QT_HT:
		case BOUNDARY_FLOW_TYPE_QT_ZT:
			p2 = g_alloca(n * sizeof(JBFLOAT));
			memcpy(p2, bf->p2, n * sizeof(JBFLOAT));
		case BOUNDARY_FLOW_TYPE_Q:
		case BOUNDARY_FLOW_TYPE_H:
		case BOUNDARY_FLOW_TYPE_Z:
		case BOUNDARY_FLOW_TYPE_QT:
		case BOUNDARY_FLOW_TYPE_HT:
		case BOUNDARY_FLOW_TYPE_ZT:
		case BOUNDARY_FLOW_TYPE_DAM:
		case BOUNDARY_FLOW_TYPE_PIPE:
		case BOUNDARY_FLOW_TYPE_GATE:
			t = g_alloca(n * sizeof(JBFLOAT));
			p1 = g_alloca(n * sizeof(JBFLOAT));
			for (i = 0; i < n; ++i) t[i] = bf->t[i];
			memcpy(p1, bf->p1, n * sizeof(JBFLOAT));
			break;
		default:
			jbw_graphic_draw_lines(editor->graphic, t, p1, p2, NULL, NULL, n);
	}
	jbw_graphic_draw_logo(editor->graphic);
#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
	glutSwapBuffers();
#elif JBW_GRAPHIC == JBW_GRAPHIC_CLUTTER
	glFlush();
#endif
	#if DEBUG_EDITOR_BOUNDARY_FLOW_DRAW
		fprintf(stderr, "editor_boundary_flow_draw: end\n");
	#endif
}

/**
 * \fn void editor_boundary_flow_destroy(EditorBoundaryFlow *editor)
 * \brief Function to destroy a flow boundary conditions editor.
 * \param editor
 * \brief flow boundary conditions editor.
 */
void editor_boundary_flow_destroy(EditorBoundaryFlow *editor)
{
	#if DEBUG_EDITOR_BOUNDARY_FLOW_DESTROY
		fprintf(stderr, "editor_boundary_flow_destroy: start\n");
	#endif
	editor_junction_destroy(editor->editor_junction);
	gtk_widget_destroy(GTK_WIDGET(editor->grid));
	boundary_flow_delete(editor->bf);
	#if DEBUG_EDITOR_BOUNDARY_FLOW_DESTROY
		fprintf(stderr, "editor_boundary_flow_destroy: end\n");
	#endif
}

/**
 * \fn void editor_boundary_flow_new(EditorBoundaryFlow *editor, \
 *   GtkNotebook *notebook, char **section_name, int nsections)
 * \brief Function to create a new flow boundary conditions
 *   editor.
 * \param editor
 * \brief flow boundary conditions editor.
 * \param notebook
 * \brief GtkNotebook to pack the widgets.
 * \param section_name
 * \brief array of cross section names.
 * \param nsections
 * \brief number of cross sections.
 */
void editor_boundary_flow_new(EditorBoundaryFlow *editor, GtkNotebook *notebook,
	int position, int channel, char **channel_name, int nchannels,
	char ***section_name, int *nsections)
{
	int i, ntypes;
	char **label_type;
	BoundaryFlow *bf;
	EditorJunction *editor_junction;
	const char *label_type_extern[N_BOUNDARY_FLOW_EXTERN_TYPES] = {
		gettext("Constant discharge"),
		gettext("Constant depth"),
		gettext("Constant level"),
		gettext("Constant discharge and depth"),
		gettext("Constant discharge and level"),
		gettext("Discharge variable in time"),
		gettext("Depth variable in time"),
		gettext("Level variable in time"),
		gettext("Discharge and depth variable in time"),
		gettext("Discharge and level variable in time"),
		gettext("Discharge variable in depth"),
		gettext("Discharge variable in level"),
		gettext("Supercritical outlet"),
		gettext("Gate"),
		gettext("Junction")},
	*label_type_inner[N_BOUNDARY_FLOW_INNER_TYPES] = {
		gettext("Constant discharge"),
		gettext("Discharge variable in time"),
		gettext("Discharge variable in depth"),
		gettext("Discharge variable in level"),
		gettext("Dam"),
		gettext("Pipe"),
		gettext("Gate"),
		gettext("Junction")},
	*label_array[3] = {gettext("Time"), gettext("Discharge"), gettext("Depth")};
	#if DEBUG_EDITOR_BOUNDARY_FLOW_NEW
		fprintf(stderr, "editor_boundary_flow_new: start\n");
	#endif
	bf = editor->bf;
	editor_junction = editor->editor_junction;
	editor->notebook = notebook;
	boundary_flow_init_empty(bf);
	editor_junction_new(editor_junction, channel_name, nchannels,
		section_name, nsections);
	g_signal_connect_swapped(editor_junction->combo_channel, "changed",
		(void(*))&editor_boundary_flow_update_junction, editor);
	editor->grid = (GtkGrid*)gtk_grid_new();
	editor->label_name = (GtkLabel*)gtk_label_new(gettext("Name"));
	gtk_grid_attach(editor->grid, GTK_WIDGET(editor->label_name), 0, 0, 1, 1);
	editor->entry_name = (GtkEntry*)gtk_entry_new();
	gtk_grid_attach(editor->grid, GTK_WIDGET(editor->entry_name), 1, 0, 2, 1);
	bf->position = position;
	if (position < 0)
	{
		gtk_notebook_append_page(notebook,
			GTK_WIDGET(editor->grid), gtk_label_new(gettext("Inlet")));
		bf->pos = bf->pos2 = 0;
		ntypes = N_BOUNDARY_FLOW_EXTERN_TYPES;
		label_type = (char**)label_type_extern;
	}
	else if (position > 0)
	{
		gtk_notebook_append_page(notebook,
			GTK_WIDGET(editor->grid), gtk_label_new(gettext("Outlet")));
		bf->pos = bf->pos2 = nsections[channel] - 1;
		ntypes = N_BOUNDARY_FLOW_EXTERN_TYPES;
		label_type = (char**)label_type_extern;
	}
	else
	{
		ntypes = N_BOUNDARY_FLOW_INNER_TYPES;
		label_type = (char**)label_type_inner;
		gtk_notebook_append_page(notebook, GTK_WIDGET(editor->grid),
			gtk_label_new(gettext("Boundary condition")));
		editor->label_initial
			= (GtkLabel*)gtk_label_new(gettext("Initial cross section"));
		gtk_grid_attach
			(editor->grid, GTK_WIDGET(editor->label_initial), 0, 1, 1, 1);
		editor->combo_initial = jbw_combo_box_new_with_strings
			(section_name[channel], nsections[channel]);
		gtk_grid_attach
			(editor->grid, GTK_WIDGET(editor->combo_initial), 1, 1, 2, 1);
		editor->label_final
			= (GtkLabel*)gtk_label_new(gettext("Final cross section"));
		gtk_grid_attach
			(editor->grid, GTK_WIDGET(editor->label_final), 0, 2, 1, 1);
		editor->combo_final = jbw_combo_box_new_with_strings
			(section_name[channel], nsections[channel]);
		gtk_grid_attach
			(editor->grid, GTK_WIDGET(editor->combo_final), 1, 2, 2, 1);
	}
	editor->label_delay = (GtkLabel*)gtk_label_new(gettext("Delay"));
	gtk_grid_attach(editor->grid, GTK_WIDGET(editor->label_delay), 0, 3, 1, 1);
	editor->entry_delay
		= (GtkSpinButton*)gtk_spin_button_new_with_range(0., 86400., 0.01);
	gtk_grid_attach(editor->grid, GTK_WIDGET(editor->entry_delay), 1, 3, 2, 1);
	editor->frame_type = (GtkFrame*)gtk_frame_new(gettext("Type"));
	editor->grid_type = (GtkGrid*)gtk_grid_new();
	editor->array_type[0] = NULL;
	for (i = 0; i < ntypes; ++i)
	{
		editor->array_type[i]
			= (GtkRadioButton*)gtk_radio_button_new_with_label_from_widget
				(editor->array_type[0], label_type[i]);
		gtk_grid_attach
			(editor->grid_type, GTK_WIDGET(editor->array_type[i]), 0, i, 1, 1);
		g_signal_connect_swapped(editor->array_type[i], "clicked",
			(void(*))&editor_boundary_flow_update, editor);
	}
	gtk_container_add
		(GTK_CONTAINER(editor->frame_type), GTK_WIDGET(editor->grid_type));
	gtk_grid_attach(editor->grid, GTK_WIDGET(editor->frame_type), 3, 0, 1, 13);
	editor->button_insert
		= (GtkButton*)gtk_button_new_with_label(gettext("Insert point"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->button_insert), 0, 4, 1, 1);
	g_signal_connect_swapped(editor->button_insert, "clicked",
		(void(*))&editor_boundary_flow_insert_point, editor);
	editor->button_remove
		= (GtkButton*)gtk_button_new_with_label(gettext("Remove point"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->button_remove), 1, 4, 1, 1);
	g_signal_connect_swapped(editor->button_remove, "clicked",
		(void(*))&editor_boundary_flow_remove_point, editor);
	editor->button_plot
		= (GtkButton*)gtk_button_new_with_label(gettext("Update plot"));
	gtk_grid_attach(editor->grid, GTK_WIDGET(editor->button_plot), 2, 4, 1, 1);
	g_signal_connect(editor->button_plot, "clicked", &editor_draw, NULL);
	editor->array = jbw_array_editor_new(3, 3, 1, label_array);
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->array->scrolled), 0, 5, 3, 1);
	editor->label_discharge = (GtkLabel*)gtk_label_new(gettext("Discharge"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->label_discharge), 0, 6, 1, 1);
	editor->entry_discharge
		= (GtkSpinButton*)gtk_spin_button_new_with_range(-1e6, 1e6, 1e-6);
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->entry_discharge), 1, 6, 2, 1);
	editor->label_depth = (GtkLabel*)gtk_label_new(gettext("Depth"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->label_depth), 0, 7, 1, 1);
	editor->entry_depth
		= (GtkSpinButton*)gtk_spin_button_new_with_range(0., 1000., 0.001);
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->entry_depth), 1, 7, 2, 1);
	editor->label_level = (GtkLabel*)gtk_label_new(gettext("Level"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->label_level), 0, 8, 1, 1);
	editor->entry_level
		= (GtkSpinButton*)gtk_spin_button_new_with_range(-1000., 10000., 0.001);
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->entry_level), 1, 8, 2, 1);
	editor->label_roughness = (GtkLabel*)gtk_label_new(gettext("Roughness"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->label_roughness), 0, 9, 1, 1);
	editor->entry_roughness
		= (GtkSpinButton*)gtk_spin_button_new_with_range(0., 1., 0.0001);
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->entry_roughness), 1, 9, 2, 1);
	editor->label_height = (GtkLabel*)gtk_label_new(gettext("Height"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->label_height), 0, 10, 1, 1);
	editor->entry_height
		= (GtkSpinButton*)gtk_spin_button_new_with_range(-1000., 10000., 0.001);
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->entry_height), 1, 10, 2, 1);
	editor->label_width = (GtkLabel*)gtk_label_new(gettext("Width"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->label_width), 0, 11, 1, 1);
	editor->entry_width
		= (GtkSpinButton*)gtk_spin_button_new_with_range(0., 100., 0.001);
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->entry_width), 1, 11, 2, 1);
	editor->grid_junction = (GtkGrid*)gtk_grid_new();
	editor->button_insert_junction = (GtkButton*)gtk_button_new_with_label
		(gettext("Insert junction data"));
	gtk_grid_attach(editor->grid_junction,
		GTK_WIDGET(editor->button_insert_junction), 0, 0, 1, 1);
	g_signal_connect_swapped(editor->button_insert_junction, "clicked",
		(void(*))&editor_boundary_flow_insert_junction, editor);
	editor->button_remove_junction = (GtkButton*)gtk_button_new_with_label
		(gettext("Remove junction data"));
	gtk_grid_attach(editor->grid_junction,
		GTK_WIDGET(editor->button_remove_junction), 1, 0, 1, 1);
	g_signal_connect_swapped(editor->button_remove_junction, "clicked",
		(void(*))&editor_boundary_flow_remove_junction, editor);
	editor->label_junction = (GtkLabel*)gtk_label_new(gettext("Junction data"));
	gtk_grid_attach(editor->grid_junction, GTK_WIDGET(editor->label_junction),
		0, 1, 1, 1);
	editor->combo_junction = (GtkComboBoxText*)gtk_combo_box_text_new();
	gtk_grid_attach(editor->grid_junction, GTK_WIDGET(editor->combo_junction),
		1, 1, 1, 1);
	editor->id_junction = g_signal_connect_swapped(editor->combo_junction,
		"changed", (void(*))&editor_boundary_flow_open_junction, editor);
	gtk_grid_attach(editor->grid_junction,
		GTK_WIDGET(editor->editor_junction->frame), 0, 2, 2, 1);
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->grid_junction), 0, 12, 3, 1);
	editor->graphic = jbw_graphic_new(NULL, 6, 6, 0, &editor_draw);
	jbw_graphic_set_title(editor->graphic, gettext("Boundary condition"));
	jbw_graphic_set_logo(editor->graphic, "swigs.png");
	jbw_graphic_set_xlabel(editor->graphic, gettext("Time (s)"));
	gtk_widget_show_all(GTK_WIDGET(editor->grid));
	#if DEBUG_EDITOR_BOUNDARY_FLOW_NEW
		fprintf(stderr, "editor_boundary_flow_new: end\n");
	#endif
}

#if TEST_EDITOR_BOUNDARY_FLOW

void ok(char *name)
{
	xmlNode *node;
	xmlDoc *doc;
	if (!editor_boundary_flow_get(editor))
	{
		jbw_show_error(message);
		return;
	}
	doc = xmlNewDoc((const xmlChar*)"1.0");
	node = xmlNewDocNode(doc, 0, XML_BOUNDARY_FLOW, 0);
	xmlDocSetRootElement(doc, node);
	boundary_flow_save_xml(editor->bf, node);
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
	int nsections[2] = {3, 2};
	char *channel_name[2] = {"Channel1", "Channel2"};
	char *section_name1[3] = {"Section1", "Section2", "Section3"},
		*section_name2[2] = {"SectionA", "SectionB"};
	char **section_name[2] = {section_name1, section_name2};
	xmlKeepBlanksDefault(0);
	if (!jbw_graphic_init(&argn, &argc)) return 1;
	glutIdleFunc((void(*))&gtk_main_iteration);
	notebook = (GtkNotebook*)gtk_notebook_new();
	editor_boundary_flow_new(editor, notebook, 0, 1, channel_name, 2,
		section_name, nsections);
	doc = xmlParseFile(argc[1]);
	if (!doc) return 2;
	node = xmlDocGetRootElement(doc);
	if (!boundary_flow_open_xml(editor->bf, node, 0))
	{
		jbw_show_error(message);
		return 3;
	}
	xmlFreeDoc(doc);
	dlg = (GtkDialog*)gtk_dialog_new();
	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(dlg)),
		GTK_WIDGET(notebook));
	gtk_window_set_title(GTK_WINDOW(dlg),
		"Test editor flow boundary conditions");
	button_ok = (GtkButton*)gtk_dialog_add_button
		(dlg, gettext("_OK"), GTK_RESPONSE_OK);
	g_signal_connect_swapped(button_ok, "clicked", (void(*))&ok, argc[2]);
	button_cancel = (GtkButton*)gtk_dialog_add_button
		(dlg, gettext("_Cancel"), GTK_RESPONSE_CANCEL);
	g_signal_connect(button_cancel, "clicked", &glutLeaveMainLoop, NULL);
	gtk_widget_show_all(GTK_WIDGET(dlg));
	editor_boundary_flow_open(editor);
	glutMainLoop();
	editor_boundary_flow_destroy(editor);
	gtk_widget_destroy(GTK_WIDGET(dlg));
	return 0;
}

#endif
