#include "ILI9341_GUI.h"
/*******************************************************************************/
const unsigned char ascii_1206[][FONTSIZE_12]={
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 0 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 1 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 2 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 3 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 4 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 5 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 6 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 7 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 8 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 9 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 10 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 11 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 12 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 13 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 14 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 15 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 16 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 17 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 18 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 19 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 20 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 21 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 22 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 23 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 24 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 25 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 26 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 27 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 28 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 29 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 30 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 31 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",space, 32*/
    {0x00,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x00,0x04,0x00,0x00},/*"!"*/
    {0x00,0x14,0x0A,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"""*/
    {0x00,0x00,0x14,0x14,0x3F,0x14,0x0A,0x3F,0x0A,0x0A,0x00,0x00},/*"#"*/
    {0x00,0x04,0x1E,0x15,0x05,0x06,0x0C,0x14,0x15,0x0F,0x04,0x00},/*"$"*/
    {0x00,0x00,0x12,0x15,0x0D,0x0A,0x14,0x2C,0x2A,0x12,0x00,0x00},/*"%"*/
    {0x00,0x00,0x04,0x0A,0x0A,0x1E,0x15,0x15,0x09,0x36,0x00,0x00},/*"&"*/
    {0x00,0x02,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"'"*/
    {0x00,0x20,0x10,0x08,0x08,0x08,0x08,0x08,0x08,0x10,0x20,0x00},/*"("*/
    {0x00,0x02,0x04,0x08,0x08,0x08,0x08,0x08,0x08,0x04,0x02,0x00},/*")"*/
    {0x00,0x00,0x00,0x04,0x15,0x0E,0x0E,0x15,0x04,0x00,0x00,0x00},/*"*"*/
    {0x00,0x00,0x04,0x04,0x04,0x1F,0x04,0x04,0x04,0x00,0x00,0x00},/*"+",11*/
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x02,0x01},/*",",12*/
    {0x00,0x00,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x00,0x00,0x00},/*"-",13*/
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00},/*".",14*/
    {0x00,0x10,0x08,0x08,0x08,0x04,0x04,0x02,0x02,0x02,0x01,0x00},/*"/",15*/
    {0x00,0x00,0x0E,0x11,0x11,0x11,0x11,0x11,0x11,0x0E,0x00,0x00},/*"0",16*/
    {0x00,0x00,0x04,0x06,0x04,0x04,0x04,0x04,0x04,0x0E,0x00,0x00},/*"1",17*/
    {0x00,0x00,0x0E,0x11,0x11,0x08,0x04,0x02,0x01,0x1F,0x00,0x00},/*"2",18*/
    {0x00,0x00,0x0E,0x11,0x10,0x0C,0x10,0x10,0x11,0x0E,0x00,0x00},/*"3",19*/
    {0x00,0x00,0x08,0x0C,0x0A,0x0A,0x09,0x1E,0x08,0x18,0x00,0x00},/*"4",20*/
    {0x00,0x00,0x1F,0x01,0x01,0x0F,0x10,0x10,0x11,0x0E,0x00,0x00},/*"5",21*/
    {0x00,0x00,0x0E,0x09,0x01,0x0F,0x11,0x11,0x11,0x0E,0x00,0x00},/*"6",22*/
    {0x00,0x00,0x1F,0x09,0x08,0x04,0x04,0x04,0x04,0x04,0x00,0x00},/*"7",23*/
    {0x00,0x00,0x0E,0x11,0x11,0x0E,0x11,0x11,0x11,0x0E,0x00,0x00},/*"8",24*/
    {0x00,0x00,0x0E,0x11,0x11,0x11,0x1E,0x10,0x12,0x0E,0x00,0x00},/*"9",25*/
    {0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x04,0x00,0x00},/*":",26*/
    {0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x04,0x04,0x00},/*";",27*/
    {0x00,0x20,0x10,0x08,0x04,0x02,0x04,0x08,0x10,0x20,0x00,0x00},/*"<",28*/
    {0x00,0x00,0x00,0x00,0x1F,0x00,0x00,0x1F,0x00,0x00,0x00,0x00},/*"=",29*/
    {0x00,0x02,0x04,0x08,0x10,0x20,0x10,0x08,0x04,0x02,0x00,0x00},/*">",30*/
    {0x00,0x00,0x0E,0x11,0x11,0x08,0x04,0x04,0x00,0x04,0x00,0x00},/*"?",31*/
    {0x00,0x00,0x0E,0x11,0x19,0x15,0x15,0x1D,0x01,0x1E,0x00,0x00},/*"@",32*/
    {0x00,0x00,0x04,0x04,0x0C,0x0A,0x0A,0x1E,0x12,0x33,0x00,0x00},/*"A",33*/
    {0x00,0x00,0x0F,0x12,0x12,0x0E,0x12,0x12,0x12,0x0F,0x00,0x00},/*"B",34*/
    {0x00,0x00,0x1E,0x11,0x01,0x01,0x01,0x01,0x11,0x0E,0x00,0x00},/*"C",35*/
    {0x00,0x00,0x0F,0x12,0x12,0x12,0x12,0x12,0x12,0x0F,0x00,0x00},/*"D",36*/
    {0x00,0x00,0x1F,0x12,0x0A,0x0E,0x0A,0x02,0x12,0x1F,0x00,0x00},/*"E",37*/
    {0x00,0x00,0x1F,0x12,0x0A,0x0E,0x0A,0x02,0x02,0x07,0x00,0x00},/*"F",38*/
    {0x00,0x00,0x1C,0x12,0x01,0x01,0x39,0x11,0x12,0x0C,0x00,0x00},/*"G",39*/
    {0x00,0x00,0x33,0x12,0x12,0x1E,0x12,0x12,0x12,0x33,0x00,0x00},/*"H",40*/
    {0x00,0x00,0x1F,0x04,0x04,0x04,0x04,0x04,0x04,0x1F,0x00,0x00},/*"I",41*/
    {0x00,0x00,0x3E,0x08,0x08,0x08,0x08,0x08,0x08,0x09,0x07,0x00},/*"J",42*/
    {0x00,0x00,0x37,0x12,0x0A,0x06,0x0A,0x0A,0x12,0x37,0x00,0x00},/*"K",43*/
    {0x00,0x00,0x07,0x02,0x02,0x02,0x02,0x02,0x22,0x3F,0x00,0x00},/*"L",44*/
    {0x00,0x00,0x1B,0x1B,0x1B,0x1B,0x15,0x15,0x15,0x15,0x00,0x00},/*"M",45*/
    {0x00,0x00,0x3B,0x12,0x16,0x16,0x1A,0x1A,0x12,0x17,0x00,0x00},/*"N",46*/
    {0x00,0x00,0x0E,0x11,0x11,0x11,0x11,0x11,0x11,0x0E,0x00,0x00},/*"O",47*/
    {0x00,0x00,0x0F,0x12,0x12,0x0E,0x02,0x02,0x02,0x07,0x00,0x00},/*"P",48*/
    {0x00,0x00,0x0E,0x11,0x11,0x11,0x11,0x17,0x19,0x0E,0x18,0x00},/*"Q",49*/
    {0x00,0x00,0x0F,0x12,0x12,0x0E,0x0A,0x12,0x12,0x37,0x00,0x00},/*"R",50*/
    {0x00,0x00,0x1E,0x11,0x01,0x06,0x08,0x10,0x11,0x0F,0x00,0x00},/*"S",51*/
    {0x00,0x00,0x1F,0x15,0x04,0x04,0x04,0x04,0x04,0x0E,0x00,0x00},/*"T",52*/
    {0x00,0x00,0x33,0x12,0x12,0x12,0x12,0x12,0x12,0x0C,0x00,0x00},/*"U",53*/
    {0x00,0x00,0x33,0x12,0x12,0x0A,0x0A,0x0C,0x04,0x04,0x00,0x00},/*"V",54*/
    {0x00,0x00,0x15,0x15,0x15,0x0E,0x0A,0x0A,0x0A,0x0A,0x00,0x00},/*"W",55*/
    {0x00,0x00,0x1B,0x0A,0x0A,0x04,0x04,0x0A,0x0A,0x1B,0x00,0x00},/*"X",56*/
    {0x00,0x00,0x1B,0x0A,0x0A,0x04,0x04,0x04,0x04,0x0E,0x00,0x00},/*"Y",57*/
    {0x00,0x00,0x1F,0x09,0x08,0x04,0x04,0x02,0x12,0x1F,0x00,0x00},/*"Z",58*/
    {0x00,0x1C,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x1C,0x00},/*"[",59*/
    {0x00,0x02,0x02,0x02,0x04,0x04,0x08,0x08,0x08,0x10,0x00,0x00},/*"\",60*/
    {0x00,0x0E,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x0E,0x00},/*"]",61*/
    {0x00,0x04,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"^",62*/
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F},/*"_",63*/
    {0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"`",64*/
    {0x00,0x00,0x00,0x00,0x00,0x0C,0x12,0x1C,0x12,0x3C,0x00,0x00},/*"a",65*/
    {0x00,0x00,0x03,0x02,0x02,0x0E,0x12,0x12,0x12,0x0E,0x00,0x00},/*"b",66*/
    {0x00,0x00,0x00,0x00,0x00,0x1C,0x12,0x02,0x02,0x1C,0x00,0x00},/*"c",67*/
    {0x00,0x00,0x18,0x10,0x10,0x1C,0x12,0x12,0x12,0x3C,0x00,0x00},/*"d",68*/
    {0x00,0x00,0x00,0x00,0x00,0x0C,0x12,0x1E,0x02,0x1C,0x00,0x00},/*"e",69*/
    {0x00,0x00,0x38,0x04,0x04,0x1E,0x04,0x04,0x04,0x1E,0x00,0x00},/*"f",70*/
    {0x00,0x00,0x00,0x00,0x00,0x3C,0x12,0x0C,0x02,0x1E,0x22,0x1C},/*"g",71*/
    {0x00,0x00,0x03,0x02,0x02,0x0E,0x12,0x12,0x12,0x37,0x00,0x00},/*"h",72*/
    {0x00,0x00,0x04,0x00,0x00,0x06,0x04,0x04,0x04,0x0E,0x00,0x00},/*"i",73*/
    {0x00,0x00,0x08,0x00,0x00,0x0C,0x08,0x08,0x08,0x08,0x08,0x07},/*"j",74*/
    {0x00,0x00,0x03,0x02,0x02,0x3A,0x0A,0x0E,0x12,0x37,0x00,0x00},/*"k",75*/
    {0x00,0x00,0x07,0x04,0x04,0x04,0x04,0x04,0x04,0x1F,0x00,0x00},/*"l",76*/
    {0x00,0x00,0x00,0x00,0x00,0x0F,0x15,0x15,0x15,0x15,0x00,0x00},/*"m",77*/
    {0x00,0x00,0x00,0x00,0x00,0x0F,0x12,0x12,0x12,0x37,0x00,0x00},/*"n",78*/
    {0x00,0x00,0x00,0x00,0x00,0x0C,0x12,0x12,0x12,0x0C,0x00,0x00},/*"o",79*/
    {0x00,0x00,0x00,0x00,0x00,0x0F,0x12,0x12,0x12,0x0E,0x02,0x07},/*"p",80*/
    {0x00,0x00,0x00,0x00,0x00,0x1C,0x12,0x12,0x12,0x1C,0x10,0x38},/*"q",81*/
    {0x00,0x00,0x00,0x00,0x00,0x1B,0x06,0x02,0x02,0x07,0x00,0x00},/*"r",82*/
    {0x00,0x00,0x00,0x00,0x00,0x1E,0x02,0x0C,0x10,0x1E,0x00,0x00},/*"s",83*/
    {0x00,0x00,0x00,0x04,0x04,0x0E,0x04,0x04,0x04,0x18,0x00,0x00},/*"t",84*/
    {0x00,0x00,0x00,0x00,0x00,0x1B,0x12,0x12,0x12,0x3C,0x00,0x00},/*"u",85*/
    {0x00,0x00,0x00,0x00,0x00,0x37,0x12,0x0A,0x0C,0x04,0x00,0x00},/*"v",86*/
    {0x00,0x00,0x00,0x00,0x00,0x15,0x15,0x0E,0x0A,0x0A,0x00,0x00},/*"w",87*/
    {0x00,0x00,0x00,0x00,0x00,0x1B,0x0A,0x04,0x0A,0x1B,0x00,0x00},/*"x",88*/
    {0x00,0x00,0x00,0x00,0x00,0x37,0x12,0x0A,0x0C,0x04,0x04,0x03},/*"y",89*/
    {0x00,0x00,0x00,0x00,0x00,0x1E,0x08,0x04,0x04,0x1E,0x00,0x00},/*"z",90*/
    {0x00,0x18,0x08,0x08,0x08,0x04,0x08,0x08,0x08,0x08,0x18,0x00},/*"{",91*/
    {0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08},/*"|",92*/
    {0x00,0x06,0x04,0x04,0x04,0x08,0x04,0x04,0x04,0x04,0x06,0x00},/*"}",93*/
    {0x02,0x25,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} /*"~",94*/
}; 

const unsigned char ascii_1608[][FONTSIZE_16]={
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 0 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 1 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 2 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 3 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 4 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 5 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 6 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 7 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 8 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 9 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 10 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 11 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 12 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 13 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 14 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 15 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 16 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 17 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 18 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 19 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 20 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 21 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 22 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 23 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 24 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 25 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 26 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 27 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 28 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 29 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 30 */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/* 31 */

    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ", space */
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x0C,0x1E,0x1E,0x1E,0x0C,0x0C,0x00,0x0C,0x0C,0x00,0x00,0x00,0x00},/*"!",1*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x33,0x33,0x33,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*""",2*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x36,0x36,0x7F,0x36,0x36,0x36,0x7F,0x36,0x36,0x00,0x00,0x00,0x00},/*"#",3*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x0C,0x0C,0x1E,0x33,0x03,0x06,0x0C,0x18,0x30,0x33,0x1E,0x0C,0x0C,0x00,0x00},/*"$",4*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x07,0x0D,0x2D,0x37,0x18,0x0C,0x06,0x3B,0x6D,0x6C,0x38,0x00,0x00,0x00},/*"%",5*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x0E,0x1B,0x1B,0x0E,0x03,0x7B,0x33,0x33,0x6E,0x00,0x00,0x00,0x00},/*"&",6*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x0C,0x0C,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"'",7*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x18,0x0C,0x0C,0x06,0x06,0x06,0x06,0x06,0x0C,0x0C,0x18,0x00,0x00},/*"(",8*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x06,0x0C,0x0C,0x18,0x18,0x18,0x18,0x18,0x0C,0x0C,0x06,0x00,0x00},/*")",9*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x36,0x1C,0x7F,0x1C,0x36,0x00,0x00,0x00,0x00,0x00,0x00},/*"*",10*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x0C,0x0C,0x3F,0x0C,0x0C,0x00,0x00,0x00,0x00,0x00,0x00},/*"+",11*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x1C,0x18,0x0C,0x00,0x00},/*",",12*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"-",13*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x1C,0x00,0x00,0x00,0x00},/*".",14*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x30,0x30,0x18,0x18,0x0C,0x0C,0x06,0x06,0x03,0x03,0x00,0x00,0x00},/*"/",15*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x3C,0x66,0x76,0x76,0x66,0x6E,0x6E,0x66,0x3C,0x00,0x00,0x00,0x00},/*"0",16*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x18,0x1C,0x1F,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00},/*"1",17*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x1E,0x33,0x33,0x30,0x18,0x0C,0x06,0x03,0x3F,0x00,0x00,0x00,0x00},/*"2",18*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x1E,0x33,0x33,0x30,0x1C,0x30,0x33,0x33,0x1E,0x00,0x00,0x00,0x00},/*"3",19*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x06,0x06,0x36,0x36,0x36,0x33,0x7F,0x30,0x30,0x00,0x00,0x00,0x00},/*"4",20*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x3F,0x03,0x03,0x03,0x1F,0x30,0x30,0x18,0x0F,0x00,0x00,0x00,0x00},/*"5",21*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x1C,0x0C,0x06,0x1F,0x33,0x33,0x33,0x33,0x1E,0x00,0x00,0x00,0x00},/*"6",22*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x3F,0x30,0x18,0x18,0x0C,0x0C,0x06,0x06,0x06,0x00,0x00,0x00,0x00},/*"7",23*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x1E,0x33,0x33,0x37,0x1E,0x3B,0x33,0x33,0x1E,0x00,0x00,0x00,0x00},/*"8",24*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x1E,0x33,0x33,0x33,0x33,0x3E,0x18,0x0C,0x0E,0x00,0x00,0x00,0x00},/*"9",25*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x1C,0x1C,0x00,0x00,0x00,0x1C,0x1C,0x00,0x00,0x00,0x00},/*":",26*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x1C,0x1C,0x00,0x00,0x00,0x1C,0x1C,0x18,0x0C,0x00,0x00},/*";",27*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x30,0x18,0x0C,0x06,0x03,0x06,0x0C,0x18,0x30,0x00,0x00,0x00,0x00},/*"<",28*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x00,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"=",29*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x03,0x06,0x0C,0x18,0x30,0x18,0x0C,0x06,0x03,0x00,0x00,0x00,0x00},/*">",30*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x1E,0x33,0x33,0x18,0x0C,0x0C,0x00,0x0C,0x0C,0x00,0x00,0x00,0x00},/*"?",31*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x3F,0x61,0x61,0x79,0x6D,0x6D,0x79,0x01,0x7F,0x00,0x00,0x00,0x00},/*"@",32*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x0C,0x1E,0x33,0x33,0x33,0x3F,0x33,0x33,0x33,0x00,0x00,0x00,0x00},/*"A",33*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x1F,0x33,0x33,0x33,0x1F,0x33,0x33,0x33,0x1F,0x00,0x00,0x00,0x00},/*"B",34*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x1E,0x33,0x33,0x03,0x03,0x03,0x33,0x33,0x1E,0x00,0x00,0x00,0x00},/*"C",35*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x0F,0x1B,0x33,0x33,0x33,0x33,0x33,0x1B,0x0F,0x00,0x00,0x00,0x00},/*"D",36*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x3F,0x03,0x03,0x03,0x1F,0x03,0x03,0x03,0x3F,0x00,0x00,0x00,0x00},/*"E",37*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x3F,0x03,0x03,0x03,0x1F,0x03,0x03,0x03,0x03,0x00,0x00,0x00,0x00},/*"F",38*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x1E,0x33,0x33,0x03,0x03,0x3B,0x33,0x33,0x3E,0x00,0x00,0x00,0x00},/*"G",39*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x33,0x33,0x33,0x33,0x3F,0x33,0x33,0x33,0x33,0x00,0x00,0x00,0x00},/*"H",40*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x1E,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x1E,0x00,0x00,0x00,0x00},/*"I",41*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x30,0x30,0x30,0x30,0x30,0x30,0x33,0x33,0x1E,0x00,0x00,0x00,0x00},/*"J",42*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x33,0x33,0x1B,0x1B,0x0F,0x1B,0x1B,0x33,0x33,0x00,0x00,0x00,0x00},/*"K",43*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x3F,0x00,0x00,0x00,0x00},/*"L",44*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x63,0x63,0x77,0x6B,0x6B,0x6B,0x63,0x63,0x63,0x00,0x00,0x00,0x00},/*"M",45*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x63,0x63,0x67,0x6F,0x7B,0x73,0x63,0x63,0x63,0x00,0x00,0x00,0x00},/*"N",46*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x1E,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x1E,0x00,0x00,0x00,0x00},/*"O",47*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x1F,0x33,0x33,0x33,0x1F,0x03,0x03,0x03,0x03,0x00,0x00,0x00,0x00},/*"P",48*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x1E,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x1E,0x18,0x30,0x00,0x00},/*"Q",49*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x1F,0x33,0x33,0x33,0x1F,0x1B,0x33,0x33,0x33,0x00,0x00,0x00,0x00},/*"R",50*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x1E,0x33,0x03,0x06,0x0C,0x18,0x30,0x33,0x1E,0x00,0x00,0x00,0x00},/*"S",51*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x3F,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x00,0x00,0x00,0x00},/*"T",52*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x1E,0x00,0x00,0x00,0x00},/*"U",53*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x1E,0x0C,0x00,0x00,0x00,0x00},/*"V",54*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x63,0x63,0x63,0x6B,0x6B,0x6B,0x36,0x36,0x36,0x00,0x00,0x00,0x00},/*"W",55*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x33,0x33,0x16,0x0C,0x0C,0x1A,0x33,0x33,0x33,0x00,0x00,0x00,0x00},/*"X",56*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x33,0x33,0x33,0x33,0x1E,0x0C,0x0C,0x0C,0x0C,0x00,0x00,0x00,0x00},/*"Y",57*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x3F,0x30,0x30,0x18,0x0C,0x06,0x03,0x03,0x3F,0x00,0x00,0x00,0x00},/*"Z",58*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x1E,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x06,0x1E,0x00},/*"[",59*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x03,0x03,0x06,0x06,0x0C,0x0C,0x18,0x18,0x30,0x30,0x00,0x00,0x00},/*"\",60*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x1E,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x1E,0x00},/*"]",61*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x0C,0x1E,0x33,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"^",62*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x00},/*"_",63*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x0E,0x0C,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"`",64*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x1E,0x30,0x30,0x3E,0x33,0x33,0x3E,0x00,0x00,0x00,0x00},/*"a",65*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x03,0x03,0x1F,0x33,0x33,0x33,0x33,0x33,0x1F,0x00,0x00,0x00,0x00},/*"b",66*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x1E,0x33,0x03,0x03,0x03,0x33,0x1E,0x00,0x00,0x00,0x00},/*"c",67*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x30,0x30,0x3E,0x33,0x33,0x33,0x33,0x33,0x3E,0x00,0x00,0x00,0x00},/*"d",68*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x1E,0x33,0x33,0x3F,0x03,0x03,0x1E,0x00,0x00,0x00,0x00},/*"e",69*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x3C,0x06,0x06,0x06,0x3F,0x06,0x06,0x06,0x06,0x00,0x00,0x00,0x00},/*"f",70*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x3E,0x33,0x33,0x33,0x33,0x33,0x3E,0x30,0x30,0x1F,0x00},/*"g",71*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x03,0x03,0x1F,0x33,0x33,0x33,0x33,0x33,0x33,0x00,0x00,0x00,0x00},/*"h",72*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x0C,0x0C,0x00,0x0F,0x0C,0x0C,0x0C,0x0C,0x0C,0x3F,0x00,0x00,0x00,0x00},/*"i",73*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x18,0x18,0x00,0x1E,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x0F,0x00},/*"j",74*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x03,0x03,0x33,0x33,0x1B,0x0F,0x1B,0x33,0x33,0x00,0x00,0x00,0x00},/*"k",75*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x0F,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3F,0x00,0x00,0x00,0x00},/*"l",76*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x3F,0x6B,0x6B,0x6B,0x6B,0x6B,0x63,0x00,0x00,0x00,0x00},/*"m",77*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x1F,0x33,0x33,0x33,0x33,0x33,0x33,0x00,0x00,0x00,0x00},/*"n",78*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x1E,0x33,0x33,0x33,0x33,0x33,0x1E,0x00,0x00,0x00,0x00},/*"o",79*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x1F,0x33,0x33,0x33,0x33,0x33,0x1F,0x03,0x03,0x03,0x00},/*"p",80*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x3E,0x33,0x33,0x33,0x33,0x33,0x3E,0x30,0x30,0x30,0x00},/*"q",81*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x33,0x3B,0x07,0x03,0x03,0x03,0x03,0x00,0x00,0x00,0x00},/*"r",82*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x3E,0x03,0x03,0x1E,0x30,0x30,0x1F,0x00,0x00,0x00,0x00},/*"s",83*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x06,0x06,0x3F,0x06,0x06,0x06,0x06,0x06,0x3C,0x00,0x00,0x00,0x00},/*"t",84*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x33,0x33,0x33,0x33,0x33,0x33,0x3E,0x00,0x00,0x00,0x00},/*"u",85*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x33,0x33,0x33,0x33,0x33,0x1E,0x0C,0x00,0x00,0x00,0x00},/*"v",86*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x63,0x6B,0x6B,0x6B,0x6B,0x36,0x36,0x00,0x00,0x00,0x00},/*"w",87*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x33,0x33,0x1E,0x0C,0x1E,0x33,0x33,0x00,0x00,0x00,0x00},/*"x",88*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x33,0x33,0x33,0x33,0x33,0x33,0x1E,0x18,0x0C,0x07,0x00},/*"y",89*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x00,0x00,0x3F,0x30,0x18,0x0C,0x06,0x03,0x3F,0x00,0x00,0x00,0x00},/*"z",90*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x18,0x0C,0x0C,0x0C,0x06,0x03,0x06,0x0C,0x0C,0x0C,0x18,0x00,0x00},/*"{",91*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x00},/*"|",92*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x06,0x0C,0x0C,0x0C,0x18,0x30,0x18,0x0C,0x0C,0x0C,0x06,0x00,0x00},/*"}",93*/
    /* (8 X 16 , Fixedsys )*/

    {0x00,0x00,0x00,0x47,0x6D,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"~",94*/
    /* (8 X 16 , Fixedsys )*/

};
/*******************************************************************************/

/*******************************************************************************/

/*******************************************************************************/