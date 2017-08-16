#ifndef __IEGAD_EMAIL__
#define __IEGAD_EMAIL__


#include <curl/curl.h>
#include <vector>
#include <string>
#include <assert.h>
#include <ctime>


namespace iegad {
namespace net {



class email {
public:
    static size_t
    payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
    {
        uint32_t *upload_line = (uint32_t *)userp;
        const char *data;

        if ((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
            return 0;
        }

        if ((*upload_line) < contents().size())
        {
            data = contents()[*upload_line].c_str();

            if (data) {
                size_t len = strlen(data);
                memcpy(ptr, data, len);
                (*upload_line)++;

                return len;
            }
        }

        return 0;
    }

    static std::vector<std::string> &
    contents()
    {
        static std::vector<std::string> s;
        return s;
    }


    email(const std::string &from,
          const std::string &user,
          const std::string &pass,
          const std::string &smtp_host) :
        from_(from),
        user_(user),
        pass_(pass),
        smtp_host_(smtp_host)
    {
        assert(!from_.empty());
        assert(!user_.empty());
        assert(!pass_.empty());
        assert(!smtp_host_.empty());
    }


    void
    addTo(const std::string &to)
    {
        if (!to.empty()) {
            tos_.push_back(to);
        }
    }


    void
    addCc(const std::string &cc)
    {
        if (!cc.empty()) {
            ccs_.push_back(cc);
        }
    }


    void
    setSubject(const std::string &subject)
    {
        subject_ = subject;
    }


    void
    setBody(const std::string &body)
    {
        body_ = body;
    }


    bool
    send()
    {
        CURL *curl = NULL;
        curl_slist *recipients = NULL;
        CURLcode res = CURLE_OK;
        uint32_t upload_line = 0;

        do {
            if (!_buildEmail()) {
                break;
            }

            curl = curl_easy_init();
            if (!curl) {
                break;
            }

            curl_easy_setopt(curl, CURLOPT_USERNAME, user_.c_str());
            curl_easy_setopt(curl, CURLOPT_PASSWORD, pass_.c_str());
            curl_easy_setopt(curl, CURLOPT_URL, smtp_host_.c_str());
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

            curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from_.c_str());

            for (size_t i = 0, n = tos_.size(); i < n; i++) {
                if (!tos_[i].empty()) {
                    recipients = curl_slist_append(recipients, tos_[i].c_str());
                }
            }

            for (size_t i = 0, n = ccs_.size(); i < n; i++) {
                if (!ccs_[i].empty()) {
                    recipients = curl_slist_append(recipients, ccs_[i].c_str());
                }
            }

            curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

            curl_easy_setopt(curl, CURLOPT_READFUNCTION, &email::payload_source);
            curl_easy_setopt(curl, CURLOPT_READDATA, &upload_line);
            curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

            res = curl_easy_perform(curl);
        } while(false);

        if (recipients) {
            curl_slist_free_all(recipients);
        }

        if (curl) {
            curl_easy_cleanup(curl);
        }

        return res == CURLE_OK;
    }


private:
    bool
    _buildEmail()
    {
        contents().clear();
        do {
            if (tos_.empty()) {
                break;
            }

            std::string to = "To: ";
            std::string cc = "Cc: ";
            std::string from = "From: " + from_ + "\r\n";
            std::string subject = "Subject: " + subject_ + "\r\n";
            std::string timestr;
            char buffer[32] = {0};

            std::time_t t = time(NULL);
            std::tm *tnow = std::localtime(&t);
            strftime(buffer, sizeof(buffer), "%Y/%m/%d %I:%M:%S +0800\r\n", tnow);
            timestr = buffer;

            for (size_t i = 0, n = tos_.size(); i < n; i++) {
                if (!tos_[i].empty()) {
                    to.append(tos_[i]);
                    if (i + 1 < n) {
                        to.append(", ");
                    }
                }
            }
            to.append("\r\n");

            for (size_t i = 0, n = ccs_.size(); i < n; i++) {
                if (!ccs_[i].empty()) {
                    cc.append(ccs_[i]);
                    if (i + 1 < n) {
                        cc.append(", ");
                    }
                }
            }
            cc.append("\r\n");

            contents().push_back(to);
            contents().push_back(from);
            contents().push_back(cc);
            contents().push_back(subject);
            contents().push_back(timestr);
            //contents().push_back("User-Agent: DXF Viewer agent\r\n");
            contents().push_back("MIME-Version: 1.0\r\n");
            contents().push_back("Content-Type: multipart/mixed;\r\n");
            contents().push_back("Content-Type: text/plain; charset=utf-8; format=flowed\r\n");
            contents().push_back("\r\n");
            contents().push_back(body_);

            return true;
        } while(false);
        return false;
    }


    std::string from_;
    std::string user_;
    std::string pass_;
    std::string smtp_host_;
    std::string subject_;
    std::string body_;
    std::vector<std::string> ccs_;
    std::vector<std::string> tos_;
}; // class email;


} // namespace net;
} // namespace iegad;


#endif // __IEGAD_EMAIL__
