// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// sample_prime_numbers.cpp - Copyright (c) Гергель В.П. 20.08.2000
// Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
// Переработано в соответсвии с C++11 (частично) Пановым А.А. 2021
// Тестирование битового поля и множества

#include <iomanip>
#include <iostream>

#define USE_SET // Использовать класс TSet,
                // закоментировать, чтобы использовать битовое поле

#ifndef USE_SET // Использовать класс TBitField

#include "tbitfield.h"

int main()
{
    int n, m, k, count;
    setlocale(LC_ALL, "Russian");
    std::cout << "Тестирование программ поддержки битового поля" << std::endl;
    std::cout << "             Решето Эратосфена" << std::endl;
    std::cout << "Введите верхнюю границу целых значений - ";
    std::cin  >> n;
    TBitField s(n + 1);
    // заполнение множества
    for (m = 2; m <= n; m++)
        s.setBit(m);
    // проверка до sqrt(n) и удаление кратных
    for (m = 2; m * m <= n; m++)
        // если m в s, удаление кратных
        if (s.getBit(m))
        for (k = 2 * m; k <= n; k += m)
            if (s.getBit(k))
                s.clrBit(k);
    // оставшиеся в s элементы - простые числа
    std::cout << std::endl << "Печать множества некратных чисел" << std::endl << s << std::endl;
    std::cout << std::endl << "Печать простых чисел" << std::endl;
    count = 0;
    k = 1;
    for (m = 2; m <= n; m++)
        if (s.getBit(m))
        {
            count++;
            std::cout << std::setw(3) << m << " ";
            if (k++ % 10 == 0)
                std::cout << std::endl;
         }
    std::cout << std::endl;
    std::cout << "В первых " << n << " числах " << count << " простых" << std::endl;
}
#else

#include "tset.h"

int main()
{
    int n, m, k, count;
    setlocale(LC_ALL, "Russian");
    std::cout << "Тестирование программ поддержки множества" << std::endl;
    std::cout << "              Решето Эратосфена" << std::endl;
    std::cout << "Введите верхнюю границу целых значений - ";
    std::cin  >> n;
    TSet s(n + 1);
    // заполнение множества
    for (m = 2; m <= n; m++)
        s.insElem(m);
    // проверка до sqrt(n) и удаление кратных
    for (m = 2; m * m <= n; m++)
        // если м в s, удаление кратных
        if (s.isMember(m))
            for (k = 2 * m; k <= n; k += m)
                if (s.isMember(k))
                    s.delElem(k);
    // оставшиеся в s элементы - простые числа
    std::cout << std::endl << "Печать множества некратных чисел" << std::endl << s << std::endl;
    std::cout << std::endl << "Печать простых чисел" << std::endl;
    count = 0;
    k = 1;
    for (m = 2; m <= n; m++)
        if (s.isMember(m))
        {
            count++;
            std::cout << std::setw(3) << m << " ";
            if (k++ % 10 == 0)
                std::cout << std::endl;
        }
    std::cout << std::endl;
    std::cout << "В первых " << n << " числах " << count << " простых" << std::endl;
}
#endif