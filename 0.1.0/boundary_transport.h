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

/*
	BOUNDARY SPECIFICATIONS
	0: Pointwise source
		p1[0]=mass
	1: Large source
		p1[i]=time, p2[i]=discharge
*/

#ifndef BOUNDARY_TRANSPORT__H
#define BOUNDARY_TRANSPORT__H 1

#include "points.h"

#define N_BOUNDARY_TRANSPORT	2

enum BoundaryTransportType
{
	BOUNDARY_TRANSPORT_M=0,
	BOUNDARY_TRANSPORT_Q=1,
};

typedef struct
{
	int type,pos,pos2,n,i,i2;
	JBFLOAT length,*p1,*p2;
	char *name;
} BoundaryTransport;

extern int simulating;

static inline void _boundary_transport_print(BoundaryTransport *bt,FILE *file)
{
	int i;
	fprintf(file,"boundary_transport_print: start\n");
	fprintf(file,"BTP name=%s\n",bt->name);
	fprintf(file,"BTP type=%d pos=%d pos2=%d\n",bt->type,bt->pos,bt->pos2);
	switch (bt->type)
	{
	case BOUNDARY_TRANSPORT_M:
		fprintf(file,"BTP p1="FWF" p2="FWF"\n",bt->p1[0],bt->p2[0]);
		break;
	case BOUNDARY_TRANSPORT_Q:
		for (i=0; i<=bt->n; ++i) fprintf(file,"BTP i=%d p1="FWF" p2="FWF"\n",
			i,bt->p1[i],bt->p2[i]);
	}
	fprintf(file,"boundary_transport_print: end\n");
}

#if INLINE_BOUNDARY_TRANSPORT_PRINT
	#define boundary_transport_print _boundary_transport_print
#else
	void boundary_transport_print(BoundaryTransport*,FILE*);
#endif

static inline void _boundary_transport_error(BoundaryTransport *bt,char *m)
{
	char *buffer;
	#if DEBUG_BOUNDARY_TRANSPORT_ERROR
		fprintf(stderr,"boundary_transport_error: start\n");
	#endif
	buffer=message;
	message=g_strconcat(gettext("Transport boundary condition"), ": ", bt->name,
		"\n", m, NULL);
	g_free(buffer);
	#if DEBUG_BOUNDARY_TRANSPORT_ERROR
		fprintf(stderr,"boundary_transport_error: end\n");
	#endif
}

#if INLINE_BOUNDARY_TRANSPORT_ERROR
	#define boundary_transport_error _boundary_transport_error
#else
	void boundary_transport_error(BoundaryTransport*,char*);
#endif

static inline void _boundary_transport_delete(BoundaryTransport *bt)
{
	#if DEBUG_BOUNDARY_TRANSPORT_DELETE
		fprintf(stderr,"boundary_transport_delete: start\n");
	#endif
	jb_free_null((void**)&bt->p1);
	jb_free_null((void**)&bt->name);
	#if DEBUG_BOUNDARY_TRANSPORT_DELETE
		fprintf(stderr,"boundary_transport_delete: end\n");
	#endif
}

#if INLINE_BOUNDARY_TRANSPORT_DELETE
	#define boundary_transport_delete _boundary_transport_delete
#else
	void boundary_transport_delete(BoundaryTransport*);
#endif

static inline int _boundary_transport_copy
	(BoundaryTransport *bt,BoundaryTransport *bt_copy)
{
	int i,j;
	#if DEBUG_BOUNDARY_TRANSPORT_COPY
		fprintf(stderr,"boundary_transport_copy: start\n");
		boundary_transport_print(bt_copy,stderr);
	#endif

	if (bt == bt_copy) goto exit0;

	memcpy(bt, bt_copy, (size_t)&bt->i - (size_t)bt);

	bt->p1 = 0;

	bt->name = jb_strdup(bt_copy->name);
	if (!bt->name) goto exit1;

	j = bt->n + 1;
	i = j * 2 * sizeof(JBFLOAT);
	bt->p1 = (JBFLOAT*)g_try_malloc(i);
	if (!bt->p1) goto exit1;
	bt->p2 = bt->p1 + j;
	memcpy(bt->p1, bt_copy->p1, i);

exit0:
	#if DEBUG_BOUNDARY_TRANSPORT_COPY
		boundary_transport_print(bt,stderr);
		fprintf(stderr,"boundary_transport_copy: end\n");
	#endif
	return 1;

exit1:
	boundary_transport_error(bt_copy,gettext("Not enough memory"));
	boundary_transport_delete(bt);
	#if DEBUG_BOUNDARY_TRANSPORT_COPY
		fprintf(stderr,"boundary_transport_copy: end\n");
	#endif
	return 0;
}

#if INLINE_BOUNDARY_TRANSPORT_COPY
	#define boundary_transport_copy _boundary_transport_copy
#else
	int boundary_transport_copy(BoundaryTransport*,BoundaryTransport*);
#endif

static inline int _boundary_transport_open_xml
	(BoundaryTransport *bt,xmlNode *node)
{
	int i,j;
	char *buffer;
	void *p;
	JBFLOAT x,y;
	FILE *file;
	#if DEBUG_BOUNDARY_TRANSPORT_OPEN_XML
		fprintf(stderr,"boundary_transport_open_xml: start\n");
	#endif

	bt->p1 = 0;
	bt->name = 0;

	if (!xmlHasProp(node,XML_NAME))
	{
		message = g_strconcat(gettext("Transport boundary condition"), "\n",
			gettext("Not name"), NULL);
		goto exit1;
	}
	buffer=(char*)xmlGetProp(node,XML_NAME);
	bt->name = jb_strdup(buffer);
	if (!bt->name)
	{
		message = g_strconcat(gettext("Transport boundary condition"), ": ",
			buffer, "\n", gettext("Not enough memory"), NULL);
		xmlFree(buffer);
		goto exit1;
	}
	xmlFree(buffer);
	#if DEBUG_BOUNDARY_TRANSPORT_OPEN_XML
		fprintf(stderr,"BTOX name=%s\n",bt->name);
	#endif


	if (!xmlHasProp(node,XML_INITIAL))
	{
		boundary_transport_error(bt,gettext("Bad position"));
		goto exit1;
	}
	bt->pos = jb_xml_node_get_int(node,XML_INITIAL,&i);
	if (i!=1)
	{
		boundary_transport_error(bt,gettext("Bad position"));
		goto exit1;
	}
	#if DEBUG_BOUNDARY_TRANSPORT_OPEN_XML
		fprintf(stderr,"BTOX position=%d\n",bt->pos);
	#endif

	bt->pos2 = jb_xml_node_get_int_with_default(node, XML_FINAL, &i, bt->pos);
	if (i!=1)
	{
		boundary_transport_error(bt,gettext("Bad position"));
		goto exit1;
	}
	#if DEBUG_BOUNDARY_TRANSPORT_OPEN_XML
		fprintf(stderr,"BTOX position2=%d\n",bt->pos2);
	#endif

	if (!xmlHasProp(node,XML_TYPE))
	{
		boundary_transport_error(bt,gettext("Unknow type"));
		goto exit1;
	}
	buffer=(char*)xmlGetProp(node,XML_TYPE);
	#if DEBUG_BOUNDARY_TRANSPORT_OPEN_XML
		fprintf(stderr,"BTOX type=%s\n",buffer);
	#endif

	if (!xmlStrcmp((const xmlChar*)buffer,XML_M))
	{
		bt->type=0;
		if (!xmlHasProp(node,XML_TIME) || !xmlHasProp(node,XML_MASS))
		{
			boundary_transport_error(bt,gettext("Bad defined"));
			goto exit2;
		}
		x=jb_xml_node_get_float(node,XML_TIME,&i);
		y=jb_xml_node_get_float(node,XML_MASS,&j);
		if (i!=1 || j!=1)
		{
			boundary_transport_error(bt,gettext("Bad defined"));
			goto exit2;
		}
		bt->p1 = (JBFLOAT*)g_try_malloc(2 * sizeof(JBFLOAT));
		if (!bt->p1)
		{
			boundary_transport_error(bt,gettext("Not enough memory"));
			goto exit2;
		}
		bt->p2 = bt->p1 + 1;
		bt->p1[0] = x;
		bt->p2[0] = y;
		bt->n = 0;
	}
	else if (!xmlStrcmp((const xmlChar*)buffer,XML_Q))
	{
		bt->type=1;
		xmlFree(buffer);
		file=jb_xml_node_get_content_file(node,&buffer);
		if (!file)
		{
			boundary_transport_error(bt,gettext("Not enough memory"));
			goto exit1;
		}
		j=-1;
		p=0;
		do
		{
			i=fscanf(file,FRF FRF,&x,&y);
			if (i<2) break;
			++j;
			p = (Point2*)jb_try_realloc(p, (j + 1) * sizeof(Point2));
			if (!p)
			{
				boundary_transport_error(bt,gettext("Not enough memory"));
				goto exit4;
			}
			point2_open(((Point2*)p)+j,x,y);
		}
		while (1);
		bt->n = j;
		++j;
		bt->p1 = (JBFLOAT*)g_try_malloc(j * 2 * sizeof(JBFLOAT));
		if (!bt->p1)
		{
			boundary_transport_error(bt,gettext("Not enough memory"));
			goto exit4;
		}
		bt->p2 = bt->p1 + j;
		for (i=j; --i>=0;)
		{
			bt->p1[i] = ((Point2*)p)[i].x;
			bt->p2[i] = ((Point2*)p)[i].y;
		}
		jb_free_null((void**)&p);
		fclose(file);
	}
	else
	{
		boundary_transport_error(bt,gettext("Unknow type"));
		goto exit2;
	}

	#if DEBUG_BOUNDARY_TRANSPORT_OPEN_XML
		boundary_transport_print(bt,stderr);
		fprintf(stderr,"boundary_transport_open_xml: end\n");
	#endif
	xmlFree(buffer);
	return 1;

exit4:
	g_free(p);
	fclose(file);

exit2:
	xmlFree(buffer);

exit1:
	boundary_transport_delete(bt);
	#if DEBUG_BOUNDARY_TRANSPORT_OPEN_XML
		fprintf(stderr,"boundary_transport_open_xml: end\n");
	#endif
	return 0;
}

#if INLINE_BOUNDARY_TRANSPORT_OPEN_XML
	#define boundary_transport_open_xml _boundary_transport_open_xml
#else
	int boundary_transport_open_xml(BoundaryTransport*,xmlNode*);
#endif

static inline void _boundary_transport_save_xml
	(BoundaryTransport *bt,xmlNode *node)
{
	int i;
	char *buffer,*buffer2,str[JB_BUFFER_SIZE];
	const xmlChar *bttype[]={XML_M,XML_Q};

	#if DEBUG_BOUNDARY_TRANSPORT_SAVE_XML
		fprintf(stderr,"boundary_transport_save_xml: start\n");
		boundary_transport_print(bt,stderr);
	#endif

	xmlSetProp(node,XML_NAME,(const xmlChar*)bt->name);
	jb_xml_node_set_int(node,XML_INITIAL,bt->pos);
	if (bt->pos != bt->pos2) jb_xml_node_set_int(node,XML_FINAL,bt->pos2);
	xmlSetProp(node,XML_TYPE,bttype[bt->type]);

	switch (bt->type)
	{
	case BOUNDARY_TRANSPORT_M:
		jb_xml_node_set_float(node,XML_TIME,bt->p1[0]);
		jb_xml_node_set_float(node,XML_MASS,bt->p2[0]);
		break;
	case BOUNDARY_TRANSPORT_Q:
		buffer=NULL;
		for (i=0; i<=bt->n; ++i)
		{
			snprintf(str,JB_BUFFER_SIZE,"\n      "FWF2 FWF,bt->p1[i],bt->p2[i]);
			if (!i) buffer2=g_strdup(str);
			else buffer2=g_strconcat(buffer,str,NULL);
			g_free(buffer);
			buffer=buffer2;
		}
		buffer2=g_strconcat(buffer,"\n  ",NULL);
		xmlNodeSetContent(node,(const xmlChar*)buffer2);
		g_free(buffer);
		g_free(buffer2);
		break;
	}

	#if DEBUG_BOUNDARY_TRANSPORT_SAVE_XML
		fprintf(stderr,"boundary_transport_save_xml: end\n");
	#endif
}

#if INLINE_BOUNDARY_TRANSPORT_SAVE_XML
	#define boundary_transport_save_xml _boundary_transport_save_xml
#else
	void boundary_transport_save_xml(BoundaryTransport*,xmlNode*);
#endif

static inline JBDOUBLE _boundary_transport_parameter
	(BoundaryTransport *bt,JBDOUBLE t)
{
	if (!simulating) return bt->p2[0];
	return jbm_farray_interpolate(t,bt->p1,bt->p2,bt->n);
}

#if INLINE_BOUNDARY_TRANSPORT_PARAMETER
	#define boundary_transport_parameter _boundary_transport_parameter
#else
	JBDOUBLE boundary_transport_parameter(BoundaryTransport*,JBDOUBLE);
#endif

#endif
