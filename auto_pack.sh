#!/bin/bash

date_string=`date +%Y%m%d%H%M%S`

bug_stats=(alpha beta release)

bug_stat=alpha

#检查输入的bug状态是否在规定的数组内
#---start-----------------------
i_bug=0
b_find=0
input_bug=unknown

if [ ! -z $1 ]; then
	input_bug=$1
fi

for i_bug in ${bug_stats[*]}
do
	if [[ $input_bug == $i_bug ]]; then
		bug_stat=$i_bug
		b_find=1
		break
	fi
done

if [ $b_find -eq 0 ]; then
	echo "Invalid bug stat $1 !"
	echo "You should input bug state as follows:"
	for dsp_bug in ${bug_stats[*]}
	do
		echo -e "\t$dsp_bug"
	done

	exit 1
fi
#---end-------------------------

main_incre=20
sub_incre=1000
vcs_version=0
version=''
if [[ -d .git ]]; then
    vcs_version=`git rev-list HEAD | wc -l | awk '{print $1}'`
    version=r${vcs_version}-$(git rev-list HEAD -n 1 | cut -c 1-7)
elif [[ -d .svn ]]; then
    vcs_version=`svnversion -c |sed 's/^.*://' |sed 's/[A-Z]*$//'`
    fix_version=$(($vcs_version%$sub_incre))
    sub_version=$(($(($(($vcs_version/$sub_incre))+1))%${main_incre}))
    main_version=$(($(($(($vcs_version/$sub_incre))+1))/${main_incre}))
    version=${main_version}.${sub_version}.${fix_version}
else
    echo "No version control information, exit."
    exit 1
fi

#定义软件名
soft_name=server_frame
#soft_name+=$date_string
soft_name+=-
soft_name+=$bug_stat
soft_name+=-
soft_name+=$version

package_name=$soft_name.tar.gz

bin_name=$soft_name.bin


#指定需要安装的文件
pack_files=" "
pack_files+=" "include
pack_files+=" "lib
pack_files+=" "uninstall.sh

pack_dest_dir=`pwd`/release/$soft_name

bin_path=$pack_dest_dir/../$bin_name
package_path=$pack_dest_dir/../$package_name
temp_bin_path=$pack_dest_dir/../temp.bin
 
#拷贝源码目录构建安装目录

 
#删除多余的文件和目录
 
 
#将安装文件打成tar包
function tar_package()
{
	echo "Start tar package:$package_name...."

	echo "Create version file...."
	#生成VERSION文件
	echo "$version">./VERSION
	
	#将VERSION文件加入打包文件列表中
	pack_files+=" "VERSION

	echo "making dir $pack_dest_dir"
	mkdir -p $pack_dest_dir

	echo "copy files to $pack_dest_dir"

	cp -rf $pack_files $pack_dest_dir/

	echo "tar package $package_name"

	c_path=`pwd`

	cd $pack_dest_dir/../

	tar -zcvf $package_name $soft_name

	cd $c_path

	echo "tar package:$package_name complete!"
}

 
#计算文件校验值并写到安装脚本中
function add_sum()
{
	echo "Add sum to $package_path..."

	soft_sum=(`/usr/bin/sum $package_path`)
	sum_value=${soft_sum[0]}
	sum_size=${soft_sum[1]}
	echo "sum_value=$sum_value, sum_size=$sum_size"
	cat install.sh | sed -e s/sum_value/$sum_value/ \
	-e s/sum_size/$sum_size/ \
	-e "s#^version:=.*#version=$version#g" \
	-e "s#^bug_stat=.*#bug_stat=$bug_stat#g" \
	-e "s#^main_version=.*#main_version=$main_version#g" \
	-e "s#^sub_version=.*#sub_version=$sub_version#g" \
	-e "s#^fix_version=.*#fix_version=$fix_version#g" \
	> $temp_bin_path

}

 

 
#将安装脚本和目标文件一起打成bin文件
function make_bin()
{
	echo "Start make bin: $bin_path..."

	cat $temp_bin_path $package_path > $bin_path
	chmod a+x $bin_path
	
	echo "Make bin: $bin_path complete!"
}

#清理临时文件
function clean_temps()
{
	echo "Clean temp files..."
	rm -rf $temp_bin_path $package_path $pack_dest_dir
}

tar_package
add_sum
make_bin
clean_temps



