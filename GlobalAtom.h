#pragma once

class CGlobalAtom
{
public:
	CGlobalAtom(void);
	~CGlobalAtom(void);
	ATOM GetID();

private:
	ATOM m_nID;
};
