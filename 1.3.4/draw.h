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

#ifndef DRAW__H
#define DRAW__H 1

#include "def.h"

#if JBW==JBW_NO

#include "jb/jb_math.h"

extern JBDOUBLE t;

static inline void _draw()
{
	printf("t="FWF"\n",t);
}

#elif JBW==JBW_GTK

#define _GNU_SOURCE
#include <time.h>
#include "dialog_graphical_options.h"
#include "dialog_simulator.h"
#include "simulate.h"

extern int animating,opening;

static inline void _draw_graphic()
{
	register int i,j;
	int i1,i2,nn;
	JBFLOAT *buffer,*xx,*y1,*y2,*z1,*z2;
	register JBDOUBLE k;
	FILE *file;
	ChannelGeometry *cg;
	TransientSection *ts;

	#if DEBUG_DRAW_GRAPHIC
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"draw_graphic: start\n");
			fprintf(stderr,"DG opening=%d type=%d\n",opening,type_draw);
		}
	#endif

	if (opening) return;

	buffer=xx=y1=y2=z1=z2=NULL;
	if (simulated)
	{
		if (type_draw==1)
		{
			read_evolution(channel,evolution_section,evolution_parameter1,
				evolution_parameter2,&xx,&y1,&z1,&nn,vsize);
		}
		else
		{
			file=g_fopen(sys->solution_path,"rb");
			if (!file)
			{
				jbw_show_error2
					(gettext("Solution"),gettext("Unable to open the file"));
				#if DEBUG_DRAW_GRAPHIC
					if (t>=DEBUG_TIME) fprintf(stderr,"draw_graphic: end\n");
				#endif
				return;
			}
			read_profile(file,v,n,istep,vsize);
			fclose(file);
		}
	}
	else variables_actualize(v);
	cg = sys->channel[channel].cg;
	i1 = cg->cs->i;
	i2 = cg->cs[cg->n].i;
	if (v->x) switch (type_draw)
	{
	case 0:
		j=i2-i1+1;
		buffer=(JBFLOAT*)g_try_malloc(j*2*sizeof(JBFLOAT));
		if (!buffer) break;
		xx = v->x + i1;
		switch (profile_parameter1)
		{
		case 0:
			y1 = v->Q + i1;
			break;
		case 1:
			y1 = buffer;
			for (i=j; --i>=0;) y1[i] = v->zs[i + i1] - v->zb[i + i1];
			break;
		case 2:
			y1 = v->zs + i1;
			y2 = v->zb + i1;
			break;
		case 3:
			y1=buffer;
			y2=buffer+j;
			for (i=j; --i>=0;)
			{
				if (v->A[i + i1] > 0.) y1[i] = v->Q[i + i1] / v->A[i + i1];
				else y1[i] = 0.;
				k = v->B[i + i1] * v->beta[i + i1];
				if (k > 0.) y2[i] = sqrt(JBM_G * v->A[i + i1] / k);
				else y2[i] = 0.;
			}
			break;
		case 4:
			y1=buffer;
			y2=buffer+j;
			for (i=j; --i>=0;)
			{
				k = v->A[i + i1] * sqrt(JBM_G * v->A[i + i1] /
					(v->B[i + i1] * v->beta[i + i1]));
				if (k > 0.) y1[i] = v->Q[i + i1] / k; else y1[i] = 0.;
				y2[i] = 1.;
			}
			break;
		case 5:
			y1 = v->A + i1;
			break;
		case 6:
			y1 = v->B + i1;
			break;
		default:
			y1 = v->p + (n + 1) * (profile_parameter1 - 7) + i1;
		}
		switch (profile_parameter2)
		{
		case 0:
			z1 = v->Q + i1;
			break;
		case 1:
			z1 = buffer;
			for (i=j; --i>=0;) z1[i] = v->zs[i + i1] - v->zb[i + i1];
			break;
		case 2:
			z1 = v->zs + i1;
			z2 = v->zb + i1;
			break;
		case 3:
			z1=buffer;
			z2=buffer+j;
			for (i=j; --i>=0;)
			{
				if (v->A[i + i1] > 0.) z1[i] = v->Q[i + i1] / v->A[i + i1];
				else z1[i] = 0.;
				k = v->B[i + i1] * v->beta[i + i1];
				if (k > 0.) z2[i] = sqrt(JBM_G * v->A[i + i1] / k);
				else z2[i] = 0.;
			}
			break;
		case 4:
			z1=buffer;
			z2=buffer+j;
			for (i=j; --i>=0;)
			{
				k = v->A[i + i1] * sqrt(JBM_G * v->A[i + i1] /
					(v->B[i + i1] * v->beta[i + i1]));
				if (k > 0.) z1[i] = v->Q[i + i1] / k; else z1[i] = 0.;
				z2[i] = 1.;
			}
			break;
		case 5:
			z1 = v->A + i1;
			break;
		case 6:
			z1 = v->B + i1;
			break;
		default:
			z1 = v->p + (n + 1) * (profile_parameter2 - 7) + i1;
		}
		break;
	case 1:
		j=nn+1;
		break;
	default:
		ts = cross_section_get_transient
			(sys->channel[channel].cg->cs+section, t);
		j = ts->n + 1;
		xx=buffer=(JBFLOAT*)g_try_malloc(j*3*sizeof(JBFLOAT));
		if (!buffer) break;
		y1=xx+j;
		y2=y1+j;
		k=p[sys->channel[channel].cg->cs[section].i].zs;
		for (i=j; --i>=0;)
		{
			xx[i]=ts->sp[i].y;
			y1[i]=k;
			y2[i]=ts->sp[i].z;
		}
	}
	#if DEBUG_DRAW_GRAPHIC
		if (t>=DEBUG_TIME)
		{
			fprintf(stderr,"DG\n");
			if (xx) for (i=0; i<j; ++i)
				fprintf(stderr,"DG i=%d x="FWF"\n",i,xx[i]);
			if (y1) for (i=0; i<j; ++i)
				fprintf(stderr,"DG i=%d y1="FWF"\n",i,y1[i]);
			if (y2) for (i=0; i<j; ++i)
				fprintf(stderr,"DG i=%d y2="FWF"\n",i,y2[i]);
			if (z1) for (i=0; i<j; ++i)
				fprintf(stderr,"DG i=%d z1="FWF"\n",i,z1[i]);
			if (z2) for (i=0; i<j; ++i)
				fprintf(stderr,"DG i=%d z2="FWF"\n",i,z2[i]);
		}
	#endif
	jbw_graphic_set_grid(dialog_simulator->graphic,draw_grid);
	jbw_graphic_draw_lines(dialog_simulator->graphic,xx,y1,y2,z1,z2,j);
	jbw_graphic_draw_logo(dialog_simulator->graphic);

	if (type_draw==1) jb_free_null((void**)&xx);
	else jb_free_null((void**)&buffer);
	#if DEBUG_DRAW_GRAPHIC
		if (t>=DEBUG_TIME) fprintf(stderr,"draw_graphic: end\n");
	#endif
}

#if INLINE_DRAW_GRAPHIC
	#define draw_graphic _draw_graphic
#else
	void draw_graphic();
#endif

static inline void _draw()
{
	#if DEBUG_DRAW
		if (t>=DEBUG_TIME) fprintf(stderr,"draw: start\n");
	#endif
#if JBW_GRAPHIC==JBW_GRAPHIC_GLUT
	glutPostRedisplay();
#endif
	if (simulating||animating||t==0.)
		dialog_simulator_actualize_bar(dialog_simulator);
	#if DEBUG_DRAW
		if (t>=DEBUG_TIME) fprintf(stderr,"draw: end\n");
	#endif
}

#if INLINE_DRAW
	#define draw _draw
#else
	void draw();
#endif

static inline void _draw_animation()
{
	long t1,t2;
	#if DEBUG_DRAW_ANIMATION
		fprintf(stderr,"draw_animation: start\n");
	#endif
	animating=1;
	for (t1=time(0), istep=-1; ++istep<nstep && animating; t1=t2)
	{
		t=fmin(t0+istep*ti,tf);
		draw();
		while (gtk_events_pending()) gtk_main_iteration();
		while (time(&t2)<=t1);
	}
	animating=0;
	#if DEBUG_DRAW_ANIMATION
		fprintf(stderr,"draw_animation: end\n");
	#endif
}

#if INLINE_DRAW_ANIMATION
	#define draw_animation _draw_animation
#else
	void draw_animation();
#endif

#endif

#endif
