#include <iostream>
#include <memory>


#include "CppGenerater.h"
#include "analyzer.h"
#include "SQLGenerater.h"


int 
main(int argc, char * argv[])
{
	using dogen::CppGenerater;
	using dogen::IGenerater;
	using data_structure::ThriftFile;
	using data_structure::ThriftClass;
	using dogen::Analyzer;
	using dogen::SQLGenerater;

	std::string errstr;

	do {
		if (argc < 3) {
			break;
		}
		ThriftFile tf(argv[1], argv[2]);
		std::shared_ptr<IGenerater> gen(new CppGenerater(tf.GetOutFilename() + "/data_obj_gen.hpp"));
		std::shared_ptr<IGenerater> sqlGen(new SQLGenerater(tf.GetOutFilename() + "/INIT_CREATE_TABLE.sql"));
		Analyzer ana(argv[1]);

		if (!ana.Analyze(&tf, &errstr)) {
			break;
		}

		if (!tf.Check(&errstr)) {
			break;
		}

		if (!gen->Generat(tf)) {
			break;
		}

		if (!sqlGen->Generat(tf)) {
			break;
		}

		exit(EXIT_SUCCESS);
	} while (false);
	std::cout << errstr << std::endl;
	std::cin.get();
	exit(EXIT_FAILURE);
}