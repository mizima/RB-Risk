#include "runclass.h"

void DataClass::destroyTrees()
{
    delete tree;
    delete MCtree;
}

void RunClass::run()
{
    emit changeProgress(0);
    calc();
    MonteCarlo();
    exportData();
    emit boxDone();
}

void RunClass::colectData(DataClass *dataCl)
{
    data = dataCl;
}

void RunClass::calc()
{
    data->tree = new tree_of_admissible_processes;
    connect(data->tree, SIGNAL(changeProgress(int)), this, SLOT(getProgress(int)));
    string realistic_optimum;
    realistic_optimum = data->tree->solve(data->input, data->heurPar);
    ofstream output_file("output.txt");
    output_file << realistic_optimum;
    output_file.close();
}

void RunClass::MonteCarlo()
{
    //MC
    data->MCtree = new tree_of_admissible_processes;
    connect(data->MCtree, SIGNAL(changeProgress(int)), this, SLOT(getProgress(int)));
    data->MCtree->load_input(data->input);
    bool variant;
    variant = (data->MCvar > 0); //fuj - musí se upravit
    data->MCtree->initialize_sequence_of_activities("output.txt",variant);//0=varianta 1, 1=varianta 2
    data->MCtree->initialize_first_floor();

    data->mc_study_dats = new int[10000];
    for (int i =0; i<10000; i++)
    data->mc_study_dats[i]=0;

    //output_file.open("mc_study.txt");

    //output_file <<
    data->MCtree->mc_study2((node)*(data->MCtree->last_floor.begin()), (int)(data->MCsamples), (int*)(data->mc_study_dats));
    //output_file.close();

    for (int i=0; i<10000; i++) {
        if(data->mc_study_dats[i] >0) {
            data->mc_study_min = i;
            break;
        }
    }

    for (int i=9999; i>-1; i--) {
        if(data->mc_study_dats[i] >0) {
            data->mc_study_max = i;
            break;
        }
    }
}

void RunClass::exportData()
{
    // output
    int i_num_act = (int) data->tree->activities.size();
    int i_num_pool = data->tree->capacity;

    ofstream output_file_data("data.js");
    string project_name = data->strProjectName.toStdString();
    string notes = data->notes.toStdString();

    std::ostringstream str_num_act, str_num_pool;
    str_num_act << i_num_act;
    str_num_pool << i_num_pool;


    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    int year, month, day;

    year = data->startDate.year();
    month = data->startDate.month();
    day = data->startDate.day();

    output_file_data << "$(function changeText(){" << endl
        << "document.getElementById('project_name').innerHTML ='" << project_name << "';" << endl
        << "document.getElementById('stats_div').innerHTML ='"

        << "Number of activities: " << str_num_act.str() << "<br/>"
        << "Size of the resource pool: " << str_num_pool.str() << "<br/>"
        << "Heuristic parameter: " << data->heurPar << "<br/>"
        << "Number of Monte Carlo samples: " << data->MCsamples << "<br/>"
        << "Monte Carlo variant: " << data->MCvar << "<br/>"
        << "Starting date: " << year << "/" << month << "/" << day << "<br/>"

        << "';" << endl

        << "document.getElementById('notes_p').innerHTML ='" << notes << "';" << endl
        << "document.getElementById('footer').innerHTML ='" << "Created on " << timeinfo->tm_year+1900 << "/" << timeinfo->tm_mon+1 << "/" << timeinfo->tm_mday << "';" << endl
        << "document.getElementById('gantt').innerHTML = '";



    ////////////////////////////////////////////////
    int magic = 10;


    bool korekce = (i_num_act % magic != 0);
    for (int j = 0; j < i_num_act/magic + korekce; j++){
        output_file_data << "<div class=\"ganttg\" id=\"ganttChart" << j << "\"></div>";
    }
        output_file_data << "';});" << endl << endl

        << "var d1 =[";

    for (int i = data->mc_study_min; i < data->mc_study_max; i++)
        output_file_data << "[" << i << "," << data->mc_study_dats[i] << "],";
        output_file_data << "[" << data->mc_study_max << "," << data->mc_study_dats[data->mc_study_max] << "]];" << endl << endl;

        output_file_data << "$(function(){" << endl
            << "$.plot($(\"#placeholder\"),[{data:d1,color:\"rgb(254,225,0)\"}]);" << endl
            << "});" << endl << endl;

    list<node>::iterator it=data->tree->last_floor.begin();
    for (int i = 0; i< data->tree->last_floor.size(); i++){
        if ((*it).ongoing_activities.size()==0 && (*it).accessible_activities.size()==0)
            break;
            it++;
        }

    list<done_activity>::iterator it2 = (*it).done_activities.begin();


    int zacatek, konec;
    for (int j = 0; j < i_num_act/magic + korekce; j++){
        output_file_data << "var ganttData" << j << " = [" << endl;

    for (int i = 0; (i < magic) && (i+magic*j < i_num_act); i++) {
        if (i==0) zacatek = day + it2->start_time - 1;
        konec = day + it2->start_time + it2->pactivity->true_time - 2;
        output_file_data << "{id:" << i << ",itemName:\""
            << it2->pactivity->name << "\",series:[{seriesName:\""
            << it2->pactivity->sources << " res.\",start:new Date(20"
            << year << "," << month << "," << day + it2->start_time - 1 << "),end:new Date(20"
            << year << "," << month << "," << day + it2->start_time + it2->pactivity->true_time - 2<<")}]}";
        if(i<i_num_act-1) output_file_data << ",";
        output_file_data << endl;

        it2++;
    }

    output_file_data << "];" << endl << endl;

    output_file_data << "$(function(){" << endl
        << "$(\"#ganttChart" << j << "\").ganttView({" << endl
        << "data:ganttData" << j << "," << endl
        << "start:new Date(20" << year << "," << month << "," << zacatek << ")," << endl
        << "end:new Date(20" << year << "," << month << "," << konec << ")," << endl
        << "slideWidth:603" << endl
        << "});" << endl
        << "});" << endl << endl;
    }


    output_file_data.close();
    /*
    QString helpString;
    helpString += "projects\\" + data->strProjectName + "\\output.html";
    QDir dir;
    dir.setPath(helpString);
    dir.makeAbsolute();
    helpString = dir.path();
    */
    QString strPath1, strPath2, strPath3;
    QStringList list1,list2;
    QFile file;
    QDir dir;
    //make folders
    strPath1 = dir.currentPath() + "\\projects\\" + data->strProjectName + "\\res";
    dir.mkpath(strPath1);
    //copy output.html
    strPath1 = dir.currentPath() + "\\res\\output.html";
    strPath2 = dir.currentPath() + "\\projects\\" + data->strProjectName + "\\output.html";
    file.copy(strPath1, strPath2);
    //move output.txt and data.js
    strPath3 = "\\output.txt";
    strPath1 = dir.currentPath() + strPath3;
    strPath2 = dir.currentPath() + "\\projects\\" + data->strProjectName + strPath3;
    file.copy(strPath1, strPath2);
    file.remove(strPath1);

    strPath3 = "\\data.js";
    strPath1 = dir.currentPath() + strPath3;
    strPath2 = dir.currentPath() + "\\projects\\" + data->strProjectName + strPath3;
    file.copy(strPath1, strPath2);
    file.remove(strPath1);
    //copy /res
    strPath3 = "\\date.js";
    strPath1 = dir.currentPath() + "\\res\\res" + strPath3;
    strPath2 = dir.currentPath() + "\\projects\\" + data->strProjectName + "\\res" + strPath3;
    file.copy(strPath1, strPath2);

    strPath3 = "\\excanvas.min.js";
    strPath1 = dir.currentPath() + "\\res\\res" + strPath3;
    strPath2 = dir.currentPath() + "\\projects\\" + data->strProjectName + "\\res" + strPath3;
    file.copy(strPath1, strPath2);

    strPath3 = "\\jquery.ganttView.css";
    strPath1 = dir.currentPath() + "\\res\\res" + strPath3;
    strPath2 = dir.currentPath() + "\\projects\\" + data->strProjectName + "\\res" + strPath3;
    file.copy(strPath1, strPath2);

    strPath3 = "\\jquery.ganttView.js";
    strPath1 = dir.currentPath() + "\\res\\res" + strPath3;
    strPath2 = dir.currentPath() + "\\projects\\" + data->strProjectName + "\\res" + strPath3;
    file.copy(strPath1, strPath2);

    strPath3 = "\\jquery-1.4.2.js";
    strPath1 = dir.currentPath() + "\\res\\res" + strPath3;
    strPath2 = dir.currentPath() + "\\projects\\" + data->strProjectName + "\\res" + strPath3;
    file.copy(strPath1, strPath2);

    strPath3 = "\\jquery-ui-1.8.4.css";
    strPath1 = dir.currentPath() + "\\res\\res" + strPath3;
    strPath2 = dir.currentPath() + "\\projects\\" + data->strProjectName + "\\res" + strPath3;
    file.copy(strPath1, strPath2);

    strPath3 = "\\jquery-ui-1.8.4.js";
    strPath1 = dir.currentPath() + "\\res\\res" + strPath3;
    strPath2 = dir.currentPath() + "\\projects\\" + data->strProjectName + "\\res" + strPath3;
    file.copy(strPath1, strPath2);

    strPath3 = "\\layout.css";
    strPath1 = dir.currentPath() + "\\res\\res" + strPath3;
    strPath2 = dir.currentPath() + "\\projects\\" + data->strProjectName + "\\res" + strPath3;
    file.copy(strPath1, strPath2);

    strPath3 = "\\logo200.png";
    strPath1 = dir.currentPath() + "\\res\\res" + strPath3;
    strPath2 = dir.currentPath() + "\\projects\\" + data->strProjectName + "\\res" + strPath3;
    file.copy(strPath1, strPath2);

    strPath3 = "\\jquery.flot.js";
    strPath1 = dir.currentPath() + "\\res\\res" + strPath3;
    strPath2 = dir.currentPath() + "\\projects\\" + data->strProjectName + "\\res" + strPath3;
    file.copy(strPath1, strPath2);
}

void RunClass::getProgress(int value)
{
    emit changeProgress(value);
}

#include "moc_runclass.cpp"
