/*
SWIGS (Shallow Water in Irregular Geometries Simulator): a software to simulate
initial or steady transports with solute transport in channels, channel networks
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
 * \file editor_boundary_transport.c
 * \brief Source file to define transported solute boundary conditions editor
 *   structures and functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2015 Javier Burguete Tolosa.
 */
#include "editor_boundary_transport.h"

#if TEST_EDITOR_BOUNDARY_TRANSPORT
char *message;
EditorBoundaryTransport editor[1];
void
editor_draw ()
{
  editor_boundary_transport_draw (editor);
}
#endif

/**
 * \fn void editor_boundary_transport_update(EditorBoundaryTransport *editor)
 * \brief Function to update the view of a transported solute boundary
 *   conditions editor.
 * \param editor
 * \brief transported solute boundary conditions editor.
 */
void
editor_boundary_transport_update (EditorBoundaryTransport * editor)
{
#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_UPDATE
  fprintf (stderr, "editor_boundary_transport_update: start\n");
#endif
  switch (jbw_array_radio_buttons_get_active (editor->array_type))
    {
    case BOUNDARY_TRANSPORT_TYPE_M:
      jbw_array_editor_set_rows (editor->array, 1);
      jbw_array_editor_set_title (editor->array, 1, gettext ("Solute mass"));
      jbw_graphic_set_ylabel (editor->graphic, gettext ("Solute mass"));
      gtk_widget_set_sensitive (GTK_WIDGET (editor->button_insert), 0);
      gtk_widget_set_sensitive (GTK_WIDGET (editor->button_remove), 0);
      break;
    default:
      jbw_array_editor_set_title
        (editor->array, 1, gettext ("Solute discharge"));
      jbw_graphic_set_ylabel (editor->graphic, gettext ("Solute discharge"));
      gtk_widget_set_sensitive (GTK_WIDGET (editor->button_insert), 1);
      gtk_widget_set_sensitive
        (GTK_WIDGET (editor->button_remove), editor->array->n > 1);
    }
#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_UPDATE
  fprintf (stderr, "editor_boundary_transport_update: end\n");
#endif
}

/**
 * \fn int editor_boundary_transport_get(EditorBoundaryTransport *editor)
 * \brief Function to get the actual transported solute boundary conditions data
 *   from the editor.
 * \param editor
 * \brief transported solute boundary conditions editor.
 * \return 1 on valid boundary condition, 0 on error.
 */
int
editor_boundary_transport_get (EditorBoundaryTransport * editor)
{
  int i, j, n = editor->array->n;
  JBDOUBLE t[n];
  JBFLOAT p[n];
  BoundaryTransport *bt = editor->bt;
#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_GET
  fprintf (stderr, "editor_boundary_transport_get: start\n");
#endif
  boundary_transport_delete (bt);
  bt->name = g_strdup (gtk_entry_get_text (editor->entry_name));
  if (!bt->name)
    {
      boundary_transport_error (bt, gettext ("Not enough memory"));
      goto error_get;
    }
  if (!strcmp (bt->name, ""))
    {
      boundary_transport_error (bt, gettext ("Not name"));
      goto error_get;
    }
  i = gtk_combo_box_get_active (GTK_COMBO_BOX (editor->combo_initial));
  j = gtk_combo_box_get_active (GTK_COMBO_BOX (editor->combo_final));
  if (j < i)
    {
      boundary_transport_error (bt, gettext ("Bad order"));
      goto error_get;
    }
  bt->section = gtk_combo_box_text_get_active_text (editor->combo_initial);
  if (!bt->section)
    {
      boundary_transport_error (bt, gettext ("Not enough memory"));
      goto error_get;
    }
  if (j > i)
    {
      bt->section2 = gtk_combo_box_text_get_active_text (editor->combo_final);
      if (!bt->section2)
        {
          boundary_transport_error (bt, gettext ("Not enough memory"));
          goto error_get;
        }
    }
  bt->type = jbw_array_radio_buttons_get_active (editor->array_type);
  bt->delay = gtk_spin_button_get_value (editor->entry_delay);
  bt->t = (JBDOUBLE *) jb_realloc (bt->t, n * sizeof (JBDOUBLE));
  bt->p = (JBFLOAT *) jb_realloc (bt->p, n * sizeof (JBFLOAT));
  if (!bt->t || !bt->p)
    {
      boundary_transport_error (bt, gettext ("Not enough memory"));
      goto error_get;
    }
  jbw_array_editor_get_column_time (editor->array, 0, t);
  jbw_array_editor_get_column_float (editor->array, 1, p);
  memcpy (bt->t, t, n * sizeof (JBDOUBLE));
  memcpy (bt->p, p, n * sizeof (JBFLOAT));
  bt->n = --n;
#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_GET
  boundary_transport_print (bt, stderr);
  fprintf (stderr, "editor_boundary_transport_get: end\n");
#endif
  return 1;

error_get:
#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_GET
  fprintf (stderr, "editor_boundary_transport_get: end\n");
#endif
  return 0;
}

/**
 * \fn void editor_boundary_transport_open(EditorBoundaryTransport *editor)
 * \brief Function to open the transported solute boundary conditions data in
 *   the editor.
 * \param editor
 * \brief transported solute boundary conditions editor.
 */
void
editor_boundary_transport_open (EditorBoundaryTransport * editor)
{
  BoundaryTransport *bt = editor->bt;
#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_OPEN
  fprintf (stderr, "editor_boundary_transport_open: start\n");
#endif
  gtk_entry_set_text (editor->entry_name, bt->name);
  jbw_array_radio_buttons_set_active (editor->array_type, bt->type, 1);
  gtk_spin_button_set_value (editor->entry_delay, bt->delay);
  jbw_array_editor_set_rows (editor->array, bt->n + 1);
  jbw_array_editor_set_column_time (editor->array, 0, bt->t);
  jbw_array_editor_set_column_float (editor->array, 1, bt->p);
  editor_boundary_transport_update (editor);
#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_OPEN
  fprintf (stderr, "editor_boundary_transport_open: end\n");
#endif
}

/**
 * \fn void editor_boundary_transport_insert_point \
 *   (EditorBoundaryTransport *editor)
 * \brief Function to insert a point in a transported solute boundary
 *   conditions editor.
 * \param editor
 * \brief transported solute boundary conditions editor.
 */
void
editor_boundary_transport_insert_point (EditorBoundaryTransport * editor)
{
#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_INSERT_POINT
  fprintf (stderr, "editor_boundary_transport_insert_point: start\n");
#endif
  jbw_array_editor_insert (editor->array);
  editor_boundary_transport_update (editor);
#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_INSERT_POINT
  fprintf (stderr, "editor_boundary_transport_insert_point: end\n");
#endif
}

/**
 * \fn void editor_boundary_transport_remove_point \
 *   (EditorBoundaryTransport *editor)
 * \brief Function to remove a point in a transported solute boundary
 *   conditions editor.
 * \param editor
 * \brief transported solute boundary conditions editor.
 */
void
editor_boundary_transport_remove_point (EditorBoundaryTransport * editor)
{
#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_REMOVE_POINT
  fprintf (stderr, "editor_boundary_transport_remove_point: start\n");
#endif
  jbw_array_editor_remove (editor->array);
  editor_boundary_transport_update (editor);
#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_REMOVE_POINT
  fprintf (stderr, "editor_boundary_transport_remove_point: end\n");
#endif
}

/**
 * \fn void editor_boundary_transport_draw(EditorBoundaryTransport *editor)
 * \brief Function to draw a transported solute boundary conditions in a
 *   transported solute boundary conditions editor.
 * \param editor
 * \brief transported solute boundary conditions editor.
 */
void
editor_boundary_transport_draw (EditorBoundaryTransport * editor)
{
#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_DRAW
  fprintf (stderr, "editor_boundary_transport_draw: start\n");
#endif
  editor_boundary_transport_get (editor);
  BoundaryTransport *bt = editor->bt;
  int i, n = bt->n + 1;
  JBFLOAT t[n], p[n];
  for (i = 0; i < n; ++i)
    t[i] = bt->t[i];
  memcpy (p, bt->p, n * sizeof (JBFLOAT));
  jbw_graphic_draw_lines (editor->graphic, t, p, 0, 0, 0, n);
  jbw_graphic_draw_logo (editor->graphic);
#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
  glutSwapBuffers ();
#elif JBW_GRAPHIC == JBW_GRAPHIC_CLUTTER
  glFlush ();
#endif
#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_DRAW
  fprintf (stderr, "editor_boundary_transport_draw: end\n");
#endif
}

/**
 * \fn void editor_boundary_transport_destroy(EditorBoundaryTransport *editor)
 * \brief Function to destroy a transported solute boundary conditions editor.
 * \param editor
 * \brief transported solute boundary conditions editor.
 */
void
editor_boundary_transport_destroy (EditorBoundaryTransport * editor)
{
#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_DESTROY
  fprintf (stderr, "editor_boundary_transport_destroy: start\n");
#endif
  gtk_widget_destroy (GTK_WIDGET (editor->grid));
  boundary_transport_delete (editor->bt);
#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_DESTROY
  fprintf (stderr, "editor_boundary_transport_destroy: end\n");
#endif
}

/**
 * \fn void editor_boundary_transport_new(EditorBoundaryTransport *editor, \
 *   GtkNotebook *notebook, char **section_name, int nsections)
 * \brief Function to create a new transported solute boundary conditions
 *   editor.
 * \param editor
 * \brief transported solute boundary conditions editor.
 * \param notebook
 * \brief GtkNotebook to pack the widgets.
 * \param section_name
 * \brief array of cross section names.
 * \param nsections
 * \brief number of cross sections.
 */
void
editor_boundary_transport_new (EditorBoundaryTransport * editor,
                               GtkNotebook * notebook, char **section_name,
                               int nsections)
{
  int i;
  const char *label_type[N_BOUNDARY_TRANSPORT_TYPES] =
    { gettext ("Mass"), gettext ("Discharge") };
  const char *label_array[2] = { gettext ("Time"), gettext ("Mass") };
#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_NEW
  fprintf (stderr, "editor_boundary_transport_new: start\n");
#endif
  boundary_transport_init_empty (editor->bt);
  editor->notebook = notebook;
  editor->grid = (GtkGrid *) gtk_grid_new ();
  gtk_notebook_append_page (notebook, GTK_WIDGET (editor->grid),
                            gtk_label_new (gettext ("Boundary condition")));
  editor->label_name = (GtkLabel *) gtk_label_new (gettext ("Name"));
  gtk_grid_attach (editor->grid, GTK_WIDGET (editor->label_name), 0, 0, 1, 1);
  editor->entry_name = (GtkEntry *) gtk_entry_new ();
  gtk_grid_attach (editor->grid, GTK_WIDGET (editor->entry_name), 1, 0, 2, 1);
  editor->label_initial
    = (GtkLabel *) gtk_label_new (gettext ("Initial cross section"));
  gtk_grid_attach
    (editor->grid, GTK_WIDGET (editor->label_initial), 0, 1, 1, 1);
  editor->combo_initial
    = jbw_combo_box_new_with_strings (section_name, nsections);
  gtk_grid_attach
    (editor->grid, GTK_WIDGET (editor->combo_initial), 1, 1, 2, 1);
  editor->label_final
    = (GtkLabel *) gtk_label_new (gettext ("Final cross section"));
  gtk_grid_attach (editor->grid, GTK_WIDGET (editor->label_final), 0, 2, 1, 1);
  editor->combo_final
    = jbw_combo_box_new_with_strings (section_name, nsections);
  gtk_grid_attach (editor->grid, GTK_WIDGET (editor->combo_final), 1, 2, 2, 1);
  editor->label_delay = (GtkLabel *) gtk_label_new (gettext ("Delay"));
  gtk_grid_attach (editor->grid, GTK_WIDGET (editor->label_delay), 0, 3, 1, 1);
  editor->entry_delay
    = (GtkSpinButton *) gtk_spin_button_new_with_range (0., 86400., 0.01);
  gtk_grid_attach (editor->grid, GTK_WIDGET (editor->entry_delay), 1, 3, 2, 1);
  editor->frame_type = (GtkFrame *) gtk_frame_new (gettext ("Type"));
  editor->grid_type = (GtkGrid *) gtk_grid_new ();
  editor->array_type[0] = NULL;
  for (i = 0; i < N_BOUNDARY_TRANSPORT_TYPES; ++i)
    {
      editor->array_type[i]
        = (GtkRadioButton *) gtk_radio_button_new_with_label_from_widget
        (editor->array_type[0], label_type[i]);
      gtk_grid_attach
        (editor->grid_type, GTK_WIDGET (editor->array_type[i]), 0, i, 1, 1);
      g_signal_connect_swapped (editor->array_type[i], "clicked",
                                (void (*)) &editor_boundary_transport_update,
                                editor);
    }
  gtk_container_add
    (GTK_CONTAINER (editor->frame_type), GTK_WIDGET (editor->grid_type));
  gtk_grid_attach (editor->grid, GTK_WIDGET (editor->frame_type), 0, 4, 3, 1);
  editor->button_insert
    = (GtkButton *) gtk_button_new_with_label (gettext ("Insert point"));
  gtk_grid_attach
    (editor->grid, GTK_WIDGET (editor->button_insert), 0, 5, 1, 1);
  g_signal_connect_swapped (editor->button_insert, "clicked",
                            (void (*)) &editor_boundary_transport_insert_point,
                            editor);
  editor->button_remove =
    (GtkButton *) gtk_button_new_with_label (gettext ("Remove point"));
  gtk_grid_attach (editor->grid, GTK_WIDGET (editor->button_remove), 1, 5, 1,
                   1);
  g_signal_connect_swapped (editor->button_remove, "clicked",
                            (void (*)) &editor_boundary_transport_remove_point,
                            editor);
  editor->button_plot =
    (GtkButton *) gtk_button_new_with_label (gettext ("Update plot"));
  gtk_grid_attach (editor->grid, GTK_WIDGET (editor->button_plot), 2, 5, 1, 1);
  g_signal_connect (editor->button_plot, "clicked", &editor_draw, NULL);
  editor->array = jbw_array_editor_new (2, 2, 1, label_array);
  gtk_grid_attach
    (editor->grid, GTK_WIDGET (editor->array->scrolled), 0, 6, 3, 1);
  editor->graphic = jbw_graphic_new (NULL, 6, 6, 0, &editor_draw);
  jbw_graphic_set_title (editor->graphic, gettext ("Boundary condition"));
  jbw_graphic_set_logo (editor->graphic, "swigs.png");
  jbw_graphic_set_xlabel (editor->graphic, gettext ("Time (s)"));
  gtk_widget_show_all (GTK_WIDGET (editor->grid));
#if DEBUG_EDITOR_BOUNDARY_TRANSPORT_NEW
  fprintf (stderr, "editor_boundary_transport_new: end\n");
#endif
}

#if TEST_EDITOR_BOUNDARY_TRANSPORT

void
ok (char *name)
{
  xmlNode *node;
  xmlDoc *doc;
  if (!editor_boundary_transport_get (editor))
    {
      jbw_show_error (message);
      return;
    }
  doc = xmlNewDoc ((const xmlChar *) "1.0");
  node = xmlNewDocNode (doc, 0, XML_BOUNDARY_TRANSPORT, 0);
  xmlDocSetRootElement (doc, node);
  boundary_transport_save_xml (editor->bt, node);
  xmlSaveFormatFile (name, doc, 1);
  xmlFree (doc);
  glutLeaveMainLoop ();
}

int
main (int argn, char **argc)
{
  xmlNode *node;
  xmlDoc *doc;
  GtkNotebook *notebook;
  GtkButton *button_ok, *button_cancel;
  GtkDialog *dlg;
  char *section_name[] = { "Sec1", "Sec2", "Sec3" };
  xmlKeepBlanksDefault (0);
  if (!jbw_graphic_init (&argn, &argc))
    return 1;
  glutIdleFunc ((void (*)) &gtk_main_iteration);
  notebook = (GtkNotebook *) gtk_notebook_new ();
  editor_boundary_transport_new (editor, notebook, section_name, 3);
  doc = xmlParseFile (argc[1]);
  if (!doc)
    return 2;
  node = xmlDocGetRootElement (doc);
  if (!boundary_transport_open_xml (editor->bt, node))
    {
      jbw_show_error (message);
      return 3;
    }
  xmlFreeDoc (doc);
  dlg = (GtkDialog *) gtk_dialog_new ();
  gtk_container_add (GTK_CONTAINER (gtk_dialog_get_content_area (dlg)),
                     GTK_WIDGET (notebook));
  gtk_window_set_title (GTK_WINDOW (dlg),
                        "Test editor transported solute boundary conditions");
  button_ok = (GtkButton *) gtk_dialog_add_button
    (dlg, gettext ("_OK"), GTK_RESPONSE_OK);
  g_signal_connect_swapped (button_ok, "clicked", (void (*)) &ok, argc[2]);
  button_cancel = (GtkButton *) gtk_dialog_add_button
    (dlg, gettext ("_Cancel"), GTK_RESPONSE_CANCEL);
  g_signal_connect (button_cancel, "clicked", &glutLeaveMainLoop, NULL);
  gtk_widget_show_all (GTK_WIDGET (dlg));
  editor_boundary_transport_open (editor);
  glutMainLoop ();
  editor_boundary_transport_destroy (editor);
  gtk_widget_destroy (GTK_WIDGET (dlg));
  return 0;
}

#endif
