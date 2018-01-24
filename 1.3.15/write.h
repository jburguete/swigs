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
 * \file write.h
 * \brief Header file to define file inputs and outputs.
 * \author Javier Burguete Tolosa.
 * \copyright Copyright 2005-2015 Javier Burguete Tolosa. All rights reserved.
 */
#ifndef WRITE_H
#define WRITE_H 1

#include "flow_scheme.h"

extern int nstep;
extern JBDOUBLE t0, ti, tf;

static inline int
_write_data (FILE * file, Variables * v, Parameters * p,
             TransportParameters * tp, int n, int vsize, int nt)
{
  int i, j;
#if DEBUG_WRITE_DATA
  if (t >= DEBUG_TIME)
    fprintf (stderr, "write_data: start\n");
#endif
  variables_actualize (v);
  j = (n + 1) * vsize;
  i = fwrite (v->x, 1, j, file);
  if (i != j)
    j = 0;
  ++nstep;
#if DEBUG_WRITE_DATA
  char *prueba;
  if (t >= DEBUG_TIME)
    {
      prueba = (char *) v->x;
      fprintf (stderr, "WD v1=%d v2=%d\n", (unsigned char) prueba[i - 1],
               (unsigned char) prueba[i]);
      fprintf (stderr, "WD n=%d i=%d j=%d vsize=%d\n", n, i, j, vsize);
      fprintf (stderr, "write_data: end\n");
    }
#endif
  return j;
}

#if INLINE_WRITE_DATA
#define write_data _write_data
#else
int write_data
  (FILE *, Variables *, Parameters *, TransportParameters *, int, int, int);
#endif

static inline int
_read_data (FILE * file, Variables * v, int n, int vsize)
{
  int j;
#if DEBUG_READ_DATA
  fprintf (stderr, "read_data: start\n");
  fprintf (stderr, "RD n=%d\n", n);
#endif
  j = fread (v->x, vsize, n + 1, file);
#if DEBUG_READ_DATA
  int i;
  fprintf (stderr, "RD j=%d\n", j);
  for (i = 0; i <= n; ++i)
    fprintf (stderr, "RD i=%d x=" FWF " Q=" FWF "\n", i, v->x[i], v->Q[i]);
  fprintf (stderr, "read_data: end\n");
#endif
  return j;
}

#if INLINE_READ_DATA
#define read_data _read_data
#else
int read_data (FILE *, Variables *, int, int);
#endif

static inline void _write_variables
  (FILE * file, Variables * v, int i, int n, int nt)
{
  int j;
  JBFLOAT *p;
#if DEBUG_WRITE_VARIABLES
  fprintf (stderr, "write_variables: start\n");
#endif
  fprintf (file, FWF2 FWF2 FWF2 FWF2 FWF2 FWF2 FWF2 FWF, v->x[i], v->Q[i],
           v->A[i], v->B[i], v->zs[i], v->zb[i], v->zmax[i], v->beta[i]);
  for (j = 0, p = v->p + i, ++n; j <= nt; ++j, p += n)
    fprintf (file, " " FWF, *p);
  fprintf (file, "\n");
#if DEBUG_WRITE_VARIABLES
  fprintf (stderr, "write_variables: end\n");
#endif
}

#if INLINE_WRITE_VARIABLES
#define write_variables _write_variables
#else
void write_variables (FILE *, Variables *, int, int, int);
#endif

static inline void _read_profile
  (FILE * file, Variables * v, int n, int profile, int vsize)
{
#if DEBUG_READ_PROFILE
  fprintf (stderr, "read_profile: start\n");
  fseek (file, 0L, SEEK_END);
  fprintf (stderr, "RP: length=%ld\n", ftell (file));
  fprintf (stderr, "RP: n=%d profile=%d\n", n, profile);
#endif
  fseek (file, vsize * (n + 1) * profile, SEEK_SET);
  read_data (file, v, n, vsize);
#if DEBUG_READ_PROFILE
  fprintf (stderr, "read_profile: end\n");
#endif
}

#if INLINE_READ_PROFILE
#define read_profile _read_profile
#else
void read_profile (FILE *, Variables *, int, int, int);
#endif

static inline void
_write_profile (FILE * file_read, FILE * file_write, int i,
                int i2, int n, int profile, int *vsize, int nt)
{
  int j;
  Variables v[1];
#if DEBUG_WRITE_PROFILE
  fprintf (stderr, "write_profile: start\n");
#endif
  v->x = NULL;
  if (!variables_create (v, n, vsize, nt))
    {
      jbw_show_error2 (gettext ("Save solution"),
                       gettext ("Not enough memory"));
#if DEBUG_WRITE_PROFILE
      fprintf (stderr, "write_profile: end\n");
#endif
      return;
    }
#if DEBUG_WRITE_PROFILE
  fprintf (stderr, "WP reading profile\n");
#endif
  read_profile (file_read, v, n, profile, *vsize);
#if DEBUG_WRITE_PROFILE
  fprintf (stderr, "WP writing variables\n");
#endif
  for (j = i; j <= i2; ++j)
    write_variables (file_write, v, j, n, nt);
#if DEBUG_WRITE_PROFILE
  fprintf (stderr, "WP freeing variables\n");
#endif
  jb_free_null ((void **) &v->x);
#if DEBUG_WRITE_PROFILE
  fprintf (stderr, "write_profile: end\n");
#endif
}

#if INLINE_WRITE_PROFILE
#define write_profile _write_profile
#else
void write_profile (FILE *, FILE *, int, int, int, int, int *, int);
#endif

static inline void
_write_evolution (FILE * file_read, FILE * file_write, int n,
                  int section, int *vsize, Channel * channel)
{
  long int i, j;
  Variables v[1];
#if DEBUG_WRITE_EVOLUTION
  fprintf (stderr, "write_evolution: start\n");
#endif
  v->x = NULL;
  if (!variables_create (v, n, vsize, channel->nt))
    {
      jbw_show_error2
        (gettext ("Time evolution"), gettext ("Not enough memory"));
#if DEBUG_WRITE_EVOLUTION
      fprintf (stderr, "write_evolution: end\n");
#endif
      return;
    }
  section = channel->cg->cs[section].i;
  j = jb_flength (file_read) / (*vsize * (n + 1));
  for (i = 0; i < j; ++i)
    {
      read_data (file_read, v, n, *vsize);
      v->x[section] = fmin (i * ti, tf);
      write_variables (file_write, v, section, n, channel->nt);
    }
  jb_free_null ((void **) &v->x);
#if DEBUG_WRITE_EVOLUTION
  fprintf (stderr, "write_evolution: end\n");
#endif
}

#if INLINE_WRITE_EVOLUTION
#define write_evolution _write_evolution
#else
void write_evolution (FILE *, FILE *, int, int, int *, Channel *);
#endif

static inline void
_read_evolution (int channel, int section, int type1,
                 int type2, JBFLOAT ** x, JBFLOAT ** y1, JBFLOAT ** z1, int *nt,
                 int vsize)
{
  int ii, nv;
  long int i, j, k;
  FILE *file;
#if DEBUG_READ_EVOLUTION
  fprintf (stderr, "read_evolution: start\n");
#endif
  *x = *y1 = *z1 = 0;
  if (t == 0.)
    {
#if DEBUG_READ_EVOLUTION
      fprintf (stderr, "read_evolution: end\n");
#endif
      return;
    }
  file = g_fopen (sys->solution_path, "rb");
  if (!file)
    {
      jbw_show_error2
        (gettext ("Solution"), gettext ("Unable to open the file"));
#if DEBUG_READ_EVOLUTION
      fprintf (stderr, "read_evolution: end\n");
#endif
      return;
    }
  nv = vsize / sizeof (JBFLOAT);
#if DEBUG_READ_EVOLUTION
  fprintf (stderr, "RE nv=%d\n", nv);
#endif
  register JBDOUBLE kk;
  JBFLOAT p[nv];
  k = vsize * (n + 1);
  j = jb_flength (file) / k;
#if DEBUG_READ_EVOLUTION
  fprintf (stderr, "RE j=%ld\n", j);
#endif
  *x = (JBFLOAT *) g_try_malloc (j * 3 * sizeof (JBFLOAT));
  if (!*x)
    {
      fclose (file);
      jbw_show_error2
        (gettext ("Time evolution"), gettext ("Not enough memory"));
#if DEBUG_READ_EVOLUTION
      fprintf (stderr, "read_evolution: end\n");
#endif
      return;
    }
  *y1 = *x + j;
  *z1 = *y1 + j;
  fseek (file, sys->channel[channel].cg->cs[section].i * sizeof (JBFLOAT),
         SEEK_SET);
#if DEBUG_READ_EVOLUTION
  fprintf (stderr, "RE t1=%d t2=%d j=%d n=%d\n",
           type1, type2, sys->channel[channel].cg->cs[section].i, n);
#endif
  for (i = 0; i < j; ++i)
    {
      (*x)[i] = i * ti;
#if DEBUG_READ_EVOLUTION
      fprintf (stderr, "RE pos=%ld\n", ftell (file));
#endif
      for (ii = 0; ii < nv; ++ii)
        {
          fread (p + ii, sizeof (JBFLOAT), 1, file);
          fseek (file, n * sizeof (JBFLOAT), SEEK_CUR);
#if DEBUG_READ_EVOLUTION
          fprintf (stderr, "RE pos=%ld\n", ftell (file));
#endif
        }
      switch (type1)
        {
        case 0:
          kk = p[1];
          break;
        case 1:
          kk = p[2] - p[5];
          break;
        case 2:
          kk = p[2];
          break;
        case 3:
          if (p[3] > 0.)
            kk = p[1] / p[3];
          else
            kk = 0.;
          break;
        case 4:
          kk = p[3] * sqrt (JBM_G * p[3] / (p[4] * p[7]));
          if (kk > 0.)
            kk = p[1] / kk;
          else
            kk = 0.;
          break;
        case 5:
          kk = p[3];
          break;
        case 6:
          kk = p[4];
          break;
        default:
          kk = p[type1 + 1];
        }
      (*y1)[i] = kk;
      switch (type2)
        {
        case 0:
          kk = p[1];
          break;
        case 1:
          kk = p[2] - p[5];
          break;
        case 2:
          kk = p[2];
          break;
        case 3:
          if (p[3] > 0.)
            kk = p[1] / p[3];
          else
            kk = 0.;
          break;
        case 4:
          kk = p[3] * sqrt (JBM_G * p[3] / (p[4] * p[7]));
          if (kk > 0.)
            kk = p[1] / kk;
          else
            kk = 0.;
          break;
        case 5:
          kk = p[3];
          break;
        case 6:
          kk = p[4];
          break;
        default:
          kk = p[type2 + 1];
        }
      (*z1)[i] = kk;
    }
  --i;
  (*x)[i] = fmin ((*x)[i], tf);
  *nt = i;
  fclose (file);
#if DEBUG_READ_EVOLUTION
  for (i = 0; i <= *nt; ++i)
    fprintf (stderr, "x=" FWF " y1=" FWF " z1=" FWF "\n",
             (*x)[i], (*y1)[i], (*z1)[i]);
  fprintf (stderr, "read_evolution: end\n");
#endif
}

#if INLINE_READ_EVOLUTION
#define read_evolution _read_evolution
#else
void read_evolution
  (int, int, int, int, JBFLOAT **, JBFLOAT **, JBFLOAT **, int *, int);
#endif

static inline void
_write_advances (FILE * file, Parameters * p, int n)
{
  int i;
#if DEBUG_WRITE_ADVANCES
  fprintf (stderr, "write_advances: start\n");
#endif
  for (i = 0; i <= n; ++i)
    fprintf (file, FWL " " FWF " " FWF "\n", p[i].x, p[i].ta, p[i].tr);
#if DEBUG_WRITE_ADVANCES
  fprintf (stderr, "write_advances: end\n");
#endif
}

#if INLINE_WRITE_ADVANCES
#define write_advances _write_advances
#else
void write_advances (FILE *, Parameters *, int);
#endif

static inline void
_write_plumes_channel (FILE * file, Parameters * p,
                       TransportParameters * tp, Transport * transport,
                       Channel * c)
{
  int i, j, k;
  JBFLOAT danger;
#if DEBUG_WRITE_PLUMES_CHANNEL
  fprintf (stderr, "write_plumes_channel: start\n");
#endif
  for (i = 0; i <= c->nt; ++i)
    {
      danger = transport[i].danger;
      for (j = c->i; tp[j].c < danger && j < c->i2;)
        ++j;
      for (k = c->i2; tp[k].c < danger && k > 0;)
        --k;
      fprintf (file, FWF " " FWF " ", p[j].x, p[k].x);
    }
#if DEBUG_WRITE_PLUMES_CHANNEL
  fprintf (stderr, "write_plumes_channel: end\n");
#endif
}

#if INLINE_WRITE_PLUMES_CHANNEL
#define write_plumes_channel _write_plumes_channel
#else
void write_plumes_channel
  (FILE *, Parameters *, TransportParameters *, Transport *, Channel *);
#endif

static inline void
_write_plumes (FILE * file, Parameters * p,
               TransportParameters * tp, JBDOUBLE t, System * s)
{
  int i;
#if DEBUG_WRITE_PLUMES
  fprintf (stderr, "write_plumes: start\n");
#endif
  if (s->nt < 0)
    return;
  fprintf (file, FWL " ", t);
  for (i = 0; i <= s->n; ++i)
    write_plumes_channel (file, p, tp, s->transport, s->channel + i);
  fprintf (file, "\n");
#if DEBUG_WRITE_PLUMES
  fprintf (stderr, "write_plumes: end\n");
#endif
}

#if INLINE_WRITE_PLUMES
#define write_plumes _write_plumes
#else
void write_plumes
  (FILE *, Parameters *, TransportParameters *, JBDOUBLE, System *);
#endif

static inline void
_write_contributions (FILE * file, JBDOUBLE t, System * s)
{
  int i, j, k;
  BoundaryFlow *bf;
  BoundaryTransport *bt;
  ChannelTransport *ct;
  Channel *channel;
  fprintf (file, FWL, t);
  for (i = 0; i <= s->n; ++i)
    {
      channel = s->channel + i;
      for (j = 0; j <= channel->n; ++j)
        {
          bf = channel->bf + j;
          fprintf (file, " " FWL, bf->contribution);
        }
      for (j = 0; j <= channel->nt; ++j)
        {
          ct = channel->ct + j;
          for (k = 0; k <= ct->n; ++k)
            {
              bt = ct->bt + k;
              fprintf (file, " " FWL, bt->contribution);
            }
        }
    }
  fprintf (file, "\n");
}

#if INLINE_WRITE_CONTRIBUTIONS
#define write_contributions _write_contributions
#else
void write_contributions (FILE *, JBDOUBLE, System *);
#endif

#endif
