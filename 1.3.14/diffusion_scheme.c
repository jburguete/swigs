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

#include "diffusion_scheme.h"

#if !INLINE_DIFFUSION_PARAMETERS_EXPLICIT_EXPLICIT
void diffusion_parameters_explicit_explicit
  (TransportParameters * tp, Parameters * p, int n, JBDOUBLE dt)
{
  _diffusion_parameters_explicit_explicit (tp, p, n, dt);
}
#endif

#if !INLINE_DIFFUSION_PARAMETERS_EXPLICIT_IMPLICIT
void diffusion_parameters_explicit_implicit
  (TransportParameters * tp, Parameters * p, int n, JBDOUBLE dt)
{
  _diffusion_parameters_explicit_implicit (tp, p, n, dt);
}
#endif

#if !INLINE_DIFFUSION_PARAMETERS_IMPLICIT_EXPLICIT
void
diffusion_parameters_implicit_explicit (Parameters * p)
{
  _diffusion_parameters_implicit_explicit (p);
}
#endif
