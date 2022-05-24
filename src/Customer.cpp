#include <limits>
#include <algorithm>
#include "Customer.h"
#include "Workout.h"

Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id) {};

int Customer::getId() const {
    return id;
}

std::string Customer::getName() const {
    return name;
}


Customer::~Customer() = default;


SweatyCustomer::SweatyCustomer(std::string c_name, int c_id) : Customer(c_name, c_id) {};

std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> workout_ids;
    for (int i = 0; i < (int)workout_options.size(); i++) {
        if (workout_options[i].getType() == WorkoutType::CARDIO) {
            workout_ids.push_back(workout_options[i].getId());
        }
    }
    return workout_ids;
}

std::string SweatyCustomer::toString() const {
    return  std::to_string(getId()) + " " + getName();
}

std::string SweatyCustomer::getLetters() const {
    return "swt";
}

SweatyCustomer::~SweatyCustomer(){};

SweatyCustomer *SweatyCustomer::clone() {
    return new SweatyCustomer(getName(),getId());
}


CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id) {};

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> workout_ids;
    if (workout_options.empty()) {
        return workout_ids;
    }
    int minId(0);
    int minPrice = workout_options[0].getPrice();
    for (int i = 1; i <(int)workout_options.size(); i++) {
        int currPrice(workout_options[i].getPrice());
        if (minPrice > currPrice) {
            minId = i;
            minPrice = currPrice;
        }
    }
    workout_ids.push_back(minId);
    return workout_ids;
}

std::string CheapCustomer::toString() const {
    return  std::to_string(getId()) + " " + getName();
}

std::string CheapCustomer::getLetters() const {
    return "chp";
}

CheapCustomer::~CheapCustomer(){};

CheapCustomer *CheapCustomer::clone() {
    return new CheapCustomer(getName(),getId());
}

HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) : Customer(name, id) {};

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> workout_ids;
    std::vector<std::pair<int, int>> workouts;
    for (int i = 0; i < (int)workout_options.size(); i++) {
        if (workout_options[i].getType() == WorkoutType::ANAEROBIC) {
            workouts.push_back(std::pair<int, int>(workout_options[i].getId(),workout_options[i].getPrice()));
        }
    }
    std::sort(workouts.begin(), workouts.end(), compareWorkoutPricePairs);
    for (std::pair<int,int> workoutPair : workouts)
        workout_ids.push_back(workoutPair.first);
    return workout_ids;
}

std::string HeavyMuscleCustomer::toString() const {
    return  std::to_string(getId()) + " " + getName();
}

std::string HeavyMuscleCustomer::getLetters() const {
    return "mcl";
}

HeavyMuscleCustomer::~HeavyMuscleCustomer(){};

HeavyMuscleCustomer *HeavyMuscleCustomer::clone() {
    return new HeavyMuscleCustomer(getName(),getId());
}

FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(name, id) {};

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> workout_ids; // empty vector
    int cardioId = -1; // -1 means no cardio workout
    int anaerobicId = -1; // -1 means no anaerobic workout
    int mixedId = -1; // -1 means no mixed workout
    int cardioMinPrice = std::numeric_limits<int>::max(); // max int
    int mixedMaxPrice = std::numeric_limits<int>::min(); // min int
    int anaerobicMinPrice = std::numeric_limits<int>::max(); // max int
    for (int i = 0; i < (int) workout_options.size(); i++) { // find the best workout
        WorkoutType type = workout_options[i].getType(); // get the type of the workout
        if(type == WorkoutType::CARDIO) { // if cardio workout
            if (workout_options[i].getPrice()<cardioMinPrice) { // if the price is better than the current best
                cardioMinPrice = workout_options[i].getPrice(); // update the best price
                cardioId = i; // update the best workout id
            }
        } else if (type == WorkoutType::ANAEROBIC) { // if anaerobic workout
            if (workout_options[i].getPrice()<anaerobicMinPrice) { // if the price is better than the current best
                anaerobicMinPrice = workout_options[i].getPrice(); // update the best price
                anaerobicId = i; // update the best workout id
            }
        } else if (type == WorkoutType::MIXED) { // if mixed workout
            if(workout_options[i].getPrice()>mixedMaxPrice) { // if the price is higher than the current higher
                mixedMaxPrice = workout_options[i].getPrice(); // update the highest price
                mixedId = i; // update the highest workout id
            }
        }
    }
    if(cardioId != -1) // if cardio workout is found
        workout_ids.push_back(cardioId); // add it to the vector
    if(mixedId != -1) //    if mixed workout is found
        workout_ids.push_back(mixedId); // add it to the vector
    if(anaerobicId != -1) // if anaerobic workout is found
        workout_ids.push_back(anaerobicId); // add it to the vector
    return workout_ids; // return the vector
}

std::string FullBodyCustomer::toString() const {
    return  std::to_string(getId()) + " " + getName();
}

std::string FullBodyCustomer::getLetters() const {
    return "fbd";
}

FullBodyCustomer::~FullBodyCustomer(){};

FullBodyCustomer *FullBodyCustomer::clone() {
    return new FullBodyCustomer(getName(),getId());
}