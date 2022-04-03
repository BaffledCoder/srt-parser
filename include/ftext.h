#pragma once
#include <iostream>
class CFormattedText
{
	bool m_bIsBold;
	bool m_bIsItalic;
	bool m_bIsUnderline;
	std::string m_sText;
	std::string m_sFontColor;
	std::string m_sBackgroundColor;
public:
	CFormattedText(std::string text, bool bold = false, bool italic = false, bool underline = false, std::string font = "#000000", std::string bg = "#00FFFFFF");
	bool IsBold();
	bool IsItalic();
	bool IsUnderline();
	std::string GetText();
	std::string GetFontColor();
	std::string GetBackgroundColor();
	void SetBold(bool bold);
	void SetItalic(bool Italic);
	void SetUnderline(bool Underline);
	void SetText(std::string text);
	void SetFontColor(std::string color);
	void SetBackgroundColor(std::string color);
};