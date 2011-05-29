#include "brfData.h"
#include "glwidgets.h"
#include "selector.h"
#include "mainwindow.h"
#include "ui_guipanel.h"

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    separatorAct = fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileActs[i]);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    updateRecentFileActions();

    //menuBar()->addSeparator();
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(editCopyAct);
    editMenu->addAction(editCopyFrameAct);
    editMenu->addAction(editCopyCompleteAct);
    editMenu->addAction(editAddToCopyAct);
    editMenu->addSeparator();
    editMenu->addAction(editCutAct);
    editMenu->addAction(editCutFrameAct);
    editMenu->addAction(editCutCompleteAct);
    editMenu->addSeparator();
    editMenu->addAction(editPasteAct);
    editMenu->addAction(editPasteRiggingAct);
    editMenu->addAction(editPasteModificationAct);
    editMenu->addAction(editPasteTimingsAct);
    editMenu->addAction(editPasteFrameAct);

    QMenu* importMenu=menuBar()->addMenu(tr("&Import"));

    importMenu->addAction(importStaticMeshAct);
    importMenu->addAction(importRiggedMeshAct);
    importMenu->addAction(importMovingMeshAct);
    importMenu->addAction(importMovingMeshFrameAct);
    importMenu->addSeparator();
    importMenu->addAction(importBodyAct);
    importMenu->addSeparator();
    importMenu->addAction(importSkeletonAct);
    importMenu->addAction(importAnimationAct);
    importMenu->addSeparator();
    importMenu->addAction(addNewMaterialAct);
    importMenu->addAction(addNewTextureAct);
    importMenu->addAction(addNewShaderAct);
    importMenu->addAction(addNewUiPictureAct);
    importMenu->addSeparator();
    importMenu->addAction(importBrfAct);

    QMenu* moduleMenu=menuBar()->addMenu(tr("Module"));
    moduleMenu->addAction(moduleSelectAct);
    moduleMenu->addSeparator();
    moduleMenu->addAction(refreshIniAct);
    moduleMenu->addAction(computeUsedByAct);
    moduleMenu->addAction(selectBrfDataAct);
    moduleMenu->addAction(showModuleStatsAct);
    moduleMenu->addSeparator();
    moduleMenu->addAction(checkIniAct);
    moduleMenu->addAction(searchIniAct);
    moduleMenu->addAction(showUnrefTexturesAct);
    //moduleMenu->addAction(searchBrfAct);
    //moduleMenu->addSeparator();
    //moduleMenu->addAction(exportNamesAct);
    moduleMenu->addSeparator();
    QMenu* navMenu = moduleMenu->addMenu(tr("Navigate"));
    navMenu->addAction(navigateRightAct);
    navMenu->addAction(navigateLeftAct);
    //navMenu->addAction(navigateDownAct);
    //navMenu->addAction(navigateUpAct);

    QMenu* toolMenu = menuBar()->addMenu(tr("&Tools"));
    toolMenu->addAction(sortEntriesAct);
    toolMenu->addAction(repeatLastCommandAct);

    QMenu* optionMenu=menuBar()->addMenu(tr("&Settings"));

    optionUseOpenGL2 = new QAction(tr("Use OpenGL 2.0"),this);
    optionUseOpenGL2->setCheckable(true);
    optionUseOpenGL2->setToolTip(tr("Allows to preview bumpmapping etc. This can create compatibility problems on some (older?) graphic card"));
    connect(optionUseOpenGL2, SIGNAL(toggled(bool)), this, SLOT(setUseOpenGL2(bool)));
    optionMenu->addAction(optionUseOpenGL2);

    optionMenu->addSeparator();


    QMenu* onImport = optionMenu->addMenu(tr("On import meshes"));

    QMenu* onAssemble = optionMenu->addMenu(tr("On assemble vertex animations"));


    QMenu *tldSpecial = new QMenu("TLD easteregg");
    mab2tldHeadAct = new QAction("Make head TLD mode",this);
    tld2mabHeadAct = new QAction("And back",this);
    connect(tld2mabHeadAct,SIGNAL(triggered()),this,SLOT(tld2mabHead()));
    connect(mab2tldHeadAct,SIGNAL(triggered()),this,SLOT(mab2tldHead()));
    tldSpecial->addAction(mab2tldHeadAct);
    tldSpecial->addAction(tld2mabHeadAct);

    tldSpecial->addSeparator();

    mab2tldArmorAct = new QAction("Make TLD armour",this);
    tld2mabArmorAct = new QAction("Unmake it",this);
    connect(tld2mabArmorAct,SIGNAL(triggered()),this,SLOT(tld2mabArmor()));
    connect(mab2tldArmorAct,SIGNAL(triggered()),this,SLOT(mab2tldArmor()));
    tldSpecial->addAction(mab2tldArmorAct);
    //tldSpecial->addAction(tld2mabArmorAct);

    tldMakeDwarfSlimAct = new QAction("Make dwarf slim",this);
    connect(tldMakeDwarfSlimAct,SIGNAL(triggered()),this,SLOT(tldMakeDwarfSlim()));
    tldSpecial->addAction(tldMakeDwarfSlimAct);

    QAction* tldMakeDwarfBoot = new QAction("Make dwarf boots out of this pair",this);
    connect(tldMakeDwarfBoot,SIGNAL(triggered()),this,SLOT(tldMakeDwarfBoots()));
    tldSpecial->addAction(tldMakeDwarfBoot);

    tldSpecial->addSeparator();

    tldShrinkAroundBonesAct = new QAction("Shrink around bones",this);
    connect(tldShrinkAroundBonesAct,SIGNAL(triggered()),this,SLOT(tldShrinkAroundBones()));
    tldSpecial->addAction(tldShrinkAroundBonesAct);

    tldMenuAction = menuBar()->addMenu(tldSpecial);
    menuBar()->removeAction(tldMenuAction);
    //tldSpecial->hide();

    optionAfterMeshLoadMerge = new QAction(tr("merge vertices and pos"),this);
    optionAfterMeshLoadMerge->setCheckable(true);
    optionAfterMeshLoadRecompute = new QAction(tr("recompute normals and merge"),this);
    optionAfterMeshLoadRecompute->setCheckable(true);
    optionAfterMeshLoadNothing = new QAction(tr("do nothing"),this);
    optionAfterMeshLoadNothing->setCheckable(true);


    QActionGroup* group=new QActionGroup(this);
    group->addAction(optionAfterMeshLoadNothing);
    group->addAction(optionAfterMeshLoadMerge);
    group->addAction(optionAfterMeshLoadRecompute);
    group->setExclusive(true);

    onImport->addActions(group->actions());


    optionAssembleAniMatchVert = new QAction(tr("trust vertex order to be the same"),this);
    optionAssembleAniMatchVert->setStatusTip(tr("Use this option if you feel lucky and hope that vertex order was preserved between the frames."));
    optionAssembleAniMatchVert->setCheckable(true);
    optionAssembleAniMatchTc   = new QAction(tr("trust texture coordinates to be unique"),this);
    optionAssembleAniMatchTc->setCheckable(true);
    optionAssembleAniMatchTc->setStatusTip(tr("Use this option if you think that each vertex can be identified uniquely by its texture coords (best option)"));

    optionAssembleAniQuiverMode = new QAction(tr("quiver mode - start with max arrows"),this);
    optionAssembleAniQuiverMode->setStatusTip(tr("When you add a frame: what is not in the exact same position as the 1st frame disappears"));
    optionAssembleAniQuiverMode->setCheckable(true);
    /*
    QMenu* autoFix = optionMenu->addMenu(tr("Auto fix DDX1 textures"));
    optionAutoFixTextureOn = new QAction(tr("On"),this);
    optionAutoFixTextureOn->setCheckable(true);
    optionAutoFixTextureOff = new QAction(tr("Off"),this);
    optionAutoFixTextureOff->setCheckable(true);
    optionAutoFixTextureInfo = new QAction(tr("About this feature"),this);
    QActionGroup* group3=new QActionGroup(this);
    group3->addAction(optionAutoFixTextureOn);
    group3->addAction(optionAutoFixTextureOff);
    group3->setExclusive(true);
    autoFix->addActions(group3->actions());
    autoFix->addSeparator();
    autoFix->addAction(optionAutoFixTextureInfo);
*/

    QMenu* autoZoom = optionMenu->addMenu(tr("Auto zoom-and-recenter"));
    optionAutoZoomUseSelected = new QAction(tr("according to selected object(s) only"),this);
    optionAutoZoomUseSelected->setCheckable(true);
    optionAutoZoomUseGlobal = new QAction(tr("according to all objects in file"),this);
    optionAutoZoomUseGlobal->setCheckable(true);
    QActionGroup* group3=new QActionGroup(this);
    group3->addAction(optionAutoZoomUseSelected);
    group3->addAction(optionAutoZoomUseGlobal);
    group3->setExclusive(true);
    autoZoom->addActions(group3->actions());


    /*
    QMenu* inferMaterial = optionMenu->addMenu(tr("Mesh rendering"));
    optionInferMaterialOn = new QAction(tr("infer settings from Material flags"),this);
    optionInferMaterialOn->setToolTip(tr("E.g. alpha-transparency will depend on Material flags"));
    optionInferMaterialOn->setCheckable(true);
    optionInferMaterialOff = new QAction(tr("always use default settings"),this);
    optionInferMaterialOn->setToolTip(tr("Never use alpha transparency, regardless of Material flags"));
    optionInferMaterialOff->setCheckable(true);
    QActionGroup* group4=new QActionGroup(this);
    group4->addAction(optionInferMaterialOn);
    group4->addAction(optionInferMaterialOff);
    group4->setExclusive(true);
    inferMaterial->addActions(group4->actions());
    */

    QActionGroup* group2=new QActionGroup(this);
    group2->addAction(optionAssembleAniMatchTc);
    group2->addAction(optionAssembleAniMatchVert);
    group2->addAction(optionAssembleAniQuiverMode);
    group2->setExclusive(true);


    QActionGroup* group5=new QActionGroup(this);
    optionAoBrightness[0] = new QAction(tr("Darkest"),this);
    optionAoBrightness[1] = new QAction(tr("Dark"),this);
    optionAoBrightness[2] = new QAction(tr("Medium"),this);
    optionAoBrightness[3] = new QAction(tr("Light"),this);
    optionAoBrightness[4] = new QAction(tr("Lightest"),this);

    for (int i=0; i<5; i++) {
      optionAoBrightness[i]->setToolTip(tr("When computing AO, use %1 shades").arg(optionAoBrightness[i]->text()) );
      optionAoBrightness[i]->setCheckable(true);
      group5->addAction(optionAoBrightness[i]);
    }

    QActionGroup* group6=new QActionGroup(this);
    optionAoFromAbove[0] = new QAction(tr("Light mostly from above"),this);
    optionAoFromAbove[1] = new QAction(tr("Light from all around"),this);
    for (int i=0; i<2; i++) {
      optionAoFromAbove[i]->setCheckable(true);
      group6->addAction(optionAoFromAbove[i]);
    }
    group6->setExclusive(true);

    QActionGroup* group7=new QActionGroup(this);
    optionAoPerFace[0] = new QAction(tr("Sample per vertex (quicker)"),this);
    optionAoPerFace[1] = new QAction(tr("Sample per wedge (softer, better on some models)"),this);
    for (int i=0; i<2; i++) {
      optionAoPerFace[i]->setCheckable(true);
      group7->addAction(optionAoPerFace[i]);
    }
    group7->setExclusive(true);


    QMenu* aoBrightMenu = optionMenu->addMenu(tr("On compute Ambient Occlusion"));
    aoBrightMenu->addActions(group5->actions());
    aoBrightMenu->addSeparator();
    aoBrightMenu->addActions(group6->actions());
    aoBrightMenu->addSeparator();
    aoBrightMenu->addActions(group7->actions());
    aoBrightMenu->addSeparator();

    optionAoInAlpha = new QAction(tr("Store in per-vertex Alpha (not RGB)"),this);
    optionAoInAlpha->setCheckable(true);
    aoBrightMenu->addAction(optionAoInAlpha);

    onAssemble->addActions(group2->actions());

    optionLodSettingsAct = new QAction(tr("On building LOD pyramids..."),this);
    optionLodSettingsAct->setToolTip(tr("Set the way OpenBRF build LODs pyramids"));
    connect(optionLodSettingsAct, SIGNAL(triggered()), this, SLOT(optionLodSettings()));
    optionMenu->addAction(optionLodSettingsAct);

    optionBgColor = new QAction(tr("Background color..."),this);
    optionBgColor->setToolTip(tr("Sets the background color"));
    connect(optionBgColor, SIGNAL(triggered()), this, SLOT(optionSetBgColor()));
    optionMenu->addAction(optionBgColor);

    optionMenu->addSeparator();
    optionMenu->addAction(editRefAct);
    optionMenu->addSeparator();    

    QMenu* lang = optionMenu->addMenu(tr("Language"));
    lang -> addAction( optionLanguage[0] = new QAction(tr("System default"),this) );
    lang -> addSeparator();
    lang -> addAction( optionLanguage[1] = new QAction("English",this) );
    lang -> addAction( optionLanguage[2] = new QAction(QString("%1%2(%3%4)")
      .arg(QChar(20013)).arg(QChar(25991)).arg(QChar(31616)).arg(QChar( 20307))
    ,this ) );
    lang -> addAction( optionLanguage[4] = new QAction("Deutsche",this) );
    lang -> addAction( optionLanguage[3] = new QAction("Espa�ol",this) );
    lang -> addSeparator();
    lang -> addAction( optionLanguageCustom = new QAction(tr("Test a custom translation file..."),this) );

    for (int i=0; i<4; i++) optionLanguage[i]->setCheckable(true);
    connect(optionLanguage[0],SIGNAL(triggered()), this, SLOT(optionLanguageSet0()));
    connect(optionLanguage[1],SIGNAL(triggered()), this, SLOT(optionLanguageSet1()));
    connect(optionLanguage[2],SIGNAL(triggered()), this, SLOT(optionLanguageSet2()));
    connect(optionLanguage[3],SIGNAL(triggered()), this, SLOT(optionLanguageSet3()));
    connect(optionLanguage[4],SIGNAL(triggered()), this, SLOT(optionLanguageSet4()));
    connect(optionLanguageCustom,SIGNAL(triggered()), this, SLOT(optionLanguageSetCustom()));

    // HIDE OPTION TO LEARN NEW FEMININIZTION
    if (0) {
      optionLearnFeminization= new QAction(tr("Learn how to femininize armours from current meshes."),this);
      optionMenu-> addAction(optionLearnFeminization);
      connect(optionLearnFeminization, SIGNAL(triggered()), this, SLOT(learnFemininzation()));
    }


    optionMenu-> addSeparator();

    optionMenu->addAction(registerMime);
    optionMenu->addAction(aboutCheckboardAct);
    optionMenu->addAction(aboutAct);
}

void MainWindow::createActions()
{
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    editCutAct = new QAction(tr("Cut"), this);
    editCutAct->setStatusTip(tr("Cut currently selected objects."));
    editCutAct->setShortcuts(QKeySequence::Cut);
    editCopyAct = new QAction(tr("Copy"), this);
    editCopyAct->setStatusTip(tr("Copy currently selected objects in the clipboard."));
    editAddToCopyAct = new QAction(tr("Add to Clipboard"), this);
    editAddToCopyAct->setStatusTip(tr("Add currently selected objects to clipboard."));
    editCopyAct->setShortcuts(QKeySequence::Copy);
    editPasteAct = new QAction(tr("Paste"), this);
    editPasteAct->setStatusTip(tr("Paste objects from the clipboard into currect BRF."));
    editPasteAct->setShortcuts(QKeySequence::Paste);
    editPasteAct->setEnabled(false);

    editCutFrameAct = new QAction(tr("Cut frame"), this);
    editCutFrameAct->setShortcut(QString("ctrl+alt+X"));
    editCutFrameAct->setStatusTip(tr("Cut current frame of a vertex animated mesh."));
    editCopyFrameAct = new QAction(tr("Copy frame"), this);
    editCopyFrameAct->setShortcut(QString("ctrl+alt+C"));
    editCopyFrameAct->setStatusTip(tr("Copy current frame of a vertex animated mesh."));

    editCopyCompleteAct = new QAction(tr("Copy complete"), this);
    editCopyCompleteAct->setShortcut(QString("Ctrl+Shift+C"));
    editCopyCompleteAct->setStatusTip(tr("Copy selected objects plus everything used by them."));

    editCutCompleteAct = new QAction(tr("Cut complete"), this);
    editCutCompleteAct->setShortcut(QString("Ctrl+Shift+X"));
    editCutCompleteAct->setStatusTip(tr("Cut selected objects plus everything used by them."));

    editPasteFrameAct = new QAction(tr("Paste frame"), this);
    editPasteFrameAct->setShortcut(QString("Ctrl+Alt+V"));
    editPasteFrameAct->setEnabled(false);
    editPasteFrameAct->setStatusTip(tr("Paste frame from clipboard as next frame in the current vertex animated mesh"));
    editPasteRiggingAct = new QAction(tr("Paste rigging"), this);
    editPasteRiggingAct->setStatusTip(tr("Make a rigging for current mesh(-es) similar to one of the meshes in the clipboard."));
    editPasteRiggingAct->setEnabled(false);

    editPasteModificationAct = new QAction(tr("Paste modifications"), this);
    editPasteModificationAct->setStatusTip(tr("Move vertices of current mesh according to a 2 frame mesh animation."));
    editPasteModificationAct->setEnabled(false);

    editPasteTimingsAct = new QAction(tr("Paste timings"), this);
    editPasteTimingsAct->setStatusTip(tr("Paste timings of vertex or skeletal animation in clipboard into other animation(s)."));
    editPasteTimingsAct->setEnabled(false);

    connect(editCutAct, SIGNAL(triggered()), this, SLOT(editCut()));
    connect(editCopyAct, SIGNAL(triggered()), this, SLOT(editCopy()));
    connect(editPasteAct, SIGNAL(triggered()), this, SLOT(editPaste()));
    connect(editCopyCompleteAct, SIGNAL(triggered()), this, SLOT(editCopyComplete()));
    connect(editCutCompleteAct, SIGNAL(triggered()), this, SLOT(editCutComplete()));
    connect(editAddToCopyAct, SIGNAL(triggered()), this, SLOT(editAddToCopy()));

    connect(editCutFrameAct, SIGNAL(triggered()), this, SLOT(editCutFrame()));
    connect(editCopyFrameAct, SIGNAL(triggered()), this, SLOT(editCopyFrame()));
    connect(editPasteFrameAct, SIGNAL(triggered()), this, SLOT(editPasteFrame()));
    connect(editPasteRiggingAct, SIGNAL(triggered()), this, SLOT(editPasteRigging()));
    connect(editPasteModificationAct, SIGNAL(triggered()), this, SLOT(editPasteMod()));
    connect(editPasteTimingsAct, SIGNAL(triggered()), this, SLOT(editPasteTimings()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    //saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Alt+F4"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    aboutCheckboardAct = new QAction(tr("Why the checkerboard pattern?"), this);
    aboutCheckboardAct->setStatusTip(tr("Diagnose why I'm seeing a checkboard pattern instead of my texture."));
    aboutCheckboardAct->setVisible(false);
    connect(aboutCheckboardAct, SIGNAL(triggered()), this, SLOT(aboutCheckboard()));


    aboutAct = new QAction(tr("About"), this);
    aboutAct->setShortcut(tr("F1"));
    aboutAct->setStatusTip(tr("About OpenBRF"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    editRefAct = new QAction(tr("_"), this);

    connect(editRefAct, SIGNAL(triggered()), this, SLOT(editRef()));

    sortEntriesAct = new QAction(tr("Sort entries"), this);
    sortEntriesAct->setStatusTip(tr("Sort current entries alphabetically"));
    connect(sortEntriesAct, SIGNAL(triggered()), this, SLOT(sortEntries()));

    importStaticMeshAct = new QAction(tr("Static mesh"), this);
    importStaticMeshAct->setStatusTip(tr("Import a static Mesh"));
    importRiggedMeshAct = new QAction(tr("Rigged mesh"), this);
    importRiggedMeshAct->setStatusTip(tr("Import rigged (skeletal animable) Mesh"));
    importMovingMeshFrameAct = new QAction(tr("Frame of vertex-animated mesh"), this);
    importMovingMeshFrameAct->setStatusTip(tr("Import a static mesh and add it as a vertex-animation frame of current Mesh"));
    importMovingMeshAct = new QAction(tr("Vertex-animated mesh"), this);
    importMovingMeshFrameAct->setStatusTip(tr("Import a vertex animated mesh from a MD3 file"));
    importSkeletonAct = new QAction(tr("Skeleton"), this);
    importSkeletonAct->setStatusTip(tr("Import a Skeleton"));



    importAnimationAct = new QAction(tr("Skeletal animation"), this);
    importAnimationAct->setStatusTip(tr("Import a skeletal Animation"));
    importBodyAct = new QAction(tr("Collision body"),this);
    importBodyAct->setStatusTip(tr("Import an (multi-object) OBJ mesh as a Collision object."));
    importBrfAct = new QAction(tr("Anything from a BRF"),this);
    importBrfAct->setStatusTip(tr("Import all content form another BRF file into current one."));
    addNewMaterialAct = new QAction(tr("New Material"),this);
    addNewMaterialAct->setStatusTip(tr("Make a new Material object."));
    addNewTextureAct = new QAction(tr("New Texture"),this);
    addNewTextureAct->setStatusTip(tr("Make a new Texture object from a dds texture"));
    addNewShaderAct = new QAction(tr("New Shader"),this);
    addNewShaderAct->setStatusTip(tr("Enlist a new Shader"));

    addNewUiPictureAct = new QAction(tr("New Menu Background"),this);
    addNewUiPictureAct->setStatusTip(tr("Add a Menu Background (Mesh, Material, and Texture)"));


    navigateRightAct = new QAction(tr("follow link"),this);
    navigateRightAct->setShortcut(tr("ctrl+right"));
    navigateRightAct->setStatusTip(tr("Go from a mesh to used material; go from a material to used textures/shader"));
    navigateRightAct->setShortcutContext(Qt::ApplicationShortcut);
    navigateLeftAct = new QAction(tr("follow back-link"),this);
    navigateLeftAct->setShortcut(tr("ctrl+left"));
    navigateLeftAct->setStatusTip(tr("Go back to the mesh (from a material) or material (from texture or shaders)."));
    navigateDownAct = new QAction(tr("next back-link"),this);
    navigateUpAct = new QAction(tr("prev back-link"),this);
    searchBrfAct = new QAction(tr("Find"),this);
    searchBrfAct->setShortcut(tr("ctrl+F"));
    refreshIniAct = new QAction(tr("Refresh"),this);
    refreshIniAct->setStatusTip(tr("Reload ini files, brf files inside it, and dds textures"));
    refreshIniAct->setShortcut(tr("F5"));
    computeUsedByAct = new QAction(tr("Scan module for usages"),this);
    computeUsedByAct->setStatusTip(tr("Scans module content and txt files, to compute what uses what"));
    computeUsedByAct->setShortcut(tr("F3"));
    moduleSelectAct = new QAction(tr("Change current Module"),this);
    moduleSelectAct->setStatusTip(tr("Choose the current module"));

    exportNamesAct = new QAction(tr("Export names"),this);
    exportNamesAct->setStatusTip(tr("Export al the contnt in a txt file"));

    checkIniAct = new QAction(tr("Scan module for errors"),this);
    checkIniAct->setShortcut(tr("ctrl+E"));
    checkIniAct->setStatusTip(tr("Scan module.ini and included brf files for inconsistencies."));

    searchIniAct = new QAction(tr("Find in module..."),this);
    searchIniAct->setShortcut(tr("ctrl+F"));
    searchIniAct->setStatusTip(tr("Look for an object in all brf listed inside current module.ini."));

    selectBrfDataAct = new QAction(tr("Select a BRF in module..."),this);
    selectBrfDataAct->setStatusTip(tr("Select a BRF file of this module."));
    selectBrfDataAct->setShortcut(tr("F7"));
    showUnrefTexturesAct = new QAction(tr("Show unreferenced texture files"),this);
    showUnrefTexturesAct->setStatusTip(tr("Show texture files non referenced in any brf"));


    showModuleStatsAct = new QAction(tr("Show module stats"),this);
    showModuleStatsAct->setStatusTip(tr("Show statistics for current Module"));

    connect(navigateRightAct, SIGNAL(triggered()), this, SLOT(navigateRight()));
    connect(navigateLeftAct, SIGNAL(triggered()), this, SLOT(navigateLeft()));
    connect(navigateUpAct, SIGNAL(triggered()), this, SLOT(navigateUp()));
    connect(navigateDownAct, SIGNAL(triggered()), this, SLOT(navigateDown()));
    connect(searchBrfAct, SIGNAL(triggered()), this, SLOT(searchBrf()));
    connect(refreshIniAct, SIGNAL(triggered()), this, SLOT(refreshIni()));
    connect(checkIniAct, SIGNAL(triggered()), this, SLOT(checkIni()));
    connect(searchIniAct, SIGNAL(triggered()), this, SLOT(searchIni()));
    connect(computeUsedByAct, SIGNAL(triggered()), this, SLOT(computeUsedBy()));
    connect(selectBrfDataAct, SIGNAL(triggered()), this, SLOT(selectBrfData()));
    connect(showUnrefTexturesAct, SIGNAL(triggered()), this, SLOT(showUnrefTextures()));
    connect(showModuleStatsAct, SIGNAL(triggered()), this, SLOT(showModuleStats()));
    connect(moduleSelectAct,SIGNAL(triggered()), this, SLOT(moduleSelect()));
    connect(exportNamesAct, SIGNAL(triggered()), this, SLOT(exportNames()));

    repeatLastCommandAct = new QAction(tr("&Repeat last command"),this);
    repeatLastCommandAct->setEnabled(false);
    repeatLastCommandAct->setShortcut(QKeySequence("Ctrl+R"));
    connect(repeatLastCommandAct, SIGNAL(triggered()), this, SLOT(repeatLastCommand()));

    registerMime = new QAction(tr("Register BRF extension"),this);
    registerMime->setStatusTip(tr("Make so that clicking on a brf file opens OpenBRF."));
    connect(registerMime, SIGNAL(triggered()), this, SLOT(registerExtension()));
}

void MainWindow::createMiniViewOptions(){
  QWidget* w = new QWidget(this);
  w->setLayout(new QBoxLayout(QBoxLayout::LeftToRight));
  w->layout()->setMargin(0);
  w->layout()->setAlignment(w,Qt::AlignRight);
  w->layout()->setSpacing(0);

  QPushButton* viewmode[3];
  QButtonGroup* bg=new QButtonGroup(this);
  viewmode[0] = new QPushButton(tr("default"),this);
  viewmode[0]->setStatusTip(tr("Default mode: rotate objects with mouse, zoom in/out with wheel."));
  viewmode[1] = new QPushButton(tr("helmet"),this);
  viewmode[1]->setStatusTip(tr("Helmet mode: for objects with vertical Z axis, like M&B helmets or weapons."));
  viewmode[2] = new QPushButton(tr("scene"),this);
  viewmode[2]->setStatusTip(tr("Scene mode: navigate with mouse and WASD (levitate with wheel, zoom with shift)"));
  bg->setExclusive(true);

  comboViewmodeSelector = new QWidget(this);
  comboViewmodeSelector->setLayout(new QBoxLayout(QBoxLayout::LeftToRight));
  comboViewmodeSelector->layout()->setMargin(0);
  comboViewmodeSelector->layout()->setAlignment(w,Qt::AlignRight);
  comboViewmodeSelector->layout()->setSpacing(0);

  QPushButton* viewmodemult[2];
  comboViewmodeBG=new QButtonGroup(this);
  viewmodemult[0] = new QPushButton(tr("combo"),this);
  viewmodemult[0]->setStatusTip(tr("See objects combined, when selecting multiple things"));
  viewmodemult[1] = new QPushButton(tr("aside"),this);
  viewmodemult[1]->setStatusTip(tr("See object side-to-side, when selecting multiple things"));
  comboViewmodeBG->setExclusive(true);
  QLabel* viewmodemultLabel = new QLabel(tr("mult-view:"));
  comboViewmodeSelector->layout()->addWidget(viewmodemultLabel);



  for (int i=0; i<2; i++) {
    viewmodemult[i]->setCheckable(true);
    viewmodemult[i]->setChecked(i==0);
    viewmodemult[i]->setFlat(true);
    viewmodemult[i]->setContentsMargins(0,0,0,0);
    QFont f = viewmode[i]->font();
    f.setPixelSize(10);
    viewmodemult[i]->setFont(f);
    viewmodemult[i]->setFixedSize(38,18);
    comboViewmodeSelector->layout()->addWidget(viewmodemult[i]);
    comboViewmodeBG->addButton(viewmodemult[i],i);
  }

  QLabel* viewmodeLabel = new QLabel(tr("view-mode:"));
  QFont f =viewmodeLabel->font(); f.setPixelSize(10); f.setItalic(true);
  viewmodeLabel->setFont(f);
  viewmodemultLabel->setFont(f);
  w->layout()->addWidget(viewmodeLabel);

  for (int i=0; i<3; i++) {
    viewmode[i]->setCheckable(true);
    viewmode[i]->setChecked(i==0);
    viewmode[i]->setFlat(true);
    viewmode[i]->setContentsMargins(0,0,0,0);
    QFont f = viewmode[i]->font();
    f.setPixelSize(10);
    viewmode[i]->setFont(f);
    viewmode[i]->setFixedSize(38,18);
    w->layout()->addWidget(viewmode[i]);
    bg->addButton(viewmode[i],i);
    //statusBar()->addPermanentWidget(viewmode[i]);
  }

  QWidget* sep = new QWidget(this);
  statusBar()->addPermanentWidget(sep);

  statusBar()->addPermanentWidget(comboViewmodeSelector);
  statusBar()->addPermanentWidget(w);
  comboViewmodeSelector->setVisible(false);
  /*
  QActionGroup* viewmode=new QActionGroup(this);
  viewmode->addAction("Default");
  viewmode->addAction("Helmet");
  viewmode->addAction("Interior");
  viewmode->setExclusive(true);

  statusBar()->addActions(viewmode->actions()); //addPermanentWidget(w);
  */
  modStatus = new QLabel("Mod",this);
  statusBar()->addPermanentWidget(modStatus);

  connect(bg,SIGNAL(buttonClicked(int)),glWidget,SLOT(setViewmode(int)));
  connect(comboViewmodeBG,SIGNAL(buttonClicked(int)),glWidget,SLOT(setViewmodeMult(int)));
}

void MainWindow::createConnections(){
  connect( selector,SIGNAL(setSelection(QModelIndexList,int)) ,
           glWidget,  SLOT(setSelection(QModelIndexList,int)) );
  connect( glWidget,SIGNAL(displayInfo(QString,int)),
           this,      SLOT(displayInfo(QString,int)));

  connect( selector, SIGNAL(breakAni(int,bool)),
           this, SLOT(breakAni(int,bool)));

  connect(glWidget,SIGNAL(setTextureData(DdsData)),guiPanel,SLOT(setTextureData(DdsData)));

  /*connect(optionAutoFixTextureOn, SIGNAL(triggered()), this, SLOT(optionAutoFixTextureUpdated()) );
  connect(optionAutoFixTextureInfo, SIGNAL(triggered()), this, SLOT(optionAutoFixTextureShowInfo()) );*/
  connect(optionAutoZoomUseGlobal, SIGNAL(triggered()), glWidget, SLOT(setCommonBBoxOn()) );
  connect(optionAutoZoomUseSelected, SIGNAL(triggered()), glWidget, SLOT(setCommonBBoxOff()) );

  //connect(optionInferMaterialOff, SIGNAL(triggered()), glWidget, SLOT(setInferMaterialOff()) );
  //connect(optionInferMaterialOn , SIGNAL(triggered()), glWidget, SLOT(setInferMaterialOn()) );

  connect(importStaticMeshAct,SIGNAL(triggered()),this,SLOT(importStaticMesh()));
  connect(importRiggedMeshAct,SIGNAL(triggered()),this,SLOT(importRiggedMesh()));
  connect(importMovingMeshAct,SIGNAL(triggered()),this,SLOT(importMovingMesh()));
  connect(importMovingMeshFrameAct,SIGNAL(triggered()),this,SLOT(importMovingMeshFrame()));

  connect(importSkeletonAct,  SIGNAL(triggered()),this,SLOT(importSkeleton()));
  connect(importAnimationAct, SIGNAL(triggered()),this,SLOT(importAnimation()));
  connect(importBodyAct, SIGNAL(triggered()),this,SLOT(importCollisionBody()));
  connect(importBrfAct, SIGNAL(triggered()),this,SLOT(importBrf()));
  connect(addNewMaterialAct, SIGNAL(triggered()),this,SLOT(addNewMaterial()));
  connect(addNewShaderAct, SIGNAL(triggered()),this,SLOT(addNewShader()));
  connect(addNewTextureAct, SIGNAL(triggered()),this,SLOT(addNewTexture()));
  connect(addNewUiPictureAct, SIGNAL(triggered()),this,SLOT(addNewUiPicture()));

  connect(guiPanel->ui->cbLighting        ,SIGNAL(stateChanged(int)),glWidget,SLOT(setLighting(int)));
  connect(guiPanel->ui->cbTexture         ,SIGNAL(stateChanged(int)),glWidget,SLOT(setTexture(int)));
  connect(guiPanel->ui->cbNormalmap       ,SIGNAL(stateChanged(int)),this,SLOT(setNormalmap(int)));
  connect(guiPanel->ui->cbSpecularmap     ,SIGNAL(stateChanged(int)),this,SLOT(setSpecularmap(int)));
  connect(guiPanel->ui->cbFloor           ,SIGNAL(stateChanged(int)),glWidget,SLOT(setFloor(int)));
  connect(guiPanel->ui->cbWireframe       ,SIGNAL(stateChanged(int)),glWidget,SLOT(setWireframe(int)));
  connect(guiPanel->ui->cbRuler           ,SIGNAL(stateChanged(int)),glWidget,SLOT(setRuler(int)));
  connect(guiPanel->ui->rbNocolor         ,SIGNAL(clicked(bool)),glWidget,SLOT(setColorPerWhite()));
  connect(guiPanel->ui->rbRiggingcolor    ,SIGNAL(clicked(bool)),glWidget,SLOT(setColorPerRig()));
  connect(guiPanel->ui->rbVertexcolor     ,SIGNAL(clicked(bool)),glWidget,SLOT(setColorPerVert()));
  connect(guiPanel->ui->buPlay            ,SIGNAL(clicked()),glWidget,SLOT(setPlay()));
  connect(guiPanel->ui->buPause           ,SIGNAL(clicked()),glWidget,SLOT(setPause()));
  connect(guiPanel->ui->buStop            ,SIGNAL(clicked()),glWidget,SLOT(setStop()));
  connect(guiPanel->ui->buStepF           ,SIGNAL(clicked()),glWidget,SLOT(setStepon()));
  connect(guiPanel->ui->buStepB           ,SIGNAL(clicked()),glWidget,SLOT(setStepback()));
  connect(guiPanel->ui->frameNumberAni    ,SIGNAL(valueChanged(int)),glWidget,SLOT(setPause(int)));
  connect(guiPanel->ui->rbAlphaColor      ,SIGNAL(clicked()),glWidget,SLOT(showAlphaPurple()));
  connect(guiPanel->ui->rbAlphaNo         ,SIGNAL(clicked()),glWidget,SLOT(showAlphaNo()));
  connect(guiPanel->ui->rbAlphaTransparent,SIGNAL(clicked()),glWidget,SLOT(showAlphaTransparent()));
  connect(guiPanel->ui->boxMaterial    ,SIGNAL(textChanged(QString)),
          this,SLOT(onChangeMeshMaterial(QString)));
  connect(guiPanel->ui->boxMaterial    ,SIGNAL(textChanged(QString)),
          guiPanel,SLOT(updateMaterial(QString)));
  connect(guiPanel->ui->rulerSlid, SIGNAL(sliderMoved(int)),glWidget,SLOT(setRulerLenght(int)));
  connect(guiPanel->ui->rulerSpin, SIGNAL(valueChanged(int)),glWidget,SLOT(setRulerLenght(int)));
  connect(guiPanel->ui->labMatName, SIGNAL(linkActivated(QString)), this, SLOT(navigateRight()) ) ;
  connect(guiPanel, SIGNAL(followLink()), this, SLOT(navigateRight()) ) ;
  connect(guiPanel->ui->labBackM, SIGNAL(linkActivated(QString)), this, SLOT(navigateLeft()) ) ;
  connect(guiPanel->ui->labBackT, SIGNAL(linkActivated(QString)), this, SLOT(navigateLeft()) ) ;
  connect(guiPanel->ui->labBackS, SIGNAL(linkActivated(QString)), this, SLOT(navigateLeft()) ) ;
  // edit material
  connect(guiPanel->ui->leMatR,SIGNAL(textEdited(QString)), this, SLOT(updateDataMaterial()));
  connect(guiPanel->ui->leMatG,SIGNAL(textEdited(QString)), this, SLOT(updateDataMaterial()));
  connect(guiPanel->ui->leMatB,SIGNAL(textEdited(QString)), this, SLOT(updateDataMaterial()));
  connect(guiPanel->ui->leMatCoeff,SIGNAL(textEdited(QString)), this, SLOT(updateDataMaterial()));
  connect(guiPanel->ui->leMatBump,SIGNAL(textChanged(QString)), this, SLOT(updateDataMaterial()));
  connect(guiPanel->ui->leMatDifA,SIGNAL(textChanged(QString)), this, SLOT(updateDataMaterial()));
  connect(guiPanel->ui->leMatDifB,SIGNAL(textChanged(QString)), this, SLOT(updateDataMaterial()));
  connect(guiPanel->ui->leMatEnv,SIGNAL(textChanged(QString)), this, SLOT(updateDataMaterial()));
  connect(guiPanel->ui->leMatShader,SIGNAL(textChanged(QString)), this, SLOT(updateDataMaterial()));
  connect(guiPanel->ui->leMatSpec,SIGNAL(textChanged(QString)), this, SLOT(updateDataMaterial()));
  connect(guiPanel->ui->leMatFlags,SIGNAL(textEdited(QString)), this, SLOT(updateDataMaterial()));
  connect(guiPanel->ui->leMatRendOrd,SIGNAL(editingFinished()), this, SLOT(updateDataMaterial()));
  //connect(guiPanel, SIGNAL(dataMaterialChanged()), this, SLOT(updateDataMaterial()));
  connect(guiPanel->ui->buFlagMes, SIGNAL(clicked()), this, SLOT(setFlagsMesh()));
  connect(guiPanel->ui->buFlagMat, SIGNAL(clicked()), this, SLOT(setFlagsMaterial()));
  connect(guiPanel->ui->buFlagBody, SIGNAL(clicked()), this, SLOT(setFlagsBody()));

  connect(guiPanel->ui->browseTextureButton, SIGNAL(clicked()), glWidget, SLOT(browseTexture()));

  // make material texture selection: select rendererd texture too
  connect(guiPanel->ui->leMatDifA,SIGNAL(cursorPositionChanged(int,int)), glWidget, SLOT(showMaterialDiffuseA()));
  connect(guiPanel->ui->leMatDifB,SIGNAL(cursorPositionChanged(int,int)), glWidget, SLOT(showMaterialDiffuseB()));
  connect(guiPanel->ui->leMatBump,SIGNAL(cursorPositionChanged(int,int)), glWidget, SLOT(showMaterialBump()));
  connect(guiPanel->ui->leMatSpec,SIGNAL(cursorPositionChanged(int,int)), glWidget, SLOT(showMaterialSpecular()));
  connect(guiPanel->ui->leMatEnv ,SIGNAL(cursorPositionChanged(int,int)), glWidget, SLOT(showMaterialEnviro()));

  //connect(guiPanel->ui->leMatShader,SIGNAL(cursorPositionChanged(int,int)), guiPanel, SLOT(showMaterialShader()));
  //connect(guiPanel->ui->leMatDifA,  SIGNAL(cursorPositionChanged(int,int)), guiPanel, SLOT(showMaterialDiffuseA()));
  //connect(guiPanel->ui->leMatDifB,  SIGNAL(cursorPositionChanged(int,int)), guiPanel, SLOT(showMaterialDiffuseB()));
  //connect(guiPanel->ui->leMatBump,  SIGNAL(cursorPositionChanged(int,int)), guiPanel, SLOT(showMaterialBump()));
  //connect(guiPanel->ui->leMatSpec,  SIGNAL(cursorPositionChanged(int,int)), guiPanel, SLOT(showMaterialSpecular()));
  //connect(guiPanel->ui->leMatEnv ,  SIGNAL(cursorPositionChanged(int,int)), guiPanel, SLOT(showMaterialEnviro()));

  connect(guiPanel->ui->labelShader,  SIGNAL(linkActivated(QString)), guiPanel, SLOT(showMaterialShader()));
  connect(guiPanel->ui->labelDiffuseA,SIGNAL(linkActivated(QString)), guiPanel, SLOT(showMaterialDiffuseA()));
  connect(guiPanel->ui->labelDiffuseB,SIGNAL(linkActivated(QString)), guiPanel, SLOT(showMaterialDiffuseB()));
  connect(guiPanel->ui->labelBump,    SIGNAL(linkActivated(QString)), guiPanel, SLOT(showMaterialBump()));
  connect(guiPanel->ui->labelSpecular,SIGNAL(linkActivated(QString)), guiPanel, SLOT(showMaterialSpecular()));
  connect(guiPanel->ui->labelEnviro,  SIGNAL(linkActivated(QString)), guiPanel, SLOT(showMaterialEnviro()));

  connect(guiPanel->ui->labelDiffuseA,SIGNAL(linkHovered(QString)), glWidget, SLOT(showMaterialDiffuseA()));
  connect(guiPanel->ui->labelDiffuseB,SIGNAL(linkHovered(QString)), glWidget, SLOT(showMaterialDiffuseB()));
  connect(guiPanel->ui->labelBump,    SIGNAL(linkHovered(QString)), glWidget, SLOT(showMaterialBump()));
  connect(guiPanel->ui->labelSpecular,SIGNAL(linkHovered(QString)), glWidget, SLOT(showMaterialSpecular()));
  connect(guiPanel->ui->labelEnviro,  SIGNAL(linkHovered(QString)), glWidget, SLOT(showMaterialEnviro()));

  // edit body
  connect(guiPanel->ui->leBodyAX,SIGNAL(textEdited(QString)), this, SLOT(updateDataBody()));
  connect(guiPanel->ui->leBodyAY,SIGNAL(textEdited(QString)), this, SLOT(updateDataBody()));
  connect(guiPanel->ui->leBodyAZ,SIGNAL(textEdited(QString)), this, SLOT(updateDataBody()));
  connect(guiPanel->ui->leBodyBX,SIGNAL(textEdited(QString)), this, SLOT(updateDataBody()));
  connect(guiPanel->ui->leBodyBY,SIGNAL(textEdited(QString)), this, SLOT(updateDataBody()));
  connect(guiPanel->ui->leBodyBZ,SIGNAL(textEdited(QString)), this, SLOT(updateDataBody()));
  connect(guiPanel->ui->leBodyRad,SIGNAL(textEdited(QString)), this, SLOT(updateDataBody()));
  connect(guiPanel->ui->leBodyFlags,SIGNAL(textEdited(QString)), this, SLOT(updateDataBody()));
  connect(guiPanel->ui->leBodySign,SIGNAL(textEdited(QString)), this, SLOT(updateDataBody()));

  // edit shader
  connect(guiPanel->ui->leShaderFlags,SIGNAL(textEdited(QString)), this, SLOT(updateDataShader()));
  connect(guiPanel->ui->leShaderFallback,SIGNAL(textEdited(QString)), this, SLOT(updateDataShader()));
  connect(guiPanel->ui->leShaderRequires,SIGNAL(textEdited(QString)), this, SLOT(updateDataShader()));
  connect(guiPanel->ui->leShaderTechnique,SIGNAL(textEdited(QString)), this, SLOT(updateDataShader()));
  connect(guiPanel->ui->leShaderTaAlphaOp,SIGNAL(textEdited(QString)), this, SLOT(updateDataShader()));
  connect(guiPanel->ui->leShaderTaColorOp,SIGNAL(textEdited(QString)), this, SLOT(updateDataShader()));
  connect(guiPanel->ui->leShaderTaFlags,SIGNAL(textEdited(QString)), this, SLOT(updateDataShader()));
  connect(guiPanel->ui->leShaderTaMap,SIGNAL(textEdited(QString)), this, SLOT(updateDataShader()));

  // shader texture access data contex meny
  connect(guiPanel->textureAccessAdd,SIGNAL(triggered()), this, SLOT(updateTextureAccessAdd()));
  connect(guiPanel->textureAccessDup,SIGNAL(triggered()), this, SLOT(updateTextureAccessDup()));
  connect(guiPanel->textureAccessDel,SIGNAL(triggered()), this, SLOT(updateTextureAccessDel()));


  connect(guiPanel->ui->boxFlags       ,SIGNAL(textEdited(QString)),
          this,SLOT(onChangeFlags(QString)));
  connect(guiPanel->ui->timeOfFrame    ,SIGNAL(textEdited(QString)),
          this,SLOT(onChangeTimeOfFrame(QString)));
  connect(guiPanel->ui->timeOfFrameAni ,SIGNAL(textEdited(QString)),
          this,SLOT(onChangeTimeOfFrame(QString)));
  //connect(guiPanel->ui->boxAnimationFlags,SIGNAL(textEdited(QString)),
  //        this,SLOT(onChangeFlags(QString)));
  connect(guiPanel->ui->boxTextureFlags,SIGNAL(textEdited(QString)),
          this,SLOT(onChangeFlags(QString)));
  connect(guiPanel->ui->cbRefani,SIGNAL(currentIndexChanged(int)), glWidget, SLOT(setRefAnimation(int)) );
  connect(guiPanel->ui->cbRefani,SIGNAL(currentIndexChanged(int)), guiPanel, SLOT(setRefAnimation(int)) );
  connect(guiPanel->ui->cbRefSkel,SIGNAL(currentIndexChanged(int)), glWidget, SLOT(setRefSkeleton(int)) );
  connect(guiPanel->ui->cbSkin,SIGNAL(currentIndexChanged(int)), glWidget, SLOT(setRefSkin(int)) );
  connect(guiPanel->ui->frameNumber,SIGNAL(valueChanged(int)),guiPanel,SLOT(updateFrameNumber(int)));
  connect(guiPanel->ui->frameNumberAni,SIGNAL(valueChanged(int)),guiPanel,SLOT(updateFrameNumber(int)));

  connect(guiPanel->ui->frameNumber,SIGNAL(valueChanged(int)),glWidget,SLOT(setFrameNumber(int)));
  connect(glWidget,SIGNAL(signalFrameNumber(int)),guiPanel,SLOT(updateFrameNumber(int)));
  connect(glWidget,SIGNAL(notifyCheckboardChanged()),this,SLOT(notifyCheckboardChanged()));

  connect( selector,SIGNAL(setSelection(QModelIndexList,int)) ,
           guiPanel,  SLOT(setSelection(QModelIndexList,int)) );

  connect( selector,SIGNAL(setSelection(QModelIndexList,int)) ,
           this ,  SLOT(setSelection(QModelIndexList,int)) );

  //connect( selector,SIGNAL(addToRefMesh(int)) ,
  //         this    ,  SLOT(addToRefMesh(int)) );

  //main->setLayout(mainLayout);
  connect( QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(onClipboardChange()) );
  onClipboardChange();
}

