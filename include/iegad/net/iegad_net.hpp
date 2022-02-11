#ifndef __IEGAD_NET_HPP__
#define __IEGAD_NET_HPP__


#include <string>
#include <memory.h>
#include <assert.h>
#include <event2/event.h>
#include <iostream>


namespace iegad {
namespace net {


  inline int
  init()
  {
#ifdef WIN32
    WSAData wdata;
    if (WSAStartup(0x0202, &wdata))
      return -1;
#endif // WIN32

    return 0;
  }


  inline bool
  is_little_endian()
  {
    static const uint16_t v = 0x0102;
    static char* p = (char*)&v;
    static bool ret = p[0] == 0x02;
    return ret;
  }
  

  inline bool
  is_big_endian()
  {
    return !is_little_endian();
  }


  inline uint32_t
  to_big_endian(uint32_t v)
  {
    static bool lit = is_little_endian();
    return lit ? htonl(v) : v;
  }


  inline uint32_t
   from_big_endian(uint32_t v)
  {
    static bool lit = is_little_endian();
    return lit ? htonl(v) : v;
  }


  inline uint32_t
  to_little_endian(uint32_t v)
  {
    static bool lit = is_little_endian();
    return lit ? v : htonl(v);
  }
  
  
  inline uint32_t
  from_little_endian(uint32_t v)
  {
    static bool lit = is_little_endian();
    return lit ? v : htonl(v);
  }

  
  inline uint32_t
  head_key()
  {
    static uint32_t key = to_little_endian(0xFAFBFCFD);
    return key;
  }


  inline evutil_socket_t
  bind_listen(const char* host, const char* svc)
  {
    addrinfo hints;
    addrinfo* result = nullptr, * rp = nullptr;
    evutil_socket_t fd;

    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    assert(!getaddrinfo(host, svc, &hints, &result));

    for (rp = result; rp != nullptr; rp = rp->ai_next) {
      fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
      if (fd < 0)
        continue;

      assert(!evutil_make_listen_socket_reuseable(fd));
      assert(!evutil_make_listen_socket_reuseable_port(fd));
      assert(!evutil_make_socket_nonblocking(fd));

      if (!bind(fd, rp->ai_addr, (int)rp->ai_addrlen))
        break;

      evutil_closesocket(fd);
    }

    assert(rp);
    freeaddrinfo(result);

    assert(fd > 0);
    assert(!listen(fd, SOMAXCONN));

    return fd;
  }


  inline int
  writen(evutil_socket_t fd, const std::string& data)
  {
    uint32_t dlen = static_cast<uint32_t>(data.size());

    dlen = ~dlen ^ head_key();
    dlen = to_big_endian(dlen);

    std::string buf((char*)&dlen, sizeof(dlen));
    buf.append(data);

    int nleft = static_cast<int>(buf.size()), n;
    const char* p = buf.c_str();

    while (nleft > 0) {
      n = send(fd, p, nleft, 0);
      if (n <= 0) {
        return n;
      }

      p += n;
      nleft -= n;
    }

    return 1;
  }


} // namespace net
} // namespace iegad

#endif // __IEGAD_NET_HPP__
