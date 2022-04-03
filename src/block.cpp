#pragma once
#include "block.h"


CBlock::CBlock() {m_sPosition = "Bottom";}
CBlock::CBlock(int st, int end, std::string plainText, std::vector<CFormattedText> formattedText, std::string pos)
{
	m_sPosition = pos;
	m_nStartTime = st;
	m_nEndTime = end;
	std::pair<std::string, std::vector<CFormattedText>> p;
	p.first = plainText;
	p.second = formattedText;
	m_Text = p;
}
std::string CBlock::GetPosition() {return m_sPosition;}
int CBlock::GetStartTime() {return m_nStartTime;}
int CBlock::GetEndTime() {return m_nEndTime;}
std::string CBlock::GetPlainText() {return m_Text.first;}
std::vector<CFormattedText> CBlock::GetFormattedText() {return m_Text.second;}
void CBlock::SetPosition(std::string pos) {m_sPosition = pos;}
void CBlock::SetStartTime(int st) {m_nStartTime = st;}
void CBlock::SetEndTime(int end) {m_nEndTime = end;}
void CBlock::SetText(std::pair<std::string,std::vector<CFormattedText>> text) {m_Text = text;}