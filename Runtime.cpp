// ============================================================================
//
// This file contains routines that are handled during the Runtime
// 
// ============================================================================

// Common Include
#include	"common.h"

// --------------------
// GetRunObjectDataSize
// --------------------
// Returns the size of the runtime datazone of the object
// 
short WINAPI DLLExport GetRunObjectDataSize(fprh rhPtr, LPEDATA edPtr)
{
	return(sizeof(RUNDATA));
}

// ---------------
// CreateRunObject
// ---------------
// The routine where the object is actually created
// 

short WINAPI DLLExport CreateRunObject(LPRDATA rdPtr, LPEDATA edPtr, fpcob cobPtr)
{
	// Do some rSDK stuff
	#include "rCreateRunObject.h"

#ifdef _DEBUG
	AllocConsole();
	freopen("conin$","r",stdin);
	freopen("conout$","w",stdout);
	freopen("conout$","w",stderr);
#endif

	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	mv* mV = rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv;

	rdPtr->rHo.hoImgWidth = edPtr->width_def;
	rdPtr->rHo.hoImgHeight = edPtr->height_def;
	rdPtr->useAbs = edPtr->useAbs;
	rdPtr->keepPoints = edPtr->keepPoints;
	rdPtr->dispTarget = edPtr->dispTarget;
	rdPtr->selectLast = edPtr->selectLast;
	rdPtr->threadedIO = edPtr->threadedIO;

	//Font
	rdPtr->hFont = 0;
	memcpy(&rdPtr->textFont,&edPtr->textFont,sizeof(LOGFONT));
	rdPtr->hFlags = edPtr->textFlags;

	//Depth
	rdPtr->depth = 24;

	//Internal blending
	rdPtr->bM = BMODE_TRANSP;
	rdPtr->bOp = BOP_COPY;
	rdPtr->bParam = 0;
	rdPtr->bFlags = 0;
	rdPtr->bdX = 0;
	rdPtr->bdY = 0;
	rdPtr->bdW = 0;
	rdPtr->bdH = 0;
	rdPtr->bStretch = 0;
	rdPtr->bsX = 0;
	rdPtr->bsY = 0;
	rdPtr->bsW = 0;
	rdPtr->bsH = 0;
	rdPtr->bsRegion = false;
	rdPtr->bCallback = 0;
	rdPtr->bProcOp = 0;
	rdPtr->bProcOpSrc = 1;
	rdPtr->bAlphaCompose = false;
	rdPtr->bAngle = 0;
	rdPtr->bAngleResample = false;
	rdPtr->bhX = 0;
	rdPtr->bhY = 0;
	rdPtr->bhMode = 1; // no longer possible to disable
	//Current & target surfaces
	rdPtr->targetId = -1;
	rdPtr->target = 0;
	rdPtr->currentId = -1;
	rdPtr->current = 0;
	rdPtr->lastId = -1;
	rdPtr->targetValid = false;
	//Default settings
	rdPtr->transCol = BLACK;
	//Collision mask and fill data
	rdPtr->collision = 0;
	rdPtr->fill = new FillDB;
	//Polygon's points
	rdPtr->points = new PolyPoints;
	//Buffer
	rdPtr->buffer = 0;
	rdPtr->buffPitch = 0;
	rdPtr->buffSurf = 0;
	//Overlay
	rdPtr->ovl = 0;
	//Callback
	rdPtr->colSrc = 0;
	rdPtr->colDest = 0;
	rdPtr->colNew = 0;
	rdPtr->colAlphaSrc = 0;
	rdPtr->colAlphaDest = 0;
	rdPtr->colAlphaNew = 0;
	rdPtr->callback = 0;
	rdPtr->callX = 0;
	rdPtr->callY = 0;
	rdPtr->colAlphaRet = 0;
	rdPtr->colRet = 0;
	//Flood fill region
	rdPtr->floodL = 0;
	rdPtr->floodT = 0;
	rdPtr->floodR = 0;
	rdPtr->floodB = 0;
	//I/O
	rdPtr->ioHandle = 0;
	//Quick store
	rdPtr->stored = 0;
	// Random color function
	rdPtr->randomSeed = 0;
	//Functions for extension integration
	rdPtr->imageAt = GetImgAt;
	rdPtr->imageCount = GetImgCount;

	//Display surface not necessary by default
	rdPtr->display = 0;
	rdPtr->rc.rcAngle = 0;
	rdPtr->rc.rcScaleX = 1.0f;
	rdPtr->rc.rcScaleY = 1.0f;

	//Images
	rdPtr->surf = new ImageBank;

	//Hotspot info
	Img imgInfo;
	//Prototype for new surfaces
	cSurface *proto = 0;

	if(edPtr->multiImg)
	{
		//First image empty
		if(!edPtr->images[0]&&edPtr->loadFirst)
		{
			cSurface *tmp = new cSurface;
			rdPtr->surf->push_back(new SImage(tmp));
			//Empty image with size
			if(edPtr->loadFirst)
			{
				GetSurfacePrototype(&proto,rdPtr->depth,ST_MEMORYWITHDC,SD_DIB);
				tmp->Create(edPtr->width,edPtr->height,proto);
				tmp->Fill(BLACK);
			}
			rdPtr->targetId = rdPtr->currentId = rdPtr->lastId = 0;
			TargetImg = CurrentImg = tmp;
			rdPtr->targetValid = true;
		}
		for(int i=0;i<edPtr->imageCount;i++)
		{
			//Get surface prototype on first loop
			if(!i)
				GetSurfacePrototype(&proto,rdPtr->depth,ST_MEMORYWITHDC,SD_DIB);
			//Valid image? Load
			if(edPtr->images[i])
			{
				cSurface is, *tmp = new cSurface;
				GetImageInfos(rhPtr->rhIdAppli,edPtr->images[i],&imgInfo);
				rdPtr->surf->push_back(new SImage(tmp,true,imgInfo.imgXSpot,imgInfo.imgYSpot));
				LockImageSurface(rhPtr->rhIdAppli,edPtr->images[i],is);
				
				if(is.GetWidth())
				{
					if(!i)
					{
						rdPtr->targetId = 0;
						TargetImg = tmp;
						rdPtr->lastId = rdPtr->targetId;
						rdPtr->targetValid = true;
						if(edPtr->loadFirst)
						{
							rdPtr->currentId = rdPtr->targetId;
							CurrentImg = TargetImg;
						}
					}
					tmp->Create(is.GetWidth(),is.GetHeight(),proto);
					tmp->SetTransparentColor(is.GetTransparentColor());
					is.Blit(*tmp,0,0,BMODE_OPAQUE,BOP_COPY,0,BLTF_COPYALPHA);
				}
				else if(!i&&edPtr->loadFirst)
				{
					rdPtr->targetId = rdPtr->currentId = 0;
					TargetImg = CurrentImg = 0;
					rdPtr->lastId = rdPtr->targetId;
				}
				UnlockImageSurface(is);
			}
			//Add prototype if there's more than one empty image
			else if(i&&edPtr->imageCount>1)
			{
				rdPtr->surf->push_back(new SImage(new cSurface));
			}

		}
	}
	//Single image
	else
	{
		//First image is set
		if(edPtr->images[0])
		{
			cSurface is, *tmp = new cSurface;
			GetImageInfos(rhPtr->rhIdAppli,edPtr->images[0],&imgInfo);
			rdPtr->surf->push_back(new SImage(tmp,true,imgInfo.imgXSpot,imgInfo.imgYSpot));
			LockImageSurface(rhPtr->rhIdAppli,edPtr->images[0],is);

			if(is.GetWidth())
			{
				GetSurfacePrototype(&proto,rdPtr->depth,ST_MEMORYWITHDC,SD_DIB);
				rdPtr->targetId = rdPtr->currentId = rdPtr->lastId = 0;
				TargetImg = CurrentImg = tmp;
				tmp->Create(is.GetWidth(),is.GetHeight(),proto);
				tmp->SetTransparentColor(is.GetTransparentColor());
				is.Blit(*tmp);
			}
			UnlockImageSurface(is);
			rdPtr->targetValid = true;
		}
		//Create empty image if wanted
		else
		{
			cSurface *tmp = new cSurface;
			rdPtr->surf->push_back(new SImage(tmp));
			//Empty image with size
			if(edPtr->loadFirst)
			{
				GetSurfacePrototype(&proto,rdPtr->depth,ST_MEMORYWITHDC,SD_DIB);
				tmp->Create(edPtr->width,edPtr->height,proto);
				tmp->Fill(BLACK);
			}
			rdPtr->targetId = rdPtr->currentId = rdPtr->lastId = 0;
			TargetImg = CurrentImg = tmp;
			rdPtr->targetValid = TargetImg->IsValid();
		}
	}
	
	//Initialize information
	if(rdPtr->current)
	{	
		rdPtr->rHo.hoImgWidth = rdPtr->current->GetWidth();
		rdPtr->rHo.hoImgHeight = rdPtr->current->GetHeight();
		rdPtr->rHo.hoImgXSpot = ImageS(rdPtr->currentId)->hotX;
		rdPtr->rHo.hoImgYSpot = ImageS(rdPtr->currentId)->hotY;
	}


	// No errors
	return 0;
}

// ----------------
// DestroyRunObject
// ----------------
// Destroys the run-time object
// 
short WINAPI DLLExport DestroyRunObject(LPRDATA rdPtr, long fast)
{
	//Wait unitl thread is done
	if(rdPtr->ioHandle)
		WaitForSingleObject(rdPtr->ioHandle,INFINITE);
	//Collision mask
	MaskChanged();
	//Fill data
	if(rdPtr->fill)
	{
		FillDB::iterator it = rdPtr->fill->begin();
		for(;it!=rdPtr->fill->end();it++)
		{
			delete it->second->fill;
			delete it->second;
		}
		delete rdPtr->fill;
	}
	//Blit callback
	if(rdPtr->bCallback)
		free(rdPtr->bCallback);
	//Blit proc operation
	if(rdPtr->bProcOp)
		free(rdPtr->bProcOp);
	//Polygon points
	delete rdPtr->points;
	//Locked buffer
	if(rdPtr->buffer&&rdPtr->buffPitch>0)
	{
		rdPtr->buffSurf->UnlockBuffer(rdPtr->buffer);
	}
	//Surfaces
	for(int i=0;i<rdPtr->surf->size();i++)
		delete rdPtr->surf->at(i);
	delete rdPtr->surf;
	//Font
	if(rdPtr->hFont)
		DeleteObject(rdPtr->hFont);
	//Binary overlay
	if(rdPtr->ovl)
		delete rdPtr->ovl;
	//Display image for rotated stuff
	if(rdPtr->display)
		delete rdPtr->display;

	delete rdPtr->rRd;
	return 0;
}


// ----------------
// HandleRunObject
// ----------------
// Called (if you want) each loop, this routine makes the object live
// 
short WINAPI DLLExport HandleRunObject(LPRDATA rdPtr)
{
	srand(time(NULL));

	if(rdPtr->rc.rcChanged)
		return REFLAG_DISPLAY;

	return 0;
}


cSurface* WINAPI DLLExport GetRunObjectSurface(LPRDATA rdPtr)
{
	MaskChanged();

	if(rdPtr->dispTarget)
	{
		rdPtr->currentId = rdPtr->targetId;
		CurrentImg = TargetImg;
	}

	if(!CurrentImg)
	{
		rdPtr->rHo.hoImgWidth = 0;
		rdPtr->rHo.hoImgHeight = 0;
		return 0;
	}

	//Get scaled size of the surface
	int scaleW = CurrentImg->GetWidth()*abs(rdPtr->rc.rcScaleX);
	int scaleH = CurrentImg->GetHeight()*abs(rdPtr->rc.rcScaleY);

	//Object is too small
	if(scaleW <= 0 || scaleH <= 0)
	{
		rdPtr->rHo.hoImgWidth = 0;
		rdPtr->rHo.hoImgHeight = 0;
		rdPtr->rHo.hoImgXSpot = 0;
		rdPtr->rHo.hoImgYSpot = 0;
		return 0;
	}

	//Update window
	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	WinAddZone(rhPtr->rhIdEditWin,&rdPtr->rHo.hoRect);

	//Transform
	if(rdPtr->rc.rcScaleX != 1.0 || rdPtr->rc.rcScaleY != 1.0 || rdPtr->rc.rcAngle)
	{
		//Set up display surface
		if(!rdPtr->display)
			rdPtr->display = new cSurface;
		if(rdPtr->display->IsValid())
			rdPtr->display->Delete();

		cSurface Temp;

//#ifndef HWABETA

		//Need scaling
		if(rdPtr->rc.rcScaleX != 1.0 || rdPtr->rc.rcScaleY != 1.0)
		{
			Temp.Create(scaleW,scaleH,CurrentImg);
			CurrentImg->Stretch(Temp,0,0,scaleW,scaleH,BMODE_OPAQUE,BOP_COPY,0,
				((rdPtr->rs.rsFlags&RSFLAG_SCALE_RESAMPLE)?STRF_RESAMPLE:0)|STRF_COPYALPHA);
		
			//Mirror for negative values.
			if(rdPtr->rc.rcScaleX < 0)
				Temp.ReverseX();
			if(rdPtr->rc.rcScaleY < 0)
				Temp.ReverseY();
		}

		cSurface* RotateSource = CurrentImg;
		if(Temp.IsValid())
			RotateSource = &Temp;

		//Need rotation
		if(rdPtr->rc.rcAngle)
			RotateSource->CreateRotatedSurface(*rdPtr->display, rdPtr->rc.rcAngle,
				rdPtr->rs.rsFlags&RSFLAG_ROTATE_ANTIA?TRUE:FALSE, CurrentImg->GetTransparentColor(), TRUE);
		//Clone the normal/scaled surface
		else
			rdPtr->display->Clone(*RotateSource);
//#else
//
//		//Need anything
//		if(rdPtr->rc.rcScaleX != 1.0 || rdPtr->rc.rcScaleY != 1.0 || rdPtr->rc.rcAngle)
//		{
//			/* Get rotated size */
//			int w = scaleW, h = scaleH;
//			cSurface::GetSizeOfRotatedRect(&w, &h, rdPtr->rc.rcAngle);
//
//			/* Create HWA display surface */
//			cSurface blitEx, *proto;
//			GetSurfacePrototype(&proto, 32, ST_MEMORY, SD_D3D9);
//			rdPtr->display->Create(w, h, proto);
//
//			int origWidth = CurrentImg->GetWidth(), origHeight = CurrentImg->GetHeight();
//			blitEx.Create(origWidth, origHeight, proto);
//			CurrentImg->Blit(blitEx, 0, 0, BMODE_OPAQUE, BOP_COPY, 0, BLTF_COPYALPHA);
//
//			//Apply transformations
//			POINT center = {origWidth/2, origHeight/2};
//			printf("blit ex %d\n", blitEx.BlitEx(*rdPtr->display, w/2, h/2, rdPtr->rc.rcScaleX, rdPtr->rc.rcScaleY, 0, 0, origWidth, origHeight, &center, rdPtr->rc.rcAngle));
//		}
//
//#endif
		//Remove temporary surface
		Temp.Delete();

		//Calculate the scaled and rotated size and hotspot
		UpdateHotspot(rdPtr);

		return rdPtr->display;
	}
	//Display surface unneccessary, but let's keep it in the memory, we might rotate again later
	else if(rdPtr->display)
		rdPtr->display->Delete();

	rdPtr->rHo.hoImgWidth = CurrentImg->GetWidth();
	rdPtr->rHo.hoImgHeight = CurrentImg->GetHeight();
	return CurrentImg;
}
//#endif


// -------------------------
// GetRunObjectCollisionMask
// -------------------------
// Implement this function if your extension supports fine collision mode (OEPREFS_FINECOLLISIONS),
// Or if it's a background object and you want Obstacle properties for this object.
//
// Should return NULL if the object is not transparent.
//
// Note: do not forget to enable the function in the .def file 
// if you remove the comments below.
//

sMask* WINAPI DLLExport GetRunObjectCollisionMask(LPRDATA rdPtr, LPARAM lParam)
{
	// Typical example for active objects
	// ----------------------------------
	// Opaque? collide with box
	if ((rdPtr->rs.rsEffect & EFFECTFLAG_TRANSPARENT) == 0)
		return 0;
	// Transparent? Create mask
	LPSMASK pMask = rdPtr->collision;
	if(!pMask)
	{
		if(rdPtr->currentId>=0&&CurrentImg)
		{	
			cSurface* Display = CurrentImg;
			if(rdPtr->display)
				Display = rdPtr->display;

			DWORD dwMaskSize = Display->CreateMask(NULL,lParam);
			if(dwMaskSize)
			{
				pMask = (LPSMASK)calloc(dwMaskSize,1);
				if (pMask)
				{
					Display->CreateMask(pMask, lParam);
					rdPtr->collision = pMask;
				}
			}
		}
	}
	return pMask;
}

// ----------------
// DisplayRunObject
// ----------------
// Draw the object in the application screen.
//
/*
#if 0//M_OEPREFS_QUICKDISPLAY
//Use MMF backsaving!
#ifndef CUSTOM_BACKSAVE
short WINAPI DLLExport DisplayRunObject(LPRDATA rdPtr)
{	
	MaskChanged();

	if(rdPtr->dispTarget)
	{
		rdPtr->currentId = rdPtr->targetId;
		CurrentImg = TargetImg;
	}

	if(rdPtr->currentId<0||!CurrentImg->IsValid())
		return 0;

	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	LPSURFACE ps = WinGetSurface((int)rhPtr->rhIdEditWin);
	WinAddZone(rhPtr->rhIdEditWin,&rdPtr->rHo.hoRect);
	if(!ps)
		return 0;
	
	CurrentImg->Blit(*ps,rdPtr->rHo.hoRect.left,rdPtr->rHo.hoRect.top,
		(rdPtr->rs.rsEffect & EFFECTFLAG_TRANSPARENT)?BMODE_TRANSP:BMODE_OPAQUE,(BlitOp)(rdPtr->rs.rsEffect&EFFECT_MASK),rdPtr->rs.rsEffectParam);
	// OK
	return 0;
}

//Use custom backsaving... broken
#else
short WINAPI DLLExport DisplayRunObject(LPRDATA rdPtr)
{	
	TargetExists();
	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	LPSURFACE ps = WinGetSurface((int)rhPtr->rhIdEditWin);

	if(ps)
	{
		//Really true, not "force false"
		if(rdPtr->useRect==TRUE)
		{
			RECT zone;
			memcpy(&zone,&rdPtr->rHo.hoRect,sizeof RECT);
			zone.left += rdPtr->rect.left;
			zone.top += rdPtr->rect.top;
			zone.right += rdPtr->rect.right;
			zone.bottom += rdPtr->rect.bottom;
			TargetImg->Blit(*ps,zone.left,zone.top,rdPtr->rect.left,rdPtr->rect.top,rdPtr->rect.right-rdPtr->rect.left,rdPtr->rect.bottom-rdPtr->rect.top,
				(rdPtr->rs.rsEffect & EFFECTFLAG_TRANSPARENT)?BMODE_TRANSP:BMODE_OPAQUE,(BlitOp)(rdPtr->rs.rsEffect&EFFECT_MASK),rdPtr->rs.rsEffectParam);
			WinAddZone(rhPtr->rhIdEditWin,&zone);
			rdPtr->useRect = FALSE;
		}
		//Update everything
		else
		{
			TargetImg->Blit(*ps,rdPtr->rHo.hoX,rdPtr->rHo.hoY,
				(rdPtr->rs.rsEffect & EFFECTFLAG_TRANSPARENT)?BMODE_TRANSP:BMODE_OPAQUE,(BlitOp)(rdPtr->rs.rsEffect&EFFECT_MASK),rdPtr->rs.rsEffectParam);
			WinAddZone(rhPtr->rhIdEditWin,&rdPtr->rHo.hoRect);
		}
	}
	// OK
	return 0;
}

short WINAPI DLLExport SaveBackground(LPRDATA rdPtr)
{
	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	LPSURFACE ps = WinGetSurface((int)rhPtr->rhIdEditWin);
	if(!rdPtr->background) rdPtr->background = new cSurface;
	//Surface dimensions have changed
	if(rdPtr->oldWidth!=rdPtr->rHo.hoImgWidth||rdPtr->oldHeight!=rdPtr->rHo.hoImgHeight)
	{
		//Delete old surface
		if(rdPtr->background&&rdPtr->background->IsValid())
			rdPtr->background->Delete();
		//Create
		cSurface* proto = 0;
		GetSurfacePrototype(&proto,24,ST_MEMORY,SD_DIB);
		rdPtr->background->Create(rdPtr->rHo.hoImgWidth,rdPtr->rHo.hoImgHeight,proto);
		//Update everything!
		rdPtr->useRect = FALSE;
	}
	//Blit if FALSE or FORCE_FALSE
	if(rdPtr->useRect!=TRUE)
		ps->Blit(*rdPtr->background,0,0,rdPtr->rHo.hoX,rdPtr->rHo.hoY,rdPtr->rHo.hoImgWidth,rdPtr->rHo.hoImgHeight,BMODE_OPAQUE);
	else
		ps->Blit(*rdPtr->background,rdPtr->rect.left,rdPtr->rect.top,rdPtr->rect.left,rdPtr->rect.top,
		rdPtr->rect.right-rdPtr->rect.left,rdPtr->rect.bottom-rdPtr->rect.top,BMODE_OPAQUE);
	return 0;
}

short WINAPI DLLExport RestoreBackground(LPRDATA rdPtr)
{
	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	LPSURFACE ps = WinGetSurface((int)rhPtr->rhIdEditWin);
	//Position has changed, need full redraw
	if(rdPtr->rHo.hoX!=rdPtr->oldX||rdPtr->rHo.hoY!=rdPtr->oldY)
		rdPtr->useRect = FALSE;
	//Restore everything
	if(rdPtr->useRect!=TRUE)
		rdPtr->background->Blit(*ps,rdPtr->oldX,rdPtr->oldY);
	//Restore zone, very similar to DisplayRunObject
	else
		rdPtr->background->Blit(*ps,rdPtr->oldX,rdPtr->oldY,
		(rdPtr->rs.rsEffect & EFFECTFLAG_TRANSPARENT)?BMODE_TRANSP:BMODE_OPAQUE,(BlitOp)(rdPtr->rs.rsEffect&EFFECT_MASK),rdPtr->rs.rsEffectParam);
	//Update values
	rdPtr->oldX = rdPtr->rHo.hoX;
	rdPtr->oldY = rdPtr->rHo.hoY;
	rdPtr->oldWidth = rdPtr->rHo.hoImgWidth;
	rdPtr->oldHeight = rdPtr->rHo.hoImgHeight;
	return 0;
}

short WINAPI DLLExport KillBackground(LPRDATA rdPtr)
{
	if(rdPtr->background)
	{
		rdPtr->background->Delete();
		delete rdPtr->background;
		rdPtr->background = 0;
	}
	return 0;
}
#endif


// -------------------
// GetRunObjectSurface
// -------------------
// Implement this function instead of DisplayRunObject if your extension
// supports ink effects and transitions. Note: you can support ink effects
// in DisplayRunObject too, but this is automatically done if you implement
// GetRunObjectSurface (MMF applies the ink effect to the transition).
//
// Note: do not forget to enable the function in the .def file 
// if you remove the comments below.

#else
*/

// ----------------
// PauseRunObject
// ----------------
// Enters the pause mode
// 
short WINAPI DLLExport PauseRunObject(LPRDATA rdPtr)
{
	// Ok
	return 0;
}


// -----------------
// ContinueRunObject
// -----------------
// Quits the pause mode
//
short WINAPI DLLExport ContinueRunObject(LPRDATA rdPtr)
{
	// Ok
	return 0;
}


// ============================================================================
//
// START APP / END APP / START FRAME / END FRAME routines
// 
// ============================================================================

// -------------------
// StartApp
// -------------------
// Called when the application starts or restarts.
// Useful for storing global data
// 
void WINAPI DLLExport StartApp(mv _far *mV, CRunApp* pApp)
{
	//ImageDB* pData = (ImageDB*)mV->mvGetExtUserData(pApp, hInstLib);
	//ImageDB::iterator it;
	////Global data was stored
	//if (pData)
	//{
	//	//Enumerate objects (not instances)
	//	for(it=pData->begin();it!=pData->end();it++)
	//	{
	//		//Delete all images
	//		for(int i=0;i<it->second->size();i++)
	//		{
	//			if(it->second->at(i)->IsValid())
	//				it->second->at(i)->Delete();
	//			//delete it->first; //no strdup
	//			delete it->second->at(i);
	//		}
	//		//Delete image bank
	//		delete it->second;
	//	}

	//	delete pData;
	//	mV->mvSetExtUserData(pApp, hInstLib, NULL);
	//}
}

// -------------------
// EndApp
// -------------------
// Called when the application ends.
// 
void WINAPI DLLExport EndApp(mv _far *mV, CRunApp* pApp)
{
	//StartApp(mV,pApp);
}

// -------------------
// StartFrame
// -------------------
// Called when the frame starts or restarts.
// 
void WINAPI DLLExport StartFrame(mv _far *mV, DWORD dwReserved, int nFrameIndex)
{

}

// -------------------
// EndFrame
// -------------------
// Called when the frame ends.
// 
void WINAPI DLLExport EndFrame(mv _far *mV, DWORD dwReserved, int nFrameIndex)
{

}

// ============================================================================
//
// TEXT ROUTINES (if OEFLAG_TEXT)
// 
// ============================================================================

/*
// -------------------
// GetRunObjectFont
// -------------------
// Return the font used by the object.
// 


  // Note: do not forget to enable the functions in the .def file 
  // if you remove the comments below.

void WINAPI GetRunObjectFont(LPRDATA rdPtr, LOGFONT* pLf)
{
	// Example
	// -------
	GetObject(rdPtr->hFont, sizeof(LOGFONT), pLf);
}

// -------------------
// SetRunObjectFont
// -------------------
// Change the font used by the object.
// 
void WINAPI SetRunObjectFont(LPRDATA rdPtr, LOGFONT* pLf, RECT* pRc)
{
	// Example
	// -------
	HFONT hFont = CreateFontIndirect(pLf);
	if ( hFont != NULL )
	{
		if (rdPtr->hFont!=0)
			DeleteObject(rdPtr->hFont);
		rdPtr->hFont = hFont;
	}

}

// ---------------------
// GetRunObjectTextColor
// ---------------------
// Return the text color of the object.
// 
COLORREF WINAPI GetRunObjectTextColor(LPRDATA rdPtr)
{
	// Example
	// -------
	return rdPtr->hColor;
}

// ---------------------
// SetRunObjectTextColor
// ---------------------
// Change the text color of the object.
// 
void WINAPI SetRunObjectTextColor(LPRDATA rdPtr, COLORREF rgb)
{
	// Example
	// -------
	rdPtr->hColor = rgb;
}*/

// ============================================================================
//
// DEBUGGER ROUTINES
// 
// ============================================================================

// -----------------
// GetDebugTree
// -----------------
// This routine returns the address of the debugger tree
//
LPWORD WINAPI DLLExport GetDebugTree(LPRDATA rdPtr)
{
#if !defined(RUN_ONLY)
	return DebugTree;
#else
	return NULL;
#endif // !defined(RUN_ONLY)
}

// -----------------
// GetDebugItem
// -----------------
// This routine returns the text of a given item.
//
void WINAPI DLLExport GetDebugItem(LPSTR pBuffer, LPRDATA rdPtr, int id)
{
#if !defined(RUN_ONLY)

	// Example
	// -------
/*
	char temp[DB_BUFFERSIZE];

	switch (id)
	{
	case DB_CURRENTSTRING:
		LoadString(hInstLib, IDS_CURRENTSTRING, temp, DB_BUFFERSIZE);
		wsprintf(pBuffer, temp, rdPtr->text);
		break;
	case DB_CURRENTVALUE:
		LoadString(hInstLib, IDS_CURRENTVALUE, temp, DB_BUFFERSIZE);
		wsprintf(pBuffer, temp, rdPtr->value);
		break;
	case DB_CURRENTCHECK:
		LoadString(hInstLib, IDS_CURRENTCHECK, temp, DB_BUFFERSIZE);
		if (rdPtr->check)
			wsprintf(pBuffer, temp, "TRUE");
		else
			wsprintf(pBuffer, temp, "FALSE");
		break;
	case DB_CURRENTCOMBO:
		LoadString(hInstLib, IDS_CURRENTCOMBO, temp, DB_BUFFERSIZE);
		wsprintf(pBuffer, temp, rdPtr->combo);
		break;
	}
*/

#endif // !defined(RUN_ONLY)
}

// -----------------
// EditDebugItem
// -----------------
// This routine allows to edit editable items.
//
void WINAPI DLLExport EditDebugItem(LPRDATA rdPtr, int id)
{
#if !defined(RUN_ONLY)

	// Example
	// -------
/*
	switch (id)
	{
	case DB_CURRENTSTRING:
		{
			EditDebugInfo dbi;
			char buffer[256];

			dbi.pText=buffer;
			dbi.lText=TEXT_MAX;
			dbi.pTitle=NULL;

			strcpy(buffer, rdPtr->text);
			long ret=callRunTimeFunction(rdPtr, RFUNCTION_EDITTEXT, 0, (LPARAM)&dbi);
			if (ret)
				strcpy(rdPtr->text, dbi.pText);
		}
		break;
	case DB_CURRENTVALUE:
		{
			EditDebugInfo dbi;

			dbi.value=rdPtr->value;
			dbi.pTitle=NULL;

			long ret=callRunTimeFunction(rdPtr, RFUNCTION_EDITINT, 0, (LPARAM)&dbi);
			if (ret)
				rdPtr->value=dbi.value;
		}
		break;
	}
*/
#endif // !defined(RUN_ONLY)
}


//ImageDB* imgdb;
//if(rdPtr->global)
//{
//	//Get global data
//	imgdb = (ImageDB*)mV->mvGetExtUserData(rhPtr->rhApp,hInstLib);
//	//Not stored yet
//	if(!imgdb)
//	{
//		imgdb = new ImageDB;
//		mV->mvSetExtUserData(rhPtr->rhApp,hInstLib,(void*)imgdb);
//	}
//}

//Check if object was not added yet OR if we don't want global data
//if((rdPtr->global&&imgdb->find(string((char*)&rdPtr->rHo.hoOiList->oilName[0]))==imgdb->end())||(!rdPtr->global))
//{

//Add new image bank to the global data
//if(rdPtr->global)
//	imgdb->insert(pair<string,ImageBank*>(string((char*)&rdPtr->rHo.hoOiList->oilName[0]),rdPtr->surf));

//}
//We want global data and it already exists.
//else if(rdPtr->global)
//{
//	//Load the image from the bank
//	rdPtr->surf = imgdb->find(std::string((char*)&rdPtr->rHo.hoOiList->oilName[0]))->second;
//	if(edPtr->loadFirst||!edPtr->multiImg)
//	{
//		rdPtr->targetId = rdPtr->currentId = 0;
//		rdPtr->lastId = rdPtr->targetId;
//		TargetImg = CurrentImg = Image(0);
//	}
//}