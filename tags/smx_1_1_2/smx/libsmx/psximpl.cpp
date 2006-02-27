/*
Copyright 1998-2005 Erik Aronesty. All rights reserved.

        1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

        2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

	3. Original source code, information and documentation is available at www.smxlang.org

THIS SOFTWARE IS PROVIDED 'AS IS' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE SMX AUTHORS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/



#include "stdafx.h"

#include "qctx.h"

#include "psxext.h"
#include "psximpl.h"

void STDCALL psxExObjectImpl::Eval(psxExContext *pCtx, psxExStreamOut *pOut, const char *pArgs[], psxArgType pArgType[], int nNumArgs)
{
	qArgAry ary;
	int i;
	for (i = 0; i < nNumArgs; ++i) {
		ary.Add(pArgs[i]);
		ary.SetQuot(i, pArgType ? (pArgType[i] == psxQuotedString) : false);
	}

	psxExStreamOutWrap wrap(pOut, false);
	myObj->Eval(((psxExContextImpl *)pCtx)->GetCtx(), &wrap);
}


void psxExFuncWrap::Eval(qCtx *ctx, qStr *out, qArgAry *args) {
	psxExContextImpl exCtx(ctx, false);
	psxExStreamOutImpl exOut(out, false);
	myFunc(myData, &exCtx, &exOut, (const char **) (args ? (const char **) *args : NULL), (psxArgType*)args->GetQuots(), args->Count());
}

void psxExObjectWrap::Eval(qCtx *ctx, qStr *out, qArgAry *args) {
	psxExContextImpl exCtx(ctx, false);
	psxExStreamOutImpl exOut(out, false);
	myObj->Eval(&exCtx, &exOut, (const char **) (args ? (const char **) *args : NULL), (psxArgType*)args->GetQuots(), args->Count());
}
