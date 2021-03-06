// Copyright (c) 2016 Mario Konrad
// All Rights reserved.

#include <QApplication>
#include <QTranslator>
#include <QSettings>
#include <QStandardPaths>
#include <QCommandLineParser>
#include "MainWindow.hpp"
#include "System.hpp"
#include "version.hpp"
#include "engine/MMSI.hpp"

int main(int argc, char ** argv)
{
	// application
	QApplication app(argc, argv);
	app.setApplicationName(vhfsim::project_name);
	app.setApplicationVersion(vhfsim::project_version);

	// command line arguments

	QCommandLineParser parser;
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addOption({"mmsi", "Specifies a non-modifable MMSI for the VHF", "mmsi"});
	parser.process(app);

	// configuration
	QSettings config(
		QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/vhf.conf",
		QSettings::IniFormat);
	vhfsim::System::init(&config);

	if (parser.isSet("mmsi")) {
		using namespace vhfsim;
		System::fix_vhf_mmsi(engine::MMSI{parser.value("mmsi").toStdString()});
	}

	// internationalization
	QTranslator translator;
	translator.load(vhfsim::System::lang(),
		app.applicationDirPath() + "/../share/" + vhfsim::project_name + "/lang");
	app.installTranslator(&translator);

	// run the application
	vhfsim::MainWindow win;
	win.show();
	auto rc = app.exec();

	// cleanup
	vhfsim::System::save();
	return rc;
}
