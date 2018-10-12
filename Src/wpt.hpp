/*
 * wpt.hpp
 *
 *  Created on: Oct 12, 2018
 *      Author: sid
 */

#ifndef SRC_WPT_HPP_
#define SRC_WPT_HPP_
#include <string>
#include <cmath>
#include <iostream>

#include "coord.hpp"

#define deg2rad(angleDegrees) ((angleDegrees) * M_PI / 180.0)
#define rad2deg(angleRadians) ((angleRadians) * 180.0 / M_PI)

/*
 * Format:
 * 3byte char      'OWB'
 * 1byte uint8_t   version number, currently 1
 * 8byte time_t	   built time
 *
 * Waypoint:
 * 4byte float     latitude rad
 * 4byte float     longitude rad
 * 2byte int16_t   altitude m
 * 1byte uint8_t   flags (reserved)
 * 1byte uint8_t   length name -> n
 * nbyte char	   name (not zero terminated)
 * +0-3 bytes padding for 32bit alignment (filled w/ 0s)
 */

class Wpt
{
public:
 	typedef struct
	{
		/* position */
		float lat_rad;
		float lon_rad;
		int16_t alt;

		/* flags */
		uint8_t flag;
		uint8_t namelen;
	} __attribute__((packed)) owb_header_t;
private:
	const char *id = "OWB\x1";
public:
	std::string name;
	Coord pos;
	int16_t alt;

	Wpt() { reset(); }
	void reset(void);

	void writeFileHeader(std::ofstream &file);
	void write(std::ofstream &file);
};


#endif /* SRC_WPT_HPP_ */
