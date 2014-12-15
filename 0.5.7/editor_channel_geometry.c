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
 * \file editor_channel_geometry.c
 * \brief Source file to define channel geometry editor structures and methods.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2014 Javier Burguete Tolosa.
 */
#include "editor_channel_geometry.h"

/**
 * \def EDITOR_CHANNEL_GEOMETRY
 * \brief Macro to get the position of the channel geometry editor in the
 *   notebook.
 * \def EDITOR_CROSS_SECTION
 * \brief Macro to get the position of the cross section editor in the notebook.
 * \def EDITOR_TRANSIENT_SECTION
 * \brief Macro to get the position of the transient section editor in the
 *   notebook.
 */
#if TEST_EDITOR_CHANNEL_GEOMETRY
	#define EDITOR_CHANNEL_GEOMETRY 0
	char *message;
	EditorChannelGeometry editor[1];
#else
	#define EDITOR_CHANNEL_GEOMETRY 1
#endif
#define EDITOR_CROSS_SECTION (EDITOR_CHANNEL_GEOMETRY + 1)
#define EDITOR_TRANSIENT_SECTION (EDITOR_CROSS_SECTION + 1)

/**
 * \fn void editor_channel_geometry_update(EditorChannelGeometry *editor)
 * \brief Function to update the view of a channel geometry editor.
 * \param editor
 * \brief channel geometry editor.
 */
void editor_channel_geometry_update(EditorChannelGeometry *editor)
{
	#if DEBUG_EDITOR_CHANNEL_GEOMETRY_UPDATE
		fprintf(stderr, "editor_channel_geometry_update: start\n");
	#endif
	gtk_widget_set_sensitive
		(GTK_WIDGET(editor->button_remove), editor->cg->n > 0);
	#if DEBUG_EDITOR_CHANNEL_GEOMETRY_UPDATE
		fprintf(stderr, "editor_channel_geometry_update: end\n");
	#endif
}

/**
 * \fn void editor_channel_geometry_get_section(EditorChannelGeometry *editor)
 * \brief Function to get the actual cross section data from the editor.
 * \param editor
 * \brief channel geometry editor.
 */
void editor_channel_geometry_get_section(EditorChannelGeometry *editor)
{
	int i;
	CrossSection *cs;
	#if DEBUG_EDITOR_CHANNEL_GEOMETRY_GET_SECTION
		fprintf(stderr, "editor_channel_geometry_get_section: start\n");
	#endif
	i = gtk_combo_box_get_active(GTK_COMBO_BOX(editor->combo_section));
	cs = editor->cg->cs + i;
	cross_section_delete(cs);
	editor_cross_section_get(editor->editor_section);
	cross_section_copy(cs, editor->editor_section->cs);
	#if DEBUG_EDITOR_CHANNEL_GEOMETRY_GET_SECTION
		fprintf(stderr, "editor_channel_geometry_get_section: end\n");
	#endif
}

/**
 * \fn void editor_channel_geometry_open_section(EditorChannelGeometry *editor)
 * \brief Function to open a cross section in the editor.
 * \param editor
 * \brief channel geometry editor.
 */
void editor_channel_geometry_open_section(EditorChannelGeometry *editor)
{
	int i;
	#if DEBUG_EDITOR_CHANNEL_GEOMETRY_OPEN_SECTION
		fprintf(stderr, "editor_channel_geometry_open_section: start\n");
	#endif
	i = gtk_combo_box_get_active(GTK_COMBO_BOX(editor->combo_section));
	cross_section_delete(editor->editor_section->cs);
	cross_section_copy(editor->editor_section->cs, editor->cg->cs + i);
	editor_cross_section_open(editor->editor_section);
	#if DEBUG_EDITOR_CHANNEL_GEOMETRY_OPEN_SECTION
		fprintf(stderr, "editor_channel_geometry_open_section: end\n");
	#endif
}

/**
 * \fn void editor_channel_geometry_open(EditorChannelGeometry *editor)
 * \brief Function to open a channel geometry in the editor.
 * \param editor
 * \brief channel geometry editor.
 */
void editor_channel_geometry_open(EditorChannelGeometry *editor)
{
	int i;
	ChannelGeometry *cg;
	#if DEBUG_EDITOR_CHANNEL_GEOMETRY_OPEN
		fprintf(stderr, "editor_channel_geometry_open: start\n");
	#endif
	cg = editor->cg;
	g_signal_handler_block(editor->combo_section, editor->id_section);
	gtk_combo_box_text_remove_all(editor->combo_section);
	for (i = 0; i <= cg->n; ++i)
		gtk_combo_box_text_append_text(editor->combo_section, cg->cs[i].name);
	g_signal_handler_unblock(editor->combo_section, editor->id_section);
	gtk_combo_box_set_active(GTK_COMBO_BOX(editor->combo_section), 0);
	editor_channel_geometry_update(editor);
	#if DEBUG_EDITOR_CHANNEL_GEOMETRY_OPEN
		fprintf(stderr, "editor_channel_geometry_open: end\n");
	#endif
}

/**
 * \fn void editor_channel_geometry_insert_section\
 *   (EditorChannelGeometry *editor)
 * \brief Function to insert a cross section in the actual channel geometry data
 *   of the editor.
 * \param editor
 * \brief channel geometry editor.
 */
void editor_channel_geometry_insert_section(EditorChannelGeometry *editor)
{
	int i;
	CrossSection *cs;
	EditorCrossSection *editor_section;
	#if DEBUG_EDITOR_CHANNEL_GEOMETRY_INSERT_SECTION
		fprintf(stderr, "editor_channel_geometry_insert_section: start\n");
	#endif
	i = 1 + gtk_combo_box_get_active(GTK_COMBO_BOX(editor->combo_section));
	channel_geometry_insert_section(editor->cg, i);
	cs = editor->cg->cs + i;
	gtk_combo_box_text_insert_text(editor->combo_section, i, cs->name);
	editor_channel_geometry_update(editor);
	editor_section = editor->editor_section;
	cross_section_delete(editor_section->cs);
	cross_section_copy(editor_section->cs, cs);
	editor_cross_section_open(editor_section);
	gtk_combo_box_set_active(GTK_COMBO_BOX(editor->combo_section), i);
	gtk_notebook_set_current_page(editor->notebook, EDITOR_CROSS_SECTION);
	#if DEBUG_EDITOR_CHANNEL_GEOMETRY_INSERT_SECTION
		fprintf(stderr, "editor_channel_geometry_insert_section: end\n");
	#endif
}

/**
 * \fn void editor_channel_geometry_remove_section\
 *   (EditorChannelGeometry *editor)
 * \brief Function to remove a cross section in the actual channel geometry data
 *   of the editor.
 * \param editor
 * \brief channel geometry editor.
 */
void editor_channel_geometry_remove_section(EditorChannelGeometry *editor)
{
	int i;
	#if DEBUG_EDITOR_CHANNEL_GEOMETRY_REMOVE_SECTION
		fprintf(stderr, "editor_channel_geometry_remove_section: start\n");
	#endif
	i = gtk_combo_box_get_active(GTK_COMBO_BOX(editor->combo_section));
	channel_geometry_remove_section(editor->cg, i);
	g_signal_handler_block(editor->combo_section, editor->id_section);
	gtk_combo_box_text_remove(editor->combo_section, i);
	g_signal_handler_unblock(editor->combo_section, editor->id_section);
	gtk_combo_box_set_active(GTK_COMBO_BOX(editor->combo_section),
		jbm_min(i, editor->cg->n));
	editor_channel_geometry_update(editor);
	#if DEBUG_EDITOR_CHANNEL_GEOMETRY_REMOVE_SECTION
		fprintf(stderr, "editor_channel_geometry_remove_section: end\n");
	#endif
}

/**
 * \fn void editor_channel_geometry_draw(EditorChannelGeometry *editor)
 * \brief Function to draw a channel geometry in a channel geometry editor.
 * \param editor
 * \brief channel geometry editor.
 */
void editor_channel_geometry_draw(EditorChannelGeometry *editor)
{
	int i, j, n = editor->cg->n;
	TransientSection *ts;
	CrossSection *cs;
	JBDOUBLE k1, k2, xmax, xmin;
	JBWGraphic *graphic;
	JBFLOAT x[n + n + 2], y[n + n + 2], x2[n + 1], y2[n + 1], *xx, *yy;

	#if DEBUG_EDITOR_CHANNEL_GEOMETRY_DRAW
		fprintf(stderr,"editor_channel_geometry_draw: start\n");
	#endif

	graphic = editor->graphic;
	for (i = 0, cs = editor->cg->cs, xx = x, yy = y; i <= n;
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
	#if DEBUG_EDITOR_CHANNEL_GEOMETRY_DRAW
		fprintf(stderr, "ECGD: xmin="FWL" xmax="FWL" ymin="FWL" ymax="FWL"\n",
			graphic->xmin, graphic->xmax, graphic->ymin, graphic->ymax);
	#endif
	jbw_graphic_labels(graphic);
	#if DEBUG_EDITOR_CHANNEL_GEOMETRY_DRAW
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
	#if DEBUG_EDITOR_CHANNEL_GEOMETRY_DRAW
		fprintf(stderr,"editor_channel_geometry_draw: end\n");
	#endif
}

/**
 * \fn void editor_channel_geometry_destroy(EditorChannelGeometry *editor)
 * \brief Function to destroy a channel geometry editor.
 * \param editor
 * \brief channel geometry editor.
 */
void editor_channel_geometry_destroy(EditorChannelGeometry *editor)
{
	#if DEBUG_EDITOR_CHANNEL_GEOMETRY_DESTROY
		fprintf(stderr, "editor_channel_geometry_destroy: start\n");
	#endif
	editor_cross_section_destroy(editor->editor_section);
	gtk_widget_destroy(GTK_WIDGET(editor->grid));
	channel_geometry_delete(editor->cg);
	#if DEBUG_EDITOR_CHANNEL_GEOMETRY_DESTROY
		fprintf(stderr, "editor_channel_geometry_destroy: end\n");
	#endif
}

/**
 * \fn void editor_channel_geometry_new(EditorChannelGeometry *editor, \
 *   GtkNotebook *notebook, char **channel_name, int nchannels, \
 *   char ***section_name, int *nsections)
 * \brief Function to create a new channel geometry editor.
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
void editor_channel_geometry_new(EditorChannelGeometry *editor,
	GtkNotebook *notebook, char **channel_name, int nchannels,
	char ***section_name, int *nsections)
{
	#if DEBUG_EDITOR_CHANNEL_GEOMETRY_NEW
		fprintf(stderr, "editor_channel_geometry_new: start\n");
	#endif
	editor->notebook = notebook;
	editor->grid = (GtkGrid*)gtk_grid_new();
	gtk_notebook_append_page(notebook, GTK_WIDGET(editor->grid),
		gtk_label_new(gettext("Geometry")));
	editor_cross_section_new(editor->editor_section, notebook, channel_name,
		nchannels, section_name, nsections);
	editor->button_insert = (GtkButton*)gtk_button_new_with_label
		(gettext("Insert cross section"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->button_insert), 0, 5, 1, 1);
	g_signal_connect_swapped(editor->button_insert, "clicked",
		(void(*))&editor_channel_geometry_insert_section, editor);
	editor->button_remove = (GtkButton*)gtk_button_new_with_label
		(gettext("Remove cross section"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->button_remove), 1, 5, 1, 1);
	g_signal_connect_swapped(editor->button_remove, "clicked",
		(void(*))&editor_channel_geometry_remove_section, editor);
	editor->button_plot
		= (GtkButton*)gtk_button_new_with_label(gettext("Update plot"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->button_plot), 2, 5, 1, 1);
	g_signal_connect(editor->button_plot, "clicked", &editor_draw, NULL);
	editor->label_section
		= (GtkLabel*)gtk_label_new(gettext("Cross section"));
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->label_section), 0, 6, 1, 1);
	editor->combo_section = (GtkComboBoxText*)gtk_combo_box_text_new();
	gtk_grid_attach
		(editor->grid, GTK_WIDGET(editor->combo_section), 1, 6, 2, 1);
	editor->id_section = g_signal_connect_swapped(editor->combo_section,
		"changed", (void(*))&editor_channel_geometry_open_section, editor);
	editor->graphic = jbw_graphic_new(NULL, 6, 6, 0, &editor_draw);
	jbw_graphic_set_title(editor->graphic, gettext("Channel geometry"));
	jbw_graphic_set_logo(editor->graphic, "swigs.png");
	jbw_graphic_set_xlabel(editor->graphic, "x (m)");
	jbw_graphic_set_ylabel(editor->graphic, "y (m)");
	editor->graphic->map = 1;
	#if DEBUG_EDITOR_CHANNEL_GEOMETRY_NEW
		fprintf(stderr, "editor_channel_geometry_new: end\n");
	#endif
}

#if TEST_EDITOR_CHANNEL_GEOMETRY

void editor_draw()
{
	switch (gtk_notebook_get_current_page(editor->notebook))
	{
		case EDITOR_CHANNEL_GEOMETRY:
			editor_channel_geometry_draw(editor);
			break;
		case EDITOR_CROSS_SECTION:
			editor_cross_section_draw(editor->editor_section);
			break;
		case EDITOR_TRANSIENT_SECTION:
			editor_transient_section_draw
				(editor->editor_section->editor_transient);
			break;
	}
}

void change_current_page(EditorChannelGeometry *editor)
{
	int i;
	CrossSection *cs;
	TransientSection *ts;
	EditorCrossSection *editor_section;
	EditorTransientSection *editor_transient;
	switch (gtk_notebook_get_current_page(editor->notebook))
	{
		case EDITOR_CROSS_SECTION:
			editor_section = editor->editor_section;
			editor_cross_section_get(editor_section);
			i = gtk_combo_box_get_active(GTK_COMBO_BOX(editor->combo_section));
			cs = editor->cg->cs + i;
			cross_section_delete(cs);
			cross_section_copy(cs, editor_section->cs);
			editor_channel_geometry_open(editor);
			gtk_combo_box_set_active(GTK_COMBO_BOX(editor->combo_section), i);
			break;
		case EDITOR_TRANSIENT_SECTION:
			editor_section = editor->editor_section;
			editor_transient = editor_section->editor_transient;
			editor_transient_section_get(editor_transient);
			i = gtk_combo_box_get_active
				(GTK_COMBO_BOX(editor_section->combo_transient));
			ts = editor_section->cs->ts + i;
			transient_section_delete(ts);
			transient_section_copy(ts, editor_transient->ts);
			editor_cross_section_get(editor_section);
			cs = editor->cg->cs + gtk_combo_box_get_active
				(GTK_COMBO_BOX(editor->combo_section));
			cross_section_delete(cs);
			cross_section_copy(cs, editor_section->cs);
			editor_cross_section_open(editor_section);
			gtk_combo_box_set_active
				(GTK_COMBO_BOX(editor_section->combo_transient), i);
			break;
	}
}

void ok(char *name)
{
	xmlNode *node;
	xmlDoc *doc;
	doc = xmlNewDoc((const xmlChar*)"1.0");
	node = xmlNewDocNode(doc, 0, XML_CHANNEL_GEOMETRY, 0);
	xmlDocSetRootElement(doc, node);
	channel_geometry_save_xml(editor->cg, node);
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
	ChannelGeometry *cg;
	cg = editor->cg;
	cg->n = -1;
	cg->x = NULL;
	cg->cs = NULL;
	xmlKeepBlanksDefault(0);
	if (!jbw_graphic_init(&argn, &argc)) return 1;
	glutIdleFunc((void(*))&gtk_main_iteration);
	notebook = (GtkNotebook*)gtk_notebook_new();
	g_signal_connect_swapped(notebook, "switch-page",
		(void(*))&change_current_page, editor);
	g_signal_connect_after(notebook, "switch-page", &editor_draw, NULL);
	editor_channel_geometry_new
		(editor, notebook, channel_name, 2, section_name, nsections);
	doc = xmlParseFile(argc[1]);
	if (!doc) return 2;
	node = xmlDocGetRootElement(doc);
	if (!channel_geometry_open_xml(editor->cg, node)) return 3;
	xmlFreeDoc(doc);
	dlg = (GtkDialog*)gtk_dialog_new();
	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(dlg)),
		GTK_WIDGET(notebook));
	gtk_window_set_title(GTK_WINDOW(dlg), "Test editor channel geometry");
	button_ok = (GtkButton*)gtk_dialog_add_button
		(dlg, gettext("_OK"), GTK_RESPONSE_OK);
	g_signal_connect_swapped(button_ok, "clicked", (void(*))&ok, argc[2]);
	button_cancel = (GtkButton*)gtk_dialog_add_button
		(dlg, gettext("_Cancel"), GTK_RESPONSE_CANCEL);
	g_signal_connect(button_cancel, "clicked", &glutLeaveMainLoop, NULL);
	gtk_widget_show_all(GTK_WIDGET(dlg));
	editor_channel_geometry_open(editor);
	glutMainLoop();
	editor_channel_geometry_destroy(editor);
	gtk_widget_destroy(GTK_WIDGET(dlg));
	return 0;
}

#endif
