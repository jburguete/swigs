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
 * \file initial_flow.h
 * \brief Header file to define initial flow conditions structures and
 *   functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2015 Javier Burguete Tolosa.
 */
#ifndef INITIAL_FLOW__H
#define INITIAL_FLOW__H 1

/**
 * \def N_INITIAL_FLOW_TYPES
 * \brief Macro to define the number of initial flow conditions types.
 */
#define N_INITIAL_FLOW_TYPES (INITIAL_FLOW_TYPE_XUZ + 1)

#include "points.h"

/**
 * \enum InitialFlowType
 * \brief Enumeration to define the initial flow conditions type.
 * \var INITIAL_FLOW_TYPE_DRY
 * \brief dry initial flow conditions.
 * \var INITIAL_FLOW_TYPE_STEADY
 * \brief steady initial flow conditions.
 * \var INITIAL_FLOW_TYPE_XQH
 * \brief tabular (x, Q, h) initial flow conditions.
 * \var INITIAL_FLOW_TYPE_XQZ
 * \brief tabular (x, Q, z) initial flow conditions.
 * \var INITIAL_FLOW_TYPE_XUH
 * \brief tabular (x, u, h) initial flow conditions.
 * \var INITIAL_FLOW_TYPE_XUZ 
 * \brief tabular (x, u, z) initial flow conditions.
 */
enum InitialFlowType
{
  INITIAL_FLOW_TYPE_DRY = 0,
  INITIAL_FLOW_TYPE_STEADY = 1,
  INITIAL_FLOW_TYPE_XQH = 2,
  INITIAL_FLOW_TYPE_XQZ = 3,
  INITIAL_FLOW_TYPE_XUH = 4,
  INITIAL_FLOW_TYPE_XUZ = 5
};

/**
 * \struct InitialFlow
 * \brief Structure to define the initial flow conditions of a channel.
 */
typedef struct
{
/**
 * \var type
 * \brief type.
 * \var n
 * \brief number of tabular points.
 * \var p
 * \brief array of tabular points.
 */
  int type, n;
  Point3 *p;
} InitialFlow;

static inline void
_initial_flow_print (InitialFlow * ifc, FILE * file)
{
  int i;
  fprintf (file, "initial_flow_print: start\n");
  fprintf (file, "IFP type=%d\n", ifc->type);
  switch (ifc->type)
    {
    case INITIAL_FLOW_TYPE_XQH:
    case INITIAL_FLOW_TYPE_XQZ:
    case INITIAL_FLOW_TYPE_XUH:
    case INITIAL_FLOW_TYPE_XUZ:
      for (i = 0; i <= ifc->n; ++i)
        fprintf (file, "IFP i=%d x=" FWF " Q=" FWF " z=" FWF "\n",
                 i, ifc->p[i].x, ifc->p[i].y, ifc->p[i].z);
    }
  fprintf (file, "initial_flow_print: end\n");
}

#if INLINE_INITIAL_FLOW_PRINT
#define initial_flow_print _initial_flow_print
#else
void initial_flow_print (InitialFlow *, FILE *);
#endif

static inline void
_initial_flow_error (char *m)
{
  char *buffer;
#if DEBUG_INITIAL_FLOW_ERROR
  fprintf (stderr, "initial_flow_error: end\n");
#endif
  buffer = message;
  message = g_strconcat (gettext ("Initial flow conditions"), "\n", m, NULL);
  g_free (buffer);
#if DEBUG_INITIAL_FLOW_ERROR
  fprintf (stderr, "initial_flow_error: end\n");
#endif
}

#if INLINE_INITIAL_FLOW_ERROR
#define initial_flow_error _initial_flow_error
#else
void initial_flow_error (char *);
#endif

static inline void
_initial_flow_delete (InitialFlow * ifc)
{
#if DEBUG_INITIAL_FLOW_DELETE
  fprintf (stderr, "initial_flow_delete: start\n");
#endif
  jb_free_null ((void **) &ifc->p);
#if DEBUG_INITIAL_FLOW_DELETE
  fprintf (stderr, "initial_flow_delete: end\n");
#endif
}

#if INLINE_INITIAL_FLOW_DELETE
#define initial_flow_delete _initial_flow_delete
#else
void initial_flow_delete (InitialFlow *);
#endif

static inline void
_initial_flow_init_empty (InitialFlow * ifc)
{
#if DEBUG_INITIAL_FLOW_INIT_EMPTY
  fprintf (stderr, "initial_flow_init_empty: start\n");
#endif
  ifc->p = NULL;
#if DEBUG_INITIAL_FLOW_INIT_EMPTY
  fprintf (stderr, "initial_flow_init_empty: end\n");
#endif
}

#if INLINE_INITIAL_FLOW_INIT_EMPTY
#define initial_flow_init_empty _initial_flow_init_empty
#else
void initial_flow_init_empty (InitialFlow *);
#endif

static inline int
_initial_flow_copy (InitialFlow * ifc, InitialFlow * ifc_copy)
{
  int j;

#if DEBUG_INITIAL_FLOW_COPY
  fprintf (stderr, "initial_flow_copy: start\n");
  initial_flow_print (ifc_copy, stderr);
#endif

  if (ifc == ifc_copy)
    goto exit0;

  initial_flow_init_empty (ifc);

  ifc->type = ifc_copy->type;
  ifc->n = ifc_copy->n;
  switch (ifc->type)
    {
    case INITIAL_FLOW_TYPE_DRY:
    case INITIAL_FLOW_TYPE_STEADY:
      goto exit0;
    }

  j = ifc_copy->n + 1;
  ifc->p = (Point3 *) g_try_malloc (j * sizeof (Point3));
  if (!ifc->p)
    goto exit1;
  memcpy (ifc->p, ifc_copy->p, j * sizeof (Point3));

exit0:
#if DEBUG_INITIAL_FLOW_COPY
  initial_flow_print (ifc, stderr);
  fprintf (stderr, "initial_flow_copy: end\n");
#endif
  return 1;

exit1:
  initial_flow_error (gettext ("Not enough memory"));
  initial_flow_delete (ifc);
#if DEBUG_INITIAL_FLOW_COPY
  fprintf (stderr, "initial_flow_copy: end\n");
#endif
  return 0;
}

#if INLINE_INITIAL_FLOW_COPY
#define initial_flow_copy _initial_flow_copy
#else
int initial_flow_copy (InitialFlow *, InitialFlow *);
#endif

static inline int
_initial_flow_check (InitialFlow * ifc)
{
  int j;
#if DEBUG_INITIAL_FLOW_CHECK
  fprintf (stderr, "initial_flow_check: start\n");
#endif
  for (j = 0; ++j <= ifc->n;)
    {
      if (ifc->p[j].x < ifc->p[j - 1].x)
        {
          initial_flow_error (gettext ("Bad order"));
#if DEBUG_INITIAL_FLOW_CHECK
          fprintf (stderr, "initial_flow_check: end\n");
#endif
          return 0;
        }
    }
#if DEBUG_INITIAL_FLOW_CHECK
  fprintf (stderr, "initial_flow_check: end\n");
#endif
  return 1;
}

#if INLINE_INITIAL_FLOW_CHECK
#define initial_flow_check _initial_flow_check
#else
int initial_flow_check (InitialFlow *);
#endif

static inline int
_initial_flow_open_xml (InitialFlow * ifc, xmlNode * node)
{
  int i, j;
  char *buffer2;
  JBFLOAT x, y, z;
  FILE *file;
  xmlChar *buffer;

#if DEBUG_INITIAL_FLOW_OPEN_XML
  fprintf (stderr, "initial_flow_open_xml: start\n");
#endif

  initial_flow_init_empty (ifc);

  if (xmlStrcmp (node->name, XML_INITIAL_FLOW))
    goto exit1;

  if (!xmlHasProp (node, XML_TYPE))
    {
      initial_flow_error (gettext ("Unknow type"));
      goto exit1;
    }
  buffer = xmlGetProp (node, XML_TYPE);
  if (!xmlStrcmp (buffer, XML_DRY))
    {
      ifc->type = INITIAL_FLOW_TYPE_DRY;
      goto exit0;
    }
  else if (!xmlStrcmp (buffer, XML_STEADY))
    {
      ifc->type = INITIAL_FLOW_TYPE_STEADY;
      goto exit0;
    }
  else if (!xmlStrcmp (buffer, XML_XQH))
    ifc->type = INITIAL_FLOW_TYPE_XQH;
  else if (!xmlStrcmp (buffer, XML_XQZ))
    ifc->type = INITIAL_FLOW_TYPE_XQZ;
  else if (!xmlStrcmp (buffer, XML_XUH))
    ifc->type = INITIAL_FLOW_TYPE_XUH;
  else if (!xmlStrcmp (buffer, XML_XUZ))
    ifc->type = INITIAL_FLOW_TYPE_XUZ;
  else
    {
      initial_flow_error (gettext ("Unknow type"));
      goto exit2;
    }
#if DEBUG_INITIAL_FLOW_OPEN_XML
  fprintf (stderr, "IFOX type=%d\n", ifc->type);
#endif

  file = jb_xml_node_get_content_file (node, &buffer2);
  if (!file)
    {
      initial_flow_error (gettext ("Not enough memory"));
      goto exit2;
    }

  j = -1;
  do
    {
      i = fscanf (file, FRF FRF FRF, &x, &y, &z);
      if (i < 3)
        break;
      ++j;
      ifc->p = (Point3 *) jb_try_realloc (ifc->p, (j + 1) * sizeof (Point3));
      if (!ifc->p)
        {
          initial_flow_error (gettext ("Not enough memory"));
          goto exit3;
        }
      point3_open (ifc->p + j, x, y, z);
    }
  while (1);

  ifc->n = j;
#if DEBUG_INITIAL_FLOW_OPEN_XML
  fprintf (stderr, "IFOX j=%d i=%d\n", j, i);
  initial_flow_print (ifc, stderr);
#endif

  if (j < 0 || i > 0)
    {
      initial_flow_error (gettext ("Bad defined"));
      goto exit3;
    }

  if (!initial_flow_check (ifc))
    goto exit3;

exit0:
#if DEBUG_INITIAL_FLOW_OPEN_XML
  initial_flow_print (ifc, stderr);
  fprintf (stderr, "initial_flow_open_xml: end\n");
#endif
  return 1;

exit3:
  fclose (file);
  xmlFree (buffer2);

exit2:
  xmlFree (buffer);
  initial_flow_delete (ifc);

exit1:
#if DEBUG_INITIAL_FLOW_OPEN_XML
  fprintf (stderr, "IFOX error\n");
  fprintf (stderr, "initial_flow_open_xml: end\n");
#endif
  return 0;
}

#if INLINE_INITIAL_FLOW_OPEN_XML
#define initial_flow_open_xml _initial_flow_open_xml
#else
int initial_flow_open_xml (InitialFlow *, xmlNode *);
#endif

static inline void
_initial_flow_save_xml (InitialFlow * ifc, xmlNode * node)
{
  int i;
  char *buffer, *buffer2, str[JB_BUFFER_SIZE];
  Point3 *p;

#if DEBUG_INITIAL_FLOW_SAVE_XML
  fprintf (stderr, "initial_flow_save_xml: start\n");
  fprintf (stderr, "IFSX type=%d\n", ifc->type);
#endif

  switch (ifc->type)
    {
    case INITIAL_FLOW_TYPE_DRY:
      xmlSetProp (node, XML_TYPE, XML_DRY);
      goto exit1;
    case INITIAL_FLOW_TYPE_STEADY:
      xmlSetProp (node, XML_TYPE, XML_STEADY);
      goto exit1;
    case INITIAL_FLOW_TYPE_XQH:
      xmlSetProp (node, XML_TYPE, XML_XQH);
      break;
    case INITIAL_FLOW_TYPE_XQZ:
      xmlSetProp (node, XML_TYPE, XML_XQZ);
      break;
    case INITIAL_FLOW_TYPE_XUH:
      xmlSetProp (node, XML_TYPE, XML_XUH);
      break;
    default:
      xmlSetProp (node, XML_TYPE, XML_XUZ);
    }

#if DEBUG_INITIAL_FLOW_SAVE_XML
  fprintf (stderr, "IFSX n=%d\n", ifc->n);
#endif
  p = ifc->p;
  buffer = NULL;
  for (i = 0; i <= ifc->n; ++i, ++p)
    {
      snprintf (str, JB_BUFFER_SIZE, "\n    " FWF2 FWF2 FWF, p->x, p->y, p->z);
      if (!i)
        buffer2 = g_strdup (str);
      else
        buffer2 = g_strconcat (buffer, str, NULL);
      g_free (buffer);
      buffer = buffer2;
    }
  buffer2 = g_strconcat (buffer, "\n  ", NULL);
  xmlNodeSetContent (node, (const xmlChar *) buffer2);
  g_free (buffer);
  g_free (buffer2);

exit1:
#if DEBUG_INITIAL_FLOW_SAVE_XML
  fprintf (stderr, "initial_flow_save_xml: end\n");
#endif
  return;
}

#if INLINE_INITIAL_FLOW_SAVE_XML
#define initial_flow_save_xml _initial_flow_save_xml
#else
void initial_flow_save_xml (InitialFlow *, xmlNode *);
#endif

#endif
