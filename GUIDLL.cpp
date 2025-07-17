#include	"InlineExtern.h"

SysLib	T8SysLib;
stSmaRootType	*pstSma;

BOOL	bSmaInitialized = FALSE;
BOOL	bLoaded=FALSE;

//char	szSysinfoFileName[64];

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch( ul_reason_for_call ) 
    { 
        case DLL_PROCESS_ATTACH:	// Initialize once for each new process.
			bSmaInitialized	=	T8SysLib.SmaInitialize(GUI_TASK_ID);
			bLoaded=T8SysLib.LoadSystemData();
			pstSma = T8SysLib.GetSMAPointer();
			
			break;
        case DLL_THREAD_ATTACH:		// Do thread-specific initialization.
			break;
        case DLL_THREAD_DETACH:		// Do thread-specific cleanup.
			break;
        case DLL_PROCESS_DETACH:	// Perform any necessary cleanup.
			break;
    }
    return TRUE;
}

