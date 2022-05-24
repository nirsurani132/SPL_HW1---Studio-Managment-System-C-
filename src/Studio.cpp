#include <Studio.h>
#include <sstream>

Studio::Studio() : open(false) {

}

Studio::Studio(const std::string &configFilePath) : open(true), customerCounter(0) {
    std::ifstream inFile;
    inFile.open(configFilePath);
    std::string line;
    if (!inFile) {
        std::cout << "Unable to open file";
        exit(1); // terminate with error
    }
    findNextContent(inFile, line);
    int numOfTrainers = std::stoi(line);//Get numOfTrainers
    findNextContent(inFile, line); // Capacities ----> Line
    int j(0);
    for (int i = 0; i < numOfTrainers; i++) {
        char x = line[j];
        std::string capacity = "";
        while (x != ',' && j < int(line.size())) {
            capacity += x;
            j++;
            x = line[j];
        }
        trainers.push_back(new Trainer(std::stoi(capacity)));
        j++;
    }
//--------------------------------Add Actions-----------------------------
    j = 0; // restart the counting for the workouts
    findNextContent(inFile, line);
    while (line.size() != 0) {
        int pointer(0);
        std::string workOutName;
//---------------Fill Workout Name----------------
        while (line[pointer] != ',') {
            workOutName += line[pointer];
            pointer++;
        }
        pointer += 2; // there are 2 spaces between each property
//---------------Fill Workout Type----------------
        std::string w_type;
        while (line[pointer] != ',') {
            w_type += line[pointer];
            pointer++;
        }
        pointer += 2;
//---------------Fill Workout Price----------------
        std::string price;
        while (pointer < int(line.size())) {
            price += line[pointer];
            pointer++;
        }
//--------------Switch Types(enum)--------
        WorkoutType type;
        if (w_type == (std::string) "Anaerobic")
            type = WorkoutType(ANAEROBIC);
        else if (w_type == "Mixed")
            type = WorkoutType(MIXED);
        else
            type = WorkoutType(CARDIO);
//------------Adding New Workout to Workouts Vector---------
        workout_options.push_back(Workout(j, workOutName, std::stoi(price), type));
        j++;
        findNextContent(inFile, line);
    }
    inFile.close();
}

int Studio::generateNewCustomerId() {
    customerCounter++;
    return customerCounter - 1;
}

void Studio::setCustomerCounter (int newVal){
    customerCounter = newVal;
};
int Studio::getCustomerCounter () const{
    return customerCounter;
};

std::vector<Workout> &Studio::getWorkoutOptions() {
    return workout_options;
}

void Studio::findNextContent(std::ifstream &file, std::string &line) {
    getline(file, line);
    while (line[0] == '#' || line.size() == 0) {
        if (!getline(file, line)) {
            break;
        };
    }
};

void Studio::start() {
    bool isRunning = true;
    std::cout << "Studio is now open!" << std::endl;
    while (isRunning) {
        std::string input;
        std::cin >> input;
        if (input == "closeall") {
            callCloseAll();
            isRunning = false;
        } else if (input == "open") {
            callOpen();
        } else if (input == "order") {
            callOrder();
        } else if (input == "move") {
            callMove();
        } else if (input == "status") {
            callStatus();
        } else if (input == "workout_options") {
            callSWorkoutOptions();
        } else if (input == "close") {
            callClose();
        } else if (input == "log") {
            callLog();
        } else if (input == "backup") {
            callBackup();
        } else if (input == "restore") {
            callRestore();
        }
    }
}

void Studio::callOpen() {
    int trainerId;
    std::cin >> trainerId; // take the first word as the trainer id
    std::vector<Customer *> customerToAdd; // vector of customers to add vector
    std::string customer;
    std::string line;
    std::getline(std::cin, line); // get the rest of the line
    std::istringstream customersStream(line); // create a stream of the line
    while (customersStream) {
        customersStream >> customer; // get the next word
        if (!customersStream) // if the stream is empty
            break;
        int comaPos = customer.find(','); // find the position of the coma
        std::string name = customer.substr(0, comaPos); // get the name
        std::string letters = customer.substr(comaPos + 1); // sort into Types
        if (letters == "swt") {
            customerToAdd.push_back(new SweatyCustomer(name, generateNewCustomerId()));
        } else if (letters == "chp") {
            customerToAdd.push_back(new CheapCustomer(name, generateNewCustomerId()));
        } else if (letters == "mcl") {
            customerToAdd.push_back(new HeavyMuscleCustomer(name, generateNewCustomerId()));
        } else if (letters == "fbd") {
            customerToAdd.push_back(new FullBodyCustomer(name, generateNewCustomerId()));
        }
    }
    OpenTrainer *openAction = new OpenTrainer(trainerId, customerToAdd); // create the open trainer action
    openAction->act(*this); // run the action.
    actionsLog.push_back(openAction); // add the action to the log
}


Trainer *Studio::getTrainer(int id) {
    if (id > (int)trainers.size() || id < 0)
        return nullptr;
    return trainers[id];
}

int Studio::getNumOfTrainers() const {
    return trainers.size();
}


void Studio::callOrder() {
    int trainerId;
    std::cin >> trainerId;
    Order *orderAction = new Order(trainerId);
    orderAction->act(*this);
    actionsLog.push_back(orderAction); // add the action to the log
}

void Studio::callMove() {
    int sourceTrainerId, destTrainerId, customerId;
    std::cin >> sourceTrainerId;
    std::cin >> destTrainerId;
    std::cin >> customerId;
    MoveCustomer *moveAction = new MoveCustomer(sourceTrainerId, destTrainerId, customerId);
    moveAction->act(*this);
    actionsLog.push_back(moveAction);
}

void Studio::callStatus() {
    int trainerId;
    std::cin >> trainerId;
    PrintTrainerStatus *statusAct = new PrintTrainerStatus(trainerId);
    statusAct->act(*this);
    actionsLog.push_back(statusAct); // add the action to the log
}

void Studio::callClose() {
    int trainerId;
    std::cin >> trainerId;
    Close *closeAct = new Close(trainerId);
    closeAct->act(*this);
    actionsLog.push_back(closeAct);
}

void Studio::callCloseAll() {
    CloseAll closeAll = CloseAll();
    closeAll.act(*this);
}

void Studio::callSWorkoutOptions() {
    PrintWorkoutOptions *workoutOptions = new PrintWorkoutOptions();
    workoutOptions->act(*this);
    actionsLog.push_back(workoutOptions); // add the action to the log
}

void Studio::callLog() {
    PrintActionsLog log = PrintActionsLog();
    log.act(*this);
}

void Studio::callBackup() {
    BackupStudio backup = BackupStudio();
    backup.act(*this);
}

void Studio::callRestore() {
    RestoreStudio restore = RestoreStudio();
    restore.act(*this);
}
const std::vector<BaseAction *> &Studio::getActionsLog() const {
    return actionsLog;
}

Studio::~Studio() {
    for (int i = 0; i < (int)actionsLog.size(); i++) {
        delete actionsLog[i];
    }
    for (int i = 0; i < (int)trainers.size(); i++) {
        delete trainers[i];
    }
    actionsLog.clear();
    trainers.clear();
}

void Studio::copyStudio(const Studio &other) {
    open = other.open;
    customerCounter = other.customerCounter;
    while(!trainers.empty()){
        delete trainers[0];
        trainers.erase(trainers.begin());
    }
    for (Trainer *currTrainer: other.trainers)
        trainers.push_back(new Trainer(*currTrainer));
    while(!actionsLog.empty()){
        delete actionsLog[0];
        actionsLog.erase(actionsLog.begin());
    }
    actionsLog.clear();
    for (BaseAction *action: other.actionsLog)
        actionsLog.push_back(action->clone(*this));
    workout_options.clear();
    for (Workout work: other.workout_options)
        workout_options.push_back(Workout(work.getId(), work.getName(), work.getPrice(), work.getType()));
}


// copy constructor
Studio::Studio(const Studio &other) : open(other.open), customerCounter(other.customerCounter) {
    copyStudio(other);
}


// copy assignment
Studio &Studio::operator=(const Studio &other) {
    if (&other != this) {
        copyStudio(other);
    }
    return *this;
}

// move constructor:
Studio::Studio(Studio &&other) {
    open = other.open;
    customerCounter = other.customerCounter;
    trainers = std::move(other.trainers);
    workout_options = std::move(other.workout_options);
    actionsLog = std::move(other.actionsLog);
}

// move assignment
Studio &Studio::operator=(Studio &&other) {
    if (this != &other) {
        open = other.open;
        customerCounter = other.customerCounter;
        trainers = std::move(other.trainers);
        workout_options = std::move(other.workout_options);
        actionsLog = std::move(other.actionsLog);
    }
    return *this;
}

