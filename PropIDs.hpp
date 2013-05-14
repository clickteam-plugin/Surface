#ifndef PropertyIdentifiers_HeaderPlusPlus
#define PropertyIdentifiers_HeaderPlusPlus

/* Prop
 * This is the namespace the property identifiers
 * are stored in. It keeps them out of the global
 * namespace and it helps with intellisense.
 */
namespace Prop
{
	/* <enum>
	 * The property identitifers.
	 */
	enum
	{
		zNOT_USED = PROPID_EXTITEM_CUSTOM_FIRST,
		Version,
		ImageBank,
			Images,
			Remove,
			MultiImage,
			LoadFirst,
			DisplayEditing,
			SelectNew,
		Settings,
			AbsCoords,
			KeepPoints,
			ThreadedIO,
			Resampling,
		//Font tab
			Multiline,
			NoClipping,
			Ellipsis,
			WordBreak,
	};
}

#endif
