
//{{BLOCK(winBg)

//======================================================================
//
//	winBg, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 255 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 16320 + 2048 = 18880
//
//	Time-stamp: 2022-12-09, 22:11:52
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_WINBG_H
#define GRIT_WINBG_H

#define winBgTilesLen 16320
extern const unsigned short winBgTiles[8160];

#define winBgMapLen 2048
extern const unsigned short winBgMap[1024];

#define winBgPalLen 512
extern const unsigned short winBgPal[256];

#endif // GRIT_WINBG_H

//}}BLOCK(winBg)
