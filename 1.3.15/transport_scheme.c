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

#include "transport_scheme.h"

#if !INLINE_TRANSPORT_SCHEME_PARAMETERS_UPWIND
void
transport_scheme_parameters_upwind (TransportParameters * tp, Parameters * p)
{
  _transport_scheme_parameters_upwind (tp, p);
}
#endif

#if TRANSPORT_SCHEME == TRANSPORT_SCHEME_TVD2
#if !INLINE_TRANSPORT_SCHEME_PARAMETERS_UPWIND2
void
transport_scheme_parameters_upwind2 (TransportParameters * tp, Parameters * p)
{
  _transport_scheme_parameters_upwind2 (tp, p);
}
#endif
#endif

#if !INLINE_TRANSPORT_SCHEME_UPWIND
void
transport_scheme_upwind (TransportParameters * tp, Parameters * p)
{
  _transport_scheme_upwind (tp, p);
}
#endif

#if TRANSPORT_SCHEME == TRANSPORT_SCHEME_TVD

#if !INLINE_TRANSPORT_SCHEME_TVD
void
transport_scheme_tvd (TransportParameters * tp, Parameters * p)
{
  _transport_scheme_tvd (tp, p);
}
#endif

#elif TRANSPORT_SCHEME == TRANSPORT_SCHEME_TVD2

#if !INLINE_TRANSPORT_SCHEME_TVD2
void
transport_scheme_tvd2 (TransportParameters * tp, Parameters * p)
{
  _transport_scheme_tvd2 (tp, p);
}
#endif

#endif

#if !INLINE_TRANSPORT_SCHEME_DIFFUSION
void
transport_scheme_diffusion (TransportParameters * tp, Parameters * p)
{
  _transport_scheme_diffusion (tp, p);
}
#endif

#if !INLINE_TRANSPORT_SCHEME_CONTRIBUTIONS_UPWIND
void transport_scheme_contributions_upwind
  (TransportParameters * tp, Parameters * p, int n, JBDOUBLE * inlet,
   JBDOUBLE * outlet)
{
  _transport_scheme_contributions_upwind (tp, p, n, inlet, outlet);
}
#endif

#if !INLINE_TRANSPORT_SCHEME_INLET_UPWIND
void transport_scheme_inlet_upwind
  (TransportParameters * tp, Parameters * p, int n, Mass * m, JBDOUBLE inlet)
{
  _transport_scheme_inlet_upwind (tp, p, n, m, inlet);
}
#endif

#if !INLINE_TRANSPORT_SCHEME_OUTLET_UPWIND
void transport_scheme_outlet_upwind
  (TransportParameters * tp, Parameters * p, int n, Mass * m, JBDOUBLE outlet)
{
  _transport_scheme_outlet_upwind (tp, p, n, m, outlet);
}
#endif

#if !INLINE_TRANSPORT_BOUNDARY
void transport_boundary
  (TransportParameters * tp, Parameters * p, Mass * m, BoundaryTransport * bt)
{
  _transport_boundary (tp, p, m, bt);
}
#endif
