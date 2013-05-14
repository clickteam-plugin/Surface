/* Edittime.cpp
 * This file contains functions for the
 * New Object dialog and the Frame Editor.
 * For properties, see Properties.cpp. For
 * custom parameters, see CustomParams.cpp.
 * For text properties, see TextProps.cpp.
 * Functions defined here:
 * MakeIconEx
 * UsesFile
 * CreateFromFile
 * CreateObject
 * EditObject
 * SetEditSize
 * PutObject
 * RemoveObject
 * CloneObject
 * GetObjectRect
 * EditorDisplay
 * IsTransparent
 * PrepareToWriteObject
 * GetFilters
 */

#include "Common.h"

/* CreateObject
 * This is the first time you have
 * access to the editdata, so it
 * needs to be initialized with
 * default values. Just be aware
 * that if the user chooses to
 * create your object from a file,
 * CreateFromFile will be called
 * instead of this function.
 */
int MMF2Func CreateObject(mv *mV, LO *lo, SerializedED *SED)
{
#ifndef RUN_ONLY
	if(IS_COMPATIBLE(mV)) //Make sure MMF2 hasn't changed in a way that makes your extension incompatible
	{
		Edif::Init(mV, SED);

		void MMF2Func CreateFromFile(mv *mV, LPTSTR FileName, SerializedED *SED);
		CreateFromFile(mV, 0, SED);

		return 0;
	}
#endif
	return -1;
}

/* MakeIconEx
 * Lets you draw the icon dynamically
 * by drawing into the Icon surface. The
 * current example just uses the
 * Icon.png from the resources.
 */
int MMF2Func MakeIconEx(mv *mV, cSurface *Icon, LPTSTR Name, OI *oi, SerializedED *SED)
{
#ifndef RUN_ONLY
	EditData ed (SED);
	int error = -1;
	//Load icon
	if((ed.multiImg && !ed.loadFirst) || !ed.images[0])
	{
		Icon->Delete();
		Icon->Clone(*SDK->Icon);
		Icon->SetTransparentColor(0xff00ff);
	}
	else //Draw image
	{
		cSurface is;
		LockImageSurface(mV->mvIdAppli, ed.images[0], is);
		is.Minimize();
		//Draw grid
		for(int x = 0; x < 7; ++x)
		{
			for(int y = 0; y < 7; ++y)
			{
				Icon->Rectangle(x*4+2,y*4+2, x*4+6,y*4+6, (x+y)%2?LIGHT_GRAY:WHITE,0,0,1);
			}
		}

		int sw = is.GetWidth(), sh = is.GetHeight();
		int dw, dh;

		//Square
		if(sw == sh)
		{
			dw = min(28, sw);
			dh = dw;
		}
		//Wide
		else if(sw > sh)
		{
			dw = min(28, sw);
			dh = sh*1.0 / sw*dw;
		}
		//Tall
		else
		{
			dh = min(28, sh);
			dw = sw*1.0 / sh*dh;
		}
		//Draw image
		if(!is.Stretch(*Icon, 16-dw/2,16-dh/2, dw,dh, BMODE_TRANSP, BOP_COPY, 0, STRF_RESAMPLE_TRANSP))
		{
			error = 0;
		}
		//Get transparent color
		COLORREF trans = is.GetTransparentColor();
		Icon->SetTransparentColor(trans);
		UnlockImageSurface(is);
		//Outline
		Icon->Rectangle(0,0, 32,32, 1, 0x333333);
		CFillFlat white (WHITE);
		Icon->Rectangle(1,1, 31,31, 1, &white, 0, BMODE_OPAQUE, BOP_BLEND, 32);
		//Smooth border
		Icon->SetPixel(0,0, trans);
		Icon->SetPixel(31,31, trans);
		Icon->SetPixel(31,0, trans);
		Icon->SetPixel(0,31, trans);
	}
	return error;
#endif
	return -1;
}

/* UsesFile
 * When the user chooses "Create From
 * File", MMF2 asks each extension if
 * it supports being created from that
 * file. Here you should investigate
 * the file and see if your extension
 * can be created from it in the
 * CreateFromFile function below. You
 * can simply check the file extension,
 * for example.
 */
BOOL MMF2Func UsesFile(mv *mV, LPTSTR FileName)
{
#ifndef RUN_ONLY
	if(IS_COMPATIBLE(mV)) //check for compatibility, since you can't return an error from CreateFromFile
	{
		return FindFilter(mV->mvImgFilterMgr, FileName) > 0;
	}
#endif
	return FALSE;
}

/* CreateFromFile
 * If the user decides to create your object
 * from the file you said you were OK with
 * above, this is where you take that file
 * and actually create your object from it.
 * The CreateObject function up above will
 * not have executed, so you need to initialize
 * the editdata just as you would in CreateObject.
 */
void MMF2Func CreateFromFile(mv *mV, LPTSTR FileName, SerializedED *SED)
{
#ifndef RUN_ONLY
	Edif::Init(mV, SED);

	EditData ed; //default EditData
	if(FileName) //null if called by other functions
	{
		//Load image from file
		cSurface surf;
		DWORD filter = FindFilter(mV->mvImgFilterMgr, FileName);
		BOOL import = ImportImage(mV->mvImgFilterMgr, FileName, &surf, &filter, 0);
		//Success! Woo!
		if(import)
		{
			ed.width = ed.width_def = surf.GetWidth();
			ed.height = ed.height_def = surf.GetHeight();
			ed.images[0] = CreateImageFromSurface(mV, &surf, ed.width,ed.height, 0,0, 0,0);
			ed.imageCount = 1;
		}
	}
	ed.Serialize(mV, SED);
#endif
}

/* PutObject
 * Each time a duplicate is made of your
 * object, MMF2 calls this function to
 * let you know. You still share the same
 * editdata, the difference is the
 * LevelObject pointer and the number of
 * duplicates.
 */
void MMF2Func PutObject(mv *mV, LO *lo, SerializedED *SED, ushort NDup)
{
#ifndef RUN_ONLY
	if(NDup == 1) //is this the first object being created?
	{
		//You can put common code to both
		//CreateObject and CreateFromFile
		//as this function will execute
		//after either one.
	}
#endif
}

/* RemoveObject
 * Just the opposite of the above, when
 * the user removes an instance of your
 * object. This also functions as a
 * DestroyObject function of sorts when
 * NDup == 1.
 */
void MMF2Func RemoveObject(mv *mV, LO *lo, SerializedED *SED, ushort NDup)
{
#ifndef RUN_ONLY
	if(NDup == 1) //is this the last object being removed?
	{
		Edif::Free(SED);
		//
	}
#endif
}

/* CloneObject
 * When the user makes a clone of your object
 * (not just another instance), MMF2 copies the
 * contents of the editdata and then lets you
 * know that the editdata in question is in
 * new hands. Here you can 'fix' anything that
 * might be caused by directly copying the
 * editdata, though you should never have
 * any problems like that in the first place.
 */
void MMF2Func CloneObject(mv *mV, OI *oi, SerializedED *SED)
{
#ifndef RUN_ONLY
	//
#endif
}

#include "PropIDs.hpp"

/* EditObject
 * If the user double-clicks your object icon
 * or chooses the Edit option from a context
 * menu, this function is called. This is useful
 * as a que to bring up an animation editor for
 * animations in your object. Return TRUE if
 * changes were made to the editdata, and FALSE
 * otherwise.
 */
BOOL MMF2Func EditObject (mv *mV, OI *oi, LO *lo, SerializedED *SED)
{
#ifndef RUN_ONLY
	BOOL MMF2Func EditProp(mv *mV, SerializedED *SED, UINT PropID);
	return EditProp(mV, SED, Prop::Images);
#endif
	return FALSE;
}

/* SetEditSize
 * If your object is resizeable, MMF2 calls this
 * to let you know that the user has just requested
 * a resize of the object. You can take the new
 * size as is, or if it reminds you too much of
 * 500 page essays you can limit the size to whatever
 * you want. If you uncomment this function, make
 * sure you also uncomment it in Ext.def. The
 * presence of this function tells MMF2 that your
 * object can be resized; otherwise it can't.
 */
BOOL MMF2Func SetEditSize(mv *mV, SerializedED *SED, int x, int y)
{
#ifndef RUN_ONLY
	EditData ed (SED);
	//Single, empty image
	if(ed.loadFirst && !ed.images[0]) //&& !ed.multiImg
	{
		ed.width = ed.width_def = cx;
		ed.height = ed.height_def = cy;
	}
	//Multiple images or single image
	else if(ed.images[0] && (ed.loadFirst || !ed.multiImg))
	{
		mvInvalidateObject(mV, SED);
		cSurface is, tmp;
		LockImageSurface(mv->mvIdAppli, ed.images[0], is);
		tmp.Clone(is, cx,cy);
		is.Delete();
		is.Clone(tmp);
		Img old;
		GetImageInfos(mv->mvIdAppli, ed.images[0], &old);
		ed.images[0] = CreateImageFromSurface(mv, &is, cx,cy, old.imgXSpot*1.0f*cx/old.imgWidth,old.imgYSpot*1.0f*cy/old.imgHeight, 0,0);
		UnlockImageSurface(is);
		ed.width = ed.width_def = cx;
		ed.height = ed.height_def = cy;
	}
	ed.Serialize(mV, SED);
	return TRUE;
#endif
	return FALSE;
}

/* GetObjectRect
 * MMF2 wants to know from time to time how much
 * space your object is taking up on the frame
 * editor. Currently this just gives the size of
 * your Icon.png, but you can change it to parallel
 * with the SetEditSize function above.
 */
void MMF2Func GetObjectRect(mv *mV, RECT *rect, LO *lo, SerializedED *SED)
{
#ifndef RUN_ONLY
	EditData ed (SED);
	int width = 32;
	int height = 32;

	int hotX = 0;
	int hotY = 0;

	//Display first image or don't use multiple images
	if(ed.loadFirst || !ed.multiImg)
	{
		//First image is valid
		if(ed.images[0])
		{
			width = ed.width_def;
			height = ed.height_def;
			//Get hotspot
			Img imgInfo;
			GetImageInfos(mV->mvIdAppli, ed.images[0], &imgInfo);
			hotX = imgInfo.imgXSpot;
			hotY = imgInfo.imgYSpot;
		}
		//No images in the bank
		else if(!ed.images[0] && ed.loadFirst) //!ed.multiImg)
		{
			width = ed.width;
			height = ed.height;
		}
	}

	//Apply hotspot
	rect->left -= hotX;
	rect->top -= hotY;

	//Apply size
	if(width == 0) width = DEF_WIDTH;
	if(height == 0) height = DEF_HEIGHT;
	rect->right = rect->left + width;
	rect->bottom = rect->top + height;
#endif
}


/* EditorDisplay
 * This function does the work of drawing your
 * object on the frame editor. In this example,
 * the Icon.png file is simply drawn to the
 * frame, but you can change this to draw
 * anything you want.
 */
void MMF2Func EditorDisplay(mv *mV, OI *oi, LO *lo, SerializedED *SED, RECT *rect)
{
#ifndef RUN_ONLY
	EditData ed (SED);
	LPSURFACE ps = WinGetSurface((int)mV->mvIdEditWin);
	if(!ps) return;
	cSurface is;
	//Draw extension icon
	if((ed.multiImg || !ed.images[0]) && (!ed.loadFirst || !ed.images[0]))
	{
		is.Clone(SDK->Icon);
		is.SetTransparentColor(0xff00ff);
		//Also draw surface borders
		if(ed.loadFirst && !ed.images[0])
		{
			//Create tile
			cSurface tile;
			tile.Create(16,16, ps);
			tile.Fill(LIGHT_GRAY);
			tile.Rectangle(0,0, 8,8, DARK_GRAY, 0,0, TRUE);
			tile.Rectangle(16,16, 8,8, DARK_GRAY, 0,0, TRUE);
			CFillMosaic mosaic (&tile);
			CFillFlat outline (BLACK);
			//Draw shapes
			ps->Rectangle(rect->left,rect->top, rect->right,rect->bottom, &mosaic, 1, &outline, 0, BMODE_TRANSP, BOP_BLEND, 32);
			//Icon
			if(rect->right - rect->left >= 32 && rect->bottom - rect->top >= 32)
				is.Blit(*ps, (rc->right+rc->left)/2 - 16, (rc->bottom+rc->top)/2 - 16, BMODE_TRANSP);
		}
		//Icon only
		else
		{
			is.Blit(*ps, rect->left, rect->top, BMODE_TRANSP);
		}	
	}
	//Draw default image
	else
	{
		BlitMode blitMode = oiPtr->oiHdr.oiInkEffect & EFFECTFLAG_TRANSPARENT ? BMODE_TRANSP : BMODE_OPAQUE;
		LockImageSurface(mV->mvIdAppli, ed.images[0],is);
		is.Blit(*ps, rect->left, rect->top, blitMode, (BlitOp)(oiPtr->oiHdr.oiInkEffect & EFFECT_MASK), oiPtr->oiHdr.oiInkEffectParam);
		UnlockImageSurface(is);
	}
#endif
}


/* IsTransparent
 * MMF2 calls this to ask if the mouse pointer
 * is over a transparent part of your object.
 * Don't ask why this isn't called "IsOpaque",
 * just accept that it isn't and move on. If the
 * given coordinates are over an opaque part of
 * your object, return TRUE, otherwise return
 * FALSE. (Protip: MMF2 calls this function a
 * LOT. Don't put a MessageBox function in here
 * or any other kind of debug function.)
 */
extern "C" BOOL MMF2Func IsTransparent(mv *mV, LO *lo, SerializedED *SED, int x, int y)
{
#ifndef RUN_ONLY
	EditData ed (SED);
	if(ed.images[0] && ed.loadFirst)
	{
		BOOL out = FALSE;
		cSurface is;
		LockImageSurface(mV->mvIdAppli, ed.images[0], is);
		//Alpha channel
		if(is.HasAlpha())
		{
			out = is.getAlpgaSurface()->GetPixelFast(x,y) == 0;
		}
		//Transparent color
		else
		{
			out = is.GetTransparentColor() == is.GetPixelFast(x,y);
		}
		UnlockImageSurface(is);
		return out;
	}
#endif
	return FALSE;
}

/* PrepareToWriteObject (DEPRECATED)
 * Just before MMF2 writes the editdata to the MFA,
 * it calls this function to let you clean up
 * a temporary copy of the editdata. Because you
 * intelligently designed your EditData::Serialize
 * function to always save cleaned data anyway,
 * this function is useless.
 */
void MMF2Func PrepareToWriteObject(mv *, SerializedED *, OI *){}

/* GetFilters
 * When the MFA filters are set to Automatic, MMF2
 * asks your extension if it uses filters or not.
 * If you use any of either kind of filters, return
 * TRUE for that kind.
 */
BOOL MMF2Func GetFilters(mv *mV, SerializedED *SED, DWORD Flags, void *)
{
#ifndef RUN_ONLY
	//If your extension uses image filters
	if((Flags & GETFILTERS_IMAGES) != 0) return TRUE;

	//If your extension uses sound filters
//	if((Flags & GETFILTERS_SOUNDS) != 0) return TRUE;
#endif
	return FALSE;
}
