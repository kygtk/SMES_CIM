#include	"IndexInfo.h"

/*============================================================================
				GET Function
============================================================================*/


// Port에 있는 Cassette의 Glass 개수를 얻는다.
long WINAPI smGetGlassCountInPort(long nPort)
{
	stIndexerInfoType* stIndexerInfo=&pstSma->stIndexInfo;

	return stIndexerInfo->PortInfo[nPort-1].nPanelCount;
}

// Casstte의 Glass Mapping 정보를 얻는다.
long WINAPI smGetPanelMapping(long nPort, long nSlot)
{
	stPortInfoType* stPortInfo=&pstSma->stIndexInfo.PortInfo[nPort-1];

	return stPortInfo->nPanelMapping[nSlot-1]; 
}

// Cassette의 각 Slot에 위치한 Glass의 정보를 얻어온다.
long WINAPI smGetPanelInfoInSlot(long nPort, long nSlot, stPanelInfoType* pDstPanelInfo)
{
	int i=0;
	
	if(nPort > 0 && nPort <= MAX_PORT_COUNT)
	{
		if(nSlot > 0 && nSlot <= MAX_GLASS_COUNT_PER_PORT )
		{
			stPanelInfoType* stPanelInfo=&pstSma->stIndexInfo.PortInfo[nPort-1].PanelInfo[nSlot-1];
			memcpy(pDstPanelInfo, stPanelInfo, sizeof(stPanelInfoType));
		}
	}
	return dSUCCESS;
}

//----- MMI ----
long WINAPI smGetIndexerTimeOut3()
{
	stIndexerInfoType* stIndexerInfo=&pstSma->stIndexInfo;
	
	return stIndexerInfo->nTimeOut3;
}

long WINAPI smGetIndexerTimeOut4()
{
	stIndexerInfoType* stIndexerInfo=&pstSma->stIndexInfo;
	
	return stIndexerInfo->nTimeOut4;
}

long WINAPI smGetIndexerTimeOutRetry()
{
	stIndexerInfoType* stIndexerInfo=&pstSma->stIndexInfo;
	
	return stIndexerInfo->nTimeOutRetry;
}

/*============================================================================
				SET Function
============================================================================*/
void WINAPI smSetIndexerTimeOut3(long nValue)
{
	stIndexerInfoType* stIndexerInfo=&pstSma->stIndexInfo;
	stIndexerInfo->nTimeOut3 =nValue;
}

void WINAPI smSetIndexerTimeOut4(long nValue)
{
	stIndexerInfoType* stIndexerInfo=&pstSma->stIndexInfo;
	stIndexerInfo->nTimeOut4 =nValue;
}

void WINAPI smSetIndexerTimeOutRetry(long nValue)
{
	stIndexerInfoType* stIndexerInfo=&pstSma->stIndexInfo;
	stIndexerInfo->nTimeOutRetry = nValue;
}
