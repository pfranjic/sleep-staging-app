#pragma once
#include "qstring.h"
class TextId
{
public:
	virtual ~TextId();
	virtual void setTextId(QString textId) = 0;
	virtual QString getTextId() = 0;
	virtual QString getValue() = 0;
};
inline TextId::~TextId() { }

