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
 * \file editor_cross_section.h
 * \brief Header file to define cross section editor structures and methods.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2014 Javier Burguete Tolosa.
 */
#ifndef EDITOR_CROSS_SECTION__H
#define EDITOR_CROSS_SECTION__H 1

#include "editor_transient_section.h"

#define N_CONTROL_TYPES 3

typedef struct
{
	GtkLabel *label_channel, *label_section, *label_parameter, *label_time,
		*label_tolerance;
	GtkComboBoxText *combo_channel, *combo_section;
	GtkSpinButton *entry_parameter, *entry_time, *entry_tolerance;
	GtkRadioButton *array_type[N_CONTROL_TYPES];
	GtkGrid *grid, *grid_type;
	GtkFrame *frame, *frame_type;
	char **channel_name, ***section_name;
	int nchannels, *nsections;
} EditorControl;

typedef struct
{
	EditorTransientSection editor_transient[1];
	EditorControl control[1];
	GtkLabel *label_name, *label_x, *label_y, *label_angle, *label_transient;
	GtkEntry *name;
	GtkSpinButton *entry_x, *entry_y, *entry_angle;
	GtkButton *button_insert, *button_remove, *button_plot;
	GtkRadioButton *array_type[2];
	GtkComboBoxText *combo_transient;
	GtkFrame *frame_type;
	GtkGrid *grid, *grid_type;
	CrossSection cs[1];
} EditorCrossSection;

#endif
