
//{{BLOCK(gameBg)

//======================================================================
//
//	gameBg, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 879 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 56256 + 2048 = 58816
//
//	Time-stamp: 2022-12-09, 16:04:04
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_GAMEBG_H
#define GRIT_GAMEBG_H

#define gameBgTilesLen 56256
extern const unsigned short gameBgTiles[28128];

#define gameBgMapLen 2048
extern const unsigned short gameBgMap[1024];

#define gameBgPalLen 512
extern const unsigned short gameBgPal[256];

#endif // GRIT_GAMEBG_H

//}}BLOCK(gameBg)
