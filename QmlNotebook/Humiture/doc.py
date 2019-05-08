"""UT-2506 CanBus设置软件"""
import struct
import sys
import os
import time
import configparser

import serial
import serial.tools.list_ports_windows as coms
import modbus_tk
import modbus_tk.defines as cst
from modbus_tk import modbus_rtu

from PyQt5 import QtWidgets, QtGui, QtCore
from PyQt5.QtWidgets import QMessageBox

from ui_main import Ui_MainWindow
from crc import usMBCRC16


BAUDRATE = [
    '1200', '2400', '4800', '9600', '19200',
    '38400', '57600', '115200',
]

BYTESIZE_KEY = list('5678')
BYTESIZE = {
    '5': serial.FIVEBITS,
    '6': serial.SIXBITS,
    '7': serial.SEVENBITS,
    '8': serial.EIGHTBITS,
}

PARITY_KEY = ['无校验', '偶校验', '奇校验', '标记', '空格']
PARITY = {
    '无校验':serial.PARITY_NONE,
    '偶校验':serial.PARITY_EVEN,
    '奇校验':serial.PARITY_ODD,
    '标记':serial.PARITY_MARK,
    '空格':serial.PARITY_SPACE
}

STOPBITS_KEY = ['1', '1.5', '2']
STOPBITS = {
    '1': serial.STOPBITS_ONE,
    '1.5': serial.STOPBITS_ONE_POINT_FIVE,
    '2': serial.STOPBITS_TWO,
}


class UpdateComThread(QtCore.QThread):
    """
    主要用来更新串口列表的，以达到有设备接入后能实时
    更新串口，该任务主要是间隔时间去访问pyserial的
    接口，但这个是耗时工作，无法放在定时器中完成
    """
    coms_signal = QtCore.pyqtSignal(str)
    coms_buf = list()

    def __init__(self, coms: str):
        super(UpdateComThread, self).__init__()
        self.coms_buf.extend(eval(coms))
        self._exit = False
        self._sleep = 500

    def exit_thread(self):
        """退出run中的while循环"""
        self._exit = True

    def run(self):
        while not self._exit:
            # 比较前后是否变化
            com_list = [str(com[0]) for com in coms.comports()]
            com_list = sorted(com_list)
            if self.coms_buf != com_list:
                self.coms_signal.emit(str(com_list))
                self.coms_buf.clear()
                self.coms_buf.extend(com_list)
            QtCore.QThread.msleep(self._sleep)


class ScanModbusThread(QtCore.QThread):
    """
    主要用来查找识别设备
    """
    # 扫描的结果，波特率和设备地址
    device_signal = QtCore.pyqtSignal(int, int)
    status_signal = QtCore.pyqtSignal(str)
    com = ''

    def __init__(self, com):
        super(ScanModbusThread, self).__init__()
        self._exit = False
        self._sleep = 300
        self.com = com

    def exit_thread(self):
        """退出run中的while循环"""
        self._exit = True

    def run(self):
        self.status_signal.emit('Scan thread started....')
        try:
            cmd = '00 A1 13 88 00 02 B8 AD'
            buf = list(map(lambda x: int(x, 16), cmd.split(' ')))
            self._ser = serial.Serial()
            self._ser.port = self.com
            self._ser.open()
        except Exception as err:
            self.status_signal.emit(str(err))
            return

        for baud in BAUDRATE:
            self.status_signal.emit('Scan %s baudrate' % baud)
            try:
                self._ser.baudrate = baud
                self._ser.bytesize = serial.EIGHTBITS
                self._ser.parity = serial.PARITY_NONE
                self._ser.stopbits = serial.STOPBITS_ONE
            except Exception as err:
                self.status_signal.emit(str(err))
                continue
            else:
                # 请求波特率和设备地址
                if self._ser.isOpen():
                    self._ser.write(buf)
                    QtCore.QThread.msleep(self._sleep)
                    # 读取回应
                    count = self._ser.inWaiting()
                    if count >= 9:
                        data = self._ser.read(count)
                        # print('data: ', data)
                        uscheck = usMBCRC16(data[:-2])
                        check = (data[-1] << 8) | data[-2]
                        if uscheck == check:
                            # 01 03 04 00 01 00 03 EB F2 中
                            # 00 01 是地址，00 03 是波特率表的序号
                            addr = (data[3] << 8) | data[4]
                            baudrate = (data[5] << 8) | data[6]
                            self.device_signal.emit(baudrate, addr)
                            # 扫描到了就停止
                            break
        # 关闭串口
        if self._ser.isOpen():
            self._ser.close()
        self.status_signal.emit('完成扫描设备')


class MainWindow(QtWidgets.QMainWindow, Ui_MainWindow):
    """主窗口"""

    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        self.setupUi(self)
        self.resize(625, 525)
        self._ser = serial.Serial()
        # 默认从机地址为 0x01
        self._addr = 0x01
        # 循环读取通道值的定时器
        self._timer = QtCore.QTimer()

        self._init_ui()
        # Tip: 这里需要注意先后顺序
        self._update_com_thread = UpdateComThread(str(self._com_list))
        self._init_connect()
        # self._com_list 在 _init_ui 中赋值
        self._update_com_thread.start()

    def closeEvent(self, event):
        """关闭事件"""
        self._update_com_thread.exit_thread()
        self._update_com_thread.quit()
        self._update_com_thread.wait(1000)
        if self._ser.isOpen():
            self._ser.close()
        event.accept()

    def _init_ui(self):
        self.setWindowIcon(QtGui.QIcon('./ico.png'))
        self.device_addr = ['0x%X' % i for i in range(1, 248)]
        self.comboBox_device_addr.addItems(self.device_addr)
        self.comboBox_device_baudrate.addItems(BAUDRATE)

        for i in range(1, 9):
            model = self.findChild(QtWidgets.QComboBox, 'comboBox_m%d' % i)
            status = self.findChild(QtWidgets.QComboBox, 'comboBox_s%d' % i)
            model.addItems(['电压模式', '电流模式'])
            status.addItems(['关', '开'])
            model.setCurrentIndex(0)
            status.setCurrentIndex(0)

        self.comboBox_select.addItems(['电压模式', '电流模式'])
        # 存储串口列表，每次更新后都更新此列表
        self._com_list = [str(com[0]) for com in coms.comports()]
        # 排序方便更新串口时对比
        self._com_list = sorted(self._com_list)
        self.comboBox_com.addItems(self._com_list)
        self.comboBox_com.setCurrentIndex(0)
        self.comboBox_baudrate.addItems(BAUDRATE)
        self.comboBox_bytesize.addItems(BYTESIZE_KEY)
        self.comboBox_stopbits.addItems(STOPBITS_KEY)
        self.comboBox_parity.addItems(PARITY_KEY)

        self.comboBox_baudrate.setCurrentIndex(3)
        self.comboBox_bytesize.setCurrentIndex(3)
        self.comboBox_stopbits.setCurrentIndex(0)
        self.comboBox_parity.setCurrentIndex(0)

        self.lineEdit_times.setText('1')

    def _init_connect(self):
        self._update_com_thread.coms_signal.connect(self.on_update_com)
        self.pushButton_open.clicked.connect(self.on_open_clicked)
        self.pushButton_model_read.clicked.connect(self.on_model_read)
        self.pushButton_model_write.clicked.connect(self.on_model_write)
        self.pushButton_value_read.clicked.connect(self.on_value_read)
        self.pushButton_status_read.clicked.connect(self.on_status_read)
        self.pushButton_di_read.clicked.connect(self.on_di_read)
        self.pushButton_di_write.clicked.connect(self.on_di_write)
        self.pushButton_read_device_info.clicked.connect(self.on_device_info_read)
        self.pushButton_device_read.clicked.connect(self.on_baudrate_addr_read)
        self.pushButton_device_write.clicked.connect(self.on_baudrate_addr_write)
        self.pushButton_scan.clicked.connect(self.on_recognition_device)
        self.comboBox_select.activated.connect(self.on_onekey_model)
        self._timer.timeout.connect(self.on_time_out)
        self.checkBox_times.stateChanged.connect(self.on_check_timer)

    def on_update_com(self, com):
        """更新串口"""
        # 清空控件
        for _ in range(self.comboBox_com.count()):
            self.comboBox_com.removeItem(0)
        # 更新控件
        coms_list = eval(com)
        current_text = self.comboBox_com.currentText()
        self.comboBox_com.addItems(coms_list)
        self.comboBox_com.setCurrentIndex(
            coms_list.index(current_text) if (current_text in coms_list) else 0
        )

    def on_open_clicked(self):
        if self.pushButton_open.text() == '打开':
            try:
                self._ser.port = self.comboBox_com.currentText()
                self._ser.baudrate = self.comboBox_baudrate.currentText()
                self._ser.bytesize = BYTESIZE[self.comboBox_bytesize.currentText()]
                self._ser.parity = PARITY[self.comboBox_parity.currentText()]
                self._ser.stopbits = STOPBITS[self.comboBox_stopbits.currentText()]
                self._ser.xonxoff = 0
                self.master = modbus_rtu.RtuMaster(self._ser)
                self.master.set_timeout(5)
                self.master.set_verbose(True)
                self.pushButton_open.setText('关闭')
                self.dump_log('串口已经打开!')
            except modbus_tk.modbus.ModbusError as err:
                self.dump_log(str(err))
        else:
            self._ser.close()
            self.pushButton_open.setText('打开')
            self.dump_log('串口已经关闭!')

    def on_model_read(self):
        """读取通道模式"""
        try:
            rst = self.master.execute(self._addr, 3, 0, 8)
            # print('on_model_read rst: ', rst)
            for i in range(1, 9):
                model = self.findChild(QtWidgets.QComboBox, 'comboBox_m%d' % i)
                model.setCurrentIndex(rst[i - 1])
            self.dump_log('成功读取通道模式!')
        except Exception as err:
            self.dump_log(str(err))

    def on_model_write(self):
        """写入通道模式"""
        try:
            buf = list()
            for i in range(1, 9):
                model = self.findChild(QtWidgets.QComboBox, 'comboBox_m%d' % i)
                index = model.currentIndex()
                buf.append(int(index))

            # print('buf: ', buf)
            self.master.execute(self._addr, cst.WRITE_MULTIPLE_REGISTERS, 0, output_value=buf)
            self.dump_log('通道模式写入成功!')
        except Exception as err:
            self.dump_log(str(err))

    def on_value_read(self):
        """读取通道值"""
        # 根据模式判断通道值单位
        self.on_model_read()
        try:
            rst = self.master.execute(self._addr, 4, 0, 8)
            for i in range(1, 9):
                value = self.findChild(QtWidgets.QLineEdit, 'lineEdit_v%d' % i)
                model = self.findChild(QtWidgets.QComboBox, 'comboBox_m%d' % i)
                index = model.currentIndex()
                if not index:
                    value.setText('%.3f 伏' % (rst[i - 1] / 1000))
                else:
                    value.setText('%.3f 毫安' % (rst[i - 1] / 1000))

            self.dump_log('通道值读取成功!')
        except Exception as err:
            self.dump_log(str(err))

    def on_status_read(self):
        """读取通道状态"""
        try:
            rst = self.master.execute(self._addr, 2, 0, 8)
            for i in range(1, 9):
                status = self.findChild(QtWidgets.QComboBox, 'comboBox_s%d' % i)
                status.setCurrentIndex(rst[i - 1])

            self.dump_log('通道状态读取成功!')
        except Exception as err:
            self.dump_log(str(err))

    def on_di_read(self):
        """读取DI阀值"""
        try:
            rst = self.master.execute(self._addr, 3, 5002, 2)
            # print('DI: ', rst)
            self.lineEdit_dih.setText('%.3f' % (rst[0] / 409.6))
            self.lineEdit_dil.setText('%.3f' % (rst[1] / 409.6))
            self.dump_log('DI阀值读取成功!')
        except Exception as err:
            self.dump_log(str(err))
    
    def on_di_write(self):
        """写入DI阀值"""
        try:
            buf = list()
            buf.append(int(float(self.lineEdit_dih.text()) * 409.6))
            buf.append(int(float(self.lineEdit_dil.text()) * 409.6))
            # print('di buf: ', buf)
            rst = self.master.execute(self._addr, cst.WRITE_MULTIPLE_REGISTERS, 5002, output_value=buf)
            self.dump_log('DI阀值写入成功!')
        except Exception as err:
            self.dump_log(str(err))

    def on_device_info_read(self):
        """读取设备信息: 设备类型，硬件版本，软件版本"""
        try:
            rst = self.master.execute(self._addr, 3, 6000, 3)
            # print('device info: ', rst)
            self.lineEdit_device_type.setText('%X' % rst[0])
            self.lineEdit_h_version.setText('%X' % rst[1])
            self.lineEdit_s_version.setText('%X' % rst[2])
            self.dump_log('设备信息度读取成功!')
        except Exception as err:
            self.dump_log(str(err))

    def on_baudrate_addr_read(self):
        """读取设备波特率和设备地址"""
        try:
            rst = self.master.execute(self._addr, 3, 5000, 2)
            # print('baud addr: ', rst)
            self.comboBox_device_addr.setCurrentIndex(rst[0] - 1)
            self.comboBox_device_baudrate.setCurrentIndex(rst[1])
            self.dump_log('设备波特率和设备地址读取成功!')
        except Exception as err:
            self.dump_log(str(err))

    def on_baudrate_addr_write(self):
        """写入设备波特率和设备地址"""
        try:
            buf = list()
            buf.append(int(self.comboBox_device_addr.currentText(), 16))
            buf.append(int(self.comboBox_device_baudrate.currentIndex()))
            # print('baud add buf: ', buf)
            self.master.execute(self._addr, cst.WRITE_MULTIPLE_REGISTERS, 5000, output_value=buf)
            self.dump_log('设备地址和波特率写入成功!')
        except Exception as err:
            self.dump_log(str(err))

    def on_recognition_device(self):
        """设备识别"""
        # print('....开始识别设备....')
        self._scan_device_thread = ScanModbusThread(self.comboBox_com.currentText())
        self._scan_device_thread.device_signal.connect(self.on_scan)
        self._scan_device_thread.status_signal.connect(self.dump_log)
        self.label_scan.setText('......')
        self._scan_device_thread.start()

    def on_scan(self, baudrate, addr):
        """设置波特率和设备地址"""
        self.comboBox_device_baudrate.setCurrentIndex(baudrate)
        self.comboBox_baudrate.setCurrentIndex(baudrate)
        self.comboBox_device_addr.setCurrentIndex(
            self.device_addr.index('0x%X' % addr) if ('0x%X' % addr in self.device_addr) else 0
        )

        self.label_scan.setText('baudrate: %s, addr: 0x%X' % (BAUDRATE[baudrate], addr))
        self._scan_device_thread.device_signal.disconnect()
        self._scan_device_thread.status_signal.disconnect()

    def on_onekey_model(self, index):
        """一键设置通道模式"""
        print('one key index: ', index)
        for i in range(1, 9):
            model = self.findChild(QtWidgets.QComboBox, 'comboBox_m%d' % i)
            model.setCurrentIndex(index)

    def on_time_out(self):
        self.on_value_read()    

    def on_check_timer(self):
        if self.lineEdit_times.text() == '':
            self.dump_log('循环读取的时间间隔不能为空')
            return

        if self.checkBox_times.isChecked():
            try:
                self._timer.start(int(self.lineEdit_times.text()) * 1000)
            except Exception as err:
                self.dump_log(str(err))
        else:
            self._timer.stop()

    def dump_log(self, msg):
        self.statusBar().showMessage(msg, 5000)


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    ui = MainWindow()
    ui.show()
    sys.exit(app.exec_())
