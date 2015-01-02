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
 * \file transport.h
 * \brief Header file to define transported solute structures and functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2014 Javier Burguete Tolosa.
 */
#ifndef TRANSPORT__H
#define TRANSPORT__H 1

#include "jb/jb_xml.h"
#include "def.h"

/**
 * \struct Transport
 * \brief Structure to define a transported solute.
 */
typedef struct
{
/**
 * \var solubility
 * \brief solubility.
 * \var danger
 * \brief danger concentration.
 * \var name
 * \brief name.
 */
	JBFLOAT solubility, danger;
	char *name;
} Transport;

static inline void _transport_print(Transport *t, FILE *file)
{
	fprintf(file, "transport_print: start\n");
	fprintf(file, "%s solubility="FWF" danger="FWF"\n",
		t->name, t->solubility, t->danger);
	fprintf(file, "transport_print: end\n");
}

#if INLINE_TRANSPORT_PRINT
	#define transport_print _transport_print
#else
	void transport_print(Transport*, FILE*);
#endif

static inline void _transport_error(Transport *t, char *m)
{
	char *buffer;
	#if DEBUG_TRANSPORT_ERROR
		fprintf(stderr, "transport_error: start\n");
	#endif
	buffer = message;
	message = g_strconcat(gettext("Transport"), ": ", t->name, "\n", m, NULL);
	g_free(buffer);
	#if DEBUG_TRANSPORT_ERROR
		fprintf(stderr, "transport_error: end\n");
	#endif
}

#if INLINE_TRANSPORT_ERROR
	#define transport_error _transport_error
#else
	void transport_error(Transport*, char*);
#endif

static inline void _transport_delete(Transport *t)
{
	#if DEBUG_TRANSPORT_DELETE
		fprintf(stderr, "transport_delete: start\n");
	#endif
	jb_free_null((void**)&t->name);
	#if DEBUG_TRANSPORT_DELETE
		fprintf(stderr, "transport_delete: end\n");
	#endif
}

#if INLINE_TRANSPORT_DELETE
	#define transport_delete _transport_delete
#else
	void transport_delete(Transport*);
#endif

static inline void _transport_init_empty(Transport *t)
{
	#if DEBUG_TRANSPORT_INIT_EMPTY
		fprintf(stderr, "transport_init_empty: start\n");
	#endif
	t->name = NULL;
	#if DEBUG_TRANSPORT_INIT_EMPTY
		fprintf(stderr, "transport_init_empty: end\n");
	#endif
}

#if INLINE_TRANSPORT_INIT_EMPTY
	#define transport_init_empty _transport_init_empty
#else
	void transport_init_empty(Transport*);
#endif

static inline int _transport_create(Transport *t, char *name)
{
	#if DEBUG_TRANSPORT_CREATE
		fprintf(stderr, "transport_create: start\n");
	#endif
	t->name = jb_strdup(name);
	if (!t->name)
	{
		message = g_strconcat(gettext("Transport"), ": ", name, "\n",
			gettext("Not enough memory"), NULL);
		#if DEBUG_TRANSPORT_CREATE
			fprintf(stderr, "transport_create: end\n");
		#endif
		return 0;
	}
	#if DEBUG_TRANSPORT_CREATE
		fprintf(stderr, "TOS %s\n", name);
		fprintf(stderr, "transport_create: end\n");
	#endif
	return 1;
}

#if INLINE_TRANSPORT_CREATE
	#define transport_create _transport_create
#else
	int transport_create(Transport*, char*);
#endif

static inline int _transport_copy(Transport *t, Transport *t_copy)
{
	#if DEBUG_TRANSPORT_COPY
		fprintf(stderr, "transport_copy: start\n");
		transport_print(t_copy, stderr);
	#endif
	
	if (t == t_copy) goto exit0;

	if (!transport_create(t, t_copy->name)) goto exit1;

	t->solubility = t_copy->solubility;
	t->danger = t_copy->danger;

exit0:
	#if DEBUG_TRANSPORT_COPY
		transport_print(t, stderr);
		fprintf(stderr, "transport_copy: end\n");
	#endif
	return 1;

exit1:
	#if DEBUG_TRANSPORT_COPY
		fprintf(stderr, "transport_copy: end\n");
	#endif
	return 0;
}

#if INLINE_TRANSPORT_COPY
	#define transport_copy _transport_copy
#else
	int transport_copy(Transport*, Transport*);
#endif

static inline int _transport_open_xml(Transport *t, xmlNode *node)
{
	int j;
	char *buffer;

	#if DEBUG_TRANSPORT_OPEN_XML
		fprintf(stderr, "transport_open_xml: start\n");
	#endif

	transport_init_empty(t);

	j = 0;

	if (!xmlHasProp(node, XML_NAME))
	{
		message = g_strconcat(gettext("Transport"), "\n",
			gettext("Not name"), NULL);
		goto exit2;
	}
	buffer = (char*)xmlGetProp(node, XML_NAME);
	if (!buffer)
	{
		message = g_strconcat(gettext("Transport"), "\n",
			gettext("Bad name"), NULL);
		goto exit2;
	}
	t->solubility
		= jb_xml_node_get_float_with_default(node, XML_SOLUBILITY, &j, 0.);
	if (!j)
	{
		message = g_strconcat(gettext("Transport"), ": ", buffer,
			"\n", gettext("Bad solubility"), NULL);
		goto exit1;
	}
	t->danger
		= jb_xml_node_get_float_with_default(node, XML_DANGER, &j, 0.);
	if (!j)
	{
		message = g_strconcat(gettext("Transport"), ": ", buffer,
			"\n", gettext("Bad danger concentration"), NULL);
		goto exit1;
	}
	if (!transport_create(t, buffer))
	{
		j = 0;
		goto exit1;
	}
	#if DEBUG_TRANSPORT_OPEN_XML
		transport_print(t, stderr);
	#endif

exit1:
	xmlFree(buffer);

exit2:
	#if DEBUG_TRANSPORT_OPEN_XML
		fprintf(stderr, "transport_open_xml: end\n");
	#endif
	return j;
}

#if INLINE_TRANSPORT_OPEN_XML
	#define transport_open_xml _transport_open_xml
#else
	int transport_open_xml(Transport*, xmlNode*);
#endif

static inline void _transport_save_xml(Transport *t, xmlNode *node)
{
	#if DEBUG_TRANSPORT_SAVE_XML
		fprintf(stderr, "transport_save_xml: start\n");
		transport_print(t, stderr);
	#endif
	xmlSetProp(node, XML_NAME, (const xmlChar*)t->name);
	jb_xml_node_set_float_with_default(node, XML_SOLUBILITY, t->solubility, 0.);
	jb_xml_node_set_float_with_default(node, XML_DANGER, t->danger, 0.);
	#if DEBUG_TRANSPORT_SAVE_XML
		fprintf(stderr, "transport_save_xml: end\n");
	#endif
}

#if INLINE_TRANSPORT_SAVE_XML
	#define transport_save_xml _transport_save_xml
#else
	void transport_save_xml(Transport*, xmlNode*);
#endif

#endif
