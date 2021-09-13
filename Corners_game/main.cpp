#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

int main()
{
    float squareSize = 84;  //размер клетки
    float offset = 42;      //для центрирования спрайта
  
    sf::RenderWindow window(sf::VideoMode(672, 672), "Corners_SFML");
    sf::Texture boardTexture;
    sf::Texture blackPawnTexture;
    sf::Texture whitePawnTexture;
    boardTexture.loadFromFile("simple_chess_board.jpg");
    blackPawnTexture.loadFromFile("bp.png");
    whitePawnTexture.loadFromFile("wp.png");

    sf::Sprite boardSprite(boardTexture);
    sf::Sprite blackPawnSprite(blackPawnTexture);
    sf::Sprite whitePawnSprite(whitePawnTexture);
    boardSprite.setScale(0.35, 0.35);
    blackPawnSprite.setScale(1.4, 1.4);
    whitePawnSprite.setScale(1.4, 1.4);

    class pawn : public sf::Sprite {
    public:
        bool isMoving = false;  //флаг перетаскивания
        bool isInZone = false;  //флаг выполнения победных условий
        sf::Vector2f prevCoord; //вектор координат пешки на момент начала движения. Фигура возвращается на координаты
    };
    
 
    pawn bp[9]; //черные пешки
    pawn wp[9]; //белые пешки

    float dx, dy;   //перемещение фигуры

//Расстановка пешек------------------------------------------------------------------------------------
    for (int i = 0; i <= 8; i++)
    {
        bp[i].setTexture(blackPawnTexture);
        bp[i].setOrigin(bp[i].getLocalBounds().width / 2, bp[i].getLocalBounds().height / 2);
        //раскидать пешки по рядам
        if(i<3)
            bp[i].setPosition(offset+squareSize*i, offset);
        else if (i<6)
            bp[i].setPosition(offset + squareSize * (i - 3), offset+squareSize);
        else
            bp[i].setPosition(offset + squareSize * (i - 6), offset + squareSize * 2);
    }
    for (int i = 0; i <= 8; i++)
    {
        wp[i].setTexture(whitePawnTexture);
        wp[i].setOrigin(wp[i].getLocalBounds().width / 2, wp[i].getLocalBounds().height / 2);
        //раскидать пешки по рядам
        if (i < 3)
            wp[i].setPosition(offset + squareSize * (5 + i), offset+squareSize * 5);
        else if (i < 6)
            wp[i].setPosition(offset + squareSize * (5 + i - 3), offset + squareSize * 6);
        else
            wp[i].setPosition(offset + squareSize * (5 + i - 6), offset + squareSize * 7);
    }
//------------------------------------------------------------------------------------------------------
 
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
                if (event.key.code == sf::Mouse::Left) 
                {
                    for (int i = 0; i <= 8; i++) //поиск перетаскиваемой фигуры
                    {
                        if (bp[i].getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
                        {
                            bp[i].isMoving = true;
                            bp[i].prevCoord = bp[i].getPosition(); 
                        }
                        if (wp[i].getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
                        {
                            wp[i].isMoving = true;
                            wp[i].prevCoord = wp[i].getPosition();
                        }
                    }

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
                    }
                    bp[i].isMoving = false;
                    wp[i].isMoving = false;           
                }
                dx = dy = 0;
            }
            //------------------------------------------------------------------------------------------------------------------------------
            //Если игровое окно закрывается
            if (event.type == sf::Event::Closed)
                window.close();
        } //конец петли событий
        //----------------------------------------------------------------------------------------------------------------------------------

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
        window.clear();
        window.draw(boardSprite);   //отрисовка доски
        //Отрисовка пешек
        for (int i = 0; i <= 8; i++)
            window.draw(bp[i]);
        for (int i = 0; i <= 8; i++)
            window.draw(wp[i]);

        window.display();
    }

    return 0;
}