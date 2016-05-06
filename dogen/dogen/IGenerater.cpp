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
	fout << "\t�ô����� dogen V0.0.1 ����\n\n";
	fout << "\t@��; : \n";
	fout << "\t\t���ļ����� thrift ����Ľṹ, ����չ thirft �ṹ\n\n";
	fout << "\t\t1,  Ϊ�ṹ�ṩ ���ݿ�� ��, ɾ, ��, �� ��SQL����\n";
	fout << "\t\t2, Ϊ�ṹ�ṩ JSON, ������ ���л��ͷ����л�\n\n";
	fout << "\t@PS : ��Ҫ�ֶ���thrift ���ɵ� XXX_XXX_type.h ��ͷ�ļ��ֶ������\n\n";
	fout << "\t@���� : iegad\n";
	fout << " =============================== */\n\n\n";
	return !fout.fail();
}
