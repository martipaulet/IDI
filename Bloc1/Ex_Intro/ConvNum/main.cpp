#include <QApplication>
#include "ConvNum.h"
int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	ConvNum convnum;
	convnum.show();
	return app.exec();
}