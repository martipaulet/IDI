#include <QApplication>
#include "Color.h"
int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	Color color;
	color.show();
	return app.exec();
}