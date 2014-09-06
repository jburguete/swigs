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

#ifndef SYSTEM__H
#define SYSTEM__H 1

#include "channel.h"

/*
JBFLOAT granulometric_coefficient: aerodynamical friction coefficient of
	granulometric roughness
JBFLOAT section_width_min: coefficient of the minimum width in pressurized cross
	sections
JBFLOAT depth_min: minimum flow depth allowing the water movement
*/

typedef struct
{
	int n,nt;
	JBDOUBLE initial_time,observation_time,final_time,measured_interval;
	JBFLOAT cfl,implicit,steady_error,max_steady_time,section_width_min,
		depth_min,granulometric_coefficient;
	FILE *solution_file;
	Transport *transport;
	Channel *channel;
	char *name,*directory,*solution_name,*solution_path;
} System;

extern System sys[1],sysnew[1];

static inline void _system_print(System *s,FILE *file)
{
	int i;
	fprintf(file,"system_print: start\n");
	fprintf(file,"SP system=%s/%s n=%d nt=%d\n",
		s->directory, s->name, s->n, s->nt);
	fprintf(file,"SP solution_name=%s\n", s->solution_name);
	fprintf(file,"SP solution_path=%s\n", s->solution_path);
	fprintf(file,
		"SP initial_time="FWL" observation_time="FWL" final_time="FWL"\n"
		"SP measured_interval="FWL"\n",
		s->initial_time, s->observation_time, s->final_time,
		s->measured_interval);
	fprintf(file,"SP cfl="FWF" implicit="FWF" steady_error="FWF"\n",
		s->cfl, s->implicit, s->steady_error);
	fprintf(file,"SP max_steady_time="FWF" section_width_min="FWF"\n",
		s->max_steady_time, s->section_width_min);
	fprintf(file,"SP depth_min="FWF" granulometric_coefficient="FWF"\n",
		s->depth_min, s->granulometric_coefficient);
	for (i=0; i<=s->nt; ++i) transport_print(s->transport + i, file);
	for (i=0; i<=s->n; ++i) channel_print(s->channel + i, file);
	fprintf(file,"system_print: end\n");
}

#if INLINE_SYSTEM_PRINT
	#define system_print _system_print
#else
	void system_print(System*,FILE*);
#endif

static inline void _system_error(System *s,char *m)
{
	#if DEBUG_SYSTEM_ERROR
		fprintf(stderr,"system_error: start\n");
	#endif
	jbw_show_error3(gettext("System"), s->name, m);
	jb_free_null((void**)&message);
	#if DEBUG_SYSTEM_ERROR
		fprintf(stderr,"system_error: end\n");
	#endif
}

#if INLINE_SYSTEM_ERROR
	#define system_error _system_error
#else
	void system_error(System*,char*);
#endif

static inline void _system_init(System *s)
{
	#if DEBUG_SYSTEM_INIT
		fprintf(stderr,"system_init: start\n");
	#endif
	s->n = s->nt = -1;
	s->transport = NULL;
	s->channel = NULL;
	s->name = s->directory = s->solution_name = s->solution_path = NULL;
	#if DEBUG_SYSTEM_INIT
		fprintf(stderr,"system_init: end\n");
	#endif
}

#if INLINE_SYSTEM_INIT
	#define system_init _system_init
#else
	void system_init(System*);
#endif

static inline void _system_delete(System *s)
{
	int i;
	#if DEBUG_SYSTEM_DELETE
		fprintf(stderr,"system_delete: start\n");
	#endif
	for (i=0; i<=s->nt; ++i) transport_delete(s->transport + i);
	for (i=0; i<=s->n; ++i) channel_delete(s->channel + i);
	jb_free_null((void**)&s->transport);
	jb_free_null((void**)&s->channel);
	jb_free_null((void**)&s->name);
	jb_free_null((void**)&s->directory);
	jb_free_null((void**)&s->solution_name);
	jb_free_null((void**)&s->solution_path);
	s->n = s->nt = -1;
	#if DEBUG_SYSTEM_DELETE
		fprintf(stderr,"system_delete: end\n");
	#endif
}

#if INLINE_SYSTEM_DELETE
	#define system_delete _system_delete
#else
	void system_delete(System*);
#endif

static inline int _system_copy(System *s,System *s_copy)
{
	int i;

	#if DEBUG_SYSTEM_COPY
		fprintf(stderr,"system_copy: start\n");
		system_print(s_copy,stderr);
	#endif
	
	if (s == s_copy) goto exit0;

	s->n = s->nt = -1;
	s->transport = NULL;
	s->channel = NULL;

	s->directory = jb_strdup(s_copy->directory);
	s->name = jb_strdup(s_copy->name);
	s->solution_name = jb_strdup(s_copy->solution_name);
	s->solution_path = jb_strdup(s_copy->solution_path);
	if (!s->name || !s->directory || !s->solution_name || !s->solution_path)
		goto exit1;
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
		for (i=0; i<=s_copy->nt; ++i)
		{
			if (!transport_copy(s->transport + i, s_copy->transport + i))
			{
				system_error(s,message);
				goto exit2;
			}
			++s->nt;
		}
	}
	#if DEBUG_SYSTEM_COPY
		fprintf(stderr, "SC n=%d\n", s_copy->n);
	#endif
	for (i=0; i<=s_copy->n; ++i)
	{
		if (!channel_copy(s->channel + i, s_copy->channel + i))
		{
			system_error(s,message);
			goto exit2;
		}
		++s->n;
	}

exit0:
	#if DEBUG_SYSTEM_COPY
		system_print(s,stderr);
		fprintf(stderr,"system_copy: end\n");
	#endif
	return 1;

exit1:
	system_error(s_copy,gettext("Not enough memory"));

exit2:
	system_delete(s);
	#if DEBUG_SYSTEM_COPY
		fprintf(stderr,"system_copy: end\n");
	#endif
	return 0;
}

#if INLINE_SYSTEM_COPY
	#define system_copy _system_copy
#else
	int system_copy(System*,System*);
#endif

static inline int _system_junctions_test(System *s)
{
	int i,j,k,type,type2;
	JunctionData *jd;
	BoundaryFlow *bf;
	Channel *c;
	#if DEBUG_SYSTEM_JUNCTIONS_TEST
		fprintf(stderr,"system_junctions_test: start\n");
	#endif
	for (i=0; i<=s->n; ++i)
	{
		for (j=0, bf=s->channel[i].bf; j<=s->channel[i].n; ++j, ++bf)
		{
			if (bf->type == BOUNDARY_FLOW_JUNCTION)
			{
				for (k=0; k<=JUNCTION_N(bf); ++k, type2=type)
				{
					jd=JUNCTION_DATA(bf,k);
					if (jd->channel < 0 || jd->channel > s->n)
					{
						message=g_strconcat(
							gettext("Channel"), ": ", s->channel[i].name, "\n",
							gettext("Boundary flow"), ": ", bf->name, "\n",
							gettext("Junction"), ": ",
							gettext("Bad channel number"), NULL);
						goto error1;
					}
					c = s->channel + jd->channel;
					if (jd->pos < 0 || jd->pos2 < 0 ||
						jd->pos > c->cg->n || jd->pos2 > c->cg->n)
					{
						message=g_strconcat(
							gettext("Channel"), ": ", s->channel[i].name, "\n",
							gettext("Boundary flow"), ": ", bf->name, "\n",
							gettext("Junction"), ": ",
							gettext("Bad section number"), NULL);
						goto error1;
					}
					if ((!jd->pos && !jd->pos2 &&
						c->bf->type != BOUNDARY_FLOW_JUNCTION) ||
						(jd->pos == c->cg->n && jd->pos2 == c->cg->n &&
						 c->bf[c->n].type != BOUNDARY_FLOW_JUNCTION))
					{
						message=g_strconcat(gettext("Channel"), ": ",
							s->channel[i].name, "\n",
							gettext("Channel"), ": ", c->name, "\n",
							gettext("Bad junction"), NULL);
						goto error1;
					}
					if ((!jd->pos && !jd->pos2) ||
						(jd->pos == c->cg->n && jd->pos2 == c->cg->n))
							type=JUNCTION_TYPE_FRONTAL;
					else type=JUNCTION_TYPE_PARALLEL;
					if (k>0 && type!=type2)
					{
						message=g_strconcat(gettext("Channel"), ": ",
							s->channel[i].name, "\n",
							gettext("Channel"), ": ",
							s->channel[JUNCTION_DATA(bf, k-1)->channel].name,
							"\n", gettext("Channel"), ": ", c->name, "\n",
							gettext("Mixing frontal and lateral junctions"),
							NULL);
						goto error1;
					}
				}
			}
		}
	}
	#if DEBUG_SYSTEM_JUNCTIONS_TEST
		fprintf(stderr,"system_junctions_test: end\n");
	#endif
	return 1;

error1:
	system_error(s,message);
	#if DEBUG_SYSTEM_JUNCTIONS_TEST
		fprintf(stderr,"system_junctions_test: end\n");
	#endif
	return 0;
}

#if INLINE_SYSTEM_JUNCTIONS_TEST
	#define system_junctions_test _system_junctions_test
#else
	int system_junctions_test(System*);
#endif

static inline int _system_open_start
	(System *s,char *name,char *directory,char *buffer)
{
	#if DEBUG_SYSTEM_OPEN_START
		fprintf(stderr,"system_open_start: start\n");
	#endif
	system_delete(s);
	s->directory = jb_strdup(directory);
	s->name = jb_strdup(name);
	if (!s->name)
	{
		jbw_show_error3(gettext("System"),name,gettext("Not enough memory"));
		#if DEBUG_SYSTEM_OPEN_START
			fprintf(stderr,"system_open_start: end\n");
		#endif
		return 0;
	}
	snprintf(buffer,JB_BUFFER_SIZE,"%s/%s",directory,name);
	#if DEBUG_SYSTEM_OPEN_START
		fprintf(stderr,"SOS %s\n",buffer);
		fprintf(stderr,"system_open_start: end\n");
	#endif
	return 1;
}

#if INLINE_SYSTEM_OPEN_START
	#define system_open_start _system_open_start
#else
	int system_open_start(System*,char*,char*,char*);
#endif

static inline int _system_open_xml(System *s,char *name,char *directory)
{
	int j,k;
	char buffer[JB_BUFFER_SIZE];
	xmlDoc *doc;
	xmlNode *node;
	Transport *transport;
	Channel *channel;

	#if DEBUG_SYSTEM_OPEN_XML
		fprintf(stderr,"system_open_xml: start\n");
	#endif

	k=0;
	#if DEBUG_SYSTEM_OPEN_XML
		fprintf(stderr,"SOXML: starting\n");
	#endif
	if (!system_open_start(s,name,directory,buffer)) goto exit2;
	#if DEBUG_SYSTEM_OPEN_XML
		fprintf(stderr,"SOXML: opening file\n");
	#endif
	doc=xmlParseFile(buffer);
	if (!doc)
	{
		jbw_show_error3
			(gettext("System"),name,gettext("Unable to open the file"));
		goto exit2;
	}

	#if DEBUG_SYSTEM_OPEN_XML
		fprintf(stderr,"SOXML: parsing file\n");
	#endif
	node = xmlDocGetRootElement(doc);
	if (xmlStrcmp(node->name,XML_SYSTEM))
	{
		#if DEBUG_SYSTEM_OPEN_XML
			fprintf(stderr,"SOXML node=%s\n",node->name);
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
	if (xmlHasProp(node,XML_SOLUTION_FILE))
		s->solution_name = (char*)xmlGetProp(node,XML_SOLUTION_FILE);
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

	for (j=0, node=node->children;
		node && !xmlStrcmp(node->name,XML_CHANNEL_TRANSPORT);
		node=node->next)
	{
		#if DEBUG_SYSTEM_OPEN_XML
			fprintf(stderr,"SOXML transport\n");
		#endif
		j = s->nt + 1;
		transport = (Transport*)
			jb_try_realloc(s->transport, (j + 1) * sizeof(Transport));
		if (!transport)
		{
			message = g_strconcat(gettext("Transport"), "\n",
				gettext("Not enough memory"), NULL);
			system_error(s,message);
			goto exit0;
		}
		j = transport_open_xml(transport + j, node);
		s->transport = transport;
		if (j) ++s->nt; else
		{
			system_error(s,message);
			goto exit0;
		}
	}
	for (; node; node=node->next)
	{
		if (!xmlStrcmp(node->name,XML_CHANNEL))
		{
			#if DEBUG_SYSTEM_OPEN_XML
				fprintf(stderr,"SOXML channel\n");
			#endif
			j = s->n + 1;
			channel =
				(Channel*)jb_try_realloc(s->channel, (j + 1) * sizeof(Channel));
			if (!channel)
			{
				message = g_strconcat(gettext("Channel"),
					gettext("Not enough memory"), NULL);
				system_error(s,message);
				goto exit0;
			}
			j = channel_open_xml(channel + j, s->nt, node);
			s->channel = channel;
			if (j) ++s->n; else
			{
				system_error(s,message);
				goto exit0;
			}
		}
		else
		{
			system_error(s,gettext("Bad defined"));
			goto exit0;
		}
	}

	#if DEBUG_SYSTEM_OPEN_XML
		fprintf(stderr,"SOXML: testing junctions\n");
	#endif
	if (!system_junctions_test(s)) goto exit0;

	#if DEBUG_SYSTEM_OPEN_XML
		system_print(s,stderr);
	#endif
	k=1;
	goto exit1;

exit0:
	system_delete(s);

exit1:
	xmlFreeDoc(doc);

exit2:
	#if DEBUG_SYSTEM_OPEN_XML
		fprintf(stderr,"system_open_xml: end\n");
	#endif
	return k;
}

#if INLINE_SYSTEM_OPEN_XML
	#define system_open_xml _system_open_xml
#else
	int system_open_xml(System*,char*,char*);
#endif

static inline void _system_save_xml(System *s)
{
	int i;
	char *buffer;
	xmlDoc *doc;
	xmlNode *node,*child;
	#if DEBUG_SYSTEM_SAVE_XML
		fprintf(stderr,"system_save_xml: start\n");
		fprintf(stderr,"SSX %s/%s\n",s->directory,s->name);
	#endif
	doc=xmlNewDoc((const xmlChar*)"1.0");
	node=xmlNewDocNode(doc,0,XML_SYSTEM,0);
	xmlDocSetRootElement(doc,node);
	if (s->initial_time > 0.)
		jb_xml_node_set_time(node, XML_INITIAL_TIME, s->initial_time);
	if (s->observation_time > s->initial_time)
		jb_xml_node_set_time(node, XML_OBSERVATION_TIME, s->observation_time);
	if (s->final_time > s->initial_time)
		jb_xml_node_set_time(node, XML_FINAL_TIME, s->final_time);
	if (s->measured_interval > 0.)
		jb_xml_node_set_float(node, XML_MEASURED_TIME, s->measured_interval);
	xmlSetProp(node, XML_SOLUTION_FILE, (const xmlChar*)s->solution_name);
	if (s->cfl != CFL_DEFAULT) jb_xml_node_set_float(node, XML_CFL, s->cfl);
	if (s->implicit != 0.5)
		jb_xml_node_set_float(node, XML_IMPLICIT_NUMBER, s->implicit);
	if (s->steady_error != 1e-12)
		jb_xml_node_set_float(node, XML_STEADY_ERROR, s->steady_error);
	if (s->max_steady_time != INFINITY)
		jb_xml_node_set_float(node, XML_MAX_STEADY_TIME, s->max_steady_time);
	if (s->section_width_min != SECTION_WIDTH_MIN)
		jb_xml_node_set_float
			(node, XML_SECTION_WIDTH_MIN, s->section_width_min);
	if (s->depth_min != DEPTH_MIN)
		jb_xml_node_set_float(node, XML_DEPTH_MIN, s->depth_min);
	if (s->granulometric_coefficient != GRANULOMETRIC_COEFFICIENT)
		jb_xml_node_set_float
			(node, XML_GRANULOMETRIC_COEFFICIENT, s->granulometric_coefficient);
	for (i=0; i<=s->nt; ++i)
	{
		child=xmlNewChild(node,0,XML_TRANSPORT,0);
		transport_save_xml(s->transport + i, child);
	}
	for (i=0; i<=s->n; ++i)
	{
		child=xmlNewChild(node,0,XML_CHANNEL,0);
		channel_save_xml(s->channel + i, child);
	}
	buffer = (char*)g_build_filename(s->directory, s->name, NULL);
	xmlSaveFormatFile(buffer,doc,1);
	xmlFreeDoc(doc);
	g_free(buffer);
	#if DEBUG_SYSTEM_SAVE_XML
		fprintf(stderr,"system_save_xml: end\n");
	#endif
}

#if INLINE_SYSTEM_SAVE_XML
	#define system_save_xml _system_save_xml
#else
	void system_save_xml(System*);
#endif

#endif

