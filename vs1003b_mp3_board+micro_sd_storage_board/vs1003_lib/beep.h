/*
MP3 Header

0xff, 0xe2, 0x19, 0xc0

1111 1111

111 '0 0' '01' '0'     => MPEG2.5 / LayerIII / CRC off

'0001' '10' '0' '1'    => 8kbit bitrate/ 12000 Freq / Padding bit / x 

'11' '00' '0' '0' '00' => Mono channel / x / copyright / original / none
*/

unsigned char szBeepMP3[] = // 432   
{   
    0xff, 0xe2, 0x19, 0xc0, 0xd4, 0x80, 0x00, 0x0a, 0x61, 0x76, 0x72, 0xe9, 0x41, 0x30, 0x01, 0x0d,   
    0xbe, 0x90, 0xcc, 0x13, 0x0f, 0xc6, 0xe3, 0xf8, 0xdf, 0xfe, 0x05, 0xfc, 0x0b, 0xc0, 0xab, 0xc8,   
    0x0b, 0xff, 0xff, 0xf6, 0x8b, 0xbf, 0xe2, 0x23, 0x2c, 0x10, 0x82, 0x18, 0xf7, 0x7a, 0xd7, 0x77,   
    0xad, 0x11, 0x8e, 0x61, 0x04, 0x00, 0x3a, 0xf0, 0xff, 0xb0, 0x04, 0xb1, 0x00, 0x00, 0x06, 0x59,   
    0xc3, 0x99, 0x00, 0x00, 0x70, 0x0b, 0x80, 0x00, 0xff, 0xe2, 0x19, 0xc0, 0xc6, 0xe8, 0x07, 0x0b,       
		0x11, 0x9e, 0xee, 0xf9, 0x81, 0x68, 0x02, 0x01, 0xb8, 0x00, 0x00, 0x39, 0x42, 0x12, 0xff, 0xff,   
		0x70, 0x0f, 0xff, 0xae, 0xbf, 0xab, 0xfe, 0xa4, 0x1b, 0xf0, 0xe5, 0x37, 0xd6, 0x1d, 0x7e, 0xa6,   
    0x7f, 0xe3, 0x30, 0xdf, 0xfe, 0x33, 0x0e, 0xbc, 0xb1, 0x97, 0xf5, 0x07, 0x7b, 0x27, 0xff, 0xff,   
    0xff, 0x25, 0x5d, 0xb8, 0xce, 0x9b, 0x0a, 0x7a, 0x9b, 0x96, 0x81, 0xaf, 0x92, 0x02, 0x83, 0x97,   
    0xff, 0xe2, 0x19, 0xc0, 0x06, 0x63, 0x13, 0x0b, 0x79, 0x7e, 0x90, 0x21, 0xc0, 0xd8, 0x00, 0xb4,   
    0xa6, 0xd4, 0xa6, 0x97, 0x1f, 0xff, 0xfe, 0x63, 0x84, 0xa9, 0x4a, 0x93, 0xe8, 0xaa, 0xe0, 0x7a,   
    0xa0, 0xe5, 0xaa, 0x4e, 0xa6, 0xb2, 0xea, 0xbc, 0x77, 0xf5, 0x00, 0xdd, 0xb0, 0x18, 0x03, 0xff,   
    0xf5, 0x90, 0x1e, 0x72, 0x2e, 0x6f, 0xff, 0xfe, 0x7c, 0xc7, 0xff, 0xa0, 0x81, 0x4c, 0x52, 0x60,   
    0x64, 0x4f, 0x09, 0x88, 0xcd, 0x93, 0xe6, 0xff, 0xff, 0xe2, 0x19, 0xc0, 0xcd, 0x5a, 0x1e, 0x0b,   
    0x69, 0x76, 0xba, 0xe0, 0x08, 0x68, 0x6c, 0xf9, 0x99, 0xba, 0x41, 0xfa, 0x00, 0x61, 0x80, 0x2d,   
    0xe8, 0xa0, 0x33, 0x05, 0x77, 0x35, 0x4f, 0x1b, 0x5b, 0x38, 0x00, 0x07, 0x1f, 0xf9, 0x85, 0x7f,   
    0xcc, 0x3f, 0x3f, 0x0a, 0xf9, 0xaf, 0xf8, 0x43, 0xff, 0xff, 0x35, 0xd6, 0xe1, 0x2b, 0x8d, 0x21,   
    0x39, 0x00, 0x64, 0x69, 0x05, 0x74, 0xf0, 0x77, 0x9d, 0x5b, 0x7f, 0xe2, 0xdf, 0x2c, 0x25, 0xf4,   
    0xff, 0xe2, 0x19, 0xc0, 0x22, 0x06, 0x29, 0x0f, 0x09, 0x7a, 0xa2, 0x38, 0x08, 0x5e, 0x6e, 0xe8,   
    0x00, 0x3c, 0x2d, 0x60, 0xe5, 0x3c, 0x71, 0x77, 0xba, 0x12, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x43,   
    0xdf, 0x0d, 0x5f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xe2, 0x19, 0xc0, 0xbc, 0xd1, 0x25, 0x00,   
    0x00, 0x02, 0x5c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00    
}; 

unsigned char test_mp3[] = {
  0xff, 0xf3, 0x80, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0x69, 0x6e,
  0x67, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x03,
  0x26, 0x00, 0x0b, 0x0b, 0x0b, 0x0b, 0x16, 0x16, 0x16, 0x16, 0x16, 0x2c,
  0x2c, 0x2c, 0x2c, 0x2c, 0x37, 0x37, 0x37, 0x37, 0x37, 0x42, 0x42, 0x42,
  0x42, 0x4d, 0x4d, 0x4d, 0x4d, 0x4d, 0x59, 0x59, 0x59, 0x59, 0x59, 0x64,
  0x64, 0x64, 0x64, 0x64, 0x6f, 0x6f, 0x6f, 0x6f, 0x7a, 0x7a, 0x7a, 0x7a,
  0x7a, 0x85, 0x85, 0x85, 0x85, 0x85, 0x90, 0x90, 0x90, 0x90, 0x90, 0x9b,
  0x9b, 0x9b, 0x9b, 0xa6, 0xa6, 0xa6, 0xa6, 0xa6, 0xb2, 0xb2, 0xb2, 0xb2,
  0xb2, 0xbd, 0xbd, 0xbd, 0xbd, 0xbd, 0xc8, 0xc8, 0xc8, 0xc8, 0xde, 0xde,
  0xde, 0xde, 0xde, 0xe9, 0xe9, 0xe9, 0xe9, 0xe9, 0xf4, 0xf4, 0xf4, 0xf4,
  0xf4, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x39, 0x4c, 0x41, 0x4d,
  0x45, 0x33, 0x2e, 0x39, 0x39, 0x72, 0x02, 0x14, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x14, 0x08, 0x24, 0x02, 0x80, 0x26, 0x00, 0x00,
  0x08, 0x00, 0x00, 0x03, 0x26, 0xc5, 0x2c, 0xe4, 0x6d, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xf3, 0x10, 0x04, 0x00, 0x00, 0x00, 0x01,
  0x52, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x02, 0xa8, 0x01, 0x40, 0x00,
  0x00, 0xff, 0x21, 0x01, 0x3b, 0xe8, 0xff, 0xf3, 0x10, 0x04, 0x05, 0x00,
  0xa0, 0x17, 0x8a, 0x00, 0xc1, 0x08, 0x00, 0x01, 0x40, 0x2f, 0x14, 0x01,
  0x82, 0x10, 0x00, 0xff, 0x21, 0x01, 0x3b, 0xe8, 0xff, 0xf3, 0x20, 0x04,
  0x00, 0x00, 0xb8, 0x27, 0x80, 0x00, 0xe3, 0x80, 0x00, 0x01, 0x78, 0x4f,
  0x00, 0x01, 0xc7, 0x00, 0x00, 0xff, 0x14, 0x07, 0x37, 0xec, 0x5b, 0xfc,
  0x50, 0x0c, 0x6f, 0xd8, 0xb2, 0xd0, 0x6f, 0x0d, 0xe8, 0x37, 0x86, 0xea,
  0x1b, 0xfa, 0x8f, 0x6f, 0x10, 0x6a, 0x37, 0xf5, 0x06, 0xf1, 0x05, 0xd4,
  0xff, 0xf3, 0x10, 0x04, 0x13, 0x00, 0x64, 0x15, 0x90, 0x00, 0x00, 0x02,
  0x00, 0x00, 0xc8, 0x2b, 0x20, 0x00, 0x00, 0x04, 0x00, 0x6f, 0xe8, 0x3b,
  0xce, 0xe6, 0xff, 0xf3, 0x10, 0x04, 0x11, 0x00, 0xa4, 0x29, 0x86, 0x00,
  0x00, 0x49, 0x24, 0x01, 0x60, 0x53, 0x14, 0x00, 0x00, 0x8a, 0x24, 0xbf,
  0x90, 0x7f, 0x3a, 0xcf, 0xff, 0xf3, 0x10, 0x04, 0x0b, 0x00, 0xa4, 0x27,
  0x88, 0x00, 0x00, 0x46, 0x16, 0x01, 0x30, 0x4f, 0x14, 0x00, 0x00, 0x4c,
  0x2c, 0xfd, 0x04, 0x7a, 0x73, 0xff, 0xff, 0xf3, 0x10, 0x04, 0x06, 0x00,
  0x88, 0x29, 0x8a, 0x00, 0x00, 0x22, 0x12, 0x01, 0x18, 0x53, 0x14, 0x00,
  0x00, 0x44, 0x24, 0x40, 0x0f, 0x4d, 0xff, 0x5f, 0xff, 0xf3, 0x10, 0x04,
  0x02, 0x00, 0x34, 0x03, 0x98, 0x00, 0x00, 0x22, 0x00, 0x00, 0x88, 0x07,
  0x34, 0x00, 0x00, 0x84, 0x00, 0xbf, 0xad, 0xf7, 0xf4, 0xfb, 0xff, 0xf3,
  0x10, 0x04, 0x03, 0x00, 0x44, 0x03, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x88, 0x07, 0x34, 0x00, 0x00, 0x00, 0x00, 0xfa, 0x55, 0xf7, 0xf4, 0xff,
  0xff, 0xf3, 0x10, 0x04, 0x03, 0x00, 0x44, 0x03, 0x94, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x68, 0x07, 0x2c, 0x00, 0x00, 0x00, 0x00, 0xa5, 0xf6, 0x75,
  0x7b, 0x3a, 0xff, 0xf3, 0x10, 0x04, 0x04, 0x00, 0x44, 0x01, 0x90, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x88, 0x03, 0x24, 0x00, 0x00, 0x00, 0x00, 0x95,
  0x11, 0xee, 0xe8, 0xf4, 0xff, 0xf3, 0x10, 0x04, 0x04, 0x00, 0x1c, 0x15,
  0x8e, 0x00, 0x00, 0x02, 0x02, 0x00, 0x88, 0x07, 0x28, 0x00, 0x00, 0x00,
  0x00, 0x79, 0x05, 0xf4, 0x7a, 0x15, 0xff, 0xf3, 0x10, 0x04, 0x06, 0x00,
  0x24, 0x03, 0x98, 0x00, 0x00, 0x02, 0x00, 0x00, 0x58, 0x07, 0x30, 0x00,
  0x00, 0x00, 0x00, 0xff, 0xc9, 0x7f, 0xe4, 0x95, 0xff, 0xf3, 0x10, 0x04,
  0x08, 0x00, 0x24, 0x03, 0x9a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x07,
  0x34, 0x00, 0x00, 0x00, 0x00, 0x8d, 0xff, 0x0b, 0x0c, 0xff, 0xff, 0xf3,
  0x10, 0x04, 0x0a, 0x00, 0x44, 0x01, 0x9c, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x88, 0x03, 0x38, 0x00, 0x00, 0x00, 0x00, 0x80, 0xbf, 0x24, 0x23, 0xff,
  0xff, 0xf3, 0x10, 0x04, 0x0a, 0x00, 0x5c, 0x2b, 0x8c, 0x00, 0x00, 0x02,
  0x02, 0x01, 0x40, 0x5b, 0x0c, 0x00, 0x00, 0x0a, 0x04, 0xae, 0x15, 0xe5,
  0x50, 0xbf, 0xff, 0xf3, 0x10, 0x04, 0x07, 0x00, 0xa8, 0x2b, 0x86, 0x00,
  0x00, 0x26, 0x24, 0x01, 0x10, 0x5b, 0x10, 0x00, 0x00, 0x0a, 0x04, 0xea,
  0x06, 0x7a, 0x40, 0xdf, 0xff, 0xf3, 0x20, 0x04, 0x02, 0x00, 0xac, 0x2d,
  0x80, 0x00, 0x00, 0xab, 0x24, 0x01, 0x48, 0x5b, 0x04, 0x00, 0x01, 0x4e,
  0x40, 0xd3, 0x54, 0x61, 0x48, 0x0f, 0xfe, 0x54, 0x10, 0x6a, 0x50, 0xd7,
  0xf5, 0xa0, 0x2d, 0x40, 0xb7, 0xfb, 0x01, 0xa9, 0x06, 0x27, 0xfe, 0x18,
  0x70, 0x62, 0x7f, 0xd0, 0x30, 0xea, 0xf5, 0xd5, 0xff, 0xf3, 0x10, 0x04,
  0x16, 0x00, 0x98, 0x2b, 0x7a, 0x00, 0x00, 0xe8, 0x24, 0x01, 0x48, 0x5a,
  0xf4, 0x00, 0x00, 0x0e, 0x04, 0x4c, 0x41, 0x4d, 0x45, 0x33, 0xff, 0xf3,
  0x10, 0x04, 0x11, 0x00, 0x90, 0x2d, 0x76, 0x00, 0x00, 0x02, 0x02, 0x01,
  0x38, 0x5a, 0xf0, 0x00, 0x00, 0x04, 0x04, 0x2e, 0x39, 0x39, 0x2e, 0x33,
  0xff, 0xf3, 0x10, 0x04, 0x0c, 0x00, 0x24, 0x01, 0x68, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x02, 0xd4, 0x00, 0x00, 0x00, 0x00, 0x55, 0x55, 0x55,
  0x55, 0x55, 0x41, 0x50, 0x45, 0x54, 0x41, 0x47, 0x45, 0x58, 0xd0, 0x07,
  0x00, 0x00, 0xae, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4d, 0x50, 0x33, 0x47, 0x41, 0x49,
  0x4e, 0x5f, 0x4d, 0x49, 0x4e, 0x4d, 0x41, 0x58, 0x00, 0x31, 0x36, 0x39,
  0x2c, 0x32, 0x30, 0x36, 0x0b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x4d, 0x50, 0x33, 0x47, 0x41, 0x49, 0x4e, 0x5f, 0x55, 0x4e, 0x44, 0x4f,
  0x00, 0x2d, 0x30, 0x30, 0x37, 0x2c, 0x2d, 0x30, 0x30, 0x37, 0x2c, 0x4e,
  0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 0x45, 0x50, 0x4c,
  0x41, 0x59, 0x47, 0x41, 0x49, 0x4e, 0x5f, 0x54, 0x52, 0x41, 0x43, 0x4b,
  0x5f, 0x47, 0x41, 0x49, 0x4e, 0x00, 0x2d, 0x31, 0x30, 0x2e, 0x34, 0x37,
  0x35, 0x30, 0x30, 0x20, 0x64, 0x42, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x52, 0x45, 0x50, 0x4c, 0x41, 0x59, 0x47, 0x41, 0x49, 0x4e,
  0x5f, 0x54, 0x52, 0x41, 0x43, 0x4b, 0x5f, 0x50, 0x45, 0x41, 0x4b, 0x00,
  0x31, 0x2e, 0x30, 0x33, 0x35, 0x35, 0x32, 0x30, 0x41, 0x50, 0x45, 0x54,
  0x41, 0x47, 0x45, 0x58, 0xd0, 0x07, 0x00, 0x00, 0xae, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};
