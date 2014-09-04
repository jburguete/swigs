/*
SWIGS (Shallow Water in Irregular Geometries Simulator): a software to simulate
transient or steady flows with solute transport in channels, channel networks and rivers

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

#ifndef INITIAL_TRANSPORT__H
#define INITIAL_TRANSPORT__H 1

#include "points.h"

enum InitialTransportType
{
	INITIAL_TRANSPORT_DRY=0,
	INITIAL_TRANSPORT_STEADY=1,
	INITIAL_TRANSPORT_XC=2
};

#define N_INITIAL_TRANSPORT (INITIAL_TRANSPORT_XC+1)

typedef struct
{
	int type,n;
	Point2 *p;
} InitialTransport;

static inline void _initial_transport_print(InitialTransport *it,FILE *file)
{
	int i;
	fprintf(file,"initial_transport_print: start\n");
	fprintf(file,"ITP type=%d\n",it->type);
	switch (it->type)
	{
	case INITIAL_TRANSPORT_XC:
		for (i=0; i<=it->n; ++i)
			fprintf(file,"ITP i=%d x="FWF" c="FWF"\n",i,it->p[i].x,it->p[i].y);
	}
	fprintf(file,"initial_transport_print: end\n");
}

#if INLINE_INITIAL_TRANSPORT_PRINT
	#define initial_transport_print _initial_transport_print
#else
	void initial_transport_print(InitialTransport*,FILE*);
#endif

static inline void _initial_transport_error(char *m)
{
	char *buffer;
	#if DEBUG_INITIAL_TRANSPORT_ERROR
		fprintf(stderr,"initial_transport_error: end\n");
	#endif
	buffer=message;
	message =
		g_strconcat(gettext("Initial transport conditions"), "\n", m, NULL);
	g_free(buffer);
	#if DEBUG_INITIAL_TRANSPORT_ERROR
		fprintf(stderr,"initial_transport_error: end\n");
	#endif
}

#if INLINE_INITIAL_TRANSPORT_ERROR
	#define initial_transport_error _initial_transport_error
#else
	void initial_transport_error(char*);
#endif

static inline void _initial_transport_delete(InitialTransport *it)
{
	#if DEBUG_INITIAL_TRANSPORT_DELETE
		fprintf(stderr,"initial_transport_delete: start\n");
	#endif
	jb_free_null((void**)&it->p);
	#if DEBUG_INITIAL_TRANSPORT_DELETE
		fprintf(stderr,"initial_transport_delete: end\n");
	#endif
}

#if INLINE_INITIAL_TRANSPORT_DELETE
	#define initial_transport_delete _initial_transport_delete
#else
	void initial_transport_delete(InitialTransport*);
#endif

static inline int _initial_transport_copy
	(InitialTransport *it,InitialTransport *it_copy)
{
	register int j;
		
	#if DEBUG_INITIAL_TRANSPORT_COPY
		fprintf(stderr,"initial_transport_copy: start\n");
		initial_transport_print(it_copy,stderr);
	#endif

	if (it == it_copy) goto exit0;

	it->p = NULL;

	it->type = it_copy->type;
	it->n = it_copy->n;
	switch (it->type)
	{
	case INITIAL_TRANSPORT_DRY:
	case INITIAL_TRANSPORT_STEADY:
	    	goto exit0;
	}

	j = (it->n + 1) * sizeof(Point2);
	it->p = (Point2*)g_try_malloc(j);
	if (!it->p) goto exit1;
	memcpy(it->p, it_copy->p, j);

exit0:
	#if DEBUG_INITIAL_TRANSPORT_COPY
		initial_transport_print(it,stderr);
		fprintf(stderr,"initial_transport_copy: end\n");
	#endif
	return 1;

exit1:
	initial_transport_error(gettext("Not enough memory"));
	initial_transport_delete(it);
	#if DEBUG_INITIAL_TRANSPORT_COPY
		fprintf(stderr,"initial_transport_copy: end\n");
	#endif
	return 0;
}

#if INLINE_INITIAL_TRANSPORT_COPY
	#define initial_transport_copy _initial_transport_copy
#else
	int initial_transport_copy(InitialTransport*,InitialTransport*);
#endif

static inline int _initial_transport_order(InitialTransport *it)
{
	int j;
	#if DEBUG_INITIAL_TRANSPORT_ORDER
		fprintf(stderr,"initial_transport_order: start\n");
	#endif
	for (j=0; ++j<=it->n;)
	{
		if (it->p[j].x < it->p[j-1].x)
		{
			initial_transport_error(gettext("Bad order"));
			#if DEBUG_INITIAL_TRANSPORT_ORDER
				fprintf(stderr,"initial_transport_order: end\n");
			#endif
			return 0;
		}
	}
	#if DEBUG_INITIAL_TRANSPORT_ORDER
		fprintf(stderr,"initial_transport_order: end\n");
	#endif
	return 1;
}

#if INLINE_INITIAL_TRANSPORT_ORDER
	#define initial_transport_order _initial_transport_order
#else
	int initial_transport_order(InitialTransport*);
#endif

static inline int _initial_transport_open_xml
	(InitialTransport *it,xmlNode *node)
{
	int i,j;
	char *buffer2;
	JBFLOAT x,y;
	FILE *file;
	xmlChar *buffer;

	#if DEBUG_INITIAL_TRANSPORT_OPEN_XML
		fprintf(stderr,"initial_transport_open_xml: start\n");
	#endif

	it->p = 0;

	if (xmlStrcmp(node->name,XML_INITIAL_TRANSPORT)) goto exit1;

	if (!xmlHasProp(node,XML_TYPE))
	{
		initial_transport_error(gettext("Unknow type"));
		goto exit1;
	}
	buffer=xmlGetProp(node,XML_TYPE);
	if (!xmlStrcmp(buffer,XML_DRY))
	{
		it->type=0;
		goto exit0;
	}
	else if (!xmlStrcmp(buffer,XML_STEADY))
	{
		it->type=1;
		goto exit0;
	}
	else if (!xmlStrcmp(buffer,XML_XC)) it->type=2;
	else
	{
		initial_transport_error(gettext("Unknow type"));
		goto exit2;
	}
	#if DEBUG_INITIAL_TRANSPORT_OPEN_XML
		fprintf(stderr,"ITOX type=%d\n",it->type);
	#endif

	file=jb_xml_node_get_content_file(node,&buffer2);
	if (!file)
	{
		initial_transport_error(gettext("Not enough memory"));
		goto exit2;
	}

	j=-1;
	do
	{
		i=fscanf(file,FRF FRF,&x,&y);
		if (i<2) break;
		++j;
		it->p = (Point2*)jb_try_realloc(it->p, (j + 1) * sizeof(Point3));
		if (!it->p)
		{
			initial_transport_error(gettext("Not enough memory"));
			goto exit3;
		}
		point2_open(it->p+j,x,y);
	}
	while (1);

	it->n = j;
	#if DEBUG_INITIAL_TRANSPORT_OPEN_XML
		fprintf(stderr,"ITOX j=%d i=%d\n",j,i);
		initial_transport_print(it,stderr);
	#endif

	if (j<0 || i>0)
	{
		initial_transport_error(gettext("Bad defined"));
		goto exit3;
	}

	if (!initial_transport_order(it)) goto exit3;

exit0:
	#if DEBUG_INITIAL_TRANSPORT_OPEN_XML
		initial_transport_print(it,stderr);
		fprintf(stderr,"initial_transport_open_xml: end\n");
	#endif
	return 1;

exit3:
	fclose(file);
	xmlFree(buffer2);

exit2:
	xmlFree(buffer);
	initial_transport_delete(it);

exit1:
	#if DEBUG_INITIAL_TRANSPORT_OPEN_XML
		fprintf(stderr,"ITOX error=%s\n",message);
		fprintf(stderr,"initial_transport_open_xml: end\n");
	#endif
	return 0;
}

#if INLINE_INITIAL_TRANSPORT_OPEN_XML
	#define initial_transport_open_xml _initial_transport_open_xml
#else
	int initial_transport_open_xml(InitialTransport*,xmlNode*);
#endif

static inline void _initial_transport_save_xml
	(InitialTransport *it,xmlNode *node)
{
	int i;
	char *buffer,*buffer2,str[JB_BUFFER_SIZE];
	Point2 *p;
		
	#if DEBUG_INITIAL_TRANSPORT_SAVE_XML
		fprintf(stderr,"initial_transport_save_xml: start\n");
		fprintf(stderr,"ITSX type=%d\n",it->type);
	#endif

	switch (it->type)
	{
	case INITIAL_TRANSPORT_DRY:
		xmlSetProp(node,XML_TYPE,XML_DRY);
		goto exit1;
	case INITIAL_TRANSPORT_STEADY:
		xmlSetProp(node,XML_TYPE,XML_STEADY);
		goto exit1;
	case INITIAL_TRANSPORT_XC:
		xmlSetProp(node,XML_TYPE,XML_XC);
		break;
	}

	#if DEBUG_INITIAL_TRANSPORT_SAVE_XML
		fprintf(stderr,"ITSX n=%d\n",it->n);
	#endif
	p = it->p;
	buffer=NULL;
	for (i=0; i<=it->n; ++i, ++p)
	{
		snprintf(str,JB_BUFFER_SIZE,"\n      "FWF2 FWF,p->x,p->y);
		if (!i) buffer2=g_strdup(str);
		else buffer2=g_strconcat(buffer,str,NULL);
		g_free(buffer);
		buffer=buffer2;
	}
	buffer2=g_strconcat(buffer,"\n    ",NULL);
	xmlNodeSetContent(node,(const xmlChar*)buffer2);
	g_free(buffer);
	g_free(buffer2);

exit1:
	#if DEBUG_INITIAL_TRANSPORT_SAVE_XML
		fprintf(stderr,"initial_transport_save_xml: end\n");
	#endif
	return;
}

#if INLINE_INITIAL_TRANSPORT_SAVE_XML
	#define initial_transport_save_xml _initial_transport_save_xml
#else
	void initial_transport_save_xml(InitialTransport*,xmlNode*);
#endif

#endif
