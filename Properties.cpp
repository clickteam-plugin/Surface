/* Properties.cpp
 * In this file you will define the edittime
 * properties for your extension. By default
 * only a version number is displayed.
 * Functions defined here:
 * GetProperties
 * ReleaseProperties
 * GetPropCreateParam
 * ReleasePropCreateParam
 * GetPropValue
 * SetPropValue
 * GetPropCheck
 * SetPropCheck
 * EditProp
 * IsPropEnabled
 */

#include "Common.h"

#ifndef RUN_ONLY

#include "PropIDs.hpp"

PropData Properties[] = //See the MMF2SDK help file for information on PropData_ macros.
{
	PropData_StaticString(Prop::Version,        (UINT_PTR)"Version #",                 (UINT_PTR)"This is the current version of the Surface object."),

	PropData_Group    (Prop::ImageBank,      (UINT_PTR)"Image bank",                (UINT_PTR)""),
	PropData_ImageList(Prop::Images,         (UINT_PTR)"Images",                    (UINT_PTR)""),
	PropData_Button   (Prop::Remove,         (UINT_PTR)"",                          (UINT_PTR)"", (UINT_PTR)"Remove"),
	PropData_CheckBox (Prop::MultiImage,     (UINT_PTR)"Use multiple images",       (UINT_PTR)"Actions to manage multiple images will be provided. Recommended over using multiple Surface objects."),
	PropData_CheckBox (Prop::LoadFirst,      (UINT_PTR)"Load first image on start", (UINT_PTR)"Loads and displays image 0 if available on start."),
	PropData_CheckBox (Prop::SelectNew,      (UINT_PTR)"Select new images",         (UINT_PTR)"When an image is added, it is automatically selected as the editing image."),
	PropData_CheckBox (Prop::DisplayEditing, (UINT_PTR)"Display editing image",     (UINT_PTR)"\"Set current image\" will not be available and the editing image is drawn instead."),

	PropData_Group   (Prop::Settings,   (UINT_PTR)"Settings",                   (UINT_PTR)""),
	PropData_CheckBox(Prop::AbsCoords,  (UINT_PTR)"Use absolute coordinates",   (UINT_PTR)"Coordinates are not relative to the surface position."),
	PropData_CheckBox(Prop::KeepPoints, (UINT_PTR)"Keep points after drawing",  (UINT_PTR)"The polygon points will be kept after drawing one."),
	PropData_CheckBox(Prop::ThreadedIO, (UINT_PTR)"Threaded file input/output", (UINT_PTR)"If checked, the application does not freeze while a file is being loaded or saved."),
	PropData_CheckBox(Prop::Resampling, (UINT_PTR)"Linear resampling",          (UINT_PTR)"Some actions like \"Resize\" and \"Rotate\" can achieve better quality with this option."),

	PropData_End()
};

char const *const FontQuality[] =
{
	0,
	"Aliased",
	"Anti-aliased",
	"ClearType",
	0
};

PropData FontProps[] =
{
	PropData_CheckBox(Prop::Multiline,  (UINT_PTR)"Multi-line",       (UINT_PTR)"If checked, line breaks will be rendered, but vertical alignment is not supported."),
	PropData_CheckBox(Prop::NoClipping, (UINT_PTR)"No text clipping", (UINT_PTR)"If checked, the text won't be clipped if it exceeds the given rectangle."),
	PropData_CheckBox(Prop::Ellipsis,   (UINT_PTR)"Add ellipsis",     (UINT_PTR)"Truncates any word that does not fit in the rectangle and adds an ellipsis."),
	PropData_CheckBox(Prop::WordBreak,  (UINT_PTR)"Break words",      (UINT_PTR)"Break words that don't fit in the rectangle."),
	PropData_End()
};

#endif

/* GetProperties
 * Here, you choose what proeprties to insert
 * into which tabs. Lots of dynamic things can
 * be done in here, such as dynamically
 * generating the properties based on the
 * number of stored values in the editdata.
 */
BOOL MMF2Func GetProperties(mv *mV, SerializedED *SED, BOOL MasterItem)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	//ed.stuff;
	mvInsertProps(mV, SED, Properties, PROPID_TAB_GENERAL, TRUE);
	mvInsertProps(mV, SED, FontProps, PROPID_TAB_TEXTOPT, TRUE);
	//if you changed ed:
	//ed.Serialize(mV, SED);
	return TRUE;
#endif
	return FALSE;
}

/* ReleaseProperties
 * If you dynamically allocated memory for
 * property arrays in GetProperties, release
 * that memory in this function.
 */
void MMF2Func ReleaseProperties(mv *mV, SerializedED *SED, BOOL MasterItem)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	//ed.stuff;
	//
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
}

/* GetPropCreateParam
 * A convenience function if you use the
 * hard-coded Properties array above. Allows
 * you to dynamically create e.g. combo
 * lists and other things that have
 * additional parameters for their
 * properties. However if you dynamically
 * allocated everything in GetProperties,
 * this function is useless.
 */
LPARAM MMF2Func GetPropCreateParam(mv *mV, SerializedED *SED, UINT PropID)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	//ed.stuff;
	//return (LPARAM)TheParameter;
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
	return NULL;
}

/* ReleasePropCreateParam
 * If you found a useful way to use the
 * above function and you dynamically
 * allocated memory, release it here.
 */
void MMF2Func ReleasePropCreateParam(mv *mV, SerializedED *SED, UINT PropID, LPARAM lParam)
{
#ifndef RUN_ONLY
	//EditData ed (SED);
	//ed.stuff;
	//MyAwesomeMemoryFreeingFunction(lParam);
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
}

/* GetPropValue
 * This is where you return the display
 * values for the properties you defined.
 * Refer to the MMF2SDK Help file for
 * information on what to return for
 * each kind of property.
 */
void *MMF2Func GetPropValue(mv *mV, SerializedED *SED, UINT PropID)
{
#ifndef RUN_ONLY
	EditData ed (SED);
	switch(PropID)
	{
	case Prop::Version:
		{
			return new CPropDataValue("1.6 beta");
		}
	case Prop::Images:
		{
			CPropDataValue *pv = new CPropDataValue((ed.imageCount + 1) * sizeof(WORD), NULL);
			if(pv->m_pData)
			{
				LPWORD pw = LPWORD(pv->m_pData);
				*pw++ = ed.imageCount;
				for(WORD w = 0; w < ed.imageCount; ++w)
				{
					*pw++ = ed.images[w];
				}
				return pv;
			}
			pv->Delete();
		} break;
	}
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
	return NULL;
}

/* SetPropValue
 * The user has just finished entering their
 * 500 page essay into your MyString field.
 * MMF2 isn't so kind as to save that for them,
 * so it entrusts it with you. Store it!
 */
void MMF2Func SetPropValue(mv *mV, SerializedED *SED, UINT PropID, CPropValue *PropVal)
{
#ifndef RUN_ONLY
	EditData ed (SED);
	switch(PropID)
	{
	case Prop::Images:
		{
			CPropDataValue *val = (CPropDataValue *)PropVal;
			if(val->m_pData)
			{
				LPWORD pw = (LPWORD)(val->m_pData);
				for(WORD w = 0; w < ed.imageCount; ++w)
				{
					ed.images[w] = *pw++;
				}
			}
			mvInvalidateObject(mV, SED);
		} break;
	}
	//since you changed ed:
	ed.Serialize(mV, SED);

	//You may want to have your object redrawn in the
	//frame editor after the modifications; in this
	//case, just call this function:
	//mvInvalidateObject(mV, SED);
#endif
}

/* GetPropCheck
 * There are checkbox properties, and
 * there are properties that have their
 * own checkbox as an option. This is
 * where you tell MMF2 whether those
 * boxes are ticked or not.
 */
BOOL MMF2Func GetPropCheck(mv *mV, SerializedED *SED, UINT PropID)
{
#ifndef RUN_ONLY
	EditData ed (SED);
	switch(PropID)
	{
	case Prop::LoadFirst:
		return ed.loadFirst;
	case Prop::MultiImage:
		return ed.multiImg;
	case Prop::AbsCoords:
		return ed.useAbs;
	case Prop::KeepPoints:
		return ed.keepPoints;
	case Prop::ThreadedIO:
		return ed.threadedIO;
	case Prop::DisplayEditing:
		return ed.dispTarget;
	case Prop::SelectNew:
		return ed.selectLast;
	case Prop::Resampling:
		return mvGetPropCheck(mV, SED, PROPID_FITEM_ANTIA); //Anti-aliasing

	case Prop::Multiline:
		return !(ed.textFlags & DT_SINGLELINE);
	case Prop::NoClipping:
		return (ed.textFlags & DT_NOCLIP) == DT_NOCLIP;
	case Prop::WordBreak:
		return (ed.textFlags & DT_WORDBREAK) == DT_WORDBREAK;
	case Prop::Ellipsis:
		return (ed.textFlags & DT_WORD_ELLIPSIS) == DT_WORD_ELLIPSIS;
	}
	//if you changed ed:
	//ed.Serialize(mV, SED);
#endif
	return FALSE;
}

/* SetPropCheck
 * The user has painstakingly moved the
 * mouse cursor over the checkbox and
 * expended the immense effort required
 * to click the mouse and toggle the
 * state of the tickbox. Don't let their
 * effort be all for naught!
 */
void MMF2Func SetPropCheck(mv *mV, SerializedED *SED, UINT PropID, BOOL Ticked)
{
#ifndef RUN_ONLY
	EditData ed (SED);
	switch(PropID)
	{
	case Prop::LoadFirst:
		ed.loadFirst = Ticked;
		mvInvalidateObject(mV, SED);
		mvRefreshProp(mV, SED, Prop::LoadFirst, true);
		break;
	case Prop::MultiImage:
		ed.MultiImage = Ticked;
		mvInvalidateObject(mV, SED);
		mvRefreshProp(mV, SED, Prop::MultiImage, true);
		mvRefreshProp(mV, SED, Prop::LoadFirst, true);
		mvRefreshProp(mV, SED, Prop::SelectNew, true);
		mvRefreshProp(mV, SED, Prop::DisplayEditing, true);
		break;
	case Prop::AbsCoords:
		ed.AbsCoords = Ticked;
		mvInvalidateObject(mV, SED);
		mvRefreshProp(mV, SED, Prop::AbsCoords, true);
		break;
	case Prop::KeepPoints:
		ed.KeepPoints = Ticked;
		mvInvalidateObject(mV, SED);
		mvRefreshProp(mV, SED, Prop::KeepPoints, true);
		break;
	case Prop::ThreadedIO:
		ed.ThreadedIO = Ticked;
		mvInvalidateObject(mV, SED);
		mvRefreshProp(mV, SED, Prop::ThreadedIO, true);
		break;
	case Prop::DisplayEditing:
		ed.DisplayEditing = Ticked;
		mvInvalidateObject(mV, SED);
		mvRefreshProp(mV, SED, Prop::DisplayEditing, true);
		break;
	case Prop::SelectNew:
		ed.SelectNew = Ticked;
		mvInvalidateObject(mV, SED);
		mvRefreshProp(mV, SED, Prop::SelectNew, true);
		break;
	case Prop::Resampling:
		mvSetPropCheck(mV,SED,Prop::FITEM_ANTIA,Ticked);
		mvInvalidateObject(mV, SED);
		mvRefreshProp(mV, SED, Prop::Resampling, true);
		mvRefreshProp(mV, SED, Prop::FITEM_ANTIA, true);
		break;

	case Prop::Multiline:
		if(Ticked)
			ed.textFlags &= ~DT_SINGLELINE;
		else
			ed.textFlags |= DT_SINGLELINE;
		mvInvalidateObject(mV, SED);
		mvRefreshProp(mV, SED, Prop::Multiline, true);
		break;
	case Prop::NoClipping:
		if(Ticked)
			ed.textFlags |= DT_NOCLIP;
		else
			ed.textFlags &= ~DT_NOCLIP;
		mvInvalidateObject(mV, SED);
		mvRefreshProp(mV, SED, Prop::NoClipping, true);
		break;
	case Prop::WordBreak:
		if(Ticked)
			ed.textFlags |= DT_WORDBREAK;
		else
			ed.textFlags &= ~DT_WORDBREAK;
		mvInvalidateObject(mV, SED);
		mvRefreshProp(mV, SED, Prop::WordBreak, true);
		break;
	case Prop::Ellipsis:
		if(Ticked)
			ed.textFlags |= DT_WORD_ELLIPSIS;
		else
			ed.textFlags &= ~DT_WORD_ELLIPSIS;
		mvInvalidateObject(mV, SED);
		mvRefreshProp(mV, SED, Prop::Ellipsis, true);
		break;
	}
	//since you changed ed:
	ed.Serialize(mV, SED);
#endif
}

/* EditProp
 * If you use a button proeprty or a
 * property that has a button, then
 * you would be impolite to ignore
 * the user when they click on that
 * button. Here is where you respond
 * to that button press e.g. with
 * a dialog.
 */
BOOL MMF2Func EditProp(mv *mV, SerializedED *SED, UINT PropID)
{
#ifndef RUN_ONLY
	EditData ed (SED);
	switch(PropID)
	{
	case Prop::Remove:
		{
			if(MessagBox(NULL, _T("Do you really want to delete all images?"), _T("Warning"), MB_YESNO|MB_ICONWARNING) == IDYES)
			{
				mvInvalidateObject(mV, SED);
				for(std::size_t i = 0; i < MAX_IMAGES; ++i)
				{
					ed.images[i] = 0;
				}
				ed.imageCount = 0;
				//ed.width = 320;
				//ed.height = 240;
				mvRefreshProp(mV, SED, Prop::LoadFirst, true);
				mvRefreshProp(mV, SED, Prop::Images, true);
				return true;
			}
		} break;
	case Prop::Images:
		{
			mvInvalidateObject(mV, SED);
			EditAnimationParams eip;
			eip.m_dwSize = sizeof(EditAnimationParams);
			eip.m_pWindowTitle = 0;
			eip.m_nImages = ed.imageCount;
			eip.m_nMaxImages = MAX_IMAGES;
			eip.m_nStartIndex = 0;
			eip.m_pImages = &(ed.images[0]);
			eip.m_pImageTitles = 0;
			eip.m_dwOptions = PICTEDOPT_CANBETRANSPARENT|PICTEDOPT_HOTSPOT;
			eip.m_dwFixedWidth = ed.width ? ed.width : DEF_WIDTH;
			eip.m_dwFixedHeight = ed.height ? ed.height : DEF_HEIGHT;
			BOOL output = mV->mvEditAnimation(edPtr, &eip, mV->mvHEditWin);
			//Update object count
			ed.imageCount = eip.m_nImages;
			//Update object dimensions
			if(output && edPtr->images[0])
			{
				cSurface is;
				LockImageSurface(mV->mvIdAppli, ed.images[0], is);
				ed.width = ed.width_def = is.GetWidth();
				ed.height = ed.height_def = is.GetHeight();
				UnlockImageSurface(is);
			}
			mvRefreshProp(mV, SED, Prop::LoadFirst, true);
			mvRefreshProp(mV, SED, Prop::Images, true);
			return output;
		}
	}
	//if you changed ed:
	ed.Serialize(mV, SED);
#endif
	return FALSE;
}

/* IsPropEnabled
 * Let's say you're tired of having to deal
 * with the user's various 500 page essays,
 * tickbox toggles, and button clicks. Here,
 * you can simply disable a property by
 * returning FALSE for that property.
 */
BOOL MMF2Func IsPropEnabled(mv *mV, SerializedED *SED, UINT PropID)
{
#ifndef RUN_ONLY
	EditData ed (SED);
	switch(PropID)
	{
	case Prop::Version:
			return FALSE;
	case Prop::LoadFirst:
		return ed.multiImg || !ed.imageCount;
	case Prop::SelectNew:
		return ed.multiImg;
	case Prop::DisplayEditing:
		return ed.multiImg;
	}
	//if you changed ed:
	//ed.Serialize(mV, SED);

	return TRUE;
#endif
	return FALSE;
}
