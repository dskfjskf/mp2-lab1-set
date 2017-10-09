// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <algorithm>

#define BITMEM (sizeof(TELEM)*8)

TBitField::TBitField(int len)
{
  if (len < 0)
    throw "negative length";
  else
  {
	  BitLen = len;
	  MemLen = (len - 1) / BITMEM + 1;
	  pMem = new TELEM[MemLen];
	  memset(pMem, 0, MemLen * sizeof(TELEM));
  }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[]pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if ((n < 0)||(n>BitLen))
		throw " ";
	else
  return (n/BITMEM);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	TELEM mask = 1;
	mask <<= (BITMEM-n-1);
  return mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n < 0) || (n>BitLen))
		throw " ";
	else
	{
		int k = GetMemIndex(n);
		int offset = n % BITMEM;
		TELEM mask = GetMemMask(offset);
		pMem[k] |= mask;
	}
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n < 0) || (n>BitLen))
		throw " ";
	else
	{
		int k = GetMemIndex(n);
		int offset = n % BITMEM;
		TELEM mask = ~GetMemMask(offset);
		pMem[k] &= mask;
	}
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n < 0) || (n>BitLen))
		throw " ";
	else
	{
		int offset = BITMEM - n % BITMEM - 1;
		int k = GetMemIndex(n);
		return (pMem[k] >> offset) & 1;
	}
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	delete[] pMem;
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
  return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int k = 1;
	if (BitLen==bf.BitLen)
		for (int i=0; i<MemLen;i++)
			if (pMem[i] != bf.pMem[i])
			{
				k = 0;
				continue;
			}					
			  return k;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (bf == *this)
		return 0;
	else
		return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	TBitField res(max(BitLen, bf.BitLen));
	for (int i = 0; i < min(MemLen, bf.MemLen); i++)
		res.pMem[i] = (pMem[i]) | (bf.pMem[i]);
	for (int i = min(MemLen, bf.MemLen); i < MemLen; i++)
		res.pMem[i] = pMem[i];
	for (int i = min(MemLen, bf.MemLen); i < bf.MemLen; i++)
		res.pMem[i] = bf.pMem[i]; 
  return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	TBitField res(min(BitLen, bf.BitLen));
	for (int i = 0; i < min(MemLen, bf.MemLen); i++)
		res.pMem[i] = (pMem[i]) & (bf.pMem[i]);
  return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(*this);
	TELEM inv = 1;
	for (int i = BitLen; i < MemLen*BITMEM - 1; i++)
	{
		inv <<= 1;
		inv += 1;
	}
	res.pMem[MemLen - 1] |= inv;
	for (int i = 0; i < MemLen; i++)
		res.pMem[i] = ~res.pMem[i];
  return res;
}

// ввод/вывод

istream &operator >> (istream &istr, TBitField &bf) // ввод
{
	int i;
	istr >> i;
	while (i >= 0)
	{
		bf.SetBit(i);
		istr >> i;
	}
  return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	ostr << "{";
	for (int i = 0; i < bf.BitLen; i++)
		if (bf.GetBit(i) == 1)
			ostr << i<<' ';
	  ostr << "}\n";
  return ostr;
}
