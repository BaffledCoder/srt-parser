#pragma once
#include "xmlconverter.h"

void block_xml(pugi::xml_node root, CBlock b)
{
	pugi::xml_node block = root.append_child("Block");
	block.append_attribute("start") = b.GetStartTime();
	block.prepend_attribute("end") = b.GetEndTime();

	pugi::xml_node pos = block.append_child("Position") ;
	pos.append_child(pugi::node_pcdata).set_value(b.GetPosition().c_str());

	pugi::xml_node pText = block.append_child("PlainText");
	pText.append_child(pugi::node_pcdata).set_value(b.GetPlainText().c_str());

	pugi::xml_node fText = block.append_child("FormattedTexts");
	std::vector<CFormattedText> vecFtext = b.GetFormattedText();
	for(int i=0;i<vecFtext.size();i++)
	{

		pugi::xml_node text = fText.append_child("FormattedText");
		text.append_attribute("isBold") = vecFtext[i].IsBold();
		text.append_attribute("isItalic") = vecFtext[i].IsItalic();
		text.append_attribute("isUnderline") = vecFtext[i].IsUnderline();
		text.append_attribute("fontColor") = vecFtext[i].GetFontColor().c_str();
		text.append_attribute("backgroundColor") = vecFtext[i].GetBackgroundColor().c_str();
		text.append_child(pugi::node_pcdata).set_value(vecFtext[i].GetText().c_str());
	}
}
void srt_xml(std::vector<CBlock> &data, const char *filename)
{
	pugi::xml_document doc;
	pugi::xml_node xml = doc.append_child(pugi::node_declaration);
	xml.append_attribute("version") = "1.0";
	xml.append_attribute("encoding") = "ISO-8859-1";
	xml.append_attribute("standalone") = "yes";

	pugi::xml_node transcript = doc.append_child("Transcript");
	for(int i=0;i<data.size();i++) block_xml(transcript, data[i]);

	doc.save_file(filename);
}