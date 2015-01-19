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
 * \file junction_data.h
 * \brief Header file to define junction structures and functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2014 Javier Burguete Tolosa.
 */
#ifndef JUNCTION_DATA__H
#define JUNCTION_DATA__H 1

#include "def.h"
#include "jb/jb_win.h"
#include "jb/jb_xml.h"

/**
 * \struct JunctionData
 * \brief Structure to define the junction data.
 */
typedef struct
{
/**
 * \var channel
 * \brief number of the channel to connect.
 * \var pos
 * \brief number of the first channel cross section to connect.
 * \var pos2
 * \brief number of the last channel cross section to connect.
 */
	int channel, pos, pos2;
	char *channel_name, *section, *section2;
} JunctionData;

static inline void _junction_data_print(JunctionData *data, FILE *file)
{
	fprintf(file, "junction_data_print: start\n");
	fprintf(file, "JDP channel=%d pos=%d pos2=%d\n",
		data->channel, data->pos, data->pos2);
	fprintf(file, "JDP channel_name=%s section=%s\n",
		data->channel_name, data->section);
	if (data->section2) fprintf(file, "JDP section2=%s\n", data->section2);
	fprintf(file, "junction_data_print: end\n");
}

#if INLINE_JUNCTION_DATA_PRINT
	#define junction_data_print _junction_data_print
#else
	void junction_data_print(JunctionData*, FILE*);
#endif

static inline void _junction_data_error(char *m)
{
	char *buffer;
	#if DEBUG_JUNCTION_DATA_ERROR
		fprintf(stderr, "junction_data_error: start\n");
	#endif
	buffer = message;
	message = g_strconcat(gettext("Junction data"), "\n", m, NULL);
	g_free(buffer);
	#if DEBUG_JUNCTION_DATA_ERROR
		fprintf(stderr, "junction_data_error: end\n");
	#endif
}

#if INLINE_JUNCTION_DATA_ERROR
	#define junction_data_error _junction_data_error
#else
	void junction_data_error(char*);
#endif

static inline void _junction_data_delete(JunctionData *data)
{
	#if DEBUG_JUNCTION_DATA_DELETE
		fprintf(stderr, "junction_data_delete: start\n");
	#endif
	jb_free_null((void**)&data->channel_name);
	jb_free_null((void**)&data->section);
	jb_free_null((void**)&data->section2);
	#if DEBUG_JUNCTION_DATA_DELETE
		fprintf(stderr, "junction_data_delete: end\n");
	#endif
}

#if INLINE_JUNCTION_DATA_DELETE
	#define junction_data_delete _junction_data_delete
#else
	void junction_data_delete(JunctionData*);
#endif

static inline void _junction_data_init_empty(JunctionData *data)
{
	#if DEBUG_JUNCTION_DATA_INIT_EMPTY
		fprintf(stderr, "junction_data_init_empty: start\n");
	#endif
	data->channel_name = data->section = data->section2 = NULL;
	#if DEBUG_JUNCTION_DATA_INIT_EMPTY
		fprintf(stderr, "junction_data_init_empty: end\n");
	#endif
}

#if INLINE_JUNCTION_DATA_INIT_EMPTY
	#define junction_data_init_empty _junction_data_init_empty
#else
	void junction_data_init_empty(JunctionData*);
#endif

static inline int
	_junction_data_copy(JunctionData *data, JunctionData *data_copy)
{
	#if DEBUG_JUNCTION_DATA_COPY
		fprintf(stderr, "junction_data_copy: start\n");
		junction_data_print(data_copy, stderr);
	#endif
	junction_data_init_empty(data);
	memcpy(data, data_copy, JB_POINTER_SIZE(data->channel_name, data->channel));
	data->channel_name = jb_strdup(data_copy->channel_name);
	data->section = jb_strdup(data_copy->section);
	if (!data->channel_name || !data->section) goto copy_error;
	if (data_copy->section2)
	{
		data->section2 = jb_strdup(data_copy->section2);
		if (!data->section2) goto copy_error;
	}
	#if DEBUG_JUNCTION_DATA_COPY
		fprintf(stderr, "junction_data_copy: end\n");
	#endif
	return 1;

copy_error:
	junction_data_error(gettext("Not enough memory"));
	junction_data_delete(data);
	#if DEBUG_JUNCTION_DATA_COPY
		fprintf(stderr, "junction_data_copy: end\n");
	#endif
	return 0;
}

#if INLINE_JUNCTION_DATA_COPY
	#define junction_data_copy _junction_data_copy
#else
	int junction_data_copy(JunctionData*, JunctionData*);
#endif

static inline int _junction_data_open_xml(JunctionData *data, xmlNode *node)
{
	char *buffer;
	#if DEBUG_JUNCTION_DATA_OPEN_XML
		fprintf(stderr, "junction_data_open_xml: start\n");
	#endif
	junction_data_init_empty(data);
	if (xmlStrcmp(node->name, XML_JUNCTION))
	{
		junction_data_error(gettext("Bad defined"));
		goto exit0;
	}
	if (!xmlHasProp(node, XML_CHANNEL))
	{
		junction_data_error(gettext("Not channel"));
		goto exit0;
	}
	buffer = (char*)xmlGetProp(node, XML_CHANNEL);
	data->channel_name = jb_strdup(buffer);
	xmlFree(buffer);
	if (!data->channel_name)
	{
		junction_data_error(gettext("Not enough memory"));
		goto exit0;
	}
	if (!xmlHasProp(node, XML_INITIAL))
	{
		junction_data_error(gettext("Not initial section"));
		goto exit0;
	}
	buffer = (char*)xmlGetProp(node, XML_INITIAL);
	data->section = jb_strdup(buffer);
	xmlFree(buffer);
	if (!data->section)
	{
		junction_data_error(gettext("Not enough memory"));
		goto exit0;
	}
	if (xmlHasProp(node, XML_FINAL))
	{
		buffer = (char*)xmlGetProp(node, XML_FINAL);
		data->section2 = jb_strdup(buffer);
		xmlFree(buffer);
		if (!data->section2)
		{
			junction_data_error(gettext("Not enough memory"));
			goto exit0;
		}
	}
	#if DEBUG_JUNCTION_DATA_OPEN_XML
		fprintf(stderr, "junction_data_open_xml: end\n");
	#endif
	return 1;

exit0:
	junction_data_delete(data);
	#if DEBUG_JUNCTION_DATA_OPEN_XML
		fprintf(stderr, "junction_data_open_xml: end\n");
	#endif
	return 0;
}

#if INLINE_JUNCTION_DATA_OPEN_XML
	#define junction_data_open_xml _junction_data_open_xml
#else
	int junction_data_open_xml(JunctionData*, xmlNode*);
#endif

static inline void _junction_data_save_xml(JunctionData *data, xmlNode *node)
{
	#if DEBUG_JUNCTION_DATA_SAVE_XML
		fprintf(stderr, "junction_data_save_xml: start\n");
	#endif
	xmlSetProp(node, XML_CHANNEL, (const xmlChar*)data->channel_name);
	xmlSetProp(node, XML_INITIAL, (const xmlChar*)data->section);
	if (data->section2)
		xmlSetProp(node, XML_FINAL, (const xmlChar*)data->section2);
	#if DEBUG_JUNCTION_DATA_SAVE_XML
		fprintf(stderr, "junction_data_save_xml: end\n");
	#endif
}

#if INLINE_JUNCTION_DATA_SAVE_XML
	#define junction_data_save_xml _junction_data_save_xml
#else
	void junction_data_save_xml(JunctionData*, xmlNode*);
#endif

#endif

