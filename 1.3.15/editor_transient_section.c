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
 * \file editor_transient_section.c
 * \brief Source file to define transient section editor structures and
 *   functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2015 Javier Burguete Tolosa.
 */
#include "editor_transient_section.h"

#if TEST_EDITOR_TRANSIENT_SECTION
char *message;
EditorTransientSection editor[1];
void
editor_draw ()
{
  editor_transient_section_draw (editor);
}
#endif

/**
 * \fn void editor_transient_section_update(EditorTransientSection *editor)
 * \brief Function to update the view of a transient section editor.
 * \param editor
 * \brief transient section editor.
 */
void
editor_transient_section_update (EditorTransientSection * editor)
{
  int i;
#if DEBUG_EDITOR_TRANSIENT_SECTION_UPDATE
  fprintf (stderr, "editor_transient_section_update: start\n");
#endif
  i = jbw_array_radio_buttons_get_active (editor->array_type);
  if (i == TRANSIENT_SECTION_TYPE_STRAIGHT)
    jbw_array_editor_hide_column (editor->array, 0);
  else
    jbw_array_editor_show_column (editor->array, 0);
  gtk_widget_set_sensitive
    (GTK_WIDGET (editor->button_remove), editor->array->n > 2);
#if DEBUG_EDITOR_TRANSIENT_SECTION_UPDATE
  fprintf (stderr, "editor_transient_section_update: end\n");
#endif
}

/**
 * \fn int editor_transient_section_get(EditorTransientSection *editor)
 * \brief Function to get the actual transient section data from the editor.
 * \param editor
 * \brief transient section editor.
 * \return 1 on valid transient section, 0 on error.
 */
int
editor_transient_section_get (EditorTransientSection * editor)
{
  int i, j = editor->array->n;
  JBFLOAT x[j], y[j], z[j], r[j];
  TransientSection *ts = editor->ts;
#if DEBUG_EDITOR_TRANSIENT_SECTION_GET
  fprintf (stderr, "editor_transient_section_get: start\n");
#endif
  transient_section_delete (ts);
  if (!transient_section_create (ts,
                                 editor->array->n,
                                 jbw_array_radio_buttons_get_active (editor->
                                                                     array_type),
                                 (char *) gtk_entry_get_text (editor->
                                                              entry_name)))
    goto get_error;
  jbw_array_editor_get_column_float (editor->array, 1, y);
  jbw_array_editor_get_column_float (editor->array, 2, z);
  jbw_array_editor_get_column_float (editor->array, 3, r);
#if INTERFACE == INTERFACE_SCIENTIFIC
  int t[j];
  jbw_array_editor_get_column_int (editor->array, 4, t);
#endif
  if (ts->type == TRANSIENT_SECTION_TYPE_STRAIGHT)
    {
#if INTERFACE == INTERFACE_SCIENTIFIC
      for (i = j; --i >= 0;)
        section_point2_open (ts->sp + i, y[i], z[i], r[i], t[i]);
#else
      for (i = j; --i >= 0;)
        section_point2_open (ts->sp + i, y[i], z[i], r[i], 0);
#endif
    }
  else
    {
      jbw_array_editor_get_column_float (editor->array, 0, x);
#if INTERFACE == INTERFACE_SCIENTIFIC
      for (i = j; --i >= 0;)
        section_point3_open (TRANSIENT_SECTION_POINT3 (ts) + i,
                             x[i], y[i], z[i], r[i], t[i]);
#else
      for (i = j; --i >= 0;)
        section_point3_open (TRANSIENT_SECTION_POINT3 (ts) + i,
                             x[i], y[i], z[i], r[i], 0);
#endif
    }
  ts->n = --j;
  ts->t = jb_get_time (gtk_entry_get_text (editor->entry_t), &i);
  if (!i)
    {
      transient_section_error (ts, gettext ("Bad time"));
      goto get_error;
    }
  ts->u = gtk_spin_button_get_value (editor->entry_u);
  ts->contraction = gtk_spin_button_get_value (editor->entry_contraction);
  ts->dz = gtk_spin_button_get_value (editor->entry_dz);
  ts->zmin = jbm_varray_min (&(ts->sp->z), sizeof (SectionPoint2), j);
  ts->hmax = gtk_spin_button_get_value (editor->entry_hmax);
  if (ts->hmax == 0.)
    ts->hmax = fminl (ts->sp[0].z, ts->sp[ts->n].z) - ts->zmin;
  jbw_graphic_set_title (editor->graphic, editor->ts->name);
#if DEBUG_EDITOR_TRANSIENT_SECTION_GET
  transient_section_print (ts, stderr);
  fprintf (stderr, "editor_transient_section_get: end\n");
#endif
  return 1;

get_error:
#if DEBUG_EDITOR_TRANSIENT_SECTION_GET
  fprintf (stderr, "editor_transient_section_get: end\n");
#endif
  return 0;
}

/**
 * \fn void editor_transient_section_open(EditorTransientSection *editor)
 * \brief Function to open a transient section in the editor.
 * \param editor
 * \brief transient section editor.
 */
void
editor_transient_section_open (EditorTransientSection * editor)
{
  TransientSection *ts = editor->ts;
  int i, n = ts->n + 1;
  JBFLOAT x[n], y[n], z[n], r[n];
#if INTERFACE == INTERFACE_SCIENTIFIC
  unsigned int t[n];
#endif
#if DEBUG_EDITOR_TRANSIENT_SECTION_OPEN
  fprintf (stderr, "editor_transient_section_open: start\n");
  transient_section_print (ts, stderr);
#endif
  gtk_entry_set_text (editor->entry_name, ts->name);
  jbw_array_radio_buttons_set_active (editor->array_type, ts->type, 1);
  jbw_array_editor_set_rows (editor->array, n);
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
    }
  else
    {
      for (i = n; --i >= 0;)
        {
          x[i] = TRANSIENT_SECTION_POINT3 (ts)[i].x;
          y[i] = TRANSIENT_SECTION_POINT3 (ts)[i].y;
          z[i] = TRANSIENT_SECTION_POINT3 (ts)[i].z;
          r[i] = TRANSIENT_SECTION_POINT3 (ts)[i].r;
#if INTERFACE == INTERFACE_SCIENTIFIC
          t[i] = TRANSIENT_SECTION_POINT3 (ts)[i].t;
#endif
        }
      jbw_array_editor_set_column_float (editor->array, 0, x);
    }
  jbw_array_editor_set_column_float (editor->array, 1, y);
  jbw_array_editor_set_column_float (editor->array, 2, z);
  jbw_array_editor_set_column_float (editor->array, 3, r);
#if INTERFACE == INTERFACE_SCIENTIFIC
  jbw_array_editor_set_column_int (editor->array, 4, t);
#endif
  gtk_entry_set_text (editor->entry_t, jb_set_time (ts->t));
  gtk_spin_button_set_value (editor->entry_u, ts->u);
  gtk_spin_button_set_value (editor->entry_contraction, ts->contraction);
  gtk_spin_button_set_value (editor->entry_dz, ts->dz);
  if (ts->hmax == fminl (ts->sp[0].z, ts->sp[ts->n].z) - ts->zmin)
    gtk_spin_button_set_value (editor->entry_hmax, 0.);
  else
    gtk_spin_button_set_value (editor->entry_hmax, ts->hmax);
  editor_transient_section_update (editor);
#if DEBUG_EDITOR_TRANSIENT_SECTION_OPEN
  fprintf (stderr, "editor_transient_section_open: end\n");
#endif
}

/**
 * \fn void editor_transient_section_insert_point \
 *   (EditorTransientSection *editor)
 * \brief Function to insert a point in a transient section editor.
 * \param editor
 * \brief transient section editor.
 */
void
editor_transient_section_insert_point (EditorTransientSection * editor)
{
#if DEBUG_EDITOR_TRANSIENT_SECTION_INSERT_POINT
  fprintf (stderr, "editor_transient_section_insert_point: start\n");
#endif
  jbw_array_editor_insert (editor->array);
  editor_transient_section_update (editor);
#if DEBUG_EDITOR_TRANSIENT_SECTION_INSERT_POINT
  fprintf (stderr, "editor_transient_section_insert_point: end\n");
#endif
}

/**
 * \fn void editor_transient_section_remove_point \
 *   (EditorTransientSection *editor)
 * \brief Function to remove a point in a transient section editor.
 * \param editor
 * \brief transient section editor.
 */
void
editor_transient_section_remove_point (EditorTransientSection * editor)
{
#if DEBUG_EDITOR_TRANSIENT_SECTION_REMOVE_POINT
  fprintf (stderr, "editor_transient_section_remove_point: start\n");
#endif
  jbw_array_editor_remove (editor->array);
  editor_transient_section_update (editor);
#if DEBUG_EDITOR_TRANSIENT_SECTION_REMOVE_POINT
  fprintf (stderr, "editor_transient_section_remove_point: end\n");
#endif
}

/**
 * \fn void editor_transient_section_draw(EditorTransientSection *editor)
 * \brief Function to draw a transient section in a transient section editor.
 * \param editor
 * \brief transient section editor.
 */
void
editor_transient_section_draw (EditorTransientSection * editor)
{
#if DEBUG_EDITOR_TRANSIENT_SECTION_DRAW
  fprintf (stderr, "editor_transient_section_draw: start\n");
#endif
  editor_transient_section_get (editor);
  TransientSection *ts = editor->ts;
  int i, n = ts->n + 1;
  JBFLOAT y[n], z[n];
  for (i = n; --i >= 0;)
    y[i] = ts->sp[i].y, z[i] = ts->sp[i].z;
  jbw_graphic_draw_lines (editor->graphic, y, z, 0, 0, 0, n);
  jbw_graphic_draw_logo (editor->graphic);
#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
  glutSwapBuffers ();
#elif JBW_GRAPHIC == JBW_GRAPHIC_CLUTTER
  glFlush ();
#endif
#if DEBUG_EDITOR_TRANSIENT_SECTION_DRAW
  fprintf (stderr, "editor_transient_section_draw: end\n");
#endif
}

/**
 * \fn void editor_transient_section_destroy(EditorTransientSection *editor)
 * \brief Function to destroy a transient section editor.
 * \param editor
 * \brief transient section editor.
 */
void
editor_transient_section_destroy (EditorTransientSection * editor)
{
#if DEBUG_EDITOR_TRANSIENT_SECTION_DESTROY
  fprintf (stderr, "editor_transient_section_destroy: start\n");
#endif
  gtk_widget_destroy (GTK_WIDGET (editor->grid));
  transient_section_delete (editor->ts);
#if DEBUG_EDITOR_TRANSIENT_SECTION_DESTROY
  fprintf (stderr, "editor_transient_section_destroy: end\n");
#endif
}

/**
 * \fn void editor_transient_section_new(EditorTransientSection *editor, \
 *   GtkNotebook *notebook)
 * \brief Function to create a new transient section editor.
 * \param editor
 * \brief transient section editor.
 * \param notebook
 * \brief GtkNotebook to pack the widgets.
 */
void editor_transient_section_new
  (EditorTransientSection * editor, GtkNotebook * notebook)
{
  int i;
  const char *label_type[N_TRANSIENT_SECTION_TYPES] =
    { gettext ("Straight"), gettext ("Polilyne") };
  const char *label_array[5]
    = { "x", "y", "z", gettext ("Roughness"), gettext ("Type") };
#if DEBUG_EDITOR_TRANSIENT_SECTION_NEW
  fprintf (stderr, "editor_transient_section_new: start\n");
#endif
  transient_section_init_empty (editor->ts);
  editor->notebook = notebook;
  editor->grid = (GtkGrid *) gtk_grid_new ();
  gtk_notebook_append_page (notebook, GTK_WIDGET (editor->grid),
                            gtk_label_new (gettext ("Transient section")));
  editor->label_name = (GtkLabel *) gtk_label_new (gettext ("Name"));
  gtk_grid_attach (editor->grid, GTK_WIDGET (editor->label_name), 0, 0, 1, 1);
  editor->entry_name = (GtkEntry *) gtk_entry_new ();
  gtk_grid_attach (editor->grid, GTK_WIDGET (editor->entry_name), 1, 0, 2, 1);
  editor->label_t = (GtkLabel *) gtk_label_new (gettext ("Time"));
  gtk_grid_attach (editor->grid, GTK_WIDGET (editor->label_t), 0, 1, 1, 1);
  editor->entry_t = (GtkEntry *) gtk_entry_new ();
  gtk_grid_attach (editor->grid, GTK_WIDGET (editor->entry_t), 1, 1, 2, 1);
  editor->label_u = (GtkLabel *) gtk_label_new (gettext ("Velocity"));
  gtk_grid_attach (editor->grid, GTK_WIDGET (editor->label_u), 0, 2, 1, 1);
  editor->entry_u
    = (GtkSpinButton *) gtk_spin_button_new_with_range (-1000., 1000., 0.0001);
  gtk_grid_attach (editor->grid, GTK_WIDGET (editor->entry_u), 1, 2, 2, 1);
  editor->label_contraction
    = (GtkLabel *) gtk_label_new (gettext ("Contraction coefficient"));
  gtk_grid_attach
    (editor->grid, GTK_WIDGET (editor->label_contraction), 0, 3, 1, 1);
  editor->entry_contraction
    = (GtkSpinButton *) gtk_spin_button_new_with_range (0., 100., 0.001);
  gtk_grid_attach
    (editor->grid, GTK_WIDGET (editor->entry_contraction), 1, 3, 2, 1);
  editor->label_hmax = (GtkLabel *) gtk_label_new (gettext ("Maximum depth"));
  gtk_grid_attach (editor->grid, GTK_WIDGET (editor->label_hmax), 0, 4, 1, 1);
  editor->entry_hmax =
    (GtkSpinButton *) gtk_spin_button_new_with_range (-10000., 10000., 0.001);
  gtk_grid_attach (editor->grid, GTK_WIDGET (editor->entry_hmax), 1, 4, 2, 1);
  editor->label_dz
    = (GtkLabel *) gtk_label_new (gettext ("Depth accuracy for friction"));
  gtk_grid_attach (editor->grid, GTK_WIDGET (editor->label_dz), 0, 5, 1, 1);
  editor->entry_dz
    = (GtkSpinButton *) gtk_spin_button_new_with_range (0., 10., 0.0001);
  gtk_grid_attach (editor->grid, GTK_WIDGET (editor->entry_dz), 1, 5, 2, 1);
  editor->frame_type = (GtkFrame *) gtk_frame_new (gettext ("Type"));
  editor->grid_type = (GtkGrid *) gtk_grid_new ();
  editor->array_type[0] = NULL;
  for (i = 0; i < N_TRANSIENT_SECTION_TYPES; ++i)
    {
      editor->array_type[i]
        = (GtkRadioButton *) gtk_radio_button_new_with_label_from_widget
        (editor->array_type[0], label_type[i]);
      gtk_grid_attach
        (editor->grid_type, GTK_WIDGET (editor->array_type[i]), 0, i, 1, 1);
      g_signal_connect_swapped (editor->array_type[i], "clicked",
                                (void (*)) &editor_transient_section_update,
                                editor);
    }
  gtk_container_add
    (GTK_CONTAINER (editor->frame_type), GTK_WIDGET (editor->grid_type));
  gtk_grid_attach (editor->grid, GTK_WIDGET (editor->frame_type), 0, 6, 3, 1);
  editor->button_insert
    = (GtkButton *) gtk_button_new_with_label (gettext ("Insert point"));
  gtk_grid_attach
    (editor->grid, GTK_WIDGET (editor->button_insert), 0, 7, 1, 1);
  g_signal_connect_swapped (editor->button_insert, "clicked",
                            (void (*)) &editor_transient_section_insert_point,
                            editor);
  editor->button_remove =
    (GtkButton *) gtk_button_new_with_label (gettext ("Remove point"));
  gtk_grid_attach (editor->grid, GTK_WIDGET (editor->button_remove), 1, 7, 1,
                   1);
  g_signal_connect_swapped (editor->button_remove, "clicked",
                            (void (*)) &editor_transient_section_remove_point,
                            editor);
  editor->button_plot =
    (GtkButton *) gtk_button_new_with_label (gettext ("Update plot"));
  gtk_grid_attach (editor->grid, GTK_WIDGET (editor->button_plot), 2, 7, 1, 1);
  g_signal_connect (editor->button_plot, "clicked", &editor_draw, NULL);
  editor->array = jbw_array_editor_new (4, 3, 2, label_array);
  gtk_grid_attach
    (editor->grid, GTK_WIDGET (editor->array->scrolled), 0, 8, 3, 1);
  editor->graphic = jbw_graphic_new (NULL, 6, 6, 0, &editor_draw);
  jbw_graphic_set_logo (editor->graphic, "swigs.png");
  jbw_graphic_set_xlabel (editor->graphic, "y (m)");
  jbw_graphic_set_ylabel (editor->graphic, "z (m)");
  gtk_widget_show_all (GTK_WIDGET (editor->grid));
#if DEBUG_EDITOR_TRANSIENT_SECTION_NEW
  fprintf (stderr, "editor_transient_section_new: end\n");
#endif
}

#if TEST_EDITOR_TRANSIENT_SECTION

void
ok (char *name)
{
  xmlNode *node;
  xmlDoc *doc;
  if (!editor_transient_section_get (editor))
    {
      jbw_show_error (message);
      return;
    }
  doc = xmlNewDoc ((const xmlChar *) "1.0");
  node = xmlNewDocNode (doc, 0, XML_TRANSIENT_SECTION, 0);
  xmlDocSetRootElement (doc, node);
  transient_section_save_xml (editor->ts, node);
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
  xmlKeepBlanksDefault (0);
  if (!jbw_graphic_init (&argn, &argc))
    return 1;
  glutIdleFunc ((void (*)) &gtk_main_iteration);
  notebook = (GtkNotebook *) gtk_notebook_new ();
  gtk_notebook_set_tab_pos (notebook, GTK_POS_LEFT);
  editor_transient_section_new (editor, notebook);
  doc = xmlParseFile (argc[1]);
  if (!doc)
    return 2;
  node = xmlDocGetRootElement (doc);
  if (!transient_section_open_xml (editor->ts, node, 0., 0., 0.))
    {
      jbw_show_error (message);
      return 3;
    }
  xmlFreeDoc (doc);
  dlg = (GtkDialog *) gtk_dialog_new ();
  gtk_container_add (GTK_CONTAINER (gtk_dialog_get_content_area (dlg)),
                     GTK_WIDGET (notebook));
  gtk_window_set_title (GTK_WINDOW (dlg), "Test editor transient section");
  button_ok = (GtkButton *) gtk_dialog_add_button
    (dlg, gettext ("_OK"), GTK_RESPONSE_OK);
  g_signal_connect_swapped (button_ok, "clicked", (void (*)) &ok, argc[2]);
  button_cancel = (GtkButton *) gtk_dialog_add_button
    (dlg, gettext ("_Cancel"), GTK_RESPONSE_CANCEL);
  g_signal_connect (button_cancel, "clicked", &glutLeaveMainLoop, NULL);
  editor_transient_section_open (editor);
  gtk_widget_show_all (GTK_WIDGET (dlg));
  glutMainLoop ();
  editor_transient_section_destroy (editor);
  gtk_widget_destroy (GTK_WIDGET (dlg));
  return 0;
}

#endif
