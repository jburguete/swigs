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

#ifndef DIALOG_GRAPHICAL_OPTIONS__H
#define DIALOG_GRAPHICAL_OPTIONS__H 1

#include "system.h"

extern JBFLOAT profile_xmin,profile_xmax,profile_ymin,profile_ymax,profile_zmin,
	profile_zmax;
extern int profile_parameter1,profile_parameter2;
extern JBFLOAT evolution_xmin,evolution_xmax,evolution_ymin,evolution_ymax,
	evolution_zmin,evolution_zmax;
extern int evolution_parameter1,evolution_parameter2;
extern JBFLOAT section_xmin,section_xmax,section_ymin,section_ymax;
extern int evolution_section,section,channel;
extern int type_draw,draw_adjust,draw_grid;

typedef struct
{
	GtkCheckButton *button_adjust,*button_grid;
	GtkLabel *label_profile_xmax,*label_profile_xmin,*label_profile_ymax,
		*label_profile_ymin,*label_profile_zmax,*label_profile_zmin,
		*label_profile,*label_profile_time;
	JBWFloatEntry *entry_profile_xmax,*entry_profile_xmin,*entry_profile_ymax,
		*entry_profile_ymin,*entry_profile_zmax,*entry_profile_zmin;
	GtkTable *table_profile_x,*table_profile_y,*table_profile_z;
	GtkFrame *frame_profile_x,*frame_profile_y,*frame_profile_z;
	GtkComboBoxText *combo_profile,*combo_profile_y,*combo_profile_z;
	GtkHBox *box_profile;
	GtkTable *table_profile;
	GtkLabel *label_evolution_xmax,*label_evolution_xmin,*label_evolution_ymax,
		*label_evolution_ymin,*label_evolution_zmax,*label_evolution_zmin,
		*label_evolution,*label_evolution_section;
	JBWFloatEntry *entry_evolution_xmax,*entry_evolution_xmin,
		*entry_evolution_ymax,*entry_evolution_ymin,
		*entry_evolution_zmax,*entry_evolution_zmin;
	GtkTable *table_evolution_x,*table_evolution_y,*table_evolution_z;
	GtkFrame *frame_evolution_x,*frame_evolution_y,*frame_evolution_z;
	GtkComboBoxText
		*combo_evolution_section,*combo_evolution_y,*combo_evolution_z;
	GtkTable *table_evolution;
	GtkLabel *label_section_xmax,*label_section_xmin,*label_section_ymax,
		*label_section_ymin,*label_section_section,*label_section;
	JBWFloatEntry *entry_section_xmax,*entry_section_xmin,
		*entry_section_ymax,*entry_section_ymin;
	GtkTable *table_section_x,*table_section_y;
	GtkFrame *frame_section_x,*frame_section_y;
	GtkComboBoxText *combo_section;
	GtkTable *table_section;
	GtkComboBoxText *combo_channel;
	GtkNotebook *notebook;
	GtkRadioButton *array_animation[3];
	GtkVBox *box_animation,*vbox;
	GtkFrame *frame_animation;
	GtkHBox *box;
	GtkDialog *window;
} DialogGraphicalOptions;

extern DialogGraphicalOptions dialog_graphical_options[1];

void string_graphical_options(char **string,System *sys);
void dialog_graphical_options_actualize(DialogGraphicalOptions *dlg);
void dialog_graphical_options_sections(DialogGraphicalOptions *dlg);
void dialog_graphical_options_channel(DialogGraphicalOptions *dlg);
void dialog_graphical_options_new(GtkWindow *parent);

extern int type_animation;
extern void draw();

#endif

