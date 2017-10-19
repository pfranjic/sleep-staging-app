#include "TextIdQLineEdit.h"



TextIdQLineEdit::TextIdQLineEdit(QWidget *parent): QLineEdit(parent)
{
	mTextId = "";
}


TextIdQLineEdit::~TextIdQLineEdit()
{
}

void TextIdQLineEdit::setTextId(QString textId)
{
	mTextId = textId;
}

QString TextIdQLineEdit::getTextId()
{
	return mTextId;
}

QString TextIdQLineEdit::getValue()
{
	return text();
}
