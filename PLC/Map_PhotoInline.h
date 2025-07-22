#ifndef	_FPDCIMPhotoInlineMap_h__
#define	_FPDCIMPhotoInlineMap_h__

/* Bit Area */
// UPPER IO
#define	B_L2_FROM_UPPER1_HS				0x2080 // Handshake
#define	B_L2_FROM_UPPER1_CS				0x20A0 // Contact State

#define	B_L2_FROM_UPPER2_HS				0x20C0 // Handshake
#define	B_L2_FROM_UPPER2_CS				0x20E0 // Contact State

#define	B_L2_FROM_UPPER3_HS				0x2100 // Handshake
#define	B_L2_FROM_UPPER3_CS				0x2120 // Contact State

// OWN EQ IO
#define	B_L2_TO_UPPER1_HS				0x2140 // Handshake
#define	B_L2_TO_UPPER1_CS				0x2160 // Contact State

#define	B_L2_TO_UPPER2_HS				0x2180 // Handshake
#define	B_L2_TO_UPPER2_CS				0x21A0 // Contact State

#define	B_L2_TO_UPPER3_HS				0x21C0 // Handshake
#define	B_L2_TO_UPPER3_CS				0x21E0 // Contact State

#define	B_L2_TO_LOWER1_HS				0x2200 // Handshake
#define	B_L2_TO_LOWER1_CS				0x2220 // Contact State

#define	B_L2_TO_LOWER2_HS				0x2240 // Handshake
#define	B_L2_TO_LOWER2_CS				0x2260 // Contact State

#define	B_L2_TO_LOWER3_HS				0x2280 // Handshake
#define	B_L2_TO_LOWER3_CS				0x22A0 // Contact State

// LOWER IO
#define	B_L2_FROM_LOWER1_HS				0x22C0 // Handshake
#define	B_L2_FROM_LOWER1_CS				0x22E0 // Contact State

#define	B_L2_FROM_LOWER2_HS				0x2300 // Handshake
#define	B_L2_FROM_LOWER2_CS				0x2320 // Contact State

#define	B_L2_FROM_LOWER3_HS				0x2340 // Handshake
#define	B_L2_FROM_LOWER3_CS				0x2360 // Contact State

#define	B_EACH_STATE_INTERVAL			0x0180
#define	B_EACH_LAYER_INLINE_INTERVAL	0x0670
#define B_EACH_HS_INTERVAL				0x0080 // HandShake Log ¿ë
#define B_EACH_UP_TO_LOWER_INTERVAL		0x0300 // HandShake Log ¿ë

/* Word Area */


// SonJaeWon 080310
// FROM UPPER DATA
#define	W_L2_FROM_UPPER1_GLASS_DATA		0x4D00
#define	W_L2_FROM_UPPER2_GLASS_DATA		0x4DB4
#define	W_L2_FROM_UPPER3_GLASS_DATA		0x4E68

// OWN EQ DATA
#define	W_L2_TO_UPPER1					0x4F1C
#define	W_L2_TO_UPPER2					0x4F2C
#define	W_L2_TO_UPPER3					0x4F3C

// TO LOWER DATA
#define	W_L2_TO_LOWER1_GLASS_DATA		0x4F4C
#define	W_L2_TO_LOWER2_GLASS_DATA		0x5000
#define	W_L2_TO_LOWER3_GLASS_DATA		0x50B4

// OWN EQ STATE
#define	W_L2_OWN_EQ_STATE				0x5168
#define	W_EACH_LAYER_INLINE_INTERVAL	0x1D00

#endif
