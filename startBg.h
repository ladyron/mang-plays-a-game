
//{{BLOCK(startBg)

//======================================================================
//
//	startBg, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 228 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 14592 + 2048 = 17152
//
//	Time-stamp: 2022-11-09, 15:48:40
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_STARTBG_H
#define GRIT_STARTBG_H

#define startBgTilesLen 14592
extern const unsigned short startBgTiles[7296];

#define startBgMapLen 2048
extern const unsigned short startBgMap[1024];

#define startBgPalLen 512
extern const unsigned short startBgPal[256];

#endif // GRIT_STARTBG_H

//}}BLOCK(startBg)
