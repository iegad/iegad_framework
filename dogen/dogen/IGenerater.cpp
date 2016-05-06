#include "IGenerater.h"


bool 
dogen::IGenerater::Generat(const data_structure::ThriftFile & thriftFile)
{
	std::ofstream fout;
	do {
		fout.open(outFilePath_, std::ios_base::out | std::ios_base::trunc);
		if (fout.fail()){
			break;
		}
		
		if (!this->_head(fout)) {
			break;
		}

		if (!this->_statment(fout)) {
			break;
		}

		if (!this->_include(fout)) {
			break;
		}

		if (!this->_namespaceBegin(thriftFile.GetNameSpace(), fout)) {
			break;
		}

		const data_structure::ThriftFile::ThriftClassMap_t & cmap = thriftFile.GetClassMap();
		for (auto itor = cmap.begin(); itor != cmap.end(); itor++) {
			if (!this->_classInfo(itor->second, fout)) {
				fout.close();
				return false;
			}
		} // for (auto itor = cmap.begin(); itor != cmap.end(); itor++);

		if (!this->_namespaceEnd(thriftFile.GetNameSpace(), fout)) {
			break;
		}

		if (!this->_tail(fout)) {
			break;
		}
		
		return true;
	} while (false);

	if (fout.is_open()) {
		fout.close();
	}
	return false;
}


bool
dogen::IGenerater::_statment(std::ofstream & fout)
{
	fout << "/* ===============================\n";
	fout << "\t该代码由 dogen V0.0.1 生成\n\n";
	fout << "\t@用途 : \n";
	fout << "\t\t该文件依赖 thrift 定义的结构, 并扩展 thirft 结构\n\n";
	fout << "\t\t1,  为结构提供 数据库的 增, 删, 改, 查 的SQL操作\n";
	fout << "\t\t2, 为结构提供 JSON, 二进制 序列化和反序列化\n\n";
	fout << "\t@PS : 需要手动将thrift 生成的 XXX_XXX_type.h 的头文件手动添进来\n\n";
	fout << "\t@作者 : iegad\n";
	fout << " =============================== */\n\n\n";
	return !fout.fail();
}
