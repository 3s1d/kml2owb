/*
 * aiphandler.hpp
 *
 *  Created on: Aug 11, 2018
 *      Author: sid
 */

#ifndef SRC_WPTHANDLER_HPP_
#define SRC_WPTHANDLER_HPP_

#include <vector>

#include "wpt.hpp"
#include "xml.hpp"

#define NEXT_DATA_NONE		0
#define NEXT_DATA_NAME		2
#define NEXT_DATA_COORDINATE	3



namespace wpt
{

class Handler
{
private:
	void start_wpt(const xml::attributes& attr);

	void alt_ref(const xml::attributes& attr);
	void polygons(std::string line);

	std::string handle_str;

	int next_data = NEXT_DATA_NONE;

	Wpt tmp_wpt;
	std::vector<Wpt> waypoints;
public:
	Handler() { }
	~Handler() { waypoints.clear(); }

	void start_element(const xml::string& name, const xml::attributes& attr);
	void end_element(const xml::string& name);
	void handle_data(const xml::string& content, const int length);
	void handle_comment(const XML_Char *comment) { }

	void test() { std::cout << "num: " << waypoints.size() << std::endl; }
	bool writeOwb(const char *fname);
};

}



#endif /* SRC_WPTHANDLER_HPP_ */
