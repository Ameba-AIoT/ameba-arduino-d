#ifndef _FLASH_MEMORY_H_
#define _FLASH_MEMORY_H_

/**
 * @brief The default flash address for application usage
 *
 * This is reference flash address that user can use.
 * If user use flash address overwirte image address, the it would corrupt image content.
 **/
#define FLASH_MEMORY_APP_BASE 0x00100000

/** 
 * @class FlashMemoryClass FlashMemory.h 
 * @brief Flash memory api
 *
 * There are 2 ways to operate memroy in this class. \n
 * The first one is to allocate a buf and mirror the content of flash. We operate this buf and update flash when needed.
 * In this way we can reduce flash memory operation since it cost time. \n
 * The second one is write to flash immediately. It would cost time but save buf.
 */
class FlashMemoryClass
{
    private:
        unsigned int base_address;
        void* pFlash;

    public:
        /**
         * @brief Constructor of FlashMemoryClass
         *
         * @param[in] _base_address the base address to operate
         * @param[in] _buf_size the buf size for mirror a copy to reduce flash memory operation.
         */
        FlashMemoryClass(unsigned int _base_address, unsigned int _buf_size);

        /**
         * @brief Destructor of FlashMemoryClass
         */
        ~FlashMemoryClass();

        /**
         * @brief Re-initialize the base address and size
         *
         * The base address shell align to size of 0x1000. And the size shell be multiple of 0x1000
         *
         * @param[in] _base_address The base address
         * @param[in] _buf_size The desired work size
         */
        void begin(unsigned int _base_address, unsigned int _buf_size);

        /**
         * @brief Read the content to buf
         *
         * Read flash memory into the buf. The size would be 0x1000.
         */
        void read();

        /**
         * @brief Write buf back to flash memory
         *
         * Write flash memory with the content of buf. The size is 0x1000.
         *
         * @param[in] erase By default it is true and erase flash memory before write to it.
         */
        void update(bool erase = true);

        /**
         * @brief Read fore bytes from flash memory
         *
         * Read 4 byte from specifc offet based on base address.
         *
         * @param[in] offset The offset according to base address
         * @return the read data with size of 4 bytes
         */
        unsigned int readWord(unsigned int offset);

        /**
         * @brief Write 4 bytes into flash memory
         *
         * It will try to write 4 bytes first. If the read data differ from the write data, then we buffer the sector of flash memory, erase it, and
         * write correct data back to it.
         *
         * @param[in] offset The offset according to base address
         * @param[in] data The data to be written
         */
        void writeWord(unsigned int offset, unsigned int data);

        /**
         * @brief The buf size. (It can be regarded as work size)
         */
        unsigned int buf_size;

        /**
         * @brief The buf to be operated.
         * @note Modify buf won't change the content of buf. It needs update to write back to flash memory.
         */
        unsigned char *buf;
};

extern FlashMemoryClass FlashMemory;

#endif
