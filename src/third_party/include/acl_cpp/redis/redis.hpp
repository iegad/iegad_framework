#pragma once
#include "acl_cpp/acl_cpp_define.hpp"
#include "acl_cpp/redis/redis_connection.hpp"
#include "acl_cpp/redis/redis_hash.hpp"
#include "acl_cpp/redis/redis_hyperloglog.hpp"
#include "acl_cpp/redis/redis_key.hpp"
#include "acl_cpp/redis/redis_list.hpp"
#include "acl_cpp/redis/redis_pubsub.hpp"
#include "acl_cpp/redis/redis_script.hpp"
#include "acl_cpp/redis/redis_server.hpp"
#include "acl_cpp/redis/redis_set.hpp"
#include "acl_cpp/redis/redis_string.hpp"
#include "acl_cpp/redis/redis_transaction.hpp"
#include "acl_cpp/redis/redis_zset.hpp"
#include "acl_cpp/redis/redis_cluster.hpp"
#include "acl_cpp/redis/redis_geo.hpp"

namespace acl
{

/**
 * ����̳������� redis �����࣬��˿���ֻͨ���������ʹ�����е� redis ���
 * inherit all the redis command class, which include all the commands
 * of Key, String, Hash, List, Set, SortedSet, Hyperloglog, Pub/Sub,
 * Transaction, Script, Connection, Server.
 */
class ACL_CPP_API redis
	: public redis_connection
	, public redis_hash
	, public redis_hyperloglog
	, public redis_key
	, public redis_list
	, public redis_pubsub
	, public redis_script
	, public redis_server
	, public redis_set
	, public redis_string
	, public redis_transaction
	, public redis_zset
	, public redis_cluster
	, public redis_geo
{
public:
	/**
	 * �Ǽ�Ⱥ��ʽ�Ĺ��캯��
	 * the constructor with no redis cluster
	 * @param conn {redis_client*} һ�� redis �ڵ�����Ӷ���
	 *  one redis node's connection
	 */
	redis(redis_client* conn = NULL);

	/**
	 * ��Ⱥ��ʽ�Ĺ��캯��
	 * the constructor in redis cluster mode
	 * @param cluster {redis_client_cluster*} ��Ⱥ����
	 *  the redis cluster object
	 * @param max_conns {size_t} ��Ⱥ��ʽ������ÿ�� redis ����ڵ��
	 *  ���ӳ��������ޣ������Ϊ 0����ÿ�����ӳ�û����������
	 *  the limit of each connections pool in redis cluster mode,
	 *  there is no connections limit of each pool if the max_conns
	 *  is set to 0.
	 */
	redis(redis_client_cluster* cluster, size_t max_conns = 0);
	~redis(void) {}
};

} // namespace acl
