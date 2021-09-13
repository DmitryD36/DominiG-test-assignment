#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

int main()
{
    float squareSize = 84;  //������ ������
    float offset = 42;      //��� ������������� �������
  
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
        bool isMoving = false;  //���� ��������������
        bool isInZone = false;  //���� ���������� �������� �������
        sf::Vector2f prevCoord; //������ ��������� ����� �� ������ ������ ��������. ������ ������������ �� ����������
    };
    
 
    pawn bp[9]; //������ �����
    pawn wp[9]; //����� �����

    float dx, dy;   //����������� ������

//����������� �����------------------------------------------------------------------------------------
    for (int i = 0; i <= 8; i++)
    {
        bp[i].setTexture(blackPawnTexture);
        bp[i].setOrigin(bp[i].getLocalBounds().width / 2, bp[i].getLocalBounds().height / 2);
        //��������� ����� �� �����
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
        //��������� ����� �� �����
        if (i < 3)
            wp[i].setPosition(offset + squareSize * (5 + i), offset+squareSize * 5);
        else if (i < 6)
            wp[i].setPosition(offset + squareSize * (5 + i - 3), offset + squareSize * 6);
        else
            wp[i].setPosition(offset + squareSize * (5 + i - 6), offset + squareSize * 7);
    }
//------------------------------------------------------------------------------------------------------
 
//������� �����
    while (window.isOpen())
    {
        sf::Event event;
        //����� ������� 
        while (window.pollEvent(event))
        {
            //���� ������ ������ ����------------------------------------------------------------------------------------------------------------------------
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.key.code == sf::Mouse::Left) 
                {
                    for (int i = 0; i <= 8; i++) //����� ��������������� ������
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

            //���� �������� ������ ����-----------------------------------------------------------------------------------------------------------
            if (event.type == sf::Event::MouseButtonReleased)
            {
                float newX, newY;   //���������� ������ ����� ������������� ������� �� ����  
                float curX, curY;   //������� ���������� ��������������� ������
                for (int i = 0; i <= 8; i++)
                {
                    if (bp[i].isMoving)
                    {
                        curX = bp[i].getPosition().x;
                        curY = bp[i].getPosition().y;
                        //������������� ������� � ����������� �� ��������� ����
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
                        //��������� �������� ������
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
                        //������������� ������� � ����������� �� ��������� ����
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
                        //��������� �������� �����
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
            //���� ������� ���� �����������
            if (event.type == sf::Event::Closed)
                window.close();
        } //����� ����� �������
        //----------------------------------------------------------------------------------------------------------------------------------

        //drag & drop-----------------------------------------------------------------------------------------------------------------------
        for (int i = 0; i <= 8; i++)
        {
            if (bp[i].isMoving)
            {
                //���������� ���������� ��������� ������
                dx = sf::Mouse::getPosition(window).x - bp[i].getPosition().x;
                dy = sf::Mouse::getPosition(window).y - bp[i].getPosition().y;
                //����� ������� (� �������� ��������������)
                bp[i].setPosition((dx + bp[i].getPosition().x), (dy + bp[i].getPosition().y));
                //����� ��������� ��� �������
                std::cout << sf::Mouse::getPosition(window).x << "," << sf::Mouse::getPosition(window).y << std::endl;
                std::cout << bp[i].getPosition().x << "," << bp[i].getPosition().y << std::endl;

            }
            if (wp[i].isMoving)
            {
                //���������� ���������� ��������� �����
                dx = sf::Mouse::getPosition(window).x - wp[i].getPosition().x;
                dy = sf::Mouse::getPosition(window).y - wp[i].getPosition().y;
                // ����� ������� (� �������� ��������������)
                wp[i].setPosition((dx + wp[i].getPosition().x), (dy + wp[i].getPosition().y));
                //����� ��������� ��� �������
                std::cout << sf::Mouse::getPosition(window).x << "," << sf::Mouse::getPosition(window).y << std::endl;
                std::cout << wp[i].getPosition().x << "," << wp[i].getPosition().y << std::endl;

            }
        }
        //---------------------------------------------------------------------------------------------------------------------------------
        window.clear();
        window.draw(boardSprite);   //��������� �����
        //��������� �����
        for (int i = 0; i <= 8; i++)
            window.draw(bp[i]);
        for (int i = 0; i <= 8; i++)
            window.draw(wp[i]);

        window.display();
    }

    return 0;
}