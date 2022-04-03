#pragma once
#include <iostream>
#include <utility>
#include <vector>
#include "ftext.h"
class CBlock
{
	//int serial;
	std::string m_sPosition;
	int m_nStartTime;
	int m_nEndTime;
	std::pair<std::string,std::vector<CFormattedText>> m_Text;
public:
	CBlock();
	CBlock(int st, int end, std::string plainText, std::vector<CFormattedText> formattedText, std::string pos = "Bottom");
	std::string GetPosition();
	int GetStartTime();
	int GetEndTime();
	std::string GetPlainText();
	std::vector<CFormattedText> GetFormattedText();
	void SetPosition(std::string pos);
	void SetStartTime(int st);
	void SetEndTime(int end);
	void SetText(std::pair<std::string,std::vector<CFormattedText>> text);
};