#include "TextIdQCheckBox.h"



TextIdQCheckBox::TextIdQCheckBox(const QString & text, QWidget * parent): QCheckBox(text, parent)
{
}


TextIdQCheckBox::~TextIdQCheckBox()
{
}

void TextIdQCheckBox::setTextId(QString textId)
{
	mTextId = textId;
}

QString TextIdQCheckBox::getTextId()
{
	return mTextId;
}
QString TextIdQCheckBox::getValue() {
	return QString::number(isChecked());
}
