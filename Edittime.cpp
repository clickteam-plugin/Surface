// ============================================================================
//
// This file contains routines that are handled only during the Edittime,
// under the Frame and Event editors.
//
// Including creating, display, and setting up your object.
// 
// ============================================================================

#include "common.h"

// --------------------
// Properties
// --------------------

PROPS_IDS_START()
	//Image bank
	PROPID_GRP0,
		PROPID_IMAGES,
		PROPID_REMOVEDEF,
		PROPID_MULTIIMG,
		PROPID_LOADIMG,
		PROPID_DISPTARGET,
		PROPID_SELECTLAST,
	//Settings
	PROPID_GRP1,
		PROPID_USEABS,
		PROPID_KEEPPOINTS,
		PROPID_THREADEDIO,
		PROPID_RESAMPLING,
	//Font tab
		PROPID_TEXT_MULTILINE,
		PROPID_TEXT_NOCLIP,
		PROPID_TEXT_WORDELLIPSIS,
		PROPID_TEXT_WORDBREAK,
PROPS_IDS_END()

PROPS_DATA_START()
	PropData_Group(PROPID_GRP0,(int)"Image bank",(int)""),
	PropData_ImageList(PROPID_IMAGES,(int)"Images",(int)""),
	PropData_Button(PROPID_REMOVEDEF,(int)"",(int)"",(int)"Remove"),
	//PropData_CheckBox(PROPID_MULTIIMG,(int)"Allow multiple images",(int)"Actions to manage multiple images will be provided. Recommended over using multiple Surface objects	."),
	PropData_CheckBox(PROPID_LOADIMG,(int)"Load first image on start",(int)"Loads and displays image 0 if available on start."),
	PropData_CheckBox(PROPID_SELECTLAST,(int)"Select new images",(int)"When an image is added, it is automatically selected for editing."),
	PropData_CheckBox(PROPID_DISPTARGET,(int)"Display selected image",(int)"\"Set display image\" will not be available and the selected editing image is drawn instead."),
	PropData_Group(PROPID_GRP1,(int)"Settings",(int)""),
	PropData_CheckBox(PROPID_USEABS,(int)"Use absolute coordinates",(int)"Coordinates are not relative to the surface position."),
	PropData_CheckBox(PROPID_KEEPPOINTS,(int)"Keep polygon points after drawing",(int)"The polygon points will be kept after drawing one."),
	PropData_CheckBox(PROPID_THREADEDIO,(int)"Background file input/output",(int)"If checked, the application does not freeze while a file is being loaded or saved."),
	PropData_CheckBox(PROPID_RESAMPLING,(int)"Linear resampling for transformations",(int)"Some actions like \"Resize\" and \"Rotate\" can achieve better quality with this option."),
PROPS_DATA_END()

char* FontQuality[] =
{
	0,
	"Aliased",
	"Anti-aliased",
	"ClearType",
	0
};

PropData FontProps[] = {
	PropData_CheckBox(PROPID_TEXT_MULTILINE,(int)"Multi-line",(int)"If checked, line breaks will be rendered, but vertical alignment is not supported."),
	PropData_CheckBox(PROPID_TEXT_NOCLIP,(int)"No text clipping",(int)"If checked, the text won't be clipped if it exceeds the given rectangle."),
	PropData_CheckBox(PROPID_TEXT_WORDELLIPSIS,(int)"Add ellipsis",(int)"Truncates any word that does not fit in the rectangle and adds an ellipsis."),
	PropData_CheckBox(PROPID_TEXT_WORDBREAK,(int)"Break words",(int)"Break words that don't fit in the rectangle."),
	PropData_End()
};


// --------------------
// Debugger
// --------------------

DEBUGGER_IDS_START()

	// DB_CURRENTSTRING,

DEBUGGER_IDS_END()

DEBUGGER_ITEMS_START()

	//  DB_CURRENTSTRING,
	//	DB_CURRENTSTRING|DB_EDITABLE,
	//	DB_CURRENTVALUE|DB_EDITABLE,
	//	DB_CURRENTCHECK,
	//	DB_CURRENTCOMBO,

DEBUGGER_ITEMS_END()

// --------------------
// GetProperties
// --------------------
// Inserts properties into the properties of the object.
//

BOOL WINAPI DLLExport GetProperties(LPMV mV, LPEDATA edPtr, BOOL bMasterItem)
{
#ifndef RUN_ONLY
	mvInsertProps(mV, edPtr, Properties, PROPID_TAB_GENERAL, TRUE);
	mvInsertProps(mV, edPtr, FontProps, PROPID_TAB_TEXTOPT, TRUE);
#endif // !RUN_ONLY

	// OK
	return TRUE;
}

// --------------------
// ReleaseProperties
// --------------------
// Called when the properties are removed from the property window.
//

void WINAPI DLLExport ReleaseProperties(LPMV mV, LPEDATA edPtr, BOOL bMasterItem)
{
#ifndef RUN_ONLY
	// Write your code here
#endif // !RUN_ONLY
}

// --------------------
// GetPropCreateParam
// --------------------
// Called when a property is initialized and its creation parameter is NULL (in the PropData).
// Allows you, for example, to change the content of a combobox property according to specific settings in the EDITDATA structure.
//

LPARAM WINAPI DLLExport GetPropCreateParam(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY
	// Example
	// -------
//	if ( nPropID == PROPID_COMBO )
//	{
//		switch (edPtr->sType)
//		{
//		case TYPE1:
//			return (LPARAM)ComboList1;
//		case TYPE2:
//			return (LPARAM)ComboList2;
//		}
//	}
#endif // !RUN_ONLY
	return NULL;
}

// ----------------------
// ReleasePropCreateParam
// ----------------------
// Called after a property has been initialized.
// Allows you, for example, to free memory allocated in GetPropCreateParam.
//

void WINAPI DLLExport ReleasePropCreateParam(LPMV mV, LPEDATA edPtr, UINT nPropID, LPARAM lParam)
{
#ifndef RUN_ONLY
#endif // !RUN_ONLY
}

// --------------------
// GetPropValue
// --------------------
// Returns the value of properties that have a value.
// Note: see GetPropCheck for checkbox properties
//

LPVOID WINAPI DLLExport GetPropValue(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY
	// Example
	// -------
	switch (nPropID) {
		case PROPID_IMAGES:
			{
				CPropDataValue* pv = new CPropDataValue((edPtr->imageCount + 1) * sizeof(WORD), NULL);
				if (pv)
				{
					if (pv->m_pData)
					{
						LPWORD pw = (LPWORD)pv->m_pData;
						*pw++ = edPtr->imageCount;
						for (WORD w=0;w<edPtr->imageCount;w++)
							*pw++ = edPtr->images[w];
						return pv;
					}
					pv->Delete();
				}
				break;
			}
	}
#endif // !RUN_ONLY
	return NULL;
}

// --------------------
// GetPropCheck
// --------------------
// Returns the checked state of properties that have a check box.
//

BOOL WINAPI DLLExport GetPropCheck(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY
	// Example
	// -------
	switch (nPropID) {
		case PROPID_LOADIMG:
			return edPtr->loadFirst;
		case PROPID_MULTIIMG:
			return edPtr->multiImg;
		case PROPID_USEABS:
			return edPtr->useAbs;
		case PROPID_KEEPPOINTS:
			return edPtr->keepPoints;
		case PROPID_THREADEDIO:
			return edPtr->threadedIO;
		case PROPID_DISPTARGET:
			return edPtr->dispTarget;
		case PROPID_SELECTLAST:
			return edPtr->selectLast;
		case PROPID_RESAMPLING:
			return mvGetPropCheck(mV,edPtr,PROPID_FITEM_ANTIA);
		//Text
		case PROPID_TEXT_MULTILINE:
			return !(edPtr->textFlags&DT_SINGLELINE);
		case PROPID_TEXT_NOCLIP:
			return (edPtr->textFlags&DT_NOCLIP)==DT_NOCLIP;
		case PROPID_TEXT_WORDBREAK:
			return (edPtr->textFlags&DT_WORDBREAK)==DT_WORDBREAK;
		case PROPID_TEXT_WORDELLIPSIS:
			return (edPtr->textFlags&DT_WORD_ELLIPSIS)==DT_WORD_ELLIPSIS;
	}

#endif // !RUN_ONLY
	return 0;		// Unchecked
}

// --------------------
// SetPropValue
// --------------------
// This routine is called by MMF after a property has been modified.
//

void WINAPI DLLExport SetPropValue(LPMV mV, LPEDATA edPtr, UINT nPropID, LPVOID lParam)
{
#ifndef RUN_ONLY
	// Gets the pointer to the CPropValue structure
	CPropValue* pValue = (CPropValue*)lParam;

	// Example
	// -------
	switch (nPropID)
	{

	case PROPID_IMAGES:
		if(((CPropDataValue*)pValue)->m_pData)
		{
			LPWORD pw = (LPWORD)((CPropDataValue*)pValue)->m_pData;
			//edPtr->imageCount = *pw++;
            for (WORD w=0;w<edPtr->imageCount;w++)
                edPtr->images[w] = *pw++;
		}
		mvInvalidateObject(mV,edPtr);
		break;
	}

#endif // !RUN_ONLY
}

// --------------------
// SetPropCheck
// --------------------
// This routine is called by MMF when the user modifies a checkbox in the properties.
//

void WINAPI DLLExport SetPropCheck(LPMV mV, LPEDATA edPtr, UINT nPropID, BOOL nCheck)
{
#ifndef RUN_ONLY
	// Example
	// -------
	switch (nPropID)
	{
	case PROPID_LOADIMG:
		edPtr->loadFirst = nCheck;
		mvInvalidateObject(mV, edPtr);
		mvRefreshProp(mV, edPtr, PROPID_LOADIMG, true);
		break;
	case PROPID_MULTIIMG:
		edPtr->multiImg = nCheck;
		mvInvalidateObject(mV, edPtr);
		mvRefreshProp(mV, edPtr, PROPID_MULTIIMG, true);
		mvRefreshProp(mV, edPtr, PROPID_LOADIMG, true);
		mvRefreshProp(mV, edPtr, PROPID_SELECTLAST, true);
		mvRefreshProp(mV, edPtr, PROPID_DISPTARGET, true);
		break;
	case PROPID_USEABS:
		edPtr->useAbs = nCheck;
		mvInvalidateObject(mV, edPtr);
		mvRefreshProp(mV, edPtr, PROPID_USEABS, true);
		break;
	case PROPID_KEEPPOINTS:
		edPtr->keepPoints = nCheck;
		mvInvalidateObject(mV, edPtr);
		mvRefreshProp(mV, edPtr, PROPID_KEEPPOINTS, true);
		break;
	case PROPID_THREADEDIO:
		edPtr->threadedIO = nCheck;
		mvInvalidateObject(mV, edPtr);
		mvRefreshProp(mV, edPtr, PROPID_THREADEDIO, true);
		break;
	case PROPID_DISPTARGET:
		edPtr->dispTarget = nCheck;
		mvInvalidateObject(mV, edPtr);
		mvRefreshProp(mV, edPtr, PROPID_DISPTARGET, true);
		break;
	case PROPID_SELECTLAST:
		edPtr->selectLast = nCheck;
		mvInvalidateObject(mV, edPtr);
		mvRefreshProp(mV, edPtr, PROPID_SELECTLAST, true);
		break;
	case PROPID_RESAMPLING:
		mvSetPropCheck(mV,edPtr,PROPID_FITEM_ANTIA,nCheck);
		mvInvalidateObject(mV, edPtr);
		mvRefreshProp(mV, edPtr, PROPID_RESAMPLING, true);
		mvRefreshProp(mV, edPtr, PROPID_FITEM_ANTIA, true);
		break;
	//Font
	case PROPID_TEXT_MULTILINE:
		if(nCheck)
			edPtr->textFlags &= ~DT_SINGLELINE;
		else
			edPtr->textFlags |= DT_SINGLELINE;
		mvInvalidateObject(mV, edPtr);
		mvRefreshProp(mV, edPtr, PROPID_TEXT_MULTILINE, true);
		break;
	case PROPID_TEXT_NOCLIP:
		if(nCheck)
			edPtr->textFlags |= DT_NOCLIP;
		else
			edPtr->textFlags &= ~DT_NOCLIP;
		mvInvalidateObject(mV, edPtr);
		mvRefreshProp(mV, edPtr, PROPID_TEXT_NOCLIP, true);
		break;
	case PROPID_TEXT_WORDBREAK:
		if(nCheck)
			edPtr->textFlags |= DT_WORDBREAK;
		else
			edPtr->textFlags &= ~DT_WORDBREAK;
		mvInvalidateObject(mV, edPtr);
		mvRefreshProp(mV, edPtr, PROPID_TEXT_WORDBREAK, true);
		break;
	case PROPID_TEXT_WORDELLIPSIS:
		if(nCheck)
			edPtr->textFlags |= DT_WORD_ELLIPSIS;
		else
			edPtr->textFlags &= ~DT_WORD_ELLIPSIS;
		mvInvalidateObject(mV, edPtr);
		mvRefreshProp(mV, edPtr, PROPID_TEXT_WORDELLIPSIS, true);
		break;
	}
#endif // !RUN_ONLY
}

// --------------------
// EditProp
// --------------------
// This routine is called when the user clicks the button of a Button or EditButton property.
//

BOOL WINAPI DLLExport EditProp(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY
 
	// Example
	// -------

	if (nPropID==PROPID_REMOVEDEF)
	{
		int kill = MessageBox(0,"Do you really want to delete all images?","Warning",MB_YESNO|MB_ICONWARNING);
		if(kill==IDYES)
		{
			mvInvalidateObject(mV,edPtr);
			for(int i=0;i<MAX_IMAGES;i++)
				edPtr->images[i] = 0;
			edPtr->imageCount = 0;
			//edPtr->width = 320;
			//edPtr->height = 240;
			mvRefreshProp(mV, edPtr, PROPID_LOADIMG, true);
			mvRefreshProp(mV, edPtr, PROPID_IMAGES, true);
			return true;
		}
		return false;
	}
	else if(nPropID==PROPID_IMAGES)
	{
		mvInvalidateObject(mV,edPtr);
		EditAnimationParams eip;
		eip.m_dwSize = sizeof(EditAnimationParams);
		eip.m_pWindowTitle = 0;
		eip.m_nImages = edPtr->imageCount;
		eip.m_nMaxImages = MAX_IMAGES;
		eip.m_nStartIndex = 0;
		eip.m_pImages = &edPtr->images[0];
		eip.m_pImageTitles = 0;
		eip.m_dwOptions = PICTEDOPT_CANBETRANSPARENT|PICTEDOPT_HOTSPOT;
		eip.m_dwFixedWidth = edPtr->width?edPtr->width:DEF_WIDTH;
		eip.m_dwFixedHeight = edPtr->height?edPtr->height:DEF_HEIGHT;
		BOOL output = mV->mvEditAnimation(edPtr,&eip,mV->mvHEditWin);
		//Update object count
		edPtr->imageCount = eip.m_nImages;
		//Update object dimensions
		if(output&&edPtr->images[0])
		{
			cSurface is;
			LockImageSurface(mV->mvIdAppli,edPtr->images[0],is);
			edPtr->width = edPtr->width_def = is.GetWidth();
			edPtr->height = edPtr->height_def = is.GetHeight();
			UnlockImageSurface(is);
		}
		mvRefreshProp(mV, edPtr, PROPID_LOADIMG, true);
		mvRefreshProp(mV, edPtr, PROPID_IMAGES, true);
		return output;
	}


#endif // !RUN_ONLY
	return FALSE;
}

// --------------------
// IsPropEnabled
// --------------------
// This routine returns the enabled state of a property.
//

BOOL WINAPI IsPropEnabled(LPMV mV, LPEDATA edPtr, UINT nPropID)
{
#ifndef RUN_ONLY
	// Example
	// -------

	switch (nPropID) {

	case PROPID_LOADIMG:
		return edPtr->multiImg||!edPtr->imageCount;
		break;

	case PROPID_SELECTLAST:
		return edPtr->multiImg;
		break;

	case PROPID_DISPTARGET:
		return edPtr->multiImg;
		break;

	}

#endif // !RUN_ONLY
	return TRUE;
}


// ============================================================================
//
// TEXT PROPERTIES
// 
// ============================================================================

// --------------------
// GetTextCaps
// --------------------
// Return the text capabilities of the object under the frame editor.
//

DWORD WINAPI DLLExport GetTextCaps(mv _far *mV, LPEDATA edPtr)
{
	return (TEXT_ALIGN_LEFT|TEXT_ALIGN_HCENTER|TEXT_ALIGN_RIGHT|TEXT_ALIGN_TOP|TEXT_ALIGN_VCENTER|TEXT_ALIGN_BOTTOM|TEXT_FONT);
}

// --------------------
// GetTextFont
// --------------------
// Return the font used the object.
// Note: the pStyle and cbSize parameters are obsolete and passed for compatibility reasons only.
//

BOOL WINAPI DLLExport GetTextFont(mv _far *mV, LPEDATA edPtr, LPLOGFONT plf, LPSTR pStyle, UINT cbSize)
{
#if !RUN_ONLY
	// Example: copy LOGFONT structure from EDITDATA
	memcpy(plf, &edPtr->textFont, sizeof(LOGFONT));
#endif // !RUN_ONLY

	return TRUE;
}

// --------------------
// SetTextFont
// --------------------
// Change the font used the object.
// Note: the pStyle parameter is obsolete and passed for compatibility reasons only.
//

BOOL WINAPI DLLExport SetTextFont(mv _far *mV, LPEDATA edPtr, LPLOGFONT plf, LPCSTR pStyle)
{
#if !RUN_ONLY
	// Example: copy LOGFONT structure to EDITDATA
	memcpy(&edPtr->textFont, plf, sizeof(LOGFONT));
#endif // !RUN_ONLY

	return TRUE;
}

// --------------------
// GetTextClr
// --------------------
// Get the text color of the object.
//

COLORREF WINAPI DLLExport GetTextClr(mv _far *mV, LPEDATA edPtr)
{
	// Example
	return  edPtr->textColor;
}

// --------------------
// SetTextClr
// --------------------
// Set the text color of the object.
//

void WINAPI DLLExport SetTextClr(mv _far *mV, LPEDATA edPtr, COLORREF color)
{
	// Example
	edPtr->textColor = color;
}

// --------------------
// GetTextAlignment
// --------------------
// Get the text alignment of the object.
//

DWORD WINAPI DLLExport GetTextAlignment(mv _far *mV, LPEDATA edPtr)
{
	DWORD dw = 0;
#if !RUN_ONLY
	// Example
	// -------

	if ( (edPtr->textFlags & DT_LEFT) != 0 )
		dw |= TEXT_ALIGN_LEFT;
	if ( (edPtr->textFlags & DT_CENTER) != 0 )
		dw |= TEXT_ALIGN_HCENTER;
	if ( (edPtr->textFlags & DT_RIGHT) != 0 )
		dw |= TEXT_ALIGN_RIGHT;
	if ( (edPtr->textFlags & DT_TOP) != 0 )
		dw |= TEXT_ALIGN_TOP;
	if ( (edPtr->textFlags & DT_VCENTER) != 0 )
		dw |= TEXT_ALIGN_VCENTER;
	if ( (edPtr->textFlags & DT_BOTTOM) != 0 )
		dw |= TEXT_ALIGN_BOTTOM;

#endif // !RUN_ONLY
	return dw;
}

// --------------------
// SetTextAlignment
// --------------------
// Set the text alignment of the object.
//

void WINAPI DLLExport SetTextAlignment(mv _far *mV, LPEDATA edPtr, DWORD dwAlignFlags)
{
#if !RUN_ONLY
	// Example
	// -------
	DWORD dw = edPtr->textFlags;

	if ( (dwAlignFlags & TEXT_ALIGN_LEFT) != 0 )
		dw = (dw & ~(DT_LEFT|DT_RIGHT|DT_CENTER)) | DT_LEFT;
	if ( (dwAlignFlags & TEXT_ALIGN_HCENTER) != 0 )
		dw = (dw & ~(DT_LEFT|DT_RIGHT|DT_CENTER)) | DT_CENTER;
	if ( (dwAlignFlags & TEXT_ALIGN_RIGHT) != 0 )
		dw = (dw & ~(DT_LEFT|DT_RIGHT|DT_CENTER)) | DT_RIGHT;

	if ( (dwAlignFlags & TEXT_ALIGN_TOP) != 0 )
		dw = (dw & ~(DT_TOP|DT_VCENTER|DT_RIGHT)) | DT_TOP;
	if ( (dwAlignFlags & TEXT_ALIGN_VCENTER) != 0 )
		dw = (dw & ~(DT_TOP|DT_VCENTER|DT_RIGHT)) | DT_VCENTER;
	if ( (dwAlignFlags & TEXT_ALIGN_BOTTOM) != 0 )
		dw = (dw & ~(DT_TOP|DT_VCENTER|DT_RIGHT)) | DT_BOTTOM;

	edPtr->textFlags = dw;

#endif // !RUN_ONLY
}


// -----------------
// BmpToImg
// -----------------
// Converts an image from the resource to an image displayable under MMF2
// Not used in this template, but it is a good example on how to create
// an image.
//

/*
WORD BmpToImg(int bmID, npAppli idApp, short HotX = 0, short HotY = 0, short ActionX = 0, short ActionY = 0)
{
	Img					ifo;
	WORD				img;
	HRSRC				hs;
	HGLOBAL				hgBuf;
	LPBYTE				adBuf;
	LPBITMAPINFOHEADER	adBmi;

	img = 0;
	if ((hs = FindResource(hInstLib, MAKEINTRESOURCE(bmID), RT_BITMAP)) != NULL)
	{
		if ((hgBuf = LoadResource(hInstLib, hs)) != NULL)
		{
			if ((adBuf = (LPBYTE)LockResource(hgBuf)) != NULL)
			{
				adBmi = (LPBITMAPINFOHEADER)adBuf;
				ifo.imgXSpot = HotX;
				ifo.imgYSpot = HotY;
				ifo.imgXAction = ActionX;
				ifo.imgYAction = ActionY;
				if (adBmi->biBitCount > 4)
					RemapDib((LPBITMAPINFO)adBmi, idApp, NULL);
				img = (WORD)DibToImage(idApp, &ifo, adBmi);
				UnlockResource(hgBuf);
			}
			FreeResource(hgBuf);
		}
	}
	return img;
}
*/

// ============================================================================
//
// ROUTINES USED UNDER FRAME EDITOR
// 
// ============================================================================

// --------------------
// MakeIcon
// --------------------
// Called once object is created or modified, just after setup.
//
// Note: this function is optional. If it's not defined in your extension,
// MMF2 will load the EXO_ICON bitmap if it's defined in your resource file.
//
// If you need to draw the icon manually, remove the comments around this function and in the .def file.
//


int WINAPI DLLExport MakeIconEx ( mv _far *mV, cSurface* pIconSf, LPSTR lpName, fpObjInfo oiPtr, LPEDATA edPtr )
{
	int error = -1;
#ifndef RUN_ONLY
	//Load icon
	if((edPtr->multiImg&&!edPtr->loadFirst)||!edPtr->images[0])
	{
		if(pIconSf->LoadImage(hInstLib, EXO_ICON))
		{
			error = 0;
			pIconSf->SetTransparentColor(0xff00ff);
		}
	}
	//Draw image
	else
	{
		cSurface is;
		LockImageSurface(mV->mvIdAppli,edPtr->images[0],is);
		is.Minimize();
		//Draw grid
		for(int x=0;x<7;x++)
			for(int y=0;y<7;y++)
				pIconSf->Rectangle(x*4+2,y*4+2,x*4+6,y*4+6,(x+y)%2?LIGHT_GRAY:WHITE,0,0,1);

		int sw = is.GetWidth(), sh = is.GetHeight();
		int dw,dh;
		
		//Square
		if(sw==sh)
		{
			dw = min(28,sw);
			dh = dw;
		}
		//Wide
		else if(sw>sh)
		{
			dw = min(28,sw);
			dh = sh*1.0/sw*dw;
		}
		//Tall
		else
		{
			dh = min(28,sh);
			dw = sw*1.0/sh*dh;
		}
		//Draw image
		if(is.Stretch(*pIconSf,16-dw/2,16-dh/2,dw,dh,BMODE_TRANSP,BOP_COPY,0,STRF_RESAMPLE_TRANSP))
			error = 0;
		//Get transparent color
		COLORREF trans = is.GetTransparentColor();
		pIconSf->SetTransparentColor(trans);
		UnlockImageSurface(is);
		//Outline
		pIconSf->Rectangle(0,0,32,32,1,0x333333);
		CFillFlat white(WHITE);
		pIconSf->Rectangle(1,1,31,31,1,&white,0,BMODE_OPAQUE,BOP_BLEND,32);
		//Smooth border
		pIconSf->SetPixel(0,0,trans);
		pIconSf->SetPixel(31,31,trans);
		pIconSf->SetPixel(31,0,trans);
		pIconSf->SetPixel(0,31,trans);
	}
#endif // !RUN_ONLY
	return error;
}	



// --------------------
// CreateObject
// --------------------
// Called when you choose "Create new object". It should display the setup box 
// and initialize everything in the datazone.
void WINAPI	DLLExport CreateFromFile (LPMV mV, LPSTR fileName, LPEDATA edPtr);
int WINAPI DLLExport CreateObject(mv _far *mV, fpLevObj loPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY

	// Do some rSDK stuff
	#include "rCreateObject.h"
	
	CreateFromFile(mV,0,edPtr);

	return 0;	// No error

#endif // !RUN_ONLY

	// Error
	return -1;
}

// --------------------
// EditObject
// --------------------
// One of the option from the menu has been selected, and not a default menu option
// automatically handled by MMF2: this routine is then called.
//

BOOL WINAPI EditObject (mv _far *mV, fpObjInfo oiPtr, fpLevObj loPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	
	return EditProp(mV,edPtr,PROPID_IMAGES);

#endif // !RUN_ONLY

	// Error
	return FALSE;
}

// --------------------
// SetEditSize
// --------------------
// Called by MMF2 when the object has been resized
//
// Note: remove the comments if your object can be resized (and remove the comments in the .def file)


BOOL WINAPI SetEditSize(LPMV mv, LPEDATA edPtr, int cx, int cy)
{
#ifndef RUN_ONLY
	//Single, empty image
	if(edPtr->loadFirst&&!edPtr->images[0]) //&&!edPtr->multiImg
	{
		edPtr->width = edPtr->width_def = cx;
		edPtr->height = edPtr->height_def = cy;
	}
	//Multiple images or single image
	else if(edPtr->images[0]&&(edPtr->loadFirst||!edPtr->multiImg))
	{
		mvInvalidateObject(mv, edPtr);
		cSurface is, tmp;
		LockImageSurface(mv->mvIdAppli,edPtr->images[0],is);
		tmp.Clone(is,cx,cy);
		is.Delete();
		is.Clone(tmp);
		Img old;
		GetImageInfos(mv->mvIdAppli,edPtr->images[0],&old);
		edPtr->images[0] = CreateImageFromSurface(mv,&is,cx,cy,old.imgXSpot*1.0f*cx/old.imgWidth,old.imgYSpot*1.0f*cy/old.imgHeight,0,0);
		UnlockImageSurface(is);
		edPtr->width = edPtr->width_def = cx;
		edPtr->height = edPtr->height_def = cy;
	}
#endif // !RUN_ONLY
	return TRUE;	// OK
}


// --------------------
// PutObject
// --------------------
// Called when each individual object is dropped in the frame.
//

void WINAPI	DLLExport PutObject(mv _far *mV, fpLevObj loPtr, LPEDATA edPtr, ushort cpt)
{
#ifndef RUN_ONLY
#endif // !RUN_ONLY
}

// --------------------
// RemoveObject
// --------------------
// Called when each individual object is removed from the frame.
//

void WINAPI	DLLExport RemoveObject(mv _far *mV, fpLevObj loPtr, LPEDATA edPtr, ushort cpt)
{
#ifndef RUN_ONLY
	// Is the last object removed?
    if (0 == cpt)
	{
		// Do whatever necessary to remove our data
	}
#endif // !RUN_ONLY
}

// --------------------
// DuplicateObject
// --------------------
// Called when an object is created from another one (note: should be called CloneObject instead...)
//

void WINAPI DLLExport DuplicateObject(mv __far *mV, fpObjInfo oiPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
#endif // !RUN_ONLY
}

// --------------------
// GetObjectRect
// --------------------
// Returns the size of the rectangle of the object in the frame editor.
// 

void WINAPI DLLExport GetObjectRect(mv _far *mV, RECT FAR *rc, fpLevObj loPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	int width = 32;
	int height = 32;

	int hotX = 0;
	int hotY = 0;

	//Display first image or don't use multiple images
	if(edPtr->loadFirst||!edPtr->multiImg)
	{
		//First image is valid
		if(edPtr->images[0])
		{
			width = edPtr->width_def;
			height = edPtr->height_def;
			//Get hotspot
			Img imgInfo;
			GetImageInfos(mV->mvIdAppli,edPtr->images[0],&imgInfo);
			hotX = imgInfo.imgXSpot;
			hotY = imgInfo.imgYSpot;
		}
		//No images in the bank
		else if(!edPtr->images[0]&&edPtr->loadFirst) //!edPtr->multiImg)
		{
			width = edPtr->width;
			height = edPtr->height;
		}
	}

	//Apply hotspot
	rc->left -= hotX;
	rc->top -= hotY;

	//Apply size
	if(width==0) width = DEF_WIDTH;
	if(height==0) height = DEF_HEIGHT;
	rc->right = rc->left + width;
	rc->bottom = rc->top + height;
#endif // !RUN_ONLY
	return;
}


// --------------------
// EditorDisplay
// --------------------
// Displays the object under the frame editor
//
// Note: this function is optional. If it's not defined in your extension,
// MMF2 will load and display the EXO_IMAGE bitmap if it's defined in your resource file.
//
// If you need to draw the icon manually, remove the comments around this function and in the .def file.
//


void WINAPI DLLExport EditorDisplay(mv _far *mV, fpObjInfo oiPtr, fpLevObj loPtr, LPEDATA edPtr, RECT FAR *rc)
{
#ifndef RUN_ONLY

	// This is a simple case of drawing an image onto MMF's frame editor window
	// First, we must get a pointer to the surface used by the frame editor


	LPSURFACE ps = WinGetSurface((int)mV->mvIdEditWin);
	if(!ps) return;
	cSurface is;
	//Draw extension icon
	if((edPtr->multiImg||!edPtr->images[0])&&(!edPtr->loadFirst||!edPtr->images[0])) {
		is.Create(4,4,ps);
		is.LoadImage(hInstLib,EXO_IMAGE,LI_REMAP);
		is.SetTransparentColor(0xff00ff);
		//Also draw surface borders
		if(edPtr->loadFirst&&!edPtr->images[0])
		{
			//Create tile
			cSurface* proto;
			GetSurfacePrototype(&proto, 24, ST_MEMORY, SD_DIB);
			cSurface tile;
			tile.Create(16,16, proto);
			tile.Fill(LIGHT_GRAY);
			tile.Rectangle(0,0,8,8,DARK_GRAY,0,0,TRUE);
			tile.Rectangle(16,16,8,8,DARK_GRAY,0,0,TRUE);
			CFillMosaic mosaic(&tile);
			CFillFlat light(LIGHT_GRAY);
			CFillFlat dark(DARK_GRAY);
			CFillFlat outline(BLACK);
			//Draw shapes
			ps->Rectangle(rc->left, rc->top, rc->right, rc->bottom, &mosaic, 1, &outline, 0, BMODE_TRANSP, BOP_BLEND, 32);
			//Icon
			if(rc->right - rc->left >=32 && rc->bottom - rc->top >= 32)
				is.Blit(*ps, (rc->right + rc->left)/2 - 16, (rc->bottom + rc->top)/2 - 16, BMODE_TRANSP);
		}
		//Icon only
		else
		{
			is.Blit(*ps,rc->left,rc->top,BMODE_TRANSP);
		}	
	}
	//Draw default image
	else
	{
		BlitMode blitMode = oiPtr->oiHdr.oiInkEffect & EFFECTFLAG_TRANSPARENT ? BMODE_TRANSP : BMODE_OPAQUE;
		LockImageSurface(mV->mvIdAppli,edPtr->images[0],is);
		is.Blit(*ps,rc->left,rc->top,blitMode,(BlitOp)(oiPtr->oiHdr.oiInkEffect & EFFECT_MASK),oiPtr->oiHdr.oiInkEffectParam);
		UnlockImageSurface(is);
	}

#endif // !RUN_ONLY
}


// --------------------
// IsTransparent
// --------------------
// This routine tells MMF2 if the mouse pointer is over a transparent zone of the object.
// 

extern "C" BOOL WINAPI DLLExport IsTransparent(mv _far *mV, fpLevObj loPtr, LPEDATA edPtr, int dx, int dy)
{
#ifndef RUN_ONLY
	if (edPtr->images[0]&&edPtr->loadFirst) {
		BOOL out = FALSE;
		cSurface is;
		LockImageSurface(mV->mvIdAppli,edPtr->images[0],is);
		//Alpha channel
		if(is.HasAlpha())
			out = is.GetAlphaSurface()->GetPixelFast(dx,dy)==0;
		//Transparent color
		else out = is.GetTransparentColor()==is.GetPixelFast(dx,dy);
		UnlockImageSurface(is);
		return out;
	}
#endif // !RUN_ONLY
	return FALSE;
}

// --------------------
// PrepareToWriteObject
// --------------------
// Just before writing the datazone when saving the application, MMF2 calls this routine.
// 

void WINAPI	DLLExport PrepareToWriteObject(mv _far *mV, LPEDATA edPtr, fpObjInfo adoi)
{
#ifndef RUN_ONLY
	// Write your code here
#endif // !RUN_ONLY
}

// --------------------
// GetFilters
// --------------------
//

BOOL WINAPI GetFilters(LPMV mV, LPEDATA edPtr, DWORD dwFlags, LPVOID pReserved)
{
#ifndef RUN_ONLY
	if (dwFlags&GETFILTERS_IMAGES)
		return TRUE;
#endif // RUN_ONLY
	return FALSE;
}

// --------------------
// UsesFile
// --------------------
// Triggers when a file is dropped onto the frame
// Return TRUE if you can create an object from the given file
//

BOOL WINAPI	DLLExport UsesFile (LPMV mV, LPSTR fileName)
{
	BOOL r = FALSE;
#ifndef RUN_ONLY

	r = FindFilter(mV->mvImgFilterMgr,fileName)>0;

#endif // !RUN_ONLY
	return r;
}


// --------------------
// CreateFromFile
// --------------------
// Creates a new object from file
//

void WINAPI	DLLExport CreateFromFile (LPMV mV, LPSTR fileName, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	// Initialize your extension data from the given file

	edPtr->width = edPtr->width_def = DEF_WIDTH;
	edPtr->height = edPtr->height_def = DEF_HEIGHT;
	edPtr->loadFirst = true;
	edPtr->useAbs = false;
	edPtr->threadedIO = false;
	edPtr->keepPoints = false;
	edPtr->multiImg = true;
	edPtr->dispTarget = false;
	edPtr->selectLast = false;

	if (mV->mvGetDefaultFont)
		mV->mvGetDefaultFont(&edPtr->textFont, NULL, 0);
	else
	{
		edPtr->textFont.lfWidth = 0;
		edPtr->textFont.lfHeight = 13;
		strcpy((char*)edPtr->textFont.lfFaceName,"Arial");
		edPtr->textFont.lfItalic = 0;
		edPtr->textFont.lfWeight = 0;
		edPtr->textFont.lfUnderline = 0;
		edPtr->textFont.lfStrikeOut = 0;
	}

	edPtr->textColor = BLACK;
	edPtr->textFlags = DT_NOCLIP|DT_SINGLELINE;

	//Empty images
	for(int i=1;i<MAX_IMAGES;i++)
		edPtr->images[i] = 0;
	edPtr->imageCount = 0;

	//Load image from file
	if(fileName)
	{
		//Load image from file
		cSurface surf;
		DWORD filter = FindFilter(mV->mvImgFilterMgr,fileName);
		BOOL import = ImportImage(mV->mvImgFilterMgr,fileName,&surf,&filter,0);
		//Success! Woo!
		if(import)
		{
			edPtr->width = edPtr->width_def = surf.GetWidth();
			edPtr->height = edPtr->height_def = surf.GetHeight();
			edPtr->images[0] = CreateImageFromSurface(mV,&surf,edPtr->width,edPtr->height,0,0,0,0);
			edPtr->imageCount = 1;
		}
	}

#endif // !RUN_ONLY
}



// ============================================================================
//
// ROUTINES USED UNDER EVENT / TIME / STEP-THROUGH EDITOR
// You should not need to change these routines
// 
// ============================================================================

// -----------------
// menucpy
// -----------------
// Internal routine used later, copy one menu onto another
// 
#ifndef RUN_ONLY
void menucpy(HMENU hTargetMenu, HMENU hSourceMenu)
{
	int			n, id, nMn;
	NPSTR		strBuf;
	HMENU		hSubMenu;

	nMn = GetMenuItemCount(hSourceMenu);
	strBuf = (NPSTR)LocalAlloc(LPTR, 80);
	for (n=0; n<nMn; n++)
	{
		if (0 == (id = GetMenuItemID(hSourceMenu, n)))
			AppendMenu(hTargetMenu, MF_SEPARATOR, 0, 0L);
		else
		{
			GetMenuString(hSourceMenu, n, strBuf, 80, MF_BYPOSITION);
			if (strBuf[0] == '~')
			{
				AppendMenu(hTargetMenu, MF_DISABLED | MF_GRAYED, id, strBuf+1);
			}
			else
			{
				if (id != -1)
					AppendMenu(hTargetMenu, GetMenuState(hSourceMenu, n, MF_BYPOSITION), id, strBuf);
				else
				{
					hSubMenu = CreatePopupMenu();
					AppendMenu(hTargetMenu, MF_POPUP | MF_STRING, (uint)hSubMenu, strBuf);
					menucpy(hSubMenu, GetSubMenu(hSourceMenu, n));
				}
			}
		}
	}
	LocalFree((HLOCAL)strBuf);
} 

// -----------------
// GetPopupMenu
// -----------------
// Internal routine used later. Returns the first popup from a menu
// 
HMENU GetPopupMenu(LPEDATA edPtr,short mn)
{
	HMENU hPopup=CreatePopupMenu();
	
	if(mn == MN_CONDITIONS)
		menucpy(hPopup,ConditionMenu(edPtr));
	else if(mn == MN_ACTIONS)
		menucpy(hPopup,ActionMenu(edPtr));
	else if(mn == MN_EXPRESSIONS)
		menucpy(hPopup,ExpressionMenu(edPtr));

	return hPopup;
}

// --------------------
// GetEventInformations
// --------------------
// Internal routine used later. Look for one event in one of the eventInfos array...
// No protection to go faster: you must properly enter the conditions/actions!
//

static LPEVENTINFOS2 GetEventInformations(LPEVENTINFOS2 eiPtr, short code)

{
	while(eiPtr->infos.code != code)
		eiPtr = EVINFO2_NEXT(eiPtr);
	
	return eiPtr;
}
#endif // !RUN_ONLY


// ----------------------------------------------------
// GetConditionMenu / GetActionMenu / GetExpressionMenu
// ----------------------------------------------------
// Load the condition/action/expression menu from the resource, eventually
// enable or disable some options, and returns it to MMF2.
//

HMENU WINAPI DLLExport GetConditionMenu(mv _far *mV, fpObjInfo oiPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	// Check compatibility
	return GetPopupMenu(edPtr,MN_CONDITIONS);
#endif // !RUN_ONLY
	return NULL;
}

HMENU WINAPI DLLExport GetActionMenu(mv _far *mV, fpObjInfo oiPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	// Check compatibility
	return GetPopupMenu(edPtr,MN_ACTIONS);
#endif // !RUN_ONLY
	return NULL;
}

HMENU WINAPI DLLExport GetExpressionMenu(mv _far *mV, fpObjInfo oiPtr, LPEDATA edPtr)
{
#ifndef RUN_ONLY
	// Check compatibility
	return GetPopupMenu(edPtr,MN_EXPRESSIONS);
#endif // !RUN_ONLY
	return NULL;
}


// -------------------------------------------------------
// GetConditionTitle / GetActionTitle / GetExpressionTitle
// -------------------------------------------------------
// Returns the title of the dialog box displayed when entering
// parameters for the condition, action or expressions, if any.
// Here, we simply return the title of the menu option
//

#ifndef RUN_ONLY
void GetCodeTitle(LPEVENTINFOS2 eiPtr, short code, short param, short mn, LPSTR strBuf, WORD maxLen)
{
	HMENU		hMn;

	// Finds event in array
	eiPtr=GetEventInformations(eiPtr, code);

	// If a special string is to be returned
	short strID = EVINFO2_PARAMTITLE(eiPtr, param);

	if(strID) {
		switch(mn) {
		case MN_CONDITIONS:
			if(code>=0&&code<(short)Conditions.size()) {
				if(param>=0&&param<(short)Conditions[code]->getParamCount())
					strcpy(strBuf,Conditions[code]->getParamName(param));
			}
			break;
		case MN_ACTIONS:
			if(code>=0&&code<(short)Actions.size()) {
				if(param>=0&&param<(short)Actions[code]->getParamCount())
					strcpy(strBuf,Actions[code]->getParamName(param));
			}
			break;
		case MN_EXPRESSIONS:
			if(code>=0&&code<(short)Expressions.size()) {
				if(param>=0&&param<(short)Expressions[code]->getParamCount())
					strcpy(strBuf,Expressions[code]->getParamName(param));
			}
			break;
		}
	} else {
		if((hMn=LoadMenu(hInstLib, MAKEINTRESOURCE(mn)))) {
			GetMenuString(hMn, eiPtr->menu, strBuf, maxLen, MF_BYCOMMAND);
			DestroyMenu(hMn);
		}
	}
}
#else
#define GetCodeTitle(a,b,c,d,e,f)
#endif // !RUN_ONLY

void WINAPI DLLExport GetConditionTitle(mv _far *mV, short code, short param, LPSTR strBuf, short maxLen) {
	GetCodeTitle((LPEVENTINFOS2)conditionsInfos, code, param, MN_CONDITIONS, strBuf, maxLen);
}
void WINAPI DLLExport GetActionTitle(mv _far *mV, short code, short param, LPSTR strBuf, short maxLen) {
	GetCodeTitle((LPEVENTINFOS2)actionsInfos, code, param, MN_ACTIONS, strBuf, maxLen);
}
void WINAPI DLLExport GetExpressionTitle(mv _far *mV, short code, LPSTR strBuf, short maxLen) {
	GetCodeTitle((LPEVENTINFOS2)expressionsInfos, code, 0, MN_EXPRESSIONS, strBuf, maxLen);
}

// -------------------------------------------------------
// GetConditionTitle / GetActionTitle / GetExpressionTitle
// -------------------------------------------------------
// From a menu ID, these routines returns the code of the condition,
// action or expression, as defined in the .H file
//

short WINAPI DLLExport GetConditionCodeFromMenu(mv _far *mV, short menuId)
{
#ifndef RUN_ONLY
	LPEVENTINFOS2 eiPtr;
	int n;
	for(n=Conditions.size(),eiPtr=(LPEVENTINFOS2)conditionsInfos;n>0&&eiPtr->menu!=menuId;n--)
		eiPtr=EVINFO2_NEXT(eiPtr);
	if(n>0)
		return eiPtr->infos.code;
#endif // !RUN_ONLY
	return -1;
}

short WINAPI DLLExport GetActionCodeFromMenu(mv _far *mV, short menuId)
{
#ifndef RUN_ONLY
	LPEVENTINFOS2 eiPtr;
	int n;

	for (n=Actions.size(),eiPtr=(LPEVENTINFOS2)actionsInfos;n>0&&eiPtr->menu!=menuId;n--)
		eiPtr = EVINFO2_NEXT(eiPtr);
	if (n>0) 
		return eiPtr->infos.code;
#endif // !RUN_ONLY
	return -1;
}

short WINAPI DLLExport GetExpressionCodeFromMenu(mv _far *mV, short menuId)
{
#ifndef RUN_ONLY
	LPEVENTINFOS2 eiPtr;
	int	n;
	for (n=Expressions.size(),eiPtr=(LPEVENTINFOS2)expressionsInfos;n>0&&eiPtr->menu!=menuId;n--)
		eiPtr = EVINFO2_NEXT(eiPtr);
	if (n>0) 
		return eiPtr->infos.code;
#endif // !RUN_ONLY
	return -1;
}


// -------------------------------------------------------
// GetConditionInfos / GetActionInfos / GetExpressionInfos
// -------------------------------------------------------
// From a action / condition / expression code, returns 
// an infosEvents structure. 
//

LPINFOEVENTSV2 WINAPI DLLExport GetConditionInfos(mv _far *mV, short code)
{
#ifndef RUN_ONLY
	return &GetEventInformations((LPEVENTINFOS2)conditionsInfos, code)->infos;
#else
	return NULL;
#endif // !RUN_ONLY
}

LPINFOEVENTSV2 WINAPI DLLExport GetActionInfos(mv _far *mV, short code)
{
#ifndef RUN_ONLY
	return &GetEventInformations((LPEVENTINFOS2)actionsInfos, code)->infos;
#else
	return NULL;
#endif // !RUN_ONLY
}

LPINFOEVENTSV2 WINAPI DLLExport GetExpressionInfos(mv _far *mV, short code)
{
#ifndef RUN_ONLY
	return &GetEventInformations((LPEVENTINFOS2)expressionsInfos, code)->infos;
#else
	return NULL;
#endif // !RUN_ONLY
}


// ----------------------------------------------------------
// GetConditionString / GetActionString / GetExpressionString
// ----------------------------------------------------------
// From a action / condition / expression code, returns 
// the string to use for displaying it under the event editor
//

void WINAPI DLLExport GetConditionString(mv _far *mV, short code, LPSTR strPtr, short maxLen)
{
#ifndef RUN_ONLY
		if(code>=0&&code<(short)Conditions.size())
			strcpy(strPtr,Conditions[code]->getName());
#endif // !RUN_ONLY
}

void WINAPI DLLExport GetActionString(mv _far *mV, short code, LPSTR strPtr, short maxLen)
{
#ifndef RUN_ONLY
		if(code>=0&&code<(short)Actions.size())
			strcpy(strPtr,Actions[code]->getName());
#endif // !RUN_ONLY
}

void WINAPI DLLExport GetExpressionString(mv _far *mV, short code, LPSTR strPtr, short maxLen)
{
#ifndef RUN_ONLY
		if(code>=0&&code<(short)Expressions.size())
			strcpy(strPtr,Expressions[code]->getName());
#endif // !RUN_ONLY
}

// ----------------------------------------------------------
// GetExpressionParam
// ----------------------------------------------------------
// Returns the parameter name to display in the expression editor
//

void WINAPI DLLExport GetExpressionParam(mv _far *mV, short code, short param, LPSTR strBuf, short maxLen)
{
#ifndef RUN_ONLY
	if(strlen(Expressions[code]->getParamName(param)))
		strcpy(strBuf,Expressions[code]->getParamName(param));
	else
		*strBuf=0;
#endif
}

// ----------------------------------------------------------
// Custom Parameters
// ----------------------------------------------------------

// --------------------
// InitParameter
// --------------------
// Initialize the parameter.
//

void WINAPI InitParameter(mv _far *mV, short code, paramExt* pExt)
{
#if !RUN_ONLY
	// Example
	// -------
	// strcpy(&pExt->pextData[0], "Parameter Test");
	// pExt->pextSize = sizeof(paramExt) + strlen(pExt->pextData)+1;
#endif // !RUN_ONLY
}

// Example of custom parameter setup proc
// --------------------------------------
/*
#if !RUN_ONLY
BOOL CALLBACK DLLExport SetupProc(HWND hDlg, UINT msgType, WPARAM wParam, LPARAM lParam)
{
	paramExt*			pExt;

	switch (msgType)
	{
		case WM_INITDIALOG: // Init dialog

			// Save edptr
			SetWindowLong(hDlg, DWL_USER, lParam);
			pExt=(paramExt*)lParam;

			SetDlgItemText(hDlg, IDC_EDIT, pExt->pextData);
			return TRUE;

		case WM_COMMAND: // Command

			// Retrieve edptr
			pExt = (paramExt *)GetWindowLong(hDlg, DWL_USER);

			switch (wmCommandID)
			{
			case IDOK:	// Exit
				GetDlgItemText(hDlg, IDC_EDIT, pExt->pextData, 500);
				pExt->pextSize=sizeof(paramExt)+strlen(pExt->pextData)+1;
				EndDialog(hDlg, TRUE);
				return TRUE;

				default:
					break;
			}
			break;

		default:
			break;
	}
	return FALSE;
}
#endif // !RUN_ONLY
*/

// --------------------
// EditParameter
// --------------------
// Edit the parameter.
//

void WINAPI EditParameter(mv _far *mV, short code, paramExt* pExt)
{
#if !RUN_ONLY

	// Example
	// -------
	// DialogBoxParam(hInstLib, MAKEINTRESOURCE(DB_TRYPARAM), mV->mvHEditWin, SetupProc, (LPARAM)(LPBYTE)pExt);

#endif // !RUN_ONLY
}

// --------------------
// GetParameterString
// --------------------
// Initialize the parameter.
//

void WINAPI GetParameterString(mv _far *mV, short code, paramExt* pExt, LPSTR pDest, short size)
{
#if !RUN_ONLY

	// Example
	// -------
	// wsprintf(pDest, "Super parameter %s", pExt->pextData);

#endif // !RUN_ONLY
}

// -----------------
// GetObjInfos
// -----------------
// Return object info
//
// Info displayed in the object's About properties
// Note: ObjComment is also displayed in the Quick Description box in the Insert Object dialog box
//

void WINAPI	DLLExport GetObjInfos (mv _far *mV, LPEDATA edPtr, LPSTR ObjName, LPSTR ObjAuthor, LPSTR ObjCopyright, LPSTR ObjComment, LPSTR ObjHttp)
{
#ifndef RUN_ONLY
	strcpy(ObjName,ObjectName);
	strcpy(ObjAuthor,ObjectAuthor);
	strcpy(ObjCopyright,ObjectCopyright);
	strcpy(ObjComment,ObjectComment);
	strcpy(ObjHttp,ObjectURL);
#endif
}

// -----------------
// GetHelpFileName
// -----------------
// Returns the help filename of the object.
//

LPCSTR WINAPI GetHelpFileName()
{
#ifndef RUN_ONLY
	return ObjectHelp;
#else
	return NULL;
#endif
}

// -----------------
// GetRegID
// -----------------
// Returns the registered ID of the object
//

int WINAPI GetRegID()
{
#ifndef RUN_ONLY
	return ObjectRegID;
#else
	return REGID_RUNTIME;
#endif
}
