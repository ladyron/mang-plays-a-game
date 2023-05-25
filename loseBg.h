
//{{BLOCK(loseBg)

//======================================================================
//
//	loseBg, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 210 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 13440 + 2048 = 16000
//
//	Time-stamp: 2022-12-09, 22:16:14
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_LOSEBG_H
#define GRIT_LOSEBG_H

#define loseBgTilesLen 13440
extern const unsigned short loseBgTiles[6720];

#define loseBgMapLen 2048
extern const unsigned short loseBgMap[1024];

#define loseBgPalLen 512
extern const unsigned short loseBgPal[256];

#endif // GRIT_LOSEBG_H

//}}BLOCK(loseBg)
