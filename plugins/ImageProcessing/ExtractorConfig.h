#ifndef EXTRACTORCONFIG_H
#define EXTRACTORCONFIG_H

#include <QObject>

class ExtractorConfig : public QObject
{
	Q_OBJECT

public:
	ExtractorConfig(QObject *parent = 0) : QObject(parent)
	{
		/* empty */
	}
	
    //~ bool colorMode = true;
    //~ bool filterMode = true;
    //~ int colorThr = 110;
    //~ float colorGain = 1.5; // contrast
    //~ float colorBias = 0;   // bright
};


#endif
