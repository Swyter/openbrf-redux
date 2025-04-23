/* OpenBRF -- by marco tarini. Provided under GNU General Public License */

#include <QApplication>
#include "mainwindow.h"

#ifdef _WIN32 /* swy: only pause on the Windows version so that the command-line doesn't blink and disappear, this isn't ideal for macOS and Linux */
 #define __PAUSE() system("pause")
#else
 #define __PAUSE() /* system("read") */
#endif

static void showUsage(){
  printf(
    "usages:\n"
    "\n"
    "  OpenBRF\n"
    "     ...starts GUI\n"
    "\n"
    "  OpenBRF <file.brf>\n"
    "     ...starts GUI, opens file.brf\n"
    "\n"
    "  OpenBRF --dump <module_path> <file.txt>\n"
    "     ...shell only, dumps objects names into file.txt\n"
    "\n"
    "\n"
  );
  __PAUSE();
}

extern const char* applVersion;

int main(int argc, char* argv[])
{

  Q_INIT_RESOURCE(resource);

  QString nextTranslator;


  //const char* argv_2[]={"OpenBrf"}; int argc_2=1;

  //QApplication app(argc_2,argv_2); //argc, argv);
  QApplication app(argc, argv);
  QStringList arguments = QCoreApplication::arguments();
  app.setApplicationVersion(applVersion);
  app.setApplicationName("OpenBrf");
  app.setOrganizationName("Marco Tarini");
  app.setOrganizationDomain("Marco Tarini");
#if 1 /* swy: in Qt6 this theme will auto-detect system-wide dark mode and change as needed */
  app.setStyle("fusion");
#endif

  bool useAlphaC = false;

  if ((arguments.size()>1)&&(arguments[1].startsWith("-"))) {
    if ((arguments[1] == "--dump")&&(arguments.size()==4)) {
      switch (MainWindow().loadModAndDump(arguments[2],arguments[3])) {
      case -1: printf("OpenBRF: invalid module folder");               __PAUSE(); break;
      case -2: printf("OpenBRF: error scanning brf data or ini file"); __PAUSE(); break;
      case -3: printf("OpenBRF: error writing output file");           __PAUSE(); break;
      default: return 0;
      }
      return -1;
    } else if ((arguments[1] == "--useAlphaCommands")&&(arguments.size()==2))  {
      useAlphaC = true;
      arguments.clear();
    } else {
      showUsage();
      return -1;
    }

  }

  while (1){
    QTranslator apTranslator;
    QTranslator qtTranslator;

    if (nextTranslator.isEmpty()){
      QLocale loc;
      switch (MainWindow::getLanguageOption()) {
      default: loc = QLocale(); break;
      case 1: loc = QLocale("en");break;
      case 2: loc = QLocale("zh_CN");break;
      case 3: loc = QLocale("es");break;
      case 4: loc = QLocale("de");break;
      case 5: loc = QLocale("ja");break;
      }
      /* swy: changed it to use the UI language, see this: https://www.kdab.com/fixing-a-common-antipattern-when-loading-translations-in-qt/ */
      apTranslator.load(loc, QString("openbrf"), QString("_"), QCoreApplication::applicationDirPath() + "/translations"); /* swy: load ./translations/openbrf_<lang>.qm (app strings) */
      qtTranslator.load(loc, QString("qtbase" ), QString("_"), QCoreApplication::applicationDirPath() + "/translations"); /* swy: load ./translations/qtbase_<lang>.qm  (built-in Qt strings for default dialogs and buttons like 'OK' and 'Accept') */
    } else {
      apTranslator.load(nextTranslator);
    }
    app.installTranslator(&apTranslator);
    app.installTranslator(&qtTranslator);

    MainWindow w;
    w.setUseAlphaCommands(useAlphaC);
    w.show();

    if (arguments.size()>1) w.loadFile(arguments[1]); arguments.clear();
    if (app.exec()==101) {
      nextTranslator = w.getNextTranslatorFilename();
      continue; // just changed language! another run
    }
    break;
  }

  return 0;
}

