/*----------------------------------------------------------------------- 
 * 
 * File Name: CCVectorDivide.c
 * 
 * Author: Creighton, J. D. E.
 * 
 * Revision: $Id$
 * 
 *----------------------------------------------------------------------- 
 * 
 * NAME 
 * CCVectorDivide
 * 
 * SYNOPSIS 
 *
 *
 * DESCRIPTION 
 * Divide a complex vector by another complex vector.
 * 
 * DIAGNOSTICS 
 * Null pointer, invalid input size, size mismatch.
 *
 * CALLS
 * 
 * NOTES
 * 
 *-----------------------------------------------------------------------
 */

#include <math.h>
#include "LALStdlib.h"
#include "VectorOps.h"

NRCSID (CCVECTORDIVIDEC, "$Id$");

void
CCVectorDivide (
    Status               *status,
    COMPLEX8Vector       *out,
    const COMPLEX8Vector *in1,
    const COMPLEX8Vector *in2
    )
{
  COMPLEX8 *a;
  COMPLEX8 *b;
  COMPLEX8 *c;
  INT4      n;

  INITSTATUS (status, "CCVectorDivide", CCVECTORDIVIDEC);

  ASSERT (out, status, VECTOROPS_ENULL, VECTOROPS_MSGENULL);
  ASSERT (in1, status, VECTOROPS_ENULL, VECTOROPS_MSGENULL);

  ASSERT (in2, status, VECTOROPS_ENULL, VECTOROPS_MSGENULL);

  ASSERT (out->data, status, VECTOROPS_ENULL, VECTOROPS_MSGENULL);
  ASSERT (in1->data, status, VECTOROPS_ENULL, VECTOROPS_MSGENULL);
  ASSERT (in2->data, status, VECTOROPS_ENULL, VECTOROPS_MSGENULL);

  ASSERT (out->length > 0, status, VECTOROPS_ESIZE, VECTOROPS_MSGESIZE);
  ASSERT (in1->length == out->length, status,
          VECTOROPS_ESZMM, VECTOROPS_MSGESZMM);
  ASSERT (in2->length == out->length, status,
          VECTOROPS_ESZMM, VECTOROPS_MSGESZMM);

  a = in1->data;
  b = in2->data;
  c = out->data;
  n = out->length;

  while (n-- > 0)
  {
    REAL4 ar = a->re;
    REAL4 ai = a->im;
    REAL4 br = b->re;
    REAL4 bi = b->im;

    if (fabs(br) > fabs(bi))
    {
      REAL4 ratio = bi/br;
      REAL4 denom = br + ratio*bi;

      c->re = (ar + ratio*ai)/denom;
      c->im = (ai - ratio*ar)/denom;
    }
    else
    {
      REAL4 ratio = br/bi;
      REAL4 denom = bi + ratio*br;

      c->re = (ar*ratio + ai)/denom;
      c->im = (ai*ratio - ar)/denom;
    }

    ++a;
    ++b;
    ++c;
  }

  RETURN (status);
}

