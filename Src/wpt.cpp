/*
 * wpt.cpp
 *
 *  Created on: Oct 12, 2018
 *      Author: sid
 */

#include <iostream>
#include <fstream>
#include <cstring>

#include "wpt.hpp"

void Wpt::reset(void)
{
	pos = Coord();
	name = "";
	alt = 0;
}

void Wpt::writeFileHeader(std::ofstream &file)
{
	file.write(id, strlen(id));

	time_t t = time(nullptr);
	file.write((char *) &t, sizeof(time_t));
}

void Wpt::write(std::ofstream &file)
{
	/* header */
	owb_header_t hdr = {0};
	hdr.lat_rad = pos.latitude;
	hdr.lon_rad = pos.longitude;
	hdr.alt = alt;
	hdr.flag = 0;
	hdr.namelen = name.length();
	file.write((char *) &hdr, sizeof(owb_header_t));

	/* name */
	file.write((char *) name.c_str(), name.length());

	/* padding */
	int padding = 4 - (name.length() % 4);
	uint8_t buf[3] = {0, 0, 0};
	if(padding < 4)
		file.write((char *) &buf, padding);
}

