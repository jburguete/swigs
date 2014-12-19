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
} JunctionData;

static inline void _junction_data_print(JunctionData *data, FILE *file)
{
	fprintf(file, "junction_data_print: start\n");
	fprintf(file, "JDP channel=%d pos=%d pos2=%d\n",
		data->channel, data->pos, data->pos2);
	fprintf(file, "junction_data_print: end\n");
}

#if INLINE_JUNCTION_DATA_PRINT
	#define junction_data_print _junction_data_print
#else
	void junction_data_print(JunctionData*, FILE*);
#endif

static inline void
	_junction_data_copy(JunctionData *data, JunctionData *data_copy)
{
	#if DEBUG_JUNCTION_DATA_COPY
		fprintf(file, "junction_data_copy: start\n");
		junction_data_print(data, stderr);
	#endif
	memcpy(data, data_copy, sizeof(JunctionData));	
	#if DEBUG_JUNCTION_DATA_COPY
		fprintf(file, "junction_data_copy: end\n");
	#endif
}

#if INLINE_JUNCTION_DATA_COPY
	#define junction_data_copy _junction_data_copy
#else
	void junction_data_copy(JunctionData*, JunctionData*);
#endif

static inline int _junction_data_open_xml(JunctionData *data, xmlNode *node)
{
	int i, j, k;
	#if DEBUG_JUNCTION_DATA_OPEN_XML
		fprintf(file, "junction_data_open_xml: start\n");
	#endif
	if (xmlStrcmp(node->name, XML_JUNCTION)) goto exit0;
	data->channel = jb_xml_node_get_int(node, XML_CHANNEL, &i) - 1;
	data->pos = jb_xml_node_get_int(node, XML_INITIAL, &j) - 1;
	if (!xmlHasProp(node, XML_FINAL))
	{
		k = 1;
		data->pos2 = data->pos;
	}
	else data->pos2 = jb_xml_node_get_int(node, XML_FINAL, &k) - 1;
	if (!i || !j || !k) goto exit0;
	if (data->pos2 < data->pos)
	{
		message = g_strconcat
			(gettext("Junction data"), "\n", gettext("Bad position"), NULL);
		goto exit1;
	}
	#if DEBUG_JUNCTION_DATA_OPEN_XML
		fprintf(file, "junction_data_open_xml: end\n");
	#endif
	return 1;

exit0:
	message = g_strconcat
		(gettext("Junction data"), "\n", gettext("Bad defined"), NULL);

exit1:
	#if DEBUG_JUNCTION_DATA_OPEN_XML
		fprintf(file, "junction_data_open_xml: end\n");
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
		fprintf(file, "junction_data_save_xml: start\n");
	#endif
	jb_xml_node_set_int(node, XML_CHANNEL, data->channel + 1);
	jb_xml_node_set_int(node, XML_INITIAL, data->pos + 1);
	jb_xml_node_set_int(node, XML_FINAL, data->pos2 + 1);
	#if DEBUG_JUNCTION_DATA_SAVE_XML
		fprintf(file, "junction_data_save_xml: end\n");
	#endif
}

#if INLINE_JUNCTION_DATA_SAVE_XML
	#define junction_data_save_xml _junction_data_save_xml
#else
	void junction_data_save_xml(JunctionData*, xmlNode*);
#endif

#endif

