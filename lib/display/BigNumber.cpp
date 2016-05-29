//===-- BigNumber.cpp - Large Format Number Rendering  ------------- c++ --===//
//
//                               UWH Timer
//
//           This file is distributed under the BSD 3-Clause License.
//                      See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "uwhd/display/BigNumber.h"

#include <canvas.h>
#include <led-matrix.h>
#include <graphics.h>

#include <cassert>
#include <cstdarg>

using namespace rgb_matrix;

#define _ 0

static const char Number5x7_0[5*7] = {
_,1,1,1,_,
1,_,_,_,1,
1,_,_,1,1,
1,_,1,_,1,
1,1,_,_,1,
1,_,_,_,1,
_,1,1,1,_,
};

static const char Number5x7_1[5*7] = {
_,_,1,_,_,
_,1,1,_,_,
_,_,1,_,_,
_,_,1,_,_,
_,_,1,_,_,
_,_,1,_,_,
_,1,1,1,_,
};

static const char Number5x7_2[5*7] = {
_,1,1,1,_,
1,_,_,_,1,
_,_,_,_,1,
_,_,_,1,_,
_,_,1,_,_,
_,1,_,_,_,
1,1,1,1,1,
};

static const char Number5x7_3[5*7] = {
_,1,1,1,_,
1,_,_,_,1,
_,_,_,_,1,
_,_,1,1,1,
_,_,_,_,1,
1,_,_,_,1,
_,1,1,1,_,
};

static const char Number5x7_4[5*7] = {
_,_,_,1,_,
_,_,1,1,_,
_,1,_,1,_,
1,1,1,1,1,
_,_,_,1,_,
_,_,_,1,_,
_,_,_,1,_,
};

static const char Number5x7_5[5*7] = {
1,1,1,1,1,
1,_,_,_,_,
1,_,_,_,_,
1,1,1,1,_,
_,_,_,_,1,
1,_,_,_,1,
_,1,1,1,_,
};

static const char Number5x7_6[5*7] = {
_,1,1,1,_,
1,_,_,_,1,
1,_,_,_,_,
1,1,1,1,_,
1,_,_,_,1,
1,_,_,_,1,
_,1,1,1,_,
};

static const char Number5x7_7[5*7] = {
1,1,1,1,1,
_,_,_,_,1,
_,_,_,1,_,
_,_,1,_,_,
_,1,_,_,_,
_,1,_,_,_,
_,1,_,_,_,
};

static const char Number5x7_8[5*7] = {
_,1,1,1,_,
1,_,_,_,1,
1,_,_,_,1,
_,1,1,1,_,
1,_,_,_,1,
1,_,_,_,1,
_,1,1,1,_,
};

static const char Number5x7_9[5*7] = {
_,1,1,1,_,
1,_,_,_,1,
1,_,_,_,1,
_,1,1,1,1,
_,_,_,_,1,
1,_,_,_,1,
_,1,1,1,_,
};

static const char Ascii5x7_A[5*7] = {
_,1,1,1,_,
1,_,_,_,1,
1,_,_,_,1,
1,1,1,1,1,
1,_,_,_,1,
1,_,_,_,1,
1,_,_,_,1,
};

static const char Ascii5x7_B[5*7] = {
1,1,1,1,_,
1,_,_,_,1,
1,_,_,_,1,
1,1,1,1,1,
1,_,_,_,1,
1,_,_,_,1,
1,1,1,1,_,
};

static const char Ascii5x7_C[5*7] = {
_,1,1,1,1,
1,_,_,_,_,
1,_,_,_,_,
1,_,_,_,_,
1,_,_,_,_,
1,_,_,_,_,
_,1,1,1,1,
};

static const char Ascii5x7_D[5*7] = {
1,1,1,1,_,
1,_,_,_,1,
1,_,_,_,1,
1,_,_,_,1,
1,_,_,_,1,
1,_,_,_,1,
1,1,1,1,_,
};

static const char Ascii5x7_E[5*7] = {
1,1,1,1,1,
1,_,_,_,_,
1,_,_,_,_,
1,1,1,1,1,
1,_,_,_,_,
1,_,_,_,_,
1,1,1,1,1,
};

static const char Ascii5x7_F[5*7] = {
1,1,1,1,1,
1,_,_,_,_,
1,_,_,_,_,
1,1,1,1,1,
1,_,_,_,_,
1,_,_,_,_,
1,_,_,_,_,
};

static const char Ascii5x7_G[5*7] = {
_,1,1,1,_,
1,_,_,_,_,
1,_,_,_,_,
1,_,_,1,1,
1,_,_,_,1,
1,_,_,_,1,
_,1,1,1,_,
};

static const char Ascii5x7_H[5*7] = {
1,_,_,_,1,
1,_,_,_,1,
1,_,_,_,1,
1,1,1,1,1,
1,_,_,_,1,
1,_,_,_,1,
1,_,_,_,1,
};

static const char Ascii5x7_I[5*7] = {
1,1,1,1,1,
_,_,1,_,_,
_,_,1,_,_,
_,_,1,_,_,
_,_,1,_,_,
_,_,1,_,_,
1,1,1,1,1,
};

static const char Ascii5x7_J[5*7] = {
1,1,1,1,1,
_,_,_,1,_,
_,_,_,1,_,
_,_,_,1,_,
1,_,_,1,_,
1,_,_,1,_,
_,1,1,_,_,
};

static const char Ascii5x7_K[5*7] = {
1,_,_,_,1,
1,_,_,1,_,
1,_,1,_,_,
1,1,_,_,_,
1,_,1,_,_,
1,_,_,1,_,
1,_,_,_,1,
};

static const char Ascii5x7_L[5*7] = {
1,_,_,_,_,
1,_,_,_,_,
1,_,_,_,_,
1,_,_,_,_,
1,_,_,_,_,
1,_,_,_,_,
1,1,1,1,1,
};

static const char Ascii5x7_M[5*7] = {
1,_,_,_,1,
1,1,_,1,1,
1,_,1,_,1,
1,_,_,_,1,
1,_,_,_,1,
1,_,_,_,1,
1,_,_,_,1,
};

static const char Ascii5x7_N[5*7] = {
1,_,_,_,1,
1,1,_,_,1,
1,_,1,_,1,
1,_,_,1,1,
1,_,_,_,1,
1,_,_,_,1,
1,_,_,_,1,
};


static const char Ascii5x7_O[5*7] = {
_,1,1,1,_,
1,_,_,_,1,
1,_,_,_,1,
1,_,_,_,1,
1,_,_,_,1,
1,_,_,_,1,
_,1,1,1,_,
};

static const char Ascii5x7_P[5*7] = {
_,1,1,1,_,
1,_,_,_,1,
1,_,_,_,1,
1,1,1,1,_,
1,_,_,_,_,
1,_,_,_,_,
1,_,_,_,_,
};

static const char Ascii5x7_Q[5*7] = {
_,1,1,1,_,
1,_,_,_,1,
1,_,_,_,1,
1,_,_,_,1,
1,_,1,_,1,
1,_,_,1,_,
_,1,1,_,1,
};

static const char Ascii5x7_R[5*7] = {
_,1,1,1,_,
1,_,_,_,1,
1,_,_,_,1,
1,1,1,1,_,
1,_,1,_,_,
1,_,_,1,_,
1,_,_,_,1,
};

static const char Ascii5x7_S[5*7] = {
_,1,1,1,_,
1,_,_,_,_,
1,_,_,_,_,
_,1,1,1,_,
_,_,_,_,1,
_,_,_,_,1,
_,1,1,1,_,
};

static const char Ascii5x7_T[5*7] = {
1,1,1,1,1,
_,_,1,_,_,
_,_,1,_,_,
_,_,1,_,_,
_,_,1,_,_,
_,_,1,_,_,
_,_,1,_,_,
};

static const char Ascii5x7_U[5*7] = {
1,_,_,_,1,
1,_,_,_,1,
1,_,_,_,1,
1,_,_,_,1,
1,_,_,_,1,
1,_,_,_,1,
_,1,1,1,_,
};

static const char Ascii5x7_V[5*7] = {
1,_,_,_,1,
1,_,_,_,1,
1,_,_,_,1,
1,_,_,_,1,
1,_,_,_,1,
_,1,_,1,_,
_,_,1,_,_,
};

static const char Ascii5x7_W[5*7] = {
1,_,_,_,1,
1,_,_,_,1,
1,_,_,_,1,
1,_,_,_,1,
1,_,1,_,1,
1,_,1,_,1,
_,1,_,1,_,
};

static const char Ascii5x7_X[5*7] = {
1,_,_,_,1,
_,1,_,1,_,
_,_,1,_,_,
_,_,1,_,_,
_,_,1,_,_,
_,1,_,1,_,
1,_,_,_,1,
};

static const char Ascii5x7_Y[5*7] = {
1,_,_,_,1,
_,1,_,1,_,
_,_,1,_,_,
_,_,1,_,_,
_,_,1,_,_,
_,_,1,_,_,
_,_,1,_,_,
};

static const char Ascii5x7_Z[5*7] = {
1,1,1,1,1,
_,_,_,_,1,
_,_,_,1,_,
_,_,1,_,_,
_,1,_,_,_,
1,_,_,_,_,
1,1,1,1,1,
};

static const char Ascii5x7_LPAREN[5*7] = {
_,_,_,1,_,
_,_,1,_,_,
_,_,1,_,_,
_,_,1,_,_,
_,_,1,_,_,
_,_,1,_,_,
_,_,_,1,_,
};

static const char Ascii5x7_RPAREN[5*7] = {
_,1,_,_,_,
_,_,1,_,_,
_,_,1,_,_,
_,_,1,_,_,
_,_,1,_,_,
_,_,1,_,_,
_,1,_,_,_,
};

static const char Ascii5x7_LCURL[5*7] = {
_,_,_,1,_,
_,_,1,_,_,
_,_,1,_,_,
_,1,_,_,_,
_,_,1,_,_,
_,_,1,_,_,
_,_,_,1,_,
};


static const char Ascii5x7_RCURL[5*7] = {
_,1,_,_,_,
_,_,1,_,_,
_,_,1,_,_,
_,_,_,1,_,
_,_,1,_,_,
_,_,1,_,_,
_,1,_,_,_,
};


static const char Number11x20_0[11*20] = {
_,_,1,1,1,1,1,1,1,_,_,
_,1,1,1,1,1,1,1,1,1,_,
1,1,1,_,_,_,_,_,1,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,_,_,_,_,_,_,1,1,1,
1,1,_,_,_,_,_,1,1,1,1,
1,1,_,_,_,_,1,1,_,1,1,
1,1,_,_,_,1,1,_,_,1,1,
1,1,_,_,1,1,_,_,_,1,1,
1,1,_,1,1,_,_,_,_,1,1,
1,1,1,1,_,_,_,_,_,1,1,
1,1,1,_,_,_,_,_,_,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,1,_,_,_,_,_,1,1,1,
_,1,1,1,1,1,1,1,1,1,_,
_,_,1,1,1,1,1,1,1,_,_,
};

static const char Number11x20_1[11*20] = {
_,_,_,_,1,1,_,_,_,_,_,
_,_,_,1,1,1,_,_,_,_,_,
_,_,1,1,1,1,_,_,_,_,_,
_,_,1,1,1,1,_,_,_,_,_,
_,_,_,_,1,1,_,_,_,_,_,
_,_,_,_,1,1,_,_,_,_,_,
_,_,_,_,1,1,_,_,_,_,_,
_,_,_,_,1,1,_,_,_,_,_,
_,_,_,_,1,1,_,_,_,_,_,
_,_,_,_,1,1,_,_,_,_,_,
_,_,_,_,1,1,_,_,_,_,_,
_,_,_,_,1,1,_,_,_,_,_,
_,_,_,_,1,1,_,_,_,_,_,
_,_,_,_,1,1,_,_,_,_,_,
_,_,_,_,1,1,_,_,_,_,_,
_,_,_,_,1,1,_,_,_,_,_,
_,_,_,_,1,1,_,_,_,_,_,
_,_,_,_,1,1,_,_,_,_,_,
_,_,1,1,1,1,1,1,_,_,_,
_,_,1,1,1,1,1,1,_,_,_,
};

static const char Number11x20_2[11*20] = {
_,_,1,1,1,1,1,1,1,_,_,
_,1,1,1,1,1,1,1,1,1,_,
1,1,1,_,_,_,_,_,1,1,1,
1,1,_,_,_,_,_,_,_,1,1,
_,_,_,_,_,_,_,_,_,1,1,
_,_,_,_,_,_,_,_,_,1,1,
_,_,_,_,_,_,_,_,_,1,1,
_,_,_,_,_,_,_,_,_,1,1,
_,_,_,_,_,_,_,_,1,1,_,
_,_,_,_,_,_,_,1,1,1,_,
_,_,_,_,_,_,1,1,1,_,_,
_,_,_,_,_,1,1,1,_,_,_,
_,_,_,_,1,1,1,_,_,_,_,
_,_,_,1,1,1,_,_,_,_,_,
_,_,1,1,1,_,_,_,_,_,_,
_,1,1,1,_,_,_,_,_,_,_,
1,1,1,_,_,_,_,_,_,_,_,
1,1,_,_,_,_,_,_,_,_,_,
1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,
};

static const char Number11x20_3[11*20] = {
_,_,1,1,1,1,1,1,1,_,_,
_,1,1,1,1,1,1,1,1,1,_,
1,1,1,_,_,_,_,_,1,1,1,
1,1,_,_,_,_,_,_,_,1,1,
_,_,_,_,_,_,_,_,_,1,1,
_,_,_,_,_,_,_,_,_,1,1,
_,_,_,_,_,_,_,_,_,1,1,
_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,1,1,1,_,
_,_,_,_,_,1,1,1,1,_,_,
_,_,_,_,_,1,1,1,1,_,_,
_,_,_,_,_,_,_,1,1,1,_,
_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,1,1,
_,_,_,_,_,_,_,_,_,1,1,
_,_,_,_,_,_,_,_,_,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,1,_,_,_,_,_,1,1,1,
_,1,1,1,1,1,1,1,1,1,_,
_,_,1,1,1,1,1,1,1,_,_,
};

static const char Number11x20_4[11*20] = {
_,_,_,_,_,_,_,_,1,1,_,
_,_,_,_,_,_,_,1,1,1,_,
_,_,_,_,_,_,1,1,1,1,_,
_,_,_,_,_,1,1,1,1,1,_,
_,_,_,_,1,1,1,_,1,1,_,
_,_,_,1,1,1,_,_,1,1,_,
_,_,1,1,1,_,_,_,1,1,_,
_,1,1,1,_,_,_,_,1,1,_,
1,1,1,_,_,_,_,_,1,1,_,
1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,
_,_,_,_,_,_,_,_,1,1,_,
_,_,_,_,_,_,_,_,1,1,_,
_,_,_,_,_,_,_,_,1,1,_,
_,_,_,_,_,_,_,_,1,1,_,
_,_,_,_,_,_,_,_,1,1,_,
_,_,_,_,_,_,_,_,1,1,_,
_,_,_,_,_,_,_,_,1,1,_,
_,_,_,_,_,_,_,_,1,1,_,
_,_,_,_,_,_,_,_,1,1,_,
};

static const char Number11x20_5[11*20] = {
1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,
1,1,_,_,_,_,_,_,_,_,_,
1,1,_,_,_,_,_,_,_,_,_,
1,1,_,_,_,_,_,_,_,_,_,
1,1,_,_,_,_,_,_,_,_,_,
1,1,_,_,_,_,_,_,_,_,_,
1,1,_,_,_,_,_,_,_,_,_,
1,1,1,1,1,1,1,_,_,_,_,
1,1,1,1,1,1,1,1,_,_,_,
_,_,_,_,_,_,1,1,1,_,_,
_,_,_,_,_,_,_,1,1,1,_,
_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,1,1,
_,_,_,_,_,_,_,_,_,1,1,
_,_,_,_,_,_,_,_,_,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,1,_,_,_,_,_,1,1,1,
_,1,1,1,1,1,1,1,1,1,_,
_,_,1,1,1,1,1,1,1,_,_,
};

static const char Number11x20_6[11*20] = {
_,_,1,1,1,1,1,1,1,_,_,
_,1,1,1,1,1,1,1,1,1,_,
1,1,1,_,_,_,_,_,1,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,_,_,_,_,_,_,_,_,_,
1,1,_,_,_,_,_,_,_,_,_,
1,1,_,_,_,_,_,_,_,_,_,
1,1,_,_,_,_,_,_,_,_,_,
1,1,_,_,_,_,_,_,_,_,_,
1,1,_,1,1,1,1,1,_,_,_,
1,1,1,1,1,1,1,1,1,_,_,
1,1,1,1,_,_,_,1,1,1,_,
1,1,1,_,_,_,_,_,1,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,1,_,_,_,_,_,1,1,1,
_,1,1,1,1,1,1,1,1,1,_,
_,_,1,1,1,1,1,1,1,_,_,
};

static const char Number11x20_7[11*20] = {
1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,
_,_,_,_,_,_,_,_,_,1,1,
_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,1,1,1,_,
_,_,_,_,_,_,1,1,1,_,_,
_,_,_,_,_,1,1,1,_,_,_,
_,_,_,_,1,1,1,_,_,_,_,
_,_,_,1,1,1,_,_,_,_,_,
_,_,1,1,1,_,_,_,_,_,_,
_,_,1,1,_,_,_,_,_,_,_,
_,_,1,1,_,_,_,_,_,_,_,
_,_,1,1,_,_,_,_,_,_,_,
_,_,1,1,_,_,_,_,_,_,_,
_,_,1,1,_,_,_,_,_,_,_,
_,_,1,1,_,_,_,_,_,_,_,
_,_,1,1,_,_,_,_,_,_,_,
_,_,1,1,_,_,_,_,_,_,_,
_,_,1,1,_,_,_,_,_,_,_,
_,_,1,1,_,_,_,_,_,_,_,
};

static const char Number11x20_8[11*20] = {
_,_,1,1,1,1,1,1,1,_,_,
_,1,1,1,1,1,1,1,1,1,_,
1,1,1,_,_,_,_,_,1,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,1,_,_,_,_,_,1,1,1,
_,1,1,1,_,_,_,1,1,1,_,
_,_,1,1,1,1,1,1,1,_,_,
_,_,1,1,1,1,1,1,1,_,_,
_,1,1,1,_,_,_,1,1,1,_,
1,1,1,_,_,_,_,_,1,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,1,_,_,_,_,_,1,1,1,
_,1,1,1,1,1,1,1,1,1,_,
_,_,1,1,1,1,1,1,1,_,_,
};

static const char Number11x20_9[11*20] = {
_,_,1,1,1,1,1,1,1,_,_,
_,1,1,1,1,1,1,1,1,1,_,
1,1,1,_,_,_,_,_,1,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,1,_,_,_,_,_,1,1,1,
_,1,1,1,_,_,_,1,1,1,1,
_,_,1,1,1,1,1,1,1,1,1,
_,_,_,1,1,1,1,1,_,1,1,
_,_,_,_,_,_,_,_,_,1,1,
_,_,_,_,_,_,_,_,_,1,1,
_,_,_,_,_,_,_,_,_,1,1,
_,_,_,_,_,_,_,_,_,1,1,
_,_,_,_,_,_,_,_,_,1,1,
1,1,_,_,_,_,_,_,_,1,1,
1,1,1,_,_,_,_,_,1,1,1,
_,1,1,1,1,1,1,1,1,1,_,
_,_,1,1,1,1,1,1,1,_,_,
};

static const char Number15x29_0[15*29] = {
_,_,_,1,1,1,1,1,1,1,1,1,_,_,_,
_,_,1,1,1,1,1,1,1,1,1,1,1,_,_,
_,1,1,1,1,1,1,1,1,1,1,1,1,1,_,
1,1,1,1,1,_,_,_,_,_,1,1,1,1,1,
1,1,1,1,_,_,_,_,_,_,_,1,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,1,1,1,1,
1,1,1,_,_,_,_,_,_,_,1,1,1,1,1,
1,1,1,_,_,_,_,_,_,1,1,1,1,1,1,
1,1,1,_,_,_,_,_,1,1,1,1,1,1,1,
1,1,1,_,_,_,_,1,1,1,1,1,1,1,1,
1,1,1,_,_,_,1,1,1,1,1,_,1,1,1,
1,1,1,_,_,1,1,1,1,1,_,_,1,1,1,
1,1,1,_,1,1,1,1,1,_,_,_,1,1,1,
1,1,1,1,1,1,1,1,_,_,_,_,1,1,1,
1,1,1,1,1,1,1,_,_,_,_,_,1,1,1,
1,1,1,1,1,1,_,_,_,_,_,_,1,1,1,
1,1,1,1,1,_,_,_,_,_,_,_,1,1,1,
1,1,1,1,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,1,_,_,_,_,_,_,_,1,1,1,1,
1,1,1,1,1,_,_,_,_,_,1,1,1,1,1,
_,1,1,1,1,1,1,1,1,1,1,1,1,1,_,
_,_,1,1,1,1,1,1,1,1,1,1,1,_,_,
_,_,_,1,1,1,1,1,1,1,1,1,_,_,_,
};

static const char Number15x29_1[15*29] = {
_,_,_,_,_,_,1,1,1,_,_,_,_,_,_,
_,_,_,_,_,1,1,1,1,_,_,_,_,_,_,
_,_,_,_,1,1,1,1,1,_,_,_,_,_,_,
_,_,_,1,1,1,1,1,1,_,_,_,_,_,_,
_,_,_,1,1,1,1,1,1,_,_,_,_,_,_,
_,_,_,1,1,1,1,1,1,_,_,_,_,_,_,
_,_,_,_,_,_,1,1,1,_,_,_,_,_,_,
_,_,_,_,_,_,1,1,1,_,_,_,_,_,_,
_,_,_,_,_,_,1,1,1,_,_,_,_,_,_,
_,_,_,_,_,_,1,1,1,_,_,_,_,_,_,
_,_,_,_,_,_,1,1,1,_,_,_,_,_,_,
_,_,_,_,_,_,1,1,1,_,_,_,_,_,_,
_,_,_,_,_,_,1,1,1,_,_,_,_,_,_,
_,_,_,_,_,_,1,1,1,_,_,_,_,_,_,
_,_,_,_,_,_,1,1,1,_,_,_,_,_,_,
_,_,_,_,_,_,1,1,1,_,_,_,_,_,_,
_,_,_,_,_,_,1,1,1,_,_,_,_,_,_,
_,_,_,_,_,_,1,1,1,_,_,_,_,_,_,
_,_,_,_,_,_,1,1,1,_,_,_,_,_,_,
_,_,_,_,_,_,1,1,1,_,_,_,_,_,_,
_,_,_,_,_,_,1,1,1,_,_,_,_,_,_,
_,_,_,_,_,_,1,1,1,_,_,_,_,_,_,
_,_,_,_,_,_,1,1,1,_,_,_,_,_,_,
_,_,_,_,_,_,1,1,1,_,_,_,_,_,_,
_,_,_,_,_,_,1,1,1,_,_,_,_,_,_,
_,_,_,_,_,_,1,1,1,_,_,_,_,_,_,
_,_,_,1,1,1,1,1,1,1,1,1,_,_,_,
_,_,_,1,1,1,1,1,1,1,1,1,_,_,_,
_,_,_,1,1,1,1,1,1,1,1,1,_,_,_,
};

static const char Number15x29_2[15*29] = {
_,_,_,1,1,1,1,1,1,1,1,1,_,_,_,
_,_,1,1,1,1,1,1,1,1,1,1,1,_,_,
_,1,1,1,1,1,1,1,1,1,1,1,1,1,_,
1,1,1,1,1,_,_,_,_,_,1,1,1,1,1,
1,1,1,1,_,_,_,_,_,_,_,1,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,1,1,1,1,
_,_,_,_,_,_,_,_,_,_,1,1,1,1,1,
_,_,_,_,_,_,_,_,_,1,1,1,1,1,_,
_,_,_,_,_,_,_,_,1,1,1,1,1,_,_,
_,_,_,_,_,_,_,1,1,1,1,1,_,_,_,
_,_,_,_,_,_,1,1,1,1,1,_,_,_,_,
_,_,_,_,_,1,1,1,1,1,_,_,_,_,_,
_,_,_,_,1,1,1,1,1,_,_,_,_,_,_,
_,_,_,1,1,1,1,1,_,_,_,_,_,_,_,
_,_,1,1,1,1,1,_,_,_,_,_,_,_,_,
_,1,1,1,1,1,_,_,_,_,_,_,_,_,_,
1,1,1,1,1,_,_,_,_,_,_,_,_,_,_,
1,1,1,1,_,_,_,_,_,_,_,_,_,_,_,
1,1,1,_,_,_,_,_,_,_,_,_,_,_,_,
1,1,1,_,_,_,_,_,_,_,_,_,_,_,_,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};

static const char Number15x29_3[15*29] = {
_,_,_,1,1,1,1,1,1,1,1,1,_,_,_,
_,_,1,1,1,1,1,1,1,1,1,1,1,_,_,
_,1,1,1,1,1,1,1,1,1,1,1,1,1,_,
1,1,1,1,1,_,_,_,_,_,1,1,1,1,1,
1,1,1,1,_,_,_,_,_,_,_,1,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,1,1,1,1,
_,_,_,_,_,_,_,_,_,_,1,1,1,1,_,
_,_,_,_,_,1,1,1,1,1,1,1,1,_,_,
_,_,_,_,_,1,1,1,1,1,1,1,1,_,_,
_,_,_,_,_,1,1,1,1,1,1,1,1,_,_,
_,_,_,_,_,_,_,_,_,_,1,1,1,1,_,
_,_,_,_,_,_,_,_,_,_,_,1,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,1,_,_,_,_,_,_,_,1,1,1,1,
1,1,1,1,1,_,_,_,_,_,1,1,1,1,1,
_,1,1,1,1,1,1,1,1,1,1,1,1,1,_,
_,_,1,1,1,1,1,1,1,1,1,1,1,_,_,
_,_,_,1,1,1,1,1,1,1,1,1,_,_,_,
};

static const char Number15x29_4[15*29] = {
_,_,_,_,_,_,_,_,_,_,_,1,1,1,_,
_,_,_,_,_,_,_,_,_,_,1,1,1,1,_,
_,_,_,_,_,_,_,_,_,1,1,1,1,1,_,
_,_,_,_,_,_,_,_,1,1,1,1,1,1,_,
_,_,_,_,_,_,_,1,1,1,1,1,1,1,_,
_,_,_,_,_,_,1,1,1,1,_,1,1,1,_,
_,_,_,_,_,1,1,1,1,_,_,1,1,1,_,
_,_,_,_,1,1,1,1,_,_,_,1,1,1,_,
_,_,_,1,1,1,1,_,_,_,_,1,1,1,_,
_,_,1,1,1,1,_,_,_,_,_,1,1,1,_,
_,1,1,1,1,_,_,_,_,_,_,1,1,1,_,
1,1,1,1,_,_,_,_,_,_,_,1,1,1,_,
1,1,1,_,_,_,_,_,_,_,_,1,1,1,_,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,1,1,1,_,
_,_,_,_,_,_,_,_,_,_,_,1,1,1,_,
_,_,_,_,_,_,_,_,_,_,_,1,1,1,_,
_,_,_,_,_,_,_,_,_,_,_,1,1,1,_,
_,_,_,_,_,_,_,_,_,_,_,1,1,1,_,
_,_,_,_,_,_,_,_,_,_,_,1,1,1,_,
_,_,_,_,_,_,_,_,_,_,_,1,1,1,_,
_,_,_,_,_,_,_,_,_,_,_,1,1,1,_,
_,_,_,_,_,_,_,_,_,_,_,1,1,1,_,
_,_,_,_,_,_,_,_,_,_,_,1,1,1,_,
_,_,_,_,_,_,_,_,_,_,_,1,1,1,_,
_,_,_,_,_,_,_,_,_,_,_,1,1,1,_,
_,_,_,_,_,_,_,_,_,_,_,1,1,1,_,
};

static const char Number15x29_5[15*29] = {
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,_,_,_,
1,1,1,_,_,_,_,_,_,_,_,_,_,_,_,
1,1,1,_,_,_,_,_,_,_,_,_,_,_,_,
1,1,1,_,_,_,_,_,_,_,_,_,_,_,_,
1,1,1,_,_,_,_,_,_,_,_,_,_,_,_,
1,1,1,_,_,_,_,_,_,_,_,_,_,_,_,
1,1,1,_,_,_,_,_,_,_,_,_,_,_,_,
1,1,1,_,_,_,_,_,_,_,_,_,_,_,_,
1,1,1,_,_,_,_,_,_,_,_,_,_,_,_,
1,1,1,1,1,1,1,1,1,1,1,_,_,_,_,
1,1,1,1,1,1,1,1,1,1,1,1,_,_,_,
1,1,1,1,1,1,1,1,1,1,1,1,1,_,_,
_,_,_,_,_,_,_,_,_,1,1,1,1,1,_,
_,_,_,_,_,_,_,_,_,_,1,1,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,1,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,1,_,_,_,_,_,_,_,1,1,1,1,
1,1,1,1,1,_,_,_,_,_,1,1,1,1,1,
_,1,1,1,1,1,1,1,1,1,1,1,1,1,_,
_,_,1,1,1,1,1,1,1,1,1,1,1,_,_,
_,_,_,1,1,1,1,1,1,1,1,1,_,_,_,
};

static const char Number15x29_6[15*29] = {
_,_,_,1,1,1,1,1,1,1,1,1,_,_,_,
_,_,1,1,1,1,1,1,1,1,1,1,1,_,_,
_,1,1,1,1,1,1,1,1,1,1,1,1,1,_,
1,1,1,1,_,_,_,_,_,_,_,1,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,_,_,_,
1,1,1,_,_,_,_,_,_,_,_,_,_,_,_,
1,1,1,_,_,_,_,_,_,_,_,_,_,_,_,
1,1,1,_,_,_,_,_,_,_,_,_,_,_,_,
1,1,1,_,_,_,_,_,_,_,_,_,_,_,_,
1,1,1,_,_,_,_,_,_,_,_,_,_,_,_,
1,1,1,_,_,_,_,_,_,_,_,_,_,_,_,
1,1,1,_,1,1,1,1,1,1,1,1,_,_,_,
1,1,1,1,1,1,1,1,1,1,1,1,1,_,_,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,_,
1,1,1,1,1,_,_,_,_,_,1,1,1,1,1,
1,1,1,1,_,_,_,_,_,_,_,1,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,1,_,_,_,_,_,_,_,1,1,1,1,
1,1,1,1,1,_,_,_,_,_,1,1,1,1,1,
_,1,1,1,1,1,1,1,1,1,1,1,1,1,_,
_,_,1,1,1,1,1,1,1,1,1,1,1,_,_,
_,_,_,1,1,1,1,1,1,1,1,1,_,_,_,
};

static const char Number15x29_7[15*29] = {
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,1,1,1,1,
_,_,_,_,_,_,_,_,_,_,1,1,1,1,1,
_,_,_,_,_,_,_,_,_,1,1,1,1,1,_,
_,_,_,_,_,_,_,_,1,1,1,1,1,_,_,
_,_,_,_,_,_,_,1,1,1,1,1,_,_,_,
_,_,_,_,_,_,1,1,1,1,1,_,_,_,_,
_,_,_,_,_,1,1,1,1,1,_,_,_,_,_,
_,_,_,_,1,1,1,1,1,_,_,_,_,_,_,
_,_,_,1,1,1,1,1,_,_,_,_,_,_,_,
_,_,1,1,1,1,1,_,_,_,_,_,_,_,_,
_,_,1,1,1,1,_,_,_,_,_,_,_,_,_,
_,_,1,1,1,_,_,_,_,_,_,_,_,_,_,
_,_,1,1,1,_,_,_,_,_,_,_,_,_,_,
_,_,1,1,1,_,_,_,_,_,_,_,_,_,_,
_,_,1,1,1,_,_,_,_,_,_,_,_,_,_,
_,_,1,1,1,_,_,_,_,_,_,_,_,_,_,
_,_,1,1,1,_,_,_,_,_,_,_,_,_,_,
_,_,1,1,1,_,_,_,_,_,_,_,_,_,_,
_,_,1,1,1,_,_,_,_,_,_,_,_,_,_,
_,_,1,1,1,_,_,_,_,_,_,_,_,_,_,
_,_,1,1,1,_,_,_,_,_,_,_,_,_,_,
_,_,1,1,1,_,_,_,_,_,_,_,_,_,_,
_,_,1,1,1,_,_,_,_,_,_,_,_,_,_,
_,_,1,1,1,_,_,_,_,_,_,_,_,_,_,
};

static const char Number15x29_8[15*29] = {
_,_,_,1,1,1,1,1,1,1,1,1,_,_,_,
_,_,1,1,1,1,1,1,1,1,1,1,1,_,_,
_,1,1,1,1,1,1,1,1,1,1,1,1,1,_,
1,1,1,1,1,_,_,_,_,_,1,1,1,1,1,
1,1,1,1,_,_,_,_,_,_,_,1,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,1,_,_,_,_,_,_,_,1,1,1,1,
_,1,1,1,1,_,_,_,_,_,1,1,1,1,_,
_,_,1,1,1,1,1,1,1,1,1,1,1,_,_,
_,_,1,1,1,1,1,1,1,1,1,1,1,_,_,
_,_,1,1,1,1,1,1,1,1,1,1,1,_,_,
_,1,1,1,1,_,_,_,_,_,1,1,1,1,_,
1,1,1,1,_,_,_,_,_,_,_,1,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,1,_,_,_,_,_,_,_,1,1,1,1,
1,1,1,1,1,_,_,_,_,_,1,1,1,1,1,
_,1,1,1,1,1,1,1,1,1,1,1,1,1,_,
_,_,1,1,1,1,1,1,1,1,1,1,1,_,_,
_,_,_,1,1,1,1,1,1,1,1,1,_,_,_,
};

static const char Number15x29_9[15*29] = {
_,_,_,1,1,1,1,1,1,1,1,1,_,_,_,
_,_,1,1,1,1,1,1,1,1,1,1,1,_,_,
_,1,1,1,1,1,1,1,1,1,1,1,1,1,_,
1,1,1,1,1,_,_,_,_,_,1,1,1,1,1,
1,1,1,1,_,_,_,_,_,_,_,1,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,1,_,_,_,_,_,_,_,1,1,1,1,
1,1,1,1,1,_,_,_,_,_,1,1,1,1,1,
_,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
_,_,1,1,1,1,1,1,1,1,1,1,1,1,1,
_,_,_,1,1,1,1,1,1,1,1,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
_,_,_,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,_,_,_,_,_,_,_,_,_,1,1,1,
1,1,1,1,_,_,_,_,_,_,_,1,1,1,1,
1,1,1,1,1,_,_,_,_,_,1,1,1,1,1,
_,1,1,1,1,1,1,1,1,1,1,1,1,1,_,
_,_,1,1,1,1,1,1,1,1,1,1,1,_,_,
_,_,_,1,1,1,1,1,1,1,1,1,_,_,_,
};
#undef _

static const char *HexUpper5x7[] = {
   Number5x7_0,
   Number5x7_1,
   Number5x7_2,
   Number5x7_3,
   Number5x7_4,
   Number5x7_5,
   Number5x7_6,
   Number5x7_7,
   Number5x7_8,
   Number5x7_9,
   Ascii5x7_A,
   Ascii5x7_B,
   Ascii5x7_C,
   Ascii5x7_D,
   Ascii5x7_E,
   Ascii5x7_F,
};

static const char *Ascii5x7[128] = {
  //                0                   1            2               3            4               5            6            7
  //                8                   9            A               B            C               D            E            F
  /* 0x00 */        0,                  0,           0,              0,           0,              0,           0,           0,
                    0,                  0,           0,              0,           0,              0,           0,           0,
  /* 0x10 */        0,                  0,           0,              0,           0,              0,           0,           0,
                    0,                  0,           0,              0,           0,              0,           0,           0,
  /* 0x20 */ /* ! */0,                  0,    /* # */0,       /* $ */0,    /* % */0,       /* & */0,    /* ' */0, Ascii5x7_LPAREN,
             Ascii5x7_RPAREN,    /* * */0,    /* + */0,       /* ' */0,    /* - */0,       /* . */0,    /* / */0, Number5x7_0,
  /* 0x30 */ Number5x7_0,     Number5x7_1, Number5x7_2,    Number5x7_3, Number5x7_4,    Number5x7_5, Number5x7_6, Number5x7_7,
             Number5x7_8,     Number5x7_9,    /* : */0,       /* ; */0,    /* < */0,       /* = */0,    /* > */0,    /* ? */0,
  /* 0x40 */ /* @ */0,         Ascii5x7_A,  Ascii5x7_B,     Ascii5x7_C,  Ascii5x7_D,     Ascii5x7_E,  Ascii5x7_F,  Ascii5x7_G,
             Ascii5x7_H,       Ascii5x7_I,  Ascii5x7_J,     Ascii5x7_K,  Ascii5x7_L,     Ascii5x7_M,  Ascii5x7_N,  Ascii5x7_O,
  /* 0x50 */ Ascii5x7_P,       Ascii5x7_Q,  Ascii5x7_R,     Ascii5x7_S,  Ascii5x7_T,     Ascii5x7_U,  Ascii5x7_V,  Ascii5x7_W,
             Ascii5x7_X,       Ascii5x7_Y,  Ascii5x7_Z,       /* [ */0,    /* / */0,       /* ] */0,    /* ^ */0,    /* _ */0,
  /* 0x60 */   /* ` */0,         /* a */0,    /* b */0,       /* c */0,    /* d */0,       /* e */0,    /* f */0,    /* g */0,
               /* h */0,         /* i */0,    /* j */0,       /* k */0,    /* l */0,       /* m */0,    /* n */0,    /* o */0,
  /* 0x70 */   /* p */0,         /* q */0,    /* r */0,       /* s */0,    /* t */0,       /* u */0,    /* v */0,    /* w */0,
               /* x */0,         /* y */0,    /* z */0, Ascii5x7_LCURL,    /* | */0, Ascii5x7_RCURL,    /* ~ */0,  /* DEL */0
};

static const char *Numbers5x7[] = {
   Number5x7_0,
   Number5x7_1,
   Number5x7_2,
   Number5x7_3,
   Number5x7_4,
   Number5x7_5,
   Number5x7_6,
   Number5x7_7,
   Number5x7_8,
   Number5x7_9,
};

static const char *Numbers11x20[] = {
   Number11x20_0,
   Number11x20_1,
   Number11x20_2,
   Number11x20_3,
   Number11x20_4,
   Number11x20_5,
   Number11x20_6,
   Number11x20_7,
   Number11x20_8,
   Number11x20_9,
};

static const char *Numbers15x29[] = {
   Number15x29_0,
   Number15x29_1,
   Number15x29_2,
   Number15x29_3,
   Number15x29_4,
   Number15x29_5,
   Number15x29_6,
   Number15x29_7,
   Number15x29_8,
   Number15x29_9,
};


static unsigned width(enum BigNumber::Font F) {
  switch (F) {
  case BigNumber::Font::Ascii5x7:    return 5;
  case BigNumber::Font::HexUpper5x7: return 5;
  case BigNumber::Font::Digit5x7:    return 5;
  case BigNumber::Font::Digit11x20:  return 11;
  case BigNumber::Font::Digit15x29:  return 15;
  }
  return 0;
}

static unsigned height(enum BigNumber::Font F) {
  switch (F) {
  case BigNumber::Font::Ascii5x7:    return 7;
  case BigNumber::Font::HexUpper5x7: return 7;
  case BigNumber::Font::Digit5x7:    return 7;
  case BigNumber::Font::Digit11x20:  return 20;
  case BigNumber::Font::Digit15x29:  return 29;
  }
  return 0;
}

static const char *character(enum BigNumber::Font F, unsigned char Digit) {
  switch (F) {
  case BigNumber::Font::Ascii5x7:
    assert(Digit < 128);
    return Ascii5x7[Digit];
  case BigNumber::Font::HexUpper5x7:
    assert(Digit < 16);
    return HexUpper5x7[Digit];
  case BigNumber::Font::Digit5x7:
    assert(Digit < 10);
    return Numbers5x7[Digit];
  case BigNumber::Font::Digit11x20:
    assert(Digit < 10);
    return Numbers11x20[Digit];
  case BigNumber::Font::Digit15x29:
    assert(Digit < 10);
    return Numbers15x29[Digit];
  }
  return nullptr;
}

void BigNumber::Render(Canvas *Canvas,
                       unsigned Display,
                       unsigned char Digit,
                       unsigned XOffs, unsigned YOffs,
                       enum BigNumber::Font F,
                       const Color &FG,
                       const Color *BG) {
  unsigned H = height(F);
  unsigned W = width(F);
  const char *Character = character(F, Digit);

  if (!Character)
    return;

  XOffs += Display * 32;
  for (unsigned y = 0; y < H; y++) {
    for (unsigned x = 0; x < W; x++) {
      if (Character[x + y * W])
        Canvas->SetPixel(XOffs + x, YOffs + y, FG.r, FG.g, FG.b);
      else if (BG)
        Canvas->SetPixel(XOffs + x, YOffs + y, BG->r, BG->g, BG->b);
    }
  }
}

void BigNumber::printf(rgb_matrix::Canvas *Canvas,
                       unsigned XS, unsigned YS,
                       const rgb_matrix::Color &FG,
                       const rgb_matrix::Color *BG,
                       const char *Fmt, ...) {
  va_list a_list;
  va_start(a_list, Fmt);

  char Buff[32];
  vsnprintf(Buff, sizeof(Buff), Fmt, a_list);

  unsigned XOffs = XS;
  unsigned YOffs = YS;
  for (unsigned I = 0, E = sizeof(Buff); I != E; ++I, XOffs += 6) {
    if (!Buff[I])
      break;
    Render(Canvas, 0, Buff[I], XOffs, YOffs, BigNumber::Font::Ascii5x7, FG, BG);
  }
}

