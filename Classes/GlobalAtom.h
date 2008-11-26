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

// inline defs
inline ATOM GlobalAtom::GetID()
{
	return m_nID;
}
#endif