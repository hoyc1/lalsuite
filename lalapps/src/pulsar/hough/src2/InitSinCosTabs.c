static REAL4 base[LUT_RES+LUT_RES/4] =
  {
    0x035255B8F,
    0x03CC90E90,
    0x03D48FE6A,
    0x03D96AB47,
    0x03DC8C01C,
	0x03DFAB5FD,
	0x03E16429B,
	0x03E2F130B,
	0x03E47C87C,
	0x03E605F1E,
	0x03E78D326,
	0x03E889068,
	0x03E94A22E,
	0x03EA09D09,
	0x03EAC7F1F,
	0x03EB8469B,
	0x03EC3F1AD,
	0x03ECF7E88,
	0x03EDAEB63,
	0x03EE6367D,
	0x03EF15E16,
	0x03EFC6077,
	0x03F039DF7,
	0x03F08F766,
	0x03F0E3BB6,
	0x03F136A18,
	0x03F1881BE,
	0x03F1D81E0,
	0x03F2269B7,
	0x03F273884,
	0x03F2BED87,
	0x03F308807,
	0x03F35074E,
	0x03F396AAB,
	0x03F3DB170,
	0x03F41DAF5,
	0x03F45E695,
	0x03F49D3B1,
	0x03F4DA1AD,
	0x03F514FF3,
	0x03F54DDF3,
	0x03F584B20,
	0x03F5B96F2,
	0x03F5EC0E7,
	0x03F61C883,
	0x03F64AD4E,
	0x03F676ED5,
	0x03F6A0CAD,
	0x03F6C866C,
	0x03F6EDBB3,
	0x03F710C25,
	0x03F73176A,
	0x03F74FD34,
	0x03F76BD36,
	0x03F78572B,
	0x03F79CAD5,
	0x03F7B17FA,
	0x03F7C3E67,
	0x03F7D3DEE,
	0x03F7E1669,
	0x03F7EC7B4,
	0x03F7F51B6,
	0x03F7FB459,
	0x03F7FEF8D,
	0x03F8001A5,
	0x03F7FEF8D,
	0x03F7FB458,
	0x03F7F51B4,
	0x03F7EC7B2,
	0x03F7E1666,
	0x03F7D3DEB,
	0x03F7C3E64,
	0x03F7B17F6,
	0x03F79CAD1,
	0x03F785726,
	0x03F76BD30,
	0x03F74FD2E,
	0x03F731764,
	0x03F710C1E,
	0x03F6EDBAC,
	0x03F6C8665,
	0x03F6A0CA4,
	0x03F676ECC,
	0x03F64AD45,
	0x03F61C879,
	0x03F5EC0DD,
	0x03F5B96E7,
	0x03F584B15,
	0x03F54DDE8,
	0x03F514FE8,
	0x03F4DA1A0,
	0x03F49D3A4,
	0x03F45E688,
	0x03F41DAE7,
	0x03F3DB162,
	0x03F396A9D,
	0x03F35073F,
	0x03F3087F8,
	0x03F2BED78,
	0x03F273874,
	0x03F2269A7,
	0x03F1D81CF,
	0x03F1881AD,
	0x03F136A07,
	0x03F0E3BA5,
	0x03F08F754,
	0x03F039DE5,
	0x03EFC6053,
	0x03EF15DF1,
	0x03EE63658,
	0x03EDAEB3E,
	0x03ECF7E62,
	0x03EC3F187,
	0x03EB84674,
	0x03EAC7EF8,
	0x03EA09CE1,
	0x03E94A207,
	0x03E889040,
	0x03E78D2D6,
	0x03E605ECD,
	0x03E47C82A,
	0x03E2F12B9,
	0x03E164249,
	0x03DFAB559,
	0x03DC8BF78,
	0x03D96AAA2,
	0x03D48FD20,
	0x03CC90BFA,
	0x0B5255B8F,
	0x0BCC90E90,
	0x0BD48FE6A,
	0x0BD96AB47,
	0x0BDC8C01C,
	0x0BDFAB5FD,
	0x0BE16429B,
	0x0BE2F130B,
	0x0BE47C87C,
	0x0BE605F1E,
	0x0BE78D326,
	0x0BE889068,
	0x0BE94A22E,
	0x0BEA09D09,
	0x0BEAC7F1F,
	0x0BEB8469B,
	0x0BEC3F1AD,
	0x0BECF7E88,
	0x0BEDAEB63,
	0x0BEE6367D,
	0x0BEF15E16,
	0x0BEFC6077,
	0x0BF039DF7,
	0x0BF08F766,
	0x0BF0E3BB6,
	0x0BF136A18,
	0x0BF1881BE,
	0x0BF1D81E0,
	0x0BF2269B7,
	0x0BF273884,
	0x0BF2BED87,
	0x0BF308807,
	0x0BF35074E,
	0x0BF396AAB,
	0x0BF3DB170,
	0x0BF41DAF5,
	0x0BF45E695,
	0x0BF49D3B1,
	0x0BF4DA1AD,
	0x0BF514FF3,
	0x0BF54DDF3,
	0x0BF584B20,
	0x0BF5B96F2,
	0x0BF5EC0E7,
	0x0BF61C883,
	0x0BF64AD4E,
	0x0BF676ED5,
	0x0BF6A0CAD,
	0x0BF6C866C,
	0x0BF6EDBB3,
	0x0BF710C25,
	0x0BF73176A,
	0x0BF74FD34,
	0x0BF76BD36,
	0x0BF78572B,
	0x0BF79CAD5,
	0x0BF7B17FA,
	0x0BF7C3E67,
	0x0BF7D3DEE,
	0x0BF7E1669,
	0x0BF7EC7B4,
	0x0BF7F51B6,
	0x0BF7FB459,
	0x0BF7FEF8D,
	0x0BF8001A5,
	0x0BF7FEF8D,
	0x0BF7FB458,
	0x0BF7F51B4,
	0x0BF7EC7B2,
	0x0BF7E1666,
	0x0BF7D3DEB,
	0x0BF7C3E64,
	0x0BF7B17F6,
	0x0BF79CAD1,
	0x0BF785726,
	0x0BF76BD30,
	0x0BF74FD2E,
	0x0BF731764,
	0x0BF710C1E,
	0x0BF6EDBAC,
	0x0BF6C8665,
	0x0BF6A0CA4,
	0x0BF676ECC,
	0x0BF64AD45,
	0x0BF61C879,
	0x0BF5EC0DD,
	0x0BF5B96E7,
	0x0BF584B15,
	0x0BF54DDE8,
	0x0BF514FE8,
	0x0BF4DA1A0,
	0x0BF49D3A4,
	0x0BF45E688,
	0x0BF41DAE7,
	0x0BF3DB162,
	0x0BF396A9D,
	0x0BF35073F,
	0x0BF3087F8,
	0x0BF2BED78,
	0x0BF273874,
	0x0BF2269A7,
	0x0BF1D81CF,
	0x0BF1881AD,
	0x0BF136A07,
	0x0BF0E3BA5,
	0x0BF08F754,
	0x0BF039DE5,
	0x0BEFC6053,
	0x0BEF15DF1,
	0x0BEE63658,
	0x0BEDAEB3E,
	0x0BECF7E62,
	0x0BEC3F187,
	0x0BEB84674,
	0x0BEAC7EF8,
	0x0BEA09CE1,
	0x0BE94A207,
	0x0BE889040,
	0x0BE78D2D6,
	0x0BE605ECD,
	0x0BE47C82A,
	0x0BE2F12B9,
	0x0BE164249,
	0x0BDFAB559,
	0x0BDC8BF78,
	0x0BD96AAA2,
	0x0BD48FD20,
	0x0BCC90BFA,
	0x035255B8F,
	0x03CC90E90,
	0x03D48FE6A,
	0x03D96AB47,
	0x03DC8C01C,
	0x03DFAB5FD,
	0x03E16429B,
	0x03E2F130B,
	0x03E47C87C,
	0x03E605F1E,
	0x03E78D326,
	0x03E889068,
	0x03E94A22E,
	0x03EA09D09,
	0x03EAC7F1F,
	0x03EB8469B,
	0x03EC3F1AD,
	0x03ECF7E88,
	0x03EDAEB63,
	0x03EE6367D,
	0x03EF15E16,
	0x03EFC6077,
	0x03F039DF7,
	0x03F08F766,
	0x03F0E3BB6,
	0x03F136A18,
	0x03F1881BE,
	0x03F1D81E0,
	0x03F2269B7,
	0x03F273884,
	0x03F2BED87,
	0x03F308807,
	0x03F35074E,
	0x03F396AAB,
	0x03F3DB170,
	0x03F41DAF5,
	0x03F45E695,
	0x03F49D3B1,
	0x03F4DA1AD,
	0x03F514FF3,
	0x03F54DDF3,
	0x03F584B20,
	0x03F5B96F2,
	0x03F5EC0E7,
	0x03F61C883,
	0x03F64AD4E,
	0x03F676ED5,
	0x03F6A0CAD,
	0x03F6C866C,
	0x03F6EDBB3,
	0x03F710C25,
	0x03F73176A,
	0x03F74FD34,
	0x03F76BD36,
	0x03F78572B,
	0x03F79CAD5,
	0x03F7B17FA,
	0x03F7C3E67,
	0x03F7D3DEE,
	0x03F7E1669,
	0x03F7EC7B4,
	0x03F7F51B6,
	0x03F7FB459,
	0x03F7FEF8D
};


  static REAL4 diff[LUT_RES+LUT_RES/4] =
    {
   0x34C90AB0,
   0x34C8EBAF,
   0x34C8ADB3,
   0x34C850C5,
   0x34C7D4F3,
   0x34C73A51,
   0x34C680F5,
   0x34C5A8FE,
   0x34C4B28C,
   0x34C39DC4,
   0x34C26AD3,
   0x34C119E6,
   0x34BFAB33,
   0x34BE1EF1,
   0x34BC755E,
   0x34BAAEBB,
   0x34B8CB4E,
   0x34B6CB63,
   0x34B4AF48,
   0x34B2774F,
   0x34B023D2,
   0x34ADB52B,
   0x34AB2BBB,
   0x34A887E6,
   0x34A5CA13,
   0x34A2F2B0,
   0x34A0022C,
   0x349CF8FC,
   0x3499D797,
   0x34969E79,
   0x34934E21,
   0x348FE711,
   0x348C69D1,
   0x3488D6EA,
   0x34852EEA,
   0x3481725F,
   0x347B43BD,
   0x34737BFD,
   0x346B8EB1,
   0x34637D12,
   0x345B485E,
   0x3452F1DA,
   0x344A7ACF,
   0x3441E48A,
   0x3439305F,
   0x34305FA6,
   0x342773BA,
   0x341E6DFC,
   0x34154FCF,
   0x340C1A9C,
   0x3402CFCF,
   0x33F2E1AA,
   0x33DFFE43,
   0x33CCF851,
   0x33B9D2C3,
   0x33A6908E,
   0x339334AA,
   0x337F8425,
   0x3358778F,
   0x33314997,
   0x330A004A,
   0x32C54367,
   0x326CCFA1,
   0x319DE7DF,
   0xB19DE7DF,
   0xB26CCFA1,
   0xB2C54367,
   0xB30A004A,
   0xB3314997,
   0xB358778F,
   0xB37F8425,
   0xB39334AA,
   0xB3A6908E,
   0xB3B9D2C3,
   0xB3CCF851,
   0xB3DFFE43,
   0xB3F2E1AA,
   0xB402CFCF,
   0xB40C1A9C,
   0xB4154FCF,
   0xB41E6DFC,
   0xB42773BA,
   0xB4305FA6,
   0xB439305F,
   0xB441E48A,
   0xB44A7ACF,
   0xB452F1DA,
   0xB45B485E,
   0xB4637D12,
   0xB46B8EB1,
   0xB4737BFD,
   0xB47B43BD,
   0xB481725F,
   0xB4852EEA,
   0xB488D6EA,
   0xB48C69D1,
   0xB48FE711,
   0xB4934E21,
   0xB4969E79,
   0xB499D797,
   0xB49CF8FC,
   0xB4A0022C,
   0xB4A2F2B0,
   0xB4A5CA13,
   0xB4A887E6,
   0xB4AB2BBB,
   0xB4ADB52B,
   0xB4B023D2,
   0xB4B2774F,
   0xB4B4AF48,
   0xB4B6CB63,
   0xB4B8CB4E,
   0xB4BAAEBB,
   0xB4BC755E,
   0xB4BE1EF1,
   0xB4BFAB33,
   0xB4C119E6,
   0xB4C26AD3,
   0xB4C39DC4,
   0xB4C4B28C,
   0xB4C5A8FE,
   0xB4C680F5,
   0xB4C73A51,
   0xB4C7D4F3,
   0xB4C850C5,
   0xB4C8ADB3,
   0xB4C8EBAF,
   0xB4C90AB0,
   0xB4C90AB0,
   0xB4C8EBAF,
   0xB4C8ADB3,
   0xB4C850C5,
   0xB4C7D4F3,
   0xB4C73A51,
   0xB4C680F5,
   0xB4C5A8FE,
   0xB4C4B28C,
   0xB4C39DC4,
   0xB4C26AD3,
   0xB4C119E6,
   0xB4BFAB33,
   0xB4BE1EF1,
   0xB4BC755E,
   0xB4BAAEBB,
   0xB4B8CB4E,
   0xB4B6CB63,
   0xB4B4AF48,
   0xB4B2774F,
   0xB4B023D2,
   0xB4ADB52B,
   0xB4AB2BBB,
   0xB4A887E6,
   0xB4A5CA13,
   0xB4A2F2B0,
   0xB4A0022C,
   0xB49CF8FC,
   0xB499D797,
   0xB4969E79,
   0xB4934E21,
   0xB48FE711,
   0xB48C69D1,
   0xB488D6EA,
   0xB4852EEA,
   0xB481725F,
   0xB47B43BD,
   0xB4737BFD,
   0xB46B8EB1,
   0xB4637D12,
   0xB45B485E,
   0xB452F1DA,
   0xB44A7ACF,
   0xB441E48A,
   0xB439305F,
   0xB4305FA6,
   0xB42773BA,
   0xB41E6DFC,
   0xB4154FCF,
   0xB40C1A9C,
   0xB402CFCF,
   0xB3F2E1AA,
   0xB3DFFE43,
   0xB3CCF851,
   0xB3B9D2C3,
   0xB3A6908E,
   0xB39334AA,
   0xB37F8425,
   0xB358778F,
   0xB3314997,
   0xB30A004A,
   0xB2C54367,
   0xB26CCFA1,
   0xB19DE7DF,
   0x319DE7DF,
   0x326CCFA1,
   0x32C54367,
   0x330A004A,
   0x33314997,
   0x3358778F,
   0x337F8425,
   0x339334AA,
   0x33A6908E,
   0x33B9D2C3,
   0x33CCF851,
   0x33DFFE43,
   0x33F2E1AA,
   0x3402CFCF,
   0x340C1A9C,
   0x34154FCF,
   0x341E6DFC,
   0x342773BA,
   0x34305FA6,
   0x3439305F,
   0x3441E48A,
   0x344A7ACF,
   0x3452F1DA,
   0x345B485E,
   0x34637D12,
   0x346B8EB1,
   0x34737BFD,
   0x347B43BD,
   0x3481725F,
   0x34852EEA,
   0x3488D6EA,
   0x348C69D1,
   0x348FE711,
   0x34934E21,
   0x34969E79,
   0x3499D797,
   0x349CF8FC,
   0x34A0022C,
   0x34A2F2B0,
   0x34A5CA13,
   0x34A887E6,
   0x34AB2BBB,
   0x34ADB52B,
   0x34B023D2,
   0x34B2774F,
   0x34B4AF48,
   0x34B6CB63,
   0x34B8CB4E,
   0x34BAAEBB,
   0x34BC755E,
   0x34BE1EF1,
   0x34BFAB33,
   0x34C119E6,
   0x34C26AD3,
   0x34C39DC4,
   0x34C4B28C,
   0x34C5A8FE,
   0x34C680F5,
   0x34C73A51,
   0x34C7D4F3,
   0x34C850C5,
   0x34C8ADB3,
   0x34C8EBAF,
   0x34C90AB0,
   0x34C90AB0,
   0x34C8EBAF,
   0x34C8ADB3,
   0x34C850C5,
   0x34C7D4F3,
   0x34C73A51,
   0x34C680F5,
   0x34C5A8FE,
   0x34C4B28C,
   0x34C39DC4,
   0x34C26AD3,
   0x34C119E6,
   0x34BFAB33,
   0x34BE1EF1,
   0x34BC755E,
   0x34BAAEBB,
   0x34B8CB4E,
   0x34B6CB63,
   0x34B4AF48,
   0x34B2774F,
   0x34B023D2,
   0x34ADB52B,
   0x34AB2BBB,
   0x34A887E6,
   0x34A5CA13,
   0x34A2F2B0,
   0x34A0022C,
   0x349CF8FC,
   0x3499D797,
   0x34969E79,
   0x34934E21,
   0x348FE711,
   0x348C69D1,
   0x3488D6EA,
   0x34852EEA,
   0x3481725F,
   0x347B43BD,
   0x34737BFD,
   0x346B8EB1,
   0x34637D12,
   0x345B485E,
   0x3452F1DA,
   0x344A7ACF,
   0x3441E48A,
   0x3439305F,
   0x34305FA6,
   0x342773BA,
   0x341E6DFC,
   0x34154FCF,
   0x340C1A9C,
   0x3402CFCF,
   0x33F2E1AA,
   0x33DFFE43,
   0x33CCF851,
   0x33B9D2C3, 
   0x33A6908E,
   0x339334AA,
   0x337F8425,
   0x3358778F,
   0x33314997,
   0x330A004A,
   0x32C54367,
   0x326CCFA1,
   0x319DE7DF
   };
