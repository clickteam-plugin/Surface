#include "common.h"

/* CALLBACK */

CFillUser::CFillUser(RUNDATA* __rdPtr, const TCHAR* name)
{
	fillName = _tcsdup(name);
	rdPtr = __rdPtr;
}

CFillUser::~CFillUser()
{
	free(fillName);
}

BOOL CFillUser::Fill(cSurface FAR * pSf, int l, int t, int r, int b, BOOL bForceOpaqueBlack)
{
	//Save temporary infos
	//rdPtr->userData = fillName;
	//rdPtr->userRect.left = l;
	//rdPtr->userRect.top = t;
	//rdPtr->userRect.right = r;
	//rdPtr->userRect.bottom = b;
	
	//Loop through all pixels
	for(rdPtr->callX=l; rdPtr->callX<=r; ++rdPtr->callX)
	{
		for(rdPtr->callY=t; rdPtr->callY<=b; ++rdPtr->callY)
		{
			rdPtr->colRet = false;
			rdPtr->callback = fillName;
			rdPtr->rRd->GenerateEvent(8);
			//Sucessfully returned from the action!
			if(rdPtr->colRet)
				pSf->SetPixel(rdPtr->callX,rdPtr->callY,rdPtr->colNew);
		}
	}
	return TRUE;
}

//BOOL CFillUser::Fill(cSurface FAR * pSf, int l, int t, int r, int b, BOOL bForceOpaqueBlack)
//{
//	//Save temporary infos
//	rdPtr->userData = fillName;
//	rdPtr->userRect.left = l;
//	rdPtr->userRect.top = t;
//	rdPtr->userRect.right = r;
//	rdPtr->userRect.bottom = b;
//
//	//Color channel
//	BYTE* buff;
//	buff = TargetImg->LockBuffer();
//	if(!buff) return FALSE;
//
//	int height = b-t;
//	int pitch = TargetImg->GetPitch();
//	if(pitch < 0)
//	{
//		pitch *= -1;
//		buff -= pitch*(height-1);
//	}
//	int size = pitch*height;
//	int byte = TargetImg->GetDepth()>>3;
//
//	for(rdPtr->userX=l;rdPtr->userX<=r;rdPtr->userX++)
//	{
//		for(rdPtr->userY=t;rdPtr->userY<=b;rdPtr->userY++)
//		{
//			BYTE* p = buff+rdPtr->userY*pitch+rdPtr->userX*byte;
//			rdPtr->userTrigger = false;
//			rdPtr->rRd->GenerateEvent(8);
//			//Sucessfully returned from the action!
//			if(rdPtr->userTrigger)
//			{
//				memcpy(p,&rdPtr->userCol,3);
//				p[0] ^= p[2] ^= p[0] ^= p[2];
//			}
//		}
//	}
//
//	TargetImg->UnlockBuffer(buff);
//
//	return TRUE;
//}

/* LINEAR GRADIENT */

CFillDirEx::CFillDirEx(COLORREF crFrom, COLORREF crTo, BOOL Vertical)
{
	m_crFrom = crFrom;
	m_crTo = crTo;
	m_vert = Vertical;
}

void CFillDirEx::SetColors(COLORREF crFrom, COLORREF crTo)
{
	m_crFrom = crFrom;
	m_crTo = crTo;
}

void CFillDirEx::SetDir(BOOL vert)
{
	m_vert = vert;
}

BOOL CFillDirEx::Fill(cSurface FAR * pSf, int l, int t, int r, int b, BOOL bForceOpaqueBlack)
{
	BYTE ra=GetRValue(m_crFrom),ga=GetGValue(m_crFrom),ba=GetBValue(m_crFrom),
		rb=GetRValue(m_crTo),gb=GetGValue(m_crTo),bb=GetBValue(m_crTo);

	for(int x=l;x<=r;x++)
	{
		for(int y=t;y<=b;y++)
		{
			//Get percentage
			float fade;
			fade = m_vert?y/(float)(b-t):x/(float)(r-l);
			//Fade colors
			int cr,cg,cb;
			cr = ra+(rb-ra)*fade;
			cg = ga+(gb-ga)*fade;
			cb = ba+(bb-ba)*fade;
			pSf->SetPixel(x,y,RGB(cr,cg,cb));
		}
	}
	return TRUE;
}

/* RADIAL GRADIENT */

CFillRadial::CFillRadial(COLORREF crFrom, COLORREF crTo)
{
	m_crFrom = crFrom;
	m_crTo = crTo;
}

void CFillRadial::SetColors(COLORREF crFrom, COLORREF crTo)
{
	m_crFrom = crFrom;
	m_crTo = crTo;
}

BOOL CFillRadial::Fill(cSurface FAR * pSf, int l, int t, int r, int b, BOOL bForceOpaqueBlack)
{
	float cx = (r-l)/2.0f;
	float cy = (b-t)/2.0f;

	BYTE ra=GetRValue(m_crFrom),ga=GetGValue(m_crFrom),ba=GetBValue(m_crFrom),
		rb=GetRValue(m_crTo),gb=GetGValue(m_crTo),bb=GetBValue(m_crTo);

	for(int x=l;x<=r;x++)
	{
		for(int y=t;y<=b;y++)
		{
			float fade = sqrt(pow((x-cx)/cx,2)+pow((y-cy)/cy,2));
			fade = max(0.0f,min(1.0f,fade));
			int cr,cg,cb;
			cr = ra+(rb-ra)*fade;
			cg = ga+(gb-ga)*fade;
			cb = ba+(bb-ba)*fade;
			pSf->SetPixel(x,y,RGB(cr,cg,cb));
		}
	}
	return TRUE;
}