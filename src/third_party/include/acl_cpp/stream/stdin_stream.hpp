#pragma once
#include "acl_cpp/acl_cpp_define.hpp"
#include "acl_cpp/stream/istream.hpp"

namespace acl {

/**
 * ��׼�����������������ܽ��ж�����
 */

class ACL_CPP_API stdin_stream : public istream
{
public:
	stdin_stream();
	~stdin_stream();
};

} // namespace acl
