// XmlHandle.h: interface for the CXmlHandle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLHANDLE_H__745B2FDF_B578_439C_8C78_04BAF6D11288__INCLUDED_)
#define AFX_XMLHANDLE_H__745B2FDF_B578_439C_8C78_04BAF6D11288__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//XML 사용 선언
#import <MSXML4.dll>
using namespace MSXML2;

class CXmlHandle  
{
public:
	CXmlHandle();
	virtual ~CXmlHandle();

protected:
	char	m_szXmlFileName[ 255 ];	// XML Config File Name

	void GetInt(char* szNodeName, long& nValue);
	void GetString(char* szNodeName, char* szValue);
	void SetXMLFileName(char* szFilePath, char *szFileName);

	//XML Pointer
	IXMLDOMDocument2Ptr m_pXmlDoc;
	IXMLDOMProcessingInstructionPtr m_pXmlPI;

	IXMLDOMNodeListPtr m_pNodeList;		// Node List
	IXMLDOMNodePtr	m_pNode;			// Single Node		
	IXMLDOMNamedNodeMapPtr m_pNodeMap;	// for attribute
};

#endif // !defined(AFX_XMLHANDLE_H__745B2FDF_B578_439C_8C78_04BAF6D11288__INCLUDED_)
