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
 * \file channel.h
 * \brief Header file to define channel structures and functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005 - 2014 Javier Burguete Tolosa.
 */
#ifndef CHANNEL__H
#define CHANNEL__H 1

#include "channel_geometry.h"
#include "initial_flow.h"
#include "boundary_flow.h"
#include "channel_transport.h"

/**
 * \struct Channel
 * \brief Structure to define a channel.
 */
typedef struct
{
/**
 * \var n
 * \brief number of flow boundary conditions.
 * \var nt
 * \brief number of transported solutes.
 * \var i
 * \brief index of the first cross section.
 * \var i2
 * \brief index of the last cross section.
 * \var dry
 * \brief 1 if a cell depth is < 0; 0 otherwise.
 * \var cell_size
 * \brief cell size.
 * \var cg
 * \brief geometry data.
 * \var ifc
 * \brief flow initial conditions.
 * \var bf
 * \brief array of flow boundary conditions.
 * \var ct
 * \brief array of transported solutes.
 * \var list_section_changes
 * \brief list of section changes.
 * \var list_section_gauge
 * \brief list of gauge sections.
 * \var list_variable_segments
 * \brief list of variable channel segments.
 * \var name
 * \brief channel name.
 */
	int n, nt, i, i2, dry;
	JBFLOAT cell_size;
	ChannelGeometry cg[1];
	InitialFlow ifc[1];
	BoundaryFlow *bf;
	ChannelTransport *ct;
	GSList *list_section_changes, *list_section_gauge, *list_variable_segments;
	char *name;
} Channel;

extern TransientSection tsnew[2];
extern BoundaryFlow bfnew[3];
extern BoundaryTransport btnew[1];
extern ChannelTransport ctnew[1];
extern Channel chanew[1];

static inline void _channel_print(Channel *c, FILE *file)
{
	int i;
	fprintf(file, "channel_print: start\n");
	fprintf(file, "CP name=%s\n", c->name);
	channel_geometry_print(c->cg, file);
	initial_flow_print(c->ifc, file);
	for (i = 0; i <= c->n; ++i) boundary_flow_print(c->bf + i, file);
	for (i = 0; i <= c->nt; ++i) channel_transport_print(c->ct + i, file);
	fprintf(file, "channel_print: end\n");
}

#if INLINE_CHANNEL_PRINT
	#define channel_print _channel_print
#else
	void channel_print(Channel*, FILE*);
#endif

static inline void _channel_error(Channel *c, char *m)
{
	char *buffer;
	#if DEBUG_CHANNEL_ERROR
		fprintf(stderr, "channel_error: start\n");
	#endif
	buffer = message;
	message = g_strconcat(gettext("Channel"), ": ", c->name, "\n", m, NULL);
	g_free(buffer);
	#if DEBUG_CHANNEL_ERROR
		fprintf(stderr, "channel_error: end\n");
	#endif
}

#if INLINE_CHANNEL_ERROR
	#define channel_error _channel_error
#else
	void channel_error(Channel*, char*);
#endif

static inline void _channel_delete(Channel *c)
{
	int i;
	#if DEBUG_CHANNEL_DELETE
		fprintf(stderr, "channel_delete: start\n");
	#endif
	channel_geometry_delete(c->cg);
	initial_flow_delete(c->ifc);
	for (i = 0; i <= c->n; ++i) boundary_flow_delete(c->bf + i);
	jb_free_null((void**)&c->bf);
	for (i = 0; i <= c->nt; ++i) channel_transport_delete(c->ct + i);
	jb_free_null((void**)&c->ct);
	jb_free_null((void**)&c->name);
	c->n = c->nt = -1;
	#if DEBUG_CHANNEL_DELETE
		fprintf(stderr, "channel_delete: end\n");
	#endif
}

#if INLINE_CHANNEL_DELETE
	#define channel_delete _channel_delete
#else
	void channel_delete(Channel*);
#endif

static inline void _channel_init_empty(Channel *c)
{
	#if DEBUG_CHANNEL_INIT_EMPTY
		fprintf(stderr, "channel_init_empty: start\n");
	#endif
	channel_geometry_init_empty(c->cg);
	initial_flow_init_empty(c->ifc);
	c->n = c->nt = -1;
	c->bf = NULL;
	c->ct = NULL;
	c->name = NULL;
	c->list_section_changes = c->list_section_gauge =
		c->list_variable_segments = NULL;
	#if DEBUG_CHANNEL_INIT_EMPTY
		fprintf(stderr, "channel_init_empty: end\n");
	#endif
}

#if INLINE_CHANNEL_INIT_EMPTY
	#define channel_init_empty _channel_init_empty
#else
	void channel_init_empty(Channel*);
#endif

static inline int _channel_insert_bf(Channel *channel, int i)
{
	int j;
	BoundaryFlow *bf;
	#if DEBUG_CHANNEL_INSERT_BF
		fprintf(stderr, "channel_insert_bf: start\n");
	#endif
	j = channel->n + 2;
	bf = (BoundaryFlow*)g_try_realloc(channel->bf, j * sizeof(BoundaryFlow));
	if (!bf)
	{
		boundary_flow_error(bf, gettext("Not enough memory"));
		goto error_insert_bf;
	}
	channel->bf = bf;
	++channel->n;
	--j;
	bf += j;
	bf->p1 = NULL;
	bf->data = NULL;
	bf->name = NULL;
	for (; j-- >= i; --bf)
	{
		boundary_flow_delete(bf);
		if (!boundary_flow_copy(bf, bf - 1)) goto error_insert_bf;
	}
	#if DEBUG_CHANNEL_INSERT_BF
		fprintf(stderr, "channel_insert_bf: end\n");
	#endif
	return 1;

error_insert_bf:
	#if DEBUG_CHANNEL_INSERT_BF
		fprintf(stderr, "channel_insert_bf: end\n");
	#endif
	return 0;
}

#if INLINE_CHANNEL_INSERT_BF
	#define channel_insert_bf _channel_insert_bf
#else
	int channel_insert_bf(Channel*, int);
#endif

static inline void _channel_remove_bf(Channel *channel, int i)
{
	int j;
	BoundaryFlow *bf;
	#if DEBUG_CHANNEL_REMOVE_BF
		fprintf(stderr, "channel_remove_bf: start\n");
	#endif
	for (j = i, bf = channel->bf + i; j < channel->n; ++j, ++bf)
	{
		boundary_flow_delete(bf);
		boundary_flow_copy(bf, bf + 1);
	}
	boundary_flow_delete(bf);
	channel->bf = g_try_realloc(channel->bf, j * sizeof(BoundaryFlow));
	--channel->n;
	#if DEBUG_CHANNEL_REMOVE_BF
		fprintf(stderr, "channel_remove_bf: end\n");
	#endif
}

#if INLINE_CHANNEL_REMOVE_BF
	#define channel_remove_bf _channel_remove_bf
#else
	void channel_remove_bf(Channel*, int);
#endif

static inline int _channel_insert_ct(Channel *channel, int i)
{
	int j;
	ChannelTransport *ct;
	#if DEBUG_CHANNEL_INSERT_CT
		fprintf(stderr, "channel_insert_ct: start\n");
	#endif
	j = channel->nt + 2;
	ct = (ChannelTransport*)g_try_realloc
		(channel->ct, j * sizeof(ChannelTransport));
	if (!ct)
	{
		channel_transport_error(channel->ct, gettext("Not enough memory"));
		goto error_insert_ct;
	}
	channel->ct = ct;
	++channel->nt;
	--j;
	ct += j;
	ct->n = -1;
	ct->bt = NULL;
	ct->it->p = NULL;
	for (; j-- >= i; --ct)
	{
		channel_transport_delete(ct);
		if (!channel_transport_copy(ct, ct - 1)) goto error_insert_ct;
	}
	#if DEBUG_CHANNEL_INSERT_CT
		fprintf(stderr, "channel_insert_ct: end\n");
	#endif
	return 1;

error_insert_ct:
	#if DEBUG_CHANNEL_INSERT_CT
		fprintf(stderr, "channel_insert_ct: end\n");
	#endif
	return 0;
}

#if INLINE_CHANNEL_INSERT_CT
	#define channel_insert_ct _channel_insert_ct
#else
	int channel_insert_ct(Channel*, ChannelTransport*, int);
#endif

static inline void _channel_remove_ct(Channel *channel, int i)
{
	int j;
	ChannelTransport *ct = channel->ct + i;
	#if DEBUG_CHANNEL_REMOVE_CT
		fprintf(stderr, "channel_remove_ct: start\n");
	#endif
	for (j = i, ct = channel->ct + i; j < channel->nt; ++j, ++ct)
	{
		channel_transport_delete(ct);
		channel_transport_copy(ct, ct + 1);
	}
	channel_transport_delete(ct);
	channel->ct = g_try_realloc(channel->ct, j * sizeof(ChannelTransport));
	--channel->nt;
	#if DEBUG_CHANNEL_REMOVE_CT
		fprintf(stderr, "channel_remove_ct: end\n");
	#endif
}

#if INLINE_CHANNEL_REMOVE_CT
	#define channel_remove_ct _channel_remove_ct
#else
	void channel_remove_ct(Channel*, int);
#endif

static inline int _channel_create(Channel *c, char *name)
{
	char *buffer;
	#if DEBUG_CHANNEL_CREATE
		fprintf(stderr, "channel_create: start\n");
	#endif
	channel_init_empty(c);
	c->name = jb_strdup(name);
	if (!c->name)
	{
		buffer=message;
		message = g_strconcat(gettext("Channel"), ": ", name, "\n",
			gettext("Not enough memory"), NULL);
		g_free(buffer);
		#if DEBUG_CHANNEL_CREATE
			fprintf(stderr, "channel_create: end\n");
		#endif
		return 0;
	}
	#if DEBUG_CHANNEL_CREATE
		fprintf(stderr, "COS %s\n", name);
		fprintf(stderr, "channel_create: end\n");
	#endif
	return 1;
}

#if INLINE_CHANNEL_CREATE
	#define channel_create _channel_create
#else
	int channel_create(Channel*, char*);
#endif

static inline int _channel_copy(Channel *c, Channel *c_copy)
{
	int i;

	#if DEBUG_CHANNEL_COPY
		fprintf(stderr, "channel_copy: start\n");
		channel_print(c_copy, stderr);
	#endif
	
	if (c == c_copy) goto exit0;

	channel_init_empty(c);

	c->cell_size = c_copy->cell_size;

	c->name = jb_strdup(c_copy->name);
	if (!c->name) goto exit3;
	#if DEBUG_CHANNEL_COPY
		fprintf(stderr, "CC=%s\n", c->name);
	#endif

	if (!channel_geometry_copy(c->cg, c_copy->cg) ||
		!initial_flow_copy(c->ifc, c_copy->ifc))
	{
		channel_error(c, message);
		goto exit2;
	}

	c->bf = (BoundaryFlow*)g_try_malloc
		((c_copy->n + 1) * sizeof(BoundaryFlow));
	if (!c->bf) goto exit1;
	for (i = 0; i <= c_copy->n; ++i)
	{
		if (!boundary_flow_copy(c->bf + i, c_copy->bf + i))
		{
			channel_error(c, message);
			goto exit2;
		}
		++c->n;
	}

	if (c_copy->nt >= 0)
	{
		c->ct = (ChannelTransport*)g_try_malloc
			((c_copy->nt + 1) * sizeof(ChannelTransport));
		if (!c->ct) goto exit1;
		for (i = 0; i <= c_copy->nt; ++i)
		{
			if (!channel_transport_copy(c->ct + i, c_copy->ct + i))
			{
				channel_error(c, message);
				goto exit2;
			}
			++c->nt;
		}
	}

exit0:
	#if DEBUG_CHANNEL_COPY
		channel_print(c, stderr);
		fprintf(stderr, "channel_copy: end\n");
	#endif
	return 1;

exit1:
	channel_error(c_copy, gettext("Not enough memory"));

exit2:
	channel_delete(c);

exit3:
	#if DEBUG_CHANNEL_COPY
		fprintf(stderr, "channel_copy: end\n");
	#endif
	return 0;
}

#if INLINE_CHANNEL_COPY
	#define channel_copy _channel_copy
#else
	int channel_copy(Channel*, Channel*);
#endif

static inline int _channel_boundary_check(Channel *c)
{
	register int i;
	register BoundaryFlow *bf = c->bf;
	#if DEBUG_CHANNEL_BOUNDARY_CHECK
		fprintf(stderr, "channel_boundary_check: start\n");
		for (i = 0; i <= c->n; ++i)
			fprintf(stderr, "CBC i=%d pos=%d\n", i, bf[i].pos);
	#endif
	if (bf->pos != 1) goto exit1;
	for (i = 0, ++bf; ++i < c->n; ++bf)
	{
		if (bf->pos < (bf - 1)->pos) goto exit1;
		if (bf->pos2 < 1 || bf->pos2 > c->cg->n + 1)
		{
			boundary_flow_error(bf, gettext("Bad position"));
			goto exit0;
		}
	}
	if (bf->pos != c->cg->n + 1) goto exit1;
	#if DEBUG_CHANNEL_BOUNDARY_CHECK
		fprintf(stderr, "channel_boundary_check: end\n");
	#endif
	return 1;

exit1:
	boundary_flow_error(bf, gettext("Bad order"));

exit0:
	#if DEBUG_CHANNEL_BOUNDARY_CHECK
		fprintf(stderr, "CBC error: %s\n", bf->name);
		fprintf(stderr, "channel_boundary_check: end\n");
	#endif
	return 0;
}

#if INLINE_CHANNEL_BOUNDARY_CHECK
	#define channel_boundary_check _channel_boundary_check
#else
	int channel_boundary_check(Channel*);
#endif

static inline int _channel_parse_node(Channel *c, xmlNode *node, int *hasof)
{
	int i, j;
	char *buffer;
	BoundaryFlow *bf;
	ChannelTransport *ct;
	#if DEBUG_CHANNEL_PARSE_NODE
		fprintf(stderr, "channel_parse_node: start\n");
		fprintf(stderr, "CPN node: %s\n", node->name);
	#endif
	i = 0;
	if (!xmlStrcmp(node->name, XML_CHANNEL_GEOMETRY))
	{
		#if DEBUG_CHANNEL_PARSE_NODE
			fprintf(stderr, "CPN channel geometry\n");
		#endif
		i = channel_geometry_open_xml(c->cg, node);
	}
	else if (!xmlStrcmp(node->name, XML_INITIAL_FLOW))
	{
		#if DEBUG_CHANNEL_PARSE_NODE
			fprintf(stderr, "CPN initial flow\n");
		#endif
		i = initial_flow_open_xml(c->ifc, node);
	}
	else if (!xmlStrcmp(node->name, XML_INLET_FLOW))
	{
		#if DEBUG_CHANNEL_PARSE_NODE
			fprintf(stderr, "CPN inlet flow\n");
		#endif
		if (c->n >= 0)
		{
			boundary_flow_error(c->bf, gettext("Duplicated definition"));
			goto exit0;
		}
		j = 0;
		c->bf = (BoundaryFlow*)g_try_malloc(sizeof(BoundaryFlow));
		if (!c->bf)
		{
			buffer=message;
			message = g_strconcat(gettext("Flow boundary conditions"), "\n",
				gettext("Not enough memory"), NULL);
			g_free(buffer);
			goto exit0;
		}
		i = boundary_flow_open_xml(c->bf, node, -1);
		if (i) ++c->n;
	}
	else if (!xmlStrcmp(node->name, XML_INNER_FLOW))
	{
		#if DEBUG_CHANNEL_PARSE_NODE
			fprintf(stderr, "CPN boundary flow\n");
		#endif
		if (c->n < 0)
		{
			buffer=message;
			message = g_strconcat(gettext("Flow boundary conditions"), "\n",
				gettext("Bad order"), NULL);
			g_free(buffer);
			goto exit0;
		}
		j = c->n + 1;
		bf = (BoundaryFlow*)
			jb_try_realloc(c->bf, (j + 1) * sizeof(BoundaryFlow));
		if (!bf)
		{
			buffer=message;
			message = g_strconcat(gettext("Flow boundary conditions"), "\n",
				gettext("Not enough memory"), NULL);
			g_free(buffer);
			goto exit0;
		}
		i = boundary_flow_open_xml(bf + j, node, 0);
		c->bf = bf;
		if (i) ++c->n;
	}
	else if (!xmlStrcmp(node->name, XML_OUTLET_FLOW))
	{
		#if DEBUG_CHANNEL_PARSE_NODE
			fprintf(stderr, "CPN outlet flow\n");
		#endif
		if (*hasof)
		{
			buffer = message;
			message = g_strconcat(gettext("Outlet flow"), "\n",
				gettext("Duplicated definition"), NULL);
			g_free(buffer);
			goto exit0;
		}
		*hasof = 1;
		if (c->n < 0)
		{
			buffer = message;
			message = g_strconcat(gettext("Flow boundary conditions"), "\n",
				gettext("Bad order"), NULL);
			goto exit0;
		}
		j = c->n + 1;
		bf = (BoundaryFlow*)
			jb_try_realloc(c->bf, (j + 1) * sizeof(BoundaryFlow));
		if (!bf)
		{
			buffer = message;
			message = g_strconcat(gettext("Outlet flow"), "\n",
				gettext("Not enough memory"), NULL);
			g_free(buffer);
			goto exit0;
		}
		i = boundary_flow_open_xml(bf + j, node, 1);
		c->bf = bf;
		if (i) ++c->n;
	}
	else if (!xmlStrcmp(node->name, XML_CHANNEL_TRANSPORT))
	{
		#if DEBUG_CHANNEL_PARSE_NODE
			fprintf(stderr, "CPN channel transport\n");
		#endif
		j = c->nt + 1;
		ct = (ChannelTransport*)
			jb_try_realloc(c->ct, (j + 1) * sizeof(ChannelTransport));
		if (!ct)
		{
			buffer=message;
			message = g_strconcat(gettext("Channel transport"), "\n",
				gettext("Not enough memory"), NULL);
			g_free(buffer);
			goto exit0;
		}
		i = channel_transport_open_xml(ct + j, node);
		c->ct = ct;
		if (i) ++c->nt;
	}
	else
	{
		buffer=message;
		message = g_strconcat((char*)node->name, "\n", gettext("Bad defined"),
			NULL);
		g_free(buffer);
	}

exit0:
	#if DEBUG_CHANNEL_PARSE_NODE
		fprintf(stderr, "channel_parse_node: end\n");
	#endif
	return i;
}

#if INLINE_CHANNEL_PARSE_NODE
	#define channel_parse_node _channel_parse_node
#else
	int channel_parse_node(Channel*, xmlNode *node, int*);
#endif

static inline int _channel_open_xml(Channel *c, int nt, xmlNode *node)
{
	int i, j, hasof;
	char *buffer;

	#if DEBUG_CHANNEL_OPEN_XML
		fprintf(stderr, "channel_open_xml: start\n");
	#endif

	j = hasof = 0;
	#if DEBUG_CHANNEL_OPEN_XML
		fprintf(stderr, "COXML: starting\n");
	#endif

	if (!xmlHasProp(node, XML_CELL_SIZE))
	{
		message = g_strconcat(gettext("Channel"), "\n",
			gettext("Not cell size"), NULL);
		goto exit2;
	}
	c->cell_size = jb_xml_node_get_float(node, XML_CELL_SIZE, &i);
	if (!i || c->cell_size <= 0.)
	{
		message = g_strconcat(gettext("Channel"), "\n",
			gettext("Bad cell size"), NULL);
		goto exit2;
	}

	if (!xmlHasProp(node, XML_NAME))
	{
		message = g_strconcat(gettext("Channel"), "\n", gettext("Not name"),
			NULL);
		goto exit2;
	}
	buffer=(char*)xmlGetProp(node, XML_NAME);
	if (!buffer)
	{
		message = g_strconcat(gettext("Channel"), "\n", gettext("Bad name"),
			NULL);
		goto exit2;
	}
	if (!channel_create(c, buffer)) goto exit2;

	#if DEBUG_CHANNEL_OPEN_XML
		fprintf(stderr, "COXML: parsing file\n");
	#endif
	for (node = node->children; node; node = node->next)
		if (!channel_parse_node(c, node, &hasof))
		{
			channel_error(c, message);
			goto exit0;
		}

	#if DEBUG_CHANNEL_OPEN_XML
		fprintf(stderr, "COXML: correcting boundaries\n");
	#endif
	if (c->n < 1)
	{
		message = g_strconcat(gettext("Flow boundary conditions"), "\n",
			gettext("Undefined"), NULL);
		channel_error(c, message);
		goto exit0;
	}
	if (c->nt != nt)
	{
		message = g_strconcat(gettext("Transport"), "\n",
			gettext("Bad number of solutes"), NULL);
		channel_error(c, message);
		goto exit0;
	}
	c->bf[0].pos = c->bf[0].pos2 = 1;
	c->bf[c->n].pos = c->bf[c->n].pos2 = c->cg->n + 1;
	#if DEBUG_CHANNEL_OPEN_XML
		fprintf(stderr, "COXML: checking boundaries\n");
	#endif
	if (!channel_boundary_check(c))
	{
		channel_error(c, message);
		goto exit0;
	}
	#if DEBUG_CHANNEL_OPEN_XML
		channel_print(c, stderr);
	#endif
	j = 1;
	goto exit2;

exit0:
	channel_delete(c);

exit2:
	#if DEBUG_CHANNEL_OPEN_XML
		fprintf(stderr, "channel_open_xml: end\n");
	#endif
	return j;
}

#if INLINE_CHANNEL_OPEN_XML
	#define channel_open_xml _channel_open_xml
#else
	int channel_open_xml(Channel*, int, xmlNode*);
#endif

static inline void _channel_save_xml(Channel *c, xmlNode *node)
{
	int i;
	xmlNode *child;
	#if DEBUG_CHANNEL_SAVE_XML
		fprintf(stderr, "channel_save_xml: start\n");
		fprintf(stderr, "CSX %s\n", c->name);
	#endif
	jb_xml_node_set_float(node, XML_CELL_SIZE, c->cell_size);
	child = xmlNewChild(node, 0, XML_CHANNEL_GEOMETRY, 0);
	channel_geometry_save_xml(c->cg, child);
	child = xmlNewChild(node, 0, XML_INITIAL_FLOW, 0);
	initial_flow_save_xml(c->ifc, child);
	child = xmlNewChild(node, 0, XML_INLET_FLOW, 0);
	boundary_flow_save_xml(c->bf, child, -1);
	for (i = 0; ++i < c->n;)
	{
		child = xmlNewChild(node, 0, XML_INNER_FLOW, 0);
		boundary_flow_save_xml(c->bf + i, child, 0);
	}
	child = xmlNewChild(node, 0, XML_OUTLET_FLOW, 0);
	boundary_flow_save_xml(c->bf + i, child, 1);
	for (i = 0; i <= c->nt; ++i)
	{
		child = xmlNewChild(node, 0, XML_CHANNEL_TRANSPORT, 0);
		channel_transport_save_xml(c->ct + i, child);
	}
	#if DEBUG_CHANNEL_SAVE_XML
		fprintf(stderr, "channel_save_xml: end\n");
	#endif
}

#if INLINE_CHANNEL_SAVE_XML
	#define channel_save_xml _channel_save_xml
#else
	void channel_save_xml(Channel*, xmlNode*);
#endif

#endif
