#ifndef _ILI9341_GUI_H_
#define _ILI9341_GUI_H_
/*******************************************************************************/
typedef struct FontInfoType
{
    unsigned char** ptr;
    int RowSize;
    int ColSize;
}FontInfoType;
/*******************************************************************************/
/* Font size : 12 row x 8 col */
#define FONTSIZE_12                 12
#define FONT_12_COL_SIZE            8
#define FONT_12_ROW_SIZE            12
/* Font size : 6 row x 8 col */
#define FONTSIZE_8                 8
#define FONT_6_COL_SIZE            8
#define FONT_6_ROW_SIZE            8
/* define ILI9341 color */
#define WHITE_16                    0xffff
#define BLACK_16                    0x00
#define RED_16                      0xf800
#define GREEN_16                    0x07e0
#define BLUE_16                     0x001f
#define PURPLE_16                   0xf81f
#define YELLOW_16                   0xffe0
#define AQUA_16                     0x07ff
/*******************************************************************************/
extern unsigned char ascii_1208[][FONTSIZE_12];
extern unsigned char ascii_0808[][FONTSIZE_8];
extern FontInfoType fontInfo[2];
extern short LinuxLogo [240*340];
/*******************************************************************************/

/*******************************************************************************/

/*******************************************************************************/
#endif