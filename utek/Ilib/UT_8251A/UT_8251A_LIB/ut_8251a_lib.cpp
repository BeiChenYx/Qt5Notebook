#include <string>
#include <iostream>
#include <fstream>
#include <locale>
#include <list>
#include <codecvt>
#include "ut_8251a_lib.h"


using std::list;
using std::string;
using std::wstring;
using std::codecvt;
using std::begin;
using std::end;

static hid_device *gPhidHandle = nullptr;
const unsigned char gHidReportID = 0; // HID的ReportID
const int gCanReadTimes = 100;

const int PKG_MAX_LEN = 16;

static CanDataFunc pRcvData = nullptr;
static void *gPUser = nullptr;


// 封装协议，1字节对齐
#pragma pack(1)
struct CANParamREQCom{
    // pc 2 mcu, 设置CAN参数
    uchar header[2];            // 包头
    uchar len;                  // 包长度
    uchar cmd;                  // 功能命令
    uchar bauterate;            // CAN通信波特率
    uchar canID[4];             // CAN帧ID， 高字节在前
    uchar frameFormat;          // 帧格式: 0x00 标准帧, 0x01 扩展帧
    uchar checkSum;             // 校验和
};

struct USARTParamREQCom{
    // pc 2 mcu, 设置串口参数
    uchar header[2];            // 包头
    uchar len;                  // 包长度
    uchar cmd;                  // 功能命令
    uchar bauderate;            // 串口波特率
    uchar dataBit;              // 数据位
    uchar parityBit;            // 校验位
    uchar stopBit;              // 停止位
    uchar flowControl;          // 流控制
    uchar serailModel;          // 串口模式: 0x00 RS232, 0x01 RS485, 0x02 RS422
    uchar checkSum;             // 校验和
};

struct TransitionParamREQCom{
    // pc 2 mcu, 设置转换参数
    uchar header[2];            // 包头
    uchar len;                  // 包长度
    uchar cmd;                  // 功能命令
    uchar transitionModel;      // 转换模式
    uchar transitionDirecte;    // 转换方向
    uchar enableSerial;         // 0x00 不允许CAN帧信息转发到串行帧中, 0x01 允许
    uchar enableID;             // 0x00 不允许CAN帧ID转发到串行帧中, 0x01 允许
    uchar serialOffset;         // CAN标识符在串行帧中的起始偏移
    uchar serialFramLen;        // CAN标识符在串行帧中的长度
    uchar serialTimes;          // 串行帧之间的时间间隔，单位 毫秒
    uchar transitionTimes;      // CAN帧转发的时间间隔, 单位 毫秒
    uchar checkSum;             // 校验和
};

struct HidIapDataREQ{
    // pc 2 mcu, 发送升级数据包
    uchar header[2];            // 包头
    uchar len;                  // 包长度
    uchar cmd;                  // 功能命令
    uchar id[4];                // 当前发送的数据包编号
    uchar pkgLen;               // 有效数据包长度
    uchar data[PKG_MAX_LEN];    // 升级数据包
    uchar checkSum;             // 校验和
};
struct FilterIdData{
    uchar index;                // 索引
    uchar frameType;            // CAN帧ID滤波类型
    uchar startId[4];           // 起始ID
    uchar endId[4];             // 结束ID
};

struct HidParAck{
    uchar header[2];            // 包头
    uchar len;                  // 包长度
    uchar cmd;                  // 功能命令
    uchar baute;                // CAN通信波特率
    uchar filterCount;          // 滤波组总数，最大10组
    uchar index;                // 当前滤波组索引
    uchar frameType;            // CAN帧ID滤波类型
    uchar startId[4];           // 起始ID
    uchar endId[4];             // 结束ID
    uchar checkSum;             // 校验和
};

struct HidFilterIdREQ{
    // pc 2 mcu, 发送升级数据包
    uchar header[2];            // 包头
    uchar len;                  // 包长度
    uchar cmd;                  // 功能命令
    uchar filterCount;          // 滤波组总数，最大10组
    uchar index;                // 当前滤波组索引
    uchar frameType;            // CAN帧ID滤波类型
    uchar startId[4];           // 起始ID
    uchar endId[4];             // 结束ID
    uchar checkSum;             // 校验和
};
struct GeneralPkgReq{
    // 通用的数据包，包含一个有效的功能命令
    uchar header[2]{0x2E, 0x30};            // 包头
    uchar len{0x03};                  // 包长度
    uchar cmd;                  // 功能命令
    uchar checkSum;             // 校验和
};
struct GeneralPkgAck{
    // 通用的数据包，包含一个有效的功能命令
    uchar header[2]{0x2E, 0x30};            // 包头
    uchar len{0x03};                  // 包长度
    uchar cmd;                  // 功能命令
    uchar checkSum;             // 校验和
};
struct GeneralPkgReAck{
    // 通用的数据包，包含一个有效的功能命令
    uchar header[2];            // 包头
    uchar len;                  // 包长度
    uchar cmd;                  // 功能命令
    uchar re;                   // 执行结果: 0x00 失败, 0x01 成功
    uchar checkSum;             // 校验和
};
struct InitInfoAck{
    uchar header[2];            // 包头
    uchar len;                  // 包长度
    uchar cmd;                  // 功能命令
    uchar mVer;                 // 主板本号
    uchar pVer;                 // 次版本号
    uchar checkSum;             // 校验和
};
struct CanReadAck{
    uchar header[2];            // 包头
    uchar len;                  // 包长度
    uchar cmd;                  // 功能命令
    uchar byteSum[2];           // CAN收到的字节总数
    uchar id[4];                // ID号
    uchar frameType;            // 帧类型
    uchar framFormat;           // 帧格式
    uchar validLen;             // 有效数据长度
    uchar data[8];              // 数据
    uchar datetime[4];           // 时间
    uchar checkSum;             // 校验和
};
struct USARTComfigACKSerial{
    // mcu 2 pc, 收到的MCU发送的USART当前配置
    uchar header[2];            // 包头
    uchar len;                  // 包长度
    uchar cmd;                  // 功能命令
    uchar transitionModel;      // 转换模式
    uchar transitionDirecte;    // 转换方向
    uchar enableSerial;         // 0x00 不允许CAN帧信息转发到串行帧中, 0x01 允许
    uchar enableID;             // 0x00 不允许CAN帧ID转发到串行帧中, 0x01 允许
    uchar serialOffset;         // CAN标识符在串行帧中的起始偏移
    uchar serialFramLen;        // CAN标识符在串行帧中的长度
    uchar serialTimes;          // 串行帧之间的时间间隔，单位 毫秒
    uchar transitionTimes;      // CAN帧转发的时间间隔, 单位 毫秒
    uchar bauderate;            // 串口波特率
    uchar dataBit;              // 数据位
    uchar parityBit;            // 校验位
    uchar stopBit;              // 停止位
    uchar flowControl;          // 流控制
    uchar serailModel;          // 串口模式: 0x00 RS232, 0x01 RS485, 0x02 RS422
    uchar checkSum;             // 校验和
};
struct USARTComfigACKCan{
    // mcu 2 pc, 收到的MCU发送的USART当前配置
    uchar header[2];            // 包头
    uchar len;                  // 包长度
    uchar cmd;                  // 功能命令
    uchar canBauderate;         // CAN通信波特率
    uchar canID[4];             // CAN帧ID， 高字节在前
    uchar frameFormat;          // 帧格式: 0x00 标准帧, 0x01 扩展帧
    uchar filterCount;          // 滤波组总数，最大10组
    uchar index;                // 当前滤波组索引
    uchar frameType;            // CAN帧ID滤波类型
    uchar startId[4];           // 起始ID
    uchar endId[4];             // 结束ID
    uchar checkSum;             // 校验和
};

#pragma pack()

void handleHidDataRcv(uchar *rst, size_t len);
int handleHidWrite(unsigned char *data, size_t length);

void writeLog(const string &msg)
{
//    const string logPath = "./UT8251A_log.txt";
//    std::ofstream fout(logPath, std::ios::app);
//    fout << msg << std::endl;
//    fout.close();
}



bool initLib()
{
    if(hid_init() == -1){
        writeLog("hid_init error");
        return false;
    }
    writeLog("hid_init ok");
    return true;
}

void freeLib()
{
    hid_exit();
    writeLog("freeLib ok");
}

hid_device_info *enumHidDevs(){
    return hid_enumerate(0, 0);
}
void freeEnumHidDevs(hid_device_info *dev){
    hid_free_enumeration(dev);
}

bool hidOpen(char *path, size_t size)
{
//    std::wstring_convert<std::codecvt_utf8<wchar_t> > strCnv;
    writeLog("hidOpen:" + std::to_string(size) + " path:" + string(path));
    if(size <= 0){return false;}
    try {
        gPhidHandle = hid_open_path(path);
        if(gPhidHandle == nullptr){
            return false;
        }
        // 打开非阻塞功能
        if(hid_set_nonblocking(gPhidHandle, 1) == -1){
            return false;
        }
        return true;
    } catch (...) {
//        writeLog("hidOpen:" +  strCnv.to_bytes(hid_error(gPhidHandle)));
    }
    return false;
}

bool hidOpenSerial(unsigned short vid, unsigned short pid, char *serial_number, size_t size)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t> > strCnv;
    if(size <= 0){return false;}
    try {
        string serialTmp(serial_number);
        std::wstring serialNumber(serialTmp.begin(), serialTmp.end());
        gPhidHandle = hid_open(vid, pid, serialNumber.c_str());
        if(gPhidHandle == nullptr){
            writeLog("hidOpen error.");
            return false;
        }
        // 打开非阻塞功能
        if(hid_set_nonblocking(gPhidHandle, 1) == -1){
            writeLog("hid_set_nonblocking error.");
            return false;
        }
        return true;
    } catch (...) {
        writeLog("hidOpen exception.");
    }
    return false;
}

void hidClose()
{
    if(gPhidHandle != nullptr){
        hid_close(gPhidHandle);
        gPhidHandle = nullptr;
    }
}

void setCallBack(CanDataFunc pFunc, void *pUser)
{
    pRcvData = pFunc;
    gPUser = pUser;
}

int handleHidWrite(unsigned char *data, size_t length)
{
    uchar buff[33]{gHidReportID, 0};
    memcpy(buff+1, data, length > 32 ? 32 : length);
    return hid_write(gPhidHandle, buff, 33);
}

int handleHidRead(unsigned char *data, size_t length)
{
    int re = hid_read_timeout(gPhidHandle, data, length, gCanReadTimes);
    if(re > 0){
        GeneralPkgAck *pAck = reinterpret_cast<GeneralPkgAck*>(data);
        if(pAck->cmd == 0x01){
            if(length < sizeof(CanReadAck)){
                uchar buf[sizeof(CanReadAck)]{0};
                memcpy(buf, data, length);
                size_t sub = sizeof(CanReadAck) - length;
                int subRe = hid_read_timeout(gPhidHandle, buf + length, sub, gCanReadTimes);
                if(subRe > 0){
                    handleHidDataRcv(buf, sizeof(CanReadAck));
                }
            }else {
                handleHidDataRcv(data, sizeof(CanReadAck));
            }
            return 0;
        }else {
            return re;
        }
    }
    return 0;
}

uchar checkSum(unsigned char *buff, int len)
{
    unsigned int sum = 0;
    for(int i = 0; i < len; ++i){
        sum += buff[i];
    }
    sum = ~sum + 1;
    return static_cast<uchar>(sum);
}

bool isAliveHid()
{
//    if(gPhidHandle == nullptr){ return false; }
//    GeneralPkgReq req;
//    req.cmd = 0xF4;
//    uchar *pReq = reinterpret_cast<uchar*>(&req);
//    pReq[sizeof(GeneralPkgReq)-1] = checkSum(pReq+2, sizeof(GeneralPkgReq)-3);
//    if(handleHidWrite(pReq, sizeof(GeneralPkgReq)) <= 0){
//        unsigned char aliveRst[sizeof(GeneralPkgAck)]{0};
//        if(handleHidRead(aliveRst, sizeof(GeneralPkgAck)) > 0){
//            uchar reCheckSum = checkSum(aliveRst+2, aliveRst[2]-1);
//            GeneralPkgAck *pRe = reinterpret_cast<GeneralPkgAck*>(aliveRst);
//            if(pRe->checkSum == reCheckSum && pRe->cmd == 0x0B){
//                return true;
//            }
//        }
//    }
//    hidClose();
//    return false;

    if(gPhidHandle == nullptr){ return false; }
    GeneralPkgReq req;
    req.cmd = 0xF4;
    uchar *pReq = reinterpret_cast<uchar*>(&req);
    pReq[sizeof(GeneralPkgReq)-1] = checkSum(pReq+2, sizeof(GeneralPkgReq)-3);
    if(handleHidWrite(pReq, sizeof(GeneralPkgReq)) <= 0){
        return false;
    }
    unsigned char aliveRst[sizeof(GeneralPkgAck)]{0};
    if(handleHidRead(aliveRst, sizeof(GeneralPkgAck)) <= 0){
        return false;
    }
    uchar reCheckSum = checkSum(aliveRst+2, aliveRst[2]-1);
    GeneralPkgAck *pRe = reinterpret_cast<GeneralPkgAck*>(aliveRst);
    if(pRe->checkSum != reCheckSum || pRe->cmd != 0x0B){
        hidClose();
        return false;
    }
    return true;
}

bool canOpen()
{
    if(gPhidHandle == nullptr){ return false; }
    try {
        unsigned char cmd[6] = {0x2E, 0x30, 0x04, 0xF7, 0x01, 0x00};
        cmd[sizeof(cmd)/sizeof(cmd[0])-1] = checkSum(cmd+2, cmd[2]-1);
        int reCount = handleHidWrite(cmd, sizeof(cmd)/sizeof(cmd[0]));
        if(reCount <= 0){
            return  false;
        }
        unsigned char rst[6]{0};
        int re = handleHidRead(rst, sizeof(rst)/sizeof(rst[0]));
        if(re <= 0){
            return false;
        }
        unsigned char rstCmp[]{0x2E, 0x30, 0x04, 0x08, 0x01, 0x00};
        rstCmp[sizeof(rstCmp)/sizeof(rstCmp[0])-1] = checkSum(rstCmp+2, rstCmp[2]-1);
        for(size_t i = 0; i<sizeof(rstCmp)/sizeof(rstCmp[0]); ++i){
            if(rst[i] != rstCmp[i]){ return false; }
        }
        return true;
    } catch (...) {
        return false;
    }
}

bool canClose()
{
    if(gPhidHandle == nullptr){ return false; }
    try {
        unsigned char cmd[] = {0x2E, 0x30, 0x04, 0xF7, 0x00, 0x00};
        cmd[sizeof(cmd)/sizeof(cmd[0])-1] = checkSum(cmd+2, cmd[2]-1);
        if(0 >= handleHidWrite(cmd, sizeof(cmd)/sizeof(cmd[0]))){ return  false; }
        unsigned char rst[6]{0};
        if(0 >= handleHidRead(rst, 6)){ return false; }
        unsigned char rstCmp[]{0x2E, 0x30, 0x04, 0x08, 0x01, 0x00};
        rstCmp[sizeof(rstCmp)/sizeof(rstCmp[0])-1] = checkSum(rstCmp+2, rstCmp[2]-1);
        for(size_t i = 0; i<sizeof(rstCmp)/sizeof(rstCmp[0]); ++i){
            if(rst[i] != rstCmp[i]){ return false; }
        }
        return true;
    } catch (...) {
        return false;
    }
}

bool canInit(char *version, size_t size)
{
    if(gPhidHandle == nullptr){ return false; }
    if(size < 7){ return false;}
    try {
        GeneralPkgReq req;
        req.header[0] = 0x2E;
        req.header[1] = 0x30;
        req.len = 0x03;
        req.cmd = 0xF6;
        req.checkSum = checkSum(reinterpret_cast<uchar*>(&req)+2, sizeof(GeneralPkgReq)-3);
        if(0 >= handleHidWrite(reinterpret_cast<uchar*>(&req), sizeof(GeneralPkgReq))){
            return  false;
        }

        uchar rst[sizeof(InitInfoAck)]{0};
        if(0 >= handleHidRead(rst, sizeof(InitInfoAck))){
            return false;
        }
        uchar calCheck = checkSum(rst + 2, sizeof(InitInfoAck)-3);
        InitInfoAck *pRst = reinterpret_cast<InitInfoAck*>(rst);
        if(pRst->cmd != 0x09 || pRst->checkSum != calCheck){
            return false;
        }
        std::sprintf(version, "V%c.%c", pRst->mVer, pRst->pVer);
        writeLog("canInit: " + string(version));
        return true;
    } catch (...) {
        // 接收处理过程中出现任何错误都重新读取，不做特殊处理
        return false;
    }
}

bool canRcvEnable(bool enable)
{
    if(gPhidHandle == nullptr){ return false; }
    try {
        unsigned char cmd[] = {0x2E, 0x30, 0x04, 0xF8, 0x00, 0x00};
        cmd[4] = static_cast<unsigned char>(enable);
        cmd[sizeof(cmd)/sizeof(cmd[0])-1] = checkSum(cmd+2, cmd[2]-1);
        if(0 >= handleHidWrite(cmd, sizeof(cmd)/sizeof(cmd[0]))){ return  false; }
        unsigned char rst[6]{0};
        if(0 >= handleHidRead(rst, 6)){ return false; }
        unsigned char rstCmp[]{0x2E, 0x30, 0x04, 0x07, 0x01, 0x00};
        rstCmp[sizeof(rstCmp)/sizeof(rstCmp[0])-1] = checkSum(rstCmp+2, rstCmp[2]-1);
        for(size_t i = 0; i<sizeof(rstCmp)/sizeof(rstCmp[0]); ++i){
            if(rst[i] != rstCmp[i]){ return false; }
        }
        return true;
    } catch (...) {
        return false;
    }
}

bool canSetModel(unsigned char model)
{
    if(gPhidHandle == nullptr){ return false; }
    try {
        unsigned char cmd[] = {0x2E, 0x30, 0x04, 0xF9, model, 0x00};
        cmd[sizeof(cmd)/sizeof(cmd[0])-1] = checkSum(cmd+2, cmd[2]-1);
        if(0 >= handleHidWrite(cmd, sizeof(cmd)/sizeof(cmd[0]))){ return  false; }
        unsigned char rst[6]{0};
        if(0 >= handleHidRead(rst, 6)){ return false; }
        unsigned char rstCmp[]{0x2E, 0x30, 0x04, 0x06, 0x01, 0x00};
        rstCmp[sizeof(rstCmp)/sizeof(rstCmp[0])-1] = checkSum(rstCmp+2, rstCmp[2]-1);
        for(size_t i = 0; i<sizeof(rstCmp)/sizeof(rstCmp[0]); ++i){
            if(rst[i] != rstCmp[i]){ return false; }
        }
        return true;
    } catch (...) {
        return false;
    }
}

bool canReset()
{
    if(gPhidHandle == nullptr){ return false; }
    try {
        unsigned char cmd[] = {0x2E, 0x30, 0x03, 0xFB, 0x00};
        cmd[sizeof(cmd)/sizeof(cmd[0])-1] = checkSum(cmd+2, cmd[2]-1);
        if(0 >= handleHidWrite(cmd, sizeof(cmd)/sizeof(cmd[0]))){ return  false; }
        unsigned char rst[5]{0};
        if(0 >= handleHidRead(rst, 5)){ return false; }
        unsigned char rstCmp[]{0x2E, 0x30, 0x03, 0x04, 0x00};
        rstCmp[sizeof(rstCmp)/sizeof(rstCmp[0])-1] = checkSum(rstCmp+2, rstCmp[2]-1);
        for(size_t i = 0; i<sizeof(rstCmp)/sizeof(rstCmp[0]); ++i){
            if(rst[i] != rstCmp[i]){ return false; }
        }
        return true;
    } catch (...) {
        return false;
    }
}

bool canResetError()
{
    if(gPhidHandle == nullptr){ return false; }
    try {
        unsigned char cmd[] = {0x2E, 0x30, 0x03, 0xFA, 0x00};
        cmd[sizeof(cmd)/sizeof(cmd[0])-1] = checkSum(cmd+2, cmd[2]-1);
        if(0 >= handleHidWrite(cmd, sizeof(cmd)/sizeof(cmd[0]))){ return  false; }
        unsigned char rst[5]{0};
        if(0 >= handleHidRead(rst, 5)){ return false; }
        unsigned char rstCmp[]{0x2E, 0x30, 0x03, 0x05, 0x00};
        rstCmp[sizeof(rstCmp)/sizeof(rstCmp[0])-1] = checkSum(rstCmp+2, rstCmp[2]-1);
        for(size_t i = 0; i<sizeof(rstCmp)/sizeof(rstCmp[0]); ++i){
            if(rst[i] != rstCmp[i]){ return false; }
        }
        return true;
    } catch (...) {
        return  false;
    }
}

bool canSetBaudRate(uchar baudrate)
{
    if(gPhidHandle == nullptr){ return false; }
    try {
        unsigned char cmd[6] = {0x2E, 0x30, 0x04, 0xFC, baudrate, 0x00};
        cmd[5] = checkSum(cmd + 2, 3);
        if(0 >= handleHidWrite(cmd, sizeof(cmd)/sizeof(cmd[0]))){ return  false; }
        unsigned char rst[6]{0};
        if(0 >= handleHidRead(rst, 6)){ return false; }
        unsigned char rstCmp[]{0x2E, 0x30, 0x04, 0x03, 0x01,0x00};
        rstCmp[sizeof(rstCmp)/sizeof(rstCmp[0])-1] = checkSum(rstCmp+2, rstCmp[2]-1);
        for(size_t i = 0; i<sizeof(rstCmp)/sizeof(rstCmp[0]); ++i){
            if(rst[i] != rstCmp[i]){ return false; }
        }
        return true;
    } catch (...) {
        return false;
    }
}

int canWrite(unsigned int id, uchar frameType, uchar frameFormat, uchar *data, uchar size)
{
    if(gPhidHandle == nullptr){ return -1; }
    if(size > 8){ return -1; }
    if(frameType > 0x01 || frameFormat > 0x01){ return -1; }
    try {
        unsigned char idByte[4]{0};
        idByte[0] = (id & 0xFF000000) >> 24;
        idByte[1] = (id & 0x00FF0000) >> 16;
        idByte[2] = (id & 0x0000FF00) >> 8;
        idByte[3] = id & 0x000000FF;
        // 封装发送数据包
        unsigned char cmd[20]{ 0x2E, 0x30, 0x12, 0xFD, idByte[0], idByte[1], idByte[2], idByte[3],
            frameType, frameFormat, 0x00
        };
        cmd[10] = size;
        memcpy(cmd+11, data, size);
        cmd[19] = checkSum(cmd + 2, 17);
        int re = handleHidWrite(cmd, sizeof(cmd)/sizeof(cmd[0]));
        if(re <= 0){ return  -1; }
        unsigned char rst[6]{0};
        if(0 >= handleHidRead(rst, 6)){ return -1; }
        unsigned char rstCmp[]{0x2E, 0x30, 0x04, 0x02, 0x01, 0x00};
        rstCmp[sizeof(rstCmp)/sizeof(rstCmp[0])-1] = checkSum(rstCmp+2, rstCmp[2]-1);
        for(size_t i = 0; i<sizeof(rstCmp)/sizeof(rstCmp[0]); ++i){
            if(rst[i] != rstCmp[i]){ return -1; }
        }
        return size;
    } catch (...) {
        return -1;
    }
}

void handleHidDataRcv(uchar *rst, size_t len)
{
    if(len != sizeof(CanReadAck)){return;}
    CanReadAck *pRst = reinterpret_cast<CanReadAck*>(rst);
    uchar check = checkSum(rst+2, sizeof(CanReadAck)-3);
    if(check != pRst->checkSum || pRst->cmd != 0x01){
        return;
    }
    GeneralPkgReq req;
    req.header[0] = 0x2E;
    req.header[1] = 0x30;
    req.len = 0x03;
    req.cmd = 0xFE;
    req.checkSum = checkSum(reinterpret_cast<uchar*>(&req)+2, sizeof(GeneralPkgReq)-3);
    if(0 >= handleHidWrite(reinterpret_cast<uchar*>(&req), sizeof(GeneralPkgReq))){
        return;
    }
    CanRcvData rstBuf;
    rstBuf.id = pRst->id[0];
    rstBuf.id = (rstBuf.id) << 8 | pRst->id[1];
    rstBuf.id = (rstBuf.id) << 8 | pRst->id[2];
    rstBuf.id = (rstBuf.id) << 8 | pRst->id[3];
    rstBuf.frameType = pRst->frameType;
    rstBuf.frameFormat = pRst->framFormat;
    if(pRst->validLen > 8){ return;}
    memcpy(rstBuf.data, pRst->data, pRst->validLen);
    rstBuf.timestamp = pRst->datetime[0];
    rstBuf.timestamp = rstBuf.timestamp << 8 | pRst->datetime[1];
    rstBuf.timestamp = rstBuf.timestamp << 8 | pRst->datetime[2];
    rstBuf.timestamp = rstBuf.timestamp << 8 | pRst->datetime[3];
    rstBuf.size = pRst->validLen;
    // 调用回调
    if(pRcvData != nullptr){
        pRcvData(&rstBuf, gPUser);
        writeLog("handleHidDataRcv size: " + std::to_string(rstBuf.size));
    }
}

void canRead()
{
    if(gPhidHandle == nullptr){ return; }
    try {
        unsigned char rst[sizeof(CanReadAck)]{0};
        if(hid_read(gPhidHandle, rst, sizeof(CanReadAck)) <= 0){
            return;
        }
        handleHidDataRcv(rst, sizeof(CanReadAck));
    } catch (...) {
        return;
    }
}

bool setFilterId(uchar filterCount, uchar index, uchar filterType, unsigned int startId, unsigned int endId)
{
    try {
        if(filterCount > 10 || index > 10){ return false; }
        if(filterType > 0x03){ return false; }
        HidFilterIdREQ req;
        req.header[0] = 0x2E; req.header[1] = 0x30;
        req.len = static_cast<uchar>(sizeof(HidFilterIdREQ)-2);
        req.cmd = 0xF1;
        req.filterCount = filterCount;
        req.index = index;
        req.frameType = filterType;
        req.startId[0] = (startId & 0xFF000000) >> 24;
        req.startId[1] = (startId & 0x00FF0000) >> 16;
        req.startId[2] = (startId & 0x0000FF00) >> 8;
        req.startId[3] = (startId & 0x000000FF);
        req.endId[0] = (endId & 0xFF000000) >> 24;
        req.endId[1] = (endId & 0x00FF0000) >> 16;
        req.endId[2] = (endId & 0x0000FF00) >> 8;
        req.endId[3] = (endId & 0x000000FF);
        req.checkSum = checkSum(reinterpret_cast<uchar*>(&req)+2, sizeof(HidFilterIdREQ)-3);
        uchar *pReq = reinterpret_cast<uchar*>(&req);
        if(0 >= handleHidWrite(pReq, sizeof(HidFilterIdREQ))){
            return false;
        }
        unsigned char rst[sizeof(GeneralPkgReAck)]{0};
        if(0 >= handleHidRead(rst, sizeof(GeneralPkgReAck))){
            return false;
        }
        GeneralPkgReAck *pRst = reinterpret_cast<GeneralPkgReAck*>(rst);
        uchar check = checkSum(rst+2, sizeof(GeneralPkgReAck)-3);
        if(check != pRst->checkSum || pRst->cmd != 0x0E || pRst->re == 0x00){
            return false;
        }
        return true;
    } catch (...) {
        return false;
    }
}

bool clearFilterId()
{
    try {
        HidFilterIdREQ req;
        req.header[0] = 0x2E; req.header[1] = 0x30;
        req.len = static_cast<uchar>(sizeof(HidFilterIdREQ)-2);
        req.cmd = 0xF1;
        req.filterCount = 0;
        req.index = 0;
        req.frameType = 0;
        req.startId[0] = 0;
        req.startId[1] = 0;
        req.startId[2] = 0;
        req.startId[3] = 0;
        req.endId[0] = 0;
        req.endId[1] = 0;
        req.endId[2] = 0;
        req.endId[3] = 0;
        req.checkSum = checkSum(reinterpret_cast<uchar*>(&req)+2, sizeof(HidFilterIdREQ)-3);
        uchar *pReq = reinterpret_cast<uchar*>(&req);
        if(0 >= handleHidWrite(pReq, sizeof(HidFilterIdREQ))){
            return false;
        }
        unsigned char rst[sizeof(GeneralPkgReAck)]{0};
        if(0 >= handleHidRead(rst, sizeof(GeneralPkgReAck))){
            return false;
        }
        GeneralPkgReAck *pRst = reinterpret_cast<GeneralPkgReAck*>(rst);
        uchar check = checkSum(rst+2, sizeof(GeneralPkgReAck)-3);
        if(check != pRst->checkSum || pRst->cmd != 0x0E || pRst->re == 0x00){
            return false;
        }
        return true;
    } catch (...) {
        return false;
    }
}

bool canParameter(uchar *baudrate, char *filter, size_t size)
{
    try {
        GeneralPkgReq req;
        req.header[0] = 0x2E; req.header[1] = 0x30;
        req.len = static_cast<uchar>(sizeof(GeneralPkgReq)-2);
        req.cmd = 0xF0;
        req.checkSum = checkSum(reinterpret_cast<uchar*>(&req)+2, sizeof(GeneralPkgReq)-3);
        uchar *pReq = reinterpret_cast<uchar*>(&req);
        if(0 >= handleHidWrite(pReq, sizeof(GeneralPkgReq))){
            return false;
        }
        int filterSum = 10, currentIndex = 1;
        string filterInfos;
        while (true) {
            if(currentIndex >= filterSum){
                break;
            }
            uchar rst[sizeof(HidParAck)]{0};
            if(0 >= handleHidRead(rst, sizeof(HidParAck))){
                return false;
            }
            HidParAck *pRst = reinterpret_cast<HidParAck *>(rst);
            uchar check = checkSum(rst+2, sizeof(HidParAck)-3);
            if(check != pRst->checkSum || pRst->cmd != 0x0F){
                return false;
            }
            *baudrate = pRst->baute;
            filterInfos += std::to_string(pRst->frameType) + ",";
            unsigned int start, end;
            start = pRst->startId[0];
            start = start << 8 | pRst->startId[1];
            start = start << 8 | pRst->startId[2];
            start = start << 8 | pRst->startId[3];
            end = pRst->endId[0];
            end = end << 8 | pRst->endId[1];
            end = end << 8 | pRst->endId[2];
            end = end << 8 | pRst->endId[3];
            filterInfos += std::to_string(start) + ",";
            filterInfos += std::to_string(end) + ";";
            filterSum = pRst->filterCount;
            currentIndex = pRst->index;
            string tmp;
            for (size_t i=0; i<sizeof(HidParAck); i++) {
                char ch[10];
                sprintf(ch, " %02X", rst[i]);
                tmp += string(ch);
            }
            writeLog("canParameter HEX: " + tmp);
        }
        writeLog("canParameter: " + filterInfos);
        memcpy(filter, filterInfos.data(), filterInfos.length() > size ? size : filterInfos.length());
        return true;
    } catch (...) {
        return false;
    }
}

bool canSetSerialPortTransitionPar(TransiteParameter par)
{
    try {
        TransitionParamREQCom req;
        req.header[0] = 0x2E; req.header[1] = 0x30;
        req.len = sizeof(TransitionParamREQCom) - 2;
        req.cmd = 0xEF;
        req.transitionModel = par.model;
        req.transitionDirecte = par.directe;
        req.enableSerial = par.enableSerial;
        req.enableID = par.enableId;
        req.serialOffset = par.offset;
        req.serialFramLen = par.frameLen;
        req.serialTimes = par.serialTimes;
        req.transitionTimes = par.transiteTimes;
        uchar *pReq = reinterpret_cast<uchar *>(&req);
        pReq[sizeof(TransitionParamREQCom)-1] = checkSum(pReq+2, sizeof(TransitionParamREQCom)-3);
        int re = handleHidWrite(pReq, sizeof(TransitionParamREQCom));
        if(0 >= re){
            return false;
        }
        uchar readbuf[sizeof(GeneralPkgReAck)]{0};
        re = handleHidRead(readbuf, sizeof(GeneralPkgReAck));
        if(0 >= re){
            return false;
        }
        GeneralPkgReAck *pRead = reinterpret_cast<GeneralPkgReAck *>(readbuf);
        uchar rCheckSum = checkSum(readbuf+2, static_cast<int>(re-3));
        if(rCheckSum != pRead->checkSum || pRead->cmd != 0x10 || pRead->re == 0x00){
            return false;
        }
        return true;
    } catch (...) {
        return false;
    }
}

bool canSetSerialPortPar(SerialPortParameter par)
{
    try {
        USARTParamREQCom req;
        req.header[0] = 0x2E; req.header[1] = 0x30;
        req.len = sizeof(USARTParamREQCom) - 2;
        req.cmd = 0xEE;
        req.bauderate = par.baudrate;
        req.dataBit = par.databit;
        req.parityBit = par.paritybit;
        req.stopBit = par.stopbit;
        req.flowControl = par.flow;
        req.serailModel = par.model;
        uchar *pReq = reinterpret_cast<uchar *>(&req);
        pReq[sizeof(USARTParamREQCom)-1] = checkSum(pReq+2, sizeof(USARTParamREQCom)-3);
        int re = handleHidWrite(pReq, sizeof(USARTParamREQCom));
        if(0 >= re){
            return false;
        }
        uchar readbuf[sizeof(GeneralPkgReAck)]{0};
        re = handleHidRead(readbuf, sizeof(GeneralPkgReAck));
        if(0 >= re){
            return false;
        }
        GeneralPkgReAck  *pRead = reinterpret_cast<GeneralPkgReAck *>(readbuf);
        uchar rCheckSum = checkSum(readbuf+2, static_cast<int>(re-3));
        if(rCheckSum != pRead->checkSum || pRead->cmd != 0x11 || pRead->re == 0x00){
            return false;
        }
        return true;
    } catch (...) {
        return false;
    }
}

bool canSetSerialPortCanPar(uchar baudrate, unsigned int id, uchar frameFormat)
{
    try {
        CANParamREQCom req;
        req.header[0] = 0x2E; req.header[1] = 0x30;
        req.len = sizeof(CANParamREQCom) - 2;
        req.cmd = 0xEB;
        req.bauterate = baudrate;
        req.canID[0] = (id & 0xFF000000) >> 24;
        req.canID[1] = (id & 0x00FF0000) >> 16;
        req.canID[2] = (id & 0x0000FF00) >> 8;
        req.canID[3] = (id & 0x000000FF);
        req.frameFormat = frameFormat;
        uchar *pReq = reinterpret_cast<uchar *>(&req);
        pReq[sizeof(CANParamREQCom)-1] = checkSum(pReq+2, sizeof(CANParamREQCom)-3);
        int re = handleHidWrite(pReq, sizeof(CANParamREQCom));
        if(0 >= re){
            return false;
        }
        uchar readbuf[sizeof(GeneralPkgReAck)]{0};
        re = handleHidRead(readbuf, sizeof(GeneralPkgReAck));
        if(0 >= re){
            return false;
        }
        GeneralPkgReAck *pRead = reinterpret_cast<GeneralPkgReAck *>(readbuf);
        uchar rCheckSum = checkSum(readbuf+2, static_cast<int>(re-3));
        if(rCheckSum != pRead->checkSum || pRead->cmd != 0x14 || pRead->re == 0x00){
            return false;
        }
        return true;
    } catch (...) {
        return false;
    }
}

bool canSerialPortPar(TransiteParameter *transitePar, SerialPortParameter *serialPar)
{
    try {
        GeneralPkgReq req;
        req.header[0] = 0x2E; req.header[1] = 0x30;
        req.len = sizeof(GeneralPkgReq) - 2;
        req.cmd = 0xED;
        uchar *pReq = reinterpret_cast<uchar *>(&req);
        pReq[sizeof(GeneralPkgReq)-1] = checkSum(pReq+2, sizeof(GeneralPkgReq)-3);
        int re = handleHidWrite(pReq, sizeof(CANParamREQCom));
        if(0 >= re){
            return false;
        }
        uchar readbuf[sizeof(USARTComfigACKSerial)]{0};
        re = handleHidRead(readbuf, sizeof(USARTComfigACKSerial));
        if(0 >= re){
            return false;
        }
        uchar rCheckSum = checkSum(readbuf+2, static_cast<int>(re-3));
        if(rCheckSum != readbuf[re-1] || readbuf[3] != 0x12){
            return false;
        }
        // 解析数据
        USARTComfigACKSerial *pConfig = reinterpret_cast<USARTComfigACKSerial*>(readbuf);
        transitePar->model = pConfig->transitionModel;
        transitePar->directe = pConfig->transitionDirecte;
        transitePar->enableSerial = pConfig->enableSerial;
        transitePar->enableId = pConfig->enableID;
        transitePar->offset = pConfig->serialOffset;
        transitePar->frameLen = pConfig->serialFramLen;
        transitePar->serialTimes = pConfig->serialTimes;
        transitePar->transiteTimes = pConfig->transitionTimes;
        serialPar->baudrate = pConfig->bauderate;
        serialPar->databit = pConfig->dataBit;
        serialPar->paritybit = pConfig->parityBit;
        serialPar->stopbit = pConfig->stopBit;
        serialPar->flow = pConfig->flowControl;
        serialPar->model = pConfig->serailModel;
        return true;
    } catch (...) {
        return false;
    }
}

bool canSerialPortCanPar(uchar *canBaudrate, unsigned int *canId, uchar *frameFormat,
                         char *filter, size_t size)
{
    try {
        GeneralPkgAck req;
        req.header[0] = 0x2E; req.header[1] = 0x30;
        req.len = sizeof(GeneralPkgAck) - 2;
        req.cmd = 0xEC;
        uchar *pReq = reinterpret_cast<uchar *>(&req);
        pReq[sizeof(GeneralPkgAck)-1] = checkSum(pReq+2, sizeof(GeneralPkgAck)-3);
        int re = handleHidWrite(pReq, sizeof(GeneralPkgAck));
        if(0 >= re){
            writeLog("canSerialPortCanPar write re: " + std::to_string(re));
            return false;
        }
        // 解析数据
        string filterInfos;
        int filterSum = 10, currentIndex = 1;
        while (true) {
            if(currentIndex >= filterSum){
                break;
            }
            uchar readbuf[sizeof(USARTComfigACKCan)]{0};
            re = handleHidRead(readbuf, sizeof(USARTComfigACKCan));
            if(0 >= re){
                writeLog("canSerialPortCanPar handleHidRead re: " + std::to_string(re));
                return false;
            }
            uchar rCheckSum = checkSum(readbuf+2, static_cast<int>(re-3));
            if(rCheckSum != readbuf[re-1] || readbuf[3] != 0x13){
                writeLog("canSerialPortCanPar check error");
                string tmp;
                for (size_t i=0; i<sizeof(HidParAck); i++) {
                    char ch[10];
                    sprintf(ch, " %02X", readbuf[i]);
                    tmp += string(ch);
                }
                writeLog("canSerialPortCanPar HEX: " + tmp);
                writeLog("canSerialPortCanPar rCheckSum: " + std::to_string(rCheckSum));
                return false;
            }
            USARTComfigACKCan *pRst = reinterpret_cast<USARTComfigACKCan*>(readbuf);
            *canBaudrate = pRst->canBauderate;
            *canId = pRst->canID[0];
            *canId = (*canId) << 8 | pRst->canID[1];
            *canId = (*canId) << 8 | pRst->canID[2];
            *canId = (*canId) << 8 | pRst->canID[3];
            *frameFormat  = pRst->frameFormat;
            filterInfos += std::to_string(pRst->frameType) + ",";
            unsigned int start, end;
            start = pRst->startId[0];
            start = start << 8 | pRst->startId[1];
            start = start << 8 | pRst->startId[2];
            start = start << 8 | pRst->startId[3];
            end = pRst->endId[0];
            end = end << 8 | pRst->endId[1];
            end = end << 8 | pRst->endId[2];
            end = end << 8 | pRst->endId[3];
            filterInfos += std::to_string(start) + ",";
            filterInfos += std::to_string(end) + ";";

            filterSum = pRst->filterCount;
            currentIndex = pRst->index;
        }
        memcpy(filter, filterInfos.data(), filterInfos.length() > size ? size : filterInfos.length());
        return true;
    }catch (...) {
        writeLog("canSerialPortCanPar exception");
        return false;
    }
}

/*
 * 隐藏接口, 发送升级请求
 * @fileSize: 升级包 BIN 文件大小， 4 个字节高字节在前，低字节在后
 * @return:
 *      1: 请求成功
 *      0: 发生未知错误
 *     -1: 升级请求发送失败
 *     -2: 升级请求无响应
 *     -3: 升级请求反馈校验失败
 */
int upgradeReq(size_t fileSize)
{
    try {
        unsigned char update_req[] = {0x2E, 0x30, 0x07, 0xF3, 0x00, 0x00, 0x00, 0x00, 0x00};
        unsigned int fileLen = fileSize;
        update_req[4] = (fileLen & 0xFF000000) >> 24;
        update_req[5] = (fileLen & 0x00FF0000) >> 16;
        update_req[6] = (fileLen & 0x0000FF00) >> 8;
        update_req[7] = fileLen & 0x000000FF;
        update_req[8] = checkSum(update_req + 2, 6);
        if(0 >= handleHidWrite(update_req, sizeof(update_req)/sizeof(update_req[0]))){
            return -1;
        }

        unsigned char rst[5]{0};
        if(0 >= hid_read_timeout(gPhidHandle, rst, 5, 500)){
            return -2;
        }
        unsigned char rstCmp[]{0x2E, 0x30, 0x03, 0x0C, 0x0F};
        rstCmp[sizeof(rstCmp)/sizeof(rstCmp[0])-1] = checkSum(rstCmp+2, rstCmp[2]-1);
        for(size_t i = 0; i<sizeof(rstCmp)/sizeof(rstCmp[0]); ++i){
            // 升级失败
            if(rst[i] != rstCmp[i]){
                return -3;
            }
        }
        return 1;
    } catch (...) {
        return 0;
    }
}

/*
 * 发送升级数据
 * @index: 流水序号
 * @data: 文件数据，最多16个字节
 * @size: data的大小
 * @return:
 *      1: 发送成功
 *      0: 发生未知错误
 *     -4: 升级数据发送失败
 *     -5: 升级数据反馈超时
 *     -6: 升级数据反馈校验错误
 */
int upgradeDataReq(unsigned int index, uchar *data, size_t size)
{
    try {
        HidIapDataREQ req;
        req.header[0] = 0x2E;
        req.header[1] = 0x30;
        req.len = sizeof(HidIapDataREQ) - 2;
        req.cmd = 0xF2;
        // 包序号
        req.id[0] = (index & 0xFF000000) >> 24;
        req.id[1] = (index & 0x00FF0000) >> 16;
        req.id[2] = (index & 0x0000FF00) >> 8;
        req.id[3] = index & 0x000000FF;
        // 有效数据包长度
        req.pkgLen = static_cast<uchar>(size);
        // 数据
        memcpy(req.data, data, size);
        // 校验
        req.checkSum = checkSum(reinterpret_cast<uchar*>(&req)+ 2, sizeof(HidIapDataREQ)-3);

        // 发送
        uchar *pReq = reinterpret_cast<uchar *>(&req);
        if(0 >= handleHidWrite(pReq, sizeof(HidIapDataREQ))){
            return -4;
        }
        unsigned char rst[5]{0};
        if(0 >= hid_read_timeout(gPhidHandle, rst, 5, 500)){
            return -5;
        }
        unsigned char rstCmp[]{0x2E, 0x30, 0x03, 0x0D, 0x00};
        rstCmp[sizeof(rstCmp)/sizeof(rstCmp[0])-1] = checkSum(rstCmp+2, rstCmp[2]-1);
        for(size_t i = 0; i<sizeof(rstCmp)/sizeof(rstCmp[0]); ++i){
            // 升级失败
            if(rst[i] != rstCmp[i]){
                return -6;
            }
        }
        return 1;
    } catch (...) {
        return 0;
    }
}

/*
 * 接收升级状态, 在发送完文件后立即调用
 * @return:
 *      1: 升级成功
 *      0: 发送未知错误
 *     -7: 升级文件状态校验错误
 *     -8: 升级文件状态确认结果发送失败
 */
int upgradeStatus()
{
    try {
        unsigned char rstFile[6]{0x00};
        if(0 >= hid_read_timeout(gPhidHandle, rstFile, 6, 500)){ return false; }
        unsigned char rstCmpFile[]{0x2E, 0x30, 0x04, 0x15, 0x01, 0x00};
        rstCmpFile[sizeof(rstCmpFile)/sizeof(rstCmpFile[0])-1] = checkSum(rstCmpFile+2, rstCmpFile[2]-1);
        for(size_t i = 0; i<sizeof(rstCmpFile)/sizeof(rstCmpFile[0]); ++i){
            if(rstFile[i] != rstCmpFile[i]){
                return -7;
            }
        }
        unsigned char cmd[5] = {0x2E, 0x30, 0x04, 0xEA, 0x00};
        cmd[4] = checkSum(cmd + 2, 3);
        if(0 >= handleHidWrite(cmd, sizeof(cmd)/sizeof(cmd[0]))){
            return  -8;
        }
        return 1;
    } catch (...) {
        return 0;
    }
}






