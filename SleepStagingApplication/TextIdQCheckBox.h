#pragma once
#include "qcheckbox.h"
#include "TextId.h"
class TextIdQCheckBox :
	public QCheckBox, public TextId
{
public:
	TextIdQCheckBox(const QString & text, QWidget * parent = 0);
	~TextIdQCheckBox();
	QString mTextId;
	void setTextId(QString textId);
	QString getTextId();
	QString getValue();
};

