OLD_COMMIT := $(shell cat src/commit.h)
COMMIT := $(shell git rev-parse --short HEAD)
COMMIT_FILE := $(shell echo '\#define GIT_COMMIT  "$(COMMIT)"')
##TEST := $(shell echo '$(COMMIT_FILE) | $(OLD_COMMIT)' > test.h)
ifneq ($(COMMIT_FILE), $(OLD_COMMIT))
	UPDATE_FILE := $(shell echo '$(COMMIT_FILE)' > src/commit.h)
endif
## Haiku Generic Makefile v2.6 ##

## Fill in this file to specify the project being created, and the referenced
## Makefile-Engine will do all of the hard work for you. This handles any
## architecture of Haiku.

# The name of the binary.
NAME = XRS
TARGET_DIR = .

# The type of binary, must be one of:
#	APP:	Application
#	SHARED:	Shared library or add-on
#	STATIC:	Static library archive
#	DRIVER: Kernel driver
TYPE = APP

# 	If you plan to use localization, specify the application's MIME signature.
APP_MIME_SIG = application/x-vnd.xeD.XRS

#	The following lines tell Pe and Eddie where the SRCS, RDEFS, and RSRCS are
#	so that Pe and Eddie can fill them in for you.
#%{
# @src->@ 

#	Specify the source files to use. Full paths or paths relative to the 
#	Makefile can be included. All files, regardless of directory, will have
#	their object files created in the common object directory. Note that this
#	means this Makefile will not work correctly if two source files with the
#	same name (source.c or source.cpp) are included from different directories.
#	Also note that spaces in folder names do not work well with this Makefile.
SRCS =  ./src/Configurator.cpp \
./src/DrawCentredText.cpp \
./src/Engine/Engine.cpp \
./src/Engine/JuiceEngine.cpp \
./src/Extensions/JMDrum/src/Drum.cpp \
./src/Extensions/JMDrum/src/JMDrumPanel.cpp \
./src/Extensions/JMDrum/src/JMDrumTrack.cpp \
./src/Extensions/JMDrum/src/JMDrumTrackBoost.cpp \
./src/Extensions/JMDrum/src/JmDrums.cpp \
./src/Extensions/JMDrum/src/oscillator.cpp \
./src/Extensions/Sampler/ExternalManager.cpp \
./src/Extensions/Sampler/Sample.cpp \
./src/Extensions/Sampler/SampleView.cpp \
./src/Extensions/Sampler/SamplerPanel.cpp \
./src/Extensions/Sampler/SamplerTrack.cpp \
./src/Extensions/Sampler/SamplerTrackBoost.cpp \
./src/Extensions/Tn306/TNPanel.cpp \
./src/Extensions/Tn306/TNTrack.cpp \
./src/Extensions/Tn306/TNTrackBoost.cpp \
./src/Extensions/Tn306/deverb.cpp \
./src/Extensions/Tn306/knob303.cpp \
./src/Extensions/Tn306/synel.cpp \
./src/Extensions/Tn306/vca303.cpp \
./src/Extensions/Tn306/vcf303.cpp \
./src/Extensions/Tn306/vco303.cpp \
./src/Extensions/VIW/VIWPanel.cpp \
./src/Extensions/VIW/VIWTrack.cpp \
./src/Extensions/VIW/VIWTrackBoost.cpp \
./src/Extensions/VIW/VSTInstrument.cpp \
./src/Extensions/XFluidSynth/SFSPanel.cpp \
./src/Extensions/XFluidSynth/SFSTrack.cpp \
./src/Extensions/XFluidSynth/SFSTrackBoost.cpp \
./src/Extensions/XFluidSynth/SFWindow.cpp \
./src/Extensions/XFluidSynth/XFSynth.cpp \
./src/GUI/ADigit.cpp \
./src/GUI/APot.cpp \
./src/GUI/AboutBox.cpp \
./src/GUI/BzWindow.cpp \
./src/GUI/Cursors.cpp \
./src/GUI/FixedText.cpp \
./src/GUI/GfxMeter.cpp \
./src/GUI/MainWindow.cpp \
./src/GUI/MediaBrowser/IconListItem.cpp \
./src/GUI/MediaBrowser/MBWindow.cpp \
./src/GUI/MediaBrowser/VerticalTitle.cpp \
./src/GUI/MediaBrowser/XListItem.cpp \
./src/GUI/MediaBrowser/XListView.cpp \
./src/GUI/MixerUI/MixerLine.cpp \
./src/GUI/MixerUI/MixerWindow.cpp \
./src/GUI/Panels.cpp \
./src/GUI/ParameterValueKnob.cpp \
./src/GUI/PeakView.cpp \
./src/GUI/PlugWindow.cpp \
./src/GUI/PositionView.cpp \
./src/GUI/PotViewer.cpp \
./src/GUI/StereoVolumesView.cpp \
./src/GUI/TextControlFloater.cpp \
./src/GUI/TickView.cpp \
./src/GUI/TrackInfoWindow.cpp \
./src/GUI/TracksPanel.cpp \
./src/GUI/VSTKnob.cpp \
./src/GUI/VSTParamsView.cpp \
./src/GUI/ValuablePeakView.cpp \
./src/GUI/VertPeakView.cpp \
./src/GUI/XDigit.cpp \
./src/GUI/XPanel.cpp \
./src/GUI/XPot.cpp \
./src/GUI/XrsWindow.cpp \
./src/GUI/new_style2/PianoControl.cpp \
./src/GUI/new_style2/PianoEdit.cpp \
./src/GUI/new_style2/TrackBlock.cpp \
./src/GUI/new_style2/TrackEnd.cpp \
./src/GUI/new_style2/TrackList.cpp \
./src/GUI/new_style2/VolView.cpp \
./src/GUI/playlist/MatrixWindow.cpp \
./src/GUI/playlist/XMName.cpp \
./src/GUI/playlist/XMPoz.cpp \
./src/GUI/playlist/XMatrix.cpp \
./src/GUI/track/JTrack.cpp \
./src/GUI/track/XNotesView.cpp \
./src/GUI/track/XTrack.cpp \
./src/MainManager/BasicModelManager.cpp \
./src/MainManager/JFileManager.cpp \
./src/MainManager/LoadingError.cpp \
./src/MainManager/Main.cpp \
./src/MainManager/MidiTools.cpp \
./src/MainManager/TheApp.cpp \
./src/MainManager/TrackBoost.cpp \
./src/MainManager/TrackManager.cpp \
./src/MainManager/VstManager.cpp \
./src/MainManager/XHost.cpp \
./src/MeasureManager.cpp \
./src/Mixer/PBus.cpp \
./src/Mixer/PEffector.cpp \
./src/Mixer/PMixable.cpp \
./src/Mixer/PMixer.cpp \
./src/SJFilter.cpp \
./src/Valuable/Valuable.cpp \
./src/Valuable/ValuableManager.cpp \
./src/Valuable/ValuableMonitorWindow.cpp \
./src/WindowManager.cpp \
./src/media/VSTItem.cpp \
./src/struct/Note.cpp \
./src/struct/Pattern.cpp \
./src/struct/Sequence.cpp \
./src/struct/Song.cpp \
./src/struct/Track.cpp \
./src/utils/Activity.cpp \
./src/utils/ImageCache.cpp \
./src/utils/Logger.cpp \
./src/utils/PBox.cpp \
./src/utils/Setting.cpp \
./src/utils/SettingsTextView.cpp \
./src/utils/Utils.cpp \
./src/utils/WU_AppUtils.cpp \
./src/utils/Xed_Utils.cpp \
./src/vst_host/ParameterSlider.cpp \
./src/vst_host/VSTConfigureView.cpp \
./src/vst_host/VSTHost.cpp \
./src/vst_host/VSTList.cpp

#	Specify the resource definition files to use. Full or relative paths can be
#	used.
RDEFS = resources/XRS.rdef

#	Specify the resource files to use. Full or relative paths can be used.
#	Both RDEFS and RSRCS can be utilized in the same Makefile.
RSRCS =  resources/XRS.rsrc resources/skin.rsrc

# End Pe/Eddie support.
# @<-src@ 
#%}

#	Specify libraries to link against.
#	There are two acceptable forms of library specifications:
#	-	if your library follows the naming pattern of libXXX.so or libXXX.a,
#		you can simply specify XXX for the library. (e.g. the entry for
#		"libtracker.so" would be "tracker")
#
#	-	for GCC-independent linking of standard C++ libraries, you can use
#		$(STDCPPLIBS) instead of the raw "stdc++[.r4] [supc++]" library names.
#
#	- 	if your library does not follow the standard library naming scheme,
#		you need to specify the path to the library and it's name.
#		(e.g. for mylib.a, specify "mylib.a" or "path/mylib.a")
LIBS =  be localestub translation game media tracker stdc++ /boot/system/lib/libfluidlite.so sndfile samplerate

#	Specify additional paths to directories following the standard libXXX.so
#	or libXXX.a naming scheme. You can specify full paths or paths relative
#	to the Makefile. The paths included are not parsed recursively, so
#	include all of the paths where libraries must be found. Directories where
#	source files were specified are	automatically included.
LIBPATHS = 

#	Additional paths to look for system headers. These use the form
#	"#include <header>". Directories that contain the files in SRCS are
#	NOT auto-included here.
SYSTEM_INCLUDE_PATHS =  

#	Additional paths paths to look for local headers. These use the form
#	#include "header". Directories that contain the files in SRCS are
#	automatically included.
LOCAL_INCLUDE_PATHS =  \
./src/ \
./src/defs \
./src/Engine \
./src/Extensions/JMDrum \
./src/Extensions/Sampler \
./src/Extensions/Tn306 \
./src/Extensions/VIW \
./src/Extensions/XFluidSynth \
./src/GUI \
./src/GUI/MediaBrowser \
./src/GUI/MixerUI \
./src/GUI/new_style2 \
./src/GUI/playlist \
./src/GUI/track \
./src/MainManager \
./src/Mixer \
./src/Valuable \
./src/media \
./src/struct \
./src/utils \
./src/vst_host

#	Specify the level of optimization that you want. Specify either NONE (O0),
#	SOME (O1), FULL (O2), or leave blank (for the default optimization level).
OPTIMIZE := NONE

# 	Specify the codes for languages you are going to support in this
# 	application. The default "en" one must be provided too. "make catkeys"
# 	will recreate only the "locales/en.catkeys" file. Use it as a template
# 	for creating catkeys for other languages. All localization files must be
# 	placed in the "locales" subdirectory.
LOCALES = 

#	Specify all the preprocessor symbols to be defined. The symbols will not
#	have their values set automatically; you must supply the value (if any) to
#	use. For example, setting DEFINES to "DEBUG=1" will cause the compiler
#	option "-DDEBUG=1" to be used. Setting DEFINES to "DEBUG" would pass
#	"-DDEBUG" on the compiler's command line.
DEFINES = 

#	Specify the warning level. Either NONE (suppress all warnings),
#	ALL (enable all warnings), or leave blank (enable default warnings).
WARNINGS = 

#	With image symbols, stack crawls in the debugger are meaningful.
#	If set to "TRUE", symbols will be created.
SYMBOLS := 

#	Includes debug information, which allows the binary to be debugged easily.
#	If set to "TRUE", debug info will be created.
DEBUGGER := TRUE

#	Specify any additional compiler flags to be used.
COMPILER_FLAGS = 

#	Specify any additional linker flags to be used.
LINKER_FLAGS = 

#	(Only used when "TYPE" is "DRIVER"). Specify the desired driver install
#	location in the /dev hierarchy. Example:
#		DRIVER_PATH = video/usb
#	will instruct the "driverinstall" rule to place a symlink to your driver's
#	binary in ~/add-ons/kernel/drivers/dev/video/usb, so that your driver will
#	appear at /dev/video/usb when loaded. The default is "misc".
DRIVER_PATH = 

## Include the Makefile-Engine
DEVEL_DIRECTORY := \
	$(shell findpaths -r "makefile_engine" B_FIND_PATH_DEVELOP_DIRECTORY)
include $(DEVEL_DIRECTORY)/etc/makefile-engine
