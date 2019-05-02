# !/bin/sh


# ====================================
# @用途: 
#       生成centos6.x systemd脚本
#
# @作者: iegad
# @日期: 2019-05-01
# ====================================


GREEN_LINE='\033[32m'
RED_LINE='\033[31m'
YELLOW_LINE='\033[33m'
NONE_LINE='\033[0m'


chkconfName_=''
chkconfLevel_=''
chkconfStartPriority_=''
chkconfStopPriority_=''
chkconfDesc_=''
chkconfProcName_=''



execStart_=''
execStop_=''
execReload_=''
configPath_=''



_release_var() {
    unset GREEN_LINE
    unset RED_LINE
    unset YELLOW_LINE
    unset NONE_LINE

    unset chkconfName_
    unset chkconfLevel_
    unset chkconfStartPriority_
    unset chkconfStopPriority_
    unset chkconfDesc_
    unset chkconfProcName_
    unset execStart_
    unset execStop_
    unset execReload_
    unset configPath_
}



_build_sh() {
    printf "${GREEN_LINE}请输入服务名称,以d结尾[必填]:${NONE_LINE}"
    read chkconfName_

    if [[ -z $chkconfName_ ]]; then
        printf "${RED_LINE}服务名称不能为空${NONE_LINE}\n"
        return 1
    fi

    if [[ ! $chkconfName_ =~ [a-zA-Z0-9]+d$ ]]; then
        printf "${RED_LINE}服务名称${chkconfName_}不符合命名规则${NONE_LINE}\n"
        return 1
    fi

    printf "${GREEN_LINE}请输入服务等级[选填，默认为'-']:${NONE_LINE}"
    read chkconfLevel_

    if [[ -z $chkconfLevel_ ]]; then
        chkconfLevel_='-'
    fi

    printf "${GREEN_LINE}请输入服务启动优先级[选填，默认为'85']:${NONE_LINE}"
    read chkconfStartPriority_

    if [[ -z $chkconfStartPriority_ ]]; then
        chkconfStartPriority_='85'
    fi

    printf "${GREEN_LINE}请输入服务停止优先级[选填，默认为'15']:${NONE_LINE}"
    read chkconfStopPriority_

    if [[ -z $chkconfStopPriority_ ]]; then
        chkconfStopPriority_='15'
    fi

    printf "${GREEN_LINE}请输入服务描述[必填]:${NONE_LINE}"
    read chkconfDesc_

    if [[ -z $chkconfDesc_ ]]; then
        printf "${RED_LINE}请输入服务描述${NONE_LINE}\n"
        return 1
    fi

    printf "${GREEN_LINE}请输入服务进程名称[必填]:${NONE_LINE}"
    read chkconfProcName_

    if [[ -z $chkconfProcName_ ]]; then
        printf "${RED_LINE}服务进程名称不能为空${NONE_LINE}\n"
        return 1
    fi

    if [[ ! $chkconfProcName_ =~ [a-zA-Z0-9]+ ]]; then
        printf "${RED_LINE}服务进程名称${chkconfProcName_}不符合命名规则${NONE_LINE}\n"
        return 1
    fi

    printf "${GREEN_LINE}请输入服务启动命令[必填]:${NONE_LINE}"
    read execStart_

    if [[ -z $execStart_ ]]; then
        printf "${RED_LINE}服务启动命令不能为空${NONE_LINE}\n"
        return 1
    fi

    printf "${GREEN_LINE}请输入服务停止命令[必填]:${NONE_LINE}"
    read execStop_

    if [[ -z $execStop_ ]]; then
        printf "${RED_LINE}服务停止命令不能为空${NONE_LINE}\n"
        return 1
    fi

    printf "${GREEN_LINE}请输入服务重启命令[必填]:${NONE_LINE}"
    read execReload_

    if [[ -z $execReload_ ]]; then
        printf "${RED_LINE}服务重启命令不能为空${NONE_LINE}\n"
        return 1
    fi

    printf "${GREEN_LINE}请输入服务配置文件路径[选填]:${NONE_LINE}"
    read configPath_

    return 0
}


_generator_file() {
    printf "${GREEN_LINE}@开始生成文件...${NONE_LINE}\n"

    if [[ ! -w '/etc/init.d' ]]; then
        printf "${RED_LINE}未获得目录'/etc/init.d'相应权限${NONE_LINE}\n"
        return 1
    fi

    local fileName="/etc/init.d/${chkconfName_}"

    touch $fileName

    printf "# !/bin/sh\n\n\n" > $fileName
    printf "# chkconfig: ${chkconfLevel_} ${chkconfStartPriority_} ${chkconfStopPriority_}\n" >> $fileName
    printf "# description: ${chkconfDesc_}\n" >> $fileName
    printf "# processname: ${chkconfProcName_}\n\n" >> $fileName

    printf 'case $1 in\n' >> $fileName
    printf "    start )\n" >> $fileName
    printf "        \`${execStart_}\`\n" >> $fileName
    printf "        ;;\n\n" >> $fileName
    printf "    stop )\n" >> $fileName
    printf "        \`${execStop_}\`\n" >> $fileName
    printf "        ;;\n\n" >> $fileName
    printf "    reload )\n" >> $fileName
    printf "        \`${execReload_}\`\n" >> $fileName
    printf "        ;;\n\n" >> $fileName
    printf "    restart )\n" >> $fileName
    printf "        \`${execReload_}\`\n" >> $fileName
    printf "        ;;\n\n" >> $fileName
    printf "    * )\n" >> $fileName
    printf "        exit 1\n" >> $fileName
    printf "        ;;\n" >> $fileName
    printf "esac\n\n\n" >> $fileName
    printf "exit 0" >> $fileName

    return 0
}


_install_service() {
    printf "${GREEN_LINE}@开始部署服务...${NONE_LINE}\n"

    local svcName=${chkconfName_}

    `chmod 744 /etc/init.d/$svcName`
    
    if [ $? != 0 ]; then
        printf "${RED_LINE}'chmod 744 ${svcName}'执行失败${NONE_LINE}\n"
        return 1
    fi
    `chkconfig --add $svcName`

    if [ $? != 0 ]; then
        printf "${RED_LINE}'chkconfig -- add ${svcName}'执行失败${NONE_LINE}\n"
        return 1
    fi

    `chkconfig $svcName on`

    if [ $? != 0 ]; then
        printf "${RED_LINE}'chkconfig ${svcName} on'执行失败${NONE_LINE}\n"
        return 1
    fi

    return 0
}




__main__() {
    _build_sh

    if [ $? != 0 ]; then
        _release_var
        exit 1
    fi

    _generator_file

    if [ $? != 0 ]; then
        _release_var
        exit 1
    fi

    _install_service

    if [ $? != 0 ]; then
        _release_var
        exit 1
    fi

    printf "${GREEN_LINE}****** 服务配置完成，重启后生效 ******${NONE_LINE}\n"

    return 0
}



__main__
_release_var
