#ifndef __IEGAD_NET_HPP__
#define __IEGAD_NET_HPP__


// ============ 说明 ============
//
// @创建日期 : 2022-02-11
// @创建人 : iegad
//
// ============================
// @用途: 跨平台网络工具函数
// ============================


#include <string>
#include <memory.h>
#include <assert.h>
#include <event2/event.h>
#include <iostream>


namespace iegad {
namespace net {


extern "C" {


  // -----------------------
  // init 网络初始化, 非windows下总是返回0
  // @return 成功返回0, 否则返回-1
  int
  init()
  {
#ifdef WIN32
    WSAData wdata;
    if (WSAStartup(0x0202, &wdata))
      return -1;
#endif // WIN32

    return 0;
  }

  // -----------------------
  // is_little_endian 当前平台是否为小端序
  // @return 当前平台为小端序时返回true, 否则返回false
  bool
  is_little_endian()
  {
    static const uint16_t v = 0x0102;
    static char* p = (char*)&v;
    static bool ret = p[0] == 0x02;
    return ret;
  }
  
  // -----------------------
  // is_big_endian 当前平台是否为大端序
  // @return 当前平台为大端序时返回true, 否则返回false
  bool
  is_big_endian()
  {
    return !is_little_endian();
  }


  // -----------------------
  // to_big_endian 将当前平台字节序转为大端序
  uint32_t
  to_big_endian(uint32_t v)
  {
    static bool lit = is_little_endian();
    return lit ? htonl(v) : v;
  }


  // -----------------------
  // from_big_endian 将大端序转换为当前平台字节序
  uint32_t
   from_big_endian(uint32_t v)
  {
    static bool lit = is_little_endian();
    return lit ? htonl(v) : v;
  }


  // -----------------------
  // to_lit_endian 将当前平台字节序转为小端序
  uint32_t
  to_little_endian(uint32_t v)
  {
    static bool lit = is_little_endian();
    return lit ? v : htonl(v);
  }
  
  
  // -----------------------
  // from_little_endian 将小端序转换为当前平台字节序
  uint32_t
  from_little_endian(uint32_t v)
  {
    static bool lit = is_little_endian();
    return lit ? v : htonl(v);
  }

  
  // -----------------------
  // head_key 获取消息头的KEY值
  uint32_t
  head_key()
  {
    static uint32_t key = to_lit_endian(0xFAFBFCFD);
    return key;
  }


  // -----------------------
  // bind_listen 监听TCP地址host:svc
  // @return: 成功返回监听套接字 大于 0的值, 否则返回 -1
  evutil_socket_t
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


  int
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
} // extern "C"


} // namespace net
} // namespace iegad

#endif // __IEGAD_NET_HPP__
