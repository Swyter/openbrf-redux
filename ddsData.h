/* OpenBRF -- by marco tarini. Provided under GNU General Public License */

#ifndef DDSDATA_H
#define DDSDATA_H

typedef struct {
  unsigned int bind;
  int sx;
  int sy;
  int mipmap;
  int filesize;
  int ddxversion;
  int location;
  bool pixel_data_present;
  float r, g, b, a; /* swy: average color of the last mipmap; for content detection like distinguishing between green and blue normalmaps */
} DdsData;

#endif // DDSDATA_H
