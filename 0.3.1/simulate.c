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
 * \file simulate.c
 * \brief Source file defining the main functions of the simulation.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2005-2014 Javier Burguete Tolosa. All rights reserved.
 */
#define _GNU_SOURCE
#include <time.h>
#include "simulate.h"

#if !INLINE_SCHEME_OPEN
void scheme_open() {_scheme_open();}
#endif

#if JBW == JBW_GTK

/**
 * \fn void dialog_open_parameters_destroy()
 * \brief Function to destroy an open parameters dialog.
 */
void dialog_open_parameters_destroy()
{
	gtk_widget_destroy((GtkWidget*)dialog_open_parameters->window);
}

/**
 * \fn void dialog_open_parameters_actualize()
 * \brief Function to actualize the data of an open parameters dialog.
 */
void dialog_open_parameters_actualize()
{
	float x;
	if (n>0) x = n_open_sections/(float)(n+1); else x = 0.;
	gtk_progress_bar_set_fraction(dialog_open_parameters->progress, x);
	while (gtk_events_pending()) gtk_main_iteration();
}

/**
 * \fn void dialog_open_parameters_response(GtkDialog *dlg)
 * \brief Function to abort the opening of parameters.
 * \param dlg
 * \brief window parent.
*/
void dialog_open_parameters_response(GtkDialog *dlg)
{
	GtkMessageDialog *message_dialog;
	message_dialog = (GtkMessageDialog*)gtk_message_dialog_new((GtkWindow*)dlg,
		GTK_DIALOG_MODAL, GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
		gettext("This abort the program! Do you like to continue?"));
	gtk_window_set_title((GtkWindow*)message_dialog, gettext("Warning!"));
	if (gtk_dialog_run((GtkDialog*)message_dialog) == GTK_RESPONSE_YES) exit(0);
	gtk_widget_destroy((GtkWidget*)message_dialog);
}

/**
 * \fn void dialog_open_parameters_new()
 * \brief Function to open an open parameters dialog.
 */
void dialog_open_parameters_new()
{
	DialogOpenParameters *dlg = dialog_open_parameters;
	dlg->label = (GtkLabel*)gtk_label_new
		(gettext("Calculating geometrical parameters"));
	gtk_widget_set_sensitive((GtkWidget*)dlg->label, 0);
	dlg->progress = (GtkProgressBar*)gtk_progress_bar_new();
	dlg->box = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach(dlg->box, GTK_WIDGET(dlg->label), 0, 0, 1, 1);
	gtk_grid_attach(dlg->box, GTK_WIDGET(dlg->progress), 0, 1, 1, 1);
	dlg->window = (GtkDialog*)gtk_dialog_new_with_buttons(
		gettext("Opening mesh"), window_parent,
		GTK_DIALOG_MODAL, gettext("_Stop"), GTK_RESPONSE_OK, NULL);
	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(dlg->window)),
		GTK_WIDGET(dlg->box));
	gtk_widget_show_all(GTK_WIDGET(dlg->window));
	g_signal_connect_swapped(dlg->window, "delete-event", (void(*))exit, 0);
	g_signal_connect
		(dlg->window, "response", (void(*))dialog_open_parameters_response, 0);
}

/**
 * \fn void dialog_simulate_steady_actualize(DialogSimulateSteady *dlg, \
 *   JBDOUBLE steady_error)
 * \brief Function to actualize the steady error of a steady dialog.
 * \param dlg
 * \brief steady dialog.
 * \param steady_error
 * \brief steady error.
 */
void dialog_simulate_steady_actualize
	(DialogSimulateSteady *dlg, JBDOUBLE steady_error)
{
	jbw_float_entry_set_value(dlg->entry, steady_error);
}

/**
 * \fn void dialog_simulate_steady_response()
 * \brief Function to send a signal to close a steady dialog.
 */
void dialog_simulate_steady_response()
{
	steading = 0;
}

/**
 * \fn void dialog_simulate_steady_new(DialogSimulateSteady *dlg)
 * \brief Function to open a steady dialog.
 * \param dlg
 * \brief steady dialog.
 */
void dialog_simulate_steady_new(DialogSimulateSteady *dlg)
{
	dlg->label = (GtkLabel*)gtk_label_new
		(gettext("Calculating steady initial conditions ..."));
	dlg->label_error = (GtkLabel*)gtk_label_new
		(gettext("Maximum steady initial discharge error"));
	dlg->entry = jbw_float_entry_new();
	gtk_widget_set_sensitive(GTK_WIDGET(dlg->entry), 0);
	dlg->box = (GtkGrid*)gtk_grid_new();
	gtk_grid_attach(dlg->box, GTK_WIDGET(dlg->label), 0, 0, 1, 1);
	gtk_grid_attach(dlg->box, GTK_WIDGET(dlg->label_error), 1, 0, 1, 1);
	gtk_grid_attach(dlg->box, GTK_WIDGET(dlg->entry), 2, 0, 1, 1);
	dlg->window = (GtkDialog*)gtk_dialog_new_with_buttons(
		gettext("Flow initial conditions"), window_parent,
		GTK_DIALOG_MODAL, gettext("_Stop"), GTK_RESPONSE_OK, NULL);
	gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(dlg->window)),
		GTK_WIDGET(dlg->box));
  	g_signal_connect
		(dlg->window, "response", dialog_simulate_steady_response, NULL);
	gtk_widget_show_all(GTK_WIDGET(dlg->window));
}

#endif

/**
 * \fn void simulate_steady()
 * \brief Function to calculate steady initial conditions.
 */
void simulate_steady()
{
	time_t t1, t2;
	JBDOUBLE e1, e2, tsteady;
	#if JBW == JBW_GTK
		DialogSimulateSteady dlg[1];
	#endif
	#if DEBUG_SIMULATE_STEADY
		fprintf(stderr, "Simulate steady: start\n");
	#endif
	steading = 1;
	#if JBW == JBW_GTK
		dialog_simulate_steady_new(dlg);
	#endif
	t1 = time(0);
	e1 = INFINITY;
	tsteady = 0.;
	do
	{
		dt = dtmax;
		dt2 = 0.5 * dt;
		tmax = t + dtmax;
		#if DEBUG_SIMULATE_STEADY
			if (t >= DEBUG_TIME)
			{
				fprintf(stderr, "SS tmax="FWL" dtmax="FWL"\n", tmax, dtmax);
				fprintf(stderr, "SS system_decomposition\n");
			}
		#endif
		system_decomposition();
		#if DEBUG_SIMULATE_STEADY
			if (t >= DEBUG_TIME) fprintf(stderr, "SS system_simulate_step\n");
		#endif
		system_simulate_step();
		#if DEBUG_SIMULATE_STEADY
			if (t >= DEBUG_TIME) fprintf(stderr, "SS system_simulate_step2\n");
		#endif
		system_simulate_step2();
		#if DEBUG_SIMULATE_STEADY
			if (t >= DEBUG_TIME) fprintf(stderr, "SS system_transport_step\n");
		#endif
		if (sys->nt >= 0) system_transport_step();
		#if DEBUG_SIMULATE_STEADY
			if (t >= DEBUG_TIME) fprintf(stderr, "SS system_parameters2\n");
		#endif
		e2 = flow_steady_error(p, n);
		system_parameters2();
		#if MODEL_PRESSURE == MODEL_PRESSURE_HIGH_ORDER
			dt2v = dt2;
		#endif
		tsteady+=dtmax;
		#if DEBUG_SIMULATE_STEADY
			fprintf(stderr, "SS e1="FWL" e2="FWL" tsteady="FWL"\n",
				e1, e2, tsteady);
		#endif
		if ((e2 < flow_steady_error_max && e2 >= e1) ||
			tsteady > sys->max_steady_time)
				steading = 0;
		#if JBW == JBW_GTK
			dialog_simulate_steady_actualize(dlg, e2);
		#endif
		#if JBW==JBW_GTK
			#if DEBUG_SIMULATE_STEADY
				if (t >= DEBUG_TIME) fprintf(stderr, "Iterating main\n");
			#endif
			while (gtk_events_pending()) gtk_main_iteration();
			#if DEBUG_SIMULATE_STEADY
				if (t >= DEBUG_TIME) fprintf(stderr, "Main iterated\n");
			#endif
		#endif
		switch (type_animation)
		{
		case 1:
			if (time(&t2) <= t1) break;
			t1 = t2;
		case 2:
			draw();
			printf("e1="FWL" e2="FWL"\ntsteady="FWL" max="FWL"\n",
				e1, e2, tsteady, sys->max_steady_time);
		}
		e1 = e2;
	}
	while (steading);
	#if JBW == JBW_GTK
		gtk_widget_destroy((GtkWidget*)dlg->window);
	#endif
	#if DEBUG_SIMULATE_STEADY
		fprintf(stderr, "Drawing\n");
	#endif
	draw();
	#if DEBUG_SIMULATE_STEADY
		fprintf(stderr, "Simulate steady: end\n");
	#endif
}

/**
 * \fn int simulate_start()
 * \brief Function to start the simulation. It opens the numerical scheme, the
 *   mesh and the initial conditions.
 * \return 0 on error, 1 on success.
 */
int simulate_start()
{
	int i, j;
	Channel *channel;
	#if DEBUG_SIMULATE_START
		fprintf(stderr, "simulate_start: start\n");
	#endif

	simulating = simulated = nstep = 0;
	#if JBW == JBW_GTK
		dialog_simulator_actualize_menu(dialog_simulator, 1);
	#endif

	if (sys->n < 0) goto exit;

	#if DEBUG_SIMULATE_START
		fprintf(stderr, "SS opening flow scheme\n");
	#endif
	scheme_open();

	#if DEBUG_SIMULATE_START
		system_print(sys, stderr);
		fprintf(stderr, "SS deleting mesh\n");
	#endif
	mesh_delete(sys, &p, &tp, v, &junction, &m, &n, &njunctions);
	#if JBW == JBW_GTK
		#if DEBUG_SIMULATE_START
			fprintf(stderr, "SS opening dialog\n");
		#endif
		dialog_open_parameters_new();
		opening = 1;
	#endif
	#if DEBUG_SIMULATE_START
		fprintf(stderr, "SS opening mesh\n");
	#endif
	if (!mesh_open(sys, &p, &tp, v, &junction, &m, &n, &njunctions, &vsize))
		goto exit;

	#if JBW == JBW_GTK
		#if DEBUG_SIMULATE_START
			fprintf(stderr, "SS destroying open parameters dialog\n");
		#endif
		dialog_open_parameters_destroy();
		opening = 0;
	#endif

	t = t0 = sys->initial_time;

	#if DEBUG_SIMULATE_START
		fprintf(stderr, "SS system_parameters\n");
	#endif
	system_parameters();

	channel_overflow = 0;
	tf = sys->final_time;
	#if DEBUG_SIMULATE_START
		fprintf(stderr, "SS tf="FWL"\n", tf);
	#endif
	ti = sys->measured_interval;
	if (ti <= 0.) ti = tf-t0;
	fcfl = sys->cfl;

	fo = flow_implicit;

	flow_steady_error_max = sys->steady_error;

	#if DEBUG_SIMULATE_START
		fprintf(stderr, "SS tf="FWL"\n", tf);
		fprintf(stderr, "SS starting simulation\n");
	#endif

	#if DEBUG_SIMULATE_START
		fprintf(stderr, "SS system parameters2\n");
	#endif
	system_parameters2();

	#if DEBUG_SIMULATE_START
		fprintf(stderr, "SS t="FWL" tf="FWL" dtmax="FWL"\n", t, tf, dtmax);
	#endif

	if (sys->channel->fic->type == INITIAL_FLOW_STEADY)
	{
		simulate_steady();
		for (j = sys->n, channel = sys->channel; j >= 0; --j, ++channel)
			for (i = 0; i <= channel->nt; ++i)
				if (channel->ct[i].it->type != INITIAL_TRANSPORT_STEADY)
					transport_variables_open
						(p, tp+i*(n+1), n, channel->ct[i].it);
		system_parameters2();
	}
	#if DEBUG_SIMULATE_START
		fprintf(stderr, "SS tf="FWL"\n", tf);
	#endif

	#if DEBUG_SIMULATE_START
		fprintf(stderr, "SS drawing\n");
	#endif
	draw();
	started = 1;
	#if DEBUG_SIMULATE_START
		fprintf(stderr, "SS tf="FWL"\n", tf);
		fprintf(stderr, "simulate_start: end\n");
	#endif
	return n;

exit:
	#if JBW == JBW_GTK
		dialog_open_parameters_destroy();
		opening = 0;
	#endif
	started = 0;
	#if DEBUG_SIMULATE_START
		fprintf(stderr, "simulate_start: end with error\n");
	#endif
	return 0;
}

/**
 * \fn void simulate()
 * \brief Main function to make the simulation.
 */
void simulate()
{
	int i, j;
	#if GUAD2D
		int iRec, iEnv;
	#endif
	JBDOUBLE at;
	FILE *file2, *file3, *file4;
	#if JBW == JBW_GTK
		GtkMessageDialog *dlg;
	#endif

	#if DEBUG_SIMULATE
		fprintf(stderr, "simulate: start\n");
		fprintf(stderr, "S tf="FWL"\n", tf);
	#endif

	if (!started && !simulate_start())
	{
		nstep = 0;
		goto exit2;
	}

	#if JBW == JBW_GTK
		sys->solution_file = g_fopen(sys->solution_path, "rb");
		if (sys->solution_file)
		{
			fclose(sys->solution_file);
			dlg = (GtkMessageDialog*)gtk_message_dialog_new
				(dialog_simulator->window, GTK_DIALOG_MODAL,
				GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,
				gettext("Do you want to overwrite the solution file?"));
			gtk_window_set_title((GtkWindow*)dlg, gettext("Warning!"));
			i = gtk_dialog_run((GtkDialog*)dlg);
			gtk_widget_destroy((GtkWidget*)dlg);
			if (i == GTK_RESPONSE_YES)
				sys->solution_file = g_fopen(sys->solution_path, "wb");
			else
			{
				nstep = 0;
				goto exit2;
			}
		}
	#endif
	
	simulating = 1;
	#if JBW == JBW_GTK
		dialog_simulator_actualize_menu(dialog_simulator, 0);
	#endif
	sys->solution_file = g_fopen(sys->solution_path, "wb");
	write_data(sys->solution_file, v, p, tp, n, vsize, sys->channel->nt);
	if (sys->plumes_path)
	{
		file3 = g_fopen(sys->plumes_path, "w");
		write_plumes(file3, p, tp, t, sys);
	}
	if (sys->contributions_path)
	{
		file4 = g_fopen(sys->contributions_path, "w");
		write_contributions(file4, t, sys);
	}
	#if DEBUG_SIMULATE
		fprintf(stderr, "S t="FWL" tf="FWL" ti="FWL" dtmax="FWL"\n",
			t, tf, ti, dtmax);
	#endif

	mass_start(m);
	m[0].m0 = flow_mass(p, n);
	#if DEBUG_SIMULATE
		fprintf(stderr, "S mass m0="FWL"\n", m[0].m0);
	#endif
	for (i = j = 0; i <= sys->nt; ++i, j += n + 1)
	{
		mass_start(m + i + 1);
		m[i + 1].m0 = transport_mass(tp + j, n);
		#if DEBUG_SIMULATE
			fprintf(stderr, "S solute=%d m0="FWL"\n", i, m[i + 1].m0);
		#endif
	}

	#if GUAD2D
// abrir interfaces
	#endif
	while (t < tf)
	{
		at = fmin(tf, t + ti);
		#if DEBUG_SIMULATE
			if (t >= DEBUG_TIME) fprintf(stderr, "S at="FWL"\n", at);
		#endif
		for (; t < at;)
		{
			if (!simulating) goto exit;
			tmax = fmin(tmax, at);
			dt = tmax-t;
			#if DEBUG_SIMULATE
				if (t >= DEBUG_TIME)
					fprintf(stderr, "S t="FWL" dt="FWL"\n", t, dt);
			#endif
			#if DEBUG_SIMULATE
				if (t >= DEBUG_TIME)
					fprintf(stderr, "S t="FWL" dt="FWL"\n", t, dt);
			#endif
			#if GUAD2D
// enviar recibir dt
			#endif
			dt2 = 0.5*dt;
			#if DEBUG_SIMULATE
				if (t >= DEBUG_TIME)
					fprintf(stderr, "S system_decomposition\n");
			#endif
			system_decomposition();
			#if DEBUG_SIMULATE
				if (t >= DEBUG_TIME)
					fprintf(stderr, "S system_simulate_step\n");
			#endif
			system_simulate_step();
			#if DEBUG_SIMULATE
				if (t >= DEBUG_TIME)
					fprintf(stderr, "S system_simulate_step2\n");
			#endif
			system_simulate_step2();
			#if DEBUG_SIMULATE
				if (t >= DEBUG_TIME)
					fprintf(stderr, "S system_transport_step\n");
			#endif
			if (sys->nt >= 0) system_transport_step();
			#if DEBUG_SIMULATE
				if (t >= DEBUG_TIME) fprintf(stderr, "S system_parameters2\n");
			#endif
			t = tmax;
			system_parameters2();
			#if MODEL_PRESSURE == MODEL_PRESSURE_HIGH_ORDER
				dt2v = dt2;
			#endif
			#if JBW == JBW_GTK
				#if DEBUG_SIMULATE
					if (t >= DEBUG_TIME) fprintf(stderr, "S iterating main\n");
				#endif
				while (gtk_events_pending()) gtk_main_iteration();
				#if DEBUG_SIMULATE
					if (t >= DEBUG_TIME) fprintf(stderr, "S main iterated\n");
				#endif
			#endif
			if (type_animation == 2) draw();
			if (sys->plumes_path) write_plumes(file3, p, tp, t, sys);
			if (sys->contributions_path) write_contributions(file4, t, sys);
		}
		#if DEBUG_SIMULATE
			if (t >= DEBUG_TIME) fprintf(stderr, "S draw\n");
		#endif
		if (type_animation == 1) draw();
		#if DEBUG_SIMULATE
			if (t >= DEBUG_TIME) fprintf(stderr, "S write_data\n");
		#endif
		write_data(sys->solution_file, v, p, tp, n, vsize, sys->channel->nt);
	}
exit:
	#if GUAD2D
// cerrar interfaces
	#endif
	#if DEBUG_SIMULATE
		fprintf(stderr, "S drawing\n");
	#endif
	draw();
	fclose(sys->solution_file);
	if (sys->advances_path)
	{
		file2 = g_fopen(sys->advances_path, "w");
		write_advances(file2, p, n);
		fclose(file2);
	}
	if (sys->plumes_path) fclose(file3);
	if (sys->contributions_path) fclose(file4);
	if (channel_overflow) jbw_show_warning(gettext("Channel overflow"));
	simulated = 1;
exit2:
	simulating = started = 0;
	#if JBW == JBW_GTK
		istep = nstep - 1;
		dialog_simulator_actualize_menu(dialog_simulator, 1);
		#if DEBUG_SIMULATE
			fprintf(stderr, "S iterating main\n");
		#endif
		while (gtk_events_pending()) gtk_main_iteration();
		#if DEBUG_SIMULATE
			fprintf(stderr, "S main iterated\n");
		#endif
	#endif
	#if DEBUG_SIMULATE
		fprintf(stderr, "simulate: end\n");
	#endif
}
