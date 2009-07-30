
#include "brfData.h"

#include "saveLoad.h"


 char * tokenBrfName[N_TOKEN] = {
  "mesh",
  "texture",
  "shader",
  "material",
  "skeleton",
  "skeleton_anim",
  "body",
};

char * tokenTabName[N_TOKEN] = {
  "&Mesh",
  "&Texture",
  "&Shader",
  "M&aterial",
  "S&keleton",
  "A&nimation",
  "&Collision",
};


BrfData::BrfData(FILE*f,int verbose, int stopAt){
  Load(f,verbose,stopAt);
}
BrfData::BrfData(char*f,int verbose, int stopAt){
  Load(f,verbose,stopAt);
}


template <class T>
static void mergeVec(vector<T> &a, const vector<T> &b){
  for (unsigned int i=0; i<b.size(); i++) a.push_back(b[i]);
}

void  BrfData::Merge(const BrfData& b){
  mergeVec(mesh, b.mesh);
  mergeVec(texture, b.texture);
  mergeVec(shader, b.shader);
  mergeVec(material, b.material);
  mergeVec(skeleton, b.skeleton);
  mergeVec(animation, b.animation);
  mergeVec(body, b.body);
}


bool BrfData::Load(char*filename,int verbose, int stopAt){
  FILE *f = fopen(filename,"rb");
  if (!f) return false;
  return Load(f, verbose, stopAt);
}

template<class BrfType> bool BrfData::LoadAll(FILE *f, vector<BrfType> &v, int k){
  int verbose=1;
  for (int i=0; i<k; i++) {
        BrfType a;
        if (!a.Load(f, verbose)) return false;
        printf("  loading \"%s\"\n",a.name);
        v.push_back( a );
  }
  lastLoaded=BrfType::tokenIndex();
  return true;
}

template<class BrfType> void BrfData::SaveAll(FILE *f, const vector<BrfType> &v) const{
  if (v.size()==0) return;
  SaveString(f,tokenBrfName[ BrfType::tokenIndex() ]);
  SaveInt(f, v.size());
  for (unsigned int i=0; i<v.size(); i++) {
     v[i].Save(f);
  }
}

BrfSkeleton* BrfData::getOneSkeleton(int nbones){
  for (unsigned int i=0; i<skeleton.size(); i++){
    if ((int)skeleton[i].bone.size()==nbones) return &(skeleton[i]);
  }
  return NULL;
}

bool BrfData::Save(const char*fn) const{

  FILE *f = fopen(fn,"wb");
  if (!f) return false;
  SaveAll(f,mesh);
  SaveAll(f,texture);
  SaveAll(f,shader);
  SaveAll(f,material);
  SaveAll(f,skeleton);
  SaveAll(f,animation);
  SaveAll(f,body);

  SaveString(f,"end");
  fclose(f);

  return true;
}

bool BrfData::Load(FILE*f,int verbose, int stopAt){
  printf("LOADING BRF...\n");

  mesh.clear();
  texture.clear();
  shader.clear();
  material.clear();
  skeleton.clear();
  animation.clear();
  body.clear();

  bool res=true;

  lastLoaded = -1;
  while (1) {
    char str[255];
    LoadString(f, str);
    if (verbose>1) printf("Read \"%s\"\n",str);
    if (!strcmp(str,"end")) break;
    int k;
    LoadInt(f, k);
    if (!strcmp(str,"mesh")) res&= LoadAll(f,mesh,k);
    else if (!strcmp(str,"texture")) res&= LoadAll(f,texture,k);
    else if (!strcmp(str,"shader")) res&= LoadAll(f,shader, k);
    else if (!strcmp(str,"material")) res&= LoadAll(f,material, k);
    else if (!strcmp(str,"skeleton")) res&= LoadAll(f,skeleton, k);
    else if (!strcmp(str,"skeleton_anim")) res&= LoadAll(f,animation,k);
    else if (!strcmp(str,"body")) res&= LoadAll(f,body,k);
    else {
      printf("ERROR! Unknown token \"%s\"\n",str);
      fflush(stdout);
      res=false;
    }
    if (lastLoaded>=stopAt) return res;
    if (!res) break;
  }
  return res;

}
