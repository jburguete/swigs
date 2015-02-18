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
 * \file system.h
 * \brief Header file to define channels system structures and functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2015 Javier Burguete Tolosa.
 */
#ifndef SYSTEM__H
#define SYSTEM__H 1

#include "channel.h"

/**
 * \struct System
 * \brief Structure to define a system of channels.
 */
typedef struct
{
/**
 * \var n
 * \brief number of channels.
 * \var nt
 * \brief number of transported solutes.
 * \var initial_time
 * \brief simulation initial time.
 * \var observation_time
 * \brief final time with observed measures.
 * \var final_time
 * \brief simulation final time.
 * \var measured_interval
 * \brief interval time to save the numerical results.
 * \var cfl
 * \brief CFL number.
 * \var implicit
 * \brief implicit number.
 * \var steady_error
 * \brief maximum error allowed in steady calculations.
 * \var max_steady_time
 * \brief maximum simulation time in steady calculations.
 * \var section_width_min
 * \brief coefficient of the minimum width in pressurized cross sections.
 * \var depth_min
 * \brief minimum flow depth allowing the water movement.
 * \var granulometric_coefficient
 * \brief aerodynamical friction coefficient of granulometric roughness.
 * \var solution_file
 * \brief solution file.
 * \var transport
 * \brief array of transported solutes.
 * \var channel
 * \brief array of channels.
 * \var *name
 * \brief name.
 * \var *directory
 * \brief directory.
 * \var *solution_name
 * \brief solution file name.
 * \var *advances_name
 * \brief advances solution file name.
 * \var *plumes_name
 * \brief plumes solution file name.
 * \var *contributions_name
 * \brief contributions solution file name.
 * \var *solution_path
 * \brief solution file path.
 * \var *advances_path
 * \brief advances solution file path.
 * \var *plumes_path
 * \brief plumes solution file path.
 * \var *contributions_path
 * \brief contributions solution file path.
 */
	int n, nt;
	JBDOUBLE initial_time, observation_time, final_time, measured_interval;
	JBFLOAT cfl, implicit, steady_error, max_steady_time, section_width_min,
		depth_min, granulometric_coefficient;
	FILE *solution_file;
	Transport *transport;
	Channel *channel;
	char *name, *directory, *solution_name, *advances_name, *plumes_name,
		 *contributions_name, *solution_path, *advances_path, *plumes_path,
		 *contributions_path;
} System;

extern System sys[1], sysnew[1];

static inline void _system_print(System *s, FILE *file)
{
	int i;
	fprintf(file, "system_print: start\n");
	fprintf(file, "SP system=%s/%s n=%d nt=%d\n",
		s->directory, s->name, s->n, s->nt);
	fprintf(file, "SP solution_name=%s\n", s->solution_name);
	fprintf(file, "SP solution_path=%s\n", s->solution_path);
	fprintf(file, "SP advances_name=%s\n", s->advances_name);
	fprintf(file, "SP plumes_name=%s\n", s->plumes_name);
	fprintf(file, "SP contributions_name=%s\n", s->contributions_name);
	fprintf(file,
		"SP initial_time="FWL" observation_time="FWL" final_time="FWL"\n"
		"SP measured_interval="FWL"\n",
		s->initial_time, s->observation_time, s->final_time,
		s->measured_interval);
	fprintf(file, "SP cfl="FWF" implicit="FWF" steady_error="FWF"\n",
		s->cfl, s->implicit, s->steady_error);
	fprintf(file, "SP max_steady_time="FWF" section_width_min="FWF"\n",
		s->max_steady_time, s->section_width_min);
	fprintf(file, "SP depth_min="FWF" granulometric_coefficient="FWF"\n",
		s->depth_min, s->granulometric_coefficient);
	for (i = 0; i <= s->nt; ++i) transport_print(s->transport + i, file);
	for (i = 0; i <= s->n; ++i) channel_print(s->channel + i, file);
	fprintf(file, "system_print: end\n");
}

#if INLINE_SYSTEM_PRINT
	#define system_print _system_print
#else
	void system_print(System*, FILE*);
#endif

static inline void _system_error(System *s, char *m)
{
	#if DEBUG_SYSTEM_ERROR
		fprintf(stderr, "system_error: start\n");
	#endif
	jbw_show_error3(gettext("System"), s->name, m);
	jb_free_null((void**)&message);
	#if DEBUG_SYSTEM_ERROR
		fprintf(stderr, "system_error: end\n");
	#endif
}

#if INLINE_SYSTEM_ERROR
	#define system_error _system_error
#else
	void system_error(System*, char*);
#endif

static inline void _system_delete(System *s)
{
	int i;
	#if DEBUG_SYSTEM_DELETE
		fprintf(stderr, "system_delete: start\n");
	#endif
	for (i = 0; i <= s->nt; ++i) transport_delete(s->transport + i);
	for (i = 0; i <= s->n; ++i) channel_delete(s->channel + i);
	jb_free_null((void**)&s->transport);
	jb_free_null((void**)&s->channel);
	jb_free_null((void**)&s->name);
	jb_free_null((void**)&s->directory);
	jb_free_null((void**)&s->solution_name);
	jb_free_null((void**)&s->solution_path);
	jb_free_null((void**)&s->advances_name);
	jb_free_null((void**)&s->advances_path);
	jb_free_null((void**)&s->plumes_name);
	jb_free_null((void**)&s->plumes_path);
	jb_free_null((void**)&s->contributions_name);
	jb_free_null((void**)&s->contributions_path);
	s->n = s->nt = -1;
	#if DEBUG_SYSTEM_DELETE
		fprintf(stderr, "system_delete: end\n");
	#endif
}

#if INLINE_SYSTEM_DELETE
	#define system_delete _system_delete
#else
	void system_delete(System*);
#endif

static inline void _system_init_empty(System *s)
{
	#if DEBUG_SYSTEM_INIT_EMPTY
		fprintf(stderr, "system_init_empty: start\n");
	#endif
	s->transport = NULL;
	s->channel = NULL;
	s->name = s->directory = s->solution_name = s->solution_path
		= s->advances_name = s->plumes_name = s->contributions_name
		= s->advances_path = s->plumes_path = s->contributions_path = NULL;
	s->n = s->nt = -1;
	#if DEBUG_SYSTEM_INIT_EMPTY
		fprintf(stderr, "system_init_empty: end\n");
	#endif
}

#if INLINE_SYSTEM_INIT_EMPTY
	#define system_init_empty _system_init_empty
#else
	void system_init_empty(System*);
#endif

static inline int _system_insert_channel(System *system, int i)
{
	int j;
	Channel *channel;
	#if DEBUG_SYSTEM_INSERT_CHANNEL
		fprintf(stderr, "system_insert_channel: start\n");
	#endif
	j = system->n + 2;
	channel = (Channel*)g_try_realloc(system->channel, j * sizeof(Channel));
	if (!channel)
	{
		system_error(system, gettext("Not enough memory"));
		goto error_insert_channel;
	}
	system->channel = channel;
	++system->n;
	--j;
	channel += j;
	channel_init_empty(channel);
	for (; j-- >= i; --channel)
	{
		channel_delete(channel);
		if (!channel_copy(channel, channel - 1))
			goto error_insert_channel;
	}
	#if DEBUG_SYSTEM_INSERT_CHANNEL
		fprintf(stderr, "system_insert_channel: end\n");
	#endif
	return 1;

error_insert_channel:
	system_error(system, message);
	#if DEBUG_SYSTEM_INSERT_CHANNEL
		fprintf(stderr, "system_insert_channel: end\n");
	#endif
	return 0;
}

#if INLINE_SYSTEM_INSERT_CHANNEL
	#define system_insert_channel _system_insert_channel
#else
	int system_insert_channel(System*, int);
#endif

static inline void _system_remove_channel(System *system, int i)
{
	int j;
	Channel *channel;
	#if DEBUG_SYSTEM_REMOVE_CHANNEL
		fprintf(stderr, "system_remove_channel: start\n");
	#endif
	for (j = i, channel = system->channel + i; j < system->n; ++j, ++channel)
	{
		channel_delete(channel);
		channel_copy(channel, channel + 1);
	}
	channel_delete(channel);
	system->channel = g_try_realloc(system->channel, j * sizeof(Channel));
	--system->n;
	#if DEBUG_SYSTEM_REMOVE_CHANNEL
		fprintf(stderr, "system_remove_channel: end\n");
	#endif
}

#if INLINE_SYSTEM_REMOVE_CHANNEL
	#define system_remove_channel _system_remove_channel
#else
	void system_remove_channel(System*, int);
#endif

static inline int _system_insert_transport(System *system, int i)
{
	int j;
	Transport *transport;
	#if DEBUG_SYSTEM_INSERT_TRANSPORT
		fprintf(stderr, "system_insert_transport: start\n");
	#endif
	j = system->n + 2;
	transport
		= (Transport*)g_try_realloc(system->transport, j * sizeof(Transport));
	if (!transport)
	{
		system_error(system, gettext("Not enough memory"));
		goto error_insert_transport;
	}
	system->transport = transport;
	++system->n;
	--j;
	transport += j;
	transport_init_empty(transport);
	for (; j-- >= i; --transport)
	{
		transport_delete(transport);
		if (!transport_copy(transport, transport - 1))
			goto error_insert_transport;
	}
	#if DEBUG_SYSTEM_INSERT_TRANSPORT
		fprintf(stderr, "system_insert_transport: end\n");
	#endif
	return 1;

error_insert_transport:
	system_error(system, message);
	#if DEBUG_SYSTEM_INSERT_TRANSPORT
		fprintf(stderr, "system_insert_transport: end\n");
	#endif
	return 0;
}

#if INLINE_SYSTEM_INSERT_TRANSPORT
	#define system_insert_transport _system_insert_transport
#else
	int system_insert_transport(System*, int);
#endif

static inline void _system_remove_transport(System *system, int i)
{
	int j;
	Transport *transport;
	#if DEBUG_SYSTEM_REMOVE_TRANSPORT
		fprintf(stderr, "system_remove_transport: start\n");
	#endif
	for (j = i, transport = system->transport + i; j < system->n;
		++j, ++transport)
	{
		transport_delete(transport);
		transport_copy(transport, transport + 1);
	}
	transport_delete(transport);
	system->transport = g_try_realloc(system->transport, j * sizeof(Transport));
	--system->n;
	#if DEBUG_SYSTEM_REMOVE_TRANSPORT
		fprintf(stderr, "system_remove_transport: end\n");
	#endif
}

#if INLINE_SYSTEM_REMOVE_TRANSPORT
	#define system_remove_transport _system_remove_transport
#else
	void system_remove_transport(System*, int);
#endif

static inline int _system_create
	(System *s, char *name, char *directory, char *buffer)
{
	#if DEBUG_SYSTEM_CREATE
		fprintf(stderr, "system_create: start\n");
	#endif
	system_init_empty(s);
	s->directory = jb_strdup(directory);
	s->name = jb_strdup(name);
	if (!s->name || !s->directory)
	{
		jbw_show_error3(gettext("System"), name, gettext("Not enough memory"));
		#if DEBUG_SYSTEM_CREATE
			fprintf(stderr, "system_create: end\n");
		#endif
		return 0;
	}
	snprintf(buffer, JB_BUFFER_SIZE, "%s/%s", directory, name);
	#if DEBUG_SYSTEM_CREATE
		fprintf(stderr, "SOS %s\n", buffer);
		fprintf(stderr, "system_create: end\n");
	#endif
	return 1;
}

#if INLINE_SYSTEM_CREATE
	#define system_create _system_create
#else
	int system_create(System*, char*, char*, char*);
#endif

static inline int _system_copy(System *s, System *s_copy)
{
	int i;

	#if DEBUG_SYSTEM_COPY
		fprintf(stderr, "system_copy: start\n");
		system_print(s_copy, stderr);
	#endif
	
	if (s == s_copy) goto exit0;

	system_init_empty(s);

	s->directory = jb_strdup(s_copy->directory);
	s->name = jb_strdup(s_copy->name);
	s->solution_name = jb_strdup(s_copy->solution_name);
	s->solution_path = jb_strdup(s_copy->solution_path);
	if (!s->name || !s->directory || !s->solution_name || !s->solution_path)
		goto exit1;
	if (s_copy->advances_name)
	{
		s->advances_name = jb_strdup(s_copy->advances_name);
		s->advances_path = jb_strdup(s_copy->advances_path);
		if (!s->advances_name || !s->advances_path) goto exit1;
	}
	else s->advances_name = s->advances_path = NULL;
	if (s_copy->plumes_name)
	{
		s->plumes_name = jb_strdup(s_copy->plumes_name);
		s->plumes_path = jb_strdup(s_copy->plumes_path);
		if (!s->plumes_name || !s->plumes_path) goto exit1;
	}
	else s->plumes_name = s->plumes_path = NULL;
	if (s_copy->contributions_name)
	{
		s->contributions_name = jb_strdup(s_copy->contributions_name);
		s->contributions_path = jb_strdup(s_copy->contributions_path);
		if (!s->contributions_name || !s->contributions_path) goto exit1;
	}
	else s->contributions_name = s->contributions_path = NULL;
	#if DEBUG_SYSTEM_COPY
		fprintf(stderr, "SC dir=%s name=%s solution=%s\n",
			s->directory, s->name, s->solution_name);
	#endif

	memcpy(&s->initial_time, &s_copy->initial_time,
		(size_t)&s->solution_file - (size_t)&s->initial_time);
	#if DEBUG_SYSTEM_COPY
		fprintf(stderr, "SC n=%d nt=%d\n", s_copy->n, s_copy->nt);
	#endif
	s->channel = (Channel*)g_try_malloc((s_copy->n + 1) * sizeof(Channel));
	if (!s->channel) goto exit1;
	#if DEBUG_SYSTEM_COPY
		fprintf(stderr, "SC nt=%d\n", s_copy->nt);
	#endif
	if (s_copy->nt >= 0)
	{
		s->transport =
			(Transport*)g_try_malloc((s_copy->nt + 1) * sizeof(Transport));
		if (!s->transport) goto exit1;
		for (i = 0; i <= s_copy->nt; ++i)
		{
			if (!transport_copy(s->transport + i, s_copy->transport + i))
			{
				system_error(s, message);
				goto exit2;
			}
			++s->nt;
		}
	}
	#if DEBUG_SYSTEM_COPY
		fprintf(stderr, "SC n=%d\n", s_copy->n);
	#endif
	for (i = 0; i <= s_copy->n; ++i)
	{
		if (!channel_copy(s->channel + i, s_copy->channel + i))
		{
			system_error(s, message);
			goto exit2;
		}
		++s->n;
	}

exit0:
	#if DEBUG_SYSTEM_COPY
		system_print(s, stderr);
		fprintf(stderr, "system_copy: end\n");
	#endif
	return 1;

exit1:
	system_error(s_copy, gettext("Not enough memory"));

exit2:
	system_delete(s);
	#if DEBUG_SYSTEM_COPY
		fprintf(stderr, "system_copy: end\n");
	#endif
	return 0;
}

#if INLINE_SYSTEM_COPY
	#define system_copy _system_copy
#else
	int system_copy(System*, System*);
#endif

static inline int _system_junctions_set_up(System *s)
{
	int i, j, k, l, type, type2;
	JunctionData *jd;
	BoundaryFlow *bf;
	Channel *c;
	#if DEBUG_SYSTEM_JUNCTIONS_SET_UP
		fprintf(stderr, "system_junctions_set_up: start\n");
	#endif
	for (i = 0; i <= s->n; ++i)
	{
		for (j = 0, bf = s->channel[i].bf; j <= s->channel[i].n; ++j, ++bf)
		{
			if (bf->type == BOUNDARY_FLOW_TYPE_JUNCTION)
			{
				for (k = 0; k <= JUNCTION_N(bf); ++k, type2 = type)
				{
					jd = JUNCTION_DATA(bf, k);
					for (l = 0; l <= s->n; ++l)
						if (!strcmp(jd->channel_name, s->channel[l].name))
								break;
					if (l > s->n)
					{
						junction_data_error(gettext("Bad channel"));
						goto error1;
					}
					jd->channel = l;
					c = s->channel + l;
					for (l = 0; l <= s->n; ++l)
						if (!strcmp(jd->section, c->bf[l].name)) break;
					if (l > c->n)
					{
						junction_data_error(gettext("Bad cross section"));
						goto error1;
					}
					jd->pos = l;
					if (!jd->section2) jd->pos2 = l;
					else
					{
						for (l = 0; l <= s->n; ++l)
							if (!strcmp(jd->section2, c->bf[l].name)) break;
						if (l > c->n || l < jd->pos)
						{
							junction_data_error(gettext("Bad cross section"));
							goto error1;
						}
					}
					if ((!jd->pos && !jd->pos2 &&
						c->bf->type != BOUNDARY_FLOW_TYPE_JUNCTION) ||
						(jd->pos == c->cg->n && jd->pos2 == c->cg->n &&
						 c->bf[c->n].type != BOUNDARY_FLOW_TYPE_JUNCTION))
					{
						message = g_strconcat(gettext("Channel"), ": ",
							s->channel[i].name, "\n",
							gettext("Channel"), ": ", c->name, "\n",
							gettext("Bad junction"), NULL);
						goto error2;
					}
					if ((!jd->pos && !jd->pos2) ||
						(jd->pos == c->cg->n && jd->pos2 == c->cg->n))
							type = JUNCTION_TYPE_FRONTAL;
					else type = JUNCTION_TYPE_PARALLEL;
					if (k > 0 && type != type2)
					{
						message = g_strconcat(gettext("Channel"), ": ",
							s->channel[i].name, "\n",
							gettext("Channel"), ": ",
							s->channel[JUNCTION_DATA(bf, k-1)->channel].name,
							"\n", gettext("Channel"), ": ", c->name, "\n",
							gettext("Mixing frontal and lateral junctions"),
							NULL);
						goto error2;
					}
				}
			}
		}
	}
	#if DEBUG_SYSTEM_JUNCTIONS_SET_UP
		fprintf(stderr, "system_junctions_set_up: end\n");
	#endif
	return 1;

error1:
	boundary_flow_error(bf, message);
	channel_error(s->channel + i, message);

error2:
	system_error(s, message);
	#if DEBUG_SYSTEM_JUNCTIONS_SET_UP
		fprintf(stderr, "system_junctions_set_up: end\n");
	#endif
	return 0;
}

#if INLINE_SYSTEM_JUNCTIONS_SET_UP
	#define system_junctions_set_up _system_junctions_set_up
#else
	int system_junctions_set_up(System*);
#endif

static inline int _system_controls_set_up(System *s)
{
	int i, j, k, n;
	Channel *channel;
	ChannelGeometry *cg, *cg2;
	CrossSection *cs, *cs2;
	#if DEBUG_SYSTEM_CONTROLS_SET_UP
		fprintf(stderr, "system_controls_set_up: start\n");
	#endif
	for (i = 0, channel = s->channel; i <= s->n; ++i, ++channel)
	{
		cg = channel->cg;
		for (j = 0, cs = cg->cs; j <= cg->n; ++j, ++cs)
		{
			if (cs->type != CROSS_SECTION_TYPE_TIME)
			{
				n = 0;
				for (k = 0; k <= s->n; ++k)
				{
					if (!strcmp(s->channel[k].name, cs->channel)) break;
					n += s->channel[k].cg->n + 1;
				}
				if (k > s->n)
				{
					cross_section_error(cs, gettext("Bad control channel"));
					goto set_up_error;
				}
				cs->control_channel = k;
				cg2 = s->channel[k].cg;
				for (k = 0, cs2 = cg2->cs; k <= cg2->n; ++k, ++cs2)
					if (!strcmp(cs2->name, cs->section)) break;
				if (k > cg2->n)
				{
					cross_section_error
						(cs, gettext("Bad control cross section"));
					goto set_up_error;
				}
				cs->control_section = k;
				cs->control = n + k;
			}
		}
	}
	#if DEBUG_SYSTEM_CONTROLS_SET_UP
		fprintf(stderr, "system_controls_set_up: end\n");
	#endif
	return 1;

set_up_error:
	system_error(s, message);
	#if DEBUG_SYSTEM_CONTROLS_SET_UP
		fprintf(stderr, "system_controls_set_up: end\n");
	#endif
	return 0;
}

#if INLINE_SYSTEM_CONTROLS_SET_UP
	#define system_controls_set_up _system_controls_set_up
#else
	int system_controls_set_up(System*);
#endif

static inline int _system_open_xml(System *s, char *name, char *directory)
{
	int j, k;
	char buffer[JB_BUFFER_SIZE];
	xmlDoc *doc;
	xmlNode *node;
	Transport *transport;
	Channel *channel;

	#if DEBUG_SYSTEM_OPEN_XML
		fprintf(stderr, "system_open_xml: start\n");
	#endif

	k = 0;
	#if DEBUG_SYSTEM_OPEN_XML
		fprintf(stderr, "SOXML: starting\n");
	#endif
	if (!system_create(s, name, directory, buffer)) goto exit2;
	#if DEBUG_SYSTEM_OPEN_XML
		fprintf(stderr, "SOXML: opening file\n");
	#endif
	doc = xmlParseFile(buffer);
	if (!doc)
	{
		jbw_show_error3
			(gettext("System"), name, gettext("Unable to open the file"));
		goto exit2;
	}

	#if DEBUG_SYSTEM_OPEN_XML
		fprintf(stderr, "SOXML: parsing file\n");
	#endif
	node = xmlDocGetRootElement(doc);
	if (xmlStrcmp(node->name, XML_SYSTEM))
	{
		#if DEBUG_SYSTEM_OPEN_XML
			fprintf(stderr, "SOXML node=%s\n", node->name);
		#endif
		jbw_show_error(gettext("Bad XML system"));
		goto exit1;
	}
	s->measured_interval =
		jb_xml_node_get_float_with_default(node, XML_MEASURED_TIME, &j, 0.);
	if (!j)
	{
		jbw_show_error(gettext("Bad measured time"));
		goto exit1;
	}
	if (xmlHasProp(node, XML_SOLUTION_FILE))
		s->solution_name = (char*)xmlGetProp(node, XML_SOLUTION_FILE);
	else
	{
		jbw_show_error(gettext("No solution file"));
		goto exit1;
	}
	s->solution_path =
		(char*)g_build_filename(s->directory, s->solution_name, NULL);
	if (!s->solution_path)
	{
		jbw_show_error(gettext("No solution file"));
		goto exit1;
	}
	if (xmlHasProp(node, XML_ADVANCES))
	{
		s->advances_name = (char*)xmlGetProp(node, XML_ADVANCES);
		s->advances_path =
			(char*)g_build_filename(s->directory, s->advances_name, NULL);
		if (!s->advances_path)
		{
			jbw_show_error(gettext("Bad advances file"));
			goto exit1;
		}
	}
	else s->advances_path = NULL;
	if (xmlHasProp(node, XML_PLUMES))
	{
		s->plumes_name = (char*)xmlGetProp(node, XML_PLUMES);
		s->plumes_path =
			(char*)g_build_filename(s->directory, s->plumes_name, NULL);
		if (!s->plumes_path)
		{
			jbw_show_error(gettext("Bad plumes file"));
			goto exit1;
		}
	}
	else s->plumes_path = NULL;
	if (xmlHasProp(node, XML_CONTRIBUTIONS))
	{
		s->contributions_name = (char*)xmlGetProp(node, XML_CONTRIBUTIONS);
		s->contributions_path =
			(char*)g_build_filename(s->directory, s->contributions_name, NULL);
		if (!s->contributions_path)
		{
			jbw_show_error(gettext("Bad contributions file"));
			goto exit1;
		}
	}
	else s->contributions_path = NULL;
	s->initial_time =
		jb_xml_node_get_time_with_default(node, XML_INITIAL_TIME, &j, 0.);
	if (!j)
	{
		jbw_show_error(gettext("Bad initial time"));
		goto exit1;
	}
	s->observation_time = jb_xml_node_get_time_with_default
		(node, XML_OBSERVATION_TIME, &j, s->initial_time);
	if (!j)
	{
		jbw_show_error(gettext("Bad observation time"));
		goto exit1;
	}
	if (s->observation_time < s->initial_time)
	{
		jbw_show_error(gettext("Observation time lower than initial time"));
		goto exit1;
	}
	s->final_time = jb_xml_node_get_time_with_default
		(node, XML_FINAL_TIME, &j, s->initial_time);
	if (!j)
	{
		jbw_show_error(gettext("Bad final time"));
		goto exit1;
	}
	if (s->final_time < s->initial_time)
	{
		jbw_show_error(gettext("Final time lower than initial time"));
		goto exit1;
	}
	s->cfl = jb_xml_node_get_float_with_default(node, XML_CFL, &j, 0.9);
	if (!j)
	{
		jbw_show_error(gettext("Bad CFL"));
		goto exit1;
	}
	s->implicit =
		jb_xml_node_get_float_with_default(node, XML_IMPLICIT_NUMBER, &j, 0.5);
	if (!j)
	{
		jbw_show_error(gettext("Bad implicit number"));
		goto exit1;
	}
	s->steady_error =
		jb_xml_node_get_float_with_default(node, XML_STEADY_ERROR, &j, 1e-12);
	if (!j)
	{
		jbw_show_error(gettext("Bad steady error"));
		goto exit1;
	}
	s->max_steady_time = jb_xml_node_get_float_with_default
		(node, XML_MAX_STEADY_TIME, &j, INFINITY);
	if (!j)
	{
		jbw_show_error(gettext("Bad maximum steady time"));
		goto exit1;
	}
	s->section_width_min = jb_xml_node_get_float_with_default
		(node, XML_SECTION_WIDTH_MIN, &j, SECTION_WIDTH_MIN);
	if (!j)
	{
		jbw_show_error(gettext("Bad minimum cross section width"));
		goto exit1;
	}
	s->depth_min = jb_xml_node_get_float_with_default
		(node, XML_DEPTH_MIN, &j, FLOW_DEPTH_MIN);
	if (!j)
	{
		jbw_show_error(gettext("Bad minimum depth"));
		goto exit1;
	}
	s->granulometric_coefficient = jb_xml_node_get_float_with_default
		(node, XML_GRANULOMETRIC_COEFFICIENT, &j, GRANULOMETRIC_COEFFICIENT);
	if (!j)
	{
		jbw_show_error(gettext("Bad granulometric coefficient"));
		goto exit1;
	}

	for (j = 0, node = node->children;
		node && !xmlStrcmp(node->name, XML_CHANNEL_TRANSPORT);
		node = node->next)
	{
		#if DEBUG_SYSTEM_OPEN_XML
			fprintf(stderr, "SOXML transport\n");
		#endif
		j = s->nt + 1;
		transport = (Transport*)
			jb_try_realloc(s->transport, (j + 1) * sizeof(Transport));
		if (!transport)
		{
			message = g_strconcat(gettext("Transport"), "\n",
				gettext("Not enough memory"), NULL);
			system_error(s, message);
			goto exit0;
		}
		j = transport_open_xml(transport + j, node);
		s->transport = transport;
		if (j) ++s->nt; else
		{
			system_error(s, message);
			goto exit0;
		}
	}
	for (; node; node = node->next)
	{
		if (!xmlStrcmp(node->name, XML_CHANNEL))
		{
			#if DEBUG_SYSTEM_OPEN_XML
				fprintf(stderr, "SOXML channel\n");
			#endif
			j = s->n + 1;
			channel =
				(Channel*)jb_try_realloc(s->channel, (j + 1) * sizeof(Channel));
			if (!channel)
			{
				message = g_strconcat(gettext("Channel"),
					gettext("Not enough memory"), NULL);
				system_error(s, message);
				goto exit0;
			}
			j = channel_open_xml(channel + j, s->nt, node);
			s->channel = channel;
			if (j) ++s->n; else
			{
				system_error(s, message);
				goto exit0;
			}
		}
		else
		{
			system_error(s, gettext("Bad defined"));
			goto exit0;
		}
	}

	#if DEBUG_SYSTEM_OPEN_XML
		fprintf(stderr, "SOXML: setting up junctions\n");
	#endif
	if (!system_junctions_set_up(s)) goto exit0;
	if (!system_controls_set_up(s)) goto exit0;

	#if DEBUG_SYSTEM_OPEN_XML
		system_print(s, stderr);
	#endif
	k = 1;
	goto exit1;

exit0:
	system_delete(s);

exit1:
	xmlFreeDoc(doc);

exit2:
	#if DEBUG_SYSTEM_OPEN_XML
		fprintf(stderr, "system_open_xml: end\n");
	#endif
	return k;
}

#if INLINE_SYSTEM_OPEN_XML
	#define system_open_xml _system_open_xml
#else
	int system_open_xml(System*, char*, char*);
#endif

static inline void _system_save_xml(System *s)
{
	int i;
	char *buffer;
	xmlDoc *doc;
	xmlNode *node, *child;
	#if DEBUG_SYSTEM_SAVE_XML
		fprintf(stderr, "system_save_xml: start\n");
		fprintf(stderr, "SSX %s/%s\n", s->directory, s->name);
	#endif
	doc=xmlNewDoc((const xmlChar*)"1.0");
	node=xmlNewDocNode(doc, 0, XML_SYSTEM, 0);
	xmlDocSetRootElement(doc, node);
	if (s->initial_time > 0.)
		jb_xml_node_set_time(node, XML_INITIAL_TIME, s->initial_time);
	if (s->observation_time > s->initial_time)
		jb_xml_node_set_time(node, XML_OBSERVATION_TIME, s->observation_time);
	if (s->final_time > s->initial_time)
		jb_xml_node_set_time(node, XML_FINAL_TIME, s->final_time);
	if (s->measured_interval > 0.)
		jb_xml_node_set_float(node, XML_MEASURED_TIME, s->measured_interval);
	xmlSetProp(node, XML_SOLUTION_FILE, (const xmlChar*)s->solution_name);
	jb_xml_node_set_float_with_default(node, XML_CFL, s->cfl, CFL_DEFAULT);
	jb_xml_node_set_float_with_default
		(node, XML_IMPLICIT_NUMBER, s->implicit, 0.5);
	jb_xml_node_set_float_with_default
		(node, XML_STEADY_ERROR, s->steady_error, 1e-12);
	jb_xml_node_set_float_with_default
		(node, XML_MAX_STEADY_TIME, s->max_steady_time, INFINITY);
	jb_xml_node_set_float_with_default
		(node, XML_SECTION_WIDTH_MIN, s->section_width_min, SECTION_WIDTH_MIN);
	jb_xml_node_set_float_with_default
		(node, XML_DEPTH_MIN, s->depth_min, DEPTH_MIN);
	jb_xml_node_set_float_with_default(node, XML_GRANULOMETRIC_COEFFICIENT,
		s->granulometric_coefficient, GRANULOMETRIC_COEFFICIENT);
	for (i = 0; i <= s->nt; ++i)
	{
		child=xmlNewChild(node, 0, XML_TRANSPORT, 0);
		transport_save_xml(s->transport + i, child);
	}
	for (i = 0; i <= s->n; ++i)
	{
		child=xmlNewChild(node, 0, XML_CHANNEL, 0);
		channel_save_xml(s->channel + i, child);
	}
	buffer = (char*)g_build_filename(s->directory, s->name, NULL);
	xmlSaveFormatFile(buffer, doc, 1);
	xmlFreeDoc(doc);
	g_free(buffer);
	#if DEBUG_SYSTEM_SAVE_XML
		fprintf(stderr, "system_save_xml: end\n");
	#endif
}

#if INLINE_SYSTEM_SAVE_XML
	#define system_save_xml _system_save_xml
#else
	void system_save_xml(System*);
#endif

#endif
