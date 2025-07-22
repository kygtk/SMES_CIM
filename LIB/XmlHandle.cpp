// XmlHandle.cpp: implementation of the CXmlHandle class.
//
//////////////////////////////////////////////////////////////////////

#include "XmlHandle.h"
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXmlHandle::CXmlHandle()
{

}

CXmlHandle::~CXmlHandle()
{

}

//XML 파일 이름을 설정
void CXmlHandle::SetXMLFileName(char* szFilePath, char *szFileName)
{
	sprintf(m_szXmlFileName, "%s%s", szFilePath,szFileName);
	m_pXmlDoc.CreateInstance(__uuidof(DOMDocument)); // 인스턴스 생성
	HRESULT hr=m_pXmlDoc->load((_variant_t)m_szXmlFileName);

	if(FAILED(hr)) { // load 실패하면 Error Handle

	}
}

//String값 얻어오기
void CXmlHandle::GetString(char *szNodeName, char *szValue)
{
	m_pNode=m_pXmlDoc->selectSingleNode(L"//EQType");

}

//Integer값 얻어오기
void CXmlHandle::GetInt(char *szNodeName, long &nValue)
{
	m_pNode=m_pXmlDoc->selectSingleNode(L"//EQType");
	nValue=m_pNode->GetnodeValue(); // 현재 노드 이름
}
