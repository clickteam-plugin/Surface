// --------------------
// Object information
// --------------------

// Basic object details
#define	ObjectName			"Surface"
#define	ObjectAuthor		"Looki"
#define	ObjectCopyright		"Copyright © 2013 Looki"
#define	ObjectComment		"An editable canvas much like Overlay Redux, but with support for multiple images, alpha channels and more. Thanks to Greyhill for the icon."
#define	ObjectURL			"http://www.aquadasoft.com"
#define	ObjectHelp			"Help\\Surface.chm"

// If you register your object with Clickteam, change this to the ID you were given
#define ObjectRegID			REGID_PRIVATE

// Change N,O,N,E to 4 unique characters (MMF currently still uses this to keep track)
#define	IDENTIFIER			MAKEID(S,U,R,F)

#define	ObjectVersion		1

// --------------------
// Version information
// --------------------

// PRODUCT_VERSION_TGF or PRODUCT_VERSION_DEVELOPER
#define ForVersion			PRODUCT_VERSION_STANDARD

// Set this to the latest MMF build out when you build the object
#define	MinimumBuild		251

// --------------------
// Beta information
// --------------------

// #define BETA
// #define POPUP_ON_DROP
// #define POPUP_ON_EXE
// #define POPUP_ON_BUILD
// #define POPUP_MESSAGE	"This is a beta extension- use with caution!"

// --------------------
// Handling priority
// --------------------
// If this extension will handle windows messages, specify the priority
// of the handling procedure, 0 = low and 255 = very high

#define	WINDOWPROC_PRIORITY 100
