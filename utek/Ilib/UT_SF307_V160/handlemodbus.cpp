#include "handlemodbus.h"

int g_indexUser = 0;
bool g_Hot = true;
bool g_CO2 = true;
bool g_PA = true;
bool g_Wet = true;
bool g_HotGroup = true;
bool g_LogOut = false;
bool g_NH3 = false;
int g_HotCount = 0;

HandleModbus::HandleModbus()
{
    m_SlaveHandle = nullptr;
}

HandleModbus::~HandleModbus(){
    this->closeModbus();
}


QString HandleModbus::errorString()
{
    QString errorString = modbus_strerror(errno);
    return errorString;
}

bool HandleModbus::openModbus(QString ip, int port)
{
    QMutexLocker locker(&m_mutex);
    if(m_SlaveHandle != nullptr) {
        modbus_close(m_SlaveHandle);
        m_SlaveHandle = nullptr;
    }
    QByteArray ipByte = ip.toUtf8();
    m_SlaveHandle = modbus_new_tcp(ipByte,port);
    if (m_SlaveHandle == nullptr) { return false; }
    if(modbus_set_slave(m_SlaveHandle, 1) == 0) {
        if(modbus_connect(m_SlaveHandle) == 0) {
            // 设置超时属性
            timeval time;
            time.tv_sec = 2;
            time.tv_usec = 0;
            modbus_set_byte_timeout(m_SlaveHandle, &time);
            modbus_set_response_timeout(m_SlaveHandle, &time);
            this->m_ip = ip;
            return true;
        }
    }
    return false;
}

bool HandleModbus::closeModbus()
{
    QMutexLocker locker(&m_mutex);
    if(m_SlaveHandle == nullptr) {
        return true;
    } else {
        modbus_close(m_SlaveHandle);
        m_SlaveHandle = nullptr;
        this->m_ip = "";
        return false;
    }
}

bool HandleModbus::isOpen()
{
    return m_SlaveHandle != nullptr;
}

/*
 * DESCRIPTION
-----------
The *modbus_read_bits()* function shall read the status of the _nb_ bits (coils)
to the address _addr_ of the remote device. The result of reading is stored in
_dest_ array as unsigned bytes (8 bits) set to `TRUE` or `FALSE`.

You must take care to allocate enough memory to store the results in _dest_
(at least _nb_ * sizeof(uint8_t)).

The function uses the Modbus function code 0x01 (read coil status).


RETURN VALUE
------------
The function shall return the number of read bits if successful. Otherwise it
shall return -1 and set errno.
 */
int HandleModbus::readCoilStatus(int addr, int nbit, uint8_t *dest)
{
    QMutexLocker locker(&m_mutex);
    if(m_SlaveHandle == nullptr) { return -1; }
    int re = modbus_read_bits(m_SlaveHandle, addr, nbit, dest);
    return re;
}


/*
 * DESCRIPTION
-----------
The *modbus_read_input_bits()* function shall read the content of the _nb_ input
bits to the address _addr_ of the remote device.  The result of reading is stored
in _dest_ array as unsigned bytes (8 bits) set to _TRUE_ or _FALSE_.

You must take care to allocate enough memory to store the results in _dest_
(at least _nb_ * sizeof(uint8_t)).

The function uses the Modbus function code 0x02 (read input status).


RETURN VALUE
------------
The function shall return the number of read input status if
successful. Otherwise it shall return -1 and set errno.
*/
int HandleModbus::readInputStatus(int addr, int nbit, uint8_t *dest)
{
    QMutexLocker locker(&m_mutex);
    if(m_SlaveHandle == nullptr) { return -1; }
    int re = modbus_read_input_bits(m_SlaveHandle, addr, nbit, dest);
    return re;
}

/*
 * DESCRIPTION
-----------
The *modbus_read_input_registers()* function shall read the content of the _nb_
input registers to address _addr_ of the remote device. The result of the
reading is stored in _dest_ array as word values (16 bits).

You must take care to allocate enough memory to store the results in _dest_ (at
least _nb_ * sizeof(uint16_t)).

The function uses the Modbus function code 0x04 (read input registers). The
holding registers and input registers have different historical meaning, but
nowadays it's more common to use holding registers only.


RETURN VALUE
------------
The function shall return the number of read input registers if
successful. Otherwise it shall return -1 and set errno.
*/
int HandleModbus::readInputRegisters(int addr, int nb, uint16_t *dest)
{
    QMutexLocker locker(&m_mutex);
    if(m_SlaveHandle == nullptr) { return -1; }
    int re = modbus_read_input_registers(m_SlaveHandle, addr, nb, dest);
    return re;
}

/*
 * DESCRIPTION
-----------
The *modbus_read_registers()* function shall read the content of the _nb_
holding registers to the address _addr_ of the remote device. The result of
reading is stored in _dest_ array as word values (16 bits).

You must take care to allocate enough memory to store the results in _dest_
(at least _nb_ * sizeof(uint16_t)).

The function uses the Modbus function code 0x03 (read holding registers).


RETURN VALUE
------------
The function shall return the number of read registers
if successful. Otherwise it shall return -1 and set errno.
*/
int HandleModbus::readHoldingRegisters(int addr, int nb, uint16_t *dest)
{
    QMutexLocker locker(&m_mutex);
    if(m_SlaveHandle == nullptr) { return -1; }
    int re = modbus_read_registers(m_SlaveHandle, addr, nb, dest);
    return re;
}

/* DESCRIPTION
-----------
The *modbus_write_bit()* function shall write the status of _status_ at the
address _addr_ of the remote device. The value must be set to `TRUE` or `FALSE`.

The function uses the Modbus function code 0x05 (force single coil).

RETURN VALUE
------------
The function shall return 1 if successful. Otherwise it shall return -1 and set
errno.
*/
int HandleModbus::writeSingleCoil(int addr, int value)
{
    QMutexLocker locker(&m_mutex);
    if(m_SlaveHandle == nullptr) { return -1; }
    int re = modbus_write_bit(m_SlaveHandle, addr, value);
    return re;
}

/*
 * DESCRIPTION
-----------
The *modbus_write_bits()* function shall write the status of the _nb_ bits
(coils) from _src_ at the address _addr_ of the remote device. The
_src_ array must contains bytes set to `TRUE` or `FALSE`.

The function uses the Modbus function code 0x0F (force multiple coils).


RETURN VALUE
------------
The function shall return the number of written bits if successful. Otherwise it
shall return -1 and set errno.
*/
int HandleModbus::writeMultipleCoil(int addr, int nb, const uint8_t *src)
{
    QMutexLocker locker(&m_mutex);
    if(m_SlaveHandle == nullptr) { return -1; }
    int re = modbus_write_bits(m_SlaveHandle, addr, nb, src);
    return re;
}

/*
 * DESCRIPTION
-----------
The *modbus_write_register()* function shall write the value of _value_
holding registers at the address _addr_ of the remote device.

The function uses the Modbus function code 0x06 (preset single register).


RETURN VALUE
------------
The function shall return 1 if successful. Otherwise it shall return -1 and set
errno.
*/
int  HandleModbus::writeSingleRegister(int addr, int value)
{
    QMutexLocker locker(&m_mutex);
    if(m_SlaveHandle == nullptr) { return -1; }
    int re = modbus_write_register(m_SlaveHandle, addr, value);
    return re;
}

/*
 * DESCRIPTION
-----------
The *modbus_write_registers()* function shall write the content of the _nb_
holding registers from the array _src_ at address _addr_ of the remote device.

The function uses the Modbus function code 0x10 (preset multiple registers).


RETURN VALUE
------------
The function shall return the number of written registers if
successful. Otherwise it shall return -1 and set errno.
*/
int HandleModbus::writeMultipleRegisters(int addr, int nb, const uint16_t *src)
{
    QMutexLocker locker(&m_mutex);
    if(m_SlaveHandle == nullptr) { return -1; }
    int re = modbus_write_registers(m_SlaveHandle, addr,nb, src);
    return re;
}
