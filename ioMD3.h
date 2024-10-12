/* OpenBRF -- by marco tarini. Provided under GNU General Public License */

#ifndef IOMD3_H
#define IOMD3_H

class IoMD
{
public:
  static bool Import(const char*filename, std::vector<BrfMesh> &mv);
  static bool Export(const char*filename, const BrfMesh &m);
	static bool ImportMD2(const char*filename, std::vector<BrfMesh> &mv);
  static bool ExportMD2(const char*filename, const BrfMesh &m);
  static char* LastErrorString();
private:
  static bool Import(FILE *f, BrfMesh &m);
  static bool Import(FILE *f, BrfFrame &fr);
	static bool ImportMD2(FILE *f, BrfMesh &m);
  static bool ImportMD2(FILE *f, BrfFrame &fr);
};

#endif // IOMD3_H
