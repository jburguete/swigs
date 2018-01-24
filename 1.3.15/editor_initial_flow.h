/*
SWIGS (Shallow Water in Irregular Geometries Simulator): a software to simulate
initial or steady flows with solute transport in channels, channel networks
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
 * \file editor_initial_flow.h
 * \brief Header file to define initial flow editor structures and functions.
 * \authors Javier Burguete Tolosa.
 * \copyright Copyright 2005-2015 Javier Burguete Tolosa.
 */
#ifndef EDITOR_INITIAL_FLOW__H
#define EDITOR_INITIAL_FLOW__H 1

#include "initial_flow.h"

/**
 * \struct EditorInitialFlow
 * \brief Structure to define an editor of initial flows.
 */
typedef struct
{
/**
 * \var ifc
 * \brief InitialFlow to define the initial flow data.
 * \var array
 * \brief JBWArrayEditor to define the flow points.
 * \var graphic
 * \brief JBWGraphic to show the initial flow graphic.
 * \var button_insert
 * \brief GtkButton to insert a point.
 * \var button_remove
 * \brief GtkButton to remove a point.
 * \var button_plot
 * \brief GtkButton to update the initial flow graphic.
 * \var array_type
 * \brief array of GtkRadioButtons to set the initial flow type.
 * \var frame_type
 * \brief GtkFrame to group the array_type buttons.
 * \var grid
 * \brief GtkGrid to pack the widgets.
 * \var grid_type
 * \brief GtkGrid to pack the array_type buttons.
 * \var grid_parameters
 * \brief GtkGrid to pack parameter widgets.
 * \var notebook
 * \brief GtkNotebook to put the widgets.
 */
  InitialFlow ifc[1];
  JBWArrayEditor *array;
  JBWGraphic *graphic;
  GtkButton *button_insert, *button_remove, *button_plot;
  GtkRadioButton *array_type[N_INITIAL_FLOW_TYPES];
  GtkFrame *frame_type;
  GtkGrid *grid, *grid_type, *grid_parameters;
  GtkNotebook *notebook;
} EditorInitialFlow;

void editor_initial_flow_update (EditorInitialFlow * editor);
void editor_initial_flow_get (EditorInitialFlow * editor);
void editor_initial_flow_open (EditorInitialFlow * editor);
void editor_initial_flow_insert_point (EditorInitialFlow * editor);
void editor_initial_flow_remove_point (EditorInitialFlow * editor);
void editor_initial_flow_draw (EditorInitialFlow * editor);
int editor_initial_flow_check (EditorInitialFlow * editor);
void editor_initial_flow_destroy (EditorInitialFlow * editor);
void editor_initial_flow_new
  (EditorInitialFlow * editor, GtkNotebook * notebook);

extern void editor_draw ();

#endif
