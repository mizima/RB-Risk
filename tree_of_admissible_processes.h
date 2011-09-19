#ifndef TREE_OF_ADMISSIBLE_PROCESSES_H
#define TREE_OF_ADMISSIBLE_PROCESSES_H

#include "node.h"
#include <QObject>

class tree_of_admissible_processes : public QObject //strom moznych prac. postupu
{
    Q_OBJECT
public:
    int time_step;	//casovy krok
    int max_time;
    int capacity;	//celkova kapacita
    int MDs;		//celkova pracnost projektu
    int heuristic_threshold;
    list<activity> activities; //zadani
    list<node> last_floor; //posledni patro stromu, z nej lze zrekonstruovat strom cely...

    string solve(string path, int heur_par);				//resic - pro inicializaci vola fill_tree
    int load_input(string path);			//naplni strom daty z otevreneho souboru, pokud tam je smeti, zhavaruje to :], vraci kod chyby (0=ok)
    int add_activity(string new_activity);	//prida aktivitu ctenou ze souboru, hlida dostatek kapacit
    void initialize_first_floor();			//sestavi prvni patro stromu
    bool iterate_last_floor();				//prejde do dalsiho casoveho okamziku a zalozi vsechna mozna vetveni
    void heuristic();						//heuristicky proreze strom
    string write_output();
    string export_process(list<done_activity> * process);

    void initialize_sequence_of_activities(string path,bool dependency_on_fixed_time); //zinicializuje zacatky aktivit dle souboru daneho cestou, ridi, jaka metoda umele navaznosti se pouzije (cas/predchudce)
    string mc_study(node process, int N);
    void mc_study2(node process, int N, int* a);
    //int mc_realization(node process,mt19937 * peng);
    int mc_realization(node process);
signals:
    void changeProgress(int);
};

#endif // TREE_OF_ADMISSIBLE_PROCESSES_H
