/*!
 * \file ppm_lib.h
 */

#ifndef PPM_LIB_H
    #define PPM_LIB_H
    
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #define HEADER_SIZE 1024
    #define SHIFT_R     16      
    #define SHIFT_G      8
    #define SHIFT_B      0

    /*! macro used to get the RED component byte of a pixel value */
    #define red(v)          ((v >> SHIFT_R) & 0xFF)
    /*! macro used to get the GREEN component byte of a pixel value */
    #define green(v)        ((v >> SHIFT_G) & 0xFF)
    /*! macro used to get the BLUE component byte of a pixel value */
    #define blue(v)         ((v           ) & 0xFF)
    /*! macro used to get the pixel value from RED, GREEN and BLUE components (3 x 1-byte) */
    #define pixel(r,g,b)    (((r&0xFF)<<SHIFT_R) | ((g&0xFF)<<SHIFT_G) | ((b&0xFF)))
    

    /*! 
     * @brief Structure for a PPM image
     */
    typedef struct{
        /*! width of the image in pixels */
        int W;
        /*! height of the image in pixels */
        int H;
        /*! range of dynamic (between black (0) and white (range): usually we will use 255) */
        int range;            
        /*! number of color components (either 1 or 3, for grayscale-PGM or color-PPM) */
        int nbColors;           
        /*! number of bytes in the header part (useful size, not HEADER_SIZE) */
        int hdrSize;           
        /*! number of bytes in the data part (header+data means the total size of the file) */
        int dataSize;           
        /*! pointer to the first byte of the header */
        unsigned char* pHeader; 
        /*! pointer to the first byte of the data part */
        unsigned char* pData;   
    } PPM_IMG;

    /*!
     * @brief This function creates an empty (black image) @c PPM_IMG structure according to the parameters. 
     *
     * This function shall be used when you want to create an empty image structure.
     * Once the function has returned the PPM_IMG structure pointer, you are free to use all the other functions
     * to set pixels values, and save it to a file on your disk.
     *
     * @param w         total width of the new image in pixels.
     * @param h         total height of the new image in pixels
     * @param rng       range of each pixel component value. Thix value is the maximum allowed value.
     *                  Usually for 8-bit components, this value will be set to 255.
     * @param nbColors  the number of components of your image. Allowed values are either 1 or 3, respectively for
     *                  gray level images and RGB color images. For gray images, the pixel value will be the less
     *                  significant byte. For color images, the red, green and blue component values will be stored 
     *                  in the 3 less significant bytes : e.g. @c 0x00RRGGBB. 
     * @return          the pointer to a dynamically allocated PPM structure. This pointer must be used in any
     *                  call to the ppm_lib functions. The memory free is performed during the call of @c ppmClose().
     *                  The returned structure pointer can be used like the one returned by @c ppmOpen() function.
     */
    PPM_IMG* ppmNew      (int w, int h, int rng, int nbColors);

    /*!
     * @brief This function opens an existing ppm/pgm image from a file on your disk. 
     *
     * This function shall be used when you want to create an @c PPM_IMG structure fomr an existing file.
     * The file can be either a valid PPM image (RGB color image) or PGM image (gray level image).
     * The returned structure pointer can be used like the one returned by @c ppmNew() function.
     *
     * @param path      Relative path to the image file to open.
     * @return          the pointer to a dynamically allocated PPM structure. This pointer must be used in any
     *                  call to the ppm_lib functions. The memory free is performed during the call of @c ppmClose().
     */
    PPM_IMG* ppmOpen     (char* path);

    /*!
     * @brief This function reads a pixel from an existing @c PPM_IMG structure pointer. 
     *
     * You just have to give the X and Y coordinates. The origin is in the top left corner of the image.
     * X increasing to the right, Y increasing to the bottom. 
     *
     * @param img       a valid @c PPM_IMG structure pointer.
     * @param x         positive or zero integer value for x coordinate. 
     * @param y         positive or zero integer value for y coordinate. 
     * @return          the pixel value stored on a 32-bit value. Check @ref red, @ref green, or @ref blue macros
     *                  to extract a specific component value.
     */
    int      ppmRead     (PPM_IMG* img, int x, int y);
    
    /*!
     * @brief This function writes a pixel into an existing @c PPM_IMG structure pointer. 
     *
     * You just have to give the X and Y coordinates, and the pixel value (32 bits).
     * The origin is in the top left corner of the image.
     * X increasing to the right, Y increasing to the bottom.
     *
     * @param img       a valid @c PPM_IMG structure pointer.
     * @param x         positive or zero integer value for x coordinate. 
     * @param y         positive or zero integer value for y coordinate. 
     * @param value     32-bit pixel value. Look at @ref pixel macro to create
     *                  a pixel value from the 3 RGB components.  
     * @return          nothing.
     */
    void     ppmWrite    (PPM_IMG* img, int x, int y, int value);

    /*!
     * @brief This function displays the properties of a @c PPM_IMG structure pointer on the standard output. 
     *
     * @param img       a valid @c PPM_IMG structure pointer.
     * @return          nothing.
     */
    void     ppmDisplay  (PPM_IMG* img);  

    /*!
     * @brief This function saves a complete @c PPM_IMG structure into a valid PPM/PGM file. 
     *
     * @param img       a valid @c PPM_IMG structure pointer.
     * @param path      output PPM/PGM file path
     * @return          nothing.
     */
    void     ppmSave     (PPM_IMG* img, char* path);

    /*!
     * @brief This function destroys a @c PPM_IMG structure.
     * This means all the previously allocated memory areas will be freed. 
     *
     * @param img       a valid @c PPM_IMG structure pointer.
     * @return          nothing.
     */
    void     ppmClose    (PPM_IMG* img);

    /*!
     * @brief This function returns the current image width from the @c PPM_IMG structure pointer. 
     *
     * @param img       a valid @c PPM_IMG structure pointer.
     * @return          integer value for the image width in pixels.
     */
    int      ppmGetWidth (PPM_IMG* img);

    /*!
     * @brief This function returns the current image height from the @c PPM_IMG structure pointer. 
     *
     * @param img       a valid @c PPM_IMG structure pointer.
     * @return          integer value for the image height in pixels.
     */
    int      ppmGetHeight(PPM_IMG* img);

    /*!
     * @brief           This function returns the number of components of the current image
     *                  from the @c PPM_IMG structure pointer. 
     *
     * @param img       a valid @c PPM_IMG structure pointer.
     * @return          integer value for the number of color components.
     */
    int      ppmGetColors(PPM_IMG* img);

    /*!
     * @brief This function returns the current image color range from the @c PPM_IMG structure pointer. 
     *
     * @param img       a valid @c PPM_IMG structure pointer.
     * @return          integer value for the color component range (maximum value).
     */
    int      ppmGetRange (PPM_IMG* img);
    
#endif

