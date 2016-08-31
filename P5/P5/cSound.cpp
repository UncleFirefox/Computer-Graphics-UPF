
/****************************************************************************
                 cSound.cpp  by  Elmü (elmue.de.vu)
****************************************************************************/


#include "stdafx.h"
#include "cSound.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define _AFXDLL

cSound::cSound()
{
}

cSound::~cSound()
{
}

/****************************************
       Plays Wav or Midi file
****************************************/

DWORD cSound::PlaySoundFile(char *p_s8File)
{
	// It is important to check if the file exists !
	// On Windows NT PlaySound() returns TRUE even if the file does not exist !
	// Then PlaySound() makes the PC speaker beep !!!
	
	// mciSendString("open...") also gives an absolutely stupid error message
	// if the file does not exist !

	DWORD u32_Attr = ::GetFileAttributes(p_s8File);
	if (u32_Attr == 0xFFFFFFFF || (u32_Attr & FILE_ATTRIBUTE_DIRECTORY)) 
		return ERROR_FILE_NOT_FOUND;


	// Get file extension
	char *p_s8Ext = strrchr(p_s8File, '.');

	if (stricmp(p_s8Ext, ".wav") == 0)
	{
		//StopSoundFile();
		
		// PlaySound() is very primitive: no Error Code available
		if (!PlaySound(p_s8File, 0, SND_FILENAME | SND_ASYNC))
			return ERR_PLAY_WAV;

		return 0;
	}
	

	DWORD u32_Err;
	if (!stricmp(p_s8Ext, ".mid") || !stricmp(p_s8Ext, ".midi") || !stricmp(p_s8Ext, ".rmi"))
	{
		StopSoundFile();

		static char s8_LastFile[MAX_PATH] = "";

		// the mciSendString("open...") command is slow (on Windows NT, 2000 and XP) 
		// so we call it only if necessary
		if (strcmp(s8_LastFile, p_s8File) != 0)
		{
			strcpy(s8_LastFile, p_s8File);

			mciSendString("close all", 0, 0, 0);

			char s8_Buf[300];

			sprintf(s8_Buf, "play \"%s\"", p_s8File);
			if (u32_Err=mciSendString(s8_Buf, NULL, 0, 0))
				return u32_Err;
		}

		return 0;
	}

	return ERR_INVALID_FILETYPE;
}

DWORD cSound::PlayWavFileLoop(char *p_s8File)
{
	// It is important to check if the file exists !
	// On Windows NT PlaySound() returns TRUE even if the file does not exist !
	// Then PlaySound() makes the PC speaker beep !!!
	
	// mciSendString("open...") also gives an absolutely stupid error message
	// if the file does not exist !

	DWORD u32_Attr = ::GetFileAttributes(p_s8File);
	if (u32_Attr == 0xFFFFFFFF || (u32_Attr & FILE_ATTRIBUTE_DIRECTORY)) 
		return ERROR_FILE_NOT_FOUND;


	// Get file extension
	char *p_s8Ext = strrchr(p_s8File, '.');

	if (stricmp(p_s8Ext, ".wav") == 0)
	{
		//StopSoundFile();
		
		// PlaySound() is very primitive: no Error Code available
		if (!PlaySound(p_s8File, 0, SND_FILENAME | SND_ASYNC | SND_LOOP | SND_NOSTOP | SND_NOWAIT))
			return ERR_PLAY_WAV;

		return 0;
	}

	return ERR_INVALID_FILETYPE;
}

DWORD cSound::PlayWavFile(char *p_s8File)
{
	// It is important to check if the file exists !
	// On Windows NT PlaySound() returns TRUE even if the file does not exist !
	// Then PlaySound() makes the PC speaker beep !!!
	
	// mciSendString("open...") also gives an absolutely stupid error message
	// if the file does not exist !

	DWORD u32_Attr = ::GetFileAttributes(p_s8File);
	if (u32_Attr == 0xFFFFFFFF || (u32_Attr & FILE_ATTRIBUTE_DIRECTORY)) 
		return ERROR_FILE_NOT_FOUND;


	// Get file extension
	char *p_s8Ext = strrchr(p_s8File, '.');

	if (stricmp(p_s8Ext, ".wav") == 0)
	{
		//StopSoundFile();
		
		// PlaySound() is very primitive: no Error Code available
		if (!PlaySound(p_s8File, 0, SND_FILENAME | SND_ASYNC | SND_NOSTOP | SND_NOWAIT ))
			return ERR_PLAY_WAV;

		return 0;
	}

	return ERR_INVALID_FILETYPE;
}



/**************************************************
       Stops the currently playing Wav and Midi
***************************************************/

void cSound::StopSoundFile()
{
	PlaySound(0, 0, SND_PURGE); // Stop Wav

    mciSendString("stop MidiDemo", 0, 0, 0); // Stop Midi
}

