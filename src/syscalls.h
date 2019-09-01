#ifndef __SYSCALLS_H__
#define __SYSCALLS_H__

// system call definitions
#define SYSCALL_A 0xD201D002
#define SYSCALL_B 0x422B0009
#define SYSCALL_ENTRY 0x80010070

// function type definitions
typedef int (*_ifp_v)(void);
typedef int (*_ifp_i)(int);
typedef int (*_ifp_uc)(unsigned char);
typedef int (*_ifp_ucp)(unsigned char*);
typedef int (*_ifp_i_ucp)(int, unsigned char*);
typedef int (*_ifp_ucp_i)(unsigned char*, int);
typedef int (*_ifp_ucp_i_sp)(unsigned char*, int, short*);

const unsigned int _sc_0x40C[] = { SYSCALL_A, SYSCALL_B, SYSCALL_ENTRY, 0x40C };
/**
 * int Serial_ReadByte(unsigned char* dest)
 * 
 * Fetches one byte from the receive buffer to the character pointed to by dest
 * 
 * @return 0 on success, 1 if receive buffer is empty, or 3 if device is not open
 */
#define Serial_ReadByte (*(_ifp_ucp) _sc_0x40C)

const unsigned int _sc_0x40D[] = { SYSCALL_A, SYSCALL_B, SYSCALL_ENTRY, 0x40D };
/**
 * int Serial_ReadBytes(unsigned char* dest, int max, short* size );
 * 
 * Fetches up to max bytes from the character buffer into the buffer pointed to by dest
 * The number of bytes actually read from the buffer are stored at size
 * 
 * @return 0 on success, 1 if receive buffer is empty, or 3 if device is not open
 */
#define Serial_ReadBytes (*(_ifp_ucp_i_sp) _sc_0x40D)

const unsigned int _sc_0x40E[] = { SYSCALL_A, SYSCALL_B, SYSCALL_ENTRY, 0x40E };
/**
 * int Serial_WriteByte(unsigned char byte);
 * 
 * Writes byte to the transmit buffer
 * 
 * @return 0 on success, or 3 if device is not open
 */
#define Serial_WriteByte (*(_ifp_uc) _sc_0x40E)

const unsigned int _sc_0x40F[] = { SYSCALL_A, SYSCALL_B, SYSCALL_ENTRY, 0x40F };
/**
 * int Serial_WriteBytes(unsigned char* src, int size);
 * 
 * Writes size bytes from the buUer at src to the transmit buffer, if there is room for it
 * 
 * @return 0 on success, 2 if buffer is too full, or 3 if the device is not open
 */
#define Serial_WriteBytes (*(_ifp_ucp_i) _sc_0x40F)

const unsigned int _sc_0x410[] = { SYSCALL_A, SYSCALL_B, SYSCALL_ENTRY, 0x410 };
/**
 * int Serial_DirectWriteByte(unsigned char byte);
 * 
 * Writes byte directly to the serial FIFO
 * 
 * @return 0 on success, 1 if FIFO is non-empty, or 3 if device is not open
 */
#define Serial_DirectWriteByte (*(_ifp_uc) _sc_0x410)

const unsigned int _sc_0x411[] = { SYSCALL_A, SYSCALL_B, SYSCALL_ENTRY, 0x411 };
/**
 * int Serial_GetRxBytesAvailable(void)
 * 
 * @return the number of bytes available to be read
 */
#define Serial_GetRxBytesAvailable (*(_ifp_v) _sc_0x411)

const unsigned int _sc_0x412[] = { SYSCALL_A, SYSCALL_B, SYSCALL_ENTRY, 0x412 };
/**
 * int Serial_GetTxBytesAvailable(void)
 * 
 * @return the number of bytes free in the transmit buffer
 */
#define Serial_GetTxBytesAvailable (*(_ifp_v) _sc_0x412)

const unsigned int _sc_0x413[] = { SYSCALL_A, SYSCALL_B, SYSCALL_ENTRY, 0x413 };
/**
 * int Serial_ClearRxBuffer(void)
 * 
 * Clears the receive buffer of the opened device
 * 
 * @return 0 on success, or 3 if the device is not open
 */
#define Serial_ClearRxBuffer (*(_ifp_v) _sc_0x413)

const unsigned int _sc_0x414[] = { SYSCALL_A, SYSCALL_B, SYSCALL_ENTRY, 0x414 };
/**
 * int Serial_ClearTxBuffer(void)
 * 
 * Clears the transmit buffer
 * 
 * @return 0
 */
#define Serial_ClearTxBuffer (*(_ifp_v) _sc_0x414)

const unsigned int _sc_0x418[] = { SYSCALL_A, SYSCALL_B, SYSCALL_ENTRY, 0x418 };
/**
 * int Serial_Open(unsigned char* conf)
 * 
 * Opens and prepares the serial interface for communication. The provided conf is a pointer to
 * an array, consisting of five bytes describing the interface settings:
 *      conf[0] = 0 (always)
 *      conf[1] = baud rate
 *          0 ->  300, 1 ->   600, 2 ->  1200, 3 ->  2400, 4 ->   4800
 *          5 -> 9600, 6 -> 19200, 7 -> 38400, 8 -> 57600, 9 -> 115200
 *      conf[2] = parity
 *          0 -> none, 1 -> odd, 2 -> even
 *      conf[3] = byte length
 *          0 -> 8 bits
 *          1 -> 7 bits
 *      conf[4] = stop bits
 *          0 -> 1 bit
 *          1 -> 2 bits
 *      conf[5] = 0 (always)
 * 
 * The size of the transmit buffer is 256B, and the receive buffer is 1kB
 * 
 * @return 0 on success, 3 if the interface is already open, and 4 if conf[0] is non-zero
 */
#define Serial_Open (*(_ifp_ucp) _sc_0x418)

const unsigned int _sc_0x419[] = { SYSCALL_A, SYSCALL_B, SYSCALL_ENTRY, 0x419 };
/**
 * int Serial_Close(int mode)
 * 
 * Clears transmission buffers, and disables the serial link function.
 * If mode is 1, the interface is disabled regardless of waiting (not transmitted) data.
 * If mode is not 1, this function will not close the interface if data is waiting, and will return 5
 * 
 * @return 0 on success (link closed) or 5 if there is data waiting to be transmitted (link not closed)
 */
#define Serial_Close (*(_ifp_i) _sc_0x419)

const unsigned int _sc_0x422[] = { SYSCALL_A, SYSCALL_B, SYSCALL_ENTRY, 0x422 };
/**
 * int Serial_PeekAt(int index, unsigned char* dest)
 * 
 * Copies the byte at index in the receive buffer to the character pointed to by dest if it is within
 * the size of the receive buffer
 * 
 * @return 0 on success, 1 if index does not point to an active byte in the buffer, or 3 if the device
 *          is not open
 */
#define Serial_PeekAt (*(_ifp_i_ucp) _sc_0x422)

const unsigned int _sc_0x425[] = { SYSCALL_A, SYSCALL_B, SYSCALL_ENTRY, 0x425 };
/**
 * int Serial_IsOpen(void)
 * 
 * Gets the status of the serial link
 * 
 * @return 1 if the device is open, or 3 if closed
 */
#define Serial_IsOpen (*(_ifp_v) _sc_0x425)

#endif //__SYSCALLS_H__