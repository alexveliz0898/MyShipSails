#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
using namespace std;
//printing the cheat board needs some touching up. Its ugly
struct Card{
    string suite, face, card;
    Card(): suite(""), face(""){};                        //defualt constructor for discardList[5]
    Card(string a, string b):suite(a), face(b){           //our constructor
        card = suite + face;
    }
    string getCard(){ return card;}     //memeber functions to return specific strings
    string getFace(){ return face;}
    string getSuite(){return suite;}
};

class Player{
public:
    Player(int a, int b, vector<Card> hand): hand(hand), selfIndex(a), takeFrom(b), discardIndex(0){};

    void printHand(){
        cout<<"         ";
        for(int i = 0; i < hand.size(); i ++){
            Card var = hand[i];
            cout<<var.getCard()<<"  ";
        }
    }
    void replenish(int takeFrom, int discardIndex){
        this->discardIndex = discardIndex;
        cout<<"Player "<<selfIndex<<" gave Player "<<(selfIndex + 1) % 5<<" "<<hand[discardIndex].getCard()<<endl;
        hand[discardIndex] = discardList[takeFrom];
    }

    virtual void chooseDiscard(int& discardIndex){
        this->discardIndex = discardIndex;
        discardList[selfIndex] = hand[discardIndex];
        //hand.erase(hand.begin() + discardIndex);    //try not to erase, lets just reassgin
    }
    int discardIndex;           //position within hand of card being discarded
    int takeFrom;               //player supplying new Card
    vector<Card> hand;          //holds 7
    
protected:
    int selfIndex;              //index within player vector, starting with 0 for the human
    static Card discardList[5]; //this list will be accessed by all Players
    virtual ~Player() {};
};

class AutoPlayer: public Player{
public:
    AutoPlayer(int a, int b, vector<Card> hand): Player(a, b, hand){};
    void chooseDiscard(int& discardIndex);
};

void createDeck(vector<Card>& deck);                                //will create & shuffle deck;
void createPlayers(vector<Player *>& players, vector<Card>& deck);  //creates player & autoplayer
void cheatCodes(vector<Player *>& players);
void printOurHand(vector<Player *> players);
void WillShipSail(vector<Player *> players, int&, bool&);
//void CheckHand(vector<Player *> players);

Card Player::discardList[5]; //why the fuck cant u be in main <- this guy.stupid();
//because it is a static member, memory mst be allocated outside of the Class 

int main(){
    vector<Card> deck;
    vector<Player *> players;
    createDeck(deck);
    createPlayers(players, deck);
    bool MyShipSails = false;
    char command;
    int removeAt, winner;

    while(MyShipSails == false){
        printOurHand(players);
        cout<<"cmd: ";
        cin>>command;

        if(command == '?')
            cheatCodes(players);

        if(command == 'x')
            MyShipSails = true;

        //find the index of the card desired to be removed
        switch(command){
            case 'a': removeAt = 0;
                break;
            case 'b': removeAt = 1;
                break;
            case 'c': removeAt = 2;
                break;
            case 'd': removeAt = 3;
                break;
            case 'e': removeAt = 4;
                break;
            case 'f': removeAt = 5;
                break;
            case 'g': removeAt = 6;
                break;

        }

        players[0]->chooseDiscard(removeAt);

        //logic for player 1, 2, 3, & 4
        for(int i = 1; i <= 4; i++){
            players[i]->chooseDiscard(players[i]->discardIndex);
        }

        players[0]->replenish(4, removeAt);
        //replenish logic for players 1-->4

        for(int i = 1; i <=4; i++){
            players[i]->replenish(players[i]->takeFrom, players[i]->discardIndex);
        }

        WillShipSail(players, winner, MyShipSails);

    }//end of game
    cout<<"Winner is Player "<<winner<<"!"<<endl;
}//end of main

void createDeck(vector<Card>& deck){        //will create & shuffle deck;
    //local var
    srand(time(0));
    string card_suite = "SHCD";
    string card_face[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    string a, b;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j <13; j++){
            a = card_suite[i];
            b = card_face[j];
            Card card(a, b);
            deck.push_back(card);
        }
    }
    random_shuffle(deck.begin(), deck.end());
}

void createPlayers(vector<Player *>& players, vector<Card>& deck){
    vector<Card>hold;
    //temporary local vector variable to pick and choose the first 7 cards fromt the shuffled deck
    hold.assign(deck.begin(), deck.begin() + 7);
    deck.erase(deck.begin(), deck.begin() + 7);

    players.push_back(new Player(0, 4, hold));    //first player(us)
    hold.clear();
    //players 1 --> 4
    int toTakeFrom  = 0;
    for(int i = 1; i <= 4; i++){
        hold.assign(deck.begin(), deck.begin() + 7);
        deck.erase(deck.begin(), deck.begin() + 7);

        players.push_back(new AutoPlayer(i, toTakeFrom, hold));

        hold.clear();
        toTakeFrom++;
    }

};

void AutoPlayer::chooseDiscard(int& discardIndex){
    srand(time(0));
    int H = 0, C = 0, S = 0, D = 0;
    int n, m, discard;

    for(int i = 0; i < hand.size(); i++){
        if(hand[i].getSuite() == "H")
            H++;

        if(hand[i].getSuite() == "D")
            D++;

        if(hand[i].getSuite() == "C")
            C++;

        if(hand[i].getSuite() == "S")
            S++;
    }
    string least1;
    string least2;
    if(H < D){
        m = H;
        least1 = "H";
    }
    else{
        least1 = "D";
        m = D;
    }

    if(C < S){
        least2 = "C";
        n = C;
    }
    else{
        least2 = "S";
        n = S;
    }
    string leastoccuring;


    if(m < n || m == n){
        if(m != 0)
            leastoccuring = least1;
    }
    else
        leastoccuring = least2;

    for(unsigned int i = 0; i < 7; i++){
        if(hand[i].getSuite() == leastoccuring){
            //cout<<i<<endl;
            discardIndex = i;
            break;}
            }
    if(m == 0 || n == 0 || m == 1 || n == 1){
        discardIndex = rand() % 7;
    }
   // cout<<"Player "<<selfIndex<<" least occuring string: "<<leastoccuring<<endl;
   // cout<<"Player "<<selfIndex<<" is giving card at index: "<<discardIndex<<endl;
    this->discardIndex = discardIndex;
    //too avoid an infinite repirtion of cards being thrown out from AutoPlayer index[0]
    if(this->discardIndex == 0){
        this->discardIndex = rand() % 7;
    }

    discardList[selfIndex] = hand[discardIndex];
   // cout<<"discardIndex in Discard function: "<<endl;
}

void printOurHand(vector<Player *> players){
    cout<<"Your hand:"<<endl;
    cout<<"0:";
    players[0]-> printHand();
    cout<<endl;
    cout<<"           a  b  c  d  e  f  g"<<endl;
}

void cheatCodes(vector<Player *>& players){
    cout<<"0:          ";
    players[0]->printHand();
    cout<<endl;

    cout<<"1:          ";
    players[1]->printHand();
    cout<<endl;

    cout<<"2:          ";
    players[2]->printHand();
    cout<<endl;

    cout<<"3:          ";
    players[3]->printHand();
    cout<<endl;

    cout<<"4:          ";
    players[4]->printHand();
    cout<<endl;

}

void WillShipSail(vector<Player *> players, int& winner, bool& MyShipSails){
    string currentSuite;
    vector<string>suites;
    for(int i = 0; i < players.size(); i++){
        bool ShipSails = true;
        suites.clear();

        for(int j = 0; j <= 6; j ++){
            currentSuite = players[i]->hand[j].getSuite();
            suites.push_back(currentSuite);
        }

        for(int h = 0; h < suites.size(); h++){                   //the 'currentSuite' being checked here is the last card of the player
            if(currentSuite != suites[h]){                         //if the currentSuite(last card) does not equal the first card, then that player has not won yet, & it breaks, and goes
                ShipSails = false;
                break;                                            //onto the next player. if it does equal the first card, then it will continue testing until the end of the deck, etc
            }
        }

        if(ShipSails == true){
            winner = i;
            MyShipSails = true;
            break;
        }

    }

}
