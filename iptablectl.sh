# !/bin/sh



GREEN_LINE='\033[32m'
RED_LINE='\033[31m'
YELLOW_LINE='\033[33m'
NONE_LINE='\033[0m'



_release_var() {
    unset GREEN_LINE
    unset RED_LINE
    unset YELLOW_LINE
    unset NONE_LINE
}



_add_port() {
    local port=''
    local protocol='tcp'

    printf "${GREEN_LINE}请输入要开发的端口: ${NONE_LINE}"
    read port
    echo $port

    if [[ ! $port =~ ^[1-9][0-9]*$ ]]; then
        printf "${RED_LINE}端口无效${NONE_LINE}\n"
        return 1
    fi

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



_save() {
    service iptables save
    if [ $? != 0 ]; then
        printf "${RED_LINE}service iptables save 执行失败${NONE_LINE}\n"
        return 1
    fi

    printf "${GREEN_LINE}配置保存成功${NONE_LINE}\n"
    return 0
}



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
    printf "${GREEN_LINE}请选择操作类型:\n1) 添加端口;\n2) 删除端口;${NONE_LINE}\n"

    local n=''
    read n

    case $n in
        1 )
            _add_port
            ;;
        2 ) 
            _del_port
            ;;
        * )
            printf "${RED_LINE}未知的选项${NONE_LINE}\n"
    esac
    
    if [ $? != 0 ]; then
        exit 1
    fi
    
    _save

    if [ $? != 0 ]; then
        exit 1
    fi

    _restart

    if [ $? != 0 ]; then
        exit 1
    fi
    printf "${GREEN_LINE}脚本完成${NONE_LINE}\n"

    _release_var
    exit 0
}



__main__
