#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ncurses.h>

class monster
{
  public:
    std::string name;
    std::string description;
    //    int color;
    std::string color;
    std::string speed;
    std::string ability;
    std::string health;
    std::string damage;
    char symbol;
    int rarity;

    void clear()
    {
        name = "";
        description = "";
        //        color = NULL;
        color = "";
        speed = "";
        ability = "";
        health = "";
        damage = "";
        symbol = ' ';
        rarity = -1;
    }

    void print_monster()
    {
        std::cout << "Name: " << name << std::endl;
        std::cout << "Description: " << std::endl
                  << description;
        std::cout << "Color: " << color << std::endl;
        std::cout << "Speed: " << speed << std::endl;
        std::cout << "Ability: " << ability << std::endl;
        std::cout << "Health: " << health << std::endl;
        std::cout << "Damage: " << damage << std::endl;
        std::cout << "Symbol: " << symbol << std::endl;
        std::cout << "Rarity: " << rarity << std::endl;
        std::cout << std::endl;
    }
};

int main(int argc, char *argv[])
{
    std::ifstream file;

    if (!std::ifstream(argv[1]))
    {
        std::cout << "File not found." << std::endl;
        std::cout << "Usage: ./rlg327 <Monster_Description_File>" << std::endl;
    }
    else
    {

        file.open(argv[1]);

        monster m;
        std::string s;

        std::vector<monster> monster_list;

        getline(file, s);
        getline(file, s);

        char symbol;

        while (file.peek() != EOF)
        {
            file >> s;
            if (s == "BEGIN")
            {
                m.clear();
                while (s != "END")
                {
                    file >> s;
                    if (s == "NAME")
                    {
                        if (m.name == "")
                        {
                            getline(file, s);
                            m.name = s;
                        }
                        else
                        {
                            m.clear();
                            break;
                        }
                    }
                    else if (s == "SYMB")
                    {
                        if (m.symbol == ' ')
                        {
                            file >> symbol;
                            m.symbol = symbol;
                        }
                        else
                        {
                            m.clear();
                            break;
                        }
                    }
                    else if (s == "COLOR")
                    {
                        if (m.color == "")
                        {
                            getline(file, s);
                            m.color = s;
                        }
                        else
                        {
                            m.clear();
                            break;
                        }
                    }
                    else if (s == "DESC")
                    {
                        if (m.description == "")
                        {
                            getline(file, s); // this gets rid of the original end line after DESC
                            while (file.peek() != '.')
                            {
                                getline(file, s);
                                if (s.length() < 78)
                                {
                                    m.description += s;
                                    m.description += '\n';
                                }
                                else
                                {
                                    m.description = "";
                                    m.clear();
                                    break;
                                }
                            }
                        }
                        else
                        {
                            m.clear();
                            break;
                        }
                    }
                    else if (s == "SPEED")
                    {
                        if (m.speed == "")
                        {
                            file >> s;
                            m.speed = s;
                        }
                        else
                        {
                            m.clear();
                            break;
                        }
                    }
                    else if (s == "DAM")
                    {
                        if (m.damage == "")
                        {
                            file >> s;
                            m.damage = s;
                        }
                        else
                        {
                            m.clear();
                            break;
                        }
                    }
                    else if (s == "HP")
                    {
                        if (m.health == "")
                        {
                            file >> s;
                            m.health = s;
                        }
                        else
                        {
                            m.clear();
                            break;
                        }
                    }
                    else if (s == "ABIL")
                    {
                        if (m.ability == "")
                        {
                            getline(file, s);
                            m.ability = s;
                        }
                        else
                        {
                            m.clear();
                            break;
                        }
                    }
                    else if (s == "RRTY")
                    {
                        if (m.rarity == -1)
                        {
                            int r;
                            file >> r;
                            m.rarity = r;
                        }
                        else
                        {
                            m.clear();
                            break;
                        }
                    }
                    else
                    {
                    }
                }
                if (m.name != "" &&
                    m.rarity != -1 &&
                    m.ability != "" &&
                    m.health != "" &&
                    m.description != "" &&
                    m.color != "" &&
                    m.symbol != ' ' &&
                    m.damage != "" &&
                    m.speed != "")
                {
                    monster_list.push_back(m);
                }
                m.clear();
            }
        }

        int i;

        for (i = 0; i < (int)monster_list.size(); i++)
        {
            monster_list[i].print_monster();
        }

        std::cout << "Number of Monsters: " << monster_list.size() << std::endl;

        file.close();

        return 0;
    }
}
