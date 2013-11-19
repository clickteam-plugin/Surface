#define ITEM_MULTIPLEIMG	ITEM(-1,"(Enable multiple images)")

//:-------------------------------
// Condition menu
//:-------------------------------

#ifdef CONDITION_MENU

	SEPARATOR
	SUB_START("Patterns")
		ITEM(14,"Pattern exists?")
		//SEPARATOR
		//SUB_START("User callback")
		//	ITEM(8,"On callback")
		//SUB_END
	SUB_END
	SUB_START("Image")
		if(!edPtr||edPtr->multiImg)
		{
			//if(!edPtr||!edPtr->dispTarget)
				ITEM(9,"Compare to displayed image")
			ITEM(10,"Compare to selected image")
			ITEM(7,"Select image (inline)")
			SEPARATOR
		}
		ITEM(0,"Has alpha channel?")
		ITEM(6,"Pixel is equal to color?")
		ITEM(16,"Point is inside image?")
		//ITEM(11,"Compare to red value")
		//ITEM(12,"Compare to green value")
		//ITEM(13,"Compare to blue value")
	SUB_END
	SUB_START("Input / Output")
		ITEM(1,"Image in clipboard?")
		SEPARATOR
		ITEM(2,"Loading failed?")
		ITEM(3,"Saving failed?")
		ITEM(4,"Loading succeeded?")
		ITEM(5,"Saving succeeded?")
		SEPARATOR
		ITEM(17,"File I/O in progress?")
		SEPARATOR
		ITEM(15,"Buffer is locked?")
	SUB_END
	SEPARATOR
	ITEM(8,"On callback")
#endif

//:-------------------------------
// Action menu
//:-------------------------------

#ifdef ACTION_MENU

	SEPARATOR
	SUB_START("Display settings")
		if(!edPtr||!edPtr->dispTarget)
			ITEM(1,"Set image to display")
		SEPARATOR
		ITEM(27,"Force redraw")
		ITEM(104,"Skip redraw")
		SEPARATOR
		ITEM(111,"Set transparent")
		SEPARATOR
		ITEM(139,"Set scale")
		ITEM(140,"Set X scale")
		ITEM(141,"Set Y scale")
		ITEM(152,"Set angle")
	SUB_END
	SUB_START("General settings")
		if(!edPtr||edPtr->multiImg)
		{
			ITEM(110,"Set select new images")
			ITEM(109,"Set display selected image")
			SEPARATOR
		}
		ITEM(42,"Set use absolute coords")
		ITEM(134,"Set keep points after drawing")
		ITEM(135,"Set background file input/output")
		ITEM(81,"Set linear resampling")
	SUB_END
	SEPARATOR
	SUB_START("Images")
	if(!edPtr||edPtr->multiImg)
	{
		ITEM(29,"Select image")
		SEPARATOR
		ITEM(17,"Add image")
		ITEM(12,"Insert image")
		SEPARATOR
		SUB_START("Copy image")
			ITEM(28,"From another image")
			ITEM(121,"From Surface object")
			ITEM(131,"From image reference")
		SUB_END
		SEPARATOR
		ITEM(91,"Swap two images")
		SEPARATOR
		ITEM(11,"Delete image")
		ITEM(18,"Delete all images")
		SEPARATOR
		SUB_START("References")
			ITEM(129,"Add reference")
			ITEM(130,"Insert reference")
			SEPARATOR
			ITEM(132,"Set reference value")
			ITEM(133,"Set reference state")
		SUB_END
	}
	else
	{
		ITEM_MULTIPLEIMG
	}
	SUB_END
	SUB_START("Patterns")
		SUB_START("Color")
			ITEM(43,"Create")
			ITEM(72,"Set color")
		SUB_END
		SUB_START("Linear gradient")
			ITEM(46,"Create")
			ITEM(73,"Set colors")
			ITEM(74,"Set vertical")
		SUB_END
		SUB_START("Radial gradient")
			ITEM(107,"Create")
			ITEM(73,"Set colors")
		SUB_END
		SUB_START("Callback")
			ITEM(145,"Create")
		SUB_END
		if(!edPtr||edPtr->multiImg)
		{
			SUB_START("Tiled image")
				ITEM(45,"Create")
				ITEM(76,"Set tiled image")
				ITEM(75,"Set origin")
			SUB_END
		}
		else
		{
			SUB_START("Tiled image")
				ITEM_MULTIPLEIMG
			SUB_END
		}
		SEPARATOR
		ITEM(77,"Delete")
	SUB_END
	SUB_START("Polygons")
		ITEM(142,"Add point")
		ITEM(33,"Add points from string")
		SEPARATOR
		ITEM(31,"Insert point")
		SEPARATOR
		ITEM(99,"Create star")
		ITEM(103,"Create regular polygon")
		SEPARATOR
		ITEM(34,"Move points")
		ITEM(35,"Rotate points")
		ITEM(100,"Scale points")
		SEPARATOR
		ITEM(36,"Remove point")
		ITEM(32,"Remove all points")
	SUB_END
	SUB_START("Text")
		ITEM(56,"Set font face")
		ITEM(57,"Set font size")
		ITEM(59,"Set font weight")
		ITEM(60,"Set font decoration")
		ITEM(58,"Set font quality")
		SEPARATOR
		ITEM(53,"Set horizontal align")
		ITEM(54,"Set vertical align")
		ITEM(118,"Set angle")
		SEPARATOR
		ITEM(85,"Set add ellipsis")
		ITEM(84,"Set clipping")
		ITEM(55,"Set multi-line")
		ITEM(86,"Set word break")
	SUB_END
	SEPARATOR
	ITEM(0,"~- Selected image -")
	SUB_START("Settings")
		ITEM(24,"Set transparent color")
		SEPARATOR
		ITEM(143,"Set hot spot")
		ITEM(144,"Set hot spot (percent)")
		SEPARATOR
		ITEM(96,"Set clipping rectangle")
		ITEM(97,"Clear clipping rectangle")
#ifdef HWABETA
		SEPARATOR
		ITEM(156, "Convert to bitmap")
		ITEM(154, "Convert to HWA texture")
		ITEM(155, "Convert to HWA target")
#endif
	SUB_END
	SUB_START("Input / Output")
		ITEM(153,"Load from file")
		ITEM(14,"Save to file")
		SEPARATOR
		ITEM(47,"Load from clipboard")
		ITEM(48,"Save to clipboard")
		SEPARATOR
		ITEM(163, "Quick restore")
		ITEM(162, "Quick store")
		SEPARATOR
		SUB_START("Advanced")
			ITEM(68,"Export as Overlay")
			SEPARATOR
			ITEM(112,"Lock buffer")
			ITEM(113,"Unlock buffer")
			ITEM(128,"Write bytes")
		SUB_END
	SUB_END
	ITEM(0,"~- Pixel processing -")
	SUB_START("Adjustments")
		ITEM(164, "Apply brightness")
		ITEM(165, "Apply contrast")
		ITEM(98,"Invert colors")
		//ITEM(166, "Normalize")
		SUB_START("Perform operation")
			ITEM(26,"With color for RGB")
			ITEM(123,"With value for...")
			//SEPARATOR
			//ITEM(154,"With condition and value for...")
		SUB_END	
		SEPARATOR
		ITEM(106,"Convert to grayscale")
		ITEM(92,"Move color channels")
		ITEM(161,"Apply color matrix")
		SEPARATOR
		ITEM(61,"Apply convolution matrix")
		//SEPARATOR
		//ITEM(155,"Evaluate")
	SUB_END
	SUB_START("Alpha channel")
		ITEM(4,"Create")
		ITEM(127,"Remove")
		SEPARATOR
		ITEM(6,"Clear")
		ITEM(5,"Set pixel")
		ITEM(25,"Draw line")
		ITEM(7,"Draw rectangle")
	SUB_END
	SUB_START("Blitting")
		SUB_START("From...")
			if(!edPtr||edPtr->multiImg)
			{
				SUB_START("Onto the alpha channel")
					ITEM(125,"Alpha channel")
				SUB_END
				SEPARATOR
				ITEM(63,"Image")
				ITEM(67,"Alpha channel")
				SEPARATOR
			}
			SUB_START("External")
				SUB_START("Active object")
					ITEM(49,"At destination")
					ITEM(122,"At its position")
				SUB_END
				ITEM(62,"Background")
				ITEM(70,"Overlay")
				ITEM(90,"Surface")
			SUB_END
			SUB_START("Advanced")
				ITEM(82,"Image reference")
				ITEM(87,"Window handle")
			SUB_END
		SUB_END
		SUB_START("Onto...")
			if(!edPtr||edPtr->multiImg)
			{
				SUB_START("From the alpha channel")
					ITEM(124,"Alpha channel")
				SUB_END
				SEPARATOR
				ITEM(0,"Image")
				ITEM(19,"Alpha channel")
				SEPARATOR
			}
			SUB_START("External")
				ITEM(65,"Background")
				ITEM(71,"Overlay")
				ITEM(89,"Surface")
				SEPARATOR
				ITEM(64,"Add backdrop")
			SUB_END
			SUB_START("Advanced")
				ITEM(83,"Image reference")
				ITEM(88,"Window handle")
			SUB_END
		SUB_END
		SEPARATOR
		ITEM(0,"~- Position && Size -")
		SUB_START("Destination")
			ITEM(41,"Set position")
			ITEM(66,"Set dimensions")
			ITEM(116,"Set stretch mode")
			SEPARATOR
			ITEM(105,"Set all")
		SUB_END
		SUB_START("Source")
			ITEM(114,"Set position")
			ITEM(115,"Set dimensions")
			ITEM(117,"Set region flag")
			SEPARATOR
			ITEM(146,"Set all")
		SUB_END
		SUB_START("Hot spot")
			ITEM(148,"Set pixels")
			ITEM(150,"Set percent")
			//ITEM(149,"Set flag")
		SUB_END
		SEPARATOR
		SUB_START("Angle")
			ITEM(147,"Set angle")
			ITEM(151,"Set rotation quality")
		SUB_END
		SEPARATOR
		SUB_START("Effect")
			ITEM(40,"Set by index")
			ITEM(126,"Set by name")
			SUB_START("Semi-transparency...")
				ITEM(39,"Set semi-transparency")
			SUB_END
			SUB_START("Tint...")
				ITEM(157, "Set tint (HWA only)")
			SUB_END
			SEPARATOR
			ITEM(136,"Set callback (overrides effect)")
		SUB_END
		SUB_START("Transparency")
			ITEM(38,"Set alpha mode")
			ITEM(37,"Set transparency")
		SUB_END
	SUB_END
	SUB_START("Callbacks")
		ITEM(95,"Loop through image")
		ITEM(137,"Loop through area")
		//SEPARATOR
		//ITEM(153,"Loop through image with condition")
		SEPARATOR
		ITEM(0,"~On callback:")
		ITEM(138,"Return alpha")
		ITEM(94,"Return color")
	SUB_END
	SUB_START("Transformations")
		ITEM(13,"Resize")
		ITEM(80,"Rotate")
		SEPARATOR
		ITEM(78,"Resize canvas")
		ITEM(23,"Minimize canvas")
		SEPARATOR
		ITEM(21,"Reverse X")
		ITEM(22,"Reverse Y")
		ITEM(93,"Scroll")
	SUB_END
	ITEM(0,"~- Drawing -")
	SUB_START("With color")
		ITEM(3,"Clear")
		ITEM(2,"Set pixel")
		ITEM(16,"Flood fill")
		ITEM(20,"Replace color")
		SEPARATOR
		ITEM(10,"Draw line")
		SUB_START("Draw rectangle")
			ITEM(0,"~Simple:")
			ITEM(9,"Via bounding box")
			ITEM(119,"Via position and size")
			SEPARATOR
			ITEM(0,"~With outline:")
			ITEM(159,"Via bounding box")
			ITEM(166,"Via position and size")
		SUB_END
		SUB_START("Draw ellipse")
			ITEM(0,"~Simple:")
			ITEM(8,"Via bounding box")
			ITEM(101,"Via center and size")
			SEPARATOR
			ITEM(0,"~With outline:")
			ITEM(158,"Via bounding box")
			ITEM(167,"Via center and size")
		SUB_END
		SUB_START("Draw polygon")
			ITEM(160,"Simple")
			ITEM(30,"With outline")
		SUB_END
		ITEM(52,"Draw text")
	SUB_END
	SUB_START("With pattern")
			ITEM(79,"Clear")
			SEPARATOR
			ITEM(69,"Draw line")
			SUB_START("Draw rectangle")
				ITEM(44,"Via bounding box")
				ITEM(120,"Via position and size")
			SUB_END
			SUB_START("Draw ellipse")
				ITEM(50,"Via bounding box")
				ITEM(102,"Via center and size")
			SUB_END
			ITEM(51,"Draw polygon")
	SUB_END
	SEPARATOR
#endif


//:-------------------------------
// Expression menu
//:-------------------------------

#ifdef EXPRESSION_MENU
	
	SEPARATOR
	SUB_START("Scale / Angle")
		ITEM(69,"X scale")
		ITEM(70,"Y scale")
		ITEM(71,"Angle")
	SUB_END
	SEPARATOR
	SUB_START("Pattern")
		ITEM(41,"Pattern count")
		ITEM(40,"Pattern from index")
		SEPARATOR
		ITEM(42,"Color")
		SUB_START("Linear gradient")
			ITEM(43,"Color A")
			ITEM(44,"Color B")
		SUB_END
		SUB_START("Radial gradient")
			ITEM(43,"Color A")
			ITEM(44,"Color B")
		SUB_END
		ITEM(45,"Tiled image")
	SUB_END
	if(!edPtr||edPtr->multiImg)
	{
		SEPARATOR
		ITEM(0,"Image count")
		ITEM(6,"Last added image")
		SEPARATOR
	}
	if(!edPtr||(edPtr->multiImg/*&&!edPtr->dispTarget*/))
	{
		SUB_START("Current image")
			ITEM(2,"Index")
			SEPARATOR
			ITEM(8,"Width")
			ITEM(9,"Height")
		SUB_END
		SUB_START("Selected image")
			ITEM(1,"Index")
			SEPARATOR
	}
	else
	{
		SUB_START("Image")
	}
		ITEM(4,"Width")
		ITEM(5,"Height")
		SEPARATOR
		ITEM(3,"RGB at")
		ITEM(10,"Red at")
		ITEM(11,"Green at")
		ITEM(12,"Blue at")
		ITEM(13,"Alpha at")
		SEPARATOR
		ITEM(27,"Transparent color")
		ITEM(62,"Hot spot X")
		ITEM(63,"Hot spot Y")
		SEPARATOR
		ITEM(52,"Reference value")
	SUB_END
	if(!edPtr||edPtr->multiImg)
	{
		SUB_START("Any image")
			ITEM(15,"Width")
			ITEM(16,"Height")
			SEPARATOR
			ITEM(14,"RGB at")
			ITEM(46,"Red at")
			ITEM(47,"Green at")
			ITEM(48,"Blue at")
			ITEM(49,"Alpha at")
			SEPARATOR
			ITEM(64,"Transparent color")
			ITEM(60,"Hot spot X")
			ITEM(61,"Hot spot Y")
			SEPARATOR
			ITEM(53,"Reference value")
		SUB_END
	}
	SEPARATOR
	SUB_START("Callback")
		ITEM(58,"Source alpha")
		ITEM(65,"Destination alpha")
		SEPARATOR
		ITEM(54,"Source color")
		ITEM(55,"Destination color")
		SEPARATOR
		ITEM(17,"X position")
		ITEM(18,"Y position")
	SUB_END
	SUB_START("Flood fill")
			ITEM(36,"Area X1")
			ITEM(37,"Area Y1")
			ITEM(38,"Area X2")
			ITEM(39,"Area Y2")
	SUB_END
	SUB_START("Input / Output")
		SUB_START("Files")
			ITEM(28,"Filter count")
			SEPARATOR
			ITEM(29,"Filter name")
			ITEM(32,"Filter can save")
			SEPARATOR
			ITEM(31,"Filter extension count")
			ITEM(35,"Filter extension list")
			SEPARATOR
			ITEM(30,"Filter extension name")
		SUB_END
		SEPARATOR
		ITEM(7,"Overlay address")
		SEPARATOR
		ITEM(33,"Buffer address")
		ITEM(34,"Buffer pitch")
	SUB_END
	SEPARATOR
	SUB_START("Tools")
		ITEM(50,"Color from HEX")
		ITEM(23,"Color from RGB")
		SEPARATOR
		ITEM(51,"Random color")
		SEPARATOR
		ITEM(25,"Invert color")
		SEPARATOR
		ITEM(66,"Add two colors")
		ITEM(67,"Subtract two colors")
		ITEM(26,"Multiply two colors")
		ITEM(24,"Blend two colors")
		SUB_START("Compose two colors")
			ITEM(75, "Composed color")
			ITEM(76, "Composed alpha")
		SUB_END
	SUB_END
	SUB_START("Image references")
		ITEM(68,"Transformed Surface")
		SEPARATOR
		ITEM(57,"Background")
		SEPARATOR
		ITEM(56,"Object")
	SUB_END
	SUB_START("Window handles")
		ITEM(59,"Frame window")
	SUB_END
	SEPARATOR
#endif