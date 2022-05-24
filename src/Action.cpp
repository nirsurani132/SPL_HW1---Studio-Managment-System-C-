#include "Action.h"
#include "Studio.h"
#include "Trainer.h"

extern Studio *backup;

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = COMPLETED;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

void BaseAction::error(std::string ErrorMsg) {
    errorMsg = ErrorMsg;
    status = ActionStatus::ERROR;
    std::cout << errorMsg << std::endl;
}

BaseAction::BaseAction() : status(ActionStatus::ERROR) {};

BaseAction::~BaseAction() = default;

// ---------------------------------OpenTrainer------------------------------

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList)
        : BaseAction(), trainerId(id), customers(customersList) {
    logCommend = " open " + std::to_string(trainerId);
    for (Customer *customer: customersList)
        logCommend += " " + customer->getName() + "," + customer->getLetters();
}

OpenTrainer::OpenTrainer(int id, std::string &logCommend)
        : BaseAction(), trainerId(id), logCommend(logCommend) {};

OpenTrainer *OpenTrainer::clone(Studio &studio) {
    return new OpenTrainer(trainerId, logCommend);
}

void OpenTrainer::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId);
    if (trainer == nullptr || trainer->isOpen()) { // trainer not exist or already open
        error("Workout session does not exist or is already open.");
        failedToAct(studio, 0);
        return;
    }
    if ((int) customers.size() == 0) { // no customers
        error("Cannot open trainers without any customers");
        return;
    }
    int numCustomersInserted = 0;
    while(trainer->getCapacity() >= numCustomersInserted + 1 && numCustomersInserted<(int)customers.size()) {
        trainer ->addCustomer(customers[numCustomersInserted]);
        numCustomersInserted++;
    }
    if(numCustomersInserted  < int(customers.size())){
        failedToAct(studio , numCustomersInserted);
    }
    trainer->openTrainer(); // open trainer
    complete();
}

void OpenTrainer::failedToAct(Studio &studio , int numCustomersInserted) {
    studio.setCustomerCounter(studio.getCustomerCounter() - customers.size() + numCustomersInserted);
    for (int i = customers.size() - 1; i >= numCustomersInserted; i--) { // remove customers from studio
        delete customers[i]; // delete customer
    }
    customers.clear(); // clear customers
}

std::string OpenTrainer::toString() const {
    std::string ans = logCommend;
    if (getStatus() == ActionStatus::COMPLETED) // if action completed
        ans += "  Completed";
    else
        ans += " Error: " + getErrorMsg();
    return ans;
}

Order::Order(int id) : BaseAction(), trainerId(id) {}

void Order::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId); // get the trainer
    if (trainer == nullptr || !trainer->isOpen()) { // if the trainer is not open
        error("Workout session does not exist or is not open.");
        return;
    }
    std::vector<Customer *> &customers = trainer->getCustomers(); // get customers from trainer
    for (int i = 0; i < (int) customers.size(); i++) { // for each customer
        std::vector<int> options = customers[i]->order(studio.getWorkoutOptions());
        if (!options.empty()) // if customer ordered
            trainer->order(customers[i]->getId(), options, studio.getWorkoutOptions());
        else
            trainer->removeCustomer(customers[i]->getId()); // remove customer from trainer
    }
    complete();
}

std::string Order::toString() const {
    std::string ans = "order " + std::to_string(trainerId);
    if (getStatus() == ActionStatus::COMPLETED)
        ans += "  Completed";
    else
        ans += " Error: " + getErrorMsg();
    return ans;
}

Order *Order::clone(Studio &studio) {
    return new Order(*this);
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId)
        : BaseAction(), srcTrainer(src), dstTrainer(dst), id(customerId) {};

MoveCustomer *MoveCustomer::clone(Studio &studio) {
    return new MoveCustomer(*this);
}

std::string MoveCustomer::toString() const {
    std::string ans =
            "move " + std::to_string(srcTrainer) + " " + std::to_string(dstTrainer) + " " + std::to_string(dstTrainer);
    if (getStatus() == ActionStatus::COMPLETED)
        ans += "  Completed";
    else
        ans += " Error: " + getErrorMsg();
    return ans;
}

void MoveCustomer::act(Studio &studio) {
    Trainer *sourceTrainer = studio.getTrainer(srcTrainer); // get source trainer
    Trainer *destinationTrainer = studio.getTrainer(dstTrainer); // get destination trainer
    if (sourceTrainer == nullptr || destinationTrainer == nullptr || srcTrainer == dstTrainer ||
        destinationTrainer->getCapacity() == (int) destinationTrainer->getCustomers().size() ||
        !destinationTrainer->isOpen()) { // if the trainers are the same
        error("Cannot Move Customer");
        return;
    }
    Customer *customer = sourceTrainer->getCustomer(id); // get customer
    destinationTrainer->addCustomer(customer); // add customer to destination trainer
    std::vector<OrderPair> &orders = sourceTrainer->getOrders(); // get orders from source trainer
    for (OrderPair order: orders)  // for each order
        if (order.first == id) // if the order is for the customer
            destinationTrainer->addOrderToList(order); // add order to destination trainer
    sourceTrainer->deleteOrdersByCustomer(id); // delete orders from source trainer
    sourceTrainer->removeCustomer(id); // remove customer from source trainer
    complete(); // complete action
}

PrintTrainerStatus::PrintTrainerStatus(int id) : BaseAction(), trainerId(id) {}

PrintTrainerStatus *PrintTrainerStatus::clone(Studio &studio) {
    return new PrintTrainerStatus(*this);
}

std::string PrintTrainerStatus::toString() const {
    std::string ans = "status " + std::to_string(trainerId);
    if (getStatus() == ActionStatus::COMPLETED)
        ans += "  Completed";
    else
        ans += " Error: " + getErrorMsg();
    return ans;
}

void PrintTrainerStatus::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId); // get trainer
    std::cout << "Trainer " + std::to_string(trainerId) + " status: " + (trainer->isOpen() ? "open" : "close")
              << std::endl; // print status
    if (trainer->isOpen()) { // if the trainer is open
        std::vector<Customer *> &customers = trainer->getCustomers(); // get customers from trainer
        if (!customers.empty()) { // if the trainer has customers
            std::cout << "Customers:" << std::endl;
            for (Customer *customer: customers) {
                std::cout << customer->toString() << std::endl;
            }
            for (OrderPair orderPair: trainer->getOrders()) {
                std::cout << orderPair.second.getName() + " " + std::to_string(orderPair.second.getPrice()) + "NIS " +
                             std::to_string(orderPair.first) << std::endl;
            }
            std::cout << "Current Trainer's Salary: " + std::to_string(trainer->getSalary()) + "NIS " << std::endl;
        }
    }
    complete();
}

Close::Close(int id) : BaseAction(), trainerId(id) {};


Close *Close::clone(Studio &studio) {
    return new Close(*this);
}


void Close::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId);
    if (trainer == nullptr || !trainer->isOpen()) {
        error("Trainer does not exist or is not open");
        return;
    }
    std::cout << "Trainer " + std::to_string(trainerId) + " closed. Salary " + std::to_string(trainer->getSalary()) +
                 "NIS" << std::endl;
    trainer->closeTrainer();
    complete();
}

std::string Close::toString() const {
    std::string ans = "close " + std::to_string(trainerId);
    if (getStatus() == ActionStatus::COMPLETED)
        ans += "  Completed";
    else
        ans += " Error: " + getErrorMsg();
    return ans;
}


CloseAll::CloseAll() : BaseAction() {};

CloseAll *CloseAll::clone(Studio &studio) {
    return new CloseAll(*this);
}

void CloseAll::act(Studio &studio) {
    for (int i = 0; i < studio.getNumOfTrainers(); i++) {
        if (studio.getTrainer(i)->isOpen()) {
            Close toClose = Close(i);
            toClose.act(studio);
        }
    }
    complete();
}

std::string CloseAll::toString() const {
    std::string ans = "CloseAll";
    if (getStatus() == ActionStatus::COMPLETED)
        ans += "  Completed";
    else
        ans += " Error: " + getErrorMsg();
    return ans;
}

PrintWorkoutOptions::PrintWorkoutOptions() : BaseAction() {};

PrintWorkoutOptions *PrintWorkoutOptions::clone(Studio &studio) {
    return new PrintWorkoutOptions(*this);
}

void PrintWorkoutOptions::act(Studio &studio) {
    std::vector<Workout> &ourWorkouts = studio.getWorkoutOptions();
    for (Workout currWorkout: ourWorkouts)
        std::cout << currWorkout << std::endl;
    complete();
}


std::string PrintWorkoutOptions::toString() const {
    std::string ans = "workout_options";
    if (getStatus() == ActionStatus::COMPLETED)
        ans += "  Completed";
    else
        ans += " Error: " + getErrorMsg();
    return ans;
}

PrintActionsLog::PrintActionsLog() : BaseAction() {};

void PrintActionsLog::act(Studio &studio) {
    const std::vector<BaseAction *> &actionLog = studio.getActionsLog();
    for (BaseAction *action: actionLog)
        std::cout << action->toString() << std::endl;
    complete();
}

PrintActionsLog *PrintActionsLog::clone(Studio &studio) {
    return new PrintActionsLog(*this);
}


std::string PrintActionsLog::toString() const {
    return "";
}

OpenTrainer::~OpenTrainer() = default;

Order::~Order() = default;

MoveCustomer::~MoveCustomer() = default;

Close::~Close() = default;

CloseAll::~CloseAll() = default;

PrintWorkoutOptions::~PrintWorkoutOptions() = default;

PrintTrainerStatus::~PrintTrainerStatus() = default;

PrintActionsLog::~PrintActionsLog() = default;

BackupStudio::BackupStudio() : BaseAction() {};

BackupStudio *BackupStudio::clone(Studio &studio) {
    return new BackupStudio(*this);
}

void BackupStudio::act(Studio &studio) {
    delete backup;
    backup = new Studio(studio);
    complete();
}

std::string BackupStudio::toString() const {
    return "Backup";
}

BackupStudio::~BackupStudio() = default;

RestoreStudio::RestoreStudio() : BaseAction() {};

void RestoreStudio::act(Studio &studio) {
    if (backup == nullptr)
        error("No backup available");
    else {
        studio = *backup;
        complete();
    }
}

RestoreStudio::~RestoreStudio() = default;

std::string RestoreStudio::toString() const {
    std::string ans = "restore";
    if (getStatus() == ActionStatus::COMPLETED)
        ans += "  Completed";
    else
        ans += " Error: " + getErrorMsg();
    return ans;
}

RestoreStudio *RestoreStudio::clone(Studio &studio) {
    return new RestoreStudio();
}

