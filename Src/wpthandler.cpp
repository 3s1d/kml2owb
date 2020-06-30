/*
 * aiphandler.cpp
 *
 *  Created on: Aug 11, 2018
 *      Author: sid
 */


#include "wpthandler.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <fstream>

#include "coord.hpp"
#include "xml.hpp"

using namespace wpt;


void Handler::start_element(const xml::string& name, const xml::attributes& attr)
{
	if(name == "Placemark")
		start_wpt(attr);
	else if(name == "name")
		next_data = NEXT_DATA_NAME;
	else if(name == "coordinates")
		next_data = NEXT_DATA_COORDINATE;
//	else if(name == "visibility" || name  == "description"  || name  == "styleUrl"  || name  == "Point")
//		{ /* do nothing */ }
	else
	{
//		std::cerr << "Unknown element: " << name << std::endl;
//		exit(EXIT_FAILURE);
	}

}

void Handler::end_element(const xml::string& name)
{
	if(next_data != NEXT_DATA_NONE)
	{
		if(next_data == NEXT_DATA_NAME)
		{
//			std::cout << "name: " << handle_str;
			tmp_wpt.name = handle_str;
		}
		else if(next_data == NEXT_DATA_COORDINATE)
		{
			/* convert coordinate */
			//<coordinates>7.51228609605054,49.7926619217307,0 </coordinates>
			std::istringstream ss(handle_str);
			std::string token;

			std::vector<Coord> poly;
			if(std::getline(ss, token, ','))
				tmp_wpt.pos.longitude = deg2rad(std::stof(token));
			if(std::getline(ss, token, ','))
				tmp_wpt.pos.latitude = deg2rad(std::stof(token));
			if(std::getline(ss, token, ','))
				tmp_wpt.alt = std::stoi(token);

//			std::cout << rad2deg(tmp_wpt.pos.latitude) << "," << rad2deg(tmp_wpt.pos.longitude) << " " << tmp_wpt.alt << "m\n";
		}

		handle_str.clear();
		next_data = NEXT_DATA_NONE;
	}
	else if(name == "Placemark")
	{
		waypoints.push_back(tmp_wpt);
	}
}

void Handler::handle_data(const xml::string& content, const int length)
{
	if(next_data == NEXT_DATA_NONE)
		return;

	std::string element(content.c_str(), 0 , length);
	handle_str += element;
}


/*
 * airspace functions
 */

void Handler::start_wpt(const xml::attributes& attr)
{
	/* re-init asp */
	tmp_wpt.reset();

	next_data = NEXT_DATA_NONE;
}

bool Handler::writeOwb(const char *fname)
{
	std::ofstream myFile (fname, std::ios::out | std::ios::binary);

	/* header */
	tmp_wpt.writeFileHeader(myFile);

	/* waypoints */
	for(auto wpt : waypoints)
		wpt.write(myFile);

	myFile.close();

	std::cout << "Written " << waypoints.size() << " waypoints" << std::endl;

	return true;
}


bool Handler::writeWpt(const char *fname)
{
	std::ofstream myFile (fname, std::ios::out);

	/* header */
	char buffer[1024];
	snprintf(buffer, sizeof(buffer), "G  WGS 84\r\nU  1\r\n");
	myFile.write(buffer, strlen(buffer));

	/* waypoints */
	for(auto wpt : waypoints)
	{
		char shortname[8];
		for(int i=0, idx=0; i<6; idx++)
		{
			if(wpt.name.c_str()[idx] != ' ' && (int)wpt.name.c_str()[idx] < 0x80)
				shortname[i++] = wpt.name.c_str()[idx];
			shortname[i] = '\0';
			if(wpt.name.c_str()[idx] == '\0')
				break;
		}
		snprintf(buffer, sizeof(buffer), "W  %s A %.10fº%c %.10fº%c 27-MAR-62 00:00:00 %d.000000 %s\r\n"
				,//"w Waypoint,0,-1.0,16777215,255,1,7,,0.0,\r\n",
				shortname, std::abs(rad2deg(wpt.pos.latitude)), wpt.pos.latitude >=0.0f?'N':'S',
					std::abs(rad2deg(wpt.pos.longitude)), wpt.pos.longitude>=0.0f?'E':'W',
				wpt.alt, wpt.name.c_str());
		myFile.write(buffer, strlen(buffer));
	}

	myFile.close();

	std::cout << "Written " << waypoints.size() << " waypoints to WPT file" << std::endl;

	return true;

}

