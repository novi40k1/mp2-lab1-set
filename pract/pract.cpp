// pract.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iosfwd>

typedef unsigned int uint;

class TBitField
{
private:
    size_t bitLen;   // длина битового поля - макс. к-во битов
    uint* pMem;      // память для представления битового поля
    size_t memLen;   // к-во эл-тов uint для представления бит.поля //длина массива pMem

    // методы реализации
    size_t getIndex(const size_t n) const; // индекс в pМем для бита n
    uint getMask(const size_t n) const;    // битовая маска для бита n

public:
    TBitField(size_t len);
    TBitField(const TBitField& bf);
    ~TBitField();

    // доступ к битам
    uint getLength() const;            // получить длину (к-во битов)
    size_t getNumBytes() const;        // получить количество байт выделенной памяти
    void setBit(const size_t n);       // установить бит
    void clrBit(const size_t n);       // очистить бит
    bool getBit(const size_t n) const; // получить значение бита

    // битовые операции
    bool operator==(const TBitField& bf) const; // сравнение
    bool operator!=(const TBitField& bf) const; // сравнение
    TBitField& operator=(const TBitField& bf);  // присваивание
    TBitField  operator|(const TBitField& bf);  // операция "или"
    TBitField  operator&(const TBitField& bf);  // операция "и"
    TBitField  operator~();                 // отрицание

    friend std::istream& operator>>(std::istream& istr, TBitField& bf);
    friend std::ostream& operator<<(std::ostream& ostr, const TBitField& bf);
};
#include <limits>
#include <iostream>

TBitField::TBitField(size_t len)
{
    bitLen = len;
    memLen = size_t(ceil(double(len) / (8 * sizeof(uint))));
    pMem = new uint[memLen];
    for (size_t i = 0; i < memLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    bitLen = bf.bitLen;
    memLen = bf.memLen;
    pMem = new uint[memLen];
    for (size_t i = 0; i < memLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

size_t TBitField::getIndex(const size_t n) const  // индекс в pМем для бита n
{
    return n / (8 * sizeof(uint));
}

uint TBitField::getMask(const size_t n) const // битовая маска для бита n
{
    return 1 << n;
}

// доступ к битам битового поля
uint TBitField::getLength() const // получить длину (к-во битов)
{
    return bitLen;
}

size_t TBitField::getNumBytes() const // получить количество байт выделенной памяти
{
    return memLen * sizeof(uint);
}

void TBitField::setBit(const size_t n) // установить бит
{
    if (n < 0 || n >= bitLen)
        throw "Error";
    pMem[getIndex(n)] = pMem[getIndex(n)] | getMask(n - 8 * getIndex(n));
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
    return (pMem[getIndex(n)] | ~getMask(n - 8 * getIndex(n))) == UINT_MAX;
}

// битовые операции
TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    this->bitLen = bf.bitLen;
    this->memLen = bf.memLen;
    this->pMem = new uint[this->memLen];
    for (size_t i = 0; i < this->memLen; i++) {
        this->pMem[i] = bf.pMem[i];
    }
    return *this;
}

bool TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (this->bitLen != bf.bitLen)
        return false;
    for (size_t i = 0; i < this->memLen; i++) {
        if (this->pMem[i] != bf.pMem[i])
            return false;
    }
    return true;
}

bool TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    TBitField A(std::max(bf.bitLen, this->bitLen));
    for (size_t i = 0; i < this->memLen; i++)
        A.pMem[i] = this->pMem[i] | bf.pMem[i];
    return A;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
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
    for (size_t i = 0; i < A.memLen-1; i++)
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
    for (size_t i = 0; i < bf.bitLen; i++){
        uint x;
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
    for (size_t i = 0; i < bf.bitLen; i++){
        ostr << bf.getBit(i);
    }
    return ostr;
}

int main()
{
    const size_t size = 2;
    TBitField bf(size), negBf(size), expNegBf(size);
    // bf = 01
    bf.setBit(1);
    negBf = ~bf;
    // expNegBf = 10
    expNegBf.setBit(0);
    std::cout << bf << " " << negBf <<" " << expNegBf << std::endl;
    std::cin >> bf;
    std::cout << std::endl << bf;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
