// PLC_Version.cpp: implementation of the PLC_Version class.
//
//////////////////////////////////////////////////////////////////////

#include "PLC_Version.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void PLC_Version::V102_20090307()
{
	/* 
		책갈피 : V102
		작성자 : 손재원
		원  인 : USER 요청
		내  용 : 1. [Slit Coater] DVID에 Barcode 추가 보고
				 2. [전설비] Task별 Version 및 History 관리 시작
	*/
}
void PLC_Version::V103_20090522()
{
	/* 
		책갈피 : V103
		작성자 : 고우영
		원  인 : USER 요청
		내  용 : 1. Module out 보고 후 Module In보고 500ms Delay 추가
				 2. FDC보고시 사용 탱크만 구분해서 보고하도록 수정
				 3. S7F107 보고시 ModuleName 보고하도록 수정
	*/			

}

void PLC_Version::V104_20090715()
{
	/* 
		책갈피 : V104
		작성자 : 백승규
		원  인 : USER 요청
		내  용 : 1. 액교환/액보충 횟수에 대한 로그 기록.
				 2. bChemicalSupply[4] 변수를 추가.
				 3. GUI에서 Bit 변화를 감지하여 Count를 센다.
	*/			

}
