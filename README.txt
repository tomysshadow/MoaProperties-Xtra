MoaProperties Xtra 1.0.4
By Anthony Kleine

	This Director Xtra exposes MOA Properties to Lingo.
	There are a couple advantages to using MOA Properties:

	-dynamic property names: access properties by passing in symbols
	-unique properties: there are MOA Properties that can't be accessed with Lingo alone

	Note that not every property that can be accessed with Lingo is possible
	to access with MOA. To read about all the MOA Properties, see the topic
	titled Director Types and Miscellaneous API in the Director XDK.

	Some of the handlers take in an optional argument called options. It is
	a property list which can specify any combination of the following properties:

	-#movie: the index of a Movie, as an integer, from which to access the property.
	For example, you could access properties of a Movie In A Window. The default is
	the Active Movie.

	-#filmLoop: for Frame, Sound and Sprite MOA Properties, it is the Film Loop Member from
	which to access the property. The default is to use the Score. For other MOA Properties,
	this option is ignored.

	-#frame: for Frame, Sound and Sprite MOA Properties, it is the index of a Frame, as
	an integer, from which to access the property. The default is the Current Frame for
	the Score, or the First Frame for Film Loops. For other MOA Properties, this
	option is ignored.

	If a property in the options property list is void, or left unspecified, the default
	for that option is used.

	Works with all Director versions from 5 - 12.


Usage
	on exitFrame me
		-- get the authorMode of the Player
		set playerAuthorMode to getPlayerMoaProperty(#authorMode)

		-- set the frame of the Active Movie to 10 (setting this does a "go to frame")
		setMovieMoaProperty(#frame, 10)

		-- get the createName of the Active Movie
		set movieCreateName to getMovieMoaProperty(#createName)

		-- get the name of Movie 2
		set movieName to getMovieMoaProperty(#name, [#movie: 2])

		-- set the preloadMode of Cast Library 3 to 2 (Before Frame One)
		setCastLibMoaProperty(castLib(3), #preloadMode, 2)

		-- set the purgePriority of Member 10 to 0 (Never)
		setMemberMoaProperty(member(10), #purgePriority, 0)

		-- set the loaded of Member 20 of Cast Library 4 of Movie 5 to 0 (unload the cast member)
		setMemberMoaProperty(member 20 of castLib 4, #loaded, 0, [#movie: 5])

		-- get the tempo of the Current Frame
		set frameTempo to getFrameMoaProperty(#tempo)

		-- get the script of Frame 20
		set frameScript to getFrameMoaProperty(#script, [#frame: 20])

		-- set the palette of Frame 30 of Movie 6 to -8 (Web 216 Palette)
		setFrameMoaProperty(#palette, -1, [#frame: 30, #movie: 6])

		-- get the member of the Sound in Channel 2 on Frame 40
		set soundMember to getSoundMoaProperty(2, #member, [#frame: 40])

		-- set the backColor of the Sprite in Channel 10 on Frame 50 of Film Loop "Hello World" to 0 (Black in the System - Win Palette)
		setSpriteMoaProperty(10, #backColor, 0, [#frame: 50, #filmLoop: member("Hello World")])
	end


Message Table
	new object me
	-- Get MOA Properties
	* getPlayerMoaProperty symbol name -- gets a MOA Property of the Player
	* getMovieMoaProperty symbol name, * options -- gets a MOA Property of a Movie
	* getCastLibMoaProperty object castLib, symbol name, * options -- gets a MOA Property of a Cast Library
	* getMemberMoaProperty object member, symbol name, * options -- gets a MOA Property of a Member
	* getFrameMoaProperty symbol name, * options -- gets a MOA Property of a Frame
	* getSoundMoaProperty integer channel, symbol name, * options -- gets a MOA Property of a Sound
	* getSpriteMoaProperty integer channel, symbol name, * options -- gets a MOA Property of a Sprite
	-- Set MOA Properties
	* setPlayerMoaProperty symbol name, any value -- sets a MOA Property of the Player
	* setMovieMoaProperty symbol name, any value, * options -- sets a MOA Property of a Movie
	* setCastLibMoaProperty object castLib, symbol name, any value, * options -- sets a MOA Property of a Cast Library
	* setMemberMoaProperty object member, symbol name, any value, * options -- sets a MOA Property of a Member
	* setFrameMoaProperty symbol name, any value, * options -- sets a MOA Property of a Frame
	* setSoundMoaProperty integer channel, symbol name, any value, * options -- sets a MOA Property of a Sound
	* setSpriteMoaProperty integer channel, symbol name, any value, * options -- sets a MOA Property of a Sprite