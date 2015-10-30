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

#include "write.h"
#include "dialog_simulator.h"
#include "dialog_save_solution.h"

void
dialog_save_solution_new ()
{
  int i, j;
  char *name, *string_profiles, *string_channel[sys->n + 1];
  JBFLOAT t;
  FILE *file_read, *file_write;
  DialogSaveSolution dlg;

  file_read = g_fopen (sys->solution_path, "rb");
  if (!file_read)
    {
      jbw_show_error2
        (gettext ("Solution"), gettext ("Unable to open the file"));
      return;
    }
  j = vsize * (n + 1);
  j = jb_flength (file_read) / j;
  string_profiles = (char *) g_malloc (j * 32 * sizeof (char));

  for (i = 0; i <= sys->n; ++i)
    string_channel[i] = sys->channel[i].name;

  dlg.widget_file = (GtkFileChooserWidget *) gtk_file_chooser_widget_new
    (GTK_FILE_CHOOSER_ACTION_SAVE);
  gtk_file_chooser_set_current_folder
    (GTK_FILE_CHOOSER (dlg.widget_file), sys->directory);
  gtk_file_chooser_set_do_overwrite_confirmation
    (GTK_FILE_CHOOSER (dlg.widget_file), 1);

  dlg.combo_channel =
    jbw_combo_box_new_with_strings (string_channel, sys->n + 1);
  gtk_combo_box_set_active (GTK_COMBO_BOX (dlg.combo_channel), channel_write);

  dlg.combo_profile = (GtkComboBoxText *) gtk_combo_box_text_new ();
  for (i = 0; i < j; ++i)
    {
      t = fmin (i * ti, tf - t0);
      snprintf (string_profiles + 32 * i, 32, FGF, t);
      gtk_combo_box_text_append_text
        (dlg.combo_profile, string_profiles + 32 * i);
    }
  gtk_combo_box_set_active (GTK_COMBO_BOX (dlg.combo_profile), 0);

  dlg.combo_evolution = (GtkComboBoxText *) gtk_combo_box_text_new ();
  for (i = 0; i <= sys->channel[channel_write].cg->n; ++i)
    gtk_combo_box_text_append_text
      (dlg.combo_evolution, sys->channel[channel_write].cg->cs[i].name);
  gtk_combo_box_set_active (GTK_COMBO_BOX (dlg.combo_evolution), 0);

  dlg.label_time = (GtkLabel *) gtk_label_new (gettext ("Time"));

  dlg.box_profile = (GtkGrid *) gtk_grid_new ();
  gtk_grid_attach (dlg.box_profile, GTK_WIDGET (dlg.label_time), 0, 0, 1, 1);
  gtk_grid_attach (dlg.box_profile, GTK_WIDGET (dlg.combo_profile), 0, 1, 1, 1);

  dlg.label_section = (GtkLabel *) gtk_label_new (gettext ("Cross section"));

  dlg.box_evolution = (GtkGrid *) gtk_grid_new ();
  gtk_grid_attach (dlg.box_evolution, GTK_WIDGET (dlg.label_section),
                   0, 0, 1, 1);
  gtk_grid_attach (dlg.box_evolution, GTK_WIDGET (dlg.combo_evolution),
                   0, 1, 1, 1);

  dlg.label_profile
    = (GtkLabel *) gtk_label_new (gettext ("Longitudinal profile"));
  dlg.label_evolution = (GtkLabel *) gtk_label_new (gettext ("Time evolution"));

  dlg.notebook = (GtkNotebook *) gtk_notebook_new ();
  gtk_notebook_append_page (dlg.notebook, GTK_WIDGET (dlg.box_profile),
                            GTK_WIDGET (dlg.label_profile));
  gtk_notebook_append_page (dlg.notebook, GTK_WIDGET (dlg.box_evolution),
                            GTK_WIDGET (dlg.label_evolution));

  dlg.box = (GtkGrid *) gtk_grid_new ();
  gtk_grid_attach (dlg.box, GTK_WIDGET (dlg.combo_channel), 0, 0, 1, 1);
  gtk_grid_attach (dlg.box, GTK_WIDGET (dlg.widget_file), 0, 1, 1, 1);
  gtk_grid_attach (dlg.box, GTK_WIDGET (dlg.notebook), 0, 2, 1, 1);

  dlg.window =
    (GtkDialog *) gtk_dialog_new_with_buttons (gettext ("Save solution"),
                                               dialog_simulator->window,
                                               GTK_DIALOG_MODAL,
                                               gettext ("_OK"), GTK_RESPONSE_OK,
                                               gettext ("_Cancel"),
                                               GTK_RESPONSE_CANCEL, NULL);
  gtk_container_set_border_width ((GtkContainer *) dlg.window, 10);
  gtk_container_add (GTK_CONTAINER (gtk_dialog_get_content_area (dlg.window)),
                     GTK_WIDGET (dlg.box));
  gtk_widget_show_all (GTK_WIDGET (dlg.box));

  if (gtk_dialog_run (dlg.window) == GTK_RESPONSE_OK)
    {
      channel_write
        = gtk_combo_box_get_active (GTK_COMBO_BOX (dlg.combo_channel));

      i = 0;
      if (!file_read)
        goto exit2;

      name = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dlg.widget_file));
      file_write = g_fopen (name, "w");
      if (!file_write)
        goto exit1;

      switch (gtk_notebook_get_current_page (dlg.notebook))
        {
        case 0:
          write_profile (file_read,
                         file_write,
                         sys->channel[channel_write].i,
                         sys->channel[channel_write].i2, n,
                         gtk_combo_box_get_active (GTK_COMBO_BOX
                                                   (dlg.combo_profile)), &vsize,
                         sys->nt);
          break;
        default:
          write_evolution (file_read,
                           file_write, n,
                           gtk_combo_box_get_active (GTK_COMBO_BOX
                                                     (dlg.combo_evolution)),
                           &vsize, sys->channel + channel_write);
        }
      i = 1;
      fclose (file_write);

    exit1:
      fclose (file_read);

    exit2:
      window_parent = (GtkWindow *) dlg.window;
      if (!i)
        jbw_show_error2
          (gettext ("Save solution"), gettext ("Unable to open the file"));
      window_parent = dialog_simulator->window;
    }

  gtk_widget_destroy (GTK_WIDGET (dlg.window));
  g_free (string_profiles);
}
