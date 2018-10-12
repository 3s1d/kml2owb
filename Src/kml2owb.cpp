/*
 * oaip2oab.cpp
 *
 *  Created on: Aug 11, 2018
 *      Author: sid
 */


#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include "wpthandler.hpp"

#include "xml.hpp"

using namespace std;

struct path_leaf_string
{
    std::string operator()(const boost::filesystem::directory_entry& entry) const
    {
        return entry.path().leaf().string();
    }
};

void read_directory(const std::string& name, vector<string>& v)
{
	boost::filesystem::path p(name);
	boost::filesystem::directory_iterator start(p);
	boost::filesystem::directory_iterator end;
	std::transform(start, end, std::back_inserter(v), path_leaf_string());
}

int main(int argc, char* argv[])
{
	cerr << "KML to open waypoint binary (OWB) converter" << endl; // prints API to waypoint converter

	if (argc != 2)
	{
		cerr << "usage: " << argv[0] << " kmlFolder " << endl;
		return 1;
	}

	/* get files */
	// note: no subfolders supported
	vector<string> files;
	read_directory(argv[1], files);

	wpt::Handler wptHandler;

	/* iterate over all files */
	for(auto file : files)
	{
		/* is aip file */
		//todo improve check
		if(file.find(".kml") == std::string::npos)
			continue;
		string fname = string(argv[1]);
		fname += "/" + file;
		std::cout << "Converting file: " << fname << std::endl;

		/* add to aip handler */
		xml::parser <wpt::Handler> parser(wptHandler);
		parser.parse(fname);
	}

	wptHandler.writeOwb("world.owb");

	return 0;
}


