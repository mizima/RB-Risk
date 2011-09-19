#ifndef ACTIVITIES_H
#define ACTIVITIES_H

#include "user_def.h"

class activity;

class inaccessible_activity
{
public:
        activity * pactivity;	//pointer na aktivitu ze zadani
        list<string> unsatisfied_dependencies; //navaznosti na dalsi aktivity, ktere se jeste nepodarilo splnit, inicializace ze zadani
};

class accessible_activity
{
public:
        activity * pactivity;	//pointer na aktivitu ze zadani
        list<string> artificial_dependencies; //navaznosti na aktivity, na ktere se cekalo ackoliv jiz byla dana aktivita pristupna. ve vystupu bude na tyto aktivity vytvorena umela navaznost (moznost dependency_on_activities)
};

class ongoing_activity
{
public:
        activity * pactivity;	//pointer na aktivitu ze zadani
        int remaining_time;		//okamzik, ve kterem dana aktivita zacala, dostupnost zdroju je hlidana vyse
        list<string> artificial_dependencies;	//navaznosti na aktivity, na ktere se cekalo ackoliv jiz byla dana aktivita pristupna. ve vystupu bude na tyto aktivity vytvorena umela navaznost (moznost dependency_on_activities)
};

class done_activity
{
public:
        activity * pactivity;					//pointer na aktivitu ze zadani
        int start_time;							//okamzik, ve kterem dana aktivita zacala, ve vystupu bude na tento casovy okamzik vytvorena umela navaznost (moznost dependency_on_time)
        list<string> artificial_dependencies;	//navaznosti na aktivity, na ktere se cekalo ackoliv jiz byla dana aktivita pristupna. ve vystupu bude na tyto aktivity vytvorena umela navaznost (moznost dependency_on_activities)
};

class activity
{
public:
        string name;			//jmeno aktivity
        int sources;			//potrebne zdroje
        int estimates[3];		//optimisticky, realisticky a pesimisticky odhad, lepe resit celou tridou...
        int sharpness;			//jak moc verim realistickemu odhadu oproti ostatnim - 0 -> U(a,c), inf -> Dirac(b)
        int true_time;		    //jak dlouho aktivita skutecne potrva, v optimalizacni casti nastavena na realisticky odhad, v MC casti se bere nahodna realizace
        int MDs;				//man-days potrebne na danou aktivitu
        list<string> dependencies; //navaznosti na dalsi aktivity,

        //void initialize_true_duration(mt19937 * peng);
        void initialize_true_duration();
        inaccessible_activity get_inaccessible_activity();
        accessible_activity get_accessible_activity();
        ongoing_activity get_ongoing_activity(list<string> artificial_dependencies);
        done_activity get_done_activity(int start_time,list<string> artificial_dependencies);
};

bool operator< (accessible_activity x, accessible_activity y);
bool operator==(activity x, activity y);
bool operator==(accessible_activity x, accessible_activity y);
bool operator==(inaccessible_activity x, inaccessible_activity y);
bool operator== (ongoing_activity x, ongoing_activity y);

#endif // ACTIVITIES_H
