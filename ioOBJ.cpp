/* OpenBRF -- by marco tarini. Provided under GNU General Public License */

#include <vector>
#include <map>
#include <stdio.h>

#include <vcg/space/box3.h>
#include <vcg/space/point3.h>
#include <vcg/space/point2.h>

using namespace std;
using namespace vcg;

#include "brfMesh.h"
#include "brfBody.h"
#include "brfSkeleton.h"

#include <QString>
#include <QFile>
#include <QPair>
#include <QMap>

#include "ioOBJ.h"


// defined in vcgmesh.cpp
void setGotNormals(bool b);
void setGotMaterialName(bool b);
void setGotTexture(bool b);
void setGotColor(bool b);


static int cv, cp; // cumulated vert, cumulated pos


void IoOBJ::reset(){
  cv = cp = 0;
}

bool IoOBJ::open(QFile &f, QString fn){
  f.setFileName(fn);

  if (!f.open(QIODevice::WriteOnly|QIODevice::Text)) return false;
  //int fr=nframe;
  f.write("#\n# Exported by OpenBRF -- marco tarini\n#\n");
  return true;
}


bool BrfMesh::SaveOBJ(char* fn, int nframe) const{
  QFile f;
  IoOBJ::reset();
  if (! IoOBJ::open(f,fn)) return false;
  if (! IoOBJ::writeMesh(f,*this, nframe)) return false;
  f.close();
  return true;
}

void IoOBJ::writeHitbox(QFile &f, const BrfBody& b, const BrfSkeleton& s){
  assert(b.part.size()==s.bone.size());
  std::vector<vcg::Matrix44f> v = s.GetBoneMatrices();
  //s.getRotationMatrix(0)
}

bool IoOBJ::writeMesh(QFile &f, const BrfMesh& m, int fr){

//bool BrfMesh::SaveOBJ(char* fn, int nframe) const{
  QString s;
  s = QString("s %1\n").arg(m.name);
  f.write(s.toLatin1());
  //s = QString("newmtl %1\nusemtl %1\n").arg(m.material);
  s = QString("newmtl %1\nKa 0.8 0.8 0.8\nKd 0.2 0.2 0.2\nusemtl %1\n").arg(m.material);
  f.write(s.toLatin1());

  struct uniqueVertex {
    int pos;
    unsigned int col;
  };

typedef QPair<int, unsigned int> position_and_color_pair;

  QMap<position_and_color_pair, uint32_t> unique_pos;

  for (unsigned int i=0; i<m.vert.size(); i++) {
    position_and_color_pair uv = {m.vert[i].index, m.vert[i].col};
    unique_pos.insert(uv, 0);
  }


  int np = 0, nv =0; int index = 0;
  for (auto i = unique_pos.begin(), end = unique_pos.end(); i != end; ++i) {
    int cur_pos_idx = i.key().first;
    unsigned int cur_col = i.key().second;
    i.value() = index++;

    s = QString("v %1 %2 %3")
      .arg(-m.frame[fr].pos[cur_pos_idx].X())
      .arg( m.frame[fr].pos[cur_pos_idx].Y())
      .arg( m.frame[fr].pos[cur_pos_idx].Z());

    /* swy: add support for exporting vertex colors using this Wavefront OBJ extension:
            https://en.wikipedia.org/wiki/Wavefront_.obj_file#cite_ref-2 */
    if (m.hasVertexColor) {
      s += QString("  %3 %2 %1  %4")
        .arg(((cur_col >> (8*0)) & 0xFF) / 255.f)  /* swy: B */
        .arg(((cur_col >> (8*1)) & 0xFF) / 255.f)  /* swy: G */
        .arg(((cur_col >> (8*2)) & 0xFF) / 255.f)  /* swy: R */
        .arg(((cur_col >> (8*3)) & 0xFF) / 255.f); /* swy: A */
    }

    s += "\n";
    f.write(s.toLatin1());
    np++;
  }
  for (unsigned int i=0; i<m.vert.size(); i++) {
    s = QString("vn %1 %2 %3\n")
      .arg(-m.frame[fr].norm[i].X())
      .arg( m.frame[fr].norm[i].Y())
      .arg( m.frame[fr].norm[i].Z());
    f.write(s.toLatin1().data());
    s = QString("vt %1 %2\n")
      .arg(     m.vert[i].ta.X())
      .arg(1.0f-m.vert[i].ta.Y());
    f.write(s.toLatin1());
    nv++;
  }


  for (unsigned int i=0; i<m.face.size(); i++) {
    f.write("f");
    for (int w=0; w<3; w++) {
      position_and_color_pair uv = {m.vert[m.face[i].index[w]].index, m.vert[m.face[i].index[w]].col};
      auto uniquePosIdx = unique_pos.value(uv);
      s = QString(" %1/%2/%2")
        .arg(uniquePosIdx +1 + cp)
        .arg(m.face[i].index[w] +1 +cv);
      f.write(s.toLatin1());
    }
    f.write("\n");
  }

  cp += np;
  cv += nv;
  return true;
}

class Triple{
public:
  int a,b,c;
  Triple(int a0,int b0, int c0){a=a0;b=b0;c=c0;}
  bool operator <(const Triple &t) const {
    if (a<t.a) return true;
    if (b<t.b) return true;
    return (c<t.c);
  }
};

typedef QPair<QString,QString> MatMesh; // pairs Material(Name)-(mesh)Name
static std::map<MatMesh,int> matMeshMap; //
static std::vector<MatMesh> matMeshVec; //
static std::vector<int> matMeshIndex; // parallel vector: material index for each face


bool IoOBJ::wasMultpileMat(){
  return matMeshVec.size()>1;
}

void IoOBJ::subdivideLast(const BrfMesh& m, std::vector<BrfMesh> &res){
  assert(m.face.size()==matMeshIndex.size());
  int nmat = (int) matMeshVec.size();
  res.resize(nmat);
  for (int i=0; i<nmat; i++){
    res[i] = m;
    res[i].face.clear();
    sprintf(res[i].material,"%s",matMeshVec[i].second.toLatin1().data());
    if (i>0)
      sprintf(res[i].name,"%s.%d",m.name,i);
    else
      sprintf(res[i].name,"%s",m.name);
  }
  for (unsigned int i=0; i<m.face.size(); i++) {
    int j = matMeshIndex[i];
    assert(j<nmat);
    res[j].face.push_back(m.face[i]);
  }
  for (int i=0; i<nmat; i++){
    res[i].RemoveUnreferenced();
  }

}

bool BrfMesh::LoadOBJ(char* fn){
  name[0]=0;
  frame.resize(1);
  QFile f(fn);
  flags=0;
  material[0]=0;
  if (!f.open(QIODevice::ReadOnly|QIODevice::Text)) return false;
  std::vector<Point3f> norm;
  std::vector<Point2f> ta;
  std::map<Triple,int> map;
  std::vector<uint32_t> posColor; /* swy: store optional vertex colors retrieved from the .OBJ position lines all lined up, so that we can paste them in vertex properties (multiple vertices can share a XYZ position, each having different colors) */

  matMeshMap.clear();
  matMeshVec.clear();
  matMeshIndex.clear();

  MatMesh curMM("","");
  int curMMi = 0;
  bool newMM = false;

  setGotColor(false);
  setGotNormals(false);
  setGotMaterialName(false);
  setGotTexture(false);

  bool norms=false;

  char st[512];
  while (f.readLine(st,511)!=-1) {
    QString s = QString("%1").arg(st);
    s = s.trimmed();
    if (s.startsWith("usemtl ")) {
      char cp[512]; sprintf(cp,"%s",s.toLatin1().data());
      sscanf( cp, "usemtl %s", material);
      curMM.second=QString("%1").arg(material);
      newMM= true;
      setGotMaterialName(true);
    }
    else if (s.startsWith("v ")) {
      char cp[512]; sprintf(cp,"%s",s.toLatin1().data());
      Point3f p; float r, g, b, a = 1.f;
      int foundElemCount = sscanf( cp,"v %f %f %f %f %f %f %f",&(p[0]),&(p[1]),&(p[2]), &r, &g, &b, &a); p[0]=-p[0];
      frame[0].pos.push_back(p);

      uint32_t cur_col = 0xFFFFFFFF;
      if (foundElemCount >= (3 + 3)) /* swy: have the optional colors part at the end of the line been parsed? alpha is optional and will be 1.f otherwise */
      {
        cur_col = (((uint32_t)(b * 255) & 0xFF) << (8*0)) |  /* swy: B */
                  (((uint32_t)(g * 255) & 0xFF) << (8*1)) |  /* swy: G */
                  (((uint32_t)(r * 255) & 0xFF) << (8*2)) |  /* swy: R */
                  (((uint32_t)(a * 255) & 0xFF) << (8*3));   /* swy: A */
      }
      posColor.push_back(cur_col); /* swy: make it so that the posColor index line ups with the vertex position one; see below */
    }
    else if (s.startsWith("vn ")) {
      char cp[512]; sprintf(cp,"%s",s.toLatin1().data());
      Point3f p;
      sscanf( cp,"vn %f %f %f",&(p[0]),&(p[1]),&(p[2])); p[0]=-p[0];
      norm.push_back(p);
    }
    else if (s.startsWith("vt ")) {
      char cp[512]; sprintf(cp,"%s",s.toLatin1().data());
      Point2f p;
      sscanf( cp,"vt %f %f",&(p[0]),&(p[1])); p[1]=1-p[1];
      ta.push_back(p);
    }
    else if (s.startsWith("s ")) {
      char cp[512]; sprintf(cp,"%s",s.toLatin1().data());
      char meshName[4096];
      sscanf( cp, "s %s", meshName);
      curMM.first=QString("%1").arg(meshName);
      newMM= true;
    }
    else if (s.startsWith("f ")) {
      char cp[512]; sprintf(cp,"%s",s.toLatin1().data());


      if (newMM) {
        //std::map<MatName,int>::iterator ite = ;
        if (matMeshMap.find(curMM)==matMeshMap.end() ) {
          curMMi= matMeshMap[curMM] = matMeshVec.size();
          matMeshVec.push_back(curMM);
        } else curMMi = matMeshMap[curMM];
        newMM=false;
      }

      char st[100][100];
      int res=sscanf(cp, "f %s %s %s %s %s %s",st[0],st[1],st[2],st[3],st[4],st[5]);
      int remap[6];
      for (int w=0; w<res; w++) {
        int n=strlen(st[w]);
        int rr[3]; rr[0]=rr[1]=rr[2]=0; int rri=0;
        for(int i=0; i<n; i++) {
          char c=st[w][i];
          if (c=='/') {if (rri<3) rri++;}
          else rr[rri]=rr[rri]*10+(c-'0');
        }
        Triple t ( rr[0]-1,rr[1]-1 , rr[2]-1);

        if (map.find(t)==map.end()) {
          // new a,b,c. Add new vertex
          BrfVert v;
          v.index=t.a;
          assert(t.b<(int)ta.size());
          if (t.b>=0) {
            v.ta=v.tb=ta[t.b];
            setGotTexture(true);
          }
          assert(t.c<(int)norm.size());
          if (t.c>=0) {
            v.__norm=norm[t.c];
            norms=true;
            setGotNormals(true);
          }
          v.col=posColor[t.a];
          vert.push_back(v);
          remap[w] = map[t] = vert.size()-1;
        } else remap[w] = map[t];
      }
      for (int w=2; w<res; w++){
        BrfFace f;
        f.index[0]=remap[0];
        f.index[1]=remap[w-1];
        f.index[2]=remap[w];
        face.push_back(f);
        matMeshIndex.push_back(curMMi);
      }

    }

  }
  frame[0].norm.resize( vert.size());
  for (unsigned int i=0; i<vert.size(); i++) frame[0].norm[i]=vert[i].__norm;
  f.close();

  if (!norms) {
    UnifyVert(false);
    ComputeNormals();
  }

  AfterLoad();
  return true;
}
