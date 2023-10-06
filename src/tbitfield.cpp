// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
// Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
// Переработано в соответсвии с C++11 (частично) Пановым А.А. 2021
// Битовое поле

#include <limits>
#include <math.h>
#include <iostream>
#include "tbitfield.h"

TBitField::TBitField(size_t len)
{
    bitLen = len;
    memLen = size_t(ceil(double(len) / (8*sizeof(elType))));
    pMem = new elType[memLen];
    for (size_t i = 0; i < memLen; i++){
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    bitLen = bf.bitLen;
    memLen = bf.memLen;
    pMem = new elType[memLen];
    for (size_t i = 0; i < memLen; i++){
        pMem[i] = bf.pMem[i];
    }
}

size_t TBitField::getIndex(const size_t n) const  // индекс в pМем для бита n
{
    return n/(8*sizeof(elType));
}

elType TBitField::getMask(const size_t n) const // битовая маска для бита n
{
    return (elType)1<<n;
}

// доступ к битам битового поля
elType TBitField::getLength() const // получить длину (к-во битов)
{
    return (elType)bitLen;
}

size_t TBitField::getNumBytes() const // получить количество байт выделенной памяти
{
    return memLen * sizeof(elType);
}

void TBitField::setBit(const size_t n) // установить бит
{
    if (n<0 || n>=bitLen)
        throw "Error";
    pMem[getIndex(n)] = pMem[getIndex(n)] | getMask(n - 8*getIndex(n));
}

void TBitField::clrBit(const size_t n) // очистить бит
{
    if (n < 0 || n >= bitLen)
        throw "Error";
    pMem[getIndex(n)] = pMem[getIndex(n)] & ~getMask(n - 8 * getIndex(n));
}

bool TBitField::getBit(const size_t n) const // получить значение бита
{
    if (n < 0 || n >= bitLen)
        throw "Error";
    return (pMem[getIndex(n)] | ~getMask(n - 8 * getIndex(n))) == std::numeric_limits<elType>::max();
}

// битовые операции
TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    this->bitLen = bf.bitLen;
    this->memLen = bf.memLen;
    this->pMem = new elType[this->memLen];
    for (size_t i = 0; i < this->memLen; i++){
        this->pMem[i] = bf.pMem[i];
    }
    return *this;
}

bool TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (this->bitLen != bf.bitLen)
        return false;
    for (size_t i = 0; i < this->memLen; i++){
        if (this->pMem[i] != bf.pMem[i])
            return false;
    }
    return true;
}

bool TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this==bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    TBitField A(std::max(bf.bitLen, this->bitLen));
    for (size_t i = 0; i < this->memLen; i++)
        A.pMem[i] = this->pMem[i] | bf.pMem[i];
    return A;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    TBitField A(std::max(bf.bitLen, this->bitLen));
    for (size_t i = 0; i < this->memLen; i++)
        A.pMem[i] = this->pMem[i] & bf.pMem[i];
    return A;
}

TBitField TBitField::operator~() // отрицание
{
    TBitField A(*this);
    for (size_t i = 8 * getIndex(bitLen - 1); i < bitLen; i++) {
        if (A.getBit(i))
            A.clrBit(i);
        else
            A.setBit(i);
    }
    for (size_t i = 0; i < A.memLen - 1; i++)
        A.pMem[i] = ~(this->pMem[i]);
    return A;
}

TBitField::~TBitField()
{
    delete[] pMem;
    bitLen = 0;
    memLen = 0;
}

// ввод/вывод
std::istream& operator>>(std::istream& istr, TBitField& bf) // ввод
{
    for (size_t i = 0; i < bf.bitLen; i++) {
        elType x;
        istr >> x;
        if (x)
            bf.setBit(i);
        else
            bf.clrBit(i);
    }
    return istr;
}

std::ostream& operator<<(std::ostream& ostr, const TBitField& bf) // вывод
{
    for (size_t i = 0; i < bf.bitLen; i++) {
        ostr << bf.getBit(i);
    }
    return ostr;
}
