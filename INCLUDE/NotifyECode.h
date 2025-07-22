#ifndef		__NotifyECode_h__
#define		__NotifyECode_h__

/**************************************************************
				 Notify Error Code 관련 Structure : 미사용 Reserved
**************************************************************/
enum eNotify_ECode
{
	eCode_DUPL_INDEX	= 600,
	eCode_NONE_INDEX	= 601,
	eCode_SMA_ACCESS	= 602,
	eCode_DB_ACCESS		= 603,	
	eCode_DISPLAY_ERR	= 604,
	eCode_THREAD_ERR	= 605,
};

enum ePLC_ECode
{
	eCode_PPID_NAK		= 200,
	eCode_AlarmTr_ERR	= 201,
	eCode_ECID_ERR		= 202,
	eCode_Machine_ERR	= 203,
	eCode_EndEvent_ERR	= 204,
	eCode_Recipe_Cancel	= 205,
	eCode_Recipe_NotUsed = 206,
	eCode_Recipe_NULL	= 207,
	eCode_Scrap_Cancel	= 208,
	eCode_Scrap_Code_ERR = 209,
	eCode_Unscrap_Cancel = 210,
	eCode_Unscrap_NULL	 = 211,
};
#endif	//NotifyECode.H