#ifndef _XRS_LOCALE_
#define _XRS_LOCALE_

#ifdef ZETA
#include <locale/Locale.h>
#else
#define _T(A) A 
#endif


//general

#define T_GEN_REMOVE		_T("Remove")
#define T_GEN_NO			_T("No")
#define T_GEN_DONE			_T("Done!")
#define	T_GEN_CLOSE			_T("Close")
#define	T_GEN_OK			_T("Ok!")
#define T_GEN_CANCEL		_T("Cancel")
#define T_GEN_SET			_T("Set")
#define T_GEN_SAVE			_T("Save")
#define T_GEN_EXPORT		_T("Export")
#define T_GEN_RENAME		_T("Rename")
#define T_GEN_PLAYLIST		_T("Playlist")
#define T_GEN_MEASURE		_T("Measure")
#define T_GEN_ADD			_T("Add")
#define T_GEN_RESET			_T("Reset")
#define T_GEN_RANDOMIZE		_T("Randomize")


// menus
#define T_MENU_FILE			_T("File")
#define T_MENU_EDIT			_T("Edit")
#define T_MENU_EXTENSION	_T("Extensions")
#define T_MENU_NEW			_T("New")
#define T_MENU_OPEN			_T("Open")
#define T_MENU_DEFAULT		_T("Default")
#define T_MENU_EMPTY		_T("Empty")
#define T_MENU_SAVE			T_GEN_SAVE
#define T_MENU_SAVE_AS		_T("Save As..")
#define T_MENU_EXPORT_AS_WAVE	_T("Export as WAVE")
#define T_MENU_COLLECT_AS_ZIP	_T("Collect as Zip")
#define T_MENU_ABOUT			_T("About")
#define T_MENU_QUIT				_T("Quit")
#define T_MENU_TRACK			_T("Track")
#define T_MENU_REMOVE			T_GEN_REMOVE
#define T_MENU_CUT				_T("Cut")
#define T_MENU_COPY				_T("Copy")
#define T_MENU_PASTE			_T("Paste")
#define T_MENU_MEASURE			T_GEN_MEASURE
#define T_MENU_PLAYLIST			T_GEN_PLAYLIST
#define T_MENU_SONG_SETTINGS	_T("Song Settings..")
#define T_MENU_GENERAL_PREFS 	_T("Preferences...")
#define T_MENU_HIDE_ALL			_T("Hide All")

//dialog
#define T_DIALOG_REMOVE_TRACK	_T("Remove selected Track ?")
#define T_DIALOG_LOADING_FILE	_T("Loading file...")
#define T_DIALOG_EXPORT_AS_WAVE	_T("Exporting as Wave ..")
#define T_DIALOG_CLOSE_SONG		_T("Close current Song?")
#define T_DIALOG_WRITTEN_BY		_T("Written by : ")
#define T_DIALOG_LOADING		_T("Loading..")

//song settings
#define T_SETTINGS_TITLE				_T("Song Settings")
#define T_SETTINGS_SONG_DESCRIPTION		_T("Song Description")
#define	T_SETTINGS_POPUP_ONLOAD			_T("PopUp on load")
#define T_SETTINGS_NOTES_PER_MEASURE 	_T("Beats per Measure")
#define T_SETTINGS_BEAT_DIVISION		_T("Beat division")

//export
#define	T_EXP_TITLE				_T("Export Song")
#define	T_EXP_SECTION			_T("Section")
#define	T_EXP_FORMAT			_T("Format")
#define T_EXP_MIXER				_T("MixerLines")
#define T_EXP_SINGLE_MEASURE	_T("Single Measure:")
#define	T_EXP_ALL_SONG			_T("All the song")
#define T_EXP_MASTER_LINE		_T("Export only master line")
#define T_EXP_FX_LINES			_T("Export also fx lines")

//mixer
#define T_MIXER_TITLE			_T("Mixer")
#define	T_MIXER_NO_VST			_T("<no vst selected>")
#define T_MIXER_MASTER			_T("master")
#define T_MIXER_FXLINE			_T("fx line")
#define T_MIXER_NONE			_T("<none>")
#define T_MIXER_CHOOSE_VST		_T("Choose a VST:")
#define T_MIXER_CANT_FIND		_T("Can't find VST")

//playlist
#define	T_PLAYLIST_TITLE 		_T("Playlist")
#define T_PLAYLIST_MEASURES		_T("Measures")	
#define T_PLAYLIST_LOOP_POINTS	_T("Loop points")

//mediabrowser
#define T_BROWSER_TITLE			_T("MediaBrowser")

//trackinfo
#define T_TRACKINFO_TITLE		_T("TrackInfo")
#define T_TRACKINFO_NOTRACK		_T("<no tracks>")
#define	T_TRACKINFO_MIDIN		_T("Midi-In Channel")

//Track
#define T_TRACK_PIANO			_T("Piano")
#define T_TRACK_PARAMS			_T("Params")
#define T_TRACK_VELOCITY		_T("Velocity")
#define T_TRACK_PAN				_T("Pan")
#define T_TRACK_SUSTAIN			_T("Sustain")
// vst plug window
#define T_VSTWIN_PRESETS   		_T("Presets")
#define T_VSTWIN_STANDARD		_T("Standard")
#define T_VSTWIN_USER			_T("User")
#define T_VSTWIN_NAME			_T("Name for this Preset:")

//error loading.. :(
#endif
