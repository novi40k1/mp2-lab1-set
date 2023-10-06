// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
// Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
// Переработано в соответсвии с C++11 (частично) Пановым А.А. 2021
// Множество - реализация через битовые поля

#include <iostream>
#include "tbitfield.h"
#include "tset.h"

TSet::TSet(size_t mp) : bitField(TBitField(mp)), maxPower(mp) {}

// конструктор копирования
TSet::TSet(const TSet &s) : bitField(s.bitField), maxPower(s.maxPower) {}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : bitField(bf), maxPower(bf.getLength()) {}

TSet::operator TBitField()
{
    return bitField;
}

size_t TSet::getMaxPower() const // получить макс. к-во эл-тов
{
    return maxPower;
}

bool TSet::isMember(const elType elem) const // элемент множества?
{
    return bitField.getBit(elem);
}

void TSet::insElem(const elType elem) // включение элемента множества
{
    bitField.setBit(elem);
}

void TSet::delElem(const elType elem) // исключение элемента множества
{
    bitField.clrBit(elem);
}

// теоретико-множественные операции
TSet& TSet::operator=(const TSet &s) // присваивание
{
    this->maxPower = s.maxPower;
    this->bitField = s.bitField;
    return *this;
}

bool TSet::operator==(const TSet &s) const // сравнение
{
    if(this->maxPower!=s.maxPower)
        return false;
    for (size_t i = 0; i < s.maxPower; i++) {
        if (this->bitField.getBit(i) != s.bitField.getBit(i))
            return false;
    }
    return true;
}

bool TSet::operator!=(const TSet &s) const // сравнение
{
    return !(*this==s);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TSet A(std::max(s.maxPower, this->maxPower));
    A.bitField = this->bitField | s.bitField;
    return A;
}

TSet TSet::operator+(const elType elem) // объединение с элементом
{
    this->bitField.setBit(elem);
    return *this;
}

TSet TSet::operator-(const elType elem) // разность с элементом
{
    this->bitField.clrBit(elem);
    return *this;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TSet A(std::max(s.maxPower, this->maxPower));
    A.bitField = this->bitField & s.bitField;
    return A;
}

TSet TSet::operator~() // дополнение
{
    TSet A(this->maxPower);
    A.bitField = ~this->bitField;
    return A;
}

// перегрузка ввода/вывода
std::istream &operator>>(std::istream &istr, TSet &s) // ввод
{
    istr >> s.bitField;
    return istr;
}

std::ostream& operator<<(std::ostream &ostr, const TSet &s) // вывод
{
    ostr << s.bitField;
    return ostr;
}
