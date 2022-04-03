#pragma once
#include "pugiconfig.hpp"
#include "pugixml.hpp"
#include "parser.h"

void block_xml(pugi::xml_document doc, CBlock b);
void srt_xml(std::vector<CBlock> &data, const char *filename);