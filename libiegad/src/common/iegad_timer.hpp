#ifndef __IEGAD_TIMER__
#define __IEGAD_TIMER__


// ============ 说明 ============
//
// @创建日期 : 2015-11-07
// @创建人 : iegad
//
// ============================
// @用途 :  定时器;
//		    提供两种方式的操作 :
//			    1, 间隔 相对时间(以毫秒为单位) 作指定操作
//			    2, 指定 绝对时间点(以分为单位) 作指定操作
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================


#include <thread>
#include <string>
#include <assert.h>
#include <ctime>


namespace iegad {
namespace tools {


    template <typename T>
    class timer {
    // 定时器
    public:
	// ============================
	// @用途 : 定时器 定时操作 类型声明
	// @PS : 模板参数 T 为操作函数的 参数类型
	// ============================
	typedef std::function<void(T &)> func_t;


	// ============================
	// @用途 : 构造函数, 将构造一个 "相对时间定时器"
	// @func : 操作函数
	// @param : 操作函数的
	// @ms : 间隔时间, 以毫秒为单位
	// @n : 操作次数, 默认为 -1 表示 次数不限
	// ============================
	timer(func_t func, T & param, int ms, int n = -1);


	// ============================
	// @用途 : 构造函数, 将构造一个 "绝对时间定时器"
	// @func : 操作函数
	// @param : 操作函数的
	// @timestr : 指定第一次 执行操作的 绝对时间
	// @ms : 间隔时间, 以分为单位
	// @n : 操作次数, 默认为 -1 表示 次数不限
	// ============================
	timer(func_t func, T & param, const std::string & timestr, int ms_, int n = -1);
	

	// ============================
	// @用途 : 开启 定时器
	// @timestr : 定时器开始的 绝对时间.
	// @返回值 : void
	// @PS :  timestr默认为 空串, 表示立刻执行第一次操作, 
	//		否则, 将到指定时间才执行第一次操作
	// ============================
	void start(const std::string & timestr = "");	


	// ============================
	// @用途 : 停止 定时器
	// @返回值 : void
	// ============================
	void stop();


    private:
	// ============================
	// @用途 : 检查 日期时间 是否符合规则
	// @year : 年
	// @mon : 月
	// @day : 日
	// @hour : 时
	// @min : 分
	// @sec : 秒
	// @返回值 : 符合时间规则 返回 true, 否则返回 false
	// ============================
	bool _check_datetime(int year, int mon, int day, int hour, int min, int sec);


	// ============================
	// @用途 : 根据字符串 timestr 构造一个 std::chrono::time_point 对象
	// @timestr : 格式化的时间字符串 规则 : "yyyy-MM-dd hh:mm:ss";
	// @返回值 : 构造成功的 时间点 对象
	// @PS : 如果构造失败, 该函数会使 当前应用程序终止
	// ============================
	std::chrono::time_point<std::chrono::system_clock> _mk_timepoint(const std::string & timestr);

	// 停止标志
	bool stop_flag_;
	// 定时间类型. true : 绝对时间定时器; false : 相对时间定时器
	bool is_ab_time_;
	// 执行次数, -1 : 次数不限
	int n_;
	// 间隔时间, 当为相对时间定时器, 以毫秒为单位; 当为绝对时间定时器, 以分为单位
	int ms_;
	// 操作函数
	func_t func_;
	// 操作函数 实参对象
	T & param_;
	// 绝对定间器所使用的 时间点对象
	std::chrono::time_point<std::chrono::system_clock> tp_;


	// 禁用
	timer(const timer &);
	timer & operator=(const timer &);
    }; // class scheduler <T>;


// =========================== 以下是实现部分 =============================


    template <typename T>
    void iegad::tools::timer<T>::stop()
    {
        stop_flag_ = true;
    }


    template <typename T>
    void 
    iegad::tools::timer<T>::start(const std::string & timestr /*= ""*/)
    {
        if (timestr != "") {
            auto tp = this->_mk_timepoint(timestr);
            std::this_thread::sleep_until(tp);
        }

        int i = 0;
        while (true) {
            if ((n_ != -1 && i >= n_) || stop_flag_) {
                break;
            }

            if (is_ab_time_) {
                std::this_thread::sleep_until(tp_);
                tp_ += std::chrono::minutes(ms_);
            }
            else {
                std::this_thread::sleep_for(std::chrono::milliseconds(ms_));
            }
            func_(param_);
            i++;
        }
    }


    template <typename T>
    iegad::tools::timer<T>::timer(func_t func, T & param, int ms, int n /*= -1*/)
	: stop_flag_(false), is_ab_time_(false), n_(n), ms_(ms), func_(func), param_(param)
    {}


    template <typename T>
    iegad::tools::timer<T>::timer(func_t func, T & param, const std::string & timestr, int ms, int n /*= -1*/)
	: stop_flag_(false), is_ab_time_(true), n_(n), ms_(ms), func_(func), param_(param)
    {
        tp_ = this->_mk_timepoint(timestr);
    }


    template <typename T>
    std::chrono::time_point<std::chrono::system_clock>
    iegad::tools::timer<T>::_mk_timepoint(const std::string & timestr)
    {
        std::tm abtm;
        memset(&abtm, 0, sizeof(abtm));
        int year, mon, day, hour, min, sec;
        sscanf(timestr.c_str(), "%04d-%02d-%02d %02d:%02d:%02d", &year, &mon, &day, &hour, &min, &sec);
        assert(this->_check_datetime(year, mon, day, hour, min, sec));
        abtm.tm_year = year - 1900;
        abtm.tm_mon = mon - 1;
        abtm.tm_mday = day;
        abtm.tm_hour = hour;
        abtm.tm_min = min;
        abtm.tm_sec = sec;
        return std::chrono::system_clock::from_time_t(mktime(&abtm));;
    }


    template <typename T>
    bool 
    iegad::tools::timer<T>::_check_datetime(int year, int mon, int day, int hour, int min, int sec)
    {
        do
        {
            if (year < 1900 || year > 2400) {
            // 检查 年份
                break;
            }

            if (mon < 1 || mon > 12) {
            // 检查 月份
                break;
            }

            switch (mon) {
            // 检查 天
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12: // 大月 31天
                if (day < 1 || day > 31) {
                    return false;
                } break;

            case 4:
            case 6:
            case 9:
            case 11: // 小月 30天
                if (day < 1 || day > 30) {
                    return false;
                } break;

            case 2: // 2月
                if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
                // 闰年 29天
                    if (day < 1 || day > 29) {
                        return false;
                    }
                }
                else {
                // 平年 28天
                    if (day < 1 || day > 28) {
                        return false;
                    }
                } break;
            } // switch (mon);

            if (hour < 0 || hour > 23) {
            // 检查 时
                break;
            }

            if (min < 0 || min > 59) {
            // 检查 分
                break;
            }

            if (sec < 0 || sec > 59) {
            // 检查 秒
                break;
            }

            return true;
        } while (false);
        return false;
    }


} // namespace tools;
} // namespace iegad;



#endif // __IEGAD_TIMER__
