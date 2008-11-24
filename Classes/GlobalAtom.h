#ifndef GLOBALATOM_H
#define GLOBALATOM_H

class GlobalAtom
{
public:
	GlobalAtom(void);
	~GlobalAtom(void);
	ATOM GetID();

private:
	ATOM m_nID;
};
#endif