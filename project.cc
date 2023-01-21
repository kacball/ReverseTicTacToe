#include <iostream>
#include <string>
#include <unistd.h>

int curState[9];

bool isWin(int tab[], int loser) {
    for(int i = 0; i < 3; i+=3) { //horizontally.
        if(tab[i] == loser && tab[i+1] == loser && tab[i+2] == loser)
            return true;
    }
    for(int i = 0; i < 3; i++) { // vertically.
        if(tab[i] == loser && tab[i+3] == loser && tab[i+6] == loser)
            return true;
    }
    if((tab[0] == loser && tab[4] == loser && tab[8] == loser) || (tab[2] == loser && tab[4] == loser && tab[6] == loser)) //diagonally.
        return true;
    return false;
}

bool isAlmostFull(int tab[]) {
    int res = 0;
    for(int i = 0; i < 9; i++) {
        if(tab[i] == 0)
            res++;
    }
    return res == 1;
}
char underline[]={0x1b,'[','4',';','3','9','m',0};
char normal[]={0x1b,'[','0',';','3','9','m',0};
void printTab(int tab[]) {
    for(int i = 0; i < 9; i++) {
        if(i == 0 || i == 3)
            std::cout<<underline;
        if(tab[i] == 1)
            std::cout<<"O";
        else if(tab[i] == 2)
            std::cout<<"X";
        else
            std::cout<<" ";
        if(i%3 == 2)
            std::cout<<normal<<'\n';
        else
            std::cout<<"|";
    }
    std::cout<<'\n';
}

bool isWinning(int tab[], int move) {
    tab[move] = 2;

    // We lose with that move.
    if(isWin(tab, 2)) {
        tab[move] = 0;
        return false;
    }

    if(isAlmostFull(tab)) { // Last move.
        bool nextMoveWin = true;
        for(int i = 0; i < 9; i++) {
            if(tab[i] == 0) {
                tab[i] = 1;
                if(!isWin(tab, 1))
                    nextMoveWin = false;
                tab[i] = 0;
            }
        }
        tab[move] = 0;
        return nextMoveWin;
    }

    for(int i = 0; i < 9; i++) { // Not last move.
        if(tab[i] == 0) {
            tab[i] = 1;
            bool existWinning = false;
            for(int j = 0; j < 9; j++) {
                if(tab[j] == 0) {
                    if(isWinning(tab, j) == true) {
                        existWinning = true;
                    }
                }
            }
            if(existWinning == false) { // For oponents move i, there does not exist my winning move.
                tab[i] = 0;
                tab[move] = 0;
                return false;
            }
            tab[i] = 0;
        }
    }
    tab[move] = 0;
    return true;
}
char red[]={0x1b,'[','0',';','3','1','m',0};
int main() {

    std::cout<< "Cześć, pokażę ci grę w odwrócone kółko i krzyżyk, celem gry jest przegranie.\n";
    std::cout<<normal<< "W odróżnieniu od oryginalnej gry, tutaj 2 gracz ma strategię wygrywającą.\n";
    std::cout<< "Tak będą oznaczane kolejne pola (jak chcesz wpisać tam kółko, to wpisuj dany numer):\n";
    std::cout<<underline<<"1|2|3"<<normal<<"\n"<<underline<<"4|5|6"<<normal<<"\n7|8|9\n";
    std::cout<<"Jeśli chcesz zaczynać, wpisz \"y\", a jeśli chcesz być drugi/a, wpisz \"n\": ";
    std::string n;
    std::cin>>n;
    if(n[0] == 'y') {
        for(int i = 0; i < 5; i++) {
        std::cout<<"Wpisz pole, w którym chciałbyś wpisać swoje kółko: ";
            bool wpisz = true;
            while(wpisz) {
                std::cin>>n;
                if(n[0] <= '9' && n[0] >= '1' && n.size() == 1 && curState[n[0]-'1'] == 0)
                    wpisz = false;
                else {
                    std::cout << "Niestety, źle wpisałeś numer pola (albo już jest zajęte)\n";
                    std::cout<<"Wpisz pole, w którym chciałbyś wpisać swoje kółko: ";
                }
            }
        curState[n[0]-'1'] = 1;
        printTab(curState);
        if(isWin(curState, 1)) {
            std::cout<<red<<"Niestety, właśnie masz 3 w linii i przegrałeś :(\n"<<normal;
            break;
        }
        usleep(500000);
        std::cout <<"Teraz ja wstawię swój krzyżyk, daj mi pomyśleć\n";
        usleep(2000000);
        for(int i = 0; i < 9; i++) {
            if(curState[i] == 0) {
                if(isWinning(curState, i)) {
                    curState[i] = 2;
                    break;
                }
            }
        }
        printTab(curState);
        }
    }
    if(n[0] == 'n') {
        std::cout<< "W trakcie robienia\n";
    }

    return 0;
}
