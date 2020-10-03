#ifndef UT_8251A_LIB_H
#define UT_8251A_LIB_H

typedef unsigned int size_t;
typedef unsigned char uchar;
#include "hidapi.h"

struct CanRcvData{
    /* id: 读取数据的ID号, 共四个字节(标准帧高位填0), 高字节在前
     * frameType: 帧类型, 0x00: 数据帧, 0x01: 远程帧
     * frameFormat: 帧格式, 0x00: 标准帧, 0x01: 扩展帧
     * timestamp: CAN 总线数据接收时间戳，单位毫秒 共四个字节， 高字节在前，低字节在后
     * data: 读取的数据, 最多8个字节
     * size: data的大小, 作为传出时，表示读取的字节大小
     */
    unsigned int id;
    uchar frameType;
    uchar frameFormat;
    unsigned int timestamp;
    uchar data[8];
    size_t size;
};

struct TransiteParameter{
    /*
    * model: 设置转换模式, 模式数据如下:
    *          0x00: 透明转换
    *          0x01: 透明带标识转换
    *          0x02: 格式转换
    *          0x03: MODBUS 转换
    * direte: 设置转换方向, 如下:
    *          0x00: 双向转换
    *          0x01: 仅串口转换到 CAN
    *          0x02: 仅 CAN 转换到串口
    * enableSerial: CAN 帧信息转发设置。仅在透明转换模式下有效，其它模
    *          式时该字节填 0 处理。
    *          0x00: 不允许 CAN 帧信息转发到串行帧中
    *          0x01: 允许 CAN 帧信息转发到串行帧中
    * enableId: CAN 帧 ID 转发设置。仅在透明转换模式下有效，其它模式时该字节填 0 处理。
    *          0x00: 不允许 CAN 帧 ID 转发到串行帧中
    *          0x01: 允许 CAN 帧 ID 转发到串行帧中
    * offset: 设置 CAN 标识符在串行帧中的起始偏移。仅在透明带标
    *          识转换模式下有效，其它模式时该字节填 0 处理。 有效值： 0~7
    * frameLen: 设置 CAN 标识符在串行帧中的长度。仅在透明带标识转
    *            换模式下有效，其它模式时该字节填 0 处理。 有效值： 1~4
    * serialTimes: 设置串行帧之间的时间间隔，单位 ms 毫秒。仅在透明转
    *               换和透明带标识转换模式下有效，其它模式时该字节填 0 处理。 有效值 0~250ms
    * transiteTimes: 设置 CAN 帧转发的时间间隔，单位 ms 毫秒。仅在透明转
    *                 换和透明带标识转换模式下有效，其它模式时该字节填 0 处理。 有效值 0~250ms
    */
    uchar model;
    uchar directe;
    uchar enableSerial;
    uchar enableId;
    uchar offset;
    uchar frameLen;
    uchar serialTimes;
    uchar transiteTimes;
};

struct SerialPortParameter{
    /*
    * baudrate: 设置串口波特率, 值区间[0x00, 0x0D]
    *             0x00: 600bps, 0x01: 1200bps, 0x02: 2400bps, 0x03: 4800bps 0x04: 9600bps
    *             0x05: 14400bps,  0x06: 19200bps, 0x07: 28800bps, 0x08: 38400bps, 0x09: 56000bps
    *             0x0A: 57600bps,  0x0B: 115200bps, 0x0C: 128000bps, 0x0D: 230400bps
    * databit: 设置数据位 值为: 0x00: 5, 0x01: 6, 0x02: 7, 0x03: 8
    * paritybit: 设置校验位, 值为 0x00: 无校验, 0x01: 奇校验, 0x02: 偶校验, 0x03: 强制为 0 SPACE ,0x04: 强制为 1 MARK
    * stopbit: 设置停止位 0x00: 1 位 0x01: 1.5 位 0x02: 2 位
    * flow: 设置流控制 0x00: 硬件流控制 0x01: 无流控制
    * model: 设置串口模式 0x00: RS232 模式 0x01: RS485 模式 0x02: RS422 模式
    */
    uchar baudrate;
    uchar databit;
    uchar paritybit;
    uchar stopbit;
    uchar flow;
    uchar model;
};

struct HidInfo{
    /** Platform-specific device path */
    char path[256];
    /** Device Vendor ID */
    unsigned short vendor_id;
    /** Device Product ID */
    unsigned short product_id;
    /** Manufacturer String */
    wchar_t manufacturer_string[256];
    /** Product string */
    wchar_t product_string[256];
};

/*
 * 读取数据, 一次最多读取8个字节
 */
typedef void (*CanDataFunc)(CanRcvData *pData, void *pUser);

void setCallBack(CanDataFunc pFunc, void *pUser);

/*
 * 初始化动态库, 此接口应该在所有接口前调用
 * @return: true 执行成功, 反之失败
 */
bool initLib();

/*
 * 释放动态库资源, 此接口应该在关闭此库前调用
 */
void freeLib();

/*
 * 枚举设备,返回设备信息单链表
 */
hid_device_info *enumHidDevs();

/*
 * 释放枚举的单链表, dev 为 enmuHidDevs 返回值
 */
void freeEnumHidDevs(hid_device_info *dev);

/*
 * 打开USB HID设备
 * @path: scanHid 返回的PATH字符串
 * @size: path的大小, 调用者自行保证内存大小
 * @return: true 执行成功, 反之失败
 */
bool hidOpen(char *path, size_t size);

/*
 * 通过vid, pid, serial_number 来打开设备,
 * vid, pid, serial_number都在enumHidDevs结果中
 * @return: true 执行成功, 反之失败
 */
bool hidOpenSerial(unsigned short vid, unsigned short pid, char *serial_number, size_t size);

/*
 * 关闭USB HID设备
 */
void hidClose();

/*
 * 判断USB HID设备是否在线, 在线 true, 反之 false
 */
bool isAliveHid();

/*
 * 打开 USB HID 连接的 CAN 设备
 * @return: true 执行成功, 反之失败
 */
bool canOpen();

/*
 * 关闭 USB HID 连接的 CAN 设备
 * @return: true 执行成功, 反之失败
 */
bool canClose();

/*
 * 初始化CAN设备, 并获取版本
 * @version: CAN设备的软件版本, 包含主版本和次版本号, V01.01
 * @size: version的大小，至少7个字节
 * @return: true 执行成功, 反之失败
 */
bool canInit(char *version, size_t size);

/*
 * CAN设备的接收使能
 * @enable: true 打开接收, 反之，关闭接收
 * @return: true 执行成功, 反之失败
 */
bool canRcvEnable(bool enable);

/*
 * 设置 CAN 设备工作模式
 * @model: 工作模式，0x00: 正常模式, 0x01: 自测模式
 * @return: true 执行成功, 反之失败
 */
bool canSetModel(unsigned char model);

/*
 * 复位 CAN 设备
 * @return: true 执行成功, 反之失败
 */
bool canReset();

/*
 * 清除CAN设备错误状态
 * @return: true 执行成功, 反之失败
 */
bool canResetError();

/*
 * 设置CAN设备的波特率
 * @baudrate: CAN 波特率, 目前支持的波特率为， baudrate和波特率值如下:
 *      0x00: "5K", 0x01: "10K", 0x02: "20K", 0x03: "50K",
 *      0x04: "100K", 0x05: "125K", 0x06: "250K", 0x07: "500K",
 *      0x08: "800K", 0x09: "1000K";
 * 即传递数组的索引: {"5K", "10K", "20K", "50K", "100K", "125K", "250K", "500K", "800K", "1000K"};
 * @return: true 执行成功, 反之失败
 */
bool canSetBaudRate(uchar baudrate);

/*
 * 发送数据, 一次最多发送8个字节
 * @id: ID号, 共四个字节(标准帧高位填0), 高字节在前
 * @frameType: 帧类型, 0x00: 数据帧, 0x01: 远程帧
 * @frameFormat: 帧格式, 0x00: 标准帧, 0x01: 扩展帧
 * @data: 要发送的数据, 最多8个字节，调用者自行分包
 * @size: data的长度，最大8
 * @return: 发送的字节数，错误返回 -1
 */
int canWrite(unsigned int id, uchar frameType, uchar frameFormat, uchar *data, uchar size);

/*
 * 读取CAN数据, 接口内部会调用回调
 * 如果回调为空则，则直接丢弃读取的数据.
 * 需要读取CAN设备数据，需要调用此接口。
 * 本库没有处理线程安全问题，调用者自行处理线程安全.
 * 建议在单独线程循环调用此接口.
 */
void canRead();

/*
 * 设置滤波规则, 如果有多组，需要多次调用
 * @filterCount: 滤波规则总组数, 最大10组
 * @index: 当前滤波规则在总规则里面的索引, 范围[1, 10], 左右闭区间
 * @filterType: 滤波类型, [0x00, 0x03]
 *      0x00: 标准帧单ID滤波,
 *      0x01: 标准帧组ID滤波,
 *      0x02: 扩展帧单ID滤波,
 *      0x03: 扩展帧组ID滤波
 * @startId: 起始ID, 共四个字节(标准帧高位填0), 高字节在前
 * @endId: 结束ID, 共四个字节(标准帧高位填0), 高字节在前
 * @return: true 执行成功, 反之失败
 */
bool setFilterId(uchar filterCount, uchar index, uchar filterType,
                 unsigned int startId, unsigned int endId);

/*
 * 清空滤波规则
 * @return: true 执行成功, 反之失败
 */
bool clearFilterId();

/*
 * 获取CAN参数，包括波特率和滤波规则
 * OUT:
 * @baudrate: CAN设备波特率
 *      0x00: "5K", 0x01: "10K", 0x02: "20K", 0x03: "50K",
 *      0x04: "100K", 0x05: "125K", 0x06: "250K", 0x07: "500K",
 *      0x08: "800K", 0x09: "1000K";
 * @filter: CAN滤波规则, 格式如下:
 *     滤波类型, 起始ID, 结束ID;
 *     1, 1001, 1010;
 *     2, 2001, 2020;
 *     其中滤波类型[0x00, 0x03]
 *         0x00: 标准帧单ID滤波,
 *         0x01: 标准帧组ID滤波,
 *         0x02: 扩展帧单ID滤波,
 *         0x03: 扩展帧组ID滤波
 *     字段都为十进制的数字转成的字符串, 字段用英文逗号(',')分割,
 *     每一组用英文分号(';')分割, 没有换行符;
 * @size: filter的总大小，调用者保证足够的大小
 * @return: true 执行成功, 反之失败
 */
bool canParameter(uchar *baudrate, char *filter, size_t size);

/*
 * 设置CAN设备串口转换参数
 * @par: 转换参数
 * @return: true 执行成功, 反之失败
 */
bool canSetSerialPortTransitionPar(TransiteParameter par);

/*
 * 设置CAN设备串口参数
 * @par: 串口参数
 * @return: true 执行成功, 反之失败
 */
bool canSetSerialPortPar(SerialPortParameter par);

/*
 * 设置串口端的 CAN 参数
 * @baudrate: CAN 波特率, 目前支持的波特率为， baudrate和波特率值如下:
 *      0x00: "5K", 0x01: "10K", 0x02: "20K", 0x03: "50K",
 *      0x04: "100K", 0x05: "125K", 0x06: "250K", 0x07: "500K",
 *      0x08: "800K", 0x09: "1000K";
 * @id: 设置串口端的 CAN 帧 ID，占四个字节，高字节在前，低字节在后 （标准帧高位填 0 补齐四字节）
 * @frameFormat: 设置串口端的帧格式 0x00：标准帧 0x01：扩展帧
 * @return: true 执行成功, 反之失败
 */
bool canSetSerialPortCanPar(uchar baudrate, unsigned int id, uchar frameFormat);

/*
 * 读取CAN设备串口当前配置
 * @transitePar: 转换参数
 * @serialPar: 串口参数
 * @return: true 执行成功, 反之失败
 */
bool canSerialPortPar(TransiteParameter *transitePar, SerialPortParameter *serialPar);

/*
 * 读取串口端 CAN 参数
 * @canBaudrate: CAN 波特率, 目前支持的波特率为， baudrate和波特率值如下:
 *      0x00: "5K", 0x01: "10K", 0x02: "20K", 0x03: "50K",
 *      0x04: "100K", 0x05: "125K", 0x06: "250K", 0x07: "500K",
 *      0x08: "800K", 0x09: "1000K";
 * @canId: ID号, 共四个字节(标准帧高位填0), 高字节在前
 * @frameFormat: 串口端的 CAN 帧格式 0x00：标准帧 0x01：扩展帧
 * @filter: CAN滤波规则, 格式如下:
 *     滤波类型, 起始ID, 结束ID;
 *     1, 1001, 1010;
 *     2, 2001, 2020;
 *     其中滤波类型[0x00, 0x03]
 *         0x00: 标准帧单ID滤波,
 *         0x01: 标准帧组ID滤波,
 *         0x02: 扩展帧单ID滤波,
 *         0x03: 扩展帧组ID滤波
 *     字段都为十进制的数字转成的字符串, 字段用英文逗号(',')分割,
 *     每一组用英文分号(';')分割, 没有换行符;
 * @size: filter的总大小，调用者保证足够的大小
 * @return: true 执行成功, 反之失败
 */
bool canSerialPortCanPar(uchar *canBaudrate, unsigned int *canId,
                         uchar *frameFormat, char *filter, size_t size);


#endif // UT_8251A_LIB_H
