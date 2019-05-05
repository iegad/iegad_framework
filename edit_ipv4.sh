# !/bin/bash


GREEN_LINE='\033[32m'
RED_LINE='\033[31m'
YELLOW_LINE='\033[33m'
NONE_LINE='\033[0m'


ethx_=''
netmask_=''
gateway_=''
dns1_=''
dns2_=''
broadcast_=''
ipv4addr_=''



_release_var() {
    unset GREEN_LINE
    unset RED_LINE
    unset YELLOW_LINE
    unset NONE_LINE

    unset ethx_
    unset netmask_
    unset gateway_
    unset dns1_
    unset dns2_
    unset broadcast_
    unset ipv4addr_
}



_get_netmask() {
    printf "${GREEN_LINE}请输入子网掩码[必填]: ${NONE_LINE}"
    read netmask_

    if [ -z $netmask_ ]; then
        printf "${RED_LINE}子网掩码无效${NONE_LINE}"
        return 1
    fi

    return 0;
}


_get_gateway() {
    printf "${GREEN_LINE}请输入网关[必填]: ${NONE_LINE}"
    read gateway_

    if [ -z $gateway_ ]; then
        printf "${RED_LINE}网关无效${NONE_LINE}"
        return 1
    fi

    return 0;
}



_get_dns1() {
    printf "${GREEN_LINE}请输入DNS1[选填]: ${NONE_LINE}"
    read dns1_

    return 0;
}


_get_dns2() {
    printf "${GREEN_LINE}请输入DNS2[选填]: ${NONE_LINE}"
    read dns2_

    return 0;
}



_get_broadcast() {
    printf "${GREEN_LINE}请输入广播地址: ${NONE_LINE}"
    read broadcast_

    return 0;
}


_get_ipv4addr() {
    printf "${GREEN_LINE}请输入ipv4地址: ${NONE_LINE}"
    read ipv4addr_

    if [ -z $ipv4addr_ ]; then
        printf "${RED_LINE}ipv4地址无效${NONE_LINE}"
        return 1
    fi

    return 0;
}


_put_ethx() {
    local fileName="/etc/sysconfig/network-scripts/${ethx_}"

    local device=`cat ${fileName} | grep DEVICE`
    local hwAddr=`cat ${fileName} | grep HWADDR`
    local uuid=`cat ${fileName} | grep UUID`

    printf "${device}\n" > $fileName
    printf "BOOTPROTO=none\n" >> $fileName
    printf "IPV6INIT=\"yes\"\n" >> $fileName
    printf "NM_CONTROLLED=\"yes\"\n" >> $fileName
    printf "ONBOOT=\"yes\"\n" >> $fileName
    printf "TYPE=\"Ethernet\"\n" >> $fileName
    printf "${uuid}\n" >> $fileName
    printf "IPADDR=${ipv4addr_}\n" >> $fileName
    printf "PREFIX=24\n" >> $fileName
    printf "GATEWAY=${gateway_}\n" >> $fileName
    printf "DEFROUTE=yes\n" >> $fileName
    printf "IPV4_FAILURE_FATAL=no\n" >> $fileName
    printf "IPV6_AUTOCONF=yes\n" >> $fileName
    printf "IPV6_DEFROUTE=yes\n" >> $fileName
    printf "IPV6_FAILURE_FATAL=no\n" >> $fileName
    printf "DNS1=${dns1_}\n" >> $fileName
    printf "BROADCAST=${broadcast_}\n" >> $fileName
    printf "NETMASK=${netmask_}\n" >> $fileName
    printf "${hwAddr}\n" >> $fileName
    printf "IPV6_PEERDNS=yes\n" >> $fileName
    printf "IPV6_PEERROUTES=yes\n" >> $fileName

    return 0
}



_choose_ethx() {
    printf "${GREEN_LINE}请选择要修改的网络接口: ${NONE_LINE}\n"

    local iflist=`ls /etc/sysconfig/network-scripts | grep ifcfg-eth`;
    printf "${GREEN_LINE}${iflist}${NONE_LINE}\n"
    read ethx_

    if [ -z ethx_ ]; then
        printf "${RED_LINE}网络接口选择错误${NONE_LINE}"
        return 1
    fi

    return 0
}



__main__() {
    _choose_ethx
    if [ $? != 0 ]; then
        _release_var
        exit 1
    fi

    _get_gateway
    if [ $? != 0 ]; then
        _release_var
        exit 1
    fi

    _get_netmask
    if [ $? != 0 ]; then
        _release_var
        exit 1
    fi

    _get_ipv4addr
    if [ $? != 0 ]; then
        _release_var
        exit 1
    fi


    _get_broadcast
    _get_dns1
    _get_dns2

    _put_ethx

    _release_var
    exit 0
}


__main__



