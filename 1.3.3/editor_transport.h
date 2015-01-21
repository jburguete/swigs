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
 * \file editor_transport.h
 * \brief Header file to define transported solute editor structures and
 *   functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2015 Javier Burguete Tolosa.
 */
#ifndef EDITOR_TRANSPORT__H
#define EDITOR_TRANSPORT__H 1

#include "jb/jb_win.h"
#include "transport.h"

/**
 * \struct EditorTransport
 * \brief Structure to define an editor of transported solutes.
 */
typedef struct
{
/**
 * \var transport
 * \brief Transport structure to define the transported solute data.
 * \var label_name
 * \brief GtkLabel to show a label of the transported solute name.
 * \var label_solubility
 * \brief GtkLabel to show a label of the solubility.
 * \var label_danger
 * \brief GtkLabel to show a label of the danger concentration.
 * \var entry_name
 * \brief GtkEntry to set the transported solute name.
 * \var entry_solubility
 * \brief GtkSpinButton to set the solubility.
 * \var label_danger
 * \brief GtkSpinButton to set the danger concentration.
 * \var grid
 * \brief GtkGrid to pack the widgets.
 * \var notebook
 * \brief GtkNotebook to put the widgets.
 */
	Transport transport[1];
	GtkLabel *label_name, *label_solubility, *label_danger;
	GtkEntry *entry_name;
	GtkSpinButton *entry_solubility, *entry_danger;
	GtkGrid *grid;
	GtkNotebook *notebook;
} EditorTransport;

int editor_transport_get(EditorTransport *editor);
void editor_transport_open(EditorTransport *editor);
void editor_transport_destroy(EditorTransport *editor);
void editor_transport_new(EditorTransport *editor, GtkNotebook *notebook);

#endif
