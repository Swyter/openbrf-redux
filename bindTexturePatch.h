/* OpenBRF -- by marco tarini. Provided under GNU General Public License */

#ifndef BINDTEXTUREPATCH_H
#define BINDTEXTUREPATCH_H

// DDS format structure
struct DDSFormat {
    quint32 dwSize;
    quint32 dwFlags;
    quint32 dwHeight;
    quint32 dwWidth;
    quint32 dwLinearSize;
    quint32 dummy1;
    quint32 dwMipMapCount;
    quint32 dummy2[11];
    struct {
        quint32 size, flags;
        quint32 dwFourCC;
        quint32 rgbBitCount, rBitMask, gBitMask, bBitMask, aBitMask;
    } ddsPixelFormat;
    quint32 caps, caps2, caps3, caps4, reserved;
};

enum DDSPixelFmtFlags {
  DDPF_ALPHAPIXELS =     0x1,
  DDPF_ALPHA       =     0x2,
  DDPF_FOURCC      =     0x4,
  DDPF_RGB         =    0x40,
  DDPF_YUV         =   0x200,
  DDPF_LUMINANCE   = 0x20000
};

// compressed texture pixel formats
#define FOURCC_DXT1  0x31545844
#define FOURCC_DXT2  0x32545844
#define FOURCC_DXT3  0x33545844
#define FOURCC_DXT4  0x34545844
#define FOURCC_DXT5  0x35545844

#ifndef GL_COMPRESSED_RGB_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT   0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT  0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT  0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  0x83F3
#endif

#ifndef GL_GENERATE_MIPMAP_SGIS
#define GL_GENERATE_MIPMAP_SGIS       0x8191
#define GL_GENERATE_MIPMAP_HINT_SGIS  0x8192
#endif




/*
typedef void (APIENTRY *pfn_glCompressedTexImage2DARB) (GLenum, GLint, GLenum, GLsizei,
                                                        GLsizei, GLint, GLsizei, const GLvoid *);
static pfn_glCompressedTexImage2DARB qt_glCompressedTexImage2DARB = 0;
*/

typedef QHash<QString, DdsData> QGLDDSCache;
Q_GLOBAL_STATIC(QGLDDSCache, qgl_dds_cache)
;

static void forgetChachedTextures(){
  QGLDDSCache::iterator it;
  for (it=qgl_dds_cache()->begin(); it!=qgl_dds_cache()->end(); it++){
    unsigned int b = it.value().bind;
    if (b>0) glDeleteTextures(1,&b);
  }
  qgl_dds_cache()->clear();
}

bool loadDDSHeader(QFile &f, DdsData &data,  DDSFormat &ddsHeader){
  int factor =4;

  f.open(QIODevice::ReadOnly);

  data.sx = data.sy = data.bind = data.mipmap = data.filesize = data.ddxversion=0;

  char tag[4];
  f.read(&tag[0], 4);
  if (strncmp(tag,"DDS ", 4) != 0) {
      qWarning("QGLContext::bindTexture(): not a DDS image file.");
      goto fail;
  }

  f.read((char *) &ddsHeader, sizeof(DDSFormat));

  /* swy: size of the ddsPixelFormat structure */
  if (ddsHeader.ddsPixelFormat.size != 32)
    goto fail;

  /* swy: a fourCC is a four letter code like DXT1 that signals a compressed format */
  if (ddsHeader.ddsPixelFormat.flags & DDPF_FOURCC) {
    switch(ddsHeader.ddsPixelFormat.dwFourCC) {
    default:
        qWarning("QGLContext::bindTexture() DDS image format not supported.");
        goto fail;
    case FOURCC_DXT1:
        factor = 2;
        data.ddxversion=1;
        break;
    case FOURCC_DXT3:
        data.ddxversion=3;
        break;
    case FOURCC_DXT5:
        data.ddxversion=5;
        break;
    }
  /* swy: otherwise we only support uncompressed RGBA textures that use a standard BGR/A swizzling mask, simpler */
  } else if (ddsHeader.ddsPixelFormat.flags & (DDPF_RGB | DDPF_ALPHAPIXELS)) {
    bool has_alpha = (ddsHeader.ddsPixelFormat.flags & DDPF_ALPHAPIXELS) && ddsHeader.ddsPixelFormat.aBitMask != 0;
    data.ddxversion= has_alpha ? -0 : -1;
  }

  if (ddsHeader.dwMipMapCount == 0) ddsHeader.dwMipMapCount=1;

  data.mipmap=ddsHeader.dwMipMapCount;


  int bufferSize;
  if (ddsHeader.dwMipMapCount > 1)
      bufferSize = ddsHeader.dwLinearSize * factor;
  else
      bufferSize = ddsHeader.dwLinearSize;

  data.filesize= f.size(); //bufferSize+4+sizeof(ddsHeader);
  data.sx = ddsHeader.dwWidth;
  data.sy = ddsHeader.dwHeight;

  return true;

fail:
  return false;
}


bool loadOnlyDDSHeader(const QString &fileName, DdsData &data){
  QFile f(fileName);
  DDSFormat unused;

  return loadDDSHeader(f,data,unused);
}

bool GLWidget::myBindTexture(const QString &fileName, DdsData &data)
{
    /*
    if (!qt_glCompressedTexImage2DARB) {
      QGLContext cx(QGLFormat::defaultFormat());
      cx.getProcAddress(QLatin1String("glCompressedTexImage2D"));
      qt_glCompressedTexImage2DARB = (pfn_glCompressedTexImage2DARB) cx.getProcAddress(QLatin1String("glCompressedTexImage2DARB"));
    }
    */

    QGLDDSCache::const_iterator it = qgl_dds_cache()->constFind(fileName);
    if (it != qgl_dds_cache()->constEnd()) {
      data = it.value();
      glBindTexture(GL_TEXTURE_2D, data.bind);
      return true;
    }

    QFile f(fileName);

    DDSFormat ddsHeader;
    if (!loadDDSHeader(f, data, ddsHeader )) return false;

    int blockSize = 16;
    GLenum intFormat=4, format=0, type=0, pixelSize=0;
    int factor = 4;

    switch(data.ddxversion){
    case  1: intFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; blockSize=8; factor = 2; break;
    case  3: intFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; break;
    case  5: intFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; break;
    case -0: intFormat = GL_RGBA; format = GL_BGRA; type = GL_UNSIGNED_INT_8_8_8_8_REV; pixelSize = 4 /* swy: 32 bits, 4 bytes */; break;
    case -1: intFormat = GL_RGB;  format = GL_BGR;  type = GL_UNSIGNED_BYTE;            pixelSize = 3 /* swy: 24 bits, 3 bytes */; break;
    }

    if (!ddsHeader.dwLinearSize) {
        //qWarning("QGLContext::bindTexture() DDS image size is not valid.");
        //return 0;
        ddsHeader.dwLinearSize = ddsHeader.dwHeight * ddsHeader.dwWidth * factor / 4;
    }

    int bufferSize;
    if (ddsHeader.dwMipMapCount > 1)
        bufferSize = ddsHeader.dwLinearSize * factor;
    else
        bufferSize = ddsHeader.dwLinearSize;

    /* swy: for RGBA8 or RGB8 textures just allocate a temp chunk of memory as big
            as the file itself, who cares, less code */
    if (data.ddxversion <= 0)
      bufferSize = data.filesize;


    GLubyte *pixels = (GLubyte *) malloc(bufferSize*sizeof(GLubyte));

    if (!pixels){
      qWarning("QGLContext::bindTexture() could not allocate enough memory (%u bytes) to load the DDS image data.", bufferSize);
      return 0;
    }

    f.seek(ddsHeader.dwSize + 4);
    f.read((char *) pixels, bufferSize);
    f.close();

    GLuint tx_id;
    glGenTextures(1, &tx_id);

    data.bind=tx_id;

    glBindTexture(GL_TEXTURE_2D, tx_id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (((1<<(ddsHeader.dwMipMapCount-1))>=(int)ddsHeader.dwWidth) &&
        ((1<<(ddsHeader.dwMipMapCount-1))>=(int)ddsHeader.dwHeight)) {
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      //test: glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    } else
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if (GLWidget::maxSupportedTexAnisoTaps > 1)
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, GLWidget::maxSupportedTexAnisoTaps);

    int offset = 0;
    int w = ddsHeader.dwWidth;
    int h = ddsHeader.dwHeight;

    /*if (!qt_glCompressedTexImage2DARB) {
        qWarning("QGLContext::bindTexture(): The GL implementation does not support texture"
                 "compression extensions.");
        return false;
    }*/

    // load mip-maps
    for(int i = 0; i < (int) ddsHeader.dwMipMapCount; ++i) {
        //if (i>4)  continue;
        if (w == 0) w = 1;
        if (h == 0) h = 1;

        if (data.ddxversion <= 0) { /* swy: upload the uncompressed RGBA8 or RGB8 texture into the GPU */
          glTexImage2D(GL_TEXTURE_2D, i, intFormat, w, h, 0, format, type, pixels + offset);
          offset += w * h * pixelSize;

        } else { /* swy: or upload the compressed DXT1/3/5 texture instead */
          int size = ((w+3)/4) * ((h+3)/4) * blockSize;
          if (offset+size>=bufferSize) offset = bufferSize-size;

            glCompressedTexImage2D
          //qt_glCompressedTexImage2DARB
                                      (GL_TEXTURE_2D, i, intFormat, w, h, 0,
                                       size, pixels + offset);
          offset += size;
        }

        // half size for each mip-map level; don't halve when we're at the end of the loop to reuse the values
        if (i + 1 < (int) ddsHeader.dwMipMapCount) {
          w = max(w/2, 1);
          h = max(h/2, 1);
        }
    }

    free(pixels);

    /* swy: allocate a tiny buffer in the stack; easy and fast */
    #define DEC_PIXEL_BUF_SZ (4u * 4u * sizeof(uint32_t))
    uint8_t decodedPixels[DEC_PIXEL_BUF_SZ + 64u] = {0}; /* swy: leave a bit of secret extra padding for alignment, just in case glGetTexImage() tries to write past the end */

    /* swy: if it does not fit in our small buffer (we're interested in tiny mip sizes like 1x1 or 4x4 pixels), then ignore it */
    if ((w * h * sizeof(uint32_t)) <= DEC_PIXEL_BUF_SZ) {
        /* swy: grab the decoded pixel data of the last mipmap; which should contain a single pixel with the average color,
                useful to analyze the texture contents for the normalmap encoding and RGBA channel usage */
        glGetTexImage(GL_TEXTURE_2D, max((int) ddsHeader.dwMipMapCount - 1, 0), GL_RGBA, GL_UNSIGNED_BYTE, &decodedPixels); /* swy: make sure we pick mip index zero when there's only a single mipmap, instead of -1; which will cause a crash */

        qDebug("glGetTexImage: %hhx, %hhx, %hhx, %hhx, %hhx, %hhx, %hhx, %hhx", decodedPixels[0], decodedPixels[1], decodedPixels[2], decodedPixels[3], decodedPixels[4], decodedPixels[5], decodedPixels[6], decodedPixels[7]); qDebug() << fileName;
        data.pixel_data_present = true;
    } else {
        qDebug("glGetTexImage: couldn't retrieve the decoded pixel data. mipcount=%u, w=%u, h=%u", ddsHeader.dwMipMapCount, w, h);
        data.pixel_data_present = false;
    }

    data.r = decodedPixels[0] / 256.f,
    data.g = decodedPixels[1] / 256.f,
    data.b = decodedPixels[2] / 256.f,
    data.a = decodedPixels[3] / 256.f;
    qDebug("glGetTexImage: float %f %f %f %f. mipcount=%u, w=%u, h=%u", data.r, data.g, data.b, data.a, ddsHeader.dwMipMapCount, w, h);

    qgl_dds_cache()->insert(fileName, data);
    return true;
}




#endif // BINDTEXTUREPATCH_H
