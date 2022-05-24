#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Action.h"
#include <fstream> // handle files
#include "Workout.h"
#include "Trainer.h"


class Studio{		
public:
	Studio();
    ~Studio();
    Studio(const std::string &configFilePath);
    void copyStudio(const Studio &other);
    Studio(const Studio& other);
    Studio(Studio && other);
    Studio& operator=(const Studio &other);
    Studio& operator=(Studio &&other);
    void start();
    int generateNewCustomerId();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    void setCustomerCounter(int newVal);
    int getCustomerCounter() const;
private:
    bool open;
    int customerCounter;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    static void findNextContent(std::ifstream& file, std::string& line);
    void buildTrainersFromConfig(std::string& line);
    void callOpen();
    void callOrder();
    void callMove();
    void callStatus();
    void callSWorkoutOptions();
    void callClose();
    void callCloseAll();
    void callLog();
    void callBackup();
    void callRestore();

};

#endif