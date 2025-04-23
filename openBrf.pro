#
# OpenBRF -- by marco tarini. Provided under GNU General Public License
#

QT += widgets
QT += opengl
QT += openglwidgets
QT += xml

CONFIG += exceptions
CONFIG += c++17

# swy: comment out the line below to enable debuggable builds with symbols and PDB files
# CONFIG += debug

VCGLIB = dependencies/vcglib # v1.0.1

*g++* {
    message("Generating makefile for the MinGW version.")
    QMAKE_CXXFLAGS += -std=c++0x
    QMAKE_CXXFLAGS += "-isystem $$VCGLIB"

    # swy: shut up the eigen library causing thousands of warnings slowing down gcc/MinGW:
    #      https://github.com/openscad/openscad/issues/2771
    QMAKE_CXXFLAGS += -Wno-attributes -Wno-misleading-indentation -Wno-int-in-bool-context
    QMAKE_CXXFLAGS += -Wno-deprecated-declarations

    # swy: silence some noisy, but mostly harmless, warnings that obscure the real bugs
    QMAKE_CXXFLAGS += -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-unused-result -Wno-deprecated-copy
    QMAKE_CXXFLAGS += -Wno-format-truncation -Wno-address -Wimplicit-fallthrough=2
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

TRANSLATIONS += $$files(translations/openbrf_*.ts)

RESOURCES += resource.qrc
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
    QMAKE_POST_LINK = $$[QT_INSTALL_BINS]/windeployqt --translations de,es,ja,zh_CN --no-system-d3d-compiler --no-system-dxc-compiler --skip-plugin-types generic,tls --exclude-plugins qgif,qjpeg --no-opengl-sw $$MSVC_WINDEPLOY_EXTRA_ARGS $$shell_path($$DESTDIR/$${TARGET}.exe)
}

MOC_DIR = tmp
UI_DIR = tmp

win32 {
    # swy: needed so that Qt stops appending its own generated XML manifest when we already have
    #      our own one with UTF-8 support in there. without this we'll get duplication errors.
    QMAKE_MANIFEST = openBrf.win32manifest.xml
}

win32 {
    LIBS += -lopengl32 -lglu32
} else { # swy: at least for Unix-likes, macOS will probably need frameworks instead
    LIBS += -lGL -lGLU
}

translations.files = translations/openbrf_de.qm \
                     translations/openbrf_es.qm \
                     translations/openbrf_ja.qm \
                     translations/openbrf_zh.qm
translations.path = $$DESTDIR/translations

misc_files.files += carry_positions.txt
misc_files.path = $$DESTDIR

# swy: https://evileg.com/en/post/476/ (Undocumented QMake - Copying Files)
CONFIG += file_copies
COPIES += translations misc_files

# swy: super neat way of including the year/month/day numerically as a C preprocessor define by massaging the _DATE_ (e.g. 'Mon Oct 28 11:25:58 2024') variable: https://stackoverflow.com/a/71335257/674685
DATELIST = $$split(_DATE_, " ")
{BUILD_MO_STR = $$member(DATELIST, 1)} {BUILD_MO_STR = $$replace(BUILD_MO_STR, Jan,  1)} {BUILD_MO_STR = $$replace(BUILD_MO_STR, Feb,  2)} {BUILD_MO_STR = $$replace(BUILD_MO_STR, Mar,  3)} {BUILD_MO_STR = $$replace(BUILD_MO_STR, Apr,  4)} {BUILD_MO_STR = $$replace(BUILD_MO_STR, May,  5)} {BUILD_MO_STR = $$replace(BUILD_MO_STR, Jun,  6)} {BUILD_MO_STR = $$replace(BUILD_MO_STR, Jul,  7)} {BUILD_MO_STR = $$replace(BUILD_MO_STR, Aug,  8)} {BUILD_MO_STR = $$replace(BUILD_MO_STR, Sep,  9)} {BUILD_MO_STR = $$replace(BUILD_MO_STR, Oct, 10)} {BUILD_MO_STR = $$replace(BUILD_MO_STR, Nov, 11)} {BUILD_MO_STR = $$replace(BUILD_MO_STR, Dec, 12)}
DEFINES += "QMAKE_BUILD_YEA=$$member(DATELIST, 4)" # swy; (e.g. 'Mon Oct 28 11:25:58 2024' -> '2024'        )
DEFINES += "QMAKE_BUILD_MON=$${BUILD_MO_STR}"      # swy; (e.g. 'Mon Oct 28 11:25:58 2024' ->  'Oct' -> '10')
DEFINES += "QMAKE_BUILD_DAY=$$member(DATELIST, 2)" # swy; (e.g. 'Mon Oct 28 11:25:58 2024' ->   '28'        )