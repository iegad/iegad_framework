#ifndef __IEGAD_TIMER__
#define __IEGAD_TIMER__


// ============ ˵�� ============
//
// @�������� : 2015-11-07
// @������ : iegad
//
// ============================
// @��; :  ��ʱ��;
//		    �ṩ���ַ�ʽ�Ĳ��� :
//			    1, ��� ���ʱ��(�Ժ���Ϊ��λ) ��ָ������
//			    2, ָ�� ����ʱ���(�Է�Ϊ��λ) ��ָ������
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
// =======================================


#include <thread>
#include <string>
#include <assert.h>
#include <ctime>


namespace iegad {
namespace tools {


    template <typename T>
    class timer {
    // ��ʱ��
    public:
	// ============================
	// @��; : ��ʱ�� ��ʱ���� ��������
	// @PS : ģ����� T Ϊ���������� ��������
	// ============================
	typedef std::function<void(T &)> func_t;


	// ============================
	// @��; : ���캯��, ������һ�� "���ʱ�䶨ʱ��"
	// @func : ��������
	// @param : ����������
	// @ms : ���ʱ��, �Ժ���Ϊ��λ
	// @n : ��������, Ĭ��Ϊ -1 ��ʾ ��������
	// ============================
	timer(func_t func, T & param, int ms, int n = -1);


	// ============================
	// @��; : ���캯��, ������һ�� "����ʱ�䶨ʱ��"
	// @func : ��������
	// @param : ����������
	// @timestr : ָ����һ�� ִ�в����� ����ʱ��
	// @ms : ���ʱ��, �Է�Ϊ��λ
	// @n : ��������, Ĭ��Ϊ -1 ��ʾ ��������
	// ============================
	timer(func_t func, T & param, const std::string & timestr, int ms_, int n = -1);
	

	// ============================
	// @��; : ���� ��ʱ��
	// @timestr : ��ʱ����ʼ�� ����ʱ��.
	// @����ֵ : void
	// @PS :  timestrĬ��Ϊ �մ�, ��ʾ����ִ�е�һ�β���, 
	//		����, ����ָ��ʱ���ִ�е�һ�β���
	// ============================
	void start(const std::string & timestr = "");	


	// ============================
	// @��; : ֹͣ ��ʱ��
	// @����ֵ : void
	// ============================
	void stop();


    private:
	// ============================
	// @��; : ��� ����ʱ�� �Ƿ���Ϲ���
	// @year : ��
	// @mon : ��
	// @day : ��
	// @hour : ʱ
	// @min : ��
	// @sec : ��
	// @����ֵ : ����ʱ����� ���� true, ���򷵻� false
	// ============================
	bool _check_datetime(int year, int mon, int day, int hour, int min, int sec);


	// ============================
	// @��; : �����ַ��� timestr ����һ�� std::chrono::time_point ����
	// @timestr : ��ʽ����ʱ���ַ��� ���� : "yyyy-MM-dd hh:mm:ss";
	// @����ֵ : ����ɹ��� ʱ��� ����
	// @PS : �������ʧ��, �ú�����ʹ ��ǰӦ�ó�����ֹ
	// ============================
	std::chrono::time_point<std::chrono::system_clock> _mk_timepoint(const std::string & timestr);

	// ֹͣ��־
	bool stop_flag_;
	// ��ʱ������. true : ����ʱ�䶨ʱ��; false : ���ʱ�䶨ʱ��
	bool is_ab_time_;
	// ִ�д���, -1 : ��������
	int n_;
	// ���ʱ��, ��Ϊ���ʱ�䶨ʱ��, �Ժ���Ϊ��λ; ��Ϊ����ʱ�䶨ʱ��, �Է�Ϊ��λ
	int ms_;
	// ��������
	func_t func_;
	// �������� ʵ�ζ���
	T & param_;
	// ���Զ�������ʹ�õ� ʱ������
	std::chrono::time_point<std::chrono::system_clock> tp_;


	// ����
	timer(const timer &);
	timer & operator=(const timer &);
    }; // class scheduler <T>;


// =========================== ������ʵ�ֲ��� =============================


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
            // ��� ���
                break;
            }

            if (mon < 1 || mon > 12) {
            // ��� �·�
                break;
            }

            switch (mon) {
            // ��� ��
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12: // ���� 31��
                if (day < 1 || day > 31) {
                    return false;
                } break;

            case 4:
            case 6:
            case 9:
            case 11: // С�� 30��
                if (day < 1 || day > 30) {
                    return false;
                } break;

            case 2: // 2��
                if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
                // ���� 29��
                    if (day < 1 || day > 29) {
                        return false;
                    }
                }
                else {
                // ƽ�� 28��
                    if (day < 1 || day > 28) {
                        return false;
                    }
                } break;
            } // switch (mon);

            if (hour < 0 || hour > 23) {
            // ��� ʱ
                break;
            }

            if (min < 0 || min > 59) {
            // ��� ��
                break;
            }

            if (sec < 0 || sec > 59) {
            // ��� ��
                break;
            }

            return true;
        } while (false);
        return false;
    }


} // namespace tools;
} // namespace iegad;



#endif // __IEGAD_TIMER__
