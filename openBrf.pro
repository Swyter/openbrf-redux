#
# OpenBRF -- by marco tarini. Provided under GNU General Public License
#

QT += opengl
QT += xml

CONFIG += exceptions


VCGLIB = dependencies/vcglib # v1.0.1

*g++* {
    message("Generating makefile for the MinGW version.")
    QMAKE_CXXFLAGS += -std=c++0x
    QMAKE_CXXFLAGS += "-isystem $$VCGLIB"

    # swy: shut up the eigen library causing thousands of warnings slowing down gcc/MinGW:
    #      https://github.com/openscad/openscad/issues/2771
    QMAKE_CXXFLAGS += -Wno-attributes -Wno-misleading-indentation -Wno-int-in-bool-context
    QMAKE_CXXFLAGS += -Wno-deprecated-declarations
}

# RC_FILE = openBrf.rc
TARGET = openBrf
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    glwidgets.cpp \
    saveLoad.cpp \
    brfMesh.cpp \
    brfData.cpp \
    selector.cpp \
    tablemodel.cpp \
    brfShader.cpp \
    brfTexture.cpp \
    brfMaterial.cpp \
    brfSkeleton.cpp \
    brfAnimation.cpp \
    brfBody.cpp \
    guipanel.cpp \
    vcgmesh.cpp \
    askBoneDialog.cpp \
    $$VCGLIB/wrap/ply/plylib.cpp \
    $$VCGLIB/wrap/dae/xmldocumentmanaging.cpp \
    ioSMD.cpp \
    askSkelDialog.cpp \
    askTexturenameDialog.cpp \
    askFlagsDialog.cpp \
    iniData.cpp \
    ioOBJ.cpp \
    askModErrorDialog.cpp \
    ioMB.cpp \
    askTransformDialog.cpp \
    askCreaseDialog.cpp \
    main_info.cpp \
    main_create.cpp \
    main_ImpExp.cpp \
    brfHitBox.cpp \
    ioMD3.cpp \
    askNewUiPictureDialog.cpp \
    askSelectBrfDialog.cpp \
    askUnrefTextureDialog.cpp \
    askIntervalDialog.cpp \
    askHueSatBriDialog.cpp \
    askLodOptionsDialog.cpp \
    askUvTransformDialog.cpp \
    askSkelPairDialog.cpp \
    askColorDialog.cpp \
    myselectionmodel.cpp
HEADERS += mainwindow.h \
    glwidgets.h \
    saveLoad.h \
    brfMesh.h \
    brfData.h \
    selector.h \
    tablemodel.h \
    brfShader.h \
    brfTexture.h \
    brfToken.h \
    brfMaterial.h \
    brfSkeleton.h \
    brfAnimation.h \
    brfBody.h \
    guipanel.h \
    vcgmesh.h \
    vcgExport.h \
    vcgImport.h \
    askBoneDialog.h \
    ioSMD.h \
    askSkelDialog.h \
    askTexturenameDialog.h \
    askFlagsDialog.h \
    iniData.h \
    askModErrorDialog.h \
    ioMB.h \
    askTransformDialog.h \
    bindTexturePatch.h \
    ddsData.h \
    askCreaseDialog.h \
    ioOBJ.h \
    ioMD3.h \
    askNewUiPictureDialog.h \
    askSelectBrfDialog.h \
    askUnrefTextureDialog.h \
    askIntervalDialog.h \
    askHueSatBriDialog.h \
    askLodOptionsDialog.h \
    askUvTransformDialog.h \
    askSkelPairDialog.h \
    askColorDialog.h \
    carryPosition.h \
    myselectionmodel.h
FORMS += guipanel.ui \
    askBoneDialog.ui \
    askSkelDialog.ui \
    askTexturenameDialog.ui \
    askFlagsDialog.ui \
    askModErrorDialog.ui \
    askTransformDialog.ui \
    askCreaseDialog.ui \
    mainwindow.ui \
    askNewUiPictureDialog.ui \
    askSelectBrfDialog.ui \
    askUnrefTextureDialog.ui \
    askIntervalDialog.ui \
    askHueSatBriDialog.ui \
    askLodOptionsDialog.ui \
    askUvTransformDialog.ui \
    askSkelPairDialog.ui
INCLUDEPATH += "$$VCGLIB"
INCLUDEPATH += "C:/libs/lib3ds-1.3.0"
INCLUDEPATH += "./"
RESOURCES += resource.qrc
TRANSLATIONS += translations/openbrf_zh.ts
TRANSLATIONS += translations/openbrf_en.ts
TRANSLATIONS += translations/openbrf_es.ts
TRANSLATIONS += translations/openbrf_de.ts
TRANSLATIONS += translations/openbrf_ja.ts
RC_FILE = openBrf.rc
win32 { 
    DEFINES += NOMINMAX
    DEFINES += _CRT_SECURE_NO_DEPRECATE
}
INCLUDEPATH += "C:\projects\libraries\include"
# DEFINES += GLEW_STATIC

# SOURCES += "C:\projects\libraries\sources\glew-1.5.3\src\glew.c"
#LIBS += -L"C:\projects\libraries\lib" \
#   % -lglew32

# swy: try to compile the MinGW libraries statically as part of the main .exe
#      instead of shipping them as a bunch of small, separated .dll files.
win32-g++ {
    message("Linking libgcc and libstd statically.")
    QMAKE_LFLAGS += -static-libgcc -static-libstdc++ -static
}

# swy: copy the final .exe and all the necessary Qt .dll files into the _build folder
#      automatically after finishing the compilation and linking.
#      https://forum.qt.io/topic/127083/using-config-windeployqt/2
#      https://stackoverflow.com/a/37462468/674685
win32 {
    # swy: don't ship the vcruntime installer in the visual studio builds, but do ship libgcc, libstd++ and libwinpthread-1 in mingw builds
    win32-msvc {
        MSVC_WINDEPLOY_EXTRA_ARGS = --no-compiler-runtime
    }

    message("Adding step to deploy the DLL files on Windows.")
    DESTDIR = $$PWD/_build
    QMAKE_POST_LINK = $$[QT_INSTALL_BINS]/windeployqt --no-system-d3d-compiler --no-angle --no-opengl-sw $$MSVC_WINDEPLOY_EXTRA_ARGS $$shell_path($$DESTDIR/$${TARGET}.exe)
}

MOC_DIR = tmp
UI_DIR = tmp

OTHER_FILES += shaders/bump_fragment.cpp
OTHER_FILES += shaders/bump_vertex.cpp
OTHER_FILES += shaders/iron_fragment.cpp
OTHER_FILES += femininizer.morpher

LIBS += -lopengl32 -lglu32

DISTFILES += \
    translations/openbrf_de.ts \
    translations/openbrf_en.ts \
    translations/openbrf_es.ts \
    translations/openbrf_zh.ts
