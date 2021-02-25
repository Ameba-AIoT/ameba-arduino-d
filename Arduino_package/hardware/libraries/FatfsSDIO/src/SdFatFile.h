#ifndef _SDFATFILE_H_
#define _SDFATFILE_H_

#include <inttypes.h>
#include <Stream.h>

/** 
 * @class SdFatFile SdFatFile.h 
 * @brief SD FAT File
 */
class SdFatFile : public Stream {

public:

    /**
     * @brief Constructor of SdFatFile
     */
    SdFatFile();

    /**
     * @brief Destructor of SdFatFile
     */
    ~SdFatFile();

    /**
     * @brief Write one byte to file
     *
     * @param[in] c The character to be written
     * @return The succeessful written byte count
     */
    virtual size_t write(uint8_t c);

    /**
     * @brief Write bytes to file
     *
     * @param[in] buf The buffer to be written
     * @param[in] size The length of buffer to be written
     * @return The succeessful written byte count
     */
    virtual size_t write(const uint8_t *buf, size_t size);

    /**
     * @brief Read one byte from file
     *
     * @return The read character
     */
    virtual int read();

    /**
     * @brief Read one byte from file without move curser
     *
     * @return The read character
     */
    virtual int peek();

    /**
     * @brief Check if curser is at EOF
     *
     * @return Return 1 if is not at EOF. And 0 vise versa.
     */
    virtual int available();

    /**
     * @brief nop
     *
     * This is inherited function from class Stream. And it has no effect for SD File.
     */
    virtual void flush();

    /**
     * @brief Check if file is opened
     *
     * @return True if file opened.
     */
    operator bool();

    /**
     * @brief Read bytes from file
     *
     * @param[in] buf The buffer to store the content
     * @param[in] nbyte The buffer size. (Or can be regarded as desired length to read)
     * @return The succeessful written byte count
     */
    int read(void *buf, uint16_t nbyte);

    /**
     * @brief Change curser to sepcific position
     *
     * @param[in] pos The desired position
     * @return Return 0 if success. Return negative value if failure.
     */
    int seek(uint32_t pos);

    /**
     * @brief Close file
     *
     * @return Return 0 if success. Return negative value if failure.
     */
    int close();

    friend class FatFsSD;

private:
    void *m_file;
};

#endif
