1：插件法的自定义控件使用，务必保证编译器完全一致。
2：将对应的quc.dll和quc.lib(MSVC编译器才有)集成到项目中。
3：使用到哪个控件，只需要将对应控件的头文件集成到项目中即可。
4：项目的pro文件加入代码
INCLUDEPATH += $$PWD/sdk

CONFIG(release, debug|release){
LIBS        += -L$$PWD/sdk/ -lquc
} else {
unix {LIBS  += -L$$PWD/sdk/ -lquc}
else {LIBS  += -L$$PWD/sdk/ -lqucd}
}

5：本sdkdemo下的dll是MSVC2013 32位+Qt5.7.1版本的。可自行替换成自己编译器编译出来的文件。