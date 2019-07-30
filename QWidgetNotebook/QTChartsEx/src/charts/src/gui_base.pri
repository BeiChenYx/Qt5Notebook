#########################################################################################
# 注意：此文件用于放置工程文件的公共(基本)设置，
# 在各 QT 工程文件中通过 include 函数包含，不应单独使用！
# 开发人员需要定义下面几个系统环境变量：
# TRAINDEVHOME		系统开发主目录，放置与开发相关的各个子模块 (必须定义)
# TRAINBUILDTYPE	系统编译版本定义，指定编译版本类型：debug|release|all
# TRAINBUILDBIT		系统程序编译位数定义，指定编译位数：32|64
#########################################################################################

# 需先通过环境变量 TRAINDEVHOME 指定开发目录
# 由于 isEmpty 函数不能直接对环境变量进行判断，所以先将其放入一个临时变量中
DEVHOME = $$(TRAINDEVHOME)
isEmpty(DEVHOME) {
	error('TRAINDEVHOME'环境变量必须被定义.)
}
# 设置系统执行文件路径、库文件路径、头文件包含路径变量
TRAIN_BIN_PATH = $$(TRAINDEVHOME)/bin
TRAIN_LIB_PATH = $$(TRAINDEVHOME)/lib
TRAIN_OBJ_PATH = $$(TRAINDEVHOME)/obj
TRAIN_SRC_PATH = $$(TRAINDEVHOME)/src
TRAIN_UIC_PATH = $$(TRAINDEVHOME)/obj/uic
TRAIN_INCLUDE_PATH = $$(TRAINDEVHOME)/include

QMAKE_LIBDIR *= $$TRAIN_LIB_PATH
DEPENDPATH *=  .\
		$$TRAIN_INCLUDE_PATH
INCLUDEPATH *=  .\
		$$TRAIN_INCLUDE_PATH

# 加载编译版本配置
#########################################################################################
#
# 通过环境变量 TRAINBUILDTYPE 指定编译版本类型：debug|release
# 分别对应 调试版、运行版
#
# 通过环境变量 TRAINBUILDBIT 指定编译位数类型：32|64
# 分别对应 32 位、64 位
#
#####################################################################################################

# 配置系统使用的编译版本类型
# 如果未指定 TRAINBUILDTYPE 环境变量，缺省按 debug 处理
BUILDTYPE = $$(TRAINBUILDTYPE)
isEmpty(BUILDTYPE) {	
	message('TRAINBUILDTYPE'环境变量未设置，使用缺省值debug.)
	BUILDTYPE = debug
}
# 对TRAINBUILDTYPE设置进行简单的错误检查
# BUILDTYPE的值必须为单值
!count(BUILDTYPE,1){
	error(仅能定义使用一种有效的编译版本类型，请检查TRAINBUILDTYPE环境变量设置.)
}
!equals(BUILDTYPE,all){
!equals(BUILDTYPE,debug){
!equals(BUILDTYPE,release){
	error(未被支持的的编译版本类型，请检查TRAINBUILDTYPE环境变量设置.)
		}
	}
}

# 不同编译版本相关的配置
equals(BUILDTYPE,debug){
	CONFIG += debug
	CONFIG -= release
}
equals(BUILDTYPE,release){
	CONFIG += release
	CONFIG -= debug
}
equals(BUILDTYPE,all){
	CONFIG += debug_and_release build_all
}
# 指定代码中宏定义
debug_and_release {
	CONFIG(debug, debug|release) {
	    DEFINES += TRAIN_DEBUG
	}
	CONFIG(release, debug|release) {
	    DEFINES += TRAIN_RELEASE
	}
} else {
	debug:DEFINES += TRAIN_DEBUG
	release:DEFINES += TRAIN_RELEASE
}

# 配置系统使用的编译位数类型
# 如果未指定 TRAINBUILDBIT 环境变量，缺省按 32 位处理
BUILDBIT = $$(TRAINBUILDBIT)
isEmpty(BUILDBIT) {	
	message('TRAINBUILDBIT'环境变量未设置，使用缺省值 32 位.)
	BUILDBIT = 32
}

# 对TRAINBUILDBIT设置进行简单的错误检查
# BUILDBIT的值必须为单值
!count(BUILDBIT,1){
	error(仅能定义使用一种有效的编译位数类型，请检查TRAINBUILDBIT环境变量设置.)
}
!equals(BUILDBIT,32){
	!equals(BUILDBIT,64){
		error(未被支持的的编译位数类型，请检查TRAINBUILDBIT环境变量设置.)
	}
}

# 不同编译版本相关的配置
equals(BUILDBIT,32){
	# 扩展 32 位配置项
	CONFIG *= x86

	DEFINES *= TRAIN_32
}
equals(BUILDBIT,64){
	# 扩展 64 位配置项
	CONFIG *= x64

	DEFINES *= TRAIN_64
}


# 加载编译器设置
#########################################################################################
#
# 不同平台的编译器设置
#
#####################################################################################################

# 获取编译 Qt 的编译器类型
TRAIN_QMAKESPEC = $$(QMAKESPEC)

#UNIX + gcc下声明使用预编译头文件
#GCC 3.4 及以后版本支持预编译头文件
unix{
	contains( TRAIN_QMAKESPEC, g++ ) {
		CONFIG *= precompile_header
	}
}
#WIN32下声明使用预编译头文件
win32{
	CONFIG *= precompile_header
}

#UNIX下编译设置
unix{
	DEFINES *= unix __unix
	contains( TRAIN_QMAKESPEC, solaris.* ) {
		DEFINES *= SOLARIS
	}
	contains( TRAIN_QMAKESPEC, hpux.* ) {
		# HPUX下全面支持C++
		QMAKE_CXXFLAGS *= -Aa
		LIBS += -lrt
	}
	contains( TRAIN_QMAKESPEC, aix-xlc.* ) {
		# AIX下激活 rtti 编译选项
		# 使用注册文件保存模板实例化
		QMAKE_CXXFLAGS *= -qrtti=all -qtemplateregistry
		# AIX下编译动态链接库的链接选项
		QMAKE_LFLAGS_SHLIB *= -G
		# AIX下编译可执行程序的链接选项，以和 orbaucs 链接选项匹配，解决程序运行崩溃问题
		QMAKE_LFLAGS_APP *= -bdynamic -brtl
		# AIX下编译可执行程序的链接选项，以扩展每个进程允许使用的内存数量（缺省值256M，目前设置1G——0x40000000）
		#QMAKE_LFLAGS_APP *= -bmaxdata:0x40000000
	}
}

#WIN32下编译设置
win32{
	# 0x0500 -> 0x0501 操作系统最低 Windows XP，不再支持 Windows 2000
	DEFINES	*= WIN32 _WIN32_WINNT=0x0501

	# ui 生成的文件使用 utf-8 编码，编译时产生 4819 警告，去掉
	QMAKE_CXXFLAGS *= -wd4819

	# 去掉 strcpy 等编译警告
	QMAKE_CXXFLAGS *= -wd4996

	# 去掉使用 protobuf 引起的 “导出的类是从未导出的类派生的” 等编译警告
	QMAKE_CXXFLAGS *= -wd4251 -wd4275
}

#判断SUN机器的处理器是x86还是spac
unix {
	UNAME = $$system(uname -s)
	contains( UNAME, SunOS ) {
		UNAME = $$system(uname -m)
		contains( UNAME, i86pc ) {
			CONFIG += sunos_i86pc
		}
		contains( UNAME, sun4u ) {
			CONFIG += sunos_sun4u
		}
	}
}

#激活 STL、RTTI、EXCEPTIONS 支持
CONFIG *= stl exceptions rtti
#激活多线程、编译警告
CONFIG *= thread warn_on


# 加载更多的第三方库
#include ($$TRAIN_SRC_PATH/iccs_thirdlib_define.pri)

# 指定不同编译版本中间文件目录
debug_and_release {
	CONFIG(debug, debug|release) {
	    TRAIN_OBJ_PATH = $$TRAIN_OBJ_PATH/debug
	}
	CONFIG(release, debug|release) {
	    TRAIN_OBJ_PATH = $$TRAIN_OBJ_PATH/release
	}
} else {
	debug:TRAIN_OBJ_PATH = $$TRAIN_OBJ_PATH/debug
	release:TRAIN_OBJ_PATH = $$TRAIN_OBJ_PATH/release
}