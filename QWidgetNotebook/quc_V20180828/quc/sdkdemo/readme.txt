1����������Զ���ؼ�ʹ�ã���ر�֤��������ȫһ�¡�
2������Ӧ��quc.dll��quc.lib(MSVC����������)���ɵ���Ŀ�С�
3��ʹ�õ��ĸ��ؼ���ֻ��Ҫ����Ӧ�ؼ���ͷ�ļ����ɵ���Ŀ�м��ɡ�
4����Ŀ��pro�ļ��������
INCLUDEPATH += $$PWD/sdk

CONFIG(release, debug|release){
LIBS        += -L$$PWD/sdk/ -lquc
} else {
unix {LIBS  += -L$$PWD/sdk/ -lquc}
else {LIBS  += -L$$PWD/sdk/ -lqucd}
}

5����sdkdemo�µ�dll��MSVC2013 32λ+Qt5.7.1�汾�ġ��������滻���Լ�����������������ļ���