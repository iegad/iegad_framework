#ifndef __SQL_GENERATER__
#define __SQL_GENERATER__



#include "IGenerater.h"



namespace dogen {


	class SQLGenerater : public IGenerater {
	public:
		explicit SQLGenerater(const std::string & outFilePath)
			: IGenerater(outFilePath) {}


	protected:
		virtual bool _head(std::ofstream & fout) override {
			return true;
		}

		virtual bool _include(std::ofstream & fout) override {
			return true;
		}

		virtual bool _tail(std::ofstream & fout) override {
			return true;
		}

		virtual bool _namespaceBegin(const ::data_structure::ThriftNamespace & thriftNamespace, std::ofstream & fout) override {
			return true;
		}

		virtual bool _namespaceEnd(const ::data_structure::ThriftNamespace & thriftNamespace, std::ofstream & fout) override {
			return true;
		}

		virtual bool _classInfo(const data_structure::ThriftClass & thriftClass, std::ofstream & fout) override;

		virtual bool _statment(std::ofstream & fout) override;
	private:
		bool _tabHead(const data_structure::ThriftClass & thriftClass, std::ofstream & fout);
		bool _tabTail(const data_structure::ThriftClass & thriftClass, std::ofstream & fout);
		bool _addTabFiled(const data_structure::ThriftClass & thriftClass, std::ofstream & fout);
		const std::string _getType(const std::string & typeStr);
	}; // class SQLGenerater;


} // namespace dogen;


#endif // __SQL_GENERATER__