/* EditData.hpp
 * This is where you control what data
 * you want to have at edittime. You
 * are responsible for serializing and
 * deserializing (saving/loading) the
 * data to/from the SerializedED structure
 * as if it were a file on the hard drive.
 * It doesn't need to be efficient; this
 * is just at edittime and once at the
 * start of the runtime.
 */

struct EditData
{
	short    width;
	short    height;
	short    width_def; //Width & height of the default image
	short    height_def;
	WORD     images[MAX_IMAGES];
	short    imageCount;

	bool     loadFirst;
	bool     useAbs;
	bool     threadedIO;
	bool     keepPoints;
	bool     multiImg;
	bool     dispTarget;
	bool     selectLast;

	LOGFONT  textFont;
	COLORREF textColor;
	DWORD    textFlags;

	/* <default constructor>
	 * This is where you provide default values for
	 * your editdata. This constructor is used
	 * when your extension is first created and
	 * default values are needed.
	 */
	EditData()
	: width(DEF_WIDTH)
	, height(DEF_HEIGHT)
	, loadFirst(true)
	, useAbs(false)
	, threadedIO(false)
	, keepPoints(false)
	, multiImg(true)
	, dispTarget(false)
	, selectLast(false)
	, textColor(BLACK)
	, textFlags(DT_NOCLIP|DT_SINGLELINE)
	, imageCount(0)
	{
		textFont.lfWidth = 0;
		textFont.lfHeight = 13;
		strcpy((char *)textFont.lfFaceName, "Arial");
		textFont.lfItalic = 0;
		textFont.lfWeight = 0;
		textFont.lfUnderline = 0;
		textFont.lfStrikeOut = 0;

		for(std::size_t i = 0; i < MAX_IMAGES; ++i)
		{
			images[i] = 0;
		}
	}

	//compiler-generated copy ctor & copy assignment op
	//EditData &oprtator=(EditData const &) = default;
	//EditData &operator=(EditData const &from) = default;

#ifndef RUN_ONLY
	/* Serialize
	 * This is where you need to "write" data
	 * to SerializedED like a file. Make sure
	 * you can read the data back in the
	 * constructor below!
	 */
	bool Serialize(mv *mV, SerializedED *&SED) const
	{
		//Create an instance of EDOStream, a helper class
		EDOStream os (mV, SED);

		os.write_value(width);
		os.write_value(height);
		os.write_value(width_def);
		os.write_value(height_def);
		os.write_sequence(images, images+MAX_IMAGES);
		os.write_value(imageCount);
		
		os.write_value(loadFirst);
		os.write_value(useAbs);
		os.write_value(threadedIO);
		os.write_value(keepPoints);
		os.write_value(multiImg);
		os.write_value(dispTarget);
		os.write_value(selectLast);
		
		os.write_value(textFont); //LOGFONT is POD
		os.write_value(textColor);
		os.write_value(textFlags);

		//That's it! EDOStream automatically stores the data in your extension's editdata
		return true; //return false in the event of an error
	}
#endif

	/* <constructor>
	 * This is the primary constructor for the
	 * EditData class. Here you will have to
	 * "read" SerializedED like a file and
	 * load back everything that gets saved
	 * above in Serialize. Make sure you check
	 * the version information first, this
	 * constructor is used to update from
	 * older versions of your editdata as well.
	 */
	EditData(SerializedED *SED)
	{
		if(SED->Header.extVersion == 1 || SED->Header.extVersion == 2) //current version
		{
			//Create an instance of EDIStream, a helper class
			EDIStream is (SED);

			width = is.read_value<short>();
			height = is.read_value<short>();
			width_def = is.read_value<short>();
			height_def = is.read_value<short>();
			for(std::size_t i = 0; i < MAX_IMAGES; ++i)
			{
				images[i] = is.read_value<WORD>();
			}
			imageCount = is.read_value<short>();

			loadFirst = is.read_value<bool>();
			useAbs = is.read_value<bool>();
			threadedIO = is.read_value<bool>();
			keepPoints = is.read_value<bool>();
			multiImg = is.read_value<bool>();
			dispTarget = is.read_value<bool>();
			selectLast = is.read_value<bool>();

			textFont = is.read_value<LOGFONT>(); //LOGFONT is POD
			textColor = is.read_value<COLORREF>();
			textFlags = is.read_value<DWORD>();
		}
		else //the version is newer than current
		{
			MessageBox(NULL, _T("The MFA you are trying to load was saved")
			                 _T("with a newer version of this extension."),
			                 _T("Error Loading Surface Object"), MB_OK);
		}
	}

	/* <destructor>
	 * If you grabbed any memory e.g. with new,
	 * make sure to e.g. delete it in here.
	 */
	~EditData()
	{
	}
};
