#pragma once
#include "acl_cpp/acl_cpp_define.hpp"
#include <vector>
#include <map>
#include "acl_cpp/stdlib/string.hpp"
#include "acl_cpp/redis/redis_command.hpp"

namespace acl
{

#define GEO_INVALID		360
#define GEO_LONGITUDE_MIN	-180
#define GEO_LONGITUDE_MAX	180
#define GEO_LATITUDE_MIN	-85.05112878
#define GEO_LATITUDE_MAX	85.05112878

enum
{
	GEO_UNIT_FT,
	GEO_UNIT_M,
	GEO_UNIT_MI,
	GEO_UNIT_KM,
};

enum
{
	GEO_WITH_COORD = 1 << 0,
	GEO_WITH_DIST  = 1 << 1,
	GEO_WITH_HASH  = 1 << 2,
};

enum
{
	GEO_SORT_NONE,
	GEO_SORT_ASC,
	GEO_SORT_DESC,
};

class ACL_CPP_API geo_member
{
public:
	geo_member(const char* name);
	geo_member(const geo_member& member);
	~geo_member(void);

	void set_name(const char* name);
	const char* get_name() const
	{
		return name_.c_str();
	}

	void set_dist(double dist);
	double get_dist() const
	{
		return dist_;
	}

#if defined(_WIN32) || defined(_WIN64)
	void set_hash(__int64 hash);
	__int64 get_hash() const
#else
	void set_hash(long long int hash);
	long long int get_hash() const
#endif // defined(_WIN32) || defined(_WIN64)
	{
		return hash_;
	}

	void set_coordinate(double longitude, double latitude);
	double get_longitude() const
	{
		return longitude_;
	}

	double get_latitude() const
	{
		return latitude_;
	}

private:
	string name_;
	double dist_;
#if defined(_WIN32) || defined(_WIN64)
	__int64 hash_;
#else
	long long int hash_;
#endif // defined(_WIN32) || defined(_WIN64)

	double longitude_;
	double latitude_;
};

class redis_client;

class ACL_CPP_API redis_geo : virtual public redis_command
{
public:
	/**
	 * see redis_command::redis_command()
	 */
	redis_geo();

	/**
	 * see redis_command::redis_command(redis_client*)
	 */
	redis_geo(redis_client* conn);

	/**
	 * see redis_command::redis_command(redis_client_cluster*�� size_t)
	 */
	redis_geo(redis_client_cluster* cluster, size_t max_conns = 0);
	virtual ~redis_geo();

	/////////////////////////////////////////////////////////////////////

	/**
	 * ���һ��ָ���ĵ���λ��������ָ���� key ��
	 * Add the specified geospatial item (latitude, logitude, name)
	 * to the specified key.
	 * @param key {const char*} ��Ӧ�ļ�ֵ
	 *  the specified key
	 * @param member {const char*} �õ�������ı�ʶ��
	 *  the geospatial's identifier
	 * @param loginitude {double} ����
	 *  the geospatial's loginitude
	 * @param latitude {double} γ��
	 *  the geospatial's latitude
	 * @return {int} 1����ӳɹ���0���õ��������ʶ���Ѵ��ڣ���ʹ����ֵ�������޸ģ�
	 *  Ҳ������ 0��-1����ʾ����
	 *  the return value as below:
	 *  1: add one new member successfully
	 *  0: the member already existed, and the geospatial may be changed
	 * -1: some erro happened
	 */
	int geoadd(const char* key, const char* member,
		double longitude, double latitude);

	/**
	 * ��ָ�� key ���һ���ַλ����������
	 * Add the specified geospatial items (latitude, logitude, name)
	 * to the specified key.
	 * @param key {const char*} ��Ӧ�ļ�ֵ
	 *  the specified key
	 * @param size {size_t} ����ĳ���
	 *  the array's size
	 * @param memebers {const char* []} ��Ա���飬�䳤���� size ָ��
	 *  the members array, which's length was specified by size parameter
	 * @param longitudes {const double[]} �����������飬�䳤���� size ָ��
	 *  the logintitudes array, which's length was specifed by size parameter
	 * @param latitudes {const double[]} γ���������飬�䳤���� size ָ��
	 *  the lattitudes array, which's length was specifed by size parameter
	 * @return {int} ��ӳɹ��ĳ�Ա����������ֵ�������£�
	 *  return the successfully added members's count:
	 *  > 0: ��ʾ�ɹ���ӵĳ�Ա������
	 *       represent the successfully added memebers's count
	 *    0: ��Щ��Ա���Ѿ�����
	 *       the members's belong the key already existing
	 *   -1: ��ʾ��������ͨ�� result_error �����鿴����ԭ��
	 *       some error happened, the result_error function can be used
	 *       to find the error's reason
	 */
	int geoadd(const char* key, size_t size, const char* memebers[],
		const double longitudes[], const double latitudes[]);

	/**
	 * ��ָ�� key ���һ���ַλ����������
	 * Add the specified geospatial items (latitude, logitude, name)
	 * to the specified key.
	 * @param key {const char*} ��Ӧ�ļ�ֵ
	 *  the specified key
	 * @param memebers {const std::vector<string>&} ��Ա����
	 *  the members array
	 * @param longitudes {const std::vector<double>&} ������������
	 *  the logintitudes array
	 * @param latitudes {const std::vector<double>&} γ����������
	 *  the lattitudes array
	 * @return {int} ��ӳɹ��ĳ�Ա����������ֵ�������£�
	 *  return the successfully added members's count:
	 *  > 0: ��ʾ�ɹ���ӵĳ�Ա������
	 *       represent the successfully added memebers's count
	 *    0: ��Щ��Ա���Ѿ�����
	 *       the members's belong the key already existing
	 *   -1: ��ʾ��������ͨ�� result_error �����鿴����ԭ��
	 *       some error happened, the result_error function can be used
	 *       to find the error's reason
	 *  ע�⣺��������(members, longitudes, latitudes)�����鳤�ȱ������
	 *  Notice: the three array's length must be equal between members,
	 *    longitudes and latitudes
	 */
	int geoadd(const char* key, const std::vector<string>& members,
		const std::vector<double>& longitudes,
		const std::vector<double>& latitudes);

	/**
	 * ���ַ�����ʽ����ָ����Ա�� GEOHASH ֵ
	 * Returns members of a geospatial index as standard geohash strings.
	 * @param key {const char*} ��Ӧ�ļ�ֵ
	 *  the specified key
	 * @param memebers {const std::vector<string>&} ��Ա����
	 *  the members array
	 * @param results {std::vector<string>&} �洢�������
	 *  store the result
	 * @return {bool} �����Ƿ�ɹ�
	 *  if the operation was successful.
	 */
	bool geohash(const char* key, const std::vector<string>& members,
		std::vector<string>& results);

	/**
	 * ���ַ�����ʽ����ָ����Ա�� GEOHASH ֵ
	 * Returns members of a geospatial index as standard geohash strings.
	 * @param key {const char*} ��Ӧ�ļ�ֵ
	 *  the specified key
	 * @param memeber {const char*} ��Ա��
	 *  the member of a geospatial index
	 * @param result {std::vector<string>&} �洢���
	 *  store the result
	 * @return {bool} �����Ƿ�ɹ�
	 *  if the operation was successful.
	 */
	bool geohash(const char* key, const char* member, string& result);

	/**
	 * ���ָ����Ա�ĵ���λ������
	 * Returns longitude and latitude of members of a geospatial index
	 * @param key {const char*} ��Ӧ�ļ�ֵ
	 *  the specified key
	 * @param memebers {const std::vector<string>&} ��Ա����
	 *  the members array
	 * @param results {std::vector<std::pair<double, double> >&} �洢�����
	 *  store the results
	 * @return {bool} �����Ƿ�ɹ�
	 *  if the operation was successful. 
	 */
	bool geopos(const char* key, const std::vector<string>& members,
		std::vector<std::pair<double, double> >& results);

	/**
	 * ���ĳ��ָ����Ա�ĵ���λ������
	 * Returns longitude and latitude of the one member of
	 * a geospatial index
	 * @param key {const char*} ָ����ֵ
	 *  the specifed key
	 * @param member {const char*} ָ����Ա��
	 *  the specified member
	 * @param result {std::pair<double, double>&} �洢�������
	 *  store the result of longitude and latitude of the member
	 * @return {bool} �����Ƿ�ɹ�
	 *  if the operation was successful.
	 */
	bool geopos(const char* key, const char* member,
		std::pair<double, double>& result);

	/**
	 * �����������λ������֮��ľ���
	 * Returns the distance between two members of a geospatial index
	 * @param key {const char*} ��Ӧ�ļ�ֵ
	 *  the specified key
	 * @param member1 {const char*} ���������Ա
	 *  one member of a geospatial index
	 * @param member2 {const char*} ���������Ա
	 *  another member of a geospatial index
	 * @param unit {int} ���صľ���ĵ�λֵ
	 * @return {double} ��������֮��ĳ��ȣ�����ֵ < 0 ��ʾ����
	 *  returns the distance between two members, which was less than 0
	 *  if some error happened.
	 */
	double geodist(const char* key, const char* member1,
		const char* member2, int unit = GEO_UNIT_M);

	/**
	 * ��þ���ĳָ������λ���ڸ������뷶Χ�ڵ����������
	 * Query a sorted set representing a geospatial index to fetch
	 * members matching a given maximum distance from a point
	 * @param key {const char*} ��Ӧ�ļ�ֵ
	 *  the specified key
	 * @param longitude {double} ָ�������ľ���ֵ
	 *  the longitude of the specified geospatial coordinate
	 * @param latitude {double} ָ��������γ��ֵ
	 *  the latitude of the specified geospatial coordinate
	 * @param radius {double} �޶��ľ��뷶Χ��С
	 *  the distance from the specified coordinate
	 * @param unit {int} radius ����ĵ�λ����
	 *  the unit type of the raidus
	 * @param with {int} ��ѯ����ѡ��μ�����Ķ��壺GEO_WITH_XXX
	 *  the serach operations, defined as GEO_WITH_XXX above
	 * @param sort {int} ��ѯ���������ʽ������μ���GEO_SORT_XXX
	 *  the sorted type of the results, defined as GEO_SORT_XXX above
	 * @return {const std::vector<geo_member>&} ���������������Ľ����
	 *  Returns the results according the searching conditions.
	 */
	const std::vector<geo_member>& georadius(const char* key,
		double longitude, double latitude, double radius,
		int unit = GEO_UNIT_M,
		int with = GEO_WITH_COORD | GEO_WITH_DIST,
		int sort = GEO_SORT_ASC);

	/**
	 * ��þ���ĳָ������λ���ڸ������뷶Χ�ڵ����������
	 * Query a sorted set representing a geospatial index to fetch
	 * members matching a given maximum distance from a member
	 * @param key {const char*} ��Ӧ�ļ�ֵ
	 *  the specified key
	 * @param member {const char*} ĳ��ָ����������Ա
	 *  the specified member of a geospatial index
	 * @param radius {double} �޶��ľ��뷶Χ��С
	 *  the distance from the specified coordinate
	 * @param unit {int} radius ����ĵ�λ����
	 *  the unit type of the raidus
	 * @param with {int} ��ѯ����ѡ��μ�����Ķ��壺GEO_WITH_XXX
	 *  the serach operations, defined as GEO_WITH_XXX above
	 * @param sort {int} ��ѯ���������ʽ������μ���GEO_SORT_XXX
	 *  the sorted type of the results, defined as GEO_SORT_XXX above
	 * @return {const std::vector<geo_member>&} ���������������Ľ����
	 *  Returns the results according the searching conditions.
	 */
	const std::vector<geo_member>& georadiusbymember(const char* key,
		const char* member, double radius,
		int unit = GEO_UNIT_M,
		int with = GEO_WITH_COORD | GEO_WITH_DIST,
		int sort = GEO_SORT_ASC);

private:
	std::vector<geo_member> positions_;

	void add_one_pos(const redis_result& rr);
	static const char* get_unit(int unit);
};

} // namespace acl
