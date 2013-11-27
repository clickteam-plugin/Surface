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
	FillType	type;
	uint		param; //Image index for FILL_MOSAIC
	CFillData*	fill;
	bool		img;
};

struct PolyCoord
{
	float		x;
	float		y;
};

typedef	map<string,FillGuide*>	FillDB;

struct ColorCond
{
	char channel;
	char condition[2];
	int parameter;
};

struct CondOp
{
	char symbol[3];
	int length;
	int	precedence;
};

struct SImage
{
	//Whether or not the surface was created by the extension
	bool		local;
	//The actual image
	cSurface*	surf;
	//Hot-spot
	int			hotX;
	int			hotY;

	SImage(cSurface* surf, bool local = true, int hotX = 0, int hotY = 0)
	{
		this->local = local;
		this->surf = surf;
		this->hotX = hotX;
		this->hotY = hotY;
	}

	~SImage()
	{
		if(local)
			delete surf;
	}
};

typedef	vector<SImage*>		ImageBank;
typedef	map<string,ImageBank*>	ImageDB;

typedef	vector<PolyCoord>		PolyPoints;

struct OvlStruct;
typedef struct OvlStruct Overlay;

struct BlitSet
{
	BlitMode			mode;
	BlitOp				operation;
	LPARAM				param;
	DWORD				flags;
	int					destX;
	int					destY;
	int					destW;
	int					destH;
	int					StretchMode;
	int					srcX;
	int					srcY;
	int					srcW;
	int					srcH;
	bool				srcUse;
	int					angle;
	bool				angleResample;
	TCHAR				callback[32];
	TCHAR				procOp[4];
	float				procOpSrc;
	bool				composeAlpha;
	int					hotX;
	int					hotY;
	int					hotspotMode;
};

// --------------------------------
// RUNNING OBJECT DATA STRUCTURE
// --------------------------------
// If you want to store anything between actions/conditions/expressions
// you should store it here

class rRundata;
typedef rRundata * LPRRDATA;

typedef struct tagRDATA
{
	#include "MagicRDATA.h"
	
	ImageBank*			surf;
	cSurface*			target;
	short				targetId;
	bool				targetValid;
	cSurface*			current;
	short				currentId;
	short				lastId;
	//Functions
	cSurface*			(*imageAt)(tagRDATA*,int);
	int					(*imageCount)(tagRDATA*);
	//Display surface for rotations and scaling
	cSurface*			display;
	//The default image depth
	int					depth;
	//Internal blit
	BlitSet				b;
	BlitSet				blitStack[5];
	int					blitStackCursor;
	//Callback
	COLORREF			colSrc;
	COLORREF			colDest;
	COLORREF			colNew;
	int					colAlphaSrc;
	int					colAlphaDest;
	int					colAlphaNew;
	bool				colRet;
	bool				colAlphaRet;
	TCHAR*				callback;
	int					callX;
	int					callY;
	//Data
	sMask*				collision;
	PolyPoints*			points;
	FillDB*				fill;
	//Font
	HFONT				hFont;
	DWORD				hFlags;
	LOGFONT				textFont;
	//Options
	bool				useAbs;
	bool				keepPoints;
	bool				threadedIO;
	bool				dispTarget;
	bool				selectLast;
	//Settings of non-existent surfaces
	COLORREF			transCol;
	//Buffer lock
	BYTE*				buffer;
	cSurface*			buffSurf;
	int					buffPitch;
	//Overlay compatibility
	Overlay*			ovl;
	//Flood fill
	int					floodL;
	int					floodR;
	int					floodT;
	int					floodB;
	//I/O stuff
	TCHAR*				ioFile;
	DWORD				ioFilter;
	HANDLE				ioHandle;
	bool				ioSave;
	//Stored image for quick undo
	cSurface*			stored;
	// Misc
	bool				isHWA;
	int					randomSeed;
	
} RUNDATA;
typedef	RUNDATA	* LPRDATA;

typedef struct
{
	headerObject	rHo;					// Header
	rCom			roc;
	rSpr			rs;
	rMvt			rm;
	rVal			rv;
	bool			FlipHoz;
	bool			FlipVer;
	int				Angle;
	bool			RotationAA;
	long			SourceX;
	long			SourceY;
	long			SourceWidth;
	long			SourceHeight;
	LPSURFACE		PSoverride;
	bool			StretchResample;
	LPSURFACE		sourceImg;
	LPSURFACE		rotateImg;

} Viewport;

typedef struct
{
	headerObject	rHo;					// Header
	rCom			roc;
	rSpr			rs;
	rMvt			rm;
	cSurface	*	TempImage;
	int				Effect;
	bool			Direction;
	int				ZoomValue;
	int				Offset;
	int				SineWaveWaves;
	bool			PerspectiveDir;
	int *			CustomArray;
	bool			resample;

} Perspective;

	//RECT				rect;
	//BOOL				useRect;
	//long				oldX;
	//long				oldY;
	//long				oldWidth;
	//long				oldHeight;
	//cSurface*			background;

// --------------------------------
// EDITION OF OBJECT DATA STRUCTURE
// --------------------------------
// These values let you store data in your extension that will be saved in the MFA
// You should use these with properties

typedef struct tagEDATAA_V1
{
	extHeader		eHeader;
	//Image bank
	short			width;
	short			height;
	short			width_def; //Width & height of the default image
	short			height_def;
	WORD			images[MAX_IMAGES];
	short			imageCount;

	bool			loadFirst;
	bool			useAbs;
	bool			threadedIO;
	bool			keepPoints;
	bool			multiImg;
	bool			dispTarget;
	bool			selectLast;

	LOGFONTA		textFont;
	COLORREF		textColor;
	DWORD			textFlags;

} EDITDATAA;

typedef struct tagEDATAW_V1
{
	extHeader		eHeader;
	//Image bank
	short			width;
	short			height;
	short			width_def; //Width & height of the default image
	short			height_def;
	WORD			images[MAX_IMAGES];
	short			imageCount;

	bool			loadFirst;
	bool			useAbs;
	bool			threadedIO;
	bool			keepPoints;
	bool			multiImg;
	bool			dispTarget;
	bool			selectLast;

	LOGFONTW		textFont;
	COLORREF		textColor;
	DWORD			textFlags;

} EDITDATAW;

#ifdef _UNICODE
#define EDITDATA	EDITDATAW
#define LPEDATA		LPEDATAW
#else
#define EDITDATA	EDITDATAA
#define LPEDATA		LPEDATAA
#endif

typedef EDITDATA * LPEDATA;


struct OvlStruct
{
	headerObject	rHo;					// Header
	rVal			rv;						// Values
	LPSURFACE		runSurface;
	long			X;
	long			Y;
	long			OldX;
	long			OldY;
	short			swidth;
	short			sheight;
	short			oldwidth;		//For stretching
	short			oldheight;		//For stretching
	char			rFilename[_MAX_PATH];
	BOOL			debugmode;		//Is debug mode on? (visibility)
	BOOL            stretching;		//Is stretching on?
	BOOL			AbsolutePos;	//drawing position absolute
	BOOL			Immediate;		//update display immediately
	BOOL			Automatic;		//update display automatically
	BOOL			AntiA;			//anti-aliasing
	BlitMode		Mode;			//display mode (opaque or trans)
	BlitOp			Effect;			//blit effect
	int				EffectParam;	//effect parameter
	POINT *			Points;			//points for "Plot Polygon"
	unsigned long	NumPoints;		//number of points to plot
	unsigned long	SizePoints;		//size of array of points
	BOOL			RemovePoints;	//remove points after plotting polygon
	BOOL			Fade;			//true if we should be fading
	float			FadeSpeed;		//speed at which to fade
	float			FadeInk;		//current ink effect param as a float
	/* ADDITION  lolwut? */
	int				image;
};

typedef struct APDATA
{
	headerObject 	rHo;					// Header
	rCom			rc;						// Structure commune pour les mouvements/animations
	rMvt			rm;						// Structure de mouvements
	rSpr			rs;
	rVal			rv;						// Alterable values

	char			szImageName[_MAX_PATH];
	DWORD			dwImageFlags;
	DWORD			dwTranspColor;
	DWORD			dwPictureWidth;
	DWORD			dwPictureHeight;
	DWORD			dwScreenWidth;
	DWORD			dwScreenHeight;
	DWORD			dwEditorWidth;
	DWORD			dwEditorHeight;

	int				iHotSpotX;
	int				iHotSpotY;
//	int				iRealBlendCoef;
#ifdef HWABETA
	float			fAngle;
#else
	int				iAngle;
#endif
	LPSURFACE		psfOrg;
	LPSURFACE		psf;

	RECT			rBkd;
	LPSURFACE		pSfBkd;

	int				nOffsetX;
	int				nOffsetY;

#ifdef HWABETA
	bool			bHWA;
	bool			bAlpha;
//	bool			m_bReloadAfterLostDevice;
#endif

} ActivePicture;


typedef struct
{
	headerObject rHo;
	rCom rc;
	rMvt rm;
	rSpr rs;
	rVal rv;
	LPRRDATA rRd;
	
	int				width;
	int				height;

	int				shape;
	int				fillType;
	int				gradDir;

	COLORREF		fillColor;
	COLORREF		fillColor2;
	COLORREF		lineColor;
	int				lineWeight;

	bool			useTrans;
	DWORD			imageOptions;

	COLORREF		transColor;
	WORD*			patterns;
	WORD*			images;

	size_t			patternCount;
	size_t			imageCount;

	char*			text;
	COLORREF		textColor;
	HFONT			textFont;
	DWORD			textAlignFlags;
	DWORD			textEffect;
	DWORD			textCoef;

	int				polyn;
	char*			polyCoords;
	tagPOINT*		polySet;

	int				rebuild;
	int				rebuildPoly;
	int				rebuildTrans;

	cSurface*		pSf;
	sMask*			pMask;

	int				hotX;
	int				hotY;

	// Transformed data
	cSurface*		transSf;
	sMask*			transMask;

	int				diffWidth;
	int				diffHeight;
	
} ActiveShape;

//Callback
class CFillUser : public CFillData
{
public:
	RUNDATA*	rdPtr;
	TCHAR*	fillName;

public:
	CFillUser(RUNDATA* __rdPtr,const TCHAR* name);
	~CFillUser();
	virtual BOOL Fill(cSurface FAR * pSf, int l, int t, int r, int b, BOOL bForceOpaqueBlack=FALSE);
};

//Radial gradient
class CFillRadial : public CFillData
{
public:
	COLORREF	m_crFrom;
	COLORREF	m_crTo;

public:
	CFillRadial(COLORREF crFrom, COLORREF crTo);
	void SetColors(COLORREF crFrom, COLORREF crTo);
	virtual BOOL Fill(cSurface FAR * pSf, int l, int t, int r, int b, BOOL bForceOpaqueBlack=FALSE);
};

//Linear gradient
class CFillDirEx : public CFillData
{
public:
	// Constructors
	CFillDirEx(COLORREF crFrom, COLORREF crTo, BOOL Vertical);

public:
	int m_xOrg;
	int m_yOrg;
	// Attributes
	COLORREF	m_crFrom;
	COLORREF	m_crTo;
	BOOL		m_vert;

public:
	void SetColors(COLORREF crFrom, COLORREF crTo);
	void SetDir(BOOL vert);
	// Implementation
	virtual BOOL Fill(cSurface FAR * pSf, int l, int t, int r, int b, BOOL bForceOpaqueBlack=FALSE);
};