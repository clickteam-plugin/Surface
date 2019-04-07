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
#ifdef UNICODE
	printf("Surface Unicode version\n");
#endif
#endif

	srand(time(NULL));

	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	mv* mV = rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv;

	rdPtr->rHo.hoImgWidth = edPtr->width_def;
	rdPtr->rHo.hoImgHeight = edPtr->height_def;
	rdPtr->useAbs = edPtr->useAbs;
	rdPtr->keepPoints = edPtr->keepPoints;
	rdPtr->dispTarget = edPtr->dispTarget;
	rdPtr->selectLast = edPtr->selectLast;
	rdPtr->threadedIO = edPtr->threadedIO;
	rdPtr->ioFile = 0;

	//Font
	rdPtr->hFont = 0;
	memcpy(&rdPtr->textFont,&edPtr->textFont,sizeof(LOGFONT));
	rdPtr->hFlags = edPtr->textFlags;	

	//Depth
	rdPtr->depth = 24;

	//Internal blending
	rdPtr->b.mode = BMODE_TRANSP;
	rdPtr->b.operation = BOP_COPY;
	rdPtr->b.param = 0;
	rdPtr->b.param = 0xffffffff;
	rdPtr->b.flags = 0;
	rdPtr->b.destX = 0;
	rdPtr->b.destY = 0;
	rdPtr->b.destW = 0;
	rdPtr->b.destH = 0;
	rdPtr->b.StretchMode = 0;
	rdPtr->b.srcX = 0;
	rdPtr->b.srcY = 0;
	rdPtr->b.srcW = 0;
	rdPtr->b.srcH = 0;
	rdPtr->b.srcUse = false;
	rdPtr->b.callback[0] = 0;
	rdPtr->b.procOp[0] = 0;
	rdPtr->b.procOpSrc = 1;
	rdPtr->b.composeAlpha = false;
	rdPtr->b.angle = 0;
	rdPtr->b.angleResample = false;
	rdPtr->b.hotX = 0;
	rdPtr->b.hotY = 0;
	rdPtr->b.hotspotMode = 1; // no longer possible to disable
	rdPtr->blitStackCursor = -1;
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
	//Running HWA?
	LPSURFACE wSurf = WinGetSurface((int)rhPtr->rhIdEditWin);
	int nDrv = wSurf->GetDriver();
	rdPtr->isHWA = nDrv >= SD_3DFX;
		
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
	{
		WaitForSingleObject(rdPtr->ioHandle,INFINITE);
		CloseHandle(rdPtr->ioHandle);
	}
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
	if (rdPtr->ioHandle)
	{
		DWORD code;
		if (GetExitCodeThread(rdPtr->ioHandle, &code) && code != STILL_ACTIVE)
		{
			CloseHandle(rdPtr->ioHandle);
			rdPtr->ioHandle = NULL;
		}
	}

	if (rdPtr->rc.rcChanged)
		return REFLAG_DISPLAY;

	//rdPtr->rHo.hoImgWidth = CurrentImg->GetWidth()*abs(rdPtr->rc.rcScaleX);
	//rdPtr->rHo.hoImgHeight = CurrentImg->GetHeight()*abs(rdPtr->rc.rcScaleY);

	return 0;
}


//cSurface* WINAPI DLLExport GetRunObjectSurface(LPRDATA rdPtr)
short WINAPI DLLExport DisplayRunObject(LPRDATA rdPtr)
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

	// Begin render process...
	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	LPSURFACE ps = WinGetSurface((int)rhPtr->rhIdEditWin);
	cSurface* renderImage = CurrentImg;

	// On-screen coords
	int screenX = rdPtr->rHo.hoX - rhPtr->rhWindowX;
	int screenY = rdPtr->rHo.hoY - rhPtr->rhWindowY;

	// Pure HWA blitting, yay!
#ifdef HWABETA

	if (rdPtr->isHWA)// && CurrentImg->GetType() >= ST_HWA_RTTEXTURE)
	{
		DWORD flags = 0;

		// Rotate quality 1
		if (rdPtr->rc.rcAngle && rdPtr->rs.rsFlags & RSFLAG_ROTATE_ANTIA)
			flags |= BLTF_ANTIA;

		// Scale quality 1
		if ((rdPtr->rc.rcScaleX != 1.0f || rdPtr->rc.rcScaleY != 1.0f) && rdPtr->rs.rsFlags & RSFLAG_SCALE_RESAMPLE)
			flags |= BLTF_ANTIA;

		// Hot spot (transform center)
		POINT point = {ImageS(rdPtr->currentId)->hotX, ImageS(rdPtr->currentId)->hotY};

		CurrentImg->BlitEx(*ps, screenX, screenY, 
			rdPtr->rc.rcScaleX, rdPtr->rc.rcScaleY, 0, 0,
			CurrentImg->GetWidth(), CurrentImg->GetHeight(), &point, rdPtr->rc.rcAngle,
			(rdPtr->rs.rsEffect & EFFECTFLAG_TRANSPARENT) ? BMODE_TRANSP : BMODE_OPAQUE,
			BlitOp(rdPtr->rs.rsEffect & EFFECT_MASK),
			rdPtr->rs.rsEffectParam, flags);
		return 0;
	}

#endif

	// Software blit: Offset draw position by transformed hotspot
	screenX -= rdPtr->rHo.hoImgXSpot;
	screenY -= rdPtr->rHo.hoImgYSpot;

	// Temporary surface for transformations
	cSurface temp;

	//Need scaling
	if(rdPtr->rc.rcScaleX != 1.0 || rdPtr->rc.rcScaleY != 1.0)
	{
		temp.Create(scaleW, scaleH, renderImage);
		renderImage->Stretch(temp, 0, 0, scaleW, scaleH, BMODE_OPAQUE, BOP_COPY, 0, 
			((rdPtr->rs.rsFlags&RSFLAG_SCALE_RESAMPLE)?STRF_RESAMPLE:0)|STRF_COPYALPHA);
	
		//Mirror for negative values.
		if(rdPtr->rc.rcScaleX < 0)
			temp.ReverseX();
		if(rdPtr->rc.rcScaleY < 0)
			temp.ReverseY();

		// For now, the scaled image is the 'final' one
		renderImage = &temp;
	}

	//Need rotation
	if(rdPtr->rc.rcAngle)
	{
		cSurface rotateBuffer;
		rotateBuffer.Clone(*renderImage);
		rotateBuffer.CreateRotatedSurface(temp, rdPtr->rc.rcAngle, 
			rdPtr->rs.rsFlags & RSFLAG_ROTATE_ANTIA, CurrentImg->GetTransparentColor(), TRUE);
		renderImage = &temp;
	}

	if (renderImage && renderImage->IsValid())
	{
		rdPtr->rHo.hoImgWidth = renderImage->GetWidth();
		rdPtr->rHo.hoImgHeight = renderImage->GetHeight();

		renderImage->Blit(
			*ps,
			screenX,
			screenY,
			0, 0,
			renderImage->GetWidth(),
			renderImage->GetHeight(),
			(rdPtr->rs.rsEffect & EFFECTFLAG_TRANSPARENT) ? BMODE_TRANSP : BMODE_OPAQUE,
			BlitOp(rdPtr->rs.rsEffect & EFFECT_MASK),
			rdPtr->rs.rsEffectParam
		);

		//Update window
		RECT rect;
		rect.left = screenX;
		rect.top = screenY;
		rect.right = screenX + renderImage->GetWidth();
		rect.bottom = screenY + renderImage->GetHeight();
		//ps->Rectangle(rect.left, rect.top, rect.right, rect.bottom, RED, 0, 0, TRUE);
		WinAddZone(rhPtr->rhIdEditWin, &rect);
	}

	return 0;
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
	// Opaque? collide with box
	if ((rdPtr->rs.rsEffect & EFFECTFLAG_TRANSPARENT) == 0)
		return 0;

	// Transparent? Create mask
	LPSMASK pMask = rdPtr->collision;
	if(!pMask)
	{
		if(rdPtr->currentId >=0 && CurrentImg)
		{	
			cSurface* Display = CurrentImg;
			if(rdPtr->display)
				Display = rdPtr->display;

			DWORD dwMaskSize = Display->CreateMask(NULL, lParam);
			if(dwMaskSize)
			{
				pMask = (LPSMASK)calloc(dwMaskSize,1);
				if (pMask)
				{
					Display->CreateMask(pMask, lParam);
					pMask->mkXSpot = rdPtr->rHo.hoImgXSpot;
					pMask->mkYSpot = rdPtr->rHo.hoImgYSpot;
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