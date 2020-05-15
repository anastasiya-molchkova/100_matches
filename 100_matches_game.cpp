/* Игра «100 спичек». Из кучки, первоначально содержащей 100 спичек, 
двое играющих поочерёдно берут по несколько спичек: не менее одной и не более десяти. 
Проигрывает тот, кто взял последнюю спичку. 
Количество спичек, которое берёт компьютер, определите с помощью генерации случайных чисел. */

#include <iostream>
#include <Windows.h>   // для Sleep()
#include <ctime>       // чтобы в randomize опираться на время запуска программы

// симулятор подбрасывания кубика пользователем и компьютером, возвращает истину, если игрок "выбросил" больше очков
bool users_choose_first()
{
    std::cout << "Подбрасываем кубик, определяем, кто ходит первым...\n";
    Sleep(3000);
    auto users_scores{ rand() % 6 + 1 };
    std::cout << "У Вас: " << users_scores << std::endl;
    auto computer_scores{ rand() % 6 + 1 };
    std::cout << "У компьютера: " << computer_scores << std::endl;
    while (users_scores == computer_scores)
    {
        std::cout << "Придётся бросить кубик ещё раз... " << std::endl;
        Sleep(3000);
        users_scores = rand() % 6 + 1;
        std::cout << "У Вас: " << users_scores << std::endl;
        computer_scores = rand() % 6 + 1;
        std::cout << "У компьютера: " << computer_scores << std::endl;
    }
    Sleep(1000);
    return (users_scores > computer_scores)? true: false;
}

// определяет минимальное из двух чисел
unsigned short get_min_from(const unsigned short a, const unsigned short b)
{
    return (a <= b) ? a : b;
}

// получаем от пользователя натуральное число
unsigned short get_number(const unsigned short min, const unsigned short max)
{
    int answer;
    std::cin >> answer;
    while (std::cin.fail() || (answer < min) || (answer > max))
    {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Нужно ввести число от " << min << " до " << max << ", повторите ввод: ";
        std::cin >> answer;
    }
    return static_cast<unsigned short>(answer);
}

// компьютер рассчитывает, сколько он должен взять спичек, чтобы в итоге оставить игрока с одной
unsigned short computers_smart_choice(const unsigned short left_matches, const unsigned short min, const unsigned short max)
{
    /* чтобы на последнем ходу оставить игрока с одной спичкой, 
    на предыдущем нужно оставить его с 1 + (максимальное число за ход + минимальное число за ход), 
    а на предпредыдущем - с 1 + (максимальное число за ход + минимальное число за ход)*2 и т.д., то есть 
    нужно взять столько, чтобы пользователю осталось число, дающее 1 в остатке от деления на (максимальное число за ход + минимальное число за ход) */
    unsigned short optimal_choice{ min };
    while (((left_matches - optimal_choice) % (max + min) != 1) && (optimal_choice < max))
    {
        ++optimal_choice;
    }
    return optimal_choice;
}

int main()
{
    setlocale(LC_CTYPE, "rus");                  // для вывода сообщений пользователю на кириллице
    srand(static_cast<unsigned int>(time(0)));   // аналог randomize с привязкой ко времени запуска

    const unsigned short matches_number{ 100 };  // начальное количество спичек
    const unsigned short min_matches{ 1 };       // минимальное количество спичек, которое можно взять за ход
    const unsigned short max_matches{ 10 };      // максимальное количество спичек, которое можно взять за ход
    // Объясняем правила игры польхователю и даём несколько секунд, чтобы их прочитать:
    std::cout << "Игра «" << matches_number << " спичек».\nИз кучки, первоначально содержащей " << matches_number << " спичек,\n"
        << "двое играющих поочерёдно берут по несколько спичек: не менее " << min_matches << " и не более " << max_matches
        << "\nПроигрывает тот, кто взял последнюю спичку.\n";
    Sleep(6000);
    // Даём пользователю выбрать уровень сложности. 
    // При низком уровне компьютер будет брать случайное количество спичек, при высоком - делать умный выбор
    std::cout << "Выберите уровень сложности:\n0 - низкий\n1 - высокий\n";
    bool complexity_is_high{ static_cast<bool>(get_number(0, 1)) };
   
    while (true)                                        // играем, пока пользователю не надоест
    {
        // с помощью симулятора подбрасывания кубика определяем кто ходит первым:
        bool users_turn{ users_choose_first() };
        if (users_turn) std::cout << "Вы ходите первым!\n";
        else std::cout << "Компьютер ходит первым!\n";
        Sleep(1000);

        unsigned short left_matches{ matches_number };  // количество оставшихся спичек сначала равно начальному количеству
        while (left_matches > 1)                        // цикл одного хода, игрок (пользователь или компьютер) делает ход
        {
            // сначала показываем, сколько осталось спичек, числом и чёрточками |
            std::cout << "осталось спичек: " << left_matches << '\n';
            for (unsigned short i = 1; i <= left_matches; i++)
                std::cout << '|';
            std::cout << std::endl;

            unsigned short one_choice{ 0 };             // количество спичек, которые игрок берёт за ход
            if (users_turn)                             // если ходит игрок:
            {
                std::cout << "Сколько спичек Вы берёте? ";
                /* чтобы игрок не взял все спички или больше, чем есть,
                   за максимум спичек за ход берём не больше, чем осталось минус 1 */      
                one_choice = get_number(min_matches, get_min_from(left_matches - 1, max_matches));
            }
            else                                        // если ходит компьютер:
            {
                Sleep(1000);
                if (!complexity_is_high)                // при низком уровне компьютер будет брать случайное количество спичек
                    one_choice = rand() % get_min_from((left_matches - 1), max_matches) + min_matches;
                /* чтобы игрок не взял все спички или больше, чем есть,
                   за максимум спичек за ход берём не больше, чем осталось минус 1 */
                else
                    one_choice = computers_smart_choice(left_matches, min_matches, get_min_from(left_matches - 1, max_matches));
                std::cout << "Компьютер берёт " << one_choice << '\n';
            }

            left_matches -= one_choice;                // убираем взятое количество спичек
            users_turn = !users_turn;                  // передаём право хода другому игроку
        }

        // осталась одна спичка. Чей ход - тот и проиграл:
        if (users_turn)
            std::cout << "| - последняя спичка - Ваша! Вы проиграли!..\nХотите отыграться?..\n\n";
        else
            std::cout << "Поздравляем! Вы выиграли, оставив компьютеру последнюю спичку!..\nДайте шанс ему отыграться!\n\n";
        Sleep(5000);
    }
}