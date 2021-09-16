#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <cmath>

void DragAndDrop(void);
void BoardInit(void);
void CheckWinner(void);
void blackTurn(void);


//-------------------------Глобальные переменные--------------------------
sf::RenderWindow window(sf::VideoMode(672, 672), "Corners_SFML");

sf::Font textFont;
sf::Text txt("", textFont, 60);
sf::Text txt2(L"Щелкните мышью чтобы перезапустить", textFont, 20);

sf::Texture boardTexture;
sf::Texture blackPawnTexture;
sf::Texture whitePawnTexture;

bool Game = true;

float squareSize = 84;  //размер клетки
float offset = 42;      //для центрирования спрайта
float dx, dy;   //перемещение фигуры

bool whiteTurn = true;  //переменная хода. 1 - белые, 0 - черные

class pawn : public sf::Sprite {
public:
    bool isMoving = false;  //флаг перетаскивания
    bool isInZone = false;  //флаг выполнения победных условий
   // bool isPlayerTurn = true;   //Флаг хода игрока. true - ход игрока (белые) false - ход бота
    //bool isInTrigger = false;   //пешка находится в победной зоне
    sf::Vector2f prevCoord; //вектор координат пешки на момент начала движения. Фигура возвращается на координаты
};

pawn bp[9]; //черные пешки
pawn wp[9]; //белые пешки

void BoardInit()
{
    //Расстановка пешек------------------------------------------------------------------------------------
    for (int i = 0; i <= 8; i++)
    {
        bp[i].setTexture(blackPawnTexture);
        bp[i].setOrigin(bp[i].getLocalBounds().width / 2, bp[i].getLocalBounds().height / 2);
        //раскидать пешки по рядам
        if (i < 3)
            bp[i].setPosition(offset + squareSize * i, offset);
        else if (i < 6)
            bp[i].setPosition(offset + squareSize * (i - 3), offset + squareSize);
        else
            bp[i].setPosition(offset + squareSize * (i - 6), offset + squareSize * 2);
    }
    for (int i = 0; i <= 8; i++)
    {
        wp[i].setTexture(whitePawnTexture);
        wp[i].setOrigin(wp[i].getLocalBounds().width / 2, wp[i].getLocalBounds().height / 2);
        //раскидать пешки по рядам
        if (i < 3)
            wp[i].setPosition(offset + squareSize * (5 + i), offset + squareSize * 5);
        else if (i < 6)
            wp[i].setPosition(offset + squareSize * (5 + i - 3), offset + squareSize * 6);
        else
            wp[i].setPosition(offset + squareSize * (5 + i - 6), offset + squareSize * 7);
    }
    //------------------------------------------------------------------------------------------------------
}

void DragAndDrop()
{
    //drag & drop-----------------------------------------------------------------------------------------------------------------------
    for (int i = 0; i <= 8; i++)
    {
        if (bp[i].isMoving)
        {
            //вычисление приращения координат чёрных
            dx = sf::Mouse::getPosition(window).x - bp[i].getPosition().x;
            dy = sf::Mouse::getPosition(window).y - bp[i].getPosition().y;
            //Новая позиция (в процессе перетаскивания)
            bp[i].setPosition((dx + bp[i].getPosition().x), (dy + bp[i].getPosition().y));
            //вывод координат для отладки
            std::cout << sf::Mouse::getPosition(window).x << "," << sf::Mouse::getPosition(window).y << std::endl;
            std::cout << bp[i].getPosition().x << "," << bp[i].getPosition().y << std::endl;

        }
        if (wp[i].isMoving)
        {
            //вычисление приращения координат белых
            dx = sf::Mouse::getPosition(window).x - wp[i].getPosition().x;
            dy = sf::Mouse::getPosition(window).y - wp[i].getPosition().y;
            // Новая позиция (в процессе перетаскивания)
            wp[i].setPosition((dx + wp[i].getPosition().x), (dy + wp[i].getPosition().y));
            //вывод координат для отладки
            std::cout << sf::Mouse::getPosition(window).x << "," << sf::Mouse::getPosition(window).y << std::endl;
            std::cout << wp[i].getPosition().x << "," << wp[i].getPosition().y << std::endl;

        }
    }
//---------------------------------------------------------------------------------------------------------------------------------
}

void CheckWinner()
{
int whiteCount = 0; 
int blackCount = 0;
    for (int i = 0; i <= 8; i++)
    {
        if (wp[i].getPosition().x <= squareSize * 3 && wp[i].getPosition().y <= squareSize * 3)
            whiteCount++;
        if (bp[i].getPosition().x >= squareSize * 5 && bp[i].getPosition().y >= squareSize * 5)
            blackCount++;
    }
    if (whiteCount == 9)
    {
        std::cout << "whites won" << std::endl;
        txt.setString(L"Победа белых");
        txt.setPosition(squareSize * 1.6, squareSize * 3);
        Game = false;
        return;
    }
    if (blackCount == 9)
    {
        std::cout << "blacks won" << std::endl;
        txt.setString(L"Победа Черных");
        txt.setPosition(squareSize * 1.5, squareSize * 3);
        Game = false;
        return;
    }

}

void blackTurn() //ход бота
{
    int pawnNum;    //случайная пешка
    int moveNumX;   //случайный ход
    int moveNumY;
    bool collision = false;

    
    while (1)
    {
        pawnNum = rand() % 9;   
        sf::Vector2f prevPos = bp[pawnNum].getPosition();
        //moveNumX = (rand() % 3-1)*squareSize; //рандом 
        //moveNumY = (rand() % 3-1)*squareSize;
        moveNumX = (rand() % 2) * squareSize; //рандом по диагонали в триггер (назад не ходит)
        moveNumY = (rand() % 2) * squareSize;
        sf::Vector2f newPos{ (float)moveNumX, (float)moveNumY };
        whiteTurn = 1;

        bp[pawnNum].setPosition(bp[pawnNum].getPosition().x+moveNumX, bp[pawnNum].getPosition().y + moveNumY);
        if (bp[pawnNum].getPosition().x > 0 && bp[pawnNum].getPosition().x < 672 && bp[pawnNum].getPosition().y>0 && bp[pawnNum].getPosition().y < 672) //если вписывается в окно
        {
            for (int i = 0; i <= 8; i++)    //проверка коллизии с белыми
            {
                if (bp[pawnNum].getGlobalBounds().intersects(wp[i].getGlobalBounds()))
                {
                    collision = true;
                }
            }
            for (int i = 0; i < pawnNum; i++) //коллизия с черными до номера текущей пешки
            {
                if (bp[pawnNum].getGlobalBounds().intersects(bp[i].getGlobalBounds()))
                {
                    collision = true;
                }
            }
            for (int i = pawnNum+1; i <= 8; i++)//коллизия с черными после текущей пешки
            {
                if (bp[pawnNum].getGlobalBounds().intersects(bp[i].getGlobalBounds()))
                {
                    collision = true;
                }
            }
            if (newPos==prevPos) // когда выкинет сдвиг "на ноль"
                collision = true;
            if (collision)
            {
                bp[pawnNum].setPosition(prevPos);
                collision = false;
            }
            else
                return;

        }
        else bp[pawnNum].setPosition(prevPos);
    }
}

//--------------------------------------------------------------------

int main()
{
    textFont.loadFromFile("OpenSans-Regular.ttf");
    txt.setFillColor(sf::Color::Black);
    txt2.setFillColor(sf::Color::Black);
    txt2.setPosition(squareSize * 1.5, squareSize * 4);
    boardTexture.loadFromFile("simple_chess_board.jpg");
    blackPawnTexture.loadFromFile("bp.png");
    whitePawnTexture.loadFromFile("wp.png");

    sf::Sprite boardSprite(boardTexture);
    sf::Sprite blackPawnSprite(blackPawnTexture);
    sf::Sprite whitePawnSprite(whitePawnTexture);
    boardSprite.setScale(0.35, 0.35);
    blackPawnSprite.setScale(1.4, 1.4);
    whitePawnSprite.setScale(1.4, 1.4);

    BoardInit();
 
//Игровая петля
    while (window.isOpen())
    {
        sf::Event event;
        //Петля событий 
        while (window.pollEvent(event))
        {
            //Если нажата кнопка мыши------------------------------------------------------------------------------------------------------------------------
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (Game)
                {
                    if (event.key.code == sf::Mouse::Left)
                    {
                        for (int i = 0; i <= 8; i++) //поиск перетаскиваемой фигуры
                        {
                            if (bp[i].getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y) && !whiteTurn)
                            {
                                bp[i].isMoving = true;
                                bp[i].prevCoord = bp[i].getPosition();
                            }
                            if (wp[i].getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y) && whiteTurn)
                            {
                                wp[i].isMoving = true;
                                wp[i].prevCoord = wp[i].getPosition();
                            }
                        }

                    }
                }
                else 
                {
                    Game = true;
                    BoardInit();
                    whiteTurn = true;
                }
                
            }
            //-----------------------------------------------------------------------------------------------------------------------------------

            //Если отпущена кнопка мыши-----------------------------------------------------------------------------------------------------------
            if (event.type == sf::Event::MouseButtonReleased)
            {
                float newX, newY;   //Координаты фигуры после центрирования спрайта на поле  
                float curX, curY;   //текущие координаты перетаскиваемой фигуры
                for (int i = 0; i <= 8; i++)
                {
                    if (bp[i].isMoving)
                    {
                        curX = bp[i].getPosition().x;
                        curY = bp[i].getPosition().y;
                        //центрирование спрайта и ограничение по дальности шага
                        if ((fabs(curX - bp[i].prevCoord.x) <= (squareSize+offset)) && (fabs(curY - bp[i].prevCoord.y) <= (squareSize+offset))) 
                        {
                            newX = ((int)(curX / squareSize)) * squareSize + offset;
                            newY = ((int)(curY / squareSize)) * squareSize + offset;
                            bp[i].setPosition(newX, newY);
                        }
                        else 
                        {
                            bp[i].setPosition(bp[i].prevCoord);
                        }
                        //отработка коллизии черных
                        for (int j = 0; j <= 8; j++)
                        {
                            if (bp[i].getGlobalBounds().contains(wp[j].getPosition()) || (bp[i].getGlobalBounds().contains(bp[j].getPosition()) && i!=j) )
                                bp[i].setPosition(bp[i].prevCoord);
                        }
                        whiteTurn = true;   //ход переходит белым при отпускании мыши
                    }
                    if (wp[i].isMoving)
                    {
                        curX = wp[i].getPosition().x;
                        curY = wp[i].getPosition().y;
                        //центрирование спрайта и ограничение по дальности шага
                        if ((fabs(curX - wp[i].prevCoord.x) <= (squareSize + offset)) && (fabs(curY - wp[i].prevCoord.y) <= (squareSize + offset))) 
                        {
                            newX = ((int)(curX / squareSize)) * squareSize + offset;
                            newY = ((int)(curY / squareSize)) * squareSize + offset;
                            wp[i].setPosition(newX, newY);
                        }
                        else
                        {
                            wp[i].setPosition(wp[i].prevCoord);
                        }
                        //отработка коллизии белых
                        for (int j = 0; j <= 8; j++)
                        {
                            if (wp[i].getGlobalBounds().contains(bp[j].getPosition()) || (wp[i].getGlobalBounds().contains(wp[j].getPosition()) && i != j))
                                wp[i].setPosition(wp[i].prevCoord);
                        }
                        whiteTurn = false;   //ход переходит чёрным при отпускании мыши
                        blackTurn();
                    }
                    bp[i].isMoving = false;
                    wp[i].isMoving = false;           
                }
                dx = dy = 0;

                CheckWinner();  //Проверка победителя
            }
            //------------------------------------------------------------------------------------------------------------------------------
            //Если игровое окно закрывается
            if (event.type == sf::Event::Closed)
                window.close();
        } //конец петли событий
        //----------------------------------------------------------------------------------------------------------------------------------
        DragAndDrop();
        
        window.clear();
        window.draw(boardSprite);   //отрисовка доски
        //Отрисовка пешек
        for (int i = 0; i <= 8; i++)
            window.draw(bp[i]);
        for (int i = 0; i <= 8; i++)
            window.draw(wp[i]);

        
        if (!Game)
        {
            window.draw(txt);
            window.draw(txt2);
        }
        window.display();
    }

    return 0;
}