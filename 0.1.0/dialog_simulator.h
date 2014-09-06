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

#ifndef DIALOG_SIMULATOR__H
#define DIALOG_SIMULATOR__H 1

#include "jb/jb_win.h"

#define N_DIALOG_SIMULATOR_BUTTONS			19
#define N_DIALOG_SIMULATOR_ITEM_BAR			4
#define N_DIALOG_SIMULATOR_ITEM_SYSTEM		7
#define N_DIALOG_SIMULATOR_ITEM_SIMULATE	7
#define N_DIALOG_SIMULATOR_ITEM_GRAPHIC		4
#define N_DIALOG_SIMULATOR_ITEM_HELP		1

typedef struct
{
	long int time0;
	char string_progress[32];
	GtkLabel *label_simulation,*label_cpu,*label_error;
	GtkButton *button[N_DIALOG_SIMULATOR_BUTTONS],*button_bar;
	JBWFloatEntry *entry_cpu,*entry_error;
	GtkProgressBar *progress_simulation;
	JBWMenuItem *item_bar[N_DIALOG_SIMULATOR_ITEM_BAR],
		*item_system[N_DIALOG_SIMULATOR_ITEM_SYSTEM],
		*item_simulate[N_DIALOG_SIMULATOR_ITEM_SIMULATE],
		*item_graphic[N_DIALOG_SIMULATOR_ITEM_GRAPHIC],
		*item_help[N_DIALOG_SIMULATOR_ITEM_HELP];
	GtkMenu *menu_system,*menu_simulate,*menu_graphic,*menu_help;
	GtkMenuBar *menu;
	JBWGraphic *graphic;
	GtkHBox *box_bar,*box_buttons;
	GtkFrame *frame_buttons;
	GtkVBox *box;
	GdkPixbuf *pixbuf;
	GtkWindow *window;
} DialogSimulator;

extern DialogSimulator *dialog_simulator;

void dialog_simulator_new_system();
void dialog_simulator_edit();
void dialog_simulator_about();
void dialog_simulator_actualize(DialogSimulator *dlg);
void dialog_simulator_actualize_bar(DialogSimulator *dlg);
void dialog_simulator_actualize_menu(DialogSimulator *dlg,int active);
DialogSimulator* dialog_simulator_new();

#endif
