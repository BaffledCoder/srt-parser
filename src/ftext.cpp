#pragma once
#include "ftext.h"

CFormattedText::CFormattedText(std::string text, bool bold, bool italic, bool underline, std::string font, std::string bg)
{
	m_bIsBold = bold;
	m_bIsItalic = italic;
	m_bIsUnderline = underline;
	m_sText = text;
	m_sFontColor = font;
	m_sBackgroundColor = bg;
}
bool CFormattedText::IsBold() {return m_bIsBold;}
bool CFormattedText::IsItalic() {return m_bIsItalic;}
bool CFormattedText::IsUnderline() {return m_bIsUnderline;}
std::string CFormattedText::GetText() {return m_sText;}
std::string CFormattedText::GetFontColor() {return m_sFontColor;}
std::string CFormattedText::GetBackgroundColor() {return m_sBackgroundColor;}
void CFormattedText::SetBold(bool bold) {m_bIsBold = bold;}
void CFormattedText::SetItalic(bool italic) {m_bIsItalic = italic;}
void CFormattedText::SetUnderline(bool underline) {m_bIsUnderline = underline;}
void CFormattedText::SetText(std::string text) {m_sText = text;}
void CFormattedText::SetFontColor(std::string color) {m_sFontColor = color;}
void CFormattedText::SetBackgroundColor(std::string color) {m_sBackgroundColor = color;}