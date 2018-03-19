#!/bin/bash

source ~/.bashrc

#以下变量值会被auto_pack.sh覆盖
#---start---
bug_stat=alpha
main_version=0
sub_version=1
fix_version=0
#---end---

version=$main_version.$sub_version.$fix_version

#定义软件名
soft_name=server_frame
#soft_name+=$date_string
soft_name+=-
soft_name+=$bug_stat
soft_name+=-
soft_name+=$version

package_name=$soft_name.tar.gz

AppFolder="/usr"
InstallPath=

#软件环境变量名
soft_root_macro=SF_ROOT

#系统环境变量自定义范围标识
flag_env_begin="#sf env begin"
flag_env_end="#sf env end"

#系统环境文件列表
rc_file_path=~/.bashrc
ld_so_conf=/etc/ld.so.conf
env_files=
env_files+=" "$rc_file_path
env_files+=" "$ld_so_conf
 
#打印许可
function printLicense() {
    more <<"EOF"
     
      License Agreement
    .............
EOF

	stty erase '^H'
	read -p "Do you agree to the above license terms? [y/n]:" reply leftpver
	case $reply in
    y* | Y*)
        echo "You have agreed to the license, install start...";;
    n* | N*)
        echo "You don't agreed to the license, you can't install $soft_name."
        exit 1;;
    *)
        echo "Invalid select, exit install ."
        exit 1;;
    esac
}
 
#版本号比较
function VersionComp() {
    if [ "$1" = "$2" ]; then
        echo "eq"
    else
        lrel=`printf "%s\n%s\n" $1 $2 | \
               sort -t . -k 1,1n -k 2,2n -k 3,3n -k 4,4n -k 5,5n | \
                 head -1`
        if [ "$1" = "${lrel}" ]; then
            echo "lt"
        else
            echo "gt"
        fi
    fi
}
 
#软件版本号检测
function VersionCheck() {
    root_dir=`eval echo '$'"$soft_root_macro"`
	
	if [ -z $root_dir ];then
		echo "You've never installed $soft_name before..."
		return
	fi

	
    VersionFile=$root_dir/VERSION
    if [ -e $VersionFile ]; then
        OldVersion=`head -1 $VersionFile | awk '{print $1}' | cut -d"," -f1`
        if [ "gt" = $(VersionComp "$OldVersion" "$version") ]; then
            echo "Newer version ($OldVersion) is already installed, please uninstall first!"
            exit 1
        else
			stty erase '^H'
			read -p "Older version ($OldVersion) is already installed, upgrade to $version? [y/n]:" reply leftover
            case $reply in
            y* | Y*)
				;;
            n* | N*)
                echo "You canceled to install $soft_name."
                exit 1;;
            *)
                echo "Invalid select, exit install ."
                exit 1;;
            esac
        fi
	else
		echo "Version file : $VersionFile does not exist!"
    fi
}
 
#设置安装路径
function SetInstallPath() {
	stty erase '^H'
    read -p "Install $soft_name to $AppFolder, sure? [y/n]:" reply leftover
    case $reply in
    y* | Y*)
        ;;
    n* | N*)
		stty erase '^H'
        read -p "Please input install path: " AppFolder leftover;;
    *)
        echo "Invalid select, exit install ."
        exit 1;;
    esac
     
    AppFolder=`readlink -f $AppFolder`
    InstallPath=$AppFolder
    mkdir -p $InstallPath
}
 
#文件校验
function CheckSum() {
	if [ ! -x /usr/bin/sum ]; then
		echo "Can't find /usr/bin/sum to do checksum, continuing anyway."
		return 0
    fi
 
    echo "Checksumming..."
    sumret=(`/usr/bin/sum $package_name`)
	echo "sum_v=${sumret[0]}, sum_s=${sumret[1]}"
    if [ ${sumret[0]} != sum_value -o ${sumret[1]} != sum_size ]; then
       	echo "The install file appears to be corrupted."
       	echo "Please do not attempt to install this file."
       	exit 1;
    fi
}
 
#解包
function Unpack() {
    echo "Unpacking..."
    lines=`awk '/^exit 0/{print NR+1;exit 1;}' "$0"`
	echo "shell lines=$lines"
    tail -n +$lines "$0" > $package_name      2>/dev/null
    
    CheckSum
    
    tar -zvxf $package_name -C $InstallPath/  2>/dev/null
     
    rm $package_name
}
 
#写环境变量
function WriteEnv() {
	date_string=`date +%Y%m%d%H%M%S`
	#备份配置文件
	
	echo "Backup environment file..."
	
	cp $rc_file_path $rc_file_path.bak$date_string
	cp $ld_so_conf $ld_so_conf.bak$date_string
	
	echo "Update environment file..."

	
	#删除指定范围的文本
	
	sed -i "/^$flag_env_begin/,/$flag_env_end$/d" ~/.bashrc
#	sed -i "/$flag_env_begin/d" ~/.bashrc
#	sed -i "/$flag_env_end/d" ~/.bashrc
	
	#重新设置环境变量
	
    echo "$flag_env_begin
export $soft_root_macro=$InstallPath/$soft_name
export LD_LIBRARY_PATH=\$$soft_root_macro/lib:\$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=\$(echo \$LD_LIBRARY_PATH | sed 's/:/\n/g' | sort | uniq | tr -s '\n' ':' | sed 's/:\$//g')
$flag_env_end" >>$rc_file_path

	source $rc_file_path
	
	#将.so的搜索路径加入/etc/ld.so.conf中
	
	echo "Add $InstallPath/$soft_name/lib to $ld_so_conf ..."

	sed -i "/^$flag_env_begin/,/$flag_env_end$/d" $ld_so_conf
#	sed -i "/$flag_env_begin/d" $ld_so_conf
#	sed -i "/$flag_env_end/d" $ld_so_conf
	
	echo "$flag_env_begin
$InstallPath/$soft_name/lib
$flag_env_end" >>$ld_so_conf

	ldconfig
}

#修改卸载脚本
function Modify_uninstall_script()
{
	local uninstall_file_path=$InstallPath/$soft_name/uninstall.sh
	echo "Modifying uninstall script:[$uninstall_file_path]..."
	if [ ! -e $uninstall_file_path ]; then
		echo "Warning: $uninstall_file_path missing !"
	else
		sed -i "s#^version=.*#version=$version#g" $uninstall_file_path
		sed -i "s#^soft_name=.*#soft_name=$soft_name#g" $uninstall_file_path
#		sed -i "s#^server_name=.*#server_name=$server_name#g" $uninstall_file_path
		sed -i "s/^flag_env_begin=.*/flag_env_begin=\"$flag_env_begin\"/g" $uninstall_file_path
		sed -i "s/^flag_env_end=.*/flag_env_end=\"$flag_env_end\"/g" $uninstall_file_path
		sed -i "s#^env_files=.*#env_files='$env_files'#g" $uninstall_file_path
		sed -i "s#^soft_root_macro=.*#soft_root_macro=$soft_root_macro#g" $uninstall_file_path
		
		chmod +x $uninstall_file_path
		
		echo "Modify uninstall script:[$uninstall_file_path] complete !"
	fi
}

VersionCheck
#printLicense
SetInstallPath
Unpack
Modify_uninstall_script
WriteEnv
 
echo "Install complete!"
exit 0
