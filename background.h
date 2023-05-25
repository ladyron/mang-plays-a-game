
//{{BLOCK(background)

//======================================================================
//
//	background, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 49 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 3136 + 2048 = 5696
//
//	Time-stamp: 2022-12-09, 15:47:11
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BACKGROUND_H
#define GRIT_BACKGROUND_H

#define backgroundTilesLen 3136
extern const unsigned short backgroundTiles[1568];

#define backgroundMapLen 2048
extern const unsigned short backgroundMap[1024];

#define backgroundPalLen 512
extern const unsigned short backgroundPal[256];

#endif // GRIT_BACKGROUND_H

//}}BLOCK(background)
