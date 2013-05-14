/* Common.h
 * This is the common header file included
 * by almost all other *.cpp files in
 * the project. Anything that gets put or
 * included in here will be available to you
 * everywhere else. You shouldn't need to
 * change anything in this file except
 * to include headers that you want and
 * to change the edition of MMF2 you want
 * to support.
 */

#pragma once

//Only define one of these at once:
//	#define TGFEXT	//TGF2, MMF2, MMF2 Dev
	#define MMFEXT	//MMF2, MMF2 Dev
//	#define PROEXT	//MMF2 Dev only

//Define this if you want legacy comma-less JSON support
//	#define UsingJsonWithoutCommas

#include "Edif.h"
#include "Resource.h"

#include <map>
#include <vector>

/* stdtstring
 * A std::string that knows if you're using
 * unicode or not. (Protip: strings passed
 * to your extension A/C/Es still need to be
 * const TCHAR *, and the same goes for strings
 * returned from expressions and conditions).
 */
typedef std::basic_string<TCHAR> stdtstring;

#define MAX_IMAGES 16
#define DEF_WIDTH  320
#define DEF_HEIGHT 240

#define FORCE_FALSE 2

WORD CreateImageFromSurface(mv *mV, cSurface *surf, int width, int height, int hotSpotX, int hotSpotY, int actionPoint);
//BOOL WINAPI ExportSurface(CImageFilterMgr *ImgMgr, LPTCSTR FileName, cSurface *surf, DWORD FilterID, int compress);
inline TCHAR const *GetExtension(TCHAR const *Filename);
DWORD FindFilter(CImageFilterMgr *ImgMgr, TCHAR const *file, bool isext = false);
void Operation(TCHAR const *op, BYTE *a, float b);
unsigned int HexToRGB(TCHAR const *Hex);
//int Evaluate(int colot, TCHAR const *str, TCHAR const *end = 0);
//void parseExpression(TCHAR const *string, int *code, int codeSize);
//int evalExpression(int *stack, int capacity, int *code, int *vars);

#define ImageS(id) surf->at(id)
#define Image(id)  surf->at(id)->surf
#define TargetImg  target
#define CurrentImg current

#define UpdateOverlay()              \
	if(ovl && olv->image == targetId) \
	{                                  \
		if(targetValid)                 \
		{                                \
			ovl->runSurface = TargetImg;  \
		}                                  \
		else                                \
		{                                    \
			ovl->runSurface = 0;              \
		}                                      \
	}

#define ImageRead(param1, tmp)               \
	cSurface *tmp;                            \
	if(param1 != lastId)                       \
	{                                           \
		if(param1 < 0 || param1 >= surf->size()) \
		{                                         \
			return 0;                              \
		}                                           \
		tmp = Image(param1);                         \
		if(!temp->IsValid())                          \
		{                                              \
			return 0;                                   \
		}                                                \
		last = tmp;                                       \
		lastId = param1;                                   \
	}                                                       \
	else                                                     \
	{                                                         \
		tmp = last;                                            \
	}

#define TargetExists()                          \
	if(ioHandle || !targetValid || targetId < 0) \
	{                                             \
		return 0;                                  \
	}

#define ImageExists(param1)                                                          \
	if(ioHandle || param1 < 0 || param1 >= surf->size() || !Image(param1)->IsValid()) \
	{                                                                                  \
		return 0;                                                                       \
	}

#define ImageInRange(param1)                            \
	if(ioHandle || param1 < 0 || param1 >= surf->size()) \
	{                                                     \
		return 0;                                          \
	}

#define ImageIsSafe(param1)                            \
	if(ioHandle || param1 < 0 || param1 > surf->size()) \
	{                                                    \
		return 0;                                         \
	}

#define Current(param1) (param1 == currentId)

#define	ImageChanged() \
{                       \
	rc.rcChanged = true; \
}
#define RectChanged(x1,y1,x2,y2) ImageChanged() //Obsolete

#define ParentRedraw() //\
{                       \
	redraw = true;       \
}
#define MaskChanged() FreeColMask()

#define GetXPos(param) param - (useAbs ? (rd->rHo.hoRect.left + rd->rHo.hoAdRunHeader->rhWindowX) : 0)
#define GetYPos(param) param - (useAbs ? (rd->rHo.hoRect.top  + rd->rHo.hoAdRunHeader->rhWindowY) : 0)
#define GetXPos2 GetXPos
#define GetYPos2 GetYPos

inline int round(float fl)
{
	if(fl - int(fl) >= 0.5)
	{
		return int(/*std::*/ceil(fl));
	}
	return int(fl);
}

inline int swap(int color)
{
	char *bytes = static_cast<char *>(&color);

	bytes[3] = bytes[0];
	bytes[0] = bytes[2];
	bytes[2] = bytes[3];

	return color;
}

#define USE_AA (rd->rs.rsEffect & EFFECTFLAG_ANTIALIAS ? TRUE : FALSE)

enum //FillType
{
	FILL_NONE,

	//Inbuilt
	FILL_COLOR,
	FILL_GRADIENT,
	FILL_MOSAIC,

	//Custom
	FILL_USER,
	FILL_RADIAL,

	//Image flag
	FILL_IMAGE = 0xf0000000
};
typedef int FillType;

struct FillGuide
{
	FillType   type;
	uint       param;
	CFillData *fill;
	bool       img;
};

struct PolyCoord
{
	float x, y;
};

typedef std::map<stdtstring, FillGuide *> FillDB;

struct ColorCond
{
	char channel;
	char condition[2];
	int  parameter;
};

struct CondOp
{
	char symbol[3];
	int  length;
	int  precedence;
};

struct SImage
{
	bool      local;      //Whether or not the surface was created by the extension
	cSurface *surf;       //The actual image
	int       hotX, hotY; //Hot-spot

	SImage(cSurface *surf, bool local = true, int hotx = 0, int hoty = 0)
	: local(local)
	, surf(surf)
	, hotX(hotx)
	, hotY(hoty)
	{
	}
	~SImage()
	{
		if(local)
		{
			delete surf; //surf->Delete() ?
		}
	}
};

typedef std::vector<SImage *>             ImageBank;
typedef std::map<stdtstring, ImageBank *> ImageDB;

typedef std::vector<PolyCoord> PolyPoints;

struct OvlStruct;
typedef struct OvlStruct Overlay;

struct Viewport
{
	headerObject rHo;
	rCom         roc;
	rSpr         rs;
	rMvt         rm;
	rVal         rv;
	bool         FlipHoz
	,            FlipVer;
	int          Angle;
	bool         RotationAA;
	long         SourceX
	,            SourceY
	,            SourceWidth
	,            SourceHeight;
	LPSURFACE    PSoverride;
	bool         StretchResample;
	LPSURFACE    sourceImg
	,            rotateImg;
};

struct Perspective
{
	headerObject rHo;
	rCom         roc;
	rSpr         rs;
	rMvt         rm;
	cSurface    *TempImage;
	int          Effect;
	bool         Direction;
	int          ZoomValue
	,            Offset
	,            SineWaveWaves;
	bool         PerspectiveDir;
	int         *CustomArray;
	bool         resample;
};

struct OvlStruct
{
	headerObject  rHo;
	rVal          rv;
	LPSURFACE     runSurface;
	long          X
	,             Y
	,             OldX
	,             OldY;
	short         swidth
	,             sheight
	,             oldwidth     //for
	,             oldheight;   //stretching
	char          rFilename[_MAX_PATH];
	BOOL          debugmode    //Is debug mode on? (visibility)
	,             stretching   //Is stretching on?
	,             AbsolutePos  //drawing position absolute
	,             Immediate    //update display immediately
	,             Automatic    //update display automatically
	,             AntiA;       //anti-aliasing
	BlitMode      Mode;        //display mode (opaque or trans)
	BlitOp        Effect;      //blit effect
	int           EffectParam; //effect parameter
	POINT        *Points;      //points for "Plot Polygon"
	unsigned long NumPoints    //number of points to plot
	,             SizePoints;  //size of array of points
	BOOL          RemovePoints //remove points after plotting polygon
	,             Fade;        //true if we should be fading
	float         FadeSpeed    //speed at which to fade
	,             FadeInk;     //current ink effect param as a float
	/* ADDITION   lolwut? */
	int           image;
};

struct ActivePicture
{
	headerObject rHo;
	rCom         rc;
	rMvt         rm;
	rSpr         rs;
	rVal         rv;

	char         szImageName[_MAX_PATH];
	DWORD        dwImageFlags
	,            dwTranspColor
	,            dwPictureWidth
	,            dwPictureHeight
	,            dwScreenWidth
	,            dwScreenHeight
	,            dwEditorWidth
	,            dwEditorHeight;

	int          iHotSpotX
	,            iHotSpotY;
//	int          iRealBlendCoef;
#ifdef HEWBETA
	float        fAngle;
#else
	int          iAngle;
#endif
	LPSURFACE    psfOrg
	,            psf;

	RECT         rBkd;
	LPSURFACE    pSfBkd;

	int          nOffsetX
	,            nOffsetY;

#ifdef HWABETA
	bool         bHWA
	,            bAlpha;
//	bool         m_bReloadAfterLostDevice;
#endif
};
typedef ActivePicture APDATA;

struct ActiveShape
{
	headerObject rHo;
	rCom         rc;
	rMvt         rm;
	rSpr         rs;
	rVal         rv;
	LPRRDATA     rRd;

	int          width
	,            height;

	int          shape
	,            fillType
	,            gradDir;

	COLORREF     fillColor
	,            fillColor2
	,            lineColor;
	int          lineWeight;

	bool         useTrans;
	DWORD        imageOptions;

	COLORREF     transColor;
	WORD        *patterns
	,           *images;

	std::size_t  patternCount
	,            imageCount;

	char const  *text;
	COLORREF     textColor;
	HFONT        textFont;
	DWORD        txtAlignFlags
	,            textEffect
	,            textCoef;

	int          polyn;
	char const  *polyCoords;
	tagPOINT    *polySet;

	int          rebuild
	,            rebuildPoly
	,            rebuildTrans;

	cSurface    *pSf;
	sMask       *pMask;

	int          hotX
	,            hotY;

	//Transformed data
	cSurface    *transSf;
	sMask       *transMask;

	int          diffWidth
	,            diffHeight;
};

//Callback
struct CFillUser : CFillData
{
	Extension  *ext;
	char const *fillName;

	CFillUser(Extension *ext, char const *name);
	virtual ~CFillUser();
	virtual BOOL Fill(cSurface *surf, int l, int t, int r, int b, BOOL ForceOpaqueBlack = FALSE);
};

//Radial gradient
struct CFillRadial : CFillData
{
	COLORREF m_crFrom
	,        m_crTo;

	CFillRadial(COLORREF from, COLORREF to);
	virtual ~CFillRadial();
	void SetColors(COLORREF from, COLORREF to);
	virtual BOOL Fill(cSurface *surf, int l, int t, int r, int b, BOOL ForceOpaqueBlack = FALSE);
};

//Linear gradient
struct CFillDirEx : CFillData
{
	CFillDirEx(COLORREF from, COLORREF to, BOOL vertical);
	virtual ~CFillDirEx();

	int      m_xOrg
	,        m_yOrg;
	//Attributes
	COLORREF m_crFrom
	,        m_crTo;
	BOOL     m_vert;

	void SetColors(COLORREF from, COLORREF to);
	void SetDir(BOLL vert);
	//Implementation
	virtual BOOL Fill(cSurface *surf, int l, int t, int r, int b, BOOL ForceOpaqueBlack = FALSE);
};

#include "EditData.hpp"
#include "Extension.hpp"
