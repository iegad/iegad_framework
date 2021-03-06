# !/bin/sh



# ==============================================
# @用途:
#       为iptables常用操作提供交互模式
# 
# @作者: iegad
# @时间: 2019-05-05
# ==============================================
# @修改记录:
#  日期                   修改人                    修改说明
# ==============================================



GREEN_LINE='\033[32m'
RED_LINE='\033[31m'
YELLOW_LINE='\033[33m'
NONE_LINE='\033[0m'



# =============================
# @释放会话级变量
# =============================
_release_var() {
    unset GREEN_LINE
    unset RED_LINE
    unset YELLOW_LINE
    unset NONE_LINE
}



# =============================
# @恢复CentOS6.x默认配置
# =============================
_set_default() {
    local fileName='/etc/sysconfig/iptables'

    echo '# Firewall configuration written by system-config-firewall' > $fileName
    echo '# Manual customization of this file is not recommended.' >> $fileName
    echo '*filter' >> $fileName
    echo ':INPUT ACCEPT [0:0]' >> $fileName
    echo ':FORWARD ACCEPT [0:0]' >> $fileName
    echo ':OUTPUT ACCEPT [0:0]' >> $fileName
    echo '-A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT' >> $fileName
    echo '-A INPUT -p icmp -j ACCEPT' >> $fileName
    echo '-A INPUT -i lo -j ACCEPT' >> $fileName
    echo '-A INPUT -m state --state NEW -m tcp -p tcp --dport 22 -j ACCEPT' >> $fileName
    echo '-A INPUT -j REJECT --reject-with icmp-host-prohibited' >> $fileName
    echo '-A FORWARD -j REJECT --reject-with icmp-host-prohibited' >> $fileName
    echo 'COMMIT' >> $fileName

    `service iptables restart`

    if [ $? != 0 ]; then
        printf "${RED_LINE}service iptables restart 执行失败${NONE_LINE}\n"
        return 1
    fi

    printf "${GREEN_LINE}恢复默认设置成功.${NONE_LINE}\n"
    exit 0
}



# =============================
# @删除NAT转发
# =============================
_del_forward() {
    service iptables status

    local num=''
    printf "${GREEN_LINE}请选择要要删除的序号: ${NONE_LINE}"
    read num

    `iptables -D FORWARD ${num}`

    if [ $? != 0 ]; then
        printf "${RED_LINE}iptables -D FORWARD ${num} 执行失败${NONE_LINE}\n"
        return 1
    fi

    printf "${GREEN_LINE}FORWARD删除成功${NONE_LINE}\n"
    return 0
}



# =============================
# @禁止外部机器ping本机
# =============================
_ping_off() {
    `iptables -I INPUT -p icmp --icmp-type 8 -s 0/0 -j DROP`
    if [ $? != 0 ]; then
        printf "${RED_LINE}iptables -I INPUT -p icmp --icmp-type 8 -s 0/0 -j DROP 执行失败${NONE_LINE}\n"
        return 1
    fi

    return 0
}



# =============================
# @开启外部机器ping本机
# =============================
_ping_on() {
    `iptables -D INPUT -p icmp --icmp-type 8 -s 0/0 -j DROP`
    if [ $? != 0 ]; then
        printf "${RED_LINE}iptables -D INPUT -p icmp --icmp-type 8 -s 0/0 -j DROP 执行失败${NONE_LINE}\n"
        return 1
    fi

    return 0
}



# =============================
# @设置NAT转发
# =============================
_set_forward() {
    local srcHost=''
    local srcPort=''
    local dstHost=''
    local dstPort=''
    local protocol=''
    printf "${GREEN_LINE}配置转发需要修改linux内核参数: /etc/sysctl.conf 中的net.ipv4.ip_forward = 1${NONE_LINE}\n"
    printf "${GREEN_LINE}请输入源ip地址[必填]: ${NONE_LINE}"
    read srcHost

    if [ -z $srcHost ]; then
        printf "${RED_LINE}源ip地址无效${NONE_LINE}\n"
        return 1
    fi

    printf "${GREEN_LINE}请输入源端口(或范围)[必填]: ${NONE_LINE}"
    read srcPort

    if [ -z $srcPort ]; then
        printf "${RED_LINE}源端口无效${NONE_LINE}\n"
        return 1
    fi

    printf "${GREEN_LINE}请输入目标ip地址[必填]: ${NONE_LINE}"
    read dstHost

    if [ -z $dstHost ]; then
        printf "${RED_LINE}目标ip地址无效${NONE_LINE}\n"
        return 1
    fi

    printf "${GREEN_LINE}请输入目标端口(或范围)[必填]: ${NONE_LINE}"
    read dstPort

    if [ -z $dstPort ]; then
        printf "${RED_LINE}目标端口无效${NONE_LINE}\n"
        return 1
    fi

    printf "${GREEN_LINE}请输入协议类型[默认为tcp]: ${NONE_LINE}"
    read protocol

    if [ -z $protocol ]; then
        protocol='tcp'
    fi

    `iptables -t nat -I PREROUTING  -p $protocol -d $srcHost  --dport $srcPort -j DNAT --to $dstHost:${dstPort/":"/"-"}`
    `iptables -t nat -I POSTROUTING -p $protocol -d $dstHost --dport $dstPort -j SNAT --to $srcHost`

    printf "${GREEN_LINE}FORWARD设置成功${NONE_LINE}\n"

    return 0
}



# =============================
# @清空所有策略
# =============================
_cleanup() {
    iptables -F
    iptables -X
    return 0
}



# =============================
# @添加开放端口
# =============================
_add_port() {
    local port=''
    local protocol=''

    # @例:
    #   端口: "80"
    #   端口范围: "3001:3003" -- 开通3001至3003的端口
    printf "${GREEN_LINE}请输入要开发的端口(或范围)[必填]: ${NONE_LINE}"
    read port
    echo $port

    printf "${GREEN_LINE}请输入协议类型[默认为tcp]: ${NONE_LINE}"
    read protocol

    if [ -z $protocol ]; then
        protocol='tcp'
    fi

    if [ $protocol != 'tcp' ] && [ $protocol != 'udp' ]; then
        printf "${RED_LINE}协议类型无效${NONE_LINE}\n"
        return 1
    fi

    `iptables -I INPUT -p ${protocol} --dport ${port} -j ACCEPT`

    if [ $? != 0 ]; then
       printf "${RED_LINE}iptables -I INPUT -p ${protocol} --dport ${port} -j ACCEPT 执行失败${NONE_LINE}\n"
       return 1
    fi

    printf "${GREEN_LINE}端口添加成功${NONE_LINE}\n"

    return 0
}



# =============================
# @删除开放端口
# =============================
_del_port() {
    service iptables status

    local num=''
    printf "\n${GREEN_LINE}请选择要要删除的序号: ${NONE_LINE}"
    read num

    `iptables -D INPUT ${num}`

    if [ $? != 0 ]; then
        printf "${RED_LINE}iptables -D ${num} 执行失败${NONE_LINE}\n"
        return 1
    fi

    printf "${GREEN_LINE}端口删除成功${NONE_LINE}\n"
    return 0
}



# =============================
# @保存现有策略
# =============================
_save() {
    service iptables save
    if [ $? != 0 ]; then
        printf "${RED_LINE}service iptables save 执行失败${NONE_LINE}\n"
        return 1
    fi

    printf "${GREEN_LINE}配置保存成功${NONE_LINE}\n"
    return 0
}



# =============================
# @重启iptables服务
# =============================
_restart() {
    service iptables restart
    if [ $? != 0 ]; then
        printf "${RED_LINE}service iptables restart 执行失败${NONE_LINE}\n"
        return 1
    fi

    printf "${GREEN_LINE}防火墙重启成功${NONE_LINE}\n"
    return 0
}


__main__() {
    printf "${GREEN_LINE}请选择操作类型:\n"
    printf "0) 请空所有策略!!!(慎重使用);\n"
    printf "1) 添加端口;\n"
    printf "2) 删除端口;\n"
    printf "3) 恢复默认设置;\n"
    printf "4) 添加nat转发;\n"
    printf "5) 删除FORWARD --配置转发时请先删除<x REJECT all -- 0.0.0.0/0  0.0.0.0/0 reject-with icmp-host-prohibited>否则转发无法生效\n"
    printf "6) 禁用ping;\n"
    printf "7) 开启ping;\n"
    printf "${NONE_LINE}\n"

    local n=''
    read n

    case $n in
        0 )
            _cleanup
            ;;
        1 )
            _add_port
            ;;
        2 ) 
            _del_port
            ;;
        3 )
            _set_default
            ;;
        4 )
            _set_forward
            ;;
        5) 
            _del_forward
            ;;
        6)
            _ping_off
            ;;
        7)
            _ping_on
            ;;
        * )
            printf "${RED_LINE}未知的选项${NONE_LINE}\n"
            ;;
    esac
    
    if [ $? != 0 ]; then
        _release_var
        exit 1
    fi
    
    _save

    if [ $? != 0 ]; then
        _release_var
        exit 1
    fi

    _restart

    if [ $? != 0 ]; then
        _release_var
        exit 1
    fi

    printf "${GREEN_LINE}脚本完成${NONE_LINE}\n"

    _release_var
    exit 0
}



__main__
