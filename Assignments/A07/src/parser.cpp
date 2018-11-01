#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ncurses.h>
#include <sys/stat.h>
#include <cstring>

class dice
{
  public:
    int base;
    int number;
    int sides;
};

class monster
{
  public:
    std::string name;
    std::string description;
    //    int color;
    std::string color;
    dice speed;
    std::string ability;
    dice health;
    dice damage;
    char symbol;
    int rarity;

    void clear()
    {
        name = "";
        description = "";
        //        color = NULL;
        color = "";
        speed.base = -1;
        speed.number = -1;
        speed.sides = -1;
        ability = "";
        health.sides = -1;
        health.base = -1;
        health.number = -1;
        damage.sides = -1;
        damage.number = -1;
        damage.base = -1;
        symbol = ' ';
        rarity = -1;
    }

    void print_monster()
    {
        std::cout << name << std::endl;
        std::cout << description;
        std::cout << symbol << std::endl;
        std::cout << color << std::endl;
        std::cout << speed.base << "+" << speed.number << "d" << speed.sides << std::endl;
        std::cout << ability << std::endl;
        std::cout << health.base << "+" << health.number << "d" << health.sides << std::endl;
        std::cout << damage.base << "+" << damage.number << "d" << damage.sides << std::endl;
        std::cout << rarity << std::endl;
        std::cout << std::endl;
    }
};

int main(int argc, char *argv[])
{
    std::string path = getenv("HOME");
    path += "/.rlg327/monster_desc.txt";

    std::ifstream file(path);

    //    if (!std::ifstream(argv[1])) {
    //        std::cout << "File not found." << std::endl;
    //        std::cout << "Usage: ./rlg327 <Monster_Description_File>" << std::endl;
    //    } else {

    //        file.open(argv[1]);

    monster m;
    std::string s;

    std::vector<monster> monster_list;

    getline(file, s);
    //std::cout << s << std::endl;
    if (s != "RLG327 MONSTER DESCRIPTION 1")
    {
        std::cout << "Invalid file header." << std::endl;
        //            std::cout << "Usage: ./rlg327 <Monster_Description_File>" << std::endl;
    }
    else
    {
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
                        if (m.name.empty())
                        {
                            getline(file, s);
                            m.name = s.substr(1, s.size());
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
                        if (m.color.empty())
                        {
                            getline(file, s);
                            m.color = s.substr(1, s.size());
                        }
                        else
                        {
                            m.clear();
                            break;
                        }
                    }
                    else if (s == "DESC")
                    {
                        if (m.description.empty())
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
                        if (m.speed.base == -1 || m.speed.number == -1 || m.speed.sides == -1)
                        {
                            file >> s;
                            m.speed.base = std::stoi(s.substr(0, s.find('+')));
                            m.speed.number = std::stoi(s.substr(s.find('+'), s.find('d')));
                            m.speed.sides = std::stoi(s.substr(s.find('d') + 1, s.length()));
                        }
                        else
                        {
                            m.clear();
                            break;
                        }
                    }
                    else if (s == "DAM")
                    {
                        if (m.damage.base == -1 || m.damage.number == -1 || m.damage.sides == -1)
                        {
                            file >> s;
                            m.damage.base = std::stoi(s.substr(0, s.find('+')));
                            m.damage.number = std::stoi(s.substr(s.find('+'), s.find('d')));
                            m.damage.sides = std::stoi(s.substr(s.find('d') + 1, s.length()));
                        }
                        else
                        {
                            m.clear();
                            break;
                        }
                    }
                    else if (s == "HP")
                    {
                        if (m.health.base == -1 || m.health.number == -1 || m.health.sides == -1)
                        {
                            file >> s;
                            m.health.base = std::stoi(s.substr(0, s.find('+')));
                            m.health.number = std::stoi(s.substr(s.find('+'), s.find('d')));
                            m.health.sides = std::stoi(s.substr(s.find('d') + 1, s.length()));
                        }
                        else
                        {
                            m.clear();
                            break;
                        }
                    }
                    else if (s == "ABIL")
                    {
                        if (m.ability.empty())
                        {
                            getline(file, s);
                            m.ability = s.substr(1, s.size());
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
                if (!m.name.empty() &&
                    m.rarity != -1 &&
                    !m.ability.empty() &&
                    (m.health.base != -1 && m.health.number != -1 && m.health.sides != -1) &&
                    !m.description.empty() &&
                    !m.color.empty() &&
                    m.symbol != ' ' &&
                    (m.damage.base != -1 && m.damage.number != -1 && m.damage.sides != -1) &&
                    (m.speed.base != -1 && m.speed.number != -1 && m.speed.sides != -1))
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

        //            std::ofstream outFile;
        //
        //            mkdir(strcat(getenv("HOME"), "/.rlg327"), 0700);
        //
        //            outFile.open(path);
        //
        //            for (i = 0; i < (int) monster_list.size(); i++) {
        //                outFile << monster_list[i].name << std::endl;
        //                outFile << monster_list[i].description;
        //                outFile << monster_list[i].symbol << std::endl;
        //                outFile << monster_list[i].color << std::endl;
        //                outFile << monster_list[i].speed.base << "+" << monster_list[i].speed.number << "d" << monster_list[i].speed.sides << std::endl;
        //                outFile << monster_list[i].ability << std::endl;
        //                outFile << monster_list[i].health.base << "+" << monster_list[i].health.number << "d" << monster_list[i].health.sides<< std::endl;
        //                outFile << monster_list[i].damage.base << "+" << monster_list[i].damage.number << "d" << monster_list[i].damage.sides<< std::endl;
        //                outFile << monster_list[i].rarity << std::endl;
        //                outFile << std::endl;
        //            }
        //
        //            outFile.close();
        file.close();

        return 0;
    }
}
//}
