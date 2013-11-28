// ============================================================================
//
// This file contains the actions, conditions and expressions your object uses
// 
// ============================================================================

#include "common.h"

#define PARAM_OPERATOR _T("Operator (+  -  *  /  **  %%  <  >  &  |  ^  <<  >>  =)")
#define PARAM_OPERATOR_OPTIONAL _T("Operator (Empty: None, +  -  *  /  **  %%  <  >  &  |  ^  <<  >>  =)")

// ============================================================================
//
// CONDITIONS
// 
// ============================================================================

CONDITION(
	/* ID */			0,
	/* Name */			_T("%o: Image has alpha channel"),
	/* Flags */			EVFLAGS_ALWAYS|EVFLAGS_NOTABLE,
	/* Params */		(0)
) {
	TargetExists();
	return TargetImg->HasAlpha();
}


CONDITION(
	/* ID */			1,
	/* Name */			_T("Image available in clipboard"),
	/* Flags */			EVFLAGS_ALWAYS|EVFLAGS_NOTABLE,
	/* Params */		(0)
) {
	return IsClipboardFormatAvailable(CF_DIB);
}

CONDITION(
	/* ID */			2,
	/* Name */			_T("%o: On loading failed"),
	/* Flags */			0,
	/* Params */		(0)
) {
	return true;
}

CONDITION(
	/* ID */			3,
	/* Name */			_T("%o: On saving failed"),
	/* Flags */			0,
	/* Params */		(0)
) {
	return true;
}

CONDITION(
	/* ID */			4,
	/* Name */			_T("%o: On loading succeeded"),
	/* Flags */			0,
	/* Params */		(0)
) {
	return true;
}

CONDITION(
	/* ID */			5,
	/* Name */			_T("%o: On saving succeeded"),
	/* Flags */			0,
	/* Params */		(0)
) {
	return true;
}

CONDITION(
	/* ID */			6,
	/* Name */			_T("%o: RGB at (%0,%1) = %2"),
	/* Flags */			EVFLAGS_ALWAYS|EVFLAGS_NOTABLE,
	/* Params */		(3,PARAM_NUMBER,_T("X"),PARAM_NUMBER,_T("Y"),PARAM_COLOUR,_T("Color"))
) {
	TargetExists();
	int x = GetXPos();
	int y = GetYPos();
	COLORREF compare, color = GetCol();
	TargetImg->GetPixel(x,y,compare);
	return (compare&0xffffff)==(color&0xffffff);
}

CONDITION(
	/* ID */			7,
	/* Name */			_T("%o: Select image %0"),
	/* Flags */			EVFLAGS_ALWAYS,
	/* Params */		(1,PARAM_NUMBER,_T("Image"))
) {
	ImageInRange(param1);
	TargetImg = Image(param1);
	rdPtr->targetId = param1;
	rdPtr->targetValid = TargetImg->IsValid();
	if Current(rdPtr->targetId)
		ImageChanged();
	
	return true;
}

//CONDITION(
//	/* ID */			8,
//	/* Name */			_T("%o: On fill callback %0"),
//	/* Flags */			0,
//	/* Params */		(1,PARAM_STRING,_T("Pattern"))
//) {
//	return !rdPtr->userTrigger&&!_tcscmp((char*)param1,rdPtr->userData);
//}

CONDITION(
	/* ID */			8,
	/* Name */			_T("%o: On callback %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_STRING,_T("Callback"))
) {
	return !_tcscmp((TCHAR*)param1,rdPtr->callback);
}

CONDITION(
	/* ID */			9,
	/* Name */			_T("%o: Displayed image %0"),
	/* Flags */			EVFLAGS_ALWAYS,
	/* Params */		(1,PARAM_COMPARISON,_T("Image"))
) {
	return rdPtr->currentId;
}

CONDITION(
	/* ID */			10,
	/* Name */			_T("%o: Selected image %0"),
	/* Flags */			EVFLAGS_ALWAYS,
	/* Params */		(1,PARAM_COMPARISON,_T("Image"))
) {
	return rdPtr->targetId;
}

CONDITION(
	/* ID */			11,
	/* Name */			_T("%o: Red at (%0,%1) %2"),
	/* Flags */			EVFLAGS_ALWAYS,
	/* Params */		(3,PARAM_NUMBER,_T("X"),PARAM_NUMBER,_T("Y"),PARAM_COMPARISON,_T("Red value"))
) {
	TargetExists();
	int x = GetXPos();
	int y = GetYPos();
	COLORREF compare;
	TargetImg->GetPixel(x,y,compare);
	return GetRValue(compare);
}

CONDITION(
	/* ID */			12,
	/* Name */			_T("%o: Green at (%0,%1) %2"),
	/* Flags */			EVFLAGS_ALWAYS,
	/* Params */		(3,PARAM_NUMBER,_T("X"),PARAM_NUMBER,_T("Y"),PARAM_COMPARISON,_T("Green value"))
) {
	TargetExists();
	int x = GetXPos();
	int y = GetYPos();
	COLORREF compare;
	TargetImg->GetPixel(x,y,compare);
	return GetGValue(compare);
}

CONDITION(
	/* ID */			13,
	/* Name */			_T("%o: Blue at (%0,%1) %2"),
	/* Flags */			EVFLAGS_ALWAYS,
	/* Params */		(3,PARAM_NUMBER,_T("X"),PARAM_NUMBER,_T("Y"),PARAM_COMPARISON,_T("Blue value"))
) {
	TargetExists();
	int x = GetXPos();
	int y = GetYPos();
	COLORREF compare;
	TargetImg->GetPixel(x,y,compare);
	return GetBValue(compare);
}

CONDITION(
	/* ID */			14,
	/* Name */			_T("%o: Pattern %0 exists"),
	/* Flags */			EVFLAGS_ALWAYS|EVFLAG2_NOTABLE,
	/* Params */		(1,PARAM_STRING,_T("Pattern name"))
) {
	string fill(GetStr());
	return rdPtr->fill->find(fill)!=rdPtr->fill->end();
}

CONDITION(
	/* ID */			15,
	/* Name */			_T("%o: Buffer is locked"),
	/* Flags */			EVFLAGS_ALWAYS|EVFLAG2_NOTABLE,
	/* Params */		(0)
) {
	return rdPtr->buffer&&rdPtr->buffPitch;
}

CONDITION(
	/* ID */			16,
	/* Name */			_T("%o: (%0,%1) is inside image"),
	/* Flags */			EVFLAGS_ALWAYS|EVFLAGS_NOTABLE,
	/* Params */		(2,PARAM_NUMBER,_T("X"),PARAM_NUMBER,_T("Y"))
) {
	int x = GetXPos();
	int y = GetYPos();
	TargetExists();
	return (x>0&&y>0&&x<TargetImg->GetWidth()&&y<TargetImg->GetHeight());
}

CONDITION(
	/* ID */			17,
	/* Name */			_T("%o: File I/O is in progress"),
	/* Flags */			EVFLAGS_ALWAYS|EVFLAGS_NOTABLE,
	/* Params */		(0)
) {
	return rdPtr->ioHandle != 0;
}

CONDITION(
	/* ID */			18,
	/* Name */			_T("%o: File is being saved"),
	/* Flags */			EVFLAGS_ALWAYS|EVFLAGS_NOTABLE,
	/* Params */		(0)
) {
	return rdPtr->ioHandle != 0 && rdPtr->ioSave;
}

CONDITION(
	/* ID */			19,
	/* Name */			_T("%o: File is being loaded"),
	/* Flags */			EVFLAGS_ALWAYS|EVFLAGS_NOTABLE,
	/* Params */		(0)
) {
	return rdPtr->ioHandle != 0 && !rdPtr->ioSave;
}
// ============================================================================
//
// ACTIONS
// 
// ============================================================================

ACTION(
	/* ID */			0,
	/* Name */			_T("Blit onto image %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Target"))
) {
	TargetExists();
	int target = GetInt();
	ImageExists(target);
	Blit(TargetImg,Image(target),rdPtr);
	if Current(target)
		ImageChanged();
	
	return 0;
}
ACTION(
	/* ID */			1,
	/* Name */			_T("Display image %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Image"))
) {
	if(param1<0||param1>=rdPtr->surf->size()||!Image(param1)->IsValid())
	{
		rdPtr->current = 0;
		rdPtr->currentId = -1;
	}
	else
	{
		rdPtr->current = Image(param1);
		rdPtr->currentId = param1;
		UpdateHotspot(rdPtr);
	}
	ImageChanged();
	
	return 0;
}
ACTION(
	/* ID */			2,
	/* Name */			_T("Set pixel at (%0,%1) to %2"),
	/* Flags */			0,
	/* Params */		(3,PARAM_NUMBER,_T("X"),PARAM_NUMBER,_T("Y"),PARAM_COLOUR,_T("Color"))
) {
	TargetExists();
	int x = GetXPos();
	int y = GetYPos();
	COLORREF col = GetCol();
	if(rdPtr->buffer)
		TargetImg->SetPixelFast(x,y,col);
	else
		TargetImg->SetPixel(x,y,col);
	if Current(rdPtr->targetId)
		RectChanged(x,y,x+1,y+1);
	
	return 0;
}
ACTION(
	/* ID */			3,
	/* Name */			_T("Clear with color %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_COLOUR,_T("Color"))
) {
	TargetExists();
	TargetImg->Fill((COLORREF)param1);
	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}
ACTION(
	/* ID */			4,
	/* Name */			_T("Create alpha channel, keep transparency = %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Keep transparency? (0: No, 1: Yes)"))
) {
	TargetExists();
	COLORREF transp = TargetImg->GetTransparentColor();
	if(!TargetImg->HasAlpha())
	{
		TargetImg->CreateAlpha();
		//Keep transparency
		if(param1)
		{
			cSurface* alpha = TargetImg->GetAlphaSurface();
			for(int x=0;x<TargetImg->GetWidth();x++)
				for(int y=0;y<TargetImg->GetHeight();y++)
					if(TargetImg->GetPixelFast(x,y)!=transp)
						alpha->SetPixel(x,y,(COLORREF)0xffffffff);
			 TargetImg->ReleaseAlphaSurface(alpha);
		}
		else
		{
			if Current(rdPtr->targetId)
				ImageChanged();	
			
		}
	}
	else if(param1)
	{
		cSurface* alpha = TargetImg->GetAlphaSurface();
		for(int x=0;x<TargetImg->GetWidth();x++)
			for(int y=0;y<TargetImg->GetHeight();y++)
				if(TargetImg->GetPixelFast(x,y)!=transp)
					alpha->SetPixel(x,y,255);
		TargetImg->ReleaseAlphaSurface(alpha);
	}
	return 0;
}
ACTION(
	/* ID */			5,
	/* Name */			_T("Set alpha at (%0,%1) to %2"),
	/* Flags */			0,
	/* Params */		(3,PARAM_NUMBER,_T("X"),PARAM_NUMBER,_T("Y"),PARAM_NUMBER,_T("Alpha (0-255)"))
) {
	TargetExists();
	int x = GetXPos();
	int y = GetYPos();
	int a = GetInt();
	ActionFunc4(rdPtr, 1, 0);
	if(TargetImg->HasAlpha())
	{
		cSurface* alpha = TargetImg->GetAlphaSurface();
		alpha->SetPixel(x,y,a);
		TargetImg->ReleaseAlphaSurface(alpha);
		if Current(rdPtr->targetId)
			RectChanged(x,y,x+1,y+1);
		
	}
	return 0;
}
ACTION(
	/* ID */			6,
	/* Name */			_T("Clear alpha with %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Alpha (0-255)"))
) {
	TargetExists();
	ActionFunc4(rdPtr, 1, 0);
	if(TargetImg->HasAlpha())
	{
		cSurface* alpha = TargetImg->GetAlphaSurface();
		alpha->Fill(param1);
		TargetImg->ReleaseAlphaSurface(alpha);
		if Current(rdPtr->targetId)
			ImageChanged();
		
	}
	return 0;
}
ACTION(
	/* ID */			7,
	/* Name */			_T("Draw rectangle (%0,%1,%2,%3) with value %4 on the alpha channel"),
	/* Flags */			0,
	/* Params */		(5,PARAM_NUMBER,_T("X1"),PARAM_NUMBER,_T("Y1"),PARAM_NUMBER,_T("X2"),PARAM_NUMBER,_T("Y2"),PARAM_NUMBER,_T("Alpha (0-255)"))
) {
	TargetExists();
	ActionFunc4(rdPtr, 1, 0);
	if(!TargetImg->HasAlpha()) return 0;
	int x1 = GetXPos();
	int y1 = GetYPos();
	int xt = GetXPos();
	int yt = GetYPos();
	int a = GetInt();

	int x2 = max(x1,xt);
	int y2 = max(y1,yt);
	x1 = min(x1,xt);
	y1 = min(y1,yt);

	cSurface* asurf = TargetImg->GetAlphaSurface();
	asurf->Fill(x1,y1,x2-x1,y2-y1,a);
	TargetImg->ReleaseAlphaSurface(asurf);

	if Current(rdPtr->targetId)
		RectChanged(x1,y1,x2,y2);
	
	return 0;
}
ACTION(
	/* ID */			8,
	/* Name */			_T("Draw ellipse (%0,%1,%2,%3) with color %4, outline thickness %5 outline color %6"),
	/* Flags */			0,
	/* Params */		(7,PARAM_NUMBER,_T("X1"),PARAM_NUMBER,_T("Y1"),PARAM_NUMBER,_T("X2"),PARAM_NUMBER,_T("Y2"),PARAM_COLOUR,_T("Fill color (-1: None)"),PARAM_NUMBER,_T("Outline thickness"),PARAM_COLOUR,_T("Outline color"))
) {
	TargetExists();
	int x1 = GetXPos();
	int y1 = GetYPos();
	int x2 = GetXPos();
	int y2 = GetYPos();
	COLORREF fill = GetCol();
	int thick = GetInt();
	COLORREF out = GetCol();
	TargetImg->Ellipse(x1,y1,x2,y2,fill,thick,out,fill!=-1);

	if Current(rdPtr->targetId)
		RectChanged(x1,y1,x2,y2);
	
	return 0;
}
ACTION(
	/* ID */			9,
	/* Name */			_T("Draw rectangle (%0,%1,%2,%3) with color %4, outline thickness %5 outline color %6"),
	/* Flags */			0,
	/* Params */		(7,PARAM_NUMBER,_T("X1"),PARAM_NUMBER,_T("Y1"),PARAM_NUMBER,_T("X2"),PARAM_NUMBER,_T("Y2"),PARAM_COLOUR,_T("Fill color (-1: None)"),PARAM_NUMBER,_T("Outline thickness"),PARAM_COLOUR,_T("Outline color"))
) {
	TargetExists();
	int x1 = GetXPos();
	int y1 = GetYPos();
	int x2 = GetXPos();
	int y2 = GetYPos();
	COLORREF fill = GetCol();
	int thick = GetInt();
	COLORREF out = GetCol();
	TargetImg->Rectangle(x1,y1,x2,y2,fill,thick,out,fill!=-1);

	if Current(rdPtr->targetId)
		RectChanged(x1,y1,x2,y2);
	
	return 0;
}
ACTION(
	/* ID */			10,
	/* Name */			_T("Draw line (%0,%1,%2,%3) with color %4 and thickness %5"),
	/* Flags */			0,
	/* Params */		(6,PARAM_NUMBER,_T("X1"),PARAM_NUMBER,_T("Y1"),PARAM_NUMBER,_T("X2"),PARAM_NUMBER,_T("Y2"),PARAM_COLOUR,_T("Color"),PARAM_NUMBER,_T("Thickness"))
) {
	TargetExists();
	int x1 = GetXPos();
	int y1 = GetYPos();
	int x2 = GetXPos();
	int y2 = GetYPos();
	COLORREF col = GetCol();
	int thick = GetInt();

	if (thick <= 0)
		return 0;

	TargetImg->Line(x1,y1,x2,y2,thick,col);

	if Current(rdPtr->targetId)
		RectChanged(x1,y1,x2,y2);
	
	return 0;
}
ACTION(
	/* ID */			11,
	/* Name */			_T("Delete image %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Image"))
) {
	ImageExists(param1);
	SImage* image = rdPtr->surf->at(param1);
	delete image;

	rdPtr->surf->erase(rdPtr->surf->begin()+param1);
	if(!rdPtr->surf->size())
	{
		rdPtr->targetId = -1;
		rdPtr->target = 0;
		rdPtr->currentId = -1;
		rdPtr->current = 0;
		rdPtr->lastId = -1;
		ImageChanged();
		
		return 0;
	}

	//Target image
	if(rdPtr->targetId==rdPtr->surf->size()||rdPtr->targetId>param1)
	{
		if(rdPtr->targetId > 0)
			TargetImg = Image(--rdPtr->targetId);
		else
		{
			rdPtr->targetId = -1;
			TargetImg = 0;
		}
	} 
	//Only update pointer
	else if(rdPtr->targetId==param1)
		TargetImg = Image(rdPtr->targetId);

	//Current image
	if(rdPtr->currentId==rdPtr->surf->size()||rdPtr->currentId>param1)
	{
		if(rdPtr->currentId > 0)
			CurrentImg = Image(--rdPtr->currentId);
		else
		{
			rdPtr->currentId = -1;
			CurrentImg = 0;
		}
		ImageChanged();
		
	}
	//Only update pointer
	else if(rdPtr->currentId==param1)
	{
		CurrentImg = Image(rdPtr->currentId);
		ImageChanged();
		
	}

	return 0;
}
ACTION(
	/* ID */			12,
	/* Name */			_T("Insert image at %0 with dimensions (%1,%2)"),
	/* Flags */			0,
	/* Params */		(3,PARAM_NUMBER,_T("Image"),PARAM_NUMBER,_T("Width"),PARAM_NUMBER,_T("Height"))
) {
	int id = GetInt();
	ImageIsSafe(id);
	int w = GetInt();
	int h = GetInt();

	cSurface* tmp = new cSurface;
	if(w>0&&h>0)
	{
		cSurface* proto = 0;
		GetSurfacePrototype(&proto,rdPtr->depth,ST_MEMORYWITHDC,SD_DIB);
		tmp->Create(w,h,proto);
		tmp->Fill(rdPtr->transCol);
		tmp->SetTransparentColor(rdPtr->transCol);
		rdPtr->transCol = BLACK;
	}
	rdPtr->surf->insert(rdPtr->surf->begin()+id,new SImage(tmp));
	//Update current ID
	if(rdPtr->targetId>=0&&param1<=rdPtr->currentId&&rdPtr->currentId<rdPtr->surf->size()-1)
		rdPtr->targetId++;
	if(rdPtr->currentId>=0&&param1<=rdPtr->currentId&&rdPtr->currentId<rdPtr->surf->size()-1)
		rdPtr->currentId++;
	//Last added ID
	rdPtr->lastId = id;
	//Select
	if(rdPtr->selectLast)
	{
		rdPtr->targetId = rdPtr->lastId;
		TargetImg = Image(rdPtr->lastId);
		rdPtr->targetValid = TargetImg->IsValid();
		if Current(rdPtr->targetId)
		{
			CurrentImg = TargetImg;
			UpdateHotspot(rdPtr);
			ImageChanged();
		}
		
	}
	return 0;
}

ACTION(
        /* ID */                        13,
        /* Name */                      _T("Resize image to (%0,%1)"),
        /* Flags */                     0,
        /* Params */            (2,PARAM_NUMBER,_T("Width"),PARAM_NUMBER,_T("Height"))
) {
        ImageInRange(rdPtr->targetId);
        int w = GetInt();
        int h = GetInt();
        if(w>0&&h>0)
        { 
                //Resize by cloning
                if(TargetImg->IsValid())
                {
                        int ow = TargetImg->GetWidth();
                        int oh = TargetImg->GetHeight();
                        cSurface tmp;
                        tmp.Clone(*TargetImg);
                        //Create from prototype and Blit
                        cSurface* proto = 0;
                        GetSurfacePrototype(&proto,rdPtr->depth,TargetImg->GetType(),TargetImg->GetDriver());
                        TargetImg->Delete();
                        TargetImg->Create(w,h,proto);
                        int flag = STRF_COPYALPHA|(rdPtr->rs.rsEffect&EFFECTFLAG_ANTIALIAS?STRF_RESAMPLE:0);
                        tmp.Stretch(*TargetImg,0,0,w,h,0,0,ow,oh,BMODE_OPAQUE,BOP_COPY,0,flag);
                }
                //Invalid, create new from prototype
                else {
                        cSurface* proto = 0;
                        GetSurfacePrototype(&proto,rdPtr->depth,ST_MEMORYWITHDC,SD_DIB);
                        TargetImg->Create(w,h,proto);
                        TargetImg->Fill(BLACK);
                        rdPtr->targetValid = true;
                }
                //Update display image
                if Current(rdPtr->targetId)
                {
                        CurrentImg = TargetImg;
                        UpdateHotspot(rdPtr);
                        ImageChanged();
                }
				
        }
        else if(TargetImg->IsValid())
        {
                TargetImg->Delete();
                rdPtr->targetValid = false;
                if Current(rdPtr->targetId)
                {
                        rdPtr->rHo.hoImgWidth = 0;
                        rdPtr->rHo.hoImgHeight = 0;
						rdPtr->rHo.hoImgXSpot = 0;
						rdPtr->rHo.hoImgYSpot = 0;
                        ImageChanged();
                }
				
        }
        return 0;
}

void __cdecl SurfSaveImage(LPRDATA rdPtr)
{
	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	CImageFilterMgr* pImgMgr = rhPtr->rh4.rh4Mv->mvImgFilterMgr;

	if(ExportImage(pImgMgr,rdPtr->ioFile,TargetImg,rdPtr->ioFilter))
	{
		if(rdPtr->ioHandle)
			rdPtr->rRd->PushEvent(5);
		else
			rdPtr->rRd->GenerateEvent(5);
	}
	else
	{
		if(rdPtr->ioHandle)
			rdPtr->rRd->PushEvent(3);
		else
			rdPtr->rRd->GenerateEvent(3);
	}
	//Free stuff again
	free(rdPtr->ioFile);
	rdPtr->ioFile = 0;
	rdPtr->ioHandle = 0;
}

ACTION(
	/* ID */			14,
	/* Name */			_T("Save image to file %0, override extension = %1"),//, compression level = %2,
	/* Flags */			0,
	/* Params */		(2,PARAM_FILENAME,_T("File"),PARAM_STRING,_T("Override extension (Specify if path does not include an extension like \"png\""))//,PARAM_NUMBER,_T("Compression level (0-100, -1: Ignore)"))
) {
	TargetExists();
	TCHAR* file = GetStr();
	TCHAR* ext = GetStr();

	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	CImageFilterMgr* pImgMgr = rhPtr->rh4.rh4Mv->mvImgFilterMgr;
	DWORD filter = *ext?FindFilter(pImgMgr,ext,true):FindFilter(pImgMgr,file);
	//No filter
	if(!filter)
	{
		rdPtr->rRd->GenerateEvent(3);
		return 0;
	}

	if (rdPtr->ioFile)
		free(rdPtr->ioFile);
	rdPtr->ioFile = _tcsdup(file);
	rdPtr->ioFilter = filter;

	if(rdPtr->threadedIO)
	{
		if (rdPtr->ioHandle)
		{
			rdPtr->rRd->GenerateEvent(3);
			return 0;
		}

		rdPtr->ioSave = true;
		rdPtr->ioHandle = rdPtr->rRd->StartThread(SurfSaveImage);
	}
	else
	{
		SurfSaveImage(rdPtr);
	}

	return 0;
}

void __cdecl SurfLoadImage(LPRDATA rdPtr)
{
	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	CImageFilterMgr* pImgMgr = rhPtr->rh4.rh4Mv->mvImgFilterMgr;

	cSurface* psf = new cSurface, *tmp = TargetImg;
	COLORREF transCol = tmp->GetTransparentColor();

	if(ImportImage(pImgMgr,rdPtr->ioFile,psf,0,0))
	{
		//Clone image
		if(tmp->IsValid())
			tmp->Delete();
		cSurface* proto = 0;
		GetSurfacePrototype(&proto,rdPtr->depth,ST_MEMORYWITHDC,SD_DIB);
		tmp->Create(psf->GetWidth(),psf->GetHeight(),proto);
		psf->Blit(*tmp,0,0,BMODE_OPAQUE,BOP_COPY,0,BLTF_COPYALPHA);
		tmp->SetTransparentColor(transCol);

		rdPtr->targetValid = true;
		if Current(rdPtr->targetId)
		{
			UpdateHotspot(rdPtr);
			ImageChanged();
		}
		
		//Success
		if(rdPtr->threadedIO)
			rdPtr->rRd->PushEvent(4);
		else
			rdPtr->rRd->GenerateEvent(4);
		
		if Current(rdPtr->targetId)
			ImageChanged();
		
	}
	//Load failed
	else
	{
		if(rdPtr->threadedIO)
			rdPtr->rRd->PushEvent(2);
		else
			rdPtr->rRd->GenerateEvent(2);
	}
	//Delete temp
	delete psf;
	free(rdPtr->ioFile);
	rdPtr->ioFile = 0;
	rdPtr->ioHandle = 0;
}

short WINAPI DLLExport ActionFunc153(LPRDATA rdPtr, long param1, long param2);
ACTION(
	/* ID */			15,
	/* Name */			_T("Load image from file %0, override extension = %1"),
	/* Flags */			0,
	/* Params */		(2,PARAM_FILENAME2,_T("File"),PARAM_STRING,_T("Override extension"))
) {
	ActionFunc153(rdPtr,param1,param2);
	return 0;
}

ACTION(
	/* ID */			16,
	/* Name */			_T("Flood fill at (%0,%1) with color %2 and tolerance %3"),
	/* Flags */			0,
	/* Params */		(4,PARAM_NUMBER,_T("X"),PARAM_NUMBER,_T("Y"),PARAM_COLOUR,_T("Color"),PARAM_NUMBER,_T("Tolerance (0-255)"))
) {
	TargetExists();
	int x = GetXPos();
	int y = GetYPos();
	COLORREF col = GetCol();
	int tol = GetInt();
	TargetImg->FloodFill(x,y,rdPtr->floodL,rdPtr->floodT,rdPtr->floodR,rdPtr->floodB,col,USE_AA,tol);
	if Current(rdPtr->targetId)
		RectChanged(rdPtr->floodL,rdPtr->floodT,rdPtr->floodR,rdPtr->floodB);
	
	return 0;
}

ACTION(
	/* ID */			17,
	/* Name */			_T("Add image with dimensions (%0,%1)"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Width"),PARAM_NUMBER,_T("Height"))
) {
	int w = GetInt();
	int h = GetInt();
	cSurface* tmp = new cSurface;
	if(w>0&&h>0)
	{
		cSurface* proto = 0;
		GetSurfacePrototype(&proto,rdPtr->depth,ST_MEMORYWITHDC,SD_DIB);
		tmp->Create(w,h,proto);
		tmp->Fill(rdPtr->transCol);
		tmp->SetTransparentColor(rdPtr->transCol);
		rdPtr->transCol = BLACK;
	}
	rdPtr->surf->push_back(new SImage(tmp));
	rdPtr->lastId = rdPtr->surf->size()-1;
	//Select
	if(rdPtr->selectLast)
	{
		rdPtr->targetId = rdPtr->lastId;
		TargetImg = Image(rdPtr->lastId);
		rdPtr->targetValid = TargetImg->IsValid();
		if Current(rdPtr->targetId)
		{
			CurrentImg = TargetImg;
			UpdateHotspot(rdPtr);
			ImageChanged();
		}
		
	}
	return 0;
}
ACTION(
	/* ID */			18,
	/* Name */			_T("Delete all images"),
	/* Flags */			0,
	/* Params */		(0)
) {
	cSurface* tmp;
	while(rdPtr->surf->size())
	{
		delete rdPtr->surf->at(0);
		rdPtr->surf->erase(rdPtr->surf->begin());
	}
	TargetImg = CurrentImg = 0;
	rdPtr->targetId = rdPtr->currentId -1;
	rdPtr->lastId = -1;
	rdPtr->targetValid = false;
	ImageChanged();
	
	return 0;
}
ACTION(
	/* ID */			19,
	/* Name */			_T("Blit onto image %0's alpha channel"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Target image"))
) {
	TargetExists();
	int target = GetInt();
	ImageExists(target);
	if(Image(target)->HasAlpha())
	{
		cSurface* alpha = Image(target)->GetAlphaSurface();
		Blit(TargetImg,alpha,rdPtr);
		Image(target)->ReleaseAlphaSurface(alpha);
		if Current(target)
			RectChanged(x,y,x+TargetImg->GetWidth(),y+TargetImg->GetHeight());
		
	}
	return 0;
}
ACTION(
	/* ID */			20,
	/* Name */			_T("Replace %0 with %1"),
	/* Flags */			0,
	/* Params */		(2,PARAM_COLOUR,_T("Old color"),PARAM_COLOUR,_T("New color"))
) {
	TargetExists();
	TargetImg->ReplaceColor(param2,param1);
	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}
ACTION(
	/* ID */			21,
	/* Name */			_T("Reverse X"),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();
	TargetImg->ReverseX();
	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}
ACTION(
	/* ID */			22,
	/* Name */			_T("Reverse Y"),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();
	TargetImg->ReverseY();
	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}
ACTION(
	/* ID */			23,
	/* Name */			_T("Minimize"),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();
	TargetImg->Minimize();
	if Current(rdPtr->targetId)
	{
		UpdateHotspot(rdPtr);
		ImageChanged();
	}
	
	return 0;
}
ACTION(
	/* ID */			24,
	/* Name */			_T("Set transparent color to %0, replace old color = %1"),
	/* Flags */			0,
	/* Params */		(2,PARAM_COLOUR,_T("Transparent color"),PARAM_NUMBER,_T("Replace old color? (0: No, 1: Yes)"))
) {
	//Oh my, what a crappy workaround
	rdPtr->transCol = param1;
	TargetExists();
	rdPtr->transCol = 0;
	//Replace old color if wanted
	if(param2)
		TargetImg->ReplaceColor(param1,TargetImg->GetTransparentColor());
	TargetImg->SetTransparentColor(param1);

	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}
ACTION(
	/* ID */			25,
	/* Name */			_T("Draw line (%0,%1,%2,%3) with value %4 on the alpha channel"),
	/* Flags */			0,
	/* Params */		(5,PARAM_NUMBER,_T("X1"),PARAM_NUMBER,_T("Y1"),PARAM_NUMBER,_T("X2"),PARAM_NUMBER,_T("Y2"),PARAM_NUMBER,_T("Alpha (0-255)"))
) {
	TargetExists();
	ActionFunc25(rdPtr, 1, 0);
	if(!TargetImg->HasAlpha()) return 0;
	int x1 = GetXPos();
	int y1 = GetYPos();
	int x2 = GetXPos();
	int y2 = GetYPos();
	int col = GetInt();
	cSurface* alpha = TargetImg->GetAlphaSurface();
	alpha->Line(x1,y1,x2,y2,1,RGB(col,col,col));
	TargetImg->ReleaseAlphaSurface(alpha);
	if Current(rdPtr->targetId)
		RectChanged(x1,y1,x2,y2);
	
	return 0;
}
ACTION(
	/* ID */			26,
	/* Name */			_T("Perform %0 with color %1"),
	/* Flags */			0,
	/* Params */		(2,PARAM_STRING,PARAM_OPERATOR,PARAM_COLOUR,_T("Color"))
) {
	TargetExists();
	char* op = (char*)GetStr();
	COLORREF col = GetCol();
	float cr = GetRValue(col);
	float cg = GetGValue(col);
	float cb = GetBValue(col);
	//Hardcode *, **, /
	if(*op=='*'||*op=='/')
	{
		cr /= 255.0f;
		cg /= 255.0f;
		cb /= 255.0f;
	}
	//Prevent division by zero
	if(*op=='/'||*op=='%')
	{
		if(!cr) cr = 0.001f;
		if(!cg) cg = 0.001f;
		if(!cb) cb = 0.001f;
	}

	unsigned char rTable[256], gTable[256], bTable[256];
	for (int i = 0; i < 256; ++i)
	{
		rTable[i] = i;
		gTable[i] = i;
		bTable[i] = i;
		Operation(op, &rTable[i], cr);
		Operation(op, &gTable[i], cg);
		Operation(op, &bTable[i], cb);
	}

	int width = TargetImg->GetWidth();
	int height = TargetImg->GetHeight();
	
	//Color channel
	BYTE* buff;
	buff = TargetImg->LockBuffer();
	if(!buff) return 0;

	int pitch = TargetImg->GetPitch();
	if(pitch < 0)
	{
		pitch *= -1;
		buff -= pitch*(height-1);
	}
	int size = pitch*height;
	int byte = TargetImg->GetDepth()>>3;

	BYTE* end = buff+size;
	for(BYTE* y=buff; y<end ;y+=pitch)
	{
		BYTE* nextline = y+pitch;
		for(BYTE* x=y; x<nextline; x+=byte)
		{
			x[2] = rTable[x[2]];
			x[1] = gTable[x[1]];
			x[0] = bTable[x[0]];
		}
	}

	TargetImg->UnlockBuffer(buff);

	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}
ACTION(
	/* ID */			27,
	/* Name */			_T("Force redraw"),
	/* Flags */			0,
	/* Params */		(0)
) {
	ImageChanged();
	
	return 0;
}
ACTION(
	/* ID */			28,
	/* Name */			_T("Copy image %0 from %1"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Destination image"),PARAM_NUMBER,_T("Source image"))
) {
	ImageInRange(param1);
	ImageExists(param2);
	if(param1==param2) return 0;
	//Clone image
	if(Image(param1)->IsValid()) Image(param1)->Delete();
	Image(param1)->Clone(*Image(param2));
	//Make image valid
	if(rdPtr->targetId==param1)
	{
		TargetImg = Image(param1);
		rdPtr->targetValid = true;
	}
	//Redraw
	if Current(param1)
	{
		CurrentImg = Image(param1);
		UpdateHotspot(rdPtr);
		ImageChanged();
	}
	
	return 0;
}
ACTION(
	/* ID */			29,
	/* Name */			_T("Select image %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Image"))
) {
	ImageInRange(param1);
	TargetImg = Image(param1);
	rdPtr->targetId = param1;
	rdPtr->targetValid = TargetImg->IsValid();
	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}

ACTION(
	/* ID */			30,
	/* Name */			_T("Draw polygon at (%0,%1) with color %2, outline thickness %3, outline color %4"),
	/* Flags */			0,
	/* Params */		(5,PARAM_NUMBER,_T("X"),PARAM_NUMBER,_T("Y"),PARAM_COLOUR,_T("Fill color (-1: None)"),PARAM_NUMBER,_T("Outline thickness"),PARAM_COLOUR,_T("Outline color"))
) {
	TargetExists();
	int ox = GetXPos();
	int oy = GetYPos();
	COLORREF fill = GetCol();
	int thick = GetInt();
	COLORREF out = GetCol();

	int size = rdPtr->points->size();
	POINT* tmp = new POINT[size];

	for(int i=0;i<size;i++)
	{
		tmp[i].x = rdPtr->points->at(i).x+ox;
		tmp[i].y = rdPtr->points->at(i).y+oy;
	}

	TargetImg->Polygon(tmp,size,fill,thick,out,fill!=-1);

	//Delete points
	delete[] tmp;

	//Don't keep points
	if(!rdPtr->keepPoints)
		rdPtr->points->clear();

	if Current(rdPtr->targetId)
		ImageChanged();
	

	return 0;
}
ACTION(
	/* ID */			31,
	/* Name */			_T("Insert point (%1,%2) at %0"),
	/* Flags */			0,
	/* Params */		(3,PARAM_NUMBER,_T("Index (-1: Append)"),PARAM_NUMBER,_T("X"),PARAM_NUMBER,_T("Y"))
) {
	float x,y;
	int i = GetInt(), size = rdPtr->points->size();
	if(i<-1||i>size)
		return 0;
	LoadFloat(x);
	LoadFloat(y);
	PolyCoord p = {x,y};
	//Insert or append
	if(i>=0&&i<size)
		rdPtr->points->insert(rdPtr->points->begin()+i,p);
	else
		rdPtr->points->push_back(p);
	return 0;
}
ACTION(
	/* ID */			32,
	/* Name */			_T("Remove all points"),
	/* Flags */			0,
	/* Params */		(0)
) {
	rdPtr->points->clear();
	return 0;
}
ACTION(
	/* ID */			33,
	/* Name */			_T("Add points from string %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_STRING,_T("List of points X1,Y1,X2,Y2..."))
) {
	//Temp coord
	int i = 0;
	PolyCoord p;
	PolyPoints::iterator it, end;
	//Parse
	TCHAR* strp = GetStr();
	char* tok, *next, *str;
#ifdef UNICODE
	str = new char[wcslen(strp)+1];
	for (int i = 0; i < wcslen(strp)+1; ++i)
		str[i] = (char)strp[i];
#else
	str = strp;
#endif
	tok = strtok(str," ,;()\n\r\txy");
	while(tok)
	{
		//Y coordinate, add point
		if(i%2)
		{
			p.y = atof(tok);
			rdPtr->points->push_back(p);
		}
		//X coordinate
		else
			p.x = atof(tok);
		//Next
		tok = strtok(0," ,;()\n\r\txy");
		i++;
	}
#ifdef UNICODE
	delete[] str;
#endif
	return 0;
}
ACTION(
	/* ID */			34,
	/* Name */			_T("Move all points by (%0,%1) pixels"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("X Offset"),PARAM_NUMBER,_T("Y Offset"))
) {
	float ox,oy;
	LoadFloat(ox);
	LoadFloat(oy);
	PolyPoints::iterator it = rdPtr->points->begin(), end = rdPtr->points->end();
	while(it!=end)
	{
		(*it).x += ox;
		(*it).y += oy;
		++it;
	}
	return 0;
}
ACTION(
	/* ID */			35,
	/* Name */			_T("Rotate all points around (%0,%1) by %2 degrees"),
	/* Flags */			0,
	/* Params */		(3,PARAM_NUMBER,_T("Pivot point X"),PARAM_NUMBER,_T("Pivot point Y"),PARAM_NUMBER,_T("Angle (Degrees)"))
) {
	float ox,oy,a;
	LoadFloat(ox);
	LoadFloat(oy);
	LoadFloat(a);
	a = a/180.0f*PI;

	PolyPoints::iterator it = rdPtr->points->begin(), end = rdPtr->points->end();
	while(it!=end)
	{
		float tx = (*it).x-ox;
		float ty = (*it).y-oy;
		(*it).x = tx*cos(a)-ty*sin(a)+ox;
		(*it).y = ty*cos(a)+tx*sin(a)+oy;
		++it;
	}

	return 0;
}
ACTION(
	/* ID */			36,
	/* Name */			_T("Remove point %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Point index"))
) {
	int size = rdPtr->points->size();
	if(!size)
		return 0;
	param1 = max(0,min(size-1,param1));
	rdPtr->points->erase(rdPtr->points->begin()+param1);
	return 0;
}
ACTION(
	/* ID */			37,
	/* Name */			_T("Set blit transparency to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Blit transparency (0: Opaque, 1: Transparent)"))
) {
	rdPtr->b.mode = param1?BMODE_TRANSP:BMODE_OPAQUE;
	return 0;
}
ACTION(
	/* ID */			38,
	/* Name */			_T("Set blit alpha mode to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Alpha mode (0: None, 1: Copy alpha, 2: Compose alpha)"))
) {
	rdPtr->b.flags = (param1==1)?BLTF_COPYALPHA:0;
	rdPtr->b.composeAlpha = param1==2;
	return 0;
}
ACTION(
	/* ID */			39,
	/* Name */			_T("Set blit semi-transparency to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Semi-transparency (0-128, Make sure to enable the 'Semi-transparency' blit effect!)"))
) {
	// Deprecated
	rdPtr->b.param = (rdPtr->b.param & 0xffffff) | (int(max(0, min(255, 128 - param1*128.0/255))) << 24);
	return 0;
}

ACTION(
	/* ID */			40,
	/* Name */			_T("Set blit effect to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Blit effect (0: None, 1: Semi-transparency, 2: Invert, 3: XOR, 4: AND, 5: OR, 9: Add, 10: Mono, 11: Subtract)"))//, 12: HWA Tint)"))
) {
	//if(param1 == 12)
	//	param1 = (int)BOP_RGBAFILTER;

	rdPtr->b.operation = (BlitOp)param1;
	return 0;
}
ACTION(
	/* ID */			41,
	/* Name */			_T("Set blit destination position to (%0,%1)"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Destination X"),PARAM_NUMBER,_T("Destination Y"))
) {
	rdPtr->b.destX = GetInt();
	rdPtr->b.destY = GetInt();
	return 0;
}
ACTION(
	/* ID */			42,
	/* Name */			_T("Set use absolute coords to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Use absolute coordinates? (0: No, 1: Yes)"))
) {
	rdPtr->useAbs = param1;
	return 0;
}
ACTION(
	/* ID */			43,
	/* Name */			_T("Create color pattern %0 with color %1"),
	/* Flags */			0,
	/* Params */		(2,PARAM_STRING,_T("Pattern name"),PARAM_COLOUR,_T("Color"))
) {
	string str = string(GetStr());
	if(!str[0]) return 0;
	COLORREF col = GetCol();

	FillGuide* guide = new FillGuide;
	guide->type = FILL_COLOR;
	guide->img = false;
	CFillFlat* fill = new CFillFlat(col);
	guide->fill = fill;


	rdPtr->fill->erase(str);
	rdPtr->fill->insert(pair<string,FillGuide*>(str,guide));
	return 0;
}
ACTION(
	/* ID */			44,
	/* Name */			_T("Draw rectangle (%0,%1,%2,%3) with fill pattern %4, outline thickness %5, outline pattern %6"),
	/* Flags */			0,
	/* Params */		(7,PARAM_NUMBER,_T("X1"),PARAM_NUMBER,_T("Y1"),PARAM_NUMBER,_T("X2"),PARAM_NUMBER,_T("Y2"),PARAM_STRING,_T("Pattern (Empty: None)"),PARAM_NUMBER,_T("Outline thickness"),PARAM_STRING,_T("Outline pattern (Empty: None)"))
) {
	TargetExists();
	int x1 = GetXPos();
	int y1 = GetYPos();
	int x2 = GetXPos();
	int y2 = GetYPos();
	string fill = string(GetStr());
	int thick = GetInt();
	string out = string(GetStr());

	FillGuide* fillData=0,* outData=0;
	bool useFill = fill[0]!=0;
	bool useOut = (thick!=0)&&(out[0]!=0);

	if(!ProcessFillFlags(rdPtr,fill,out,useFill,useOut,fillData,outData))
		return 0;

	//rdPtr->userXOff = min(x1,x2);
	//rdPtr->userYOff = min(y1,y2);

	//Final check: Outline or not?
	if(useOut)
	{
		TargetImg->Rectangle(x1,y1,x2,y2,
		useFill?fillData->fill:0,thick, //Fill
		outData->fill,false,rdPtr->b.mode,rdPtr->b.operation,rdPtr->b.param, //Outline+Params
		useFill); //Use fill?
	}
	else if(useFill)
	{
		TargetImg->Rectangle(x1,y1,x2,y2,
		useFill?fillData->fill:0, //Fill
		USE_AA,rdPtr->b.mode,rdPtr->b.operation,rdPtr->b.param); //Params
	}

	if Current(rdPtr->targetId)
		RectChanged(x1,y1,x2,y2);
	
	return 0;
}
ACTION(
	/* ID */			45,
	/* Name */			_T("Create tiled image pattern %0 with image %1 and offset (%2,%3)"),
	/* Flags */			0,
	/* Params */		(4,PARAM_STRING,_T("Pattern name"),PARAM_NUMBER,_T("Image"),PARAM_NUMBER,_T("X Offset"),PARAM_NUMBER,_T("Y Offset"))
) {
	string str = string(GetStr());
	if(!str[0]) return 0;
	int img = GetInt();
	int x = GetInt();
	int y = GetInt();
	ImageExists(img);

	FillGuide* guide = new FillGuide;
	guide->type = FILL_MOSAIC;
	guide->img = true;
	guide->param = img;
	CFillMosaic* fill = new CFillMosaic(Image(img),x,y);
	guide->fill = fill;

	rdPtr->fill->erase(str);
	rdPtr->fill->insert(pair<string,FillGuide*>(str,guide));
	return 0;
}
ACTION(
	/* ID */			46,
	/* Name */			_T("Create linear gradient pattern %0 with colors (%1,%2), vertical = %3"),
	/* Flags */			0,
	/* Params */		(4,PARAM_STRING,_T("Pattern name"),PARAM_COLOUR,_T("Color A"),PARAM_COLOUR,_T("Color B"),PARAM_NUMBER,_T("Vertical? (0: No, 1: Yes)"))
) {
	string str = string(GetStr());
	if(!str[0]) return 0;
	COLORREF a = GetCol();
	COLORREF b = GetCol();
	int vert = GetInt();

	FillGuide* guide = new FillGuide;
	guide->type = FILL_GRADIENT;
	guide->img = false;
	CFillDirEx* fill = new CFillDirEx(a,b,vert);
	guide->fill = fill;

	rdPtr->fill->erase(str);
	rdPtr->fill->insert(pair<string,FillGuide*>(str,guide));
	return 0;
}
ACTION(
	/* ID */			47,
	/* Name */			_T("Load image from the clipboard"),
	/* Flags */			0,
	/* Params */		(0)
) {
	ImageInRange(rdPtr->targetId);

	if(IsClipboardFormatAvailable(CF_DIB)&&OpenClipboard(NULL))
	{
		COLORREF transCol = TargetImg->GetTransparentColor();
		HANDLE handle = GetClipboardData(CF_DIB);
		BITMAPINFO* bmp = (BITMAPINFO*)GlobalLock(handle);
		//Load DIB
		cSurface* tmp = TargetImg;
		if(tmp->IsValid())
			tmp->Delete();
		cSurface* proto = 0;
		GetSurfacePrototype(&proto,rdPtr->depth,ST_MEMORYWITHDC,SD_DIB);
		tmp->Create(bmp->bmiHeader.biWidth,bmp->bmiHeader.biHeight,proto);
		if(tmp->IsValid()&&tmp->LoadImage(bmp,GetDIBBitmap(bmp)))
		{
			//Sucess! Update
			tmp->SetTransparentColor(transCol);
			rdPtr->targetValid = true;
			if Current(rdPtr->targetId)
			{
				rdPtr->rHo.hoImgWidth = TargetImg->GetWidth();
				rdPtr->rHo.hoImgHeight = TargetImg->GetHeight();
				UpdateHotspot(rdPtr);
				ImageChanged();
			}
			
			rdPtr->rRd->GenerateEvent(4);
		}
		//Fail
		else
		{
			rdPtr->rRd->GenerateEvent(2);
			tmp->Delete();
		}

		GlobalUnlock(handle);
		CloseClipboard();
	}
	//Fail
	else
	{
		rdPtr->rRd->GenerateEvent(2);
	}
	return 0;
}
ACTION(
	/* ID */			48,
	/* Name */			_T("Save image to the clipboard"),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();
	if(OpenClipboard(NULL))
	{
		EmptyClipboard();
		cSurface* tmp = TargetImg;
		//Allocate buffer
		HGLOBAL cb = GlobalAlloc(GMEM_MOVEABLE,tmp->GetDIBSize());
		BITMAPINFO* bmp = (BITMAPINFO*)GlobalLock(cb);
		//Copy image
		tmp->SaveImage(bmp,(BYTE*)(bmp+1)-4);
		SetClipboardData(CF_DIB,bmp);
		bmp = bmp;
		//Done
		GlobalUnlock(cb);
		CloseClipboard();
		rdPtr->rRd->GenerateEvent(5);
	}
	//Fail
	else
	{
		rdPtr->rRd->GenerateEvent(3);
	}
	return 0;
}
ACTION(
	/* ID */			49,
	/* Name */			_T("Blit Active object %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_OBJECT,_T("Object"))
) {
	TargetExists();
	LPRO obj = (LPRO)GetParam();
	if(!obj)
		return 0;

	cSurface is;
	rdPtr->rRd->GrabSurface(obj,is);
	Blit(&is,TargetImg,rdPtr);

	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}
ACTION(
	/* ID */			50,
	/* Name */			_T("Draw ellipse (%0,%1,%2,%3) with pattern %4, outline thickness %5, outline pattern %6"),
	/* Flags */			0,
	/* Params */		(7,PARAM_NUMBER,_T("X1"),PARAM_NUMBER,_T("Y1"),PARAM_NUMBER,_T("X2"),PARAM_NUMBER,_T("Y2"),PARAM_STRING,_T("Pattern (Empty: None)"),PARAM_NUMBER,_T("Outline thickness"),PARAM_STRING,_T("Outline pattern (Empty: None)"))
) {
	TargetExists();
	int x1 = GetXPos();
	int y1 = GetYPos();
	int x2 = GetXPos();
	int y2 = GetYPos();
	string fill = string(GetStr());
	int thick = GetInt();
	string out = string(GetStr());

	FillGuide* fillData=0,* outData=0;
	bool useFill = fill[0]!=0;
	bool useOut = (thick!=0)&&(out[0]!=0);

	if(!ProcessFillFlags(rdPtr,fill,out,useFill,useOut,fillData,outData))
		return 0;

	//rdPtr->userXOff = min(x1,x2);
	//rdPtr->userYOff = min(y1,y2);

	//Final check: Outline or not?
	if(useOut)
	{
		TargetImg->Ellipse(x1,y1,x2,y2,
		useFill?fillData->fill:0,thick, //Fill
		outData->fill,false,rdPtr->b.mode,rdPtr->b.operation,rdPtr->b.param, //Outline+Params
		useFill); //Use fill
	}
	else if(useFill)
	{
		TargetImg->Ellipse(x1,y1,x2,y2,
		useFill?fillData->fill:0, //Fill
		USE_AA,rdPtr->b.mode,rdPtr->b.operation,rdPtr->b.param); //Params
	}
	
	if Current(rdPtr->targetId)
		RectChanged(x1,y1,x2,y2);
	
	return 0;
}
ACTION(
	/* ID */			51,
	/* Name */			_T("Draw polygon at (%0,%1) with pattern %2, outline thickness %3, outline pattern %4"),
	/* Flags */			0,
	/* Params */		(5,PARAM_NUMBER,_T("X"),PARAM_NUMBER,_T("Y"),PARAM_STRING,_T("Pattern (Empty: None)"),PARAM_NUMBER,_T("Outline thickness"),PARAM_STRING,_T("Outline pattern (Empty: None)"))
) {
	TargetExists();
	int ox = GetXPos();
	int oy = GetYPos();
	string fill = string(GetStr());
	int thick = GetInt();
	string out = string(GetStr());


	FillGuide* fillData=0,* outData=0;
	bool useFill = fill[0]!=0;
	bool useOut = (thick!=0)&&(out[0]!=0);

	if(!ProcessFillFlags(rdPtr,fill,out,useFill,useOut,fillData,outData))
		return 0;

	int size = rdPtr->points->size();
	POINT* tmp = new POINT[size];

	int left = 0x7fffffff, top = 0x7fffffff;

	//User callback - calculate bounding box
	if((useFill&&fillData->type==FILL_USER)||(useOut&&outData->type==FILL_USER))
	{
		for(int i=0;i<size;i++)
		{
			if((tmp[i].x = rdPtr->points->at(i).x+ox) < left)
				left = tmp[i].x;
			if((tmp[i].y = rdPtr->points->at(i).y+oy) < top)
				top = tmp[i].y;
		}

		//rdPtr->userXOff = left;
		//rdPtr->userYOff = top;
	}
	//Bounding box not necessary
	else for(int i=0;i<size;i++)
	{
		tmp[i].x = rdPtr->points->at(i).x+ox;
		tmp[i].y = rdPtr->points->at(i).y+oy;
	}

	//Don't keep points
	if(!rdPtr->keepPoints)
		rdPtr->points->clear();

	//Final check: Outline or not?
	if(useOut)
	{
		TargetImg->Polygon(tmp,size,
		useFill?fillData->fill:0,thick, //Fill
		outData->fill,false,rdPtr->b.mode,rdPtr->b.operation,rdPtr->b.param, //Outline+Params
		useFill); //Use fill?
	}
	else if(useFill)
	{
		TargetImg->Polygon(tmp,size,
		useFill?fillData->fill:0, //Fill
		USE_AA,rdPtr->b.mode,rdPtr->b.operation,rdPtr->b.param); //Params
	}
	//Delete points
	delete[] tmp;

	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}
ACTION(
	/* ID */			52,
	/* Name */			_T("Draw text %4 at (%0,%1,%2,%3) with color %5"),
	/* Flags */			0,
	/* Params */		(6,PARAM_NUMBER,_T("X1"),PARAM_NUMBER,_T("Y1"),PARAM_NUMBER,_T("X2 (Leave zero to use X1)"),PARAM_NUMBER,_T("Y2 (Leave zero to use Y1)"),PARAM_STRING,_T("Text"),PARAM_COLOUR,_T("Color"))
) {
	int x1 = GetXPos();
	int y1 = GetYPos();
	int x2 = GetXPos();
	int y2 = GetYPos();
	TCHAR* text = GetStr();
	COLORREF color = GetCol();
	TargetExists();

	if (TargetImg->GetType() != ST_MEMORYWITHDC)
		return 0;

	RECT rect = {x1,y1,x2,y2};
	//Create font if not done yet
	if(!rdPtr->hFont)
		CreateFontIndirect(&rdPtr->textFont);
	//AA?
	bool useAA = rdPtr->textFont.lfQuality==ANTIALIASED_QUALITY;
	//No clipping, set x2 and y2
	if(rdPtr->hFlags&DT_NOCLIP)
	{
		if(!x2) x2 = x1;
		if(!y2) y2 = y1;
	}

	// Draw
	TargetImg->DrawText(text, -1, &rect, rdPtr->hFlags, color, rdPtr->hFont, rdPtr->b.mode, rdPtr->b.operation, rdPtr->b.param, useAA);
	
	if Current(rdPtr->targetId)
		RectChanged(x1,y1,x2,y2);
	
	return 0;
}
ACTION(
	/* ID */			53,
	/* Name */			_T("Set horizontal text align to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Text alignment (Left: 0, Center: 1, Right: 2)"))
) {
	rdPtr->hFlags = (rdPtr->hFlags&~(DT_LEFT|DT_RIGHT|DT_CENTER))|param1;
	return 0;
}
ACTION(
	/* ID */			54,
	/* Name */			_T("Set vertical text align to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Text alignment (Top: 0, Center: 1, Bottom: 2)"))
) {
	if(param1)
		param1 <<= 2;
	rdPtr->hFlags = (rdPtr->hFlags&~(DT_TOP|DT_VCENTER|DT_BOTTOM))|param1;
	return 0;
}
ACTION(
	/* ID */			55,
	/* Name */			_T("Set text multi-line to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Multi-line (0: No, 1: Yes)"))
) {
	if(param1)
		rdPtr->hFlags &= ~DT_SINGLELINE;
	else
		rdPtr->hFlags |= DT_SINGLELINE;
	return 0;
}
ACTION(
	/* ID */			56,
	/* Name */			_T("Set text font face to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_STRING,_T("Font"))
) {
	if(rdPtr->hFont) DeleteObject(rdPtr->hFont);
	//Copy facename
	_tcsncpy(&rdPtr->textFont.lfFaceName[0],(TCHAR*)param1,LF_FACESIZE);
	rdPtr->hFont = CreateFontIndirect(&rdPtr->textFont);

	return 0;
}
ACTION(
	/* ID */			57,
	/* Name */			_T("Set text font size to %0 pt"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Font size (points)"))
) {
	if(rdPtr->hFont) DeleteObject(rdPtr->hFont);
	rdPtr->textFont.lfHeight = -MulDiv(param1, GetDeviceCaps(GetDC(0), LOGPIXELSY), 72);
	rdPtr->hFont = CreateFontIndirect(&rdPtr->textFont);

	return 0;
}
ACTION(
	/* ID */			58,
	/* Name */			_T("Set text font quality to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Font quality (0: Don't care, 1: Draft, 2: Proof, 3: Aliased, 4: Anti-aliased, 5: ClearType)"))
) {
	if(rdPtr->hFont) DeleteObject(rdPtr->hFont);
	rdPtr->textFont.lfQuality = param1;
	rdPtr->hFont = CreateFontIndirect(&rdPtr->textFont);

	return 0;
}
ACTION(
	/* ID */			59,
	/* Name */			_T("Set text font weight to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Font weight (0-9, 1: Thin, 5: Medium, 7: Bold)"))
) {
	if(rdPtr->hFont) DeleteObject(rdPtr->hFont);
	rdPtr->textFont.lfWeight = max(0,min(9,param1))	*100;
	rdPtr->hFont = CreateFontIndirect(&rdPtr->textFont);

	return 0;
}
ACTION(
	/* ID */			60,
	/* Name */			_T("Set text font decoration to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Font decoration, add together (1: Italic, 2: Underline, 4: Strike-out)"))
) {
	LOGFONT lf; GetObject(rdPtr->hFont,sizeof(lf),&lf);
	DeleteObject(rdPtr->hFont);
	rdPtr->textFont.lfItalic = param1 &1;
	rdPtr->textFont.lfUnderline = param1 &2;
	rdPtr->textFont.lfStrikeOut = param1 &4;
	rdPtr->hFont = CreateFontIndirect(&rdPtr->textFont);

	return 0;
}

ACTION(
	/* ID */			61,
	/* Name */			_T("Apply convolution matrix (%3,%4,%5|%6,%7,%8|%9,%A,%B), divisor = %0, offset = %1, iterate %2 times"),
	/* Flags */			0,
	/* Params */		(12,PARAM_NUMBER,_T("Divisor"),PARAM_NUMBER,_T("Offset"),PARAM_NUMBER,_T("Iterations"),
						PARAM_NUMBER,_T("X1Y1 (X__|___|___)"),PARAM_NUMBER,_T("X1Y2 (_X_|___|___)"),PARAM_NUMBER,_T("X1Y3 (__X|___|___)"),
						PARAM_NUMBER,_T("X2Y1 (___|X__|___)"),PARAM_NUMBER,_T("X2Y2 (___|_X_|___)"),PARAM_NUMBER,_T("X2Y3 (___|__X|___)"),
						PARAM_NUMBER,_T("X3Y1 (___|___|X__)"),PARAM_NUMBER,_T("X3Y2 (___|___|_X_)"),PARAM_NUMBER,_T("X3Y3 (___|___|__X)"))
) {
	TargetExists();
	float fdiv, foff;
	float ff[3][3];
	LoadFloat(fdiv);
	LoadFloat(foff);
	int iterations = GetInt();
	if(iterations <= 0)
		return 0;
	LoadFloat(ff[0][0]);
	LoadFloat(ff[0][1]);
	LoadFloat(ff[0][2]);
	LoadFloat(ff[1][0]);
	LoadFloat(ff[1][1]);
	LoadFloat(ff[1][2]);
	LoadFloat(ff[2][0]);
	LoadFloat(ff[2][1]);
	LoadFloat(ff[2][2]);

	//Use ints for SPEED

#define TOFIX(x) int((x) * 10000)
#define TOINT(x) int((x) / 10000)

	int div = TOFIX(max(1/255.0f, fdiv));
	if(!div) div = TOFIX(1);
	int off = TOFIX(foff);

	int filter[3][3][256];
	for(int x = 0; x < 3; ++x)
		for(int y = 0; y < 3; ++y)
			for(int i = 0; i < 256; ++i)
				filter[x][y][i] = TOFIX(i*ff[x][y]);

	// Build input->output table
	static BYTE convert[0x2000];
	for(int i = 0; i < 0x2000; ++i)
		convert[i] = max(0, min(255, TOFIX(i - 0x1000)/div + off));

	//Dimensions
	int width = TargetImg->GetWidth(), height = TargetImg->GetHeight();
	//New RGB
	int nr,ng,nb;
	//Old RGB
	COLORREF rgb;
	//New position
	int vx, vy;
	//Other variables
	int offset;

	//Lock buffer, get pitch etc.
	BYTE* buff;
	buff = TargetImg->LockBuffer();
	if(!buff) return 0;

	int pitch = TargetImg->GetPitch();
	if(pitch < 0)
	{
		pitch *= -1;
		buff -= pitch*(height-1);
	}
	int size = pitch*height;
	int byte = TargetImg->GetDepth()>>3;

	//Backup buffer
	BYTE* src = (BYTE*)malloc(size);
	memcpy(src,buff,size);

	//Loop through all pixels
	for(int loop = 0; loop < iterations; ++loop)
	{
		for(int y = 0; y < height; ++y)
		{
			for(int x = 0; x < width; ++x)
			{
				nr = 0;
				ng = 0;
				nb = 0;

				//Loop through matrix
				for(int j = 0; j < 3; ++j)
				{
					vy = min(max(y-1+j,0),height-1);
					for(int i = 0; i < 3; ++i)
					{
						//NOTE: Y upside down in buffer
						vx = min(max(x+1-i,0),width-1);
						offset = vy*pitch+vx*byte;

						BYTE* pixel = src + offset;
						int * result = &filter[j][i][0];

						nr += result[pixel[2]];
						ng += result[pixel[1]];
						nb += result[pixel[0]];
					}
				}

				offset = y*pitch+x*byte;
				buff[offset+2] = convert[TOINT(nr) + 0x1000];
				buff[offset+1] = convert[TOINT(ng) + 0x1000];
				buff[offset+0] = convert[TOINT(nb) + 0x1000];
			}
		}
		if(iterations > 1 && loop + 1 < iterations)
			memcpy(src,buff,size);
	}

	TargetImg->UnlockBuffer(buff);
	free(src);

	if Current(rdPtr->targetId)
		ImageChanged();
	

	return 0;
}
ACTION(
	/* ID */			62,
	/* Name */			_T("Blit the background"),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();

	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	LPSURFACE ps = WinGetSurface((int)rhPtr->rhIdEditWin);
	if(!ps) return 0;

	Blit(ps,TargetImg,rdPtr);

	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}
ACTION(
	/* ID */			63,
	/* Name */			_T("Blit image %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Source"))
) {
	TargetExists();
	int source = GetInt();
	ImageExists(source);
	
	Blit(Image(source),TargetImg,rdPtr);

	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}

ACTION(
	/* ID */			64,
	/* Name */			_T("Add backdrop %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_PASTE,_T("Options"))
) {
	TargetExists();

	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	mv* mV = rdPtr->rHo.hoAdRunHeader->rh4.rh4Mv;
	int type = ((LPEVP)param1)->evp.evpW.evpW0;
	mV->mvAddBackdrop(TargetImg,rdPtr->b.destX,rdPtr->b.destY,rdPtr->b.operation,rdPtr->b.param,type,rdPtr->rHo.hoLayer);
	return 0;
}
ACTION(
	/* ID */			65,
	/* Name */			_T("Blit onto the background"),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();

	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	LPSURFACE ps = WinGetSurface((int)rhPtr->rhIdEditWin); //rdPtr->rHo.hoAdRunHeader->rhFrame->m_pSurface
	if(!ps) return 0;

	Blit(TargetImg,ps,rdPtr);

	int w;
	int h;
	if(rdPtr->b.StretchMode)
	{
		w = rdPtr->b.destW;
		h = rdPtr->b.destH;
	}
	else
	{
		if(rdPtr->b.srcUse)
		{
			w = rdPtr->b.srcW;
			h = rdPtr->b.srcH;
		}
		else
		{
			w = TargetImg->GetWidth();
			h = TargetImg->GetHeight();
		}
	}
	RECT rect = {rdPtr->b.destX,rdPtr->b.destY,rdPtr->b.destX+w,rdPtr->b.destY+h};
	WinAddZone(rhPtr->rhIdEditWin,&rect);
	return 0;
}
ACTION(
	/* ID */			66,
	/* Name */			_T("Set blit destination dimensions to (%0,%1)"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Destination width"),PARAM_NUMBER,_T("Destination height"))
) {
	rdPtr->b.destW = GetInt();
	rdPtr->b.destH = GetInt();
	return 0;
}
ACTION(
	/* ID */			67,
	/* Name */			_T("Blit image %0's alpha channel"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Source"))
) {
	TargetExists();
	int source = GetInt();
	ImageExists(source);
	cSurface* tmp = Image(source);
	if(!tmp->HasAlpha()) return 0;
	
	cSurface* alpha = tmp->GetAlphaSurface();
	Blit(alpha,TargetImg,rdPtr);
	tmp->ReleaseAlphaSurface(alpha);

	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}
ACTION(
	/* ID */			68,
	/* Name */			_T("Export image as Overlay"),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();
	cSurface* img = TargetImg;

	if(rdPtr->ovl)
		delete rdPtr->ovl;

	rdPtr->ovl = new Overlay;
	rdPtr->ovl->rHo = rdPtr->rHo;
	rdPtr->ovl->rv = rdPtr->rv;
	rdPtr->ovl->runSurface = img;
	rdPtr->ovl->X = rdPtr->rHo.hoX;
	rdPtr->ovl->Y = rdPtr->rHo.hoY;
	rdPtr->ovl->OldX = rdPtr->rHo.hoX;
	rdPtr->ovl->OldY = rdPtr->rHo.hoY;
	rdPtr->ovl->swidth = img->GetWidth();
	rdPtr->ovl->sheight = img->GetHeight();
	rdPtr->ovl->oldwidth = rdPtr->ovl->swidth;
	rdPtr->ovl->oldheight = rdPtr->ovl->sheight;
	rdPtr->ovl->rFilename[0] = 0;
	rdPtr->ovl->debugmode = FALSE;
	rdPtr->ovl->stretching = FALSE;
	rdPtr->ovl->AbsolutePos = rdPtr->useAbs;
	rdPtr->ovl->Immediate = TRUE;
	rdPtr->ovl->Automatic = FALSE;
	rdPtr->ovl->AntiA = FALSE;
	rdPtr->ovl->Mode = (rdPtr->rs.rsEffect&EFFECTFLAG_TRANSPARENT)?BMODE_TRANSP:BMODE_OPAQUE;
	rdPtr->ovl->Effect = (BlitOp)(rdPtr->rs.rsEffect&EFFECT_MASK);
	rdPtr->ovl->EffectParam = rdPtr->rs.rsEffectParam;
	rdPtr->ovl->Points = 0;
	rdPtr->ovl->NumPoints = 0;
	rdPtr->ovl->SizePoints = 0;
	rdPtr->ovl->RemovePoints = !rdPtr->keepPoints;
	rdPtr->ovl->Fade = FALSE;
	rdPtr->ovl->FadeSpeed = 0.0f;
	rdPtr->ovl->FadeInk = 0.0f;
	//Image index (for pointer validation)
	rdPtr->ovl->image = rdPtr->targetId;
	return 0;
}
ACTION(
	/* ID */			69,
	/* Name */			_T("Draw line (%0,%1,%2,%3) with pattern %4 and thickness %5"),
	/* Flags */			0,
	/* Params */		(6,PARAM_NUMBER,_T("X1"),PARAM_NUMBER,_T("Y1"),PARAM_NUMBER,_T("X2"),PARAM_NUMBER,_T("Y2"),PARAM_STRING,_T("Pattern"),PARAM_NUMBER,_T("Thickness"))
) {
	TargetExists();
	int x1 = GetXPos();
	int y1 = GetYPos();
	int x2 = GetXPos();
	int y2 = GetYPos();
	string fill = string(GetStr());
	int thick = GetInt();

	FillGuide* fillData=0;

	//No pattern
	if(!fill[0] || thick <= 0)
		return 0;

	//rdPtr->userXOff = min(x1,x2);
	//rdPtr->userYOff = min(y1,y2);

	//Pattern must exist
	if(rdPtr->fill->find(fill)!=rdPtr->fill->end())
	{
		fillData = rdPtr->fill->find(fill)->second;
		//If mosaic, check if image exists
		if(fillData->type&FILL_IMAGE)
			ImageExists(fillData->param);
	}
	else
		return 0;

	TargetImg->Line(x1,y1,x2,y2,
	thick,fillData->fill, //Fill
	USE_AA,rdPtr->b.mode,rdPtr->b.operation,rdPtr->b.param); //Params

	RectChanged(x1,y1,x2,y2);
	
	return 0;
}
ACTION(
	/* ID */			70,
	/* Name */			_T("Blit Overlay at %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Overlay address"))
) {
	TargetExists();
	Overlay* ovl = (Overlay*)GetInt();
	if(!ovl)
		return 0;
	
	Blit(ovl->runSurface,TargetImg,rdPtr);

	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}
ACTION(
	/* ID */			71,
	/* Name */			_T("Blit onto Overlay at %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Overlay address"))
) {
	TargetExists();
	Overlay* ovl = (Overlay*)GetInt();
	if(!ovl)
		return 0;
	
	Blit(TargetImg,ovl->runSurface,rdPtr);
	return 0;
}
ACTION(
	/* ID */			72,
	/* Name */			_T("Set color of pattern %0 to %1"),
	/* Flags */			0,
	/* Params */		(2,PARAM_STRING,_T("Pattern name"),PARAM_COLOUR,_T("Color"))
) {
	string fill = string(GetStr());
	COLORREF color = GetCol();
	if(!fill[0]) return 0;

	if(rdPtr->fill->find(fill)!=rdPtr->fill->end())
	{
		FillGuide* fillData = rdPtr->fill->find(fill)->second;

		if(fillData->type==FILL_COLOR)
			((CFillFlat*)fillData->fill)->SetColor(color);
		else if(fillData->type==FILL_GRADIENT)
			((CFillDir*)fillData->fill)->SetColors(color,color);
		else if(fillData->type==FILL_RADIAL)
			((CFillRadial*)fillData->fill)->SetColors(color,color);
	}

	return 0;
}
ACTION(
	/* ID */			73,
	/* Name */			_T("Set colors of pattern %0 to (%1,%2)"),
	/* Flags */			0,
	/* Params */		(3,PARAM_STRING,_T("Pattern name"),PARAM_COLOUR,_T("Color A"),PARAM_COLOUR,_T("Color B"))
) {
	string fill = string(GetStr());
	if(!fill[0]) return 0;
	COLORREF a = GetCol();
	COLORREF b = GetCol();

	if(rdPtr->fill->find(fill)!=rdPtr->fill->end())
	{
		FillGuide* fillData = rdPtr->fill->find(fill)->second;

		if(fillData->type==FILL_GRADIENT)
			((CFillDir*)fillData->fill)->SetColors(a,b);
		else if(fillData->type==FILL_RADIAL)
			((CFillRadial*)fillData->fill)->SetColors(a,b);
		else if(fillData->type==FILL_COLOR)
			((CFillFlat*)fillData->fill)->SetColor(a);
	}

	return 0;
}
ACTION(
	/* ID */			74,
	/* Name */			_T("Set vertical flag of pattern %0 to %1"),
	/* Flags */			0,
	/* Params */		(2,PARAM_STRING,_T("Pattern name"),PARAM_NUMBER,_T("Vertical? (0: No, 1: Yes)"))
) {
	string fill = string(GetStr());
	if(!fill[0]) return 0;
	int vert = GetInt();

	if(rdPtr->fill->find(fill)!=rdPtr->fill->end())
	{
		FillGuide* fillData = rdPtr->fill->find(fill)->second;

		if(fillData->type==FILL_GRADIENT)
			((CFillDir*)fillData->fill)->SetDir(vert);
	}

	return 0;
}
ACTION(
	/* ID */			75,
	/* Name */			_T("Set origin of pattern %0 to (%1,%2)"),
	/* Flags */			0,
	/* Params */		(3,PARAM_STRING,_T("Pattern name"),PARAM_NUMBER,_T("X Origin"),PARAM_NUMBER,_T("Y Origin"))
) {
	string fill = string(GetStr());
	if(!fill[0]) return 0;
	int x = GetInt();
	int y = GetInt();

	if(rdPtr->fill->find(fill)!=rdPtr->fill->end())
	{
		FillGuide* fillData = rdPtr->fill->find(fill)->second;
		if(fillData->type==FILL_MOSAIC)
			((CFillMosaic*)fillData->fill)->SetFillOrg(x,y);
	}

	return 0;
}
ACTION(
	/* ID */			76,
	/* Name */			_T("Set image of pattern %0 to %1"),
	/* Flags */			0,
	/* Params */		(2,PARAM_STRING,_T("Pattern name"),PARAM_NUMBER,_T("Image"))
) {
	string fill = string(GetStr());
	if(!fill[0]) return 0;
	int id = GetInt();
	ImageExists(id);

	if(rdPtr->fill->find(fill)!=rdPtr->fill->end())
	{
		FillGuide* fillData = rdPtr->fill->find(fill)->second;
		//If mosaic...
		if(fillData->type==FILL_MOSAIC)
		{
			fillData->param = id;
			((CFillMosaic*)fillData->fill)->SetTile(Image(id));
		}
	}

	return 0;
}
ACTION(
	/* ID */			77,
	/* Name */			_T("Delete pattern %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_STRING,_T("Pattern name"))
) {
	string fill = string(GetStr());
	if(!fill[0]) return 0;
	if(rdPtr->fill->find(fill)!=rdPtr->fill->end())
	{
		FillGuide* fillData = rdPtr->fill->find(fill)->second;
		delete fillData->fill;
		delete fillData;
		rdPtr->fill->erase(fill);
	}

	return 0;
}
ACTION(
	/* ID */			78,
	/* Name */			_T("Resize canvas to (%0,%1,%2,%3)"),
	/* Flags */			0,
	/* Params */		(4,PARAM_NUMBER,_T("X1"),PARAM_NUMBER,_T("Y1"),PARAM_NUMBER,_T("X2"),PARAM_NUMBER,_T("Y2"))
) {
	TargetExists();
	int x1 = GetXPos();
	int y1 = GetYPos();
	int x2 = GetXPos();
	int y2 = GetYPos();
	//Delete
	if(!(x1-x2)||!(y1-y2))
	{
        TargetImg->Delete();
        rdPtr->targetValid = false;
        if Current(rdPtr->targetId)
        {
                rdPtr->rHo.hoImgWidth = 0;
                rdPtr->rHo.hoImgHeight = 0;
				rdPtr->rHo.hoImgXSpot = 0;
				rdPtr->rHo.hoImgYSpot = 0;
                ImageChanged();
        }
		
		return 0;
	}
	//Create temp
	cSurface tmp;
	tmp.Clone(*TargetImg);
	//Resize target
	cSurface* proto = 0;
	GetSurfacePrototype(&proto,rdPtr->depth,TargetImg->GetType(),TargetImg->GetDriver());
	COLORREF transColor = TargetImg->GetTransparentColor();
	TargetImg->Delete();
	TargetImg->Create(x2-x1,y2-y1,proto);
	TargetImg->Fill(transColor);
	TargetImg->SetTransparentColor(transColor);
	//Blit temp image
	tmp.Blit(*TargetImg,max(-x1,0),max(-y1,0),max(x1,0),max(y1,0),x2-x1,y2-y1,BMODE_OPAQUE,BOP_COPY,0,BLTF_COPYALPHA);

	if Current(rdPtr->targetId)
	{
		UpdateHotspot(rdPtr);
		ImageChanged();
	}
	
	return 0;
}
ACTION(
	/* ID */			79,
	/* Name */			_T("Clear with pattern %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_STRING,_T("Pattern"))
) {
	string fill = string(GetStr()); 
	if(!fill[0]) return 0;

	FillGuide* fillData,*outData;
	bool useFill=true, useOut=false;

	//rdPtr->userXOff = 0;
	//rdPtr->userYOff = 0;

	if(!ProcessFillFlags(rdPtr,fill,_T(""),useFill,useOut,fillData,outData))
		return 0;

	if(useFill)
	{
		TargetImg->Fill(fillData->fill);
		if Current(rdPtr->targetId)
		ImageChanged();
		
	}
	return 0;
}
ACTION(
	/* ID */			80,
	/* Name */			_T("Rotate image by %0 degrees"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Angle (Degrees)"))
) {
	TargetExists();
	//Create temp surface and Blit the current one onto it
	cSurface tmp;
	tmp.Clone(*TargetImg);
	COLORREF transp = TargetImg->GetTransparentColor();
	TargetImg->Delete();
	tmp.CreateRotatedSurface(*TargetImg,param1,rdPtr->rs.rsEffect&EFFECTFLAG_ANTIALIAS,transp);
	TargetImg->SetTransparentColor(transp);
	if Current(rdPtr->targetId)
	{
		UpdateHotspot(rdPtr);
		ImageChanged();
	}
	
	return 0;
}
ACTION(
	/* ID */			81,
	/* Name */			_T("Set linear resampling to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Use linear resampling in actions if available? (0: No, 1: Yes)"))
) {
	if(param1)
		rdPtr->rs.rsEffect |= EFFECTFLAG_ANTIALIAS;
	else
		rdPtr->rs.rsEffect &= ~EFFECTFLAG_ANTIALIAS;
	return 0;
}
ACTION(
	/* ID */			82,
	/* Name */			_T("Blit image referenced at %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Reference value"))
) {
	TargetExists();
	cSurface* surf = (cSurface*)GetInt();
	if(!surf)
		return 0;
	
	Blit(surf,TargetImg,rdPtr);

	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}
ACTION(
	/* ID */			83,
	/* Name */			_T("Blit onto image referenced at %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Reference value"))
) {
	TargetExists();
	cSurface* surf = (cSurface*)GetInt();
	if(!surf)
		return 0;
	
	Blit(TargetImg,surf,rdPtr);
	return 0;
}
ACTION(
	/* ID */			84,
	/* Name */			_T("Set text clipping to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Clipping (0: No, 1: Yes)"))
) {
	if(param1)
		rdPtr->hFlags &= ~DT_NOCLIP;
	else
		rdPtr->hFlags |= DT_NOCLIP;
	return 0;
}
ACTION(
	/* ID */			85,
	/* Name */			_T("Set text add ellipsis to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Add ellipsis (0: No, 1: Yes)"))
) {
	if(param1)
		rdPtr->hFlags |= DT_WORD_ELLIPSIS;
	else
		rdPtr->hFlags &= ~DT_WORD_ELLIPSIS;
	return 0;
}
ACTION(
	/* ID */			86,
	/* Name */			_T("Set text word break to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Word break (0: No, 1: Yes)"))
) {
	if(param1)
		rdPtr->hFlags |= DT_WORDBREAK;
	else
		rdPtr->hFlags &= ~DT_WORDBREAK;
	return 0;
}
ACTION(
	/* ID */			87,
	/* Name */			_T("Blit window at %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Window handle (0: Whole desktop)"))
) {
	TargetExists();
	HWND hwnd = (HWND)GetInt();
	RECT rect = {0,0,0,0};
	HDC hdc = GetDC(hwnd);
	//Desktop
	if(!hwnd)
	{
		rect.right = GetSystemMetrics(SM_CXSCREEN);
		rect.bottom = GetSystemMetrics(SM_CYSCREEN);
	}
	//HWND
	else if(!GetWindowRect(hwnd,&rect))
		return 0;

	int sx=rdPtr->b.srcX,sy=rdPtr->b.srcY,sw=rdPtr->b.srcW,sh=rdPtr->b.srcH;
	int dx=rdPtr->b.destX,dy=rdPtr->b.destY,dw=rdPtr->b.destW,dh=rdPtr->b.destH;
	
	//Complete window
	if(!rdPtr->b.srcUse)
	{
		sx = 0;
		sy = 0;
		sw = rect.right-rect.left;
		sh = rect.bottom-rect.top;
	}

	//Don't stretch
	if(!rdPtr->b.StretchMode)
	{
		dw = sw;
		dh = sh;
	}

	//Blit mode
	DWORD effect = SRCCOPY;
	switch(rdPtr->b.operation)
	{
	case BOP_INVERT:
		effect = NOTSRCCOPY;
	case BOP_XOR:
		effect = SRCINVERT;
	case BOP_AND:
		effect = SRCAND;
	case BOP_OR:
		effect = SRCPAINT;
	}

	//Do it
	HDC sdc = TargetImg->GetDC();
	//Don't stretch
	if((!rdPtr->b.StretchMode&&BitBlt(sdc,dx,dy,sw,sh,hdc,sx,sy,effect))
	//Stretch
	||(rdPtr->b.StretchMode&&StretchBlt(sdc,dx,dy,dw,dh,hdc,sx,sy,sw,sh,effect)))
	{
		if Current(rdPtr->targetId)
			ImageChanged();
		
	}
	TargetImg->ReleaseDC(sdc);
	ReleaseDC(hwnd,hdc);
	return 0;
}
ACTION(
	/* ID */			88,
	/* Name */			_T("Blit onto window at %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Window handle (0: Whole desktop)"))
) {
	TargetExists();
	HWND hwnd = (HWND)GetInt();
	RECT rect = {0,0,0,0};
	HDC hdc = GetDC(hwnd);
	//Desktop
	if(!hwnd)
	{
		rect.right = GetSystemMetrics(SM_CXSCREEN);
		rect.bottom = GetSystemMetrics(SM_CYSCREEN);
	}
	//HWND
	else if(!GetWindowRect(hwnd,&rect))
		return 0;

	int sx=rdPtr->b.srcX,sy=rdPtr->b.srcY,sw=rdPtr->b.srcW,sh=rdPtr->b.srcH;
	int dx=rdPtr->b.destX,dy=rdPtr->b.destY,dw=rdPtr->b.destW,dh=rdPtr->b.destH;
	
	//Complete window
	if(!rdPtr->b.srcUse)
	{
		sx = 0;
		sy = 0;
		sw = rect.right-rect.left;
		sh = rect.bottom-rect.top;
	}

	//Don't stretch
	if(!rdPtr->b.StretchMode)
	{
		dw = sw;
		dh = sh;
	}

	//Blit mode
	DWORD effect = SRCCOPY;
	switch(rdPtr->b.operation)
	{
	case BOP_INVERT:
		effect = NOTSRCCOPY;
	case BOP_XOR:
		effect = SRCINVERT;
	case BOP_AND:
		effect = SRCAND;
	case BOP_OR:
		effect = SRCPAINT;
	}

	//Do it
	HDC sdc = TargetImg->GetDC();
	if(Current(rdPtr->targetId))
	{
		//Don't stretch
		if(!rdPtr->b.StretchMode)
			BitBlt(hdc,dx,dy,sw,sh,sdc,sx,sy,effect);
		//Stretch
		else
			StretchBlt(hdc,dx,dy,dw,dh,sdc,sx,sy,sw,sh,effect);
	}
	TargetImg->ReleaseDC(sdc);
	ReleaseDC(hwnd,hdc);
	return 0;
}
ACTION(
	/* ID */			89,
	/* Name */			_T("Blit onto image %1 of Surface %0"),
	/* Flags */			0,
	/* Params */		(2,PARAM_OBJECT,_T("Surface object"),PARAM_NUMBER,_T("Image"))
) {
	TargetExists();
	RUNDATA* obj = (RUNDATA*)GetParam();
	if(!obj||obj->rHo.hoIdentifier!=IDENTIFIER)
		return 0;
	int img = GetInt();
	if(img<0||img>=obj->surf->size()||!obj->surf->at(img)->surf->IsValid())
		return 0;
	
	Blit(TargetImg,obj->surf->at(img)->surf,rdPtr);
	obj->rc.rcChanged = true;

	return 0;
}
ACTION(
	/* ID */			90,
	/* Name */			_T("Blit image %1 of Surface %0"),
	/* Flags */			0,
	/* Params */		(2,PARAM_OBJECT,_T("Surface object"),PARAM_NUMBER,_T("Image"))
) {
	TargetExists();
	RUNDATA* obj = (RUNDATA*)GetParam();
	if(!obj||obj->rHo.hoIdentifier!=IDENTIFIER)
		return 0;
	int img = GetInt();
	if(img<0||img>=obj->surf->size()||!obj->surf->at(img)->surf->IsValid())
		return 0;
	
	Blit(obj->surf->at(img)->surf,TargetImg,rdPtr);

	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}
ACTION(
	/* ID */			91,
	/* Name */			_T("Swap images %0 and %1"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Image A"),PARAM_NUMBER,_T("Image B"))
) {
	ImageExists(param1);
	ImageExists(param2);
	
	swap(Image(param1),Image(param2));

	if(Current(param1)||Current(param2))
		ImageChanged();
	
	return 0;
}
ACTION(
	/* ID */			92,
	/* Name */			_T("Move channels (r,g,b,a) to (%0,%1,%2,%3)"),
	/* Flags */			0,
	/* Params */		(4,PARAM_STRING,_T("New channel for red (r, g, b, a)"),PARAM_STRING,_T("New channel for green (r, g, b, a)"),PARAM_STRING,_T("New channel for blue (r, g, b, a)"),PARAM_STRING,_T("New channel for alpha, if any (r, g, b, a)"))
) {
	TargetExists();
	int nr = *GetStr();
	int ng = *GetStr();
	int nb = *GetStr();
	int na = *GetStr();
	//Old values
	BYTE or,og,ob;
	COLORREF newc;

	int width = TargetImg->GetWidth();
	int height = TargetImg->GetHeight();

	//Color channel
	BYTE* buff;
	buff = TargetImg->LockBuffer();
	if(!buff) return 0;

	int pitch = TargetImg->GetPitch();
	if(pitch < 0)
	{
		pitch *= -1;
		buff -= pitch*(height-1);
	}
	int size = pitch*height;
	int byte = TargetImg->GetDepth()>>3;

	//Alpha channel
	bool hasAlpha = TargetImg->HasAlpha();
	BYTE* abuff = 0;
	if(hasAlpha)
		abuff = TargetImg->LockAlpha();
	int apitch = TargetImg->GetAlphaPitch();

	//Move channels
	for(int y=0;y<height;y++)
	{
		for(int x=0;x<width;x++)
		{
			//Calculate buffer position, get old color
			BYTE* p = buff+y*pitch+x*byte, *ap;
			newc = 0;
			or = p[2];
			og = p[1];
			ob = p[0];
			if(abuff)
				ap = abuff+(height-y-1)*apitch+x;
			//Update colors
			switch(nr) //Red	
			{
				case 'r': p[2] = or; break;
				case 'g': p[2] = og; break;
				case 'b': p[2] = ob; break;
				case 'a': p[2] = *ap; break;
			}
			switch(ng) //Green
			{
				case 'r': p[1] = or; break;
				case 'g': p[1] = og; break;
				case 'b': p[1] = ob; break;
				case 'a': p[1] = *ap; break;
			}
			switch(nb) //Blue
			{
				case 'r': p[0] = or; break;
				case 'g': p[0] = og; break;
				case 'b': p[0] = ob; break;
				case 'a': p[0] = *ap; break;
			}
			if(abuff) //Alpha
			{
				switch(na)
				{
					case 'r': *ap = or; break;
					case 'g': *ap = og; break;
					case 'b': *ap = ob; break;
				}
			}
		}
	}

	TargetImg->UnlockBuffer(buff);
	if(hasAlpha)
		TargetImg->UnlockAlpha();

	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}
ACTION(
	/* ID */			93,
	/* Name */			_T("Scroll (%0,%1), wrap = %2"),
	/* Flags */			0,
	/* Params */		(3,PARAM_NUMBER,_T("X Offset"),PARAM_NUMBER,_T("Y Offset"),PARAM_NUMBER,_T("Wrap pixels? (0: No, 1: Yes)"))
) {
	TargetExists();
	int ox = GetInt();
	int oy = GetInt();
	bool wrap = GetInt();

	//Clone image temporarily
	cSurface src;
	src.Clone(*TargetImg);
	TargetImg->Fill(TargetImg->GetTransparentColor());
	if(src.HasAlpha())
	{
		cSurface* alpha = TargetImg->GetAlphaSurface();
		alpha->Fill(0);
		TargetImg->ReleaseAlphaSurface(alpha);
	}

	int w = src.GetWidth(), h = src.GetHeight();
	//Wrap offset
	ox %= w;
	oy %= h;

	src.Blit(*TargetImg,ox,oy,BMODE_OPAQUE,BOP_COPY,0,BLTF_COPYALPHA);
	if(wrap)
	{
		if(ox>0)
			src.Blit(*TargetImg,ox-w,oy,BMODE_OPAQUE,BOP_COPY,0,BLTF_COPYALPHA);
		if(ox>0&&oy>0)
			src.Blit(*TargetImg,ox-w,oy-h,BMODE_OPAQUE,BOP_COPY,0,BLTF_COPYALPHA);
		if(oy>0)
			src.Blit(*TargetImg,ox,oy-h,BMODE_OPAQUE,BOP_COPY,0,BLTF_COPYALPHA);
		if(ox<0&&oy>0)
			src.Blit(*TargetImg,ox+w,oy-h,BMODE_OPAQUE,BOP_COPY,0,BLTF_COPYALPHA);
		if(ox<0)
			src.Blit(*TargetImg,ox+w,oy,BMODE_OPAQUE,BOP_COPY,0,BLTF_COPYALPHA);
		if(ox<0&&oy<0)
			src.Blit(*TargetImg,ox+w,oy+h,BMODE_OPAQUE,BOP_COPY,0,BLTF_COPYALPHA);
		if(oy<0)
			src.Blit(*TargetImg,ox,oy+h,BMODE_OPAQUE,BOP_COPY,0,BLTF_COPYALPHA);
		if(ox>0&&oy<0)
			src.Blit(*TargetImg,ox-w,oy+h,BMODE_OPAQUE,BOP_COPY,0,BLTF_COPYALPHA);
	}
	
	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}
ACTION(
	/* ID */			94,
	/* Name */			_T("Return %0 to callback"),
	/* Flags */			0,
	/* Params */		(1,PARAM_COLOUR,_T("Color"))
) {
	if(!rdPtr->colRet)
	{
		rdPtr->colNew = param1;
		rdPtr->colRet = true;
	}
	return 0;
}
//ACTION(
//	/* ID */			94,
//	/* Name */			_T("Return %0 to fill callback"),
//	/* Flags */			0,
//	/* Params */		(1,PARAM_COLOUR,_T("Color"))
//) {
//	rdPtr->userTrigger = true;
//	rdPtr->userCol = param1;
//	return 0;
//}

//ACTION(
//	/* ID */			95,
//	/* Name */			_T("Create user callback pattern %0"),
//	/* Flags */			0,
//	/* Params */		(1,PARAM_STRING,_T("Pattern name"))
//) {
//	string str = string(GetStr());
//	if(!str[0]) return 0;
//
//	FillGuide* guide = new FillGuide;
//	guide->type = FILL_USER;
//	CFillUser* fill = new CFillUser(rdPtr,str.c_str());
//	guide->fill = fill;
//	guide->img = false;
//
//	rdPtr->fill->erase(str);
//	rdPtr->fill->insert(pair<string,FillGuide*>(str,guide));
//	return 0;
//}
ACTION(
	/* ID */			95,
	/* Name */			_T("Loop through image with callback %1 and flags %0"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Flags, add together (1: Get color, 2: Set color, 4: Store position, 8: Get alpha, 16: Set alpha)"),PARAM_STRING,_T("Callback"))
) {
	TargetExists();

	//Flags
	bool doRead = param1&1;
	bool doWrite = param1&2;
	bool doXY = param1&4;
	bool doReadA = param1&8;
	bool doWriteA = param1&16;
	//Set callback
	rdPtr->callback = (TCHAR*)param2;

	int width = TargetImg->GetWidth();
	int height = TargetImg->GetHeight();

	//Get GenerateEvent lookup
	long ( WINAPI * routine)(headerObject*, WPARAM, LPARAM);
	routine = ((LPRH)rdPtr->rHo.hoAdRunHeader)->rh4.rh4KpxFunctions[RFUNCTION_GENERATEEVENT].routine;

	//Color channel
	BYTE* buff;
	buff = TargetImg->LockBuffer();
	if(!buff) return 0;

	int pitch = TargetImg->GetPitch();
	if(pitch < 0)
	{
		pitch *= -1;
		buff -= pitch*(height-1);
	}
	int size = pitch*height;
	int byte = TargetImg->GetDepth()>>3;

	//Pointer to source variable
	BYTE* colSrc = (BYTE*)&rdPtr->colSrc;
	BYTE* dest = (BYTE*)&rdPtr->colNew;

	BYTE* end = buff+size;

	//Get the alpha channel
	cSurface* alphaSurf = 0;
	if((doReadA || doWriteA) && TargetImg->HasAlpha())
		alphaSurf = TargetImg->GetAlphaSurface();
	if(!alphaSurf)
		doWriteA = doReadA = false;

	//For getting the pixel position
	rdPtr->callX = 0;
	rdPtr->callY = height-1;

	for(BYTE* y=buff; y<end ;y+=pitch)
	{
		BYTE* nextline = y+pitch;
		for(BYTE* x=y; x<nextline; x+=byte)
		{
			//Get source color
			if(doRead)
			{
				colSrc[0] = x[2];
				colSrc[1] = x[1];
				colSrc[2] = x[0];
				rdPtr->colNew = rdPtr->colSrc;
			}
			//Get source alpha
			if(doReadA)
			{
				rdPtr->colAlphaSrc = alphaSurf->GetPixelFast8(rdPtr->callX,rdPtr->callY);
			}
			//Callback
			rdPtr->colRet = false;
			rdPtr->colAlphaRet = false;
			routine((LPHO)rdPtr,8,0);
			//Write new color
			if(doWrite&&rdPtr->colRet)
			{
				x[0] = dest[2];
				x[1] = dest[1];
				x[2] = dest[0];
			}
			//Write new alpha
			if(doWriteA&&rdPtr->colAlphaRet)
			{
				alphaSurf->SetPixelFast8(rdPtr->callX,rdPtr->callY,rdPtr->colAlphaNew);
			}

			//Update position
			if(doXY)
				++rdPtr->callX;
		}
		//Update position
		if(doXY)
		{
			rdPtr->callX = 0;
			--rdPtr->callY;
		}
	}

	//Release the alpha channel
	if(alphaSurf)
		TargetImg->ReleaseAlphaSurface(alphaSurf);

	TargetImg->UnlockBuffer(buff);

	if Current(rdPtr->targetId)
		ImageChanged();
		
	return 0;
}
ACTION(
	/* ID */			96,
	/* Name */			_T("Set clipping rectangle to (%0,%1,%2,%3)"),
	/* Flags */			0,
	/* Params */		(4,PARAM_NUMBER,_T("X1"),PARAM_NUMBER,_T("Y1"),PARAM_NUMBER,_T("X2"),PARAM_NUMBER,_T("Y2"))
) {
	TargetExists();
	int x1 = GetXPos();
	int y1 = GetYPos();
	int x2 = GetXPos();
	int y2 = GetYPos();
	TargetImg->SetClipRect(x1,y1,x2,y2);
	return 0;
}
ACTION(
	/* ID */			97,
	/* Name */			_T("Clear clipping rectangle"),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();
	TargetImg->ClearClipRect();
	return 0;
}
ACTION(
	/* ID */			98,
	/* Name */			_T("Invert image"),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();
	int width = TargetImg->GetWidth();
	int height = TargetImg->GetHeight();

	//Color channel
	BYTE* buff;
	buff = TargetImg->LockBuffer();
	if(!buff) return 0;

	int pitch = TargetImg->GetPitch();
	if(pitch < 0)
	{
		pitch *= -1;
		buff -= pitch*(height-1);
	}
	int size = pitch*height;
	int byte = TargetImg->GetDepth()>>3;

	BYTE* end = buff+size;

	for(BYTE* y=buff; y<end ;y+=pitch)
	{
		BYTE* nextline = y+pitch;
		for(BYTE* x=y; x<nextline; x+=byte)
		{
			x[0] = ~x[0];
			x[1] = ~x[1];
			x[2] = ~x[2];
		}
	}

	TargetImg->UnlockBuffer(buff);

	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}
ACTION(
	/* ID */			99,
	/* Name */			_T("Create star, inner radius = %0, outer radius = %1, spikes = %2"),
	/* Flags */			0,
	/* Params */		(3,PARAM_NUMBER,_T("Inner radius"),PARAM_NUMBER,_T("Outer radius"),PARAM_NUMBER,_T("Number of spikes"))
) {
	float inner,outer;
	LoadFloat(inner);
	LoadFloat(outer);
	if(!inner&&!outer) return 0;
	int spikes = GetInt();
	if(spikes<=0) return 0;

	PolyCoord p;
	float conv = 3.14159f/spikes;

	rdPtr->points->clear();
	for(int i=0;i<spikes*2+1;i+=2)
	{
		//Inner
		p.x = cos(i*conv)*inner;
		p.y = sin(i*conv)*inner;
		rdPtr->points->push_back(p);
		//Outer
		p.x = cos((i+1)*conv)*outer;
		p.y = sin((i+1)*conv)*outer;
		rdPtr->points->push_back(p);
	}


	return 0;
}
ACTION(
	/* ID */			100,
	/* Name */			_T("Scale all points by (%2,%3) from (%0,%1)"),
	/* Flags */			0,
	/* Params */		(4,PARAM_NUMBER,_T("Scale center X"),PARAM_NUMBER,_T("Scale center Y"),PARAM_NUMBER,_T("X scale factor (1.0: None)"),PARAM_NUMBER,_T("Y scale factor (1.0: None)"))
) {
	float ox,oy,sx,sy;
	LoadFloat(ox);
	LoadFloat(oy);
	LoadFloat(sx);
	LoadFloat(sy);

	PolyPoints::iterator it = rdPtr->points->begin(), end = rdPtr->points->end();
	while(it!=end)
	{
		(*it).x = ox+sx*((*it).x-ox);
		(*it).y = oy+sy*((*it).y-oy);
		++it;
	}

	return 0;
}
ACTION(
	/* ID */			101,
	/* Name */			_T("Draw ellipse at (%0,%1) with size (%2,%3) and color %4, outline thickness %5, outline pattern %6"),
	/* Flags */			0,
	/* Params */		(7,PARAM_NUMBER,_T("Center X"),PARAM_NUMBER,_T("Center Y"),PARAM_NUMBER,_T("Width"),PARAM_NUMBER,_T("Height"),PARAM_COLOUR,_T("Fill color (-1: None)"),PARAM_NUMBER,_T("Outline thickness"),PARAM_COLOUR,_T("Outline color"))
) {
	TargetExists();
	int x = GetXPos();
	int y = GetYPos();
	float w = GetInt()/2.0f; float h = GetInt()/2.0f;
	int x1 = x-floor(w); int y1 = y-floor(h); int x2 = x+ceil(w)+1; int y2 = y+ceil(h)+1;
	COLORREF fill = GetCol();
	int thick = GetInt();
	COLORREF out = GetCol();

	TargetImg->Ellipse(x1,y1,x2,y2,fill,thick,out,fill!=-1);

	if Current(rdPtr->targetId)
		RectChanged(x1,y1,x2,y2);
	
	return 0;
}
ACTION(
	/* ID */			102,
	/* Name */			_T("Draw ellipse at (%0,%1) with size (%2,%3) and pattern %4, outline thickness %5, outline pattern %6"),
	/* Flags */			0,
	/* Params */		(7,PARAM_NUMBER,_T("Center X"),PARAM_NUMBER,_T("Center Y"),PARAM_NUMBER,_T("Width"),PARAM_NUMBER,_T("Height"),PARAM_STRING,_T("Pattern (Empty: None)"),PARAM_NUMBER,_T("Outline thickness"),PARAM_STRING,_T("Outline pattern (Empty: None)"))
) {
	TargetExists();
	int x = GetXPos(); int y = GetYPos();
	float w = GetInt()/2.0f; float h = GetInt()/2.0f;
	int x1 = x-floor(w); int y1 = y-floor(h); int x2 = x+ceil(w)+1; int y2 = y+ceil(h)+1;
	string fill = string(GetStr());
	int thick = GetInt();
	string out = string(GetStr());

	FillGuide* fillData=0,* outData=0;
	bool useFill = fill[0]!=0;
	bool useOut = (thick!=0)&&(out[0]!=0);

	if(!ProcessFillFlags(rdPtr,fill,out,useFill,useOut,fillData,outData))
		return 0;

	//Final check: Outline or not?
	if(useOut)
	{
		TargetImg->Ellipse(x1,y1,x2,y2,
		useFill?fillData->fill:0,thick, //Fill
		outData->fill,false,rdPtr->b.mode,rdPtr->b.operation,rdPtr->b.param, //Outline+Params
		useFill); //Use fill
	}
	else if(useFill)
	{
		TargetImg->Ellipse(x1,y1,x2,y2,
		useFill?fillData->fill:0, //Fill
		USE_AA,rdPtr->b.mode,rdPtr->b.operation,rdPtr->b.param); //Params
	}
	
	if Current(rdPtr->targetId)
		RectChanged(x1,y1,x2,y2);
	
	return 0;
}
ACTION(
	/* ID */			103,
	/* Name */			_T("Create regular polygon, radius = %0, edges = %1"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Radius"),PARAM_NUMBER,_T("Number of edges"))
) {
	float radius;
	LoadFloat(radius);
	if(!radius) return 0;
	int edges = GetInt();
	if(edges<=0) return 0;

	PolyCoord p;
	float conv = 3.14159f/edges;

	rdPtr->points->clear();
	for(int i=0;i<edges+1;i++)
	{
		p.x = cos(i*conv*2)*radius;
		p.y = sin(i*conv*2)*radius;
		rdPtr->points->push_back(p);
	}


	return 0;
}
ACTION(
	/* ID */			104,
	/* Name */			_T("Skip redraw"),
	/* Flags */			0,
	/* Params */		(0)
) {
	rdPtr->rc.rcChanged = false; 
	return 0;
}

ACTION(
	/* ID */			105,
	/* Name */			_T("Set blit destination, position = (%0,%1), dimensions = (%2,%3), stretch mode = %4"),
	/* Flags */			0,
	/* Params */		(5,PARAM_NUMBER,_T("Destination X"),PARAM_NUMBER,_T("Destination Y"),
							PARAM_NUMBER,_T("Destination width"),PARAM_NUMBER,_T("Destination height"),
							PARAM_NUMBER,_T("Stretch mode (0: No stretching, 1: No resampling, 2: Resampling, 3: Opaque resampling)"))
) {
	rdPtr->b.destX = GetInt();
	rdPtr->b.destY = GetInt();
	rdPtr->b.destW = GetInt();
	rdPtr->b.destH = GetInt();
	rdPtr->b.StretchMode = GetInt();
	return 0;
}

//ACTION(
//	/* ID */			105,
//	/* Name */			_T("Export to OpenGL"),
//	/* Flags */			0,
//	/* Params */		(0)
//) {
//	//TargetExists();
//	////Load opengl32
//	//if(!rdPtr->openGL)
//	//{
//	//	rdPtr->openGL = LoadLibrary(_T("opengl32.dll"));
//	//}
//	////Library loaded?
//	//if(rdPtr->openGL)
//	//{	
//	//	//Load function
//	//	if(!rdPtr->glTexImage2D)
//	//		rdPtr->glTexImage2D = (GLTEX)GetProcAddress(rdPtr->openGL,_T("glTexImage2D"));
//
//	//	BYTE* alpha,*buffer;
//	//	int w = TargetImg->GetWidth();
//	//	int h = TargetImg->GetHeight();
//	//	bool hasAlpha = TargetImg->HasAlpha();
//	//	COLORREF transColor = TargetImg->GetTransparentColor();
//
//	//	cSurface tmp, *proto;
//	//	GetSurfacePrototype(&proto,32,ST_MEMORY,SD_DIB);
//	//	tmp.Create(w,h,proto);
//	//	TargetImg->Blit(tmp,0,0,BMODE_OPAQUE,BOP_COPY,0,BLTF_COPYALPHA);
//
//	//	//Lock buffer
//	//	buffer = tmp.LockBuffer();
//	//	if(!buffer) return 0;
//	//	//Lock alpha
//	//	if(hasAlpha)
//	//		alpha = tmp.LockAlpha();
//	//	//Locked OK
//	//	if(tmp.GetPitch()&&(!hasAlpha||(hasAlpha&&tmp.GetAlphaPitch())))
//	//	{
//	//		//Process transparency
//	//		for(int i=0;i<w*h;i++)
//	//		{
//	//			//Alpha channel
//	//			if(hasAlpha)
//	//			{
//	//				buffer[(i*4)+3] = alpha[i];
//	//			}
//	//			//Transparent color
//	//			else
//	//			{
//	//				buffer[(i*4)+3] = RGB(buffer[(i*4)+0],buffer[(i*4)+1],buffer[(i*4)+2])==transColor?0:255;
//	//			}
//	//		}
//	//		//Create texture
//	//		rdPtr->glTexImage2D(0x0DE1,0,4,w,h,0,0x80E1,0x1401,(void*)buffer);
//	//	}
//	//	tmp.UnlockBuffer(buffer);
//	//	if(hasAlpha)
//	//		tmp.UnlockAlpha();
//	//}
//	return 0;
//}

ACTION(
	/* ID */			106,
	/* Name */			_T("Convert to grayscale"),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();

	int width = TargetImg->GetWidth();
	int height = TargetImg->GetHeight();

	//Color channel
	BYTE* buff;
	buff = TargetImg->LockBuffer();
	if(!buff) return 0;

	int pitch = TargetImg->GetPitch();
	if(pitch < 0)
	{
		pitch *= -1;
		buff -= pitch*(height-1);
	}
	int size = pitch*height;
	int byte = TargetImg->GetDepth()>>3;

	BYTE* end = buff+size;
	for(BYTE* y=buff; y<end ;y+=pitch)
	{
		BYTE* nextline = y+pitch;
		for(BYTE* x=y; x<nextline; x+=byte)
		{
			int avg = (x[0]+x[1]+x[2])/3;
			x[0] = avg;
			x[1] = avg;
			x[2] = avg;
		}
	}

	TargetImg->UnlockBuffer(buff);

	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}
ACTION(
	/* ID */			107,
	/* Name */			_T("Create radial gradient pattern %0 with colors (%1,%2)"),
	/* Flags */			0,
	/* Params */		(3,PARAM_STRING,_T("Pattern name"),PARAM_COLOUR,_T("Color A"),PARAM_COLOUR,_T("Color B"))
) {
	string str = string(GetStr());
	if(!str[0]) return 0;
	COLORREF a = GetCol();
	COLORREF b = GetCol();

	FillGuide* guide = new FillGuide;
	guide->type = FILL_RADIAL;
	guide->img = false;
	CFillRadial* fill = new CFillRadial(a,b);
	guide->fill = fill;

	rdPtr->fill->erase(str);
	rdPtr->fill->insert(pair<string,FillGuide*>(str,guide));
	return 0;
}
ACTION(
	/* ID */			108,
	/* Name */			_T("Set blit effect to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_STRING,_T("Blit effect ('add', 'alpha', 'darken', 'difference', 'erase', 'hardlight', 'invert', 'layer', 'lighten', 'multiply', 'normal', 'overlay', 'screen', 'subtract')"))
) {
	//Available in flash only
	return 0;
}
ACTION(
	/* ID */			109,
	/* Name */			_T("Set display selected image to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Display selected image? (0: No, 1: Yes)"))
) {
	rdPtr->dispTarget = param1;
	return 0;
}

ACTION(
	/* ID */			110,
	/* Name */			_T("Set select new images to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Display selected image? (0: No, 1: Yes)"))
) {
	rdPtr->selectLast = param1;
	return 0;
}

ACTION(
	/* ID */			111,
	/* Name */			_T("Set transparent to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Display transparent? (0: No, 1: Yes)"))
) {
	if(param1)
		rdPtr->rs.rsEffect |= EFFECTFLAG_TRANSPARENT;
	else
		rdPtr->rs.rsEffect &= ~EFFECTFLAG_TRANSPARENT;
	return 0;
}

ACTION(
	/* ID */			112,
	/* Name */			_T("Lock buffer"),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();
	if(!rdPtr->buffer)
	{
		rdPtr->buffer = TargetImg->LockBuffer();
		rdPtr->buffPitch = TargetImg->GetPitch();
		rdPtr->buffSurf = TargetImg;
		if(rdPtr->buffPitch < 0)
		{
			rdPtr->buffPitch *= -1;
			rdPtr->buffer -= rdPtr->buffPitch*(TargetImg->GetHeight()-1);
		}
	}
	return 0;
}

ACTION(
	/* ID */			113,
	/* Name */			_T("Unlock buffer"),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();
	if(rdPtr->buffer&&rdPtr->buffPitch>0)
	{
		rdPtr->buffSurf->UnlockBuffer(rdPtr->buffer);
		rdPtr->buffSurf = 0;
		rdPtr->buffPitch = 0;
		rdPtr->buffer = 0;
		ImageChanged();
		
	}
	return 0;
}
ACTION(
	/* ID */			114,
	/* Name */			_T("Set blit source position to (%0,%1)"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Source X"),PARAM_NUMBER,_T("Source Y"))
) {
	rdPtr->b.srcX = GetInt();
	rdPtr->b.srcY = GetInt();
	return 0;
}
ACTION(
	/* ID */			115,
	/* Name */			_T("Set blit source dimensions to (%0,%1)"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Source width"),PARAM_NUMBER,_T("Source height"))
) {
	rdPtr->b.srcW = GetInt();
	rdPtr->b.srcH = GetInt();
	return 0;
}
ACTION(
	/* ID */			116,
	/* Name */			_T("Set blit stretch mode to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Stretch mode (0: No stretching, 1: No resampling, 2: Resampling, 3: Opaque resampling)"))
) {
	rdPtr->b.StretchMode = param1;
	return 0;
}
ACTION(
	/* ID */			117,
	/* Name */			_T("Set blit region flag to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Use specified source region? (0: No, 1: Yes)"))
) {
	rdPtr->b.srcUse = param1;
	return 0;
}
ACTION(
	/* ID */			118,
	/* Name */			_T("Set text angle to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Angle (Degrees)"))
) {
	float angle; LoadFloat(angle);

	if(rdPtr->hFont) DeleteObject(rdPtr->hFont);
	rdPtr->textFont.lfEscapement = angle*10;
	rdPtr->textFont.lfOrientation = rdPtr->textFont.lfEscapement;
	rdPtr->hFont = CreateFontIndirect(&rdPtr->textFont);

	return 0;
}
ACTION(
	/* ID */			119,
	/* Name */			_T("Draw rectangle at (%0,%1) with size (%2,%3) and color %4, outline thickness %5, outline color %6"),
	/* Flags */			0,
	/* Params */		(7,PARAM_NUMBER,_T("X"),PARAM_NUMBER,_T("Y"),PARAM_NUMBER,_T("Width"),PARAM_NUMBER,_T("Height"),PARAM_COLOUR,_T("Fill color (-1: None)"),PARAM_NUMBER,_T("Outline thickness"),PARAM_COLOUR,_T("Outline color"))
) {
	TargetExists();
	int x = GetXPos();
	int y = GetYPos();
	int w = GetInt();
	int h = GetInt();
	COLORREF fill = GetCol();
	int thick = GetInt();
	COLORREF out = GetCol();
	TargetImg->Rectangle(x,y,x+w,y+h,fill,thick,out,fill!=-1);

	if Current(rdPtr->targetId)
		RectChanged(x,y,x+w,y+h);
	
	return 0;
}
ACTION(
	/* ID */			120,
	/* Name */			_T("Draw rectangle at (%0,%1) with size (%2,%3) and pattern %4, outline thickness %5, outline color %6"),
	/* Flags */			0,
	/* Params */		(7,PARAM_NUMBER,_T("X"),PARAM_NUMBER,_T("Y"),PARAM_NUMBER,_T("Width"),PARAM_NUMBER,_T("Height"),PARAM_STRING,_T("Pattern (Empty: None)"),PARAM_NUMBER,_T("Outline thickness"),PARAM_STRING,_T("Outline pattern (Empty: None)"))
) {
	TargetExists();
	int x = GetXPos();
	int y = GetYPos();
	int w = GetInt();
	int h = GetInt();
	string fill = string(GetStr());
	int thick = GetInt();
	string out = string(GetStr());

	FillGuide* fillData=0,* outData=0;
	bool useFill = fill[0]!=0;
	bool useOut = (thick!=0)&&(out[0]!=0);

	if(!ProcessFillFlags(rdPtr,fill,out,useFill,useOut,fillData,outData))
		return 0;

	//Final check: Outline or not?
	if(useOut)
	{
		TargetImg->Rectangle(x,y,x+w,y+h,
		useFill?fillData->fill:0,thick, //Fill
		outData->fill,false,rdPtr->b.mode,rdPtr->b.operation,rdPtr->b.param, //Outline+Params
		useFill); //Use fill?
	}
	else if(useFill)
	{
		TargetImg->Rectangle(x,y,x+w,y+h,
		useFill?fillData->fill:0, //Fill
		USE_AA,rdPtr->b.mode,rdPtr->b.operation,rdPtr->b.param); //Params
	}

	if Current(rdPtr->targetId)
		RectChanged(x,y,x+w,y+h);
	
	return 0;
}
ACTION(
	/* ID */			121,
	/* Name */			_T("Copy image %0 from image %2 of Surface %1"),
	/* Flags */			0,
	/* Params */		(3,PARAM_NUMBER,_T("Destination image"),PARAM_OBJECT,_T("Surface object"),PARAM_NUMBER,_T("Source image"))
) {
	int img = GetInt();
	ImageInRange(img);
	RUNDATA* obj = (RUNDATA*)GetParam();
	if(!obj||obj->rHo.hoIdentifier!=IDENTIFIER)
		return 0;
	int src = GetInt();
	if(src<0||src>=obj->surf->size()||!obj->surf->at(src)->surf->IsValid())
		return 0;
	//Clone image
	if(Image(img)->IsValid()) Image(img)->Delete();
	Image(img)->Clone(*obj->surf->at(src)->surf);
	//Make image valid
	if(rdPtr->targetId==img)
	{
		TargetImg = Image(img);
		rdPtr->targetValid = true;
	}
	//Redraw
	if Current(img)
	{
		CurrentImg = Image(img);
		UpdateHotspot(rdPtr);
		ImageChanged();
	}
	
	return 0;
}
ACTION(
	/* ID */			122,
	/* Name */			_T("Blit Active object %0 at its position"),
	/* Flags */			0,
	/* Params */		(1,PARAM_OBJECT,_T("Object"))
) {
	TargetExists();
	LPRO obj = (LPRO)GetParam();
	if(!obj)
		return 0;

	int x = obj->roHo.hoX-obj->roHo.hoImgXSpot;
	int y = obj->roHo.hoY-obj->roHo.hoImgYSpot;
	if(rdPtr->useAbs)
	{
		x -= rdPtr->rHo.hoX;
		y -= rdPtr->rHo.hoY;
	}

	// Overwrite blit position temporarily
	int actualBlitX = rdPtr->b.destX;
	int actualBlitY = rdPtr->b.destY;
	rdPtr->b.destX = x;
	rdPtr->b.destY = y;

	cSurface is;
	rdPtr->rRd->GrabSurface(obj,is);
	Blit(&is,TargetImg,rdPtr);

	if Current(rdPtr->targetId)
		ImageChanged();
	
	rdPtr->b.destX = actualBlitX;
	rdPtr->b.destY = actualBlitY;
	
	return 0;
}


ACTION(
	/* ID */			123,
	/* Name */			_T("Perform %0 with %1 for channels %2"),
	/* Flags */			0,
	/* Params */		(3,PARAM_STRING,PARAM_OPERATOR,PARAM_NUMBER,_T("Operand"),PARAM_STRING,_T("Channels, add together (r, g, b, a)"))
) {
	TargetExists();
	char* op = (char*)GetStr();
	float val; LoadFloat(val);
	TCHAR* ch = GetStr();
	bool dor = _tcschr(ch,'r');
	bool dog = _tcschr(ch,'g');
	bool dob = _tcschr(ch,'b');
	bool doa = _tcschr(ch,'a');
	int width = TargetImg->GetWidth();
	int height = TargetImg->GetHeight();
	bool alpha = TargetImg->HasAlpha();

	//Prevent division by zero
	if((*op=='/'||*op=='%')&&!val)
		val = 0.001f;

	unsigned char valTable[256];
	for (int i = 0; i < 256; ++i)
	{
		valTable[i] = i;
		Operation(op,&valTable[i],val);
	}

	//Color channel
	BYTE* buff;
	buff = TargetImg->LockBuffer();
	if(!buff) return 0;

	int pitch = TargetImg->GetPitch();
	if(pitch < 0)
	{
		pitch *= -1;
		buff -= pitch*(height-1);
	}
	int size = pitch*height;
	int byte = TargetImg->GetDepth()>>3;

	BYTE* end = buff+size;
	for(BYTE* y=buff; y<end ;y+=pitch)
	{
		BYTE* nextline = y+pitch;
		for(BYTE* x=y; x<nextline; x+=byte)
		{
			if(dor) x[0] = valTable[x[0]];
			if(dog) x[1] = valTable[x[1]];
			if(dob) x[2] = valTable[x[2]];
		}
	}

	TargetImg->UnlockBuffer(buff);

	//Alpha channel
	if(doa&&alpha)
	{
		buff = TargetImg->LockAlpha();
		pitch = TargetImg->GetAlphaPitch();
		size = width*height;

		for(BYTE* i=buff;i<buff+size;i++)
			*i = valTable[*i];
		
		TargetImg->UnlockAlpha();
	}

	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}
ACTION(
	/* ID */			124,
	/* Name */			_T("Blit alpha channel onto image %0's alpha channel"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Target image"))
) {
	TargetExists();
	int target = GetInt();
	ImageExists(target);
	if(TargetImg->HasAlpha()&&Image(target)->HasAlpha())
	{
		cSurface* src = TargetImg->GetAlphaSurface(), *dest = Image(target)->GetAlphaSurface();
		Blit(src,dest,rdPtr);
		TargetImg->ReleaseAlphaSurface(src);
		Image(target)->ReleaseAlphaSurface(dest);
		if Current(target)
			ImageChanged();
		
	}
	return 0;
}
ACTION(
	/* ID */			125,
	/* Name */			_T("Blit image %0's alpha channel onto the alpha channel"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Source image"))
) {
	TargetExists();
	int target = GetInt();
	ImageExists(target);
	if(TargetImg->HasAlpha()&&Image(target)->HasAlpha())
	{
		cSurface* dest = TargetImg->GetAlphaSurface(), *src = Image(target)->GetAlphaSurface();
		Blit(src,dest,rdPtr);
		TargetImg->ReleaseAlphaSurface(dest);
		Image(target)->ReleaseAlphaSurface(src);
		if Current(target)
			ImageChanged();
		
	}
	return 0;
}
ACTION(
	/* ID */			126,
	/* Name */			_T("Set blit effect to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_STRING,_T("Blit effect (Semi-transparency, Invert, XOR, AND, OR, Add, Mono, Sub, Tint)"))
) {
	TCHAR* bmode = GetStr();
	BlitOp bop = BOP_COPY;
	if(!_tcsicmp(bmode,_T("semi-transparency"))) bop = BOP_BLEND;
	else if(!_tcsicmp(bmode,_T("invert"))) bop = BOP_INVERT;
	else if(!_tcsicmp(bmode,_T("xor"))) bop = BOP_XOR;
	else if(!_tcsicmp(bmode,_T("and"))) bop = BOP_AND;
	else if(!_tcsicmp(bmode,_T("or"))) bop = BOP_OR;
	else if(!_tcsicmp(bmode,_T("add"))) bop = BOP_ADD;
	else if(!_tcsicmp(bmode,_T("mono"))) bop = BOP_MONO;
	else if(!_tcsicmp(bmode,_T("sub"))) bop = BOP_SUB;
	else if(!_tcsicmp(bmode,_T("tint"))) bop = BOP_RGBAFILTER;
	rdPtr->b.operation = bop;
	return 0;
}
ACTION(
	/* ID */			127,
	/* Name */			_T("Remove alpha channel"),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();
	COLORREF transp = TargetImg->GetTransparentColor();
	if(TargetImg->HasAlpha())
	{	
		cSurface tmp;
		tmp.Clone(*TargetImg);
		TargetImg->Delete();
		TargetImg->Create(tmp.GetWidth(),tmp.GetHeight(),&tmp);
		tmp.Blit(*TargetImg,0,0,BMODE_OPAQUE);
		if Current(rdPtr->targetId)
			ImageChanged();
		
	}
	return 0;
}
ACTION(
	/* ID */			128,
	/* Name */			_T("Write bytes at %0 with size %2 into the buffer at %1"),
	/* Flags */			0,
	/* Params */		(3,PARAM_NUMBER,_T("Source address"),PARAM_NUMBER,_T("Buffer offset"),PARAM_NUMBER,_T("Size in bytes"))
) {
	TargetExists();
	unsigned int src = GetInt();
	int dest = GetInt();
	int size = GetInt();
	if(rdPtr->buffer&&rdPtr->buffPitch)
	{
		if(size<0) size = rdPtr->buffPitch*TargetImg->GetHeight();
		memcpy(rdPtr->buffer+dest,(void*)src,size);
	}
	return 0;
}

ACTION(
	/* ID */			129,
	/* Name */			_T("Add image reference for %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Reference value"))
) {
	cSurface* tmp = (cSurface*)GetInt();
	if(!tmp)
		return 0;
	//NOT a local image, it won't be freed automatically
	rdPtr->surf->push_back(new SImage(tmp,false));
	rdPtr->lastId = rdPtr->surf->size()-1;
	//Select
	if(rdPtr->selectLast)
	{
		rdPtr->targetId = rdPtr->lastId;
		TargetImg = Image(rdPtr->lastId);
		rdPtr->targetValid = TargetImg->IsValid();
		if Current(rdPtr->targetId)
		{
			CurrentImg = TargetImg;
			UpdateHotspot(rdPtr);
			ImageChanged();
		}
		
	}
	return 0;
}

ACTION(
	/* ID */			130,
	/* Name */			_T("Insert image reference for %1 at %0"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Image"),PARAM_NUMBER,_T("Reference value"))
) {
	int id = GetInt();
	ImageIsSafe(id);
	cSurface* tmp = (cSurface*)GetInt();
	if(!tmp)
		return 0;
	rdPtr->surf->insert(rdPtr->surf->begin()+id,new SImage(tmp,false));
	//Update current ID
	if(rdPtr->targetId>=0&&param1<=rdPtr->currentId&&rdPtr->currentId<rdPtr->surf->size()-1)
		rdPtr->targetId++;
	if(rdPtr->currentId>=0&&param1<=rdPtr->currentId&&rdPtr->currentId<rdPtr->surf->size()-1)
		rdPtr->currentId++;
	//Last added ID
	rdPtr->lastId = id;
	//Select
	if(rdPtr->selectLast)
	{
		rdPtr->targetId = rdPtr->lastId;
		TargetImg = Image(rdPtr->lastId);
		rdPtr->targetValid = TargetImg->IsValid();
		if Current(rdPtr->targetId)
		{
			CurrentImg = TargetImg;
			UpdateHotspot(rdPtr);
			ImageChanged();
		}
		
	}
	return 0;
}

ACTION(
	/* ID */			131,
	/* Name */			_T("Copy image %0 from image referenced at %1"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Destination image"),PARAM_NUMBER,_T("Reference value"))
) {
	int img = GetInt();
	ImageInRange(img);
	cSurface* tmp = (cSurface*)GetInt();
	if(!tmp)
		return 0;
	//Clone image
	if(Image(img)->IsValid()) Image(img)->Delete();
	Image(img)->Clone(*tmp);
	//Make image valid
	if(rdPtr->targetId==img)
	{
		TargetImg = Image(img);
		rdPtr->targetValid = true;
	}
	//Redraw
	if Current(img)
	{
		CurrentImg = Image(img);
		UpdateHotspot(rdPtr);
		ImageChanged();
	}
	
	return 0;
}

ACTION(
	/* ID */			132,
	/* Name */			_T("Set reference value of image %0 to %1"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Image"),PARAM_NUMBER,_T("Reference value"))
) {
	int id = GetInt();
	ImageExists(id);
	cSurface* tmp = (cSurface*)GetInt();
	if(!tmp)
		return 0;
	
	if(!rdPtr->surf->at(id)->local)
		rdPtr->surf->at(id)->surf = tmp;

	return 0;
}

ACTION(
	/* ID */			133,
	/* Name */			_T("Set reference state of image %0 to %1"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Image"),PARAM_NUMBER,_T("State (0: Local image, 1: Reference)"))
) {
	int id = GetInt();
	ImageExists(id);
	
	rdPtr->surf->at(id)->local = !GetInt();

	return 0;
}

ACTION(
	/* ID */			134,
	/* Name */			_T("Set keep points after drawing to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Keep points after drawing? (0: No, 1: Yes)"))
) {
	rdPtr->keepPoints = param1;
	return 0;
}

ACTION(
	/* ID */			135,
	/* Name */			_T("Set background file input/output to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Background file input/output? (0: No, 1: Yes)"))
) {
	rdPtr->threadedIO = param1;
	return 0;
}
ACTION(
	/* ID */			136,
	/* Name */			_T("Set blit callback to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_STRING,_T("Blit callback (Overrides ink effect, empty: None)"))
) {
	TCHAR* filter = GetStr();
	if (*filter)
		_tcscpy_s(rdPtr->b.callback, sizeof(rdPtr->b.callback), filter);
	return 0;
}
ACTION(
	/* ID */			137,
	/* Name */			_T("Loop through (%0,%1,%2,%3) with callback %5 and flags %4"),
	/* Flags */			0,
	/* Params */		(6,PARAM_NUMBER,_T("X1"),PARAM_NUMBER,_T("Y1"),PARAM_NUMBER,_T("X2"),PARAM_NUMBER,_T("Y2")
	,PARAM_NUMBER,_T("Flags, add together (1: Get color, 2: Set color, 4: Store position, 8: Get alpha, 16: Set alpha)"),PARAM_STRING,_T("Callback"))
) {
	TargetExists();

	int x1 = GetXPos();
	int y1 = GetYPos();
	int x2 = GetXPos();
	int y2 = GetYPos();
	//Flags
	int flags = GetInt();
	bool doRead = flags&1;
	bool doWrite = flags&2;
	bool doXY = flags&4;
	bool doReadA = flags&8;
	bool doWriteA = flags&16;
	//Set callback
	rdPtr->callback = GetStr();

	int width = TargetImg->GetWidth();
	int height = TargetImg->GetHeight();

	x1 = max(0,min(width-1,x1));
	x2 = max(0,min(width-1,x2));
	y1 = height-max(0,min(height-1,y1));
	y2 = height-max(0,min(height-1,y2));

	if(x1>x2)
		x1 ^= x2 ^= x1 ^= x2;
	if(y1>y2)
		y1 ^= y2 ^= y1 ^= y2;

	//Get GenerateEvent lookup
	long ( WINAPI * routine)(headerObject*, WPARAM, LPARAM);
	routine = ((LPRH)rdPtr->rHo.hoAdRunHeader)->rh4.rh4KpxFunctions[RFUNCTION_GENERATEEVENT].routine;

	//Get the alpha channel
	cSurface* alphaSurf = 0;
	if((doReadA || doWriteA) && TargetImg->HasAlpha())
		alphaSurf = TargetImg->GetAlphaSurface();
	if(!alphaSurf)
		doWriteA = doReadA = false;

	//Color channel
	BYTE* buff;
	buff = TargetImg->LockBuffer();
	if(!buff) return 0;

	int pitch = TargetImg->GetPitch();
	if(pitch < 0)
	{
		pitch *= -1;
		buff -= pitch*(height-1);
	}
	int size = pitch*height;
	int byte = TargetImg->GetDepth()>>3;

	//Pointer to source variable
	BYTE* colSrc = (BYTE*)&rdPtr->colSrc;
	BYTE* dest = (BYTE*)&rdPtr->colNew;

	BYTE* end = buff+y2*pitch;

	//For getting the pixel position
	rdPtr->callX = x1;
	rdPtr->callY = height-1-y1;

	for(BYTE* y=buff+y1*pitch; y<=end ;y+=pitch)
	{
		BYTE* nextline = y+x2*byte;
		for(BYTE* x=y+x1*byte; x<=nextline; x+=byte)
		{
			//Write source color
			if(doRead)
			{
				colSrc[0] = x[2];
				colSrc[1] = x[1];
				colSrc[2] = x[0];
				rdPtr->colNew = rdPtr->colSrc;
			}
			//Get source alpha
			if(doReadA)
			{
				rdPtr->colAlphaSrc = alphaSurf->GetPixelFast8(rdPtr->callX,rdPtr->callY);
			}
			//Callback
			rdPtr->colRet = false;
			rdPtr->colAlphaRet = false;
			routine((LPHO)rdPtr,8,0);
			//Write new color
			if(doWrite&&rdPtr->colRet)
			{
				x[0] = dest[2];
				x[1] = dest[1];
				x[2] = dest[0];
			}
			//Write new alpha
			if(doWriteA&&rdPtr->colAlphaRet)
			{
				alphaSurf->SetPixelFast8(rdPtr->callX,rdPtr->callY,rdPtr->colAlphaNew);
			}
			//Update position
			if(doXY)
				++rdPtr->callX;
		}
		//Update position
		if(doXY)
		{
			rdPtr->callX = x1;
			--rdPtr->callY;
		}
	}

	//Release the alpha channel
	if(alphaSurf)
		TargetImg->ReleaseAlphaSurface(alphaSurf);

	TargetImg->UnlockBuffer(buff);

	if Current(rdPtr->targetId)
		ImageChanged();
		
	return 0;
}

ACTION(
	/* ID */			138,
	/* Name */			_T("Return alpha %0 to callback"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Alpha (0-255)"))
) {
	if(!rdPtr->colAlphaRet)
	{
		rdPtr->colAlphaNew = param1;
		rdPtr->colAlphaRet = true;
	}
	return 0;
}

ACTION(
	/* ID */			139,
	/* Name */			_T("Set scale to %0, quality = %1"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Scale value (1.0: Normal dimension)"),PARAM_NUMBER,_T("Quality (0: Speed, 1: Quality)"))
) {
	long p1temp = CNC_GetFloatParameter(rdPtr);
	float p1 = *(float*) &p1temp;
	int p2 = Param(TYPE_INT) != 0;
	rdPtr->rc.rcScaleX = p1;
	rdPtr->rc.rcScaleY = p1;
	rdPtr->rs.rsFlags = (rdPtr->rs.rsFlags & ~RSFLAG_SCALE_RESAMPLE) | (p2 ? RSFLAG_SCALE_RESAMPLE : 0);

	UpdateHotspot(rdPtr);
	ImageChanged();

	return 0;
}

ACTION(
	/* ID */			140,
	/* Name */			_T("Set X scale to %0, quality = %1"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Scale value (1.0: Normal dimension)"),PARAM_NUMBER,_T("Quality (0: Speed, 1: Quality)"))
) {
	long p1temp = CNC_GetFloatParameter(rdPtr);
	float p1 = *(float*) &p1temp;
	int p2 = Param(TYPE_INT) != 0;
	rdPtr->rc.rcScaleX = p1;
	rdPtr->rs.rsFlags = (rdPtr->rs.rsFlags & ~RSFLAG_SCALE_RESAMPLE) | (p2 ? RSFLAG_SCALE_RESAMPLE : 0);
	
	UpdateHotspot(rdPtr);
	ImageChanged();
	
	return 0;
}

ACTION(
	/* ID */			141,
	/* Name */			_T("Set Y scale to %0, quality = %1"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Scale value (1.0: Normal dimension)"),PARAM_NUMBER,_T("Quality (0: Speed, 1: Quality)"))
) {
	long p1temp = CNC_GetFloatParameter(rdPtr);
	float p1 = *(float*) &p1temp;
	int p2 = Param(TYPE_INT) != 0;

	rdPtr->rc.rcScaleY = p1;
	rdPtr->rs.rsFlags = (rdPtr->rs.rsFlags & ~RSFLAG_SCALE_RESAMPLE) | (p2 ? RSFLAG_SCALE_RESAMPLE : 0);

	UpdateHotspot(rdPtr);
	ImageChanged();

	return 0;

}

ACTION(
	/* ID */			142,
	/* Name */			_T("Add point (%0,%1)"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("X"),PARAM_NUMBER,_T("Y"))
) {
	float x,y;
	LoadFloat(x);
	LoadFloat(y);
	PolyCoord p = {x,y};
	rdPtr->points->push_back(p);
	return 0;
}
ACTION(
	/* ID */			143,
	/* Name */			_T("Set hot spot to (%0,%1) px"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Hot spot X"),PARAM_NUMBER,_T("Hot spot Y"))
) {
	TargetExists();
	ImageS(rdPtr->targetId)->hotX = param1;
	ImageS(rdPtr->targetId)->hotY = param2;
	if Current(rdPtr->targetId)
	{
		UpdateHotspot(rdPtr);
		ImageChanged();
	}
	return 0;
}

ACTION(
	/* ID */			144,
	/* Name */			_T("Set hot spot to (%0,%1) percent"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Hot spot X (Percent)"),PARAM_NUMBER,_T("Hot spot Y (Percent)"))
) {

	TargetExists();
	param1 = param1/100.0*TargetImg->GetWidth();
	param2 = param2/100.0*TargetImg->GetHeight();
	ImageS(rdPtr->targetId)->hotX = param1;
	ImageS(rdPtr->targetId)->hotY = param2;
	if Current(rdPtr->targetId)
	{
		UpdateHotspot(rdPtr);
		ImageChanged();
	}
	return 0;
}

ACTION(
	/* ID */			145,
	/* Name */			_T("Create callback pattern %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_STRING,_T("Pattern name"))
) {
	TCHAR* callback = GetStr();
	string str = string(callback);
	if(!str[0]) return 0;

	FillGuide* guide = new FillGuide;
	guide->type = FILL_USER;
	guide->img = false;
	CFillUser* fill = new CFillUser(rdPtr,callback);
	guide->fill = fill;


	rdPtr->fill->erase(str);
	rdPtr->fill->insert(pair<string,FillGuide*>(str,guide));
	return 0;
}

ACTION(
	/* ID */			146,
	/* Name */			_T("Set blit source, position = (%0,%1), dimensions = (%2,%3), region flag = %4"),
	/* Flags */			0,
	/* Params */		(5,PARAM_NUMBER,_T("Source X"),PARAM_NUMBER,_T("Source Y"),
							PARAM_NUMBER,_T("Source width"),PARAM_NUMBER,_T("Source height"),
							PARAM_NUMBER,_T("Use specified source region? (0: No, 1: Yes)"))
) {
	rdPtr->b.srcX = GetInt();
	rdPtr->b.srcY = GetInt();
	rdPtr->b.srcW = GetInt();
	rdPtr->b.srcH = GetInt();
	rdPtr->b.srcUse = GetInt() != 0;
	return 0;
}

ACTION(
	/* ID */			147,
	/* Name */			_T("Set blit angle to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Angle (Degrees)"))
) {
	rdPtr->b.angle = param1;
	return 0;
}

ACTION(
	/* ID */			148,
	/* Name */			_T("Set blit hot spot to (%0,%1) px"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Hot spot X"),PARAM_NUMBER,_T("Hot spot Y"))
) {
	rdPtr->b.hotX = GetInt();
	rdPtr->b.hotY = GetInt();
	rdPtr->b.hotspotMode &= ~2;
	return 0;
}

ACTION(
	/* ID */			149,
	/* Name */			_T("(Deprecated) Set blit hot spot flag to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Enable blit hot spot? (0: No, 1: Yes)"))
) {
	/* Deprecated */
	rdPtr->b.hotspotMode |= GetInt()!=0?1:0;
	return 0;
}

ACTION(
	/* ID */			150,
	/* Name */			_T("Set blit hot spot to (%0,%1) percent"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Hot spot X (Percent)"),PARAM_NUMBER,_T("Hot spot Y (Percent)"))
) {
	rdPtr->b.hotX = GetInt();
	rdPtr->b.hotY = GetInt();
	rdPtr->b.hotspotMode |= 2;
	return 0;
}

ACTION(
	/* ID */			151,
	/* Name */			_T("Set blit rotation quality to %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_NUMBER,_T("Rotation quality (0: None, 1: Linear resampling)"))
) {
	rdPtr->b.angleResample = param1;
	return 0;
}

ACTION(
	/* ID */			152,
	/* Name */			_T("Set angle to %0, quality = %1"),
	/* Flags */			0,
	/* Params */		(2,PARAM_NUMBER,_T("Angle (Degrees)"),PARAM_NUMBER,_T("Quality (0: Speed, 1: Quality)"))
) {
	long p1temp = CNC_GetFloatParameter(rdPtr);
	float p1 = *(float*) &p1temp;
	int p2 = Param(TYPE_INT) != 0;
	while(p1 < 0) p1 += 360;
#ifndef ANGLEFLOAT
	rdPtr->rc.rcAngle = (int)p1 % 360;
#else
	rdPtr->rc.rcAngle = fmod(p1, 360);
#endif
	rdPtr->rs.rsFlags = (rdPtr->rs.rsFlags & ~RSFLAG_ROTATE_ANTIA) | (p2 ? RSFLAG_ROTATE_ANTIA : 0);
	
	UpdateHotspot(rdPtr);
	ImageChanged();
	
	return 0;
}

ACTION(
	/* ID */			153,
	/* Name */			_T("Load image from file %0"),
	/* Flags */			0,
	/* Params */		(1,PARAM_FILENAME2,_T("File"))
) {
	ImageInRange(rdPtr->targetId);
	TCHAR* file = GetStr();

	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	CImageFilterMgr* pImgMgr = rhPtr->rh4.rh4Mv->mvImgFilterMgr;

	if (rdPtr->ioFile)
		free(rdPtr->ioFile);
	rdPtr->ioFile = _tcsdup(file);

	if(rdPtr->threadedIO)
	{
		if (rdPtr->ioHandle)
		{
			rdPtr->rRd->GenerateEvent(2);
			return 0;
		}

		rdPtr->ioSave = false;
		rdPtr->ioHandle = rdPtr->rRd->StartThread(SurfLoadImage);
	}
	else
	{
		SurfLoadImage(rdPtr);
	}

	return 0;
}

ACTION(
	/* ID */			154,
	/* Name */			_T("Convert to HWA texture"),
	/* Flags */			0,
	/* Params */		(0)
) {
#ifdef HWABETA
	if(!rdPtr->isHWA)
		return 0;

	ImageExists(rdPtr->targetId);

	if(TargetImg->GetType() >= ST_HWA_SCREEN)
		return 0;

	// convert transp. color to alpha
	if(!TargetImg->HasAlpha())
		ActionFunc4(rdPtr, 1, 0);

	cSurface* proto;
	GetSurfacePrototype(&proto, TargetImg->GetDepth(), ST_HWA_ROMTEXTURE, SD_D3D9);

	// convert
	cSurface* hwa = new cSurface;
	hwa->Create(rdPtr->target->GetWidth(), rdPtr->target->GetHeight(), proto);
	TargetImg->Blit(*hwa);
	hwa->SetTransparentColor(TargetImg->GetTransparentColor());

	// assign new image
	delete TargetImg;
	TargetImg = hwa;
	Image(rdPtr->targetId) = TargetImg;

	if Current(rdPtr->targetId)
	{
		rdPtr->current = rdPtr->target;
		ImageChanged();
	}
#endif

	return 0;
}

ACTION(
	/* ID */			155,
	/* Name */			_T("Convert to HWA target"),
	/* Flags */			0,
	/* Params */		(0)
) {
#ifdef HWABETA
	if(!rdPtr->isHWA)
		return 0;

	ImageExists(rdPtr->targetId);

	if(TargetImg->GetType() >= ST_HWA_SCREEN)
		return 0;

	// convert transp. color to alpha
	if(!TargetImg->HasAlpha())
		ActionFunc4(rdPtr, 1, 0);

	cSurface* proto;
	GetSurfacePrototype(&proto, TargetImg->GetDepth(), ST_HWA_RTTEXTURE, SD_D3D9);

	// convert
	cSurface* hwa = new cSurface;
	hwa->Create(rdPtr->target->GetWidth(), rdPtr->target->GetHeight(), proto);

	//// Create temp. texture to draw old image onto target
	//GetSurfacePrototype(&proto, TargetImg->GetDepth(), ST_HWA_ROMTEXTURE, SD_D3D9);
	//cSurface texture;
	//texture.Create(TargetImg->GetWidth(), TargetImg->GetHeight(), proto);
	//TargetImg->Blit(texture);

	hwa->SetTransparentColor(TargetImg->GetTransparentColor()); // Probably no effect

	// assign new image
	delete TargetImg;
	TargetImg = hwa;
	Image(rdPtr->targetId) = TargetImg;

	if Current(rdPtr->targetId)
	{
		rdPtr->current = rdPtr->target;
		ImageChanged();
	}
#endif

	return 0;
}

ACTION(
	/* ID */			156,
	/* Name */			_T("Convert to bitmap"),
	/* Flags */			0,
	/* Params */		(0)
) {
#ifdef HWABETA
	if(!rdPtr->isHWA)
		return 0;

	ImageExists(rdPtr->targetId);

	if(TargetImg->GetType() < ST_HWA_SCREEN)
		return 0;

	cSurface* proto;
	GetSurfacePrototype(&proto, TargetImg->GetDepth(), ST_MEMORYWITHDC, SD_DIB);

	// convert
	cSurface* bmp = new cSurface;
	bmp->Create(rdPtr->target->GetWidth(), rdPtr->target->GetHeight(), proto);
	TargetImg->Blit(*bmp, 0, 0, BMODE_OPAQUE, BOP_COPY);
	bmp->SetTransparentColor(TargetImg->GetTransparentColor());

	// assign new image
	delete TargetImg;
	TargetImg = bmp;
	Image(rdPtr->targetId) = TargetImg;

	if Current(rdPtr->targetId)
	{
		rdPtr->current = rdPtr->target;
		ImageChanged();
	}
#endif

	return 0;
}

ACTION(
	/* ID */			157,
	/* Name */			_T("Set blit tint to %0"),
	/* Flags */			0,
	/* Params */		(1, PARAM_COLOUR, _T("Color multiplier"))//, PARAM_NUMBER, _T("Alpha multiplier (0-255)"))
) {
	rdPtr->b.param = (rdPtr->b.param & 0xff000000) | ((param1 & 0xff0000) >> 16) | (param1 & 0xff00) | ((param1 & 0xff) << 16);
	return 0;
}

ACTION(
	/* ID */			158,
	/* Name */			_T("Draw ellipse (%0,%1,%2,%3) with color %4"),
	/* Flags */			0,
	/* Params */		(5,PARAM_NUMBER,_T("X1"),PARAM_NUMBER,_T("Y1"),PARAM_NUMBER,_T("X2"),PARAM_NUMBER,_T("Y2"),PARAM_COLOUR,_T("Fill color"))
) {
	TargetExists();
	int x1 = GetXPos();
	int y1 = GetYPos();
	int x2 = GetXPos();
	int y2 = GetYPos();
	COLORREF fill = GetCol();
	TargetImg->Ellipse(x1,y1,x2,y2,fill,0,0,TRUE);

	if Current(rdPtr->targetId)
		RectChanged(x1,y1,x2,y2);
	
	return 0;
}
ACTION(
	/* ID */			159,
	/* Name */			_T("Draw rectangle (%0,%1,%2,%3) with color %4"),
	/* Flags */			0,
	/* Params */		(5,PARAM_NUMBER,_T("X1"),PARAM_NUMBER,_T("Y1"),PARAM_NUMBER,_T("X2"),PARAM_NUMBER,_T("Y2"),PARAM_COLOUR,_T("Fill color"))
) {
	TargetExists();
	int x1 = GetXPos();
	int y1 = GetYPos();
	int x2 = GetXPos();
	int y2 = GetYPos();
	COLORREF fill = GetCol();
	TargetImg->Rectangle(x1,y1,x2,y2,fill,0,0,TRUE);

	if Current(rdPtr->targetId)
		RectChanged(x1,y1,x2,y2);
	
	return 0;
}

ACTION(
	/* ID */			160,
	/* Name */			_T("Draw polygon at (%0,%1) with color %2"),
	/* Flags */			0,
	/* Params */		(3,PARAM_NUMBER,_T("X"),PARAM_NUMBER,_T("Y"),PARAM_COLOUR,_T("Fill color"))
) {
	TargetExists();
	int ox = GetXPos();
	int oy = GetYPos();
	COLORREF fill = GetCol();

	int size = rdPtr->points->size();
	POINT* tmp = new POINT[size];

	for(int i=0;i<size;i++)
	{
		tmp[i].x = rdPtr->points->at(i).x+ox;
		tmp[i].y = rdPtr->points->at(i).y+oy;
	}

	TargetImg->Polygon(tmp,size,fill,0,0,TRUE);

	//Delete points
	delete[] tmp;

	//Don't keep points
	if(!rdPtr->keepPoints)
		rdPtr->points->clear();

	if Current(rdPtr->targetId)
		ImageChanged();
	

	return 0;
}

ACTION(
	/* ID */			161,
	/* Name */			_T("Apply color matrix R (%0,%1,%2), G (%3,%4,%5), B (%6,%7,%8)"),
	/* Flags */			0,
	/* Params */		(9,
	PARAM_NUMBER,_T("Red coefficient for new red (Default: 1)"),PARAM_NUMBER,_T("Green coefficient for new red (Default: 0)"),PARAM_NUMBER,_T("Blue coefficient for new red (Default: 0)"),
						PARAM_NUMBER,_T("Red coefficient for new green (Default: 0)"),PARAM_NUMBER,_T("Green coefficient for new green  (Default: 1)"),PARAM_NUMBER,_T("Blue coefficient for new green (Default: 0)"),
						PARAM_NUMBER,_T("Red coefficient for new blue (Default: 0)"),PARAM_NUMBER,_T("Green coefficient for new blue (Default: 0)"),PARAM_NUMBER,_T("Blue coefficient for new blue  (Default: 1)"))
) {
	TargetExists();
	float r_r, r_g, r_b;
	float g_r, g_g, g_b;
	float b_r, b_g, b_b;
	LoadFloat(r_r);
	LoadFloat(r_g);
	LoadFloat(r_b);
	LoadFloat(g_r);
	LoadFloat(g_g);
	LoadFloat(g_b);
	LoadFloat(b_r);
	LoadFloat(b_g);
	LoadFloat(b_b);

	//Dimensions
	int width = TargetImg->GetWidth(), height = TargetImg->GetHeight();
	//New RGB
	int nr,ng,nb;
	//Old RGB
	COLORREF rgb;
	//New position
	int vx, vy;
	//Other variables
	int offset;

	//Lock buffer, get pitch etc.
	BYTE* buff;
	buff = TargetImg->LockBuffer();
	if(!buff) return 0;

	int pitch = TargetImg->GetPitch();
	if(pitch < 0)
	{
		pitch *= -1;
		buff -= pitch*(height-1);
	}
	int size = pitch*height;
	int byte = TargetImg->GetDepth()>>3;

	BYTE* end = buff+size;

	for(BYTE* y=buff; y<end ;y+=pitch)
	{
		BYTE* nextline = y+pitch;
		for(BYTE* x=y; x<nextline; x+=byte)
		{
			int r = x[2];
			int g = x[1];
			int b = x[0];

			x[2] = max(0, min(255, r*r_r + g*r_g + b*r_b));
			x[1] = max(0, min(255, r*g_r + g*g_g + b*g_b));
			x[0] = max(0, min(255, r*b_r + g*b_g + b*b_b));
		}
	}

	TargetImg->UnlockBuffer(buff);

	if Current(rdPtr->targetId)
		ImageChanged();
	

	return 0;
}
ACTION(
	/* ID */			162,
	/* Name */			_T("Store image"),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();

	if (rdPtr->stored)
		delete rdPtr->stored;

	rdPtr->stored = new cSurface;
	rdPtr->stored->Clone(*TargetImg);
	
	return 0;
}
ACTION(
	/* ID */			163,
	/* Name */			_T("Restore image"),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();

	if (rdPtr->stored)
	{
		Image(rdPtr->targetId)->Delete();
		Image(rdPtr->targetId)->Clone(*rdPtr->stored);
		TargetImg = Image(rdPtr->targetId);
		rdPtr->targetValid = true;

		if Current(rdPtr->targetId)
			ImageChanged();
	}
	
	return 0;
}

ACTION(
	/* ID */			164,
	/* Name */			_T("Apply brightness %0 to image"),
	/* Flags */			0,
	/* Params */		(1, PARAM_NUMBER, _T("Brightness (0.0: No change, 1.0: Entire image will be white)"))
) {
	float brightness;
	LoadFloat(brightness)
	brightness *= 255;
	TargetExists();
	int width = TargetImg->GetWidth();
	int height = TargetImg->GetHeight();

	//Color channel
	BYTE* buff;
	buff = TargetImg->LockBuffer();
	if(!buff) return 0;

	int pitch = TargetImg->GetPitch();
	if(pitch < 0)
	{
		pitch *= -1;
		buff -= pitch*(height-1);
	}
	int size = pitch*height;
	int byte = TargetImg->GetDepth()>>3;

	BYTE* end = buff+size;

	for(BYTE* y=buff; y<end ;y+=pitch)
	{
		BYTE* nextline = y+pitch;
		for(BYTE* x=y; x<nextline; x+=byte)
		{
			x[2] = max(0, min(255, x[2] + brightness));
			x[1] = max(0, min(255, x[1] + brightness));
			x[0] = max(0, min(255, x[0] + brightness));
		}
	}

	TargetImg->UnlockBuffer(buff);

	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}

ACTION(
	/* ID */			165,
	/* Name */			_T("Apply contrast %0 to image"),
	/* Flags */			0,
	/* Params */		(1, PARAM_NUMBER, _T("Contrast (1.0: No change)"))
) {
	float contrast;
	LoadFloat(contrast)
	TargetExists();
	int width = TargetImg->GetWidth();
	int height = TargetImg->GetHeight();

	//Color channel
	BYTE* buff;
	buff = TargetImg->LockBuffer();
	if(!buff) return 0;

	int pitch = TargetImg->GetPitch();
	if(pitch < 0)
	{
		pitch *= -1;
		buff -= pitch*(height-1);
	}
	int size = pitch*height;
	int byte = TargetImg->GetDepth()>>3;

	BYTE* end = buff+size;

	for(BYTE* y=buff; y<end ;y+=pitch)
	{
		BYTE* nextline = y+pitch;
		for(BYTE* x=y; x<nextline; x+=byte)
		{
			x[2] = max(0, min(255, (x[2] - 128) * contrast + 128));
			x[1] = max(0, min(255, (x[1] - 128) * contrast + 128));
			x[0] = max(0, min(255, (x[0] - 128) * contrast + 128));
		}
	}

	TargetImg->UnlockBuffer(buff);

	if Current(rdPtr->targetId)
		ImageChanged();
	
	return 0;
}
ACTION(
	/* ID */			166,
	/* Name */			_T("Draw rectangle at (%0,%1) with size (%2,%3) and color %4"),
	/* Flags */			0,
	/* Params */		(5,PARAM_NUMBER,_T("X"),PARAM_NUMBER,_T("Y"),PARAM_NUMBER,_T("Width"),PARAM_NUMBER,_T("Height"),PARAM_COLOUR,_T("Fill color"))
) {
	TargetExists();
	int x = GetXPos();
	int y = GetYPos();
	int w = GetInt();
	int h = GetInt();
	COLORREF fill = GetCol();
	TargetImg->Rectangle(x,y,x+w,y+h,fill,0,0,TRUE);

	if Current(rdPtr->targetId)
		RectChanged(x,y,x+w,y+h);
	
	return 0;
}
ACTION(
	/* ID */			167,
	/* Name */			_T("Draw ellipse at (%0,%1) with size (%2,%3) and color %4"),
	/* Flags */			0,
	/* Params */		(5,PARAM_NUMBER,_T("Center X"),PARAM_NUMBER,_T("Center Y"),PARAM_NUMBER,_T("Width"),PARAM_NUMBER,_T("Height"),PARAM_COLOUR,_T("Fill color"))
) {
	TargetExists();
	int x = GetXPos();
	int y = GetYPos();
	float w = GetInt()/2.0f; float h = GetInt()/2.0f;
	int x1 = x-floor(w); int y1 = y-floor(h); int x2 = x+ceil(w)+1; int y2 = y+ceil(h)+1;
	COLORREF fill = GetCol();

	TargetImg->Ellipse(x1,y1,x2,y2,fill,0,0,TRUE);

	if Current(rdPtr->targetId)
		RectChanged(x1,y1,x2,y2);
	
	return 0;
}


ACTION(
	/* ID */			168,
	/* Name */			_T("Set blit alpha to %0"),
	/* Flags */			0,
	/* Params */		(1, PARAM_NUMBER, _T("Alpha value (0-255)"))
) {
	rdPtr->b.param = (rdPtr->b.param & 0xffffff) | (max(0, min(255, param1)) << 24);
	return 0;
}

ACTION(
	/* ID */			169,
	/* Name */			_T("Push blit settings"),
	/* Flags */			0,
	/* Params */		(0)
) {
	if (rdPtr->blitStackCursor < (signed)(sizeof(rdPtr->blitStack)/sizeof(BlitSet)) - 1)
	{
		// Stash the current blit settings
		rdPtr->blitStack[++rdPtr->blitStackCursor] = rdPtr->b;
	}
	return 0;
}

ACTION(
	/* ID */			170,
	/* Name */			_T("Pop blit settings"),
	/* Flags */			0,
	/* Params */		(0)
) {
	if (rdPtr->blitStackCursor >= 0)
	{
		// Retrieve settings from top of stack
		rdPtr->b = rdPtr->blitStack[rdPtr->blitStackCursor--];
	}
	return 0;
}
// ============================================================================
//
// EXPRESSIONS
// 
// ============================================================================

EXPRESSION(
	/* ID */			0,
	/* Name */			_T("ImageCount("),
	/* Flags */			0,
	/* Params */		(0)
) {
	return rdPtr->surf->size();
}

EXPRESSION(
	/* ID */			1,
	/* Name */			_T("SelImage("),
	/* Flags */			0,
	/* Params */		(0)
) {
	return rdPtr->targetId;
}

EXPRESSION(
	/* ID */			2,
	/* Name */			_T("DisplayImage("),
	/* Flags */			0,
	/* Params */		(0)
) {
	return rdPtr->currentId;
}

EXPRESSION(
	/* ID */			3,
	/* Name */			_T("RGBAt("),
	/* Flags */			0,
	/* Params */		(2,EXPPARAM_NUMBER,_T("X"),EXPPARAM_NUMBER,_T("Y"))
) {
	int x = GetXPos2();
	int y = GetYPos2();
	TargetExists();
	COLORREF col;
	if(rdPtr->buffer)
		return TargetImg->GetPixelFast(x,y);
	else
		TargetImg->GetPixel(x,y,col);
	return col;
}

EXPRESSION(
	/* ID */			4,
	/* Name */			_T("Width("),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();
	return TargetImg->GetWidth();
}

EXPRESSION(
	/* ID */			5,
	/* Name */			_T("Height("),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();	
	return TargetImg->GetHeight();
}

EXPRESSION(
	/* ID */			6,
	/* Name */			_T("LastImage("),
	/* Flags */			0,
	/* Params */		(0)
) {
	return rdPtr->lastId;
}

EXPRESSION(
	/* ID */			7,
	/* Name */			_T("ExportedOvlAddr("),
	/* Flags */			0,
	/* Params */		(0)
) {
	return (long)rdPtr->ovl;
}

EXPRESSION(
	/* ID */			8,
	/* Name */			_T("DisplayWidth("),
	/* Flags */			0,
	/* Params */		(0)
) {
	ImageExists(rdPtr->currentId);
	return rdPtr->current->GetWidth();
}

EXPRESSION(
	/* ID */			9,
	/* Name */			_T("DisplayHeight("),
	/* Flags */			0,
	/* Params */		(0)
) {
	ImageExists(rdPtr->currentId);
	return rdPtr->current->GetHeight();
}

EXPRESSION(
	/* ID */			10,
	/* Name */			_T("RedAt("),
	/* Flags */			0,
	/* Params */		(2,EXPPARAM_NUMBER,_T("X"),EXPPARAM_NUMBER,_T("Y"))
) {
	int x = GetXPos2();
	int y = GetYPos2();
	TargetExists();
	COLORREF col;
	TargetImg->GetPixel(x,y,col);
	return GetRValue(col);
}

EXPRESSION(
	/* ID */			11,
	/* Name */			_T("GreenAt("),
	/* Flags */			0,
	/* Params */		(2,EXPPARAM_NUMBER,_T("X"),EXPPARAM_NUMBER,_T("Y"))
) {
	int x = GetXPos2();
	int y = GetYPos2();
	TargetExists();
	COLORREF col;
	TargetImg->GetPixel(x,y,col);
	return GetGValue(col);
}

EXPRESSION(
	/* ID */			12,
	/* Name */			_T("BlueAt("),
	/* Flags */			0,
	/* Params */		(2,EXPPARAM_NUMBER,_T("X"),EXPPARAM_NUMBER,_T("Y"))
) {
	int x = GetXPos2();
	int y = GetYPos2();
	TargetExists();
	COLORREF col;
	TargetImg->GetPixel(x,y,col);
	return GetBValue(col);
}

EXPRESSION(
	/* ID */			13,
	/* Name */			_T("AlphaAt("),
	/* Flags */			0,
	/* Params */		(2,EXPPARAM_NUMBER,_T("X"),EXPPARAM_NUMBER,_T("Y"))
) {
	int x = GetXPos2();
	int y = GetYPos2();
	TargetExists();
	int a;
	//Alpha
	if(TargetImg->HasAlpha())
	{
		cSurface* alpha = TargetImg->GetAlphaSurface();
		alpha->GetPixel(x,y,a);
		TargetImg->ReleaseAlphaSurface(alpha);
		return a;
	}
	//Transp. color
	else
	{
		TargetImg->GetPixel(x,y,a);
		return a==TargetImg->GetTransparentColor()?255:0;
	}
}

EXPRESSION(
	/* ID */			14,
	/* Name */			_T("ImgRGBAt("),
	/* Flags */			0,
	/* Params */		(3,EXPPARAM_NUMBER,_T("Image"),EXPPARAM_NUMBER,_T("X"),EXPPARAM_NUMBER,_T("Y"))
) {
	int img = GetInt2();
	int x = GetXPos2();
	int y = GetYPos2();
	ImageExists(img);
	cSurface* tmp = Image(img);
	COLORREF col;
	tmp->GetPixel(x,y,col);
	return col;
}

EXPRESSION(
	/* ID */			15,
	/* Name */			_T("ImgWidth("),
	/* Flags */			0,
	/* Params */		(1,EXPPARAM_NUMBER,_T("Image"))
) {
	int id = GetInt2();
	ImageExists(id);
	return Image(id)->GetWidth();
}

EXPRESSION(
	/* ID */			16,
	/* Name */			_T("ImgHeight("),
	/* Flags */			0,
	/* Params */		(1,EXPPARAM_NUMBER,_T("Image"))
) {
	int id = GetInt2();
	ImageExists(id);
	return Image(id)->GetHeight();
}

EXPRESSION(
	/* ID */			17,
	/* Name */			_T("CallbackX("),
	/* Flags */			0,
	/* Params */		(0)
) {
	return rdPtr->callX;
}

EXPRESSION(
	/* ID */			18,
	/* Name */			_T("CallbackY("),
	/* Flags */			0,
	/* Params */		(0)
) {
	return rdPtr->callY;
}

EXPRESSION(
	/* ID */			19,
	/* Name */			_T("CallbackAreaX1("),
	/* Flags */			0,
	/* Params */		(0)
) {
	return 0;//return rdPtr->userRect.left+rdPtr->userXOff;
}

EXPRESSION(
	/* ID */			20,
	/* Name */			_T("CallbackAreaY1("),
	/* Flags */			0,
	/* Params */		(0)
) {
	return 0;//return rdPtr->userRect.top+rdPtr->userYOff;
}

EXPRESSION(
	/* ID */			21,
	/* Name */			_T("CallbackAreaX2("),
	/* Flags */			0,
	/* Params */		(0)
) {
	return 0;//return rdPtr->userRect.right+rdPtr->userXOff;
}

EXPRESSION(
	/* ID */			22,
	/* Name */			_T("CallbackAreaY2("),
	/* Flags */			0,
	/* Params */		(0)
) {
	return 0;//return rdPtr->userRect.bottom+rdPtr->userYOff;
}

EXPRESSION(
	/* ID */			23,
	/* Name */			_T("RGB("),
	/* Flags */			0,
	/* Params */		(3,EXPPARAM_NUMBER,_T("R"),EXPPARAM_NUMBER,_T("G"),EXPPARAM_NUMBER,_T("B"))
) {
	int r = GetInt2();
	int g = GetInt2();
	int b = GetInt2();
	r = max(0,min(255,r));
	g = max(0,min(255,g));
	b = max(0,min(255,b));
	return RGB(r,g,b);
}

EXPRESSION(
	/* ID */			24,
	/* Name */			_T("Blend("),
	/* Flags */			0,
	/* Params */		(3,EXPPARAM_NUMBER,_T("Color A"),EXPPARAM_NUMBER,_T("Color B"),EXPPARAM_NUMBER,_T("Blend (0-255)"))
) {
	COLORREF a = GetInt2();
	COLORREF b = GetInt2();
	float blend = GetInt2()/255.0f;

	int ar = GetRValue(a);
	int ag = GetGValue(a);
	int ab = GetBValue(a);
	int br = GetRValue(b);
	int bg = GetGValue(b);
	int bb = GetBValue(b);

	return RGB(ar+(br-ar)*blend,ag+(bg-ag)*blend,ab+(bb-ab)*blend);
}

EXPRESSION(
	/* ID */			25,
	/* Name */			_T("Invert("),
	/* Flags */			0,
	/* Params */		(1,EXPPARAM_NUMBER,_T("Color"))
) {
	COLORREF rgb = GetInt2();
	return rgb^0xffffff;
}

EXPRESSION(
	/* ID */			26,
	/* Name */			_T("Multiply("),
	/* Flags */			0,
	/* Params */		(2,EXPPARAM_NUMBER,_T("Color A"),EXPPARAM_NUMBER,_T("Color B"))
) {
	COLORREF a = GetInt2();
	COLORREF b = GetInt2();

	float ar = GetRValue(a)/255.0f;
	float ag = GetGValue(a)/255.0f;
	float ab = GetBValue(a)/255.0f;
	float br = GetRValue(b)/255.0f;
	float bg = GetGValue(b)/255.0f;
	float bb = GetBValue(b)/255.0f;

	return RGB(ar*br*255.0f,ag*bg*255.0f,ab*bb*255.0f);
}


EXPRESSION(
	/* ID */			27,
	/* Name */			_T("TranspColor("),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();
	return TargetImg->GetTransparentColor();
}

EXPRESSION(
	/* ID */			28,
	/* Name */			_T("FilterCount("),
	/* Flags */			0,
	/* Params */		(0)
) {
	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	CImageFilterMgr* pImgMgr = rhPtr->rh4.rh4Mv->mvImgFilterMgr;
	return pImgMgr->GetFilterCount();
}

EXPRESSION(
	/* ID */			29,
	/* Name */			_T("Filter$("),
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(1,EXPPARAM_NUMBER,_T("Index"))
) {
	int index = GetInt2();
	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	CImageFilterMgr* pImgMgr = rhPtr->rh4.rh4Mv->mvImgFilterMgr;
	//Filter out of bounds
	int filters = pImgMgr->GetFilterCount();
	if(index<0||index>filters-1) { ReturnString(_T("")); }
	//Return
	ReturnStringSafe(pImgMgr->GetFilterName(index));
}

EXPRESSION(
	/* ID */			30,
	/* Name */			_T("FilterExt$("),
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(2,EXPPARAM_NUMBER,_T("Filter index"),EXPPARAM_NUMBER,_T("Extension index"))
) {
	int index = GetInt2();
	int e = GetInt2();
	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	CImageFilterMgr* pImgMgr = rhPtr->rh4.rh4Mv->mvImgFilterMgr;
	//Filter out of bounds
	int filters = pImgMgr->GetFilterCount();
	if(index<0||index>filters-1) { ReturnString(_T("")); }
	//Extension out of bounds
	const TCHAR** exts = pImgMgr->GetFilterExts(index);
	int count; for(count=0;exts[count];count++);
	if(e<0||e>count-1) { ReturnString(_T("")); }
	//Return
	ReturnStringSafe(exts[e]);
}

EXPRESSION(
	/* ID */			31,
	/* Name */			_T("FilterExtCount("),
	/* Flags */			0,
	/* Params */		(1,EXPPARAM_NUMBER,_T("Index"))
) {
	int index = GetInt2();
	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	CImageFilterMgr* pImgMgr = rhPtr->rh4.rh4Mv->mvImgFilterMgr;
	//Filter out of bounds
	int filters = pImgMgr->GetFilterCount();
	if(index<0||index>filters-1) return 0;
	//Count number of filters
	const TCHAR** exts = pImgMgr->GetFilterExts(index);
	int count;
	for(count=0;exts[count];count++);
	return count;
}

EXPRESSION(
	/* ID */			32,
	/* Name */			_T("FilterCanSave("),
	/* Flags */			0,
	/* Params */		(1,EXPPARAM_NUMBER,_T("Index"))
) {
	int index = GetInt2();
	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	CImageFilterMgr* pImgMgr = rhPtr->rh4.rh4Mv->mvImgFilterMgr;
	//Filter out of bounds
	int filters = pImgMgr->GetFilterCount();
	if(index<0||index>filters-1) return 0;
	//Return
	CImageFilter filter(pImgMgr);
	filter.UseSpecificFilterIndex(index);
	return filter.CanSave();
}

EXPRESSION(
	/* ID */			33,
	/* Name */			_T("BufferAddr("),
	/* Flags */			0,
	/* Params */		(0)
) {
	return (long)rdPtr->buffer;
}

EXPRESSION(
	/* ID */			34,
	/* Name */			_T("BufferPitch("),
	/* Flags */			0,
	/* Params */		(0)
) {
	return rdPtr->buffPitch;
}

EXPRESSION(
	/* ID */			35,
	/* Name */			_T("FilterAllExts$("),
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(1,EXPPARAM_NUMBER,_T("Filter index"))
) {
	int index = GetInt2();
	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	CImageFilterMgr* pImgMgr = rhPtr->rh4.rh4Mv->mvImgFilterMgr;
	//Filter out of bounds
	int filters = pImgMgr->GetFilterCount();
	if(index<0||index>filters-1) { ReturnString(""); }

	//Count the extensions
	const TCHAR** exts = pImgMgr->GetFilterExts(index);
	int count;
	for(count=0;exts[count];count++);

	//Write the string
	string list;
	for(int i=0;i<count;i++)
	{
		list += _T("*.");
		list += exts[i];
		if(i<count-1)
			list += _T(";");
	}

	//Returnrnst
	ReturnStringSafe(list.c_str());
}
EXPRESSION(
	/* ID */			36,
	/* Name */			_T("FloodX1("),
	/* Flags */			0,
	/* Params */		(0)
) {
	return rdPtr->floodL;
}

EXPRESSION(
	/* ID */			37,
	/* Name */			_T("FloodY1("),
	/* Flags */			0,
	/* Params */		(0)
) {
	return rdPtr->floodT;
}

EXPRESSION(
	/* ID */			38,
	/* Name */			_T("FloodX2("),
	/* Flags */			0,
	/* Params */		(0)
) {
	return rdPtr->floodR;
}

EXPRESSION(
	/* ID */			39,
	/* Name */			_T("FloodY2("),
	/* Flags */			0,
	/* Params */		(0)
) {
	return rdPtr->floodB;
}

EXPRESSION(
	/* ID */			40,
	/* Name */			_T("Pattern$("),
	/* Flags */			EXPFLAG_STRING,
	/* Params */		(1,EXPPARAM_NUMBER,_T("Index"))
) {
	int target = GetInt2();
	int i = 0;
	if(target>=0)
	{
		FillDB::iterator it;
		for(it=rdPtr->fill->begin();it!=rdPtr->fill->end();++it)
		{
			if(i==target)
			{
				ReturnStringSafe(it->first.c_str());
			}
			i++;
		}
	}
	ReturnString("");
}

EXPRESSION(
	/* ID */			41,
	/* Name */			_T("PatternCount("),
	/* Flags */			0,
	/* Params */		(0)
) {
	return rdPtr->fill->size();
}

EXPRESSION(
	/* ID */			42,
	/* Name */			_T("PatternColor("),
	/* Flags */			0,
	/* Params */		(1,EXPPARAM_STRING,_T("Pattern"))
) {
	string name(GetStr2());
	FillDB::iterator it = rdPtr->fill->find(name);
	if(it!=rdPtr->fill->end()&&it->second->type==FILL_COLOR)
		return ((CFillFlat*)it->second->fill)->GetColor();

	return 0;
}

EXPRESSION(
	/* ID */			43,
	/* Name */			_T("PatternColorA("),
	/* Flags */			0,
	/* Params */		(1,EXPPARAM_STRING,_T("Pattern"))
) {
	string name(GetStr2());
	FillDB::iterator it = rdPtr->fill->find(name);
	if(it!=rdPtr->fill->end())
	{
		if(it->second->type==FILL_GRADIENT)
			return ((CFillDirEx*)it->second->fill)->m_crFrom;
		else if(it->second->type==FILL_RADIAL)
			return ((CFillRadial*)it->second->fill)->m_crFrom;
	}

	return 0;
}

EXPRESSION(
	/* ID */			44,
	/* Name */			_T("PatternColorB("),
	/* Flags */			0,
	/* Params */		(1,EXPPARAM_STRING,_T("Pattern"))
) {
	string name(GetStr2());
	FillDB::iterator it = rdPtr->fill->find(name);
	if(it!=rdPtr->fill->end())
	{
		if(it->second->type==FILL_GRADIENT)
			return ((CFillDirEx*)it->second->fill)->m_crTo;
		else if(it->second->type==FILL_RADIAL)
			return ((CFillRadial*)it->second->fill)->m_crTo;
	}

	return 0;
}

EXPRESSION(
	/* ID */			45,
	/* Name */			_T("PatternImage("),
	/* Flags */			0,
	/* Params */		(1,EXPPARAM_STRING,_T("Pattern"))
) {
	string name(GetStr2());
	FillDB::iterator it = rdPtr->fill->find(name);
	if(it!=rdPtr->fill->end())
	{
		if(it->second->type==FILL_MOSAIC)
			return it->second->img;
	}

	return 0;
}

EXPRESSION(
	/* ID */			46,
	/* Name */			_T("ImgRedAt("),
	/* Flags */			0,
	/* Params */		(3,EXPPARAM_NUMBER,_T("Image"),EXPPARAM_NUMBER,_T("X"),EXPPARAM_NUMBER,_T("Y"))
) {
	int img = GetInt2();
	int x = GetXPos2();
	int y = GetYPos2();
	ImageExists(img);
	cSurface* tmp = Image(img);
	COLORREF col;
	tmp->GetPixel(x,y,col);
	return GetRValue(col);
}

EXPRESSION(
	/* ID */			47,
	/* Name */			_T("ImgGreenAt("),
	/* Flags */			0,
	/* Params */		(3,EXPPARAM_NUMBER,_T("Image"),EXPPARAM_NUMBER,_T("X"),EXPPARAM_NUMBER,_T("Y"))
) {
	int img = GetInt2();
	int x = GetXPos2();
	int y = GetYPos2();
	ImageExists(img);
	cSurface* tmp = Image(img);
	COLORREF col;
	tmp->GetPixel(x,y,col);
	return GetGValue(col);
}

EXPRESSION(
	/* ID */			48,
	/* Name */			_T("ImgBlueAt("),
	/* Flags */			0,
	/* Params */		(3,EXPPARAM_NUMBER,_T("Image"),EXPPARAM_NUMBER,_T("X"),EXPPARAM_NUMBER,_T("Y"))
) {
	int img = GetInt2();
	int x = GetXPos2();
	int y = GetYPos2();
	ImageExists(img);
	cSurface* tmp = Image(img);
	COLORREF col;
	tmp->GetPixel(x,y,col);
	return GetBValue(col);
}

EXPRESSION(
	/* ID */			49,
	/* Name */			_T("ImgAlphaAt("),
	/* Flags */			0,
	/* Params */		(3,EXPPARAM_NUMBER,_T("Image"),EXPPARAM_NUMBER,_T("X"),EXPPARAM_NUMBER,_T("Y"))
) {
	int img = GetInt2();
	int x = GetXPos2();
	int y = GetYPos2();
	ImageExists(img);
	int a;
	//Alpha
	cSurface* tmp = Image(img);
	if(tmp->HasAlpha())
	{
		cSurface* alpha = tmp->GetAlphaSurface();
		alpha->GetPixel(x,y,a);
		tmp->ReleaseAlphaSurface(alpha);
		return a;
	}
	//Transp. color
	else
	{
		tmp->GetPixel(x,y,a);
		return a==tmp->GetTransparentColor()?255:0;
	}
}

EXPRESSION(
	/* ID */			50,
	/* Name */			_T("HEX2RGB("),
	/* Flags */			0,
	/* Params */		(1,EXPPARAM_STRING,_T("HEX string"))
) {
	TCHAR* in = GetStr2();

	// Skip prefix
	if (_tcslen(in))
	{
		if (*in == '$' || *in == '#')
			in++;
		else if(in[0] == '0' && (in[1] == 'x' || in[1] == 'X'))
			in += 2;
	}

	// Not usable, return
	if(_tcslen(in) < 6)
		return 0;

	char str[7] = {in[4],in[5],in[2],in[3],in[0],in[1],0};
	return strtol(str, 0, 16);
}

EXPRESSION(
	/* ID */			51,
	/* Name */			_T("RandomColor("),
	/* Flags */			0,
	/* Params */		(0)
) {
	srand(rdPtr->randomSeed += rand());
	int r = rand();
	int g = rand();
	int b = rand();
	return RGB(r,g,b);
}

EXPRESSION(
	/* ID */			52,
	/* Name */			_T("SelImgRef("),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();
	return (long)TargetImg;
}

EXPRESSION(
	/* ID */			53,
	/* Name */			_T("ImgRef("),
	/* Flags */			0,
	/* Params */		(1,EXPPARAM_NUMBER,_T("Image"))
) {
	int img = GetInt2();
	ImageExists(img);
	return (long)Image(img);
}

EXPRESSION(
	/* ID */			54,
	/* Name */			_T("CallbackSrcCol("),
	/* Flags */			0,
	/* Params */		(0)
) {
	return rdPtr->colSrc;
}

EXPRESSION(
	/* ID */			55,
	/* Name */			_T("CallbackDestCol("),
	/* Flags */			0,
	/* Params */		(0)
) {
	return rdPtr->colDest;
}

EXPRESSION(
	/* ID */			56,
	/* Name */			_T("ObjImgRef("),
	/* Flags */			0,
	/* Params */		(2,EXPPARAM_NUMBER,_T("Fixed value"),EXPPARAM_NUMBER,_T("Parameter"))
) {
	int fixed = GetInt2();
	int type = GetInt2();
	LPRO o = rdPtr->rRd->Fixed2LPRO(fixed);
	if(!o) return 0;

	switch(o->roHo.hoIdentifier)
	{
	//Active picture
	case 'PICA':
		if(type==1 && ((ActivePicture*)o)->psf)
			return (long)((ActivePicture*)o)->psf;
		return (long)((ActivePicture*)o)->psfOrg;
	//Active shape
	case 'ASHP':
		return (long)((ActiveShape*)o)->pSf;
	//Surface
	case 'SURF':
		return (long)((RUNDATA*)o)->imageAt((LPRDATA)o,type);
	//Overlay
	case 'OVLY':
		return (long)((Overlay*)o)->runSurface;
	//Viewport
	case 'ViPo':
		if(type==1)
			return (long)((Viewport*)o)->rotateImg;
		return (long)((Viewport*)o)->sourceImg;
	//Perspective
	case 'pskt':
		return (long)((Perspective*)o)->TempImage;
	}

	return 0;
}

EXPRESSION(
	/* ID */			57,
	/* Name */			_T("BgImgRef("),
	/* Flags */			0,
	/* Params */		(0)
) {
	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	LPSURFACE ps = WinGetSurface((int)rhPtr->rhIdEditWin);
	return long(ps);
}

EXPRESSION(
	/* ID */			58,
	/* Name */			_T("CallbackSrcAlpha("),
	/* Flags */			0,
	/* Params */		(0)
) {
	return rdPtr->colAlphaSrc;
}

EXPRESSION(
	/* ID */			59,
	/* Name */			_T("FrameWindowHandle("),
	/* Flags */			0,
	/* Params */		(0)
) {
	LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
	return long(rhPtr->rhHEditWin);
}

EXPRESSION(
	/* ID */			60,
	/* Name */			_T("ImgHotX("),
	/* Flags */			0,
	/* Params */		(1,EXPPARAM_NUMBER,_T("Image"))
) {
	int img = GetInt2();
	ImageExists(img);
	return ImageS(img)->hotX;
}

EXPRESSION(
	/* ID */			61,
	/* Name */			_T("ImgHotY("),
	/* Flags */			0,
	/* Params */		(1,EXPPARAM_NUMBER,_T("Image"))
) {
	int img = GetInt2();
	ImageExists(img);
	return ImageS(img)->hotY;
}

EXPRESSION(
	/* ID */			62,
	/* Name */			_T("HotX("),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();
	return ImageS(rdPtr->targetId)->hotX;
}

EXPRESSION(
	/* ID */			63,
	/* Name */			_T("HotY("),
	/* Flags */			0,
	/* Params */		(0)
) {
	TargetExists();
	return ImageS(rdPtr->targetId)->hotY;
}

EXPRESSION(
	/* ID */			64,
	/* Name */			_T("TranspColor("),
	/* Flags */			0,
	/* Params */		(0)
) {
	int img = GetInt2();
	ImageExists(img);
	return Image(img)->GetTransparentColor();
}

EXPRESSION(
	/* ID */			65,
	/* Name */			_T("CallbackDestAlpha("),
	/* Flags */			0,
	/* Params */		(0)
) {
	return rdPtr->colAlphaDest;
}

EXPRESSION(
	/* ID */			66,
	/* Name */			_T("Add("),
	/* Flags */			0,
	/* Params */		(2,EXPPARAM_NUMBER,_T("Color A"),EXPPARAM_NUMBER,_T("Color B"))
) {
	COLORREF a = GetInt2();
	COLORREF b = GetInt2();

	int ar = GetRValue(a);
	int ag = GetGValue(a);
	int ab = GetBValue(a);
	int br = GetRValue(b);
	int bg = GetGValue(b);
	int bb = GetBValue(b);

	return RGB(max(0,min(255,ar+br)),max(0,min(255,ag+bg)),max(0,min(255,ab+bb)));
}

EXPRESSION(
	/* ID */			67,
	/* Name */			_T("Subtract("),
	/* Flags */			0,
	/* Params */		(2,EXPPARAM_NUMBER,_T("Color A"),EXPPARAM_NUMBER,_T("Color B"))
) {
	COLORREF a = GetInt2();
	COLORREF b = GetInt2();

	int ar = GetRValue(a);
	int ag = GetGValue(a);
	int ab = GetBValue(a);
	int br = GetRValue(b);
	int bg = GetGValue(b);
	int bb = GetBValue(b);

	return RGB(max(0,min(255,ar-br)),max(0,min(255,ag-bg)),max(0,min(255,ab-bb)));
}

EXPRESSION(
	/* ID */			68,
	/* Name */			_T("TransformedSurfaceAddr("),
	/* Flags */			0,
	/* Params */		(0)
) {
	if(rdPtr->display && rdPtr->display->IsValid())
		return (long)rdPtr->display;
	if(rdPtr->currentId >= 0)
		return (long)CurrentImg;
	return 0;
}

EXPRESSION(
	/* ID */			69,
	/* Name */			_T("XScale("),
	/* Flags */			EXPFLAG_DOUBLE,
	/* Params */		(0)
) {
	ReturnFloat(rdPtr->rc.rcScaleX);
}

EXPRESSION(
	/* ID */			70,
	/* Name */			_T("YScale("),
	/* Flags */			EXPFLAG_DOUBLE,
	/* Params */		(0)
) {
	ReturnFloat(rdPtr->rc.rcScaleY);
}

EXPRESSION(
	/* ID */			71,
	/* Name */			_T("Angle("),
	/* Flags */			0,
	/* Params */		(0)
) {
	ReturnFloat(rdPtr->rc.rcAngle);
}

EXPRESSION(
	/* ID */			72,
	/* Name */			_T("ScreenToImageX("),
	/* Flags */			0,
	/* Params */		(2,EXPPARAM_NUMBER,_T("Screen X"),EXPPARAM_NUMBER,_T("Screen Y"))
) {
	int screenX = ExParam(TYPE_INT);
	int screenY = ExParam(TYPE_INT);

	double angle = rdPtr->rc.rcAngle/180.0*PI;

	screenX -= rdPtr->rHo.hoX-rdPtr->rHo.hoImgXSpot;
	screenY -= rdPtr->rHo.hoY-rdPtr->rHo.hoImgYSpot;

	if(rdPtr->rc.rcAngle >= 0 && rdPtr->rc.rcAngle < 90)
	{
		screenY -= sin(angle)*CurrentImg->GetWidth();
	}
	else if(rdPtr->rc.rcAngle >= 90 && rdPtr->rc.rcAngle < 180)
	{
		screenY += cos(angle)*CurrentImg->GetHeight();
	}


	int rotatedX = cos(angle)*screenX - sin(angle)*screenY;
	rotatedX /= rdPtr->rc.rcScaleX;
	return rotatedX;
}


EXPRESSION(
	/* ID */			73,
	/* Name */			_T("ScreenToImageY("),
	/* Flags */			0,
	/* Params */		(2,EXPPARAM_NUMBER,_T("Screen X"),EXPPARAM_NUMBER,_T("Screen Y"))
) {
	int screenX = ExParam(TYPE_INT);
	int screenY = ExParam(TYPE_INT);

	double angle = rdPtr->rc.rcAngle/180.0*PI;

	screenX -= rdPtr->rHo.hoX-rdPtr->rHo.hoImgXSpot;
	screenY -= rdPtr->rHo.hoY-rdPtr->rHo.hoImgYSpot;

	if(rdPtr->rc.rcAngle >= 0 && rdPtr->rc.rcAngle < 90)
	{
		screenY -= sin(angle)*CurrentImg->GetWidth();
	}
	else if(rdPtr->rc.rcAngle >= 90 && rdPtr->rc.rcAngle < 180)
	{
		screenY += cos(angle)*CurrentImg->GetHeight();
	}

	int rotatedY = sin(angle)*screenX + cos(angle)*screenY;
	rotatedY /= rdPtr->rc.rcScaleY;
	return rotatedY;
}

EXPRESSION(
	/* ID */			74,
	/* Name */			_T("PatternType("),
	/* Flags */			0,
	/* Params */		(1,EXPPARAM_STRING,_T("Pattern"))
) {
	string name(GetStr2());
	FillDB::iterator it = rdPtr->fill->find(name);
	if(it!=rdPtr->fill->end())
		return it->second->type & (~FILL_IMAGE);

	return FILL_NONE;
}

EXPRESSION(
	/* ID */			75,
	/* Name */			_T("ComposedColor("),
	/* Flags */			0,
	/* Params */		(4,EXPPARAM_NUMBER,_T("Color A"),EXPPARAM_NUMBER,_T("Alpha A (0-255)"),EXPPARAM_NUMBER,_T("Color B"),EXPPARAM_NUMBER,_T("Alpha B (0-255)"))
) {
	COLORREF ca = GetInt2();
	int aa = GetInt2();
	COLORREF cb = GetInt2();
	int ab = GetInt2();

	int sr = GetRValue(ca);
	int sg = GetGValue(ca);
	int sb = GetBValue(ca);
	int dr = GetRValue(cb);
	int dg = GetGValue(cb);
	int db = GetBValue(cb);

	int a = aa+ab*(1-aa);
	int r = (sr*aa+dr*ab*(1-aa))/a;
	int g = (sg*aa+dg*ab*(1-aa))/a;
	int b = (sb*aa+db*ab*(1-aa))/a;

	return RGB(max(0,min(255,r)),max(0,min(255,g)),max(0,min(255,b)));
}

EXPRESSION(
	/* ID */			76,
	/* Name */			_T("ComposedAlpha("),
	/* Flags */			0,
	/* Params */		(2,EXPPARAM_NUMBER,_T("Alpha A (0-255)"),EXPPARAM_NUMBER,_T("Alpha B (0-255)"))
) {
	int sa = GetInt2();
	int da = GetInt2();
	int a = sa+da*(1-sa);
	return max(0, min(255, a));
}
