#include <iostream>
#include <fstream>

using namespace std;


class Game
{
    public:
    void startGame() {
        loadGame();

        do 
        {
            startNewYear();

            if (isOver)
                break;

            saveGame();
        } while (year <= C_FINALE_YEAR && !isExit);
        
        gameover();
    }
    
    private:
    void startNewYear() {
        // 1 СТАДИЯ. Сводка новостей 
        cout << "\n------------------------------------------------------------------------------------------\n";
        cout << " Мой повелитель, соизволь поведать тебе\n";
        cout << '\t' << year << " год твоего высочайшего правления\n";

        if (deathCivilsByHunger > 0) {
            cout << "\tКоличество людей, умерших от голода составляет " << deathCivilsByHunger << " человек\n";
        }

        if (newCivils > 0) {
            cout << "\tКоличество людей, приехавших в город составляет " << newCivils << " человек\n";
        }

        if (isPlague) {
            cout << "\tЧума уничтожила половину населения\n";
        }

        cout << "\tНаселение города сейчас составляет " << civils << " человек\n";
        cout << "\tМы собрали " << plusSeedsByCollect << " бушелей пшеницы, и поолучили по " << plusSeedsByAcre << " бушеля с акра\n";
        cout << "\tКрысы истребили " << eatSeedsByRats << " бушелей пшеницы, оставив " << seeds << " бушеля в амбарах\n";
        cout << "\tГород сейчас занимает " << earths << " акров\n";
        cout << "\t1 акр земли стоит сейчас " << priceEarth << " бушель\n\n";
        
        // 2 СТАДИЯ. Выбор атрибутов
        do {
            cout << " Сколько акров земли повелеваешь купить? ";
            cin >> buyEarths;
            cout << " Сколько акров земли повелеваешь продать? ";
            cin >> sellEarths;
            cout << " Сколько пшеницы использовать на семена? ";
            cin >> seedsInFoods;
            cout << " Сколько акров земли повелеваешь засеять? ";
            cin >> earthsInSeeds;

            if (buyEarths >= 0 && sellEarths >= 0 && seedsInFoods >= 20 && earthsInSeeds > 0) {
                int tempEarths = buyEarths - sellEarths;
                int tempSeeds = sellEarths * priceEarth - buyEarths * priceEarth - seedsInFoods;
                if (earths + tempEarths > 0 && seeds + tempSeeds > 0) {
                    if ((civils - earthsInSeeds / 10 >= 0) && ((seeds + tempSeeds) - earthsInSeeds * 5 > 0)) {
                        break;
                    }
                }
                cout << "О, повелитель, пощади нас! У нас только " << civils << " человек, " << seeds << " бушелей пшеницы и " << earths << " акров земли!\n\n";
            }
            else {
                cout << "О, повелитель, вы неправильно заполнили пергамент!\n\n";
            }
        } while (true);

        earths += buyEarths;
        seeds -= buyEarths * priceEarth;
        earths -= sellEarths;
        seeds += sellEarths * priceEarth;
        seeds -= seedsInFoods;

        // 3 СТАДИЯ. Рандомизация атрибутов нв следующий год
        plusSeedsByAcre = 1 + (rand() % 5);
        plusSeedsByCollect = plusSeedsByAcre * earthsInSeeds;
        eatSeedsByRats = rand() % int(0.07 * (seeds + plusSeedsByCollect));
        deathCivilsByHunger = civils - (seedsInFoods / 20);
        deathCivilsByHunger = deathCivilsByHunger > 0 ? deathCivilsByHunger : 0;
        newCivils = deathCivilsByHunger / 2 + (5 - plusSeedsByAcre) * (seeds + plusSeedsByCollect) / 600 + 1;
        newCivils = (newCivils < 0) ? 0 : 
                    (newCivils > 50) ? 50 : 
                    newCivils;
        isPlague = ((rand() % 100) < 15) ? true : false;
        civils = isPlague ? (civils / 2) : civils;
        priceEarth = C_MIN_PRICE + rand() % (C_MAX_PRICE - C_MIN_PRICE + 1);
        
        if (deathCivilsByHunger == 0) {
            seeds += (seedsInFoods - civils * 20);
        }
        seeds += plusSeedsByCollect - eatSeedsByRats - (earthsInSeeds * 5);
        civils += newCivils - deathCivilsByHunger;
        year++;
    }

    void gameover() {
        if (isExit)
            return;

        p = p / 10 * 100;
        l = earths / civils;

        cout << "\n------------------------------------------------------------------------------------------\n";
        if ((p > 33 && l < 7) || (isOver))
            cout << "Из-за вашей некомпетентности в управлении, народ устроил бунт, и изгнал вас их города.Теперь вы вынуждены влачить жалкое существование в изгнании\n";
        else if (p > 10 && l < 9)
            cout << "Вы правили железной рукой, подобно Нерону и Ивану Грозному.Народ вздохнул с облегчением, и никто больше не желает видеть вас правителем\n";
        else if (p > 3 && l < 10)
            cout << "Вы справились вполне неплохо, у вас, конечно, есть недоброжелатели, но многие хотели бы увидеть вас во главе города снова\n";
        else 
            cout << "Фантастика! Карл Великий, Дизраэли и Джефферсон вместе не справились бы лучше\n";
        cout << "------------------------------------------------------------------------------------------\n";

        system("pause");
    }

    void saveGame() {
        bool isContinue = 1;

        cout << "\tПрошел ещё один год. \n\t\tЖелаете продолжить (1)? Или хотите ненадолго отвлечься от важных дел (0)?: ";
        cin >> isContinue;

        if (!isContinue) {
            ofstream saveFile;
            saveFile.open("save.txt", ios::binary);
            saveFile << year << endl << civils << endl << seeds << endl << earths << endl;
            saveFile << deathCivilsByHunger << endl << newCivils << endl << eatSeedsByRats << endl << plusSeedsByCollect << endl << plusSeedsByAcre << endl << plusEarths << endl << priceEarth << endl << isPlague << endl;
            saveFile << p << endl << l;

            saveFile.close();

            isExit = true;
        }
    }

    void loadGame() {
        ifstream loadFile;
        loadFile.open("save.txt", ios::binary);

        if (loadFile.is_open()) {
            bool isContinue = true;

            cout << "\tНайдено сохранение. \n\t\tЖелаете продолжить (1)? Или начать новую историю (0)?: ";
            cin >> isContinue;

            if (isContinue) {
                loadFile >> year >> civils >> seeds >> earths;
                loadFile >> deathCivilsByHunger >> newCivils >> eatSeedsByRats >> plusSeedsByCollect >> plusSeedsByAcre >> plusEarths >> priceEarth >> isPlague;
                loadFile >> p >> l;
            }
        }

        loadFile.close();
    }

     // ресурсы
     int year = 1;
     int civils = 100;
     int seeds = 2800;
     int earths = 1000;
     
     // константы
     const int C_FINALE_YEAR = 10;
     const int C_MIN_PRICE = 17;
     const int C_MAX_PRICE = 26;

     // атрибуты влияющие на ресурсы
     int deathCivilsByHunger = 0;
     int newCivils = 0;
     int eatSeedsByRats = 0;
     int plusSeedsByCollect = 0;
     int plusSeedsByAcre = 0;
     int plusEarths = 0;
     int priceEarth = C_MIN_PRICE + rand() % (C_MAX_PRICE - C_MIN_PRICE + 1);
     bool isPlague = false;
     bool isOver = false;
     bool isExit = false;
     
     // атрибуты вводимые игроком
     int buyEarths = 0;
     int sellEarths = 0;
     int seedsInFoods = 0;
     int earthsInSeeds = 0;
    
     // статистика
     float p = 0.0;
     float l = earths / civils;
};


int main()
{
    setlocale(LC_CTYPE, "rus");

    Game game;
    game.startGame();
}