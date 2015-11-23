#ifndef __PUGIXML_TEST__
#define __PUGIXML_TEST__


#include "iegad_common.h"
#include <iostream>


bool 
create_xmldoc(const std::string & filename)
{
    using namespace  pugi;
    xml_document doc;
    do {
	xml_node root = doc.append_child("ApplicationConfiguration");
	xml_node data_node = root.append_child("MySQL");
	xml_node connstr_node = data_node.append_child("ConnectionString");
	if (!connstr_node.set_name("ConnectionString")) {
	    break;
	}

	std::string valstr = "server=127.0.0.1;user=iegad;pwd=1111;database=iopc;charset=gbk;";
	connstr_node.text() = valstr.c_str();
	return doc.save_file(filename.c_str());
    } while (false);
    return false;
}


bool
load_xmldoc(const std::string & filename)
{
    using namespace  pugi;
    xml_document doc;
    do {
	xml_parse_result rzt;
	if (rzt = doc.load_file(filename.c_str()), rzt.status != status_ok) {
	    break;
	}

	xml_node root = doc.child("ApplicationConfiguration");
	xml_node connstr_node = root.child("MySQL").child("ConnectionString");	
	std::cout << connstr_node.text().as_string() << std::endl;
	connstr_node.text() = "Modified";
	return doc.save_file(filename.c_str());	
    } while (false);
    return false;
}






#endif // __PUGIXML_TEST__