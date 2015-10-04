#ifndef __IEGAD_FRAMEWORK__
#define __IEGAD_FRAMEWORK__


#ifdef WIN32

#pragma comment(lib, "iegad_framework.lib")
#pragma comment(lib, "libprotoc.lib")
#pragma comment(lib, "libprotobuf.lib")

#endif // WIN32


#include "string/iegad_string.h"
#include "db/iegad_dbtab.h"
#include "net/tcp_mt_server.h"
#include "common/iegad_log.h"
#include "common/job_queue.hpp"
#include "common/job_worker.hpp"


#endif // __IEGAD_FRAMEWORK__