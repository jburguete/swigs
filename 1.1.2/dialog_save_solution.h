/*
SWIGS (Shallow Water in Irregular Geometries Simulator): a software to simulate
transient or steady flows with solute transport in channels,  channel networks
and rivers.

Copyright 2005-2014 Javier Burguete Tolosa.

Redistribution and use in source and binary forms,  with or without modification, 
are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice, 
		this list of conditions and the following disclaimer.

	2. Redistributions in binary form must reproduce the above copyright notice, 
		this list of conditions and the following disclaimer in the
		documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY Javier Burguete Tolosa ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES,  INCLUDING,  BUT NOT LIMITED TO,  THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL Javier Burguete Tolosa OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,  INDIRECT, 
INCIDENTAL,  SPECIAL,  EXEMPLARY,  OR CONSEQUENTIAL DAMAGES (INCLUDING,  BUT NOT
LIMITED TO,  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,  DATA,  OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY,  WHETHER IN CONTRACT,  STRICT LIABILITY,  OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,  EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef DIALOG_SAVE_SOLUTION__H
#define DIALOG_SAVE_SOLUTION__H 1

#include "jb/jb_win.h"

typedef struct
{
	GtkFileChooserWidget *widget_file;
	GtkLabel *label_profile, *label_evolution, *label_time, *label_section;
	GtkComboBoxText *combo_profile, *combo_evolution, *combo_channel;
	GtkGrid *box, *box_profile, *box_evolution;
	GtkNotebook *notebook;
	GtkDialog *window;
} DialogSaveSolution;

extern int channel_write;

void dialog_save_solution_new();

#endif