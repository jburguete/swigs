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
 * \file init.c
 * \brief Main source file of the swigs.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2005-2014 Javier Burguete Tolosa. All rights reserved.
 */
#define _GNU_SOURCE
#include <locale.h>
//#include <unistd.h>
#include "simulate.h"
#include "transport_simulate.h"
#if JBW == JBW_GTK
//	#include "editor_channel.h"
	#include "dialog_system.h"
	#include "dialog_simulator.h"
#endif
#include "variables_system.c"
#include "variables_mesh.c"

// Global variables of swigs defined in init.c

/**
 * \var steading
 * \brief boolean defining if the program is calculating a steady flow.
 * \var ti
 * \brief time interval between two solution saves.
 * \var tf
 * \brief time interval of simulation.
 * \var tmax
 * \brief final time in a calculation step.
 * \var btmax
 * \brief maximum final time in a calculation step allowed by the boundary
 *   conditions.
 * \var dtmax
 * \brief maximum size of time step allowed by flow.
 * \var fdtmax
 * \brief maximum size of time step allowed by friction.
 * \var dt2
 * \brief \f$0.5 * dt\f$.
 * \var etap
 * \brief semi-explicit positive parameter.
 * \var etam
 * \brief semi-explicit negative parameter.
 * \var dt2v
 * \brief dt2 in the former time step.
 * \var fcfl
 * \brief CFL number of the flow.
 * \var fo
 * \brief implicit number of the source terms.
 * \var channel_overflow
 * \brief boolean defining if they are a channel overflow event.
 * \var started
 * \brief boolean defining if the initial conditions are calculated.
 * \var simulated
 * \brief booleand defining if a simulation was finised.
 * \var type_animation
 * \brief type of solution display: 0 at the end of the simulation, 1 at every
 *   interval of solution saves and 2 at every time step.
 * \var animating
 * \brief boolean defining if the program is shown an animation of the
 *   results.
 * \var opening
 * \brief boolean defining if the program is opening a system of channels.
 * \var nstep
 * \brief number of time points with saved results.
 * \var flow_steady_error_max
 * \brief maximum flow error considered to obtain steady initial conditions
 *   convergence.
 */

int steading;
JBDOUBLE t0, ti, tf, tmax, btmax, dtmax, fdtmax, dt2, etap, etam;
#if MODEL_PRESSURE  ==  MODEL_PRESSURE_HIGH_ORDER
	JBDOUBLE dt2v;
#endif
JBFLOAT fcfl, fo;

int channel_overflow;
int started = 0;
int simulated = 0;
int type_animation = 1;
int animating = 0;
int opening = 0;
int nstep = 0;

JBFLOAT flow_steady_error_max;

void (*flow_parameters)(Parameters*);
void (*part_decomposition)(int);
void (*system_parameters)();
void (*system_parameters2)();
void (*system_decomposition)();
void (*system_simulate_step)();
void (*system_simulate_step2)();

void (*transport_parameters_step)(TransportParameters*, Parameters*, int, JBDOUBLE);
void (*transport_scheme_contributions)
	(TransportParameters*, Parameters*, int, JBDOUBLE*, JBDOUBLE*);
void (*transport_scheme_inlet)
	(TransportParameters*, Parameters*, int, Mass*, JBDOUBLE);
void (*transport_scheme_outlet)
	(TransportParameters*, Parameters*, int, Mass*, JBDOUBLE);
void (*diffusion_parameters_explicit)
	(TransportParameters*, Parameters*, int, JBDOUBLE);
void (*diffusion_parameters_implicit)(Parameters*);
void (*system_transport_step)();

#if JBW == JBW_GTK

// Global variables of swigs defined in init.c
// (used only in the graphical version).

/**
 * \char string_system
 * \brief string defining the name of the channels system file.
 * \var istep
 * \brief number of the time point with saved results to show.
 * \var channel
 * \brief number of the channel to show.
 * \var channel_write
 * \brief number of the channel to save the solution.
 * \var profile_xmin
 * \brief minimum x to show in a longitudinal profile.
 * \var profile_xmax
 * \brief maximum x to show in a longitudinal profile.
 * \var profile_ymin
 * \brief minimum y to show in a longitudinal profile.
 * \var profile_ymax
 * \brief maximum y to show in a longitudinal profile.
 * \var profile_zmin
 * \brief minimum z to show in a longitudinal profile.
 * \var profile_zmax
 * \brief maximum z to show in a longitudinal profile.
 * \var profile_parameter1
 * \brief type of 1st parameter to show in a longitudinal profile.
 * \var profile_parameter2
 * \brief type of 2nd parameter to show in a longitudinal profile.
 * \var evolution_xmin
 * \brief minimum x to show in a temporal evolution.
 * \var evolution_xmax
 * \brief maximum x to show in a temporal evolution.
 * \var evolution_ymin
 * \brief minimum y to show in a temporal evolution.
 * \var evolution_ymax
 * \brief maximum y to show in a temporal evolution.
 * \var evolution_zmin
 * \brief minimum z to show in a temporal evolution.
 * \var evolution_zmax
 * \brief maximum z to show in a temporal evolution.
 * \var evolution_parameter1
 * \brief type of 1st parameter to show in a temporal evolution.
 * \var evolution_parameter2
 * \brief type of 2nd parameter to show in a temporal evolution.
 * \var section_xmin
 * \brief minimum x to show in a cross section.
 * \var section_xmax
 * \brief maximum x to show in a cross section.
 * \var section_ymin
 * \brief minimum y to show in a cross section.
 * \var section_ymax
 * \brief maximum y to show in a cross section.
 * \var evolution_section
 * \brief number of channel cross section to show the temporal evolution.
 * \var section
 * \brief number of channel cross section to show.
 * \var type_draw
 * \brief type of graphical to show: 0 longitudinal profile, 1 temporal
 *   evolution, 2 cross section.
 * \var draw_adjust
 * \brief type of adjust of the graphics: 0 fixed by the corresponding
 *   [xmin,xmax]; 1 automatic adjust to the extreme values.
 * \var draw_grid
 * \brief boolean defining if a grid is shown.
 * \var n_open_sections
 * \brief open number of sections.
 */

char *string_system = NULL;

int istep;

int channel = 0;
int channel_write = 0;
JBFLOAT profile_xmin = 0.;
JBFLOAT profile_xmax = 0.;
JBFLOAT profile_ymin = 0.;
JBFLOAT profile_ymax = 0.;
JBFLOAT profile_zmin = 0.;
JBFLOAT profile_zmax = 0.;
int profile_parameter1 = 0;
int profile_parameter2 = 1;
JBFLOAT evolution_xmin = 0.;
JBFLOAT evolution_xmax = 0.;
JBFLOAT evolution_ymin = 0.;
JBFLOAT evolution_ymax = 0.;
JBFLOAT evolution_zmin = 0.;
JBFLOAT evolution_zmax = 0.;
int evolution_parameter1 = 0;
int evolution_parameter2 = 1;
JBFLOAT section_xmin = 0.;
JBFLOAT section_xmax = 0.;
JBFLOAT section_ymin = 0.;
JBFLOAT section_ymax = 0.;
int evolution_section = 0;
int section = 0;

int type_draw = 0;
int draw_adjust = 0;
int draw_grid = 1;

int n_open_sections;

GtkWindow *window_parent;

//EditorChannel editor[1];
DialogOpenSystem dialog_open_system[1];
DialogOpenParameters dialog_open_parameters[1];
DialogSimulator *dialog_simulator;

#endif

/**
 * \fn int solution_write(System *sys, xmlNode *node, FILE *file_read, long nl)
 * \brief Function to write an ASCII solution file and (optional) to compare the
 *   result with a test solution file (low level).
 * \param sys
 * \brief system of channels.
 * \param node
 * \brief XML node defining the solution type.
 * \param file_read
 * \brief binary file to read the solution.
 * \param nl
 * \brief number of time steps saved in the binary solution file.
 * \return 1 on succes, 0 on error.
 */
int solution_write(System *sys, xmlNode *node, FILE *file_read, long nl)
{
	int i, k;
	long j;
	char *buffer;
	JBDOUBLE herror, qerror, serror, herrorsol, qerrorsol, serrorsol;
	FILE *file_write;
	Channel *channel;
	#if DEBUG_SOLUTION_WRITE
		fprintf(stderr, "solution_write: start\n%s\n", node->name);
	#endif
	if (xmlStrcmp(node->name, XML_PROFILE)) goto evolution;

	// The type of solution file is a longitudinal profile of a channel
	if (xmlHasProp(node, XML_NAME))
	{
		buffer = (char*)xmlGetProp(node, XML_NAME);
		file_write = g_fopen(buffer, "w");
		#if DEBUG_SOLUTION_WRITE
			fprintf(stderr, "SW name=%s\n", buffer);
		#endif
		xmlFree(buffer);
		if (!file_write) goto error_file;
	}
	else goto error_file;
	buffer = gettext("Bad profile");
	if (xmlHasProp(node, XML_CHANNEL))
	{
		k = jb_xml_node_get_int(node, XML_CHANNEL, &i);
		#if DEBUG_SOLUTION_WRITE
			fprintf(stderr, "SW channel=%d\n", k);
		#endif
		if (!i)
		{
			jbw_show_error2(gettext("Solution"), buffer);
			return 0;
		}
		--k;
		if (k < 0 || k > sys->n)
		{
			jbw_show_error2(gettext("Solution"), gettext("Bad channel number"));
			return 0;
		}
	}
	else k = 0;
	i = 0;
	if (xmlHasProp(node, XML_TIME)) j = jb_xml_node_get_int(node, XML_TIME, &i);
	else
	{
		jbw_show_error2(gettext("Solution"), buffer);
		return 0;
	}
	#if DEBUG_SOLUTION_WRITE
		fprintf(stderr, "SW time=%ld nl=%ld\n", j, nl);
	#endif
	if (!i || j>nl)
	{
		jbw_show_error2(gettext("Solution"), buffer);
		return 0;
	}
	channel = sys->channel + k;
	write_profile(file_read, file_write, channel->i, channel->i2, n, j, &vsize,
		channel->nt);
	goto solution;

evolution:
	// The type of solution file is a temporal evolution of a cross section
	if (xmlStrcmp(node->name, XML_EVOLUTION))
	{
		jbw_show_error(gettext("Bad write XML node"));
		return 0;
	}
	if (xmlHasProp(node, XML_NAME))
	{
		buffer = (char*)xmlGetProp(node, XML_NAME);
		file_write = g_fopen(buffer, "w");
		xmlFree(buffer);
		if (!file_write) goto error_file;
	}
	else goto error_file;
	i = 0;
	if (xmlHasProp(node, XML_SECTION))
		j = jb_xml_node_get_int(node, XML_SECTION, &i);
	else
	{
		jbw_show_error2(gettext("Solution"), gettext("Bad evolution"));
		return 0;
	}
	if (!i)
	{
		jbw_show_error2(gettext("Solution"), gettext("Bad evolution"));
		return 0;
	}
/*
	if (!i || j>channel->cg->n)
	{
		jbw_show_error2(gettext("Solution"), gettext("Bad evolution"));
		return 0;
	}
*/
	write_evolution(file_read, file_write, n, j - 1, &vsize, sys->channel);

solution:
	// Comparing the numerical solution and a test solution
	fclose(file_write);
	i = vsize / sizeof(JBFLOAT);
	if (xmlHasProp(node, XML_SOLUTION))
	{
		qerror = jbm_file_root_mean_square_error
			((char*)xmlGetProp(node, XML_NAME), 1, 2, i,
			(char*)xmlGetProp(node, XML_SOLUTION), 1, 2, i);
		herror = jbm_file_root_mean_square_error
			((char*)xmlGetProp(node, XML_NAME), 1, 5, i,
			(char*)xmlGetProp(node, XML_SOLUTION), 1, 5, i);
		printf("qerror="FWL" herror="FWL, qerror, herror);
		if (i > N_OF_VARIABLES)
		{
			serror = jbm_file_root_mean_square_error
				((char*)xmlGetProp(node, XML_NAME), 1, 9, i,
				(char*)xmlGetProp(node, XML_SOLUTION), 1, 9, i);
			printf(" serror="FWL, serror);
		}
		printf("\n");
		if (xmlHasProp(node, XML_QERROR))
		{
			qerrorsol = jb_xml_node_get_float(node, XML_QERROR, &i);
			if (!i)
			{
				jbw_show_error(gettext("Bad q error"));
				return 0;
			}
			if (qerror > 1.01 * qerrorsol && qerror > MAXIMUM_SOLUTION_ERROR)
			{
				jbw_show_warning(gettext("High q error"));
				return 0;
			}
		}
		if (xmlHasProp(node, XML_HERROR))
		{
			herrorsol = jb_xml_node_get_float(node, XML_HERROR, &i);
			if (!i)
			{
				jbw_show_error(gettext("Bad h error"));
				return 0;
			}
			if (herror > 1.01 * herrorsol && herror > MAXIMUM_SOLUTION_ERROR)
			{
				jbw_show_warning(gettext("High h error"));
				return 0;
			}
		}
		if (i > N_OF_VARIABLES && xmlHasProp(node, XML_SERROR))
		{
			serrorsol = jb_xml_node_get_float(node, XML_SERROR, &i);
			if (!i)
			{
				jbw_show_error(gettext("Bad s error"));
				return 0;
			}
			if (serror > 1.01 * serrorsol && serror > MAXIMUM_SOLUTION_ERROR)
			{
				jbw_show_warning(gettext("High s error"));
				return 0;
			}
		}
	}
	#if DEBUG_SOLUTION_WRITE
		fprintf(stderr, "solution_write: end\n");
	#endif
	return 1;

error_file:
	jbw_show_error2(gettext("Solution file"), gettext("No name"));
	#if DEBUG_SOLUTION_WRITE
		fprintf(stderr, "solution_write: end\n");
	#endif
	return 0;
}

/**
 * \fn int parameters_open(System *sys, xmlNode *node)
 * \brief Function to write an ASCII solution file and (optional) to compare the
 *   result with a test solution file (high level).
 * \param sys
 * \brief system of channels.
 * \param node
 * \brief XML node defining the solution type.
 * \return 1 on succes, 0 on error.
 */
int parameters_open(System *sys, xmlNode *node)
{
	long j;
	FILE *file;
	int i = 0;
	printf("Saving node=%s file=%s\n", node->name, sys->solution_path);
	file = g_fopen(sys->solution_path, "rb");
	if (!file)
	{
		jbw_show_error2
			(gettext("Solution"), gettext("Unable to open the file"));
		return 0;
	}
	j = vsize*(n+1);
	j = jb_flength(file)/j;
	i = solution_write(sys, node, file, j);
	fclose(file);
	if (i) printf("OK\n");
	return i;
}

/**
 * \fn int simulation_open(char *file_name)
 * \brief Function to open a simulation file and making the simulations.
 * \param file_name
 * \brief input file name.
 * \return 1 on succes, 0 on error.
 */
int simulation_open(char *file_name)
{
	int i;
	char *system_name;
	xmlNode *node, *child;
	xmlDoc *doc;
	#if DEBUG_SIMULATION_OPEN
		fprintf(stderr, "simulation_open: start\n");
	#endif
	doc = xmlParseFile(file_name);
	if (!doc)
	{
		jbw_show_error(gettext("Unable to open the parameters"));
		goto error_file;
	}
	node = xmlDocGetRootElement(doc);
	if (xmlStrcmp(node->name, XML_SIMULATE))
	{
		jbw_show_error(gettext("Bad simulation XML file"));
		goto error_file;
	}
	for (node = node->children; node; node = node->next)
	{
		if (xmlStrcmp(node->name, XML_SYSTEM))
		{
			jbw_show_error(gettext("Bad simulation XML file"));
			return 0;
		}
		if (xmlHasProp(node, XML_NAME))
		{
			system_name = (char*)xmlGetProp(node, XML_NAME);
			if (!system_name) goto error_file;
		}
		else goto error_file;
		printf("Openning system %s\n", system_name);
		if (!system_open_xml(sys, system_name, ".")) return 0;
		printf("Simulating\n");
		simulate();
		printf("Mass="FWL"\n", flow_mass(p, n));
		for (i = 0; i <= sys->channel->nt; ++i)
			printf("Solute=%d mass="FWL"\n",
				i, transport_mass(tp + i * (n + 1), n));
		for (child = node->children; child; child = child->next)
			if (!parameters_open(sys, child)) return 0;
		system_delete(sys);
		xmlFree(system_name);
	}
	xmlFreeDoc(doc);
	#if DEBUG_SIMULATION_OPEN
		fprintf(stderr, "simulation_open: end\n");
	#endif
	return 1;

error_file:
	xmlFreeDoc(doc);
	#if DEBUG_SIMULATION_OPEN
		fprintf(stderr, "simulation_open: end\n");
	#endif
	return 0;
}

/**
 * \def LOCALE_DIR
 * \brief Macro to select the paht of locale language files.
 */
#if defined(G_OS_WIN32)
	#define LOCALE_DIR "../share/locale"
	#if JB_DEBUG_FILE
		FILE *stderr;
	#endif
#else
	#define LOCALE_DIR "po"
#endif

/**
 * \def PROGRAM_NAME
 * \brief Macro to select the name of the executable file.
 */
#if JBW == JBW_NO
	#define PROGRAM_NAME "swigsbin"
#else
	#define PROGRAM_NAME "swigs"
#endif

/**
 * \fn int main(int argn, char **argc)
 * \brief Main function.
 * \param argn
 * \brief arguments number.
 * \param argc
 * \brief arguments strings.
 * \return 0 on success, >0 on error.
 */
int main(int argn, char **argc)
{
	char *buffer;
	#if JB_DEBUG_FILE
		stderr = g_fopen("debug.log", "w");
	#endif
	#if DEBUG_MAIN
		fprintf(stderr, "Main\n");
	#endif
/*
	#ifdef G_OS_WIN32
		nthreads = 8;
	#else
		nthreads = (int)sysconf(_SC_NPROCESSORS_CONF);
	#endif
*/
	nthreads = 1;
	v->x = NULL;
	xmlKeepBlanksDefault(0);
	xmlSetCompressMode(9);
	setlocale(LC_ALL, "");
	setlocale(LC_NUMERIC, "C");
	buffer = g_get_current_dir();
	bindtextdomain(PROGRAM_NAME, g_build_filename(buffer, LOCALE_DIR, NULL));
	bind_textdomain_codeset(PROGRAM_NAME, "UTF-8");
	textdomain(PROGRAM_NAME);
	#if DEBUG_MAIN
		fprintf(stderr, "M copying system\n");
	#endif
	if (!system_copy(sys, sysnew)) return 3;
#if JBW == JBW_GTK
	if (!jbw_graphic_init(&argn, &argc))
	{
		printf("%s\n", gettext("Unable to open the external libraries"));
		return 2;
	}
	switch (argn)
	{
	case 1:
		break;
	case 3:
		if (!strcmp(argc[1], "-np") && sscanf(argc[2], "%d", &nthreads) == 1 &&
			nthreads>0) break;
	default:
		jbw_show_error2(gettext("Usage is"),
			gettext("swigs [-np number_of_processors]"));
		return 1;
	}
	string_system = g_build_filename(buffer, "New.xml", NULL);
	g_free(buffer);
	#if DEBUG_MAIN
		fprintf(stderr, "M opening dialog simulator\n");
	#endif
	dialog_simulator = dialog_simulator_new();
	if (!dialog_simulator) return 4;
	#if DEBUG_MAIN
		fprintf(stderr, "M starting simulation\n");
	#endif
	if (!simulate_start()) return 5;
	#if DEBUG_MAIN
		fprintf(stderr, "M main bucle\n");
	#endif
	#if JBW_GRAPHIC == JBW_GRAPHIC_GLUT
		glutIdleFunc((void(*))gtk_main_iteration);
		glutMainLoop();
	#else
		gtk_main();
	#endif
#else
	g_free(buffer);
	switch (argn)
	{
	case 2:
		buffer = argc[1];
		break;
	case 4:
		if (!strcmp(argc[1], "-np") && sscanf(argc[2], "%d", &nthreads) == 1 &&
			nthreads>0)
		{
			buffer = argc[3];
			break;
		}
	default:
		jbw_show_error2(gettext("Usage is"),
			gettext("swigs [-np number_of_processors] simulation_file"));
		return 1;
	}
	simulation_open(buffer);
#endif
	return 0;
}
