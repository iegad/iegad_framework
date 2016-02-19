#pragma once
#include "acl_cpp/acl_cpp_define.hpp"
#include <vector>
#include <map>
#include "acl_cpp/stdlib/string.hpp"
#include "acl_cpp/redis/redis_command.hpp"

namespace acl
{

typedef class redis_client disque_client;
typedef class redis_client_pool disque_client_pool;
typedef class redis_client_cluster disque_client_cluster;
class disque_cond;
class disque_node;
class disque_job;

/**
 * disque ���������
 */
class ACL_CPP_API disque : virtual public redis_command
{
public:
	/**
	 * see redis_command::redis_command()
	 */
	disque();

	/**
	 * see redis_command::redis_command(redis_client*)
	 */
	disque(redis_client* conn);

	/**
	 * see redis_command::redis_command(redis_client_cluster*�� size_t)
	 */
	disque(redis_client_cluster* cluster, size_t max_conns);

	virtual ~disque();

	/////////////////////////////////////////////////////////////////////

	/**
	 * add a job to the specified queue
	 * ���һ��������Ϣ��ָ���Ķ�����
	 * @param name {const char*} the name of the specified queue
	 *  ��������
	 * @param job {const char*} a message to deliver
	 *  ������Ϣ�ַ���
	 * @param timeout {int} the command timeout in milliseconds
	 *  ������ִ�еĳ�ʱʱ�䣨���룩
	 * @param args {const std::map<acl::string, int>*} the condition
	 *  for ADDJOB command, the conditions name include:
	 *  REPLICATE, DELAY, RETRY, TTL, MAXLEN, ASYNC, if the args was NULL,
	 *  none condition will be used in this operation
	 *  �����Ϣ�Ĵ����������ϣ���Ӧ�����������
	 *   REPLICATE -- ����������
	 *   DELAY -- ָ�������ڷ�������ڵ�Ķ���֮ǰ�� ��Ҫ�ȴ���������
	 *   TTL -- �����������ڣ��룩
	 *   MAXLEN -- ָ�����������Դ�Ŷ��ٸ������ݵ�����
	 *   ASYNC -- ����˲����첽��ʽ������ͬ�����丱�����
	 * @return {const char*} a ID of the job will be returned, NULL will
	 *  be returned if some error happened.
	 *  �������� ID �ţ�������� NULL ���ʾ����
	 */
	const char* addjob(const char* name, const char* job,
		int timeout, const std::map<string, int>* args = NULL);
	const char* addjob(const char* name, const string& job,
		int timeout, const std::map<string, int>* args = NULL);
	const char* addjob(const char* name, const void* job, size_t job_len,
		int timeout, const std::map<string, int>* args = NULL);

	/**
	 * add a job to the specified queue
	 * ��ָ����Ϣ�����������
	 * @param name {const char*} the name of the specified queue
	 *  ָ������Ϣ��������
	 * @param job {const char*} a message to deliver
	 *  ����ӵ�����
	 * @param timeout {int} the command timeout in milliseconds
	 *  ���뾫�ȵ����ʱ����
	 * @param cond {const acl::disque_cond*} the condition for the ADDJOB
	 *  ���������������μ��� disque_cond
	 * @return {const char*} a ID of the job will be returned, NULL will
	 *  be returned if some error happened.
	 *  �������� ID �ţ�������� NULL ���ʾ����
	 */
	const char* addjob(const char* name, const char* job,
		int timeout, const disque_cond* cond);
	const char* addjob(const char* name, const string& job,
		int timeout, const disque_cond* cond);
	const char* addjob(const char* name, const void* job, size_t job_len,
		int timeout, const disque_cond* cond);

	/**
	 * get jobs from the specified queues, or return NULL if the timeout
	 * is reached.
	 * ��ָ���Ķ��м����л��ȡָ���������������
	 * @param names {const std::vector<acl::string>&} the specified queues
	 *  ָ�����б����Ƽ���
	 * @param timeout {int} the command timeout in milliseconds
	 *  ���뾫�ȵ����ʱ����
	 * @param count {size_t} the max count of the jobs to be got
	 *  ָ���˷�����������������������
	 * @return {const std::vector<acl::disque_job*>*} return the jobs,
	 *  or return NULL if the timeout is reached or some error happens.
	 *  ���ؽ�����������ʱ���������򷵻� NULL
	 */
	const std::vector<disque_job*>* getjob(const std::vector<string>& names,
		size_t timeout, size_t count);
	const std::vector<disque_job*>* getjob(const char* name,
		size_t timeout, size_t count);

	/**
	 * acknowledge the execution of one or more jobs via IDs. The node
	 * receiving the ACK will replicate it to multiple nodes and will try
	 * to garbage collect both the job and the ACKs from the cluster so
	 * that memory can be freed.
	 * ͨ���������� ID �� ��ڵ��֪�����Ѿ���ִ�С����յ� ACK ��Ϣ�Ľڵ�Ὣ����Ϣ
	 * ����������ڵ㣬 �����Զ���������Լ�Ⱥ�� ACK ��Ϣ�����������ղ����� �Ӷ��ͷ�
	 * ��ռ�õ��ڴ档
	 * @param job_ids {const std::vector<acl::string>&} the jobs' IDs
	 *  ���� ID ����
	 * @return {int} return the number of IDs been ACKed, -1 will be
	 *  returned if some error happened
	 *  ���ر�ȷ�ϵ������������������򷵻� -1
	 */
	int ackjob(const std::vector<string>& job_ids);

	/**
	 * perform a best effort cluster wide detection of the specified
	 * job IDs.
	 * �����Ŭ���ڼ�Ⱥ��Χ�ڶԸ������������ɾ�����������������ò������нڵ㶼����ʱ�� 
	 * ��������Ч���� ACKJOB �����Ч��һ���� ������Ϊ���������������Ϣ������
	 * ACKJOB Ҫ�٣� ���������ٶȱ� ACKJOB Ҫ�첻�٣����ǵ���Ⱥ�а�����ʧЧ�ڵ��
	 * ʱ�� FASTACK ����� ACKJOB ��������׳��ֶ�η���ͬһ��Ϣ�����
	 * @param job_ids {const std::vector<acl::string>&} the jobs' IDs
	 *  ���� ID ����
	 * @return {int} return the number of IDs been ACKed, -1 will be
	 *  returned if some error happened
	 *  ���ر�ȷ�ϵ������������������򷵻� -1
	 */
	int fastack(const std::vector<string>& job_ids);

	/**
	 * peek some jobs no more than the specified count from the specified
	 * queue and remain these jobs in queue.
	 * �ڲ�ȡ�����������£� �Ӷ������淵��ָ������������
	 * @param name {const char*} the specified queue
	 *  ָ���Ķ�������
	 * @param count {int} limit the max count of jobs to be got
	 *  �޶��˷��ؽ�������������
	 * @return {const std::vector<acl::disque_job*>*} return the jobs
	 *  if the queue isn't empty. NULL will be returned if the queue
	 *  is empty or some error happened.
	 *  ���ؽ�������������Ϊ�ջ�����򷵻� NULL
	 */
	const std::vector<disque_job*>* qpeek(const char* name, int count);

	/**
	 * get the number of jobs stored in the specified queue
	 * ���ָ�������е���������
	 * @param name {const char*} the specified queue
	 *  ָ���Ķ�������
	 * @return {int} return the number of the jobs in queue
	 *  ����ָ�������е��������������س����򷵻� -1
	 */
	int qlen(const char* name);

	/**
	 * get the stat information of the specified job by job id
	 * �������� ID �������������Ϣ
	 * @param job_id {const char*} the id of the job
	 *  ָ�������� ID
	 * @return {const acl::disque_job*} return the job's information,
	 *  return NULL if the job doesn't exist or some error happens.
	 *  ����ָ���������Ϣ���ο��� disque_job��������񲻴��ڻ�����򷵻� NULL
	 */
	const disque_job* show(const char* job_id);

	/**
	 * queue jobs if not already queued
	 * �����������δ�����뵽����� ������Ƿ��뵽������
	 * @param job_ids {const std::vector<acl::string>&} the job IDs
	 *  ָ�������� ID ����
	 * @return {int} return the number of jobs been queued, -1 will be
	 *  returned if some error happens.
	 *  ���ر�����������������������������򷵻� -1
	 */
	int enqueue(const std::vector<string>& job_ids);

	/**
	 * remove the jobs from the queue
	 * �Ӷ��������Ƴ�ָ��������
	 * @param job_ids {const std::vector<acl::string>&} the job IDs
	 *  ׼���Ƴ������� ID ����
	 * @return {int} return the number of jobs been removed, -1 will be
	 *  returned if some error happens.
	 *  ���ر��Ƴ���������������������򷵻� -1
	 */
	int dequeue(const std::vector<string>& job_ids);

	/**
	 * completely delete a job from a node.
	 * �ڽڵ����泹�׵�ɾ������������ �������� FASTACK �����ƣ� Ψһ�Ĳ�ͬ�ǣ�
	 * DELJOB ����������ɾ������ֻ���ڵ����ڵ�����ִ�У� �����Ὣ DELJOB ��Ⱥ����
	 * ��Ϣ��cluster bus message�������������ڵ�
	 * @param job_ids {const std::vector<acl::string>&} the job IDs
	 * ��ɾ�������� ID ����
	 * @return {int} return the number of jobs been deleted, -1 will be
	 *  returned if some error happens.
	 *  ���ر�ɾ����������������������򷵻� -1
	 */
	int deljob(const std::vector<string>& job_ids);

	/**
	 * display the information of the disque cluster
	 * ��õ�ǰ��Ⱥ��״̬��Ϣ
	 * @param out {std::map<acl::string, acl::string>&} store result
	 *  �洢���
	 * @return {bool} if the operation is successful
	 *  �����Ƿ���������������򷵻� false
	 */
	bool info(std::map<string, string>& out);

	/**
	 * get the information of all the nodes in the cluster
	 * ��ü�Ⱥ�����н�����Ϣ
	 * @return {const std::vector<acl::disque_node*>*} all the nodes'
	 *  information in the cluster, return NULL if some error happened.
	 *  ���ؼ�Ⱥ�����н����Ϣ�Ľ��������������򷵻� NULL���ο��� disque_node
	 */
	const std::vector<disque_node*>* hello();

private:
	int jobs_bat(const std::vector<string>& job_ids, const char* cmd);

private:
	disque_job* job_;
	std::vector<disque_job*> jobs_;

	const std::vector<disque_job*>* get_jobs(const char* name);
	void free_jobs();

private:
	int version_;
	string myid_;
	std::vector<disque_node*> nodes_;

	disque_node* create_node(const redis_result* rr);
	void free_nodes();
};

} // namespace acl
