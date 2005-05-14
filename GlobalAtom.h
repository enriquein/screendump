#ifndef GLOBALATOM_H
#define GLOBALATOM_H

class CGlobalAtom
{
public:
	CGlobalAtom(void);
	~CGlobalAtom(void);
	ATOM GetID();

private:
	ATOM m_nID;
};
#endif