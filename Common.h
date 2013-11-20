// Include guard
#ifndef COMMON_H
#define COMMON_H

// Suppress the deprecated warnings for VC2005
#define _CRT_SECURE_NO_WARNINGS
#define OPTIMIZED_ACTION
#define _CFCFILTERMGR_UNICODE_DEFS

#define	MAX_IMAGES	16
#define DEF_WIDTH	320
#define DEF_HEIGHT	240

//#define	CUSTOM_BACKSAVE
#define FORCE_FALSE 2


// General includes
#include	"TemplateInc.h"
#include	<string>
#include	<vector>
#include	<map>
#include	<iostream>
using std::vector;
using std::map;
using std::cin;
using std::cout;
using std::pair;
using std::swap;
#include	<time.h>

#ifdef UNICODE
using std::wstring;
typedef wstring string;
#else
using std::string;
#endif

// Specific to this extension
#include	"Resource.h"
#include	"FlagsPrefs.h"
#include	"Information.h"
#include	"ImageFlt.h"
#include	"ImgFlt.h"
#include	"CfcFile.h"
#include	"Data.h"

WORD CreateImageFromSurface(LPMV pMV, LPSURFACE pSf, int dwWidth, int dwHeight, int hotSpotX, int hotSpotY, int actionPointX, int actionPointY);
//BOOL WINAPI ExportSurface (CImageFilterMgr* pImgMgr, LPCSTR pFileName, cSurface* psf, DWORD dwFilterID, int compress);
const TCHAR * GetExtension(const TCHAR * Filename);
DWORD FindFilter(CImageFilterMgr* pImgMgr,TCHAR* file,bool isext = false);
int GetImgCount(RUNDATA* rdPtr);
cSurface* GetImgAt(RUNDATA* rdPtr,int image);
bool CreateFillData(RUNDATA* rdPtr,string name,CFillData* fill,int type,int param);
void FreeColMask(LPRDATA rdPtr);
void RectUpdate(LPRDATA rdPtr,int x1, int y1, int x2, int y2);
bool Blit(cSurface* source,cSurface* dest,LPRDATA rdPtr);
int ProcessFillFlags(RUNDATA*rdPtr,string fill,string out,bool&useFill,bool&useOut,FillGuide*&fillData,FillGuide*&outData);
void Operation(TCHAR* op,BYTE* a,float b);
void UpdateHotspot(LPRDATA rdPtr);
//void ParseCondition(LPRDATA rdPtr, char* cond, ColorCond* conds, int n);
//bool ConditionMet(LPRDATA rdPtr, ColorCond* conds, int n, COLORREF color);
//int Evaluate(int color, char* str, char* end = 0);
//void parseExpression(char* string, int* code, int codeSize);
//int evalExpression(int* stack, int capacity, int* code, int* vars);

#define ImageS(id) rdPtr->surf->at(id)
#define Image(id) rdPtr->surf->at(id)->surf
#define TargetImg rdPtr->target
#define CurrentImg rdPtr->current

#define UpdateOverlay() if(rdPtr->ovl&&rdPtr->ovl->image==rdPtr->targetId) {\
		if(rdPtr->targetValid) rdPtr->ovl->runSurface = TargetImg;\
		else rdPtr->ovl->runSurface = 0;\
	}

#define ImageRead(param1,tmp) cSurface* tmp; if(param1!=rdPtr->lastId) {\
		if(param1<0||param1>=rdPtr->surf->size()) return 0; tmp = Image(param1);\
		if(!tmp->IsValid()) return 0; rdPtr->last = tmp; rdPtr->lastId = param1;\
	} else tmp = rdPtr->last;

#define TargetExists() if(rdPtr->ioHandle||!rdPtr->targetValid||rdPtr->targetId<0) return 0
#define ImageExists(param1) if(rdPtr->ioHandle||param1<0||param1>=rdPtr->surf->size()||!Image(param1)->IsValid()) return 0
#define ImageInRange(param1) if(rdPtr->ioHandle||param1<0||param1>=rdPtr->surf->size()) return 0
#define ImageIsSafe(param1) if(rdPtr->ioHandle||param1<0||param1>rdPtr->surf->size()) return 0
#define Current(param1) (param1==rdPtr->currentId)

#ifdef CUSTOM_BACKSAVE
#define	ImageChanged() { /* rdPtr->useRect=FORCE_FALSE; */ rdPtr->rc.rcChanged = true; }
#define RectChanged(x1,y1,x2,y2) { if(rdPtr->useRect!=FORCE_FALSE) RectUpdate(rdPtr,x1,y1,x2,y2); rdPtr->rc.rcChanged = true; }
#else
#define	ImageChanged() { rdPtr->rc.rcChanged = true; }
#define RectChanged(x1,y1,x2,y2) ImageChanged() //Obsolete
#endif

#define ParentRedraw() //{ rdPtr->redraw = true; }
#define MaskChanged() FreeColMask(rdPtr)

#define GetInt() CNC_GetIntParameter(rdPtr)
#define GetParam() CNC_GetParameter(rdPtr)
#define GetStr() (TCHAR*)CNC_GetStringParameter(rdPtr)
#define GetXPos() CNC_GetIntParameter(rdPtr)-(rdPtr->useAbs?(rdPtr->rHo.hoRect.left+rdPtr->rHo.hoAdRunHeader->rhWindowX):0)
#define GetYPos() CNC_GetIntParameter(rdPtr)-(rdPtr->useAbs?(rdPtr->rHo.hoRect.top+rdPtr->rHo.hoAdRunHeader->rhWindowY):0)
#define GetCol() Param(TYPE_GENERIC)//GetColorInline(rdPtr)//GetColorInline(rdPtr)
#define LoadFloat(foo) 	{ long tmp = CNC_GetFloatParameter(rdPtr); foo = *reinterpret_cast<float*>(&tmp); }
inline COLORREF GetColorInline(LPRDATA rdPtr)
{
	long tmpf=CNC_GetFloatParameter(rdPtr);
	float param=*(float*)&tmpf;
	return (COLORREF)param;
}
#define GetInt2() ExParam(TYPE_INT)
#define GetXPos2() ExParam(TYPE_INT)-(rdPtr->useAbs?(rdPtr->rHo.hoRect.left+rdPtr->rHo.hoAdRunHeader->rhWindowX):0)
#define GetYPos2() ExParam(TYPE_INT)-(rdPtr->useAbs?(rdPtr->rHo.hoRect.top+rdPtr->rHo.hoAdRunHeader->rhWindowY):0)
#define GetStr2() (TCHAR*)ExParam(TYPE_STRING)

__inline int round(float fl)
{
	if(fl-(int)fl>=0.5)
		return ceil(fl);
	return fl;
}

inline int swap(int color)
{
	char * bytes = (char *) &color;

	bytes[3] = bytes[0];
	bytes[0] = bytes[2];
	bytes[2] = bytes[3];
	
	return color;
}

#define USE_AA (rdPtr->rs.rsEffect&EFFECTFLAG_ANTIALIAS?TRUE:FALSE)


// rTemplate include
#include	"rTemplate.h"

// Globals and prototypes
extern HINSTANCE hInstLib;
extern short * conditionsInfos;
extern short * actionsInfos;
extern short * expressionsInfos;
extern long (WINAPI ** ConditionJumps)(LPRDATA rdPtr, long param1, long param2);
extern short (WINAPI ** ActionJumps)(LPRDATA rdPtr, long param1, long param2);
extern long (WINAPI ** ExpressionJumps)(LPRDATA rdPtr, long param);
extern PropData Properties[];
extern WORD DebugTree[];

// End include guard
#endif
