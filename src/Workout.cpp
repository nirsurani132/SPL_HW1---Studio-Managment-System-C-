#include "Workout.h"

Workout::Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type)
: id(w_id), name(w_name), price(w_price), type(w_type){}

int Workout::getId() const {return id;}
std::string Workout::getName() const {return name;}
int Workout::getPrice() const {return price;}
WorkoutType Workout::getType() const {return type;}

std::string Workout::workoutTypeToString(WorkoutType type){
    switch (type) {
        case WorkoutType::ANAEROBIC: return "Anaerobic";
        case WorkoutType::CARDIO:  return "Cardio";
        case WorkoutType::MIXED: return "Mixed";
        default: return "NO_TYPE";
    }
}

std::ostream& operator<<(std::ostream& os, const Workout& workout)
{
    os << workout.getName() + ", " + Workout::workoutTypeToString(workout.getType()) + ", " + std::to_string(workout.getPrice());
    return os;
}


bool compareWorkoutPricePairs (const std::pair<int, int> workout1, const std::pair<int, int> workout2){
    return (workout1.second > workout2.second);
}

//------------------- Copy Constructor ---------------
Workout::Workout(const Workout &other) = default;

//------------------- Move Constructor ---------------
Workout::Workout(Workout &&other) = default;

//------------------ Destructor ----------------------
Workout::~Workout() = default;