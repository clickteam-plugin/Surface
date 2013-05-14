/* Extension.h
 * This is where you define your extension
 * class. Runtime variables and function
 * declarations go in here; A/C/E function
 * definitions go in their respective .cpp
 * files, other function definitions go in
 * Extension.cpp. This header is included
 * at the bottom of Common.h, so you have
 * access to everything included by it.
 */

class Extension
{
public:
	/* rd
	 * This is a pointer to the classic
	 * RunData structure. Some things still
	 * have to be stored there for MMF2's
	 * sake, so you need to have acces to
	 * that data. You should not modify
	 * the rundata structure, however,
	 * as it is global to all extensions.
	 */
	RD *rd;

	/* rh
	 * This is a pointer to the RunHeader
	 * structure, which contains some
	 * application information.
	 */
	RunHeader *rh;

	/* Runtime
	 * This is EDIF's compendium of useful
	 * functions for every day life as an
	 * extension. You can trigger immediate
	 * events, read and write global data,
	 * etc. See the EDIF wiki for more
	 * information.
	 */
	Edif::Runtime Runtime;

	/* MinimumBuild
	 * Always set this to the latest build
	 * of MMF2 that has been released, unless
	 * you need to set it higher for a beta
	 * version of MMF2 to get access to some
	 * new SDK feature.
	 */
	static const int MinimumBuild = 257;

	/* Version
	 * This is the version of your extension
	 * that is stored in the editdata. You should
	 * only change this when your editdata structure
	 * changes, not just when you have a new version
	 * of your extension with new features.
	 */
	static const int Version = 2;
	static const int Version_rSDK = 1;

	/* OEFLAGS, OEPREFS
	 * These are settings for your extension
	 * that tell MMF2 how to treat your extension
	 * and what behaviors and responsibilities
	 * to add. Check the MMF2SDK Help file for
	 * more detailed information on the flags you
	 * can set and what they implicate.
	 */
	static const int OEFLAGS = OEFLAG_BACKSAVE|OEFLAG_MOVEMENTS|OEFLAG_VALUES|OEFLAG_SPRITES;
	static const int OEPREFS = OEPREFS_BACKSAVE|OEPREFS_SCROLLINGINDEPENDANT|OEPREFS_SLEEP|OEPREFS_KILL|OEPREFS_INKEFFECTS|OEPREFS_FINECOLLISIONS;

	/* WindowProcPriority
	 * If your extension creates a Window
	 * and implements the WindowProc function,
	 * this is the priority for that stuff.
	 */
	static const int WindowProcPriority = 100;

	Extension(RD *rd, SerializedED *SED, createObjectInfo *COB);	//defined & documented in Extension.cpp
	~Extension();													//defined & documented in Extension.cpp


	/* Add any data you want to store in your
	 * extension to this class (eg. what you'd
	 * normally store in the rundata). Unlike
	 * other SDKs, you can store real C++
	 * objects with constructors and destructors,
	 * without having to call them manually or
	 * store a pointer.
	 */

	ImageBank  *surf;
	cSurface   *target;
	short       targetId;
	bool        targetValid;
	cSurface   *current;
	short       currentId
	,           lastId;
	//Functions
	cSurface *(*imageAt)(RD *rd, int image); //external interface
	int       (*imageCount)(RD *rd);         //external interface
	bool        CreateFillData(stdtstring name, CFillData *fill, int type, int paran);
	void        FreeColmask();
	void        RectUpdate(int x1, int y1, int x2, int y2);
	void        Blit(cSurface *source, cSurface *dest);
	int         ProcessFillFlags(stdtstring fill, stdtstring out, bool &useFill, bool &useOut, FillGuide *&fillData, FillGuide *&outData);
	void        UpdateHotspot();
//	void        ParseCondition(TCHAR const *cond, ColorCond *conds, int n);
//	bool        ConditionMet(ColorCOnd *conds, int n, COLORREF color);
	//Display surface for rotations and scaling
	cSurface   *display;
	//The default image depth
	int         depth;
	//Internal blit
	BlitMode    bM;
	BlitOp      bOp;
	LPARAM      bParam;
	DWORD       bFlags;
	int         bdX
	,           bdY
	,           bdW
	,           bdH
	,           bStretch
	,           bsX
	,           bsY
	,           bsW
	,           bsH;
	bool        bsRegion;
	int         bAngle;
	bool        bAngleResample;
	char const *bCallback
	,          *bProcOp;
	float       bAlphaCompose;
	int         bhX
	,           bhY
	,           bhMode;
	//Callback
	COLORREF    colSrc
	,           colDest
	,           colNew;
	int         colAlphaSrc
	,           colAlphaDest
	,           colAlphaNew;
	bool        colRet
	,           colAlphaRet;
	char const *callback;
	int         callX
	,           callY;
	//Data
	sMask      *collision;
	PolyPoints *points;
	FillDB     *fill;
	//Font
	HFONT       hFont;
	DWORD       hFlags;
	LOGFONT     textFont;
	//Options
	bool        useAbs
	,           keepPoints
	,           threadedIO
	,           dispTarget
	,           selectLast;
	//Settings of non-existant surfaces
	COLORREF    transCol;
	//Buffer lock
	BYTE       *buffer;
	cSurface   *buffSurf;
	int         buffPitch;
	//Overlay compatibility
	Overlay    *ovl;
	//Flood fill
	int         floodL
	,           floodR
	,           floodT
	,           floodB;
	//I/O stuff
	char const *ioFile;
	DWORD       ioFilter;
	HANDLE      ioHandle;


	/* Add your actions, conditions, and expressions
	 * as real class member functions here. The arguments
	 * (and return types for expressions/conditions)
	 * must match EXACTLY what you defined in the JSON.
	 * As a handy tip, all parameters will be either
	 * a pointer, an int, or a float (each only 4 bytes)
	 * and the same applies to return types. Remember to
	 * link the actions, conditions and expressions to their
	 * numeric IDs in the class constructor in Extension.cpp.
	 */

	//Actions - Defined in Actions.cpp
	void ActionExample(int ExampleParameter);
	void SecondActionExample();

	//Conditions - Defined in Conditions.cpp
	bool AreTwoNumbersEqual(int FirstNumber, int SecondNumber);

	//Expressions - Defined in Expressions.cpp
	int Add(int FirstNumber, int SecondNumber);
	const TCHAR * HelloWorld();


	short Handle();			//defined & documented in Extension.cpp
	short Display();		//defined & documented in Extension.cpp

	short Pause();			//defined & documented in Extension.cpp
	short Continue();		//defined & documented in Extension.cpp

	bool Save(HANDLE File);	//defined & documented in Extension.cpp
	bool Load(HANDLE File);	//defined & documented in Extension.cpp

	//defined & documented in Extension.cpp
	void Action(int ID, RD *rd, long param1, long param2);
	long Condition(int ID, RD *rd, long param1, long param2);
	long Expression(int ID, RD *rd, long param);
};
