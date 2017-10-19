#pragma once
#include "qlineedit.h"
#include "TextId.h"
class TextIdQLineEdit :
	public QLineEdit, public TextId
{
	Q_OBJECT
public:
	TextIdQLineEdit(QWidget *parent = 0);
	~TextIdQLineEdit();
	QString mTextId;
	void setTextId(QString textId);
	QString getTextId();
	QString getValue();
};

